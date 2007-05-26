/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2005/2008 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttmpeg2videostream.cpp                                          */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 05/12/2005 */
/* MODIFIED: b. altendorf                                    DATE: 06/05/2005 */
/* MODIFIED: b. altendorf                                    DATE: 08/13/2005 */
/* MODIFIED: b. altendorf                                    DATE: 04/12/2007 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// -----------------------------------------------------------------------------
// TTMPEG2VIDEOSTREAM
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Overview
// -----------------------------------------------------------------------------
//
//                               +- TTAC3AudioStream
//                               |
//                               +- TTMpegAudioStream
//             +- TTAudioStream -|                    +- TTDTS14AudioStream
//             |                 +- TTDTSAudioStream -|
//             |                 |                    +- TTDTS16AudioStream
// TTAVStream -|                 +- TTPCMAudioStream
//             |
//             +- TTVideoStream -TTMpeg2VideoStream
//
// -----------------------------------------------------------------------------

/*----------------------------------------------------------------------------*/
/* This program is free software; you can redistribute it and/or modify it    */
/* under the terms of the GNU General Public License as published by the Free */
/* Software Foundation;                                                       */
/* either version 2 of the License, or (at your option) any later version.    */
/*                                                                            */
/* This program is distributed in the hope that it will be useful, but WITHOUT*/
/* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or      */
/* FITNESS FOR A PARTICULAR PURPOSE.                                          */
/* See the GNU General Public License for more details.                       */
/*                                                                            */
/* You should have received a copy of the GNU General Public License along    */
/* with this program; if not, write to the Free Software Foundation,          */
/* Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.              */
/*----------------------------------------------------------------------------*/

#define TTMPEG2VIDEOSTREAM_DEBUG
#define ENCODE_INFO

#include "ttmpeg2videostream.h"

#include "../data/ttcutlistdata.h"

#include <QDir>
#include <QStack>

const char c_name[] = "MPEG2STREAM";

/*! ////////////////////////////////////////////////////////////////////////////
 * Default constructor
 */
TTMpeg2VideoStream::TTMpeg2VideoStream()
: TTVideoStream()
{
  log = TTMessageLogger::getInstance();

  stream_type   = TTAVTypes::mpeg2_demuxed_video;
  stream_open   = false;
  stream_mode   = 0;
  mpeg2_stream  = NULL;
  header_list   = NULL;
  index_list    = NULL;
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Constructor with QFileInfo
 */
TTMpeg2VideoStream::TTMpeg2VideoStream( const QFileInfo &f_info )
: TTVideoStream( f_info )
{
  log = TTMessageLogger::getInstance();

  stream_type   = TTAVTypes::mpeg2_demuxed_video;
  mpeg2_stream  = NULL;
  header_list   = NULL;
  index_list    = NULL;
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Create a shared copy from an existing videostream
 */
void TTMpeg2VideoStream::makeSharedCopy( TTMpeg2VideoStream* v_stream )
{
  current_index = 0;
  stream_info   = v_stream->stream_info;
  stream_type   = v_stream->stream_type;
  header_list   = v_stream->header_list;
  index_list    = v_stream->index_list;
  num_index     = v_stream->num_index;
  num_header    = v_stream->num_header;
  frame_rate    = v_stream->frame_rate;
  bit_rate      = v_stream->bit_rate;

  stream_parsed = true;
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Create the list with mpeg2 header informations
 */
int TTMpeg2VideoStream::createHeaderList()
{
  QString   idd_stream_name;
  QFileInfo idd_stream_info;
  bool      header_list_exists = false;

  log->debugMsg(c_name, "Create header list for stream: %s", stream_info->filePath().toLatin1().constData());

  // open the mpeg2 stream
  if ( !openStream() )
  {
    log->errorMsg(c_name, "Could not open video stream: %s", stream_info->filePath().toLatin1().constData());

    num_header = 0;
    return num_header;
  }

  // new mpeg2-video-header-list
  header_list = new TTVideoHeaderList( 2000 );

  // ---------------------------------------------------------------------------
  // Mpeg2Schnitt idd-file; Read header list from idd file
  // ---------------------------------------------------------------------------
  if ( TTCut::readVideoIDD )
  {
    idd_stream_name = ttChangeFileExt( stream_info->filePath(), "idd" );

    log->infoMsg(c_name, "look for idd-file: %s", TTCut::toAscii(idd_stream_name));

    // check for Mpeg2Schnitt idd-stream in current directory
    idd_stream_info.setFile( idd_stream_name );

    if ( idd_stream_info.exists() )
    {
      idd_stream         = new TTFileBuffer( TTCut::toAscii(idd_stream_name), fm_open_read );
      header_list_exists = createHeaderListFromIdd();

      // we can delete the idd-stream
      idd_stream->closeFile();
      delete idd_stream;
    }
  }

  // ---------------------------------------------------------------------------
  // Read the header from mpeg2 stream 
  // ---------------------------------------------------------------------------
  if ( !header_list_exists )
  {
    header_list_exists = createHeaderListFromMpeg2();
  }

  // mpeg2 header list successfully created ???
  if (!header_list_exists )
  {
    log->errorMsg(c_name, "Could not create header list!");

    delete header_list;
    header_list = NULL;
    num_header   = 0;

    return num_header;
  }

  num_header = header_list->count();
 
  log->infoMsg(c_name, "Header list created: %d", num_header);

  return num_header;
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Create the mpeg2 video stream picture index list
 */
int TTMpeg2VideoStream::createIndexList()
{
  int               u_result         = 0;
  long              base_number      = 0;
  long              current_picture_number = 0;
  int               index            = 0;
  uint8_t           start_code       = 0xFF;
  int               sequence_index   = 0;
  long              gop_number       = 0;
  TTSequenceHeader* current_sequence = NULL;
  TTGOPHeader*      current_gop      = NULL;
  TTPicturesHeader* current_picture  = NULL;

#if defined(__TTMPEG2)
  TTVideoIndex*     prev_video_index;
  TTVideoIndex*     cur_video_index;
  long              size_index = -1;
  off64_t           frame_size = 0;
  long              i;
#endif

  index_list  = new TTVideoIndexList( 2000 );

  while ( index < header_list->count() )
  {
    start_code = header_list->at(index)->headerType();

    switch ( start_code )
    {
      case TTMpeg2VideoHeader::sequence_start_code:
        current_sequence = (TTSequenceHeader*)header_list->at(index);
        current_sequence->pictures_in_sequence = 0;
        sequence_index = index;

#ifdef __TTMPEG2
        if ( size_index >= 0 )
        {
          frame_size = current_sequence->headerOffset()-frame_size;

          prev_video_index = index_list->videoIndexAt( size_index );
          prev_video_index->picture_size = frame_size;

          frame_size = current_sequence->headerOffset();

          // I-Frame-size = sequence_size+gop_size+picture_size
          size_index = -2;
        }
#endif
        break;

      case TTMpeg2VideoHeader::group_start_code:
        current_gop = (TTGOPHeader*)header_list->at(index);
        base_number = current_picture_number;
        current_gop->pictures_in_gop = 0;
        gop_number++;

#ifdef __TTMPEG2
        // I-Frame-size = picture_size (uncomment size_index = -1 )
        frame_size = current_gop->headerOffset();
        //qDebug("%sframe size: %ld",c_name,frame_size);
        size_index = -1;    
#endif
        break;

      case TTMpeg2VideoHeader::picture_start_code:
        current_picture = (TTPicturesHeader*)header_list->at(index);
        if ( current_picture != NULL )
        {
          TTVideoIndex* video_index = new TTVideoIndex();

          video_index->setHeaderOffset( current_picture->headerOffset() );
          video_index->setHeaderType( current_picture->headerType() );
          video_index->display_order       = base_number+current_picture->temporal_reference;
          video_index->stream_order        = current_picture_number;
          video_index->header_list_index   = index;
          video_index->picture_coding_type = current_picture->picture_coding_type;
          video_index->sequence_index      = sequence_index;
          video_index->gop_number          = gop_number-1;

#ifdef __TTMPEG2
          if ( size_index == -1 )
          {
            frame_size = current_picture->headerOffset();
            size_index = current_picture_number;
          }
          else if ( size_index == -2 )
          {
            size_index = current_picture_number;   
          }
          else
          {
            frame_size = current_picture->headerOffset()-frame_size;
            //qDebug("Index_pic: %ld / size: %ld",size_index,frame_size);

            prev_video_index = index_list->videoIndexAt( size_index );
            prev_video_index->picture_size = frame_size;

            frame_size = current_picture->headerOffset();
            size_index = current_picture_number;
          }
#endif

          switch ( current_picture->picture_coding_type )
          {
            case 1:
              index_list->numIFramesPlus();
              break;
            case 2:
              index_list->numPFramesPlus();
              break;
            case 3:
              index_list->numBFramesPlus();
              break;
          }

          index_list->add( video_index );

          current_picture_number++;

          if ( current_sequence != NULL )
            current_sequence->pictures_in_sequence++;
          if ( current_gop != NULL )
            current_gop->pictures_in_gop++;
        }
        break;
    }
    index++;
  }

  num_index = index_list->count();

#if defined(TTMPEG2VIDEOSTREAM_DEBUG)
  log->infoMsg(c_name, "index list created: %d/%d", num_index, index_list->size());
#endif

#ifdef __TTMPEG2
  index_list->stream_order_list = new int[num_index];
  index_list->setDisplayOrder();

  for ( i = 0; i < num_index; i++ )
  {
    cur_video_index                  = index_list->videoIndexAt( i );
    index_list->stream_order_list[i] = cur_video_index->display_order;
    //qDebug("%sstream-index:%d",c_name,index_list->stream_order_list[i]);
  }
  index_list->setStreamOrder();
#endif

  u_result = num_index;
  return u_result;
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Returns the sequence header at current index (picture) position
 */
TTSequenceHeader* TTMpeg2VideoStream::currentSequenceHeader()
{
  return sequenceHeaderAt(currentIndex());
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Returns the sequence header at given index (picture index list) position
 */
TTSequenceHeader* TTMpeg2VideoStream::sequenceHeaderAt(int index)
{
  int    header_index;
  uint8_t header_type;

  header_index = index_list->headerListIndex( index );
  header_type  = header_list->at( header_index )->headerType();

  while( header_type != TTMpeg2VideoHeader::sequence_start_code &&
         (int)header_index >= 0 )
  {    
    header_index--;
    header_type = header_list->at( header_index )->headerType();
  }

  if ( header_type == TTMpeg2VideoHeader::sequence_start_code )
  {
    //qDebug( "%sfound sequence header at: %d",c_name,header_index );
    return header_list->sequenceHeaderAt( header_index );
  }
  else
  {
    log->warningMsg(c_name, "NO sequence header found for index: %d", index);
    return NULL;
  }
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Returns the GOP header at current index (picture) position
 */
TTGOPHeader* TTMpeg2VideoStream::currentGOPHeader()
{
  return GOPHeaderAt(currentIndex());
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Returns the GOP header at the given index (picture index list) position
 */
TTGOPHeader* TTMpeg2VideoStream::GOPHeaderAt(int index)
{
  int     header_index;
  uint8_t header_type;

  header_index = index_list->headerListIndex( index );
  header_type  = header_list->at( header_index )->headerType();

  while( header_type != TTMpeg2VideoHeader::group_start_code &&
         (int)header_index >= 0 )
  {
    header_index--;
    header_type = header_list->at( header_index )->headerType();
  }

  if ( header_type == TTMpeg2VideoHeader::group_start_code )
  {
    //qDebug( "%sfound group header at: %d",c_name,header_index );
    return header_list->gopHeaderAt( header_index );
  }
  else
  {
    log->warningMsg(c_name, "No GOP header found for index: %d", index);
    return NULL;
  }
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Returns the picture header for current index (picture) position
 */
TTPicturesHeader* TTMpeg2VideoStream::currentPictureHeader() 
{
  return pictureHeaderAt(currentIndex());
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Returns the picture header at current index (picture) position
 */
TTPicturesHeader* TTMpeg2VideoStream::pictureHeaderAt(int index)
{
  int     header_index;
  uint8_t header_type;

  header_index = index_list->headerListIndex( index );
  header_type  = header_list->at( header_index )->headerType();

  while( header_type != TTMpeg2VideoHeader::picture_start_code &&
         (int)header_index >= 0 )
  {
    header_index--;
    header_type = header_list->at( header_index )->headerType();
  }

  if ( header_type == TTMpeg2VideoHeader::picture_start_code )
  {
    //qDebug( "%sfound picture header at: %d",c_name,header_index );
    return header_list->pictureHeaderAt( header_index );
  }
  else
  {
    log->warningMsg(c_name, "No picture header found for index: %d", index);
    return NULL;
  }
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Returns the previous header object to given current header object
 */
TTVideoHeader* TTMpeg2VideoStream::getPrevMpeg2Object( TTVideoHeader* current )
{
  int found_index;

  found_index = header_list->indexOf( (TTAVHeader*)current );

  if ( found_index-1 < 0 )
    return (TTVideoHeader*)NULL;
  else
    return (TTVideoHeader*)header_list->headerAt( (int)found_index-1 );
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Returns the next header object to given current header object
 */
TTVideoHeader* TTMpeg2VideoStream::getNextMpeg2Object( TTVideoHeader* current )
{
  int found_index;

  found_index = header_list->indexOf( (TTAVHeader*)current );

  if ( found_index < 0 || found_index+1 >= header_list->count() )
    return (TTVideoHeader*)NULL;
  else
    return (TTVideoHeader*)header_list->headerAt( (int)found_index+1 );
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Open the mpeg2 video stream
 */
bool TTMpeg2VideoStream::openStream()
{
  bool b_result = false;

  // stream is already open; check for file name an file mode
  if ( stream_open )
  {
    b_result = true;
    return b_result;
  }

  // open the stream
  if ( ttAssigned(stream_info) )
  {
    // Create the file buffer read_only
    // toAscii() does not work here with german umlauts!
    mpeg2_stream = new TTFileBuffer( qPrintable(stream_info->filePath()), fm_open_read );

    if ( ttAssigned( mpeg2_stream ) )
    {
      stream_open   = true;
      stream_mode   = fm_open_read;
      stream_buffer = mpeg2_stream;
      b_result      = true;
    }
  }
  return b_result;
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Close the current mpeg2 video stream
 */
bool TTMpeg2VideoStream::closeStream()
{
  if (ttAssigned(mpeg2_stream))
    return false;

  if (!stream_open)
    return false;

  mpeg2_stream->closeFile();
  delete mpeg2_stream;

  stream_open   = false;
  stream_mode   = 0;
  stream_buffer = (TTFileBuffer*)NULL;
  mpeg2_stream  = (TTFileBuffer*)NULL;

  return true;
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Create the mpeg2 header-list from Mpeg2Schnitt idd-file
 */
bool TTMpeg2VideoStream::createHeaderListFromIdd()
{
  uint8_t  buffer4[4];
  long*    byte4;

  byte4            = (long*)&buffer4;
  idd_file_version = 0;

  if (!idd_stream->readArray(buffer4, 4))
  {
    log->errorMsg(c_name, "Could not determine idd version!");
    return false;
  }

  // idd index file ?
  if (buffer4[0] == int('i') &&
      buffer4[1] == int('d') &&
      buffer4[2] == int('d')    )
    idd_file_version = buffer4[3];

  log->infoMsg(c_name, "Read header list from idd file version: %02x", idd_file_version);

  readIDDHeader();

  if (header_list->count() > 0)
    return true;
  else
    return false;
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Create the mpeg2 header-list from mpeg2 stream
 */
bool TTMpeg2VideoStream::createHeaderListFromMpeg2()
{
  bool                b_cancel = false;
  uint8_t             header_type;
  TTMpeg2VideoHeader* new_header;

  log->infoMsg(c_name, "Create header list from mpeg2");

  if ( ttAssigned( progress_bar ) )
  {
    progress_bar->setActionText( "Create video header list." );
    progress_bar->setTotalSteps( mpeg2_stream->streamLength() );
    progress_bar->setProgress( 1 );
  }

  // create the video header list
  try
  {
    // go through the mpeg2 stream until EOF was reached
    while( !mpeg2_stream->streamEOF() && !b_cancel )
    {
      header_type = 0xFF;

      // search next header (start code)
      while (header_type != TTMpeg2VideoHeader::picture_start_code  &&
             header_type != TTMpeg2VideoHeader::sequence_start_code &&
             header_type != TTMpeg2VideoHeader::sequence_end_code   &&
             header_type != TTMpeg2VideoHeader::group_start_code    && 
             !mpeg2_stream->streamEOF() )
      {
        mpeg2_stream->nextStartCodeTS();
        mpeg2_stream->readByte( header_type );
      }

      // header found
      new_header = NULL;

      // create the appropriate header object
      switch ( header_type )
      {
        case TTMpeg2VideoHeader::sequence_start_code:
          new_header = new TTSequenceHeader();
          header_list->numSequencePlus();
          break;
        case TTMpeg2VideoHeader::picture_start_code:
          new_header = new TTPicturesHeader();
          header_list->numPicturePlus();
          break;
        case TTMpeg2VideoHeader::group_start_code:
          new_header = new TTGOPHeader();
          header_list->numGopPlus();
          break;
        case TTMpeg2VideoHeader::sequence_end_code:
          header_list->numSequenceEndPlus();
          break;
      }

      if( TTCut::logVideoIndexInfo )
        log->infoMsg(c_name, "header type: %02x - %lld", header_type, mpeg2_stream->currentOffset() );

      // insert the new header object into the header list
      if ( new_header != NULL )
      {
        new_header->readHeader( mpeg2_stream );
        header_list->add( new_header );
      }

      if ( ttAssigned(progress_bar) )
        b_cancel = progress_bar->setProgress( mpeg2_stream->currentOffset() );
    }

    if ( ttAssigned(progress_bar) )
      progress_bar->setComplete();
  }
  catch (...)
  {
  }  

  log->infoMsg(c_name, "Header list created: %d", header_list->count());

  // write an idd file with the header information
  if ( header_list->count() > 0 && TTCut::createVideoIDD ) 
    writeIDDFile();

  if (b_cancel)
    return false;
  else
    return true;
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Write MPEG2Schnit idd file
 */
void TTMpeg2VideoStream::writeIDDFile( )
{
  QString           idd_stream_name;
  QFileInfo         idd_stream_info;
  TTPicturesHeader* current_picture;
  int               index = 0;
  uint8_t           header_type;
  off64_t           offset;
  int16_t           temp_ref;
  uint8_t           coding_type;
  uint8_t           buffer[8];

  // create Mpeg2Schnitt idd-stream name
  idd_stream_name = ttChangeFileExt( stream_info->filePath(), "idd" );

  // check for Mpeg2Schnitt idd-stream in current directory
  idd_stream_info.setFile( idd_stream_name );

  if ( idd_stream_info.exists() )
  {
    // we can delete the idd-stream
    QFile idd_file( idd_stream_info.filePath() );
    idd_file.remove();
  }

  if ( ttAssigned( progress_bar ) )
  {
    progress_bar->setActionText( "create MPEG2Schnitt idd-file" );
    progress_bar->resetProgress();
    progress_bar->setTotalSteps( header_list->count(), 100 );
  }

  // create new idd-stream
  idd_stream = new TTFileBuffer(TTCut::toAscii(idd_stream_name), fm_open_write );

  buffer[0] = int('i'); // Puffer mit 'idd' fuellen
  buffer[1] = int('d');
  buffer[2] = int('d');
  buffer[3] = 2;        // Version der Indexdatei

  // Puffer an den Anfang der Datei schreiben
  idd_stream->directWrite(buffer, 4);    

  while ( index < header_list->count() )
  {
    header_type = header_list->at(index)->headerType();
    idd_stream->directWriteUInt8( header_type );  // 1 Byte

    offset      = header_list->at(index)->headerOffset();
    idd_stream->directWriteUInt64( offset );      // 8 Byte

    // picture header
    if ( header_type == TTMpeg2VideoHeader::picture_start_code )
    {

      current_picture = (TTPicturesHeader*)header_list->at(index);
      if ( current_picture != NULL )
      {
        temp_ref = current_picture->temporal_reference;
        idd_stream->directWriteUInt16( temp_ref );     // 2 Byte

        coding_type = current_picture->picture_coding_type;
        idd_stream->directWriteUInt8( coding_type );  // 1 Byte
      }
    }
    index++;

    if ( ttAssigned(progress_bar) )
      progress_bar->setProgress( index );
  }

  if( ttAssigned(progress_bar) )
    progress_bar->setComplete();

  // write sequence end header type
  buffer[0] = 0xB7;
  idd_stream->directWrite(buffer, 1);  // 1 Byte

  // write last file offset
  offset      = header_list->at(index-1)->headerOffset();
  idd_stream->directWriteUInt64( offset );  // 8 Byte
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Read MPEG2Schnitt *.idd file (also created by projectX)
 */
void TTMpeg2VideoStream::readIDDHeader( )
{
  uint8_t             header_type;
  uint32_t            offset_32;
  uint64_t            offset;
  TTMpeg2VideoHeader* new_header = NULL;

  // TODO: check idd-file against current mpeg2-stream

  if ( ttAssigned( progress_bar ) )
  {
    progress_bar->setActionText( "read MPEG2Schnitt idd-file" );
    progress_bar->setTotalSteps( idd_stream->streamLength() );
    progress_bar->setProgress(1);
  }

  try
  {
    while( !idd_stream->streamEOF() )
    {
      // read header type
      idd_stream->readUInt8( header_type );

      if ( idd_file_version < 1 )
      {
        idd_stream->readUInt32( offset_32 );   // Adresse 32 Bit
        offset = offset_32;
      }
      else
      {
        idd_stream->readUInt64( offset );      // Adresse 64 Bit
      }

      // create appropriate header object
      switch ( header_type )
      {
        case TTMpeg2VideoHeader::sequence_start_code:
          new_header = new TTSequenceHeader();
          header_list->numSequencePlus();
          break;
        case TTMpeg2VideoHeader::picture_start_code:
          new_header = new TTPicturesHeader();
          header_list->numPicturePlus();
          // skip information about frame coding type and temporal reference
          idd_stream->seekRelative( 3 );
          break;
        case TTMpeg2VideoHeader::group_start_code:
          new_header = new TTGOPHeader();
          header_list->numGopPlus();
          break;
        case TTMpeg2VideoHeader::sequence_end_code:
          new_header = new TTSequenceEndHeader();
          header_list->numSequenceEndPlus();
          break;
      }

      // insert the new header object into the header list
      if ( new_header != NULL )
      {
        new_header->readHeader( mpeg2_stream, offset );
        header_list->add( new_header );
      }

      if ( ttAssigned(progress_bar) )
      {
        progress_bar->setProgress( idd_stream->currentOffset() );

        if (progress_bar->isCanceled())
          return;
      }

      // old idd version; not tested (!)
      // full index-list
      if ( idd_file_version < 2 )  
      {
        switch ( header_type )
        {
          case 0xB3 : idd_stream->seekRelative(16); // skip information
                      break;
          case 0xB8 : idd_stream->seekRelative(8);
                      break;
          case 0x00 : idd_stream->seekRelative(23);
                      break;
        }
      }
    }
    if( ttAssigned(progress_bar) )
      progress_bar->setComplete();
  }
  catch (...)
  {
  }  
}

/*! ////////////////////////////////////////////////////////////////////////////
 * IsCutInPoint
 * Returns true, if the current index position (picture) is a valid cut-in point
 * In encoder-mode every picture position is a valid cut-in position, else the
 * cut-in is only allowed on I-frames
 */
bool TTMpeg2VideoStream::isCutInPoint( int pos )
{
  if ( pos < 0 )
    pos = currentIndex();

  if ( TTCut::encoderMode )
    return true;

  int frame_type = index_list->pictureCodingType( pos );

  if ( frame_type == 1 )
    return true;
  else
    return false;
}

/*! ////////////////////////////////////////////////////////////////////////////
 * IsCutOutPoint
 * Returns true, if the current index position (picture) is a valid cut-out 
 * point.
 * In encoder-mode every picture position is a valid cut-out position, else, the
 * cut-out is only valid on P- or I-frames
 */
bool TTMpeg2VideoStream::isCutOutPoint( int pos )
{
  if ( pos < 0 )
    pos = currentIndex();

  if ( TTCut::encoderMode )
    return true;

  int frame_type = index_list->pictureCodingType( pos );

  if ( frame_type == 1  || frame_type == 2 )
    return true;
  else
    return false;
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Cut the mpeg2 video stream according to the given cut list data
 */
void TTMpeg2VideoStream::cut( TTFileBuffer* cut_stream, TTCutListData* cut_list )
{
  TTCutParameter* cut_param = new TTCutParameter();
  int             start_pos;
  int             end_pos;
  QString         action_string;

#if defined (TTMPEG2VIDEOSTREAM_DEBUG)
  log->debugMsg(c_name, "--------------------------------------------------");
  log->debugMsg(c_name, "Cut stream   :  %s", TTCut::toAscii(mpeg2_stream->fileName()));
  log->debugMsg(c_name, "Target stream:  %s", TTCut::toAscii(cut_stream->fileName())); 
  log->debugMsg(c_name, "Cut list count: %d", cut_list->count());
#endif

  // cut each cut in cut list
  for (int i = 0; i < cut_list->count(); i++)
  {
    if ( i == 0 )
      cut_param->first_call = true;
    else
      cut_param->first_call = false;

    if ( i == cut_list->count()-1 )
      cut_param->last_call = true;

    start_pos = cut_list->cutInPosAt( i );
    end_pos   = cut_list->cutOutPosAt( i );

    if ( ttAssigned( progress_bar ) )
    {
      action_string.sprintf( "Video cut: %d/%d-%d",i+1,start_pos,end_pos );
      progress_bar->setActionText( action_string );
    }
    
    // cut the mpeg2 video stream according to start and end positon of
    // current cut list entry
    cut( cut_stream, start_pos, end_pos, cut_param );

    if (ttAssigned(progress_bar))
    {
      if (progress_bar->isCanceled())
        return;
    }
  }

#if defined (TTMPEG2VIDEOSTREAM_DEBUG)
  log->debugMsg(c_name, "--------------------------------------------------");
#endif

  delete cut_param;
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Cut Method
 * TTFileBuffer*   targetStream: The mpeg2 video target stream
 * int             startIndex:   Start index of current cut
 * int             endIndex:     End index of current cut
 * TTCutParameter: cutParams:    Cut parameter
 */
void TTMpeg2VideoStream::cut(TTFileBuffer* targetStream, 
                             int startIndex, int endIndex, 
                             TTCutParameter* cutParams)
{
  TTVideoHeader*    currentVideoHeader;
  TTVideoHeader*    startObjectVideoHeader;
  TTVideoHeader*    endObjectVideoHeader;
  int               startObjectHeaderPos;
  int               endObjectHeaderPos;
  int               currentIndexListPos;
  int               currentHeaderListPos;
  int               tempEndIndexPos;
  uint8_t           sequenceEndCode[4];

  sequenceEndCode[0] = 0x00;
  sequenceEndCode[1] = 0x00;
  sequenceEndCode[2] = 0x01;
  sequenceEndCode[3] = 0xb7;

#if defined (TTMPEG2VIDEOSTREAM_DEBUG)
  log->debugMsg(c_name, ">>> cut video stream");
  log->debugMsg(c_name, "--------------------------------------------------");
  log->debugMsg(c_name, "target stream:    %s", TTCut::toAscii(targetStream->fileName()));
  log->debugMsg(c_name, "startIndex:       %d  |  endIndex: %d", startIndex, endIndex);
  log->debugMsg(c_name, "Index list count: %d", index_list->count());
#endif

  // open the source stream
  openStream();

  // at the first call to cut create new header list
  if (cutParams->first_call &&
      cutParams->result_header_list == (TTVideoHeaderList*)NULL)
  {
    cutParams->result_header_list = new TTVideoHeaderList(1000);
  }

  // cut-in
  // --------------------------------------------------------------------------
  //
  // start object
  currentIndexListPos = startIndex;

  // get picture coding type from index list (display order!)
  // if coding type is not I-frame, we must encode
  if (index_list->pictureCodingType(currentIndexListPos) != 1)
  {
    tempEndIndexPos = currentIndexListPos+1;

    log->debugMsg(c_name, "Search next I-frame from pos: %d", tempEndIndexPos);

    // search the next I-frame in index list
    while(tempEndIndexPos < index_list->count()-1 && index_list->pictureCodingType(tempEndIndexPos) != 1)
      tempEndIndexPos++;

    log->debugMsg(c_name, "Found next I-frame at pos:    %d", tempEndIndexPos);

    // encode (without the founded I-frame!)
    encodePart(currentIndexListPos, tempEndIndexPos, cutParams, targetStream);

    // set current position in index list
    currentIndexListPos = tempEndIndexPos;
  }

  // set the current position in video header list
  // this must be the picture header from the current I-frame!
  // transfer must begin with sequence!
  currentHeaderListPos = index_list->headerListIndex(currentIndexListPos) - 2;

  if (currentHeaderListPos < 0)
  {
    log->errorMsg(c_name, "wrong header list pos: %d", currentHeaderListPos);
    currentHeaderListPos = 0;
  }

  // frame coding type at currentIndexListPos is I-frame
  // check if we have an sequence header at this position
  // remark:  SG|I| -> two header before I-frame must exists the sequence header!
  // so we must go back to header steps
  currentVideoHeader = (TTVideoHeader*)header_list->headerAt(currentHeaderListPos);

  // current header isn't a sequence header
  if (currentVideoHeader->headerType() != TTMpeg2VideoHeader::sequence_start_code)
  {
    log->warningMsg(c_name, "No sequence header found for GOP at index %d", currentIndexListPos);

    TTSequenceHeader* seqHeader = header_list->sequenceHeaderAt(currentHeaderListPos);
    
    if (!ttAssigned(seqHeader))
    {
      log->errorMsg(c_name, "Theire is no sequence header before header list pos %d",
                    currentHeaderListPos);
      log->errorMsg(c_name, "May be your video stream is borken?");
      log->errorMsg(c_name, "We must abort at this point, sorry.");

      //TODO: throw an exception at this point
    }

    TTVideoHeader* nextHeader = header_list->headerAt(header_list->headerIndex((TTVideoHeader*)seqHeader)+1);

    if (!ttAssigned(nextHeader))
    {
      log->errorMsg(c_name, "Theire is no following header after header list pos %d",
                    header_list->headerIndex((TTVideoHeader*)seqHeader));
      log->errorMsg(c_name, "May be your video stream is borken?");
      log->errorMsg(c_name, "We must abort at this point, sorry.");

      //TODO: trow an excpetion at this point
    }

    // inject the sequence header into the target stream
    copySegment(targetStream, seqHeader->headerOffset(), nextHeader->headerOffset()-1);

    if (cutParams->max_bitrate < seqHeader->bit_rate_value)
      cutParams->max_bitrate = seqHeader->bit_rate_value;

    //TODO: here we need a sequence copy constructor!
    TTSequenceHeader* newSequence = new TTSequenceHeader();
    newSequence->setHeaderOffset((off64_t)0);
    newSequence->aspect_ratio_information = seqHeader->aspect_ratio_information;

    cutParams->result_header_list->add(newSequence);

    // current header list position is still the positon of the I-frame
    // picture header
    // for the further processing we must go to the GOP header because the
    // sequence is already written! see above
    currentHeaderListPos += 1; 
  }

  // now we have hopefully a valid start object ;-)
  // if the start object isn't a picture header we must rewrite the following GOP!
  startObjectHeaderPos   = currentHeaderListPos;
  startObjectVideoHeader = header_list->headerAt(startObjectHeaderPos);

  log->debugMsg(c_name, "Start object pos: %d",     startObjectHeaderPos);
  log->debugMsg(c_name, "Start object:     0x%02x", startObjectVideoHeader->headerType());
 
  // cut-out
  // --------------------------------------------------------------------------
  //
  currentIndexListPos = endIndex;
  tempEndIndexPos     = endIndex;

  log->debugMsg(c_name, "Search P- or I-frame from pos; %d", currentIndexListPos);

  // cut-out position without reencoding mus P- or I-frame
  while (tempEndIndexPos > 0 && index_list->pictureCodingType(tempEndIndexPos) == 3)
    tempEndIndexPos--;

  log->debugMsg(c_name, "Found P- or I-frame at pos: %d", tempEndIndexPos);

 // now the current end index is at P- or I-frame
 currentIndexListPos  = tempEndIndexPos;
 currentHeaderListPos = index_list->headerListIndex(currentIndexListPos);

 // what's about the following B-frames ??? see old version of cut method
 // imho the proceeding theire is wrong
 // ...
 
 // now we have hopefully a valid end object ;-)
 endObjectHeaderPos   = currentHeaderListPos;
 endObjectVideoHeader = header_list->headerAt(endObjectHeaderPos);

 // transfer the stream part to target stream
 transferMpegObjects(targetStream,
                     startObjectVideoHeader, startObjectHeaderPos,
                     endObjectVideoHeader,   endObjectHeaderPos,
                     cutParams);

 if (ttAssigned(progress_bar))
 {
   if (progress_bar->isCanceled())
     return;
 }

 // something to encode: cut-out isn't lies on a B-frame?
 if (tempEndIndexPos != endIndex)
 {
   log->infoMsg(c_name, "encode part: startIndex: %d", tempEndIndexPos+1);
   log->infoMsg(c_name, "encode part: endIndex  : %d", endIndex);

   encodePart(tempEndIndexPos+1, endIndex, cutParams, targetStream);
 }

 // last call to cut: write sequence end code
 if (cutParams->last_call)
 {
   log->infoMsg(c_name, "last call to cut: cut is complete ?-)");
   log->infoMsg(c_name, "write the sequence end code");

   TTSequenceEndHeader* sequenceEndHeader = new TTSequenceEndHeader();
   sequenceEndHeader->setHeaderOffset(targetStream->currentOffset());

   cutParams->result_header_list->add(sequenceEndHeader);

   targetStream->directWrite(sequenceEndCode, 4);
 }

 // close the source stream
 closeStream();

#if defined (TTMPEG2VIDEOSTREAM_DEBUG)
 log->infoMsg(c_name, ">>> finished cut");
#endif
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Transfer the objects in intervall [startObject, endObject[ to the target
 * stream
 */
void TTMpeg2VideoStream::transferMpegObjects(TTFileBuffer* fs,
                                             TTVideoHeader* start_object, int start_object_index,
                                             TTVideoHeader* end_object,  __attribute__ ((unused))int end_object_index,
                                             TTCutParameter* cr )
{
  uint8_t*          buffer  = new uint8_t[65536];
  off64_t           count   = end_object->headerOffset()-start_object->headerOffset();
  off64_t           abs_pos = start_object->headerOffset();
  off64_t           process = 0;
  int               bytes_processed;
  TTVideoHeader*    current_object = start_object;
  TTPicturesHeader* current_picture = NULL;
  bool              close_next_GOP = true;      // remove B-frames
  short             temp_ref_delta = 0;         // delta for temporal reference if closed GOP
  const int         watermark  = 12;        // size of header type-code (12 byte)
  int               current_header_list_pos;
  int               current_index_pos;
  long              outer_loop_count = 0;
  long              inner_loop_count = 0;
  bool              object_processed;
  uint8_t*          time_code = new uint8_t[4];

#if defined(TTMPEG2VIDEOSTREAM_DEBUG)
  log->debugMsg(c_name, "Transfer to target stream");
  log->debugMsg(c_name, "-------------------------------------------");
  log->debugMsg(c_name, "StartHeaderPos: %d",     start_object_index);
  log->debugMsg(c_name, "EndHeaderPos  : %d",     end_object_index);
  log->debugMsg(c_name, "StartObject   : 0x%02x", start_object->headerType());
  log->debugMsg(c_name, "EndObject     : 0x%02x", end_object->headerType());
#endif

  QStack<TTBreakObject*>* break_objects = new QStack<TTBreakObject*>; 

  __attribute__ ((unused))TTVideoHeader* newMpeg2Object = (TTVideoHeader*)NULL;

  current_index_pos = start_object_index;

  QTime proc_time;
  proc_time.start();

  // source mpeg2-stream to start objects offset
  stream_buffer->seekAbsolute( start_object->headerOffset() );

  // initialize progress bar
  if ( ttAssigned( progress_bar ) )
  {
    progress_bar->resetProgress();
    progress_bar->setTotalSteps( count, 50 );
  }

  // ---------------------------------------------------------------------------
  // copy count bytes of data
  // ---------------------------------------------------------------------------
  while( count > 0 )
  {
    outer_loop_count++;

    // lets assume we can process all bytes
    if ( count < 65536 )
      bytes_processed = stream_buffer->readCount2( buffer, 0, count );
    else
      bytes_processed = stream_buffer->readCount2( buffer, 0, 65536 );

    // 0 bytes read from stream, must be an error ???
    if ( bytes_processed == 0 )
    {
      //#if defined (TTMPEG2VIDEOSTREAM_DEBUG)
      log->warningMsg( c_name, "0 bytes read (!)" );
      //#endif
      count = -1;
      break;
    }

    // -----------------------------------------------------------------------
    // process all read bytes
    // -----------------------------------------------------------------------
    //object_processed = false;
    do
    {
      inner_loop_count++;
      object_processed = true;

      // Ist die Start-Adresse des current_objects im Speicher?
      if ( current_object->headerOffset() >= abs_pos && 
          current_object->headerOffset() <= abs_pos+bytes_processed-1 )
      {
        // 12 Bytes weniger, wg. der Headerdaten
        if ( current_object->headerOffset() < abs_pos+bytes_processed-watermark ) 
        {
          object_processed = false;

          // -------------------------------------------------------------------
          // removing unwanted objects
          // -------------------------------------------------------------------	  
          if ( break_objects->count() > 0 )
          {
#if defined (TTMPEG2VIDEOSTREAM_DEBUG)
            log->infoMsg(c_name, ">>> remove unwanted objects: %d", break_objects->count());
#endif
            TTBreakObject* current_break = (TTBreakObject*)break_objects->top();

            if ( current_break->stopObject() != NULL && 
                current_break->stopObject()->headerOffset() == current_object->headerOffset() )
            {
              //qDebug( "%sstop    object-1: %d",c_name,current_break->stopObject() );
              //qDebug( "%srestart object-1: %d",c_name,current_break->restartObject() );
              // Wir müssen so tun, als würden wir die Objekte schreiben
              off64_t adress_delta = current_break->restartObject()->headerOffset()-current_object->headerOffset();
              bytes_processed = (int)(current_object->headerOffset()-abs_pos);
              count   -= adress_delta;
              abs_pos += adress_delta;
              current_object    = current_break->restartObject(); // hier gehts weiter
              current_index_pos = current_break->restartObjectIndex();
              stream_buffer->seekAbsolute( current_break->restartObject()->headerOffset() );
              current_break->setStopObject( (TTVideoHeader*)NULL );
              // erzwingen des Abbruchs der while Schleife und 
              object_processed = true; 
              // zurückschreiben des Buffers.
              continue; 
            }
            if (current_break->restartObject()->headerOffset() == current_object->headerOffset() )
            {
              if (current_break->copyStart() !=-1 )
              {
                // Encoder anschmeißen
                encodePart( current_break->copyStart(), current_break->copyStop(), cr, fs );
                // Die B-Frames des nächsten GOP entfernen lassen
                close_next_GOP = true; 
              }
              delete break_objects->pop();
              // Jetzt können wir ganz normal weiter machen...
            }
          }

          // -------------------------------------------------------------------
          // sequence header
          // -------------------------------------------------------------------
          if ( current_object->headerType() == TTMpeg2VideoHeader::sequence_start_code )
          {
            //TTBreakObject* new_break = (TTBreakObject*)NULL;

            // Nur, falls gewünscht!
            //if ( cr->createDVDCompilantStream() )
            //{
            //new_break = checkCompliance( current_object as Sequence, 
            //				  start_object,
            //				  end_object,
            //				  cr, close_next_GOP);
            //if ( ttAssigned( new_reak) )
            //	break_objects->push( new_break );
            // Nur weitermachen, wenn die gesamte GOP encodet werden muss.
            // sonst sofort zum Beginn der while-Schleife
            //if ( ttAssigned( new_break ) &&
            // new_break->stop_object == current_object)
            //continue; 
            //}

            // Maximale Bitrate ermitteln
            if ( cr->max_bitrate < ((TTSequenceHeader*)current_object)->bit_rate_value )
              cr->max_bitrate =((TTSequenceHeader*)current_object)->bit_rate_value;
          }

          // -------------------------------------------------------------------
          // sequence-end header
          // -------------------------------------------------------------------
          // Sequence Ende Codes immer rausnehmen
          if ( current_object->headerType() == TTMpeg2VideoHeader::sequence_end_code )
          {
            TTBreakObject* new_break = new TTBreakObject();

            new_break->setStopObject( current_object, current_index_pos );
            new_break->setRestartObject( header_list->headerAt( current_index_pos+1 ), current_index_pos+1 );
            break_objects->push( new_break );
            continue;
          }

          // -------------------------------------------------------------------
          // gop header
          // -------------------------------------------------------------------
          if ( current_object->headerType() == TTMpeg2VideoHeader::group_start_code )
          {
            TTGOPHeader* gop = (TTGOPHeader*)current_object;
            TTTimeCode* tc   = ttFrameToTimeCode( cr->pictures_written, frameRate() );

            // Diese GOP bereits zu, so müssen wir nicht noch mal schließen!
            current_header_list_pos = current_index_pos;
            do
            {
              current_header_list_pos++;
            } while ( header_list->headerAt( current_header_list_pos )->headerType() == TTMpeg2VideoHeader::picture_start_code &&
                current_header_list_pos < header_list->count()-1);

            TTPicturesHeader* next_pic = header_list->pictureHeaderAt( current_header_list_pos );

            if ( close_next_GOP && ttAssigned( next_pic ) && 
                next_pic->temporal_reference == 0 )
              close_next_GOP = false;

            time_code[0]=(uint8_t)(((tc->drop_frame_flag?1:0)<<7)
                +((tc->hours & 0x1f)<<2)
                +((tc->minutes & 0x30)>>4));
            time_code[1]=(uint8_t)(((tc->minutes & 0x0f)<<4)
                +((tc->marker_bit?1:0)<<3)
                +((tc->seconds & 0x38)>>3));
            time_code[2]=(uint8_t)(((tc->seconds & 0x07)<<5)
                + ((tc->pictures & 0x3e)>>1));
            time_code[3]=(uint8_t)((((tc->pictures & 0x01)==1)?0x80:0x00)
                | ((close_next_GOP || gop->closed_gop)?0x40:0)
                | (gop->broken_link ? 0x20:0x00));
            buffer[current_object->headerOffset()-abs_pos+4] = time_code[0];
            buffer[current_object->headerOffset()-abs_pos+5] = time_code[1];
            buffer[current_object->headerOffset()-abs_pos+6] = time_code[2];
            buffer[current_object->headerOffset()-abs_pos+7] = time_code[3];

            delete tc;
          }

          // -------------------------------------------------------------------
          // picture header
          // -------------------------------------------------------------------
          if ( current_object->headerType() == TTMpeg2VideoHeader::picture_start_code )
          {
            current_picture = (TTPicturesHeader*)current_object;

            // Sollen die B-Frames raus?
            if ( close_next_GOP       && 
                temp_ref_delta  != 0 && 
                current_picture->picture_coding_type == 3 )
            {
              // Vorrücken auf das nächste Objekt, was kein B-Frame ist.
              current_header_list_pos = current_index_pos;
              do	      
              {
                current_header_list_pos++;
              }
              while ( header_list->headerTypeAt( current_header_list_pos ) == TTMpeg2VideoHeader::picture_start_code &&
                  ((TTPicturesHeader*)header_list->headerAt( current_header_list_pos ))->picture_coding_type == 3 &&
                  current_header_list_pos < header_list->count()-1);

              TTBreakObject* new_break = new TTBreakObject();

              new_break->setStopObject( current_object, current_index_pos );
              new_break->setRestartObject( header_list->headerAt( current_header_list_pos ), current_header_list_pos );

              //qDebug( "%sstop    object-2: %d",c_name,new_break->stopObject() );
              //qDebug( "%srestart object-2: %d",c_name,new_break->restartObject() );

              break_objects->push( new_break );

              // Das wird jetzt implizit gemacht.
              close_next_GOP = false; 

              // An den Beginn der while-Schleife springen
              continue; 
            }

            // Bildchen zählen ;-)
            cr->pictures_written++;

            if ( current_picture->picture_coding_type == 1 )
            {
              if ( close_next_GOP )
                temp_ref_delta = current_picture->temporal_reference;
              else
                temp_ref_delta = 0;
            }

//            log->infoMsg( c_name, "Close next GOP: %d / tempRefDelta: %d", close_next_GOP, temp_ref_delta);

            // Müssen neue temporärere Referenzen geschrieben werden?
            if ( temp_ref_delta != 0) 
            {
              short new_temp_ref = (short)(current_picture->temporal_reference-temp_ref_delta);
              int   offset       = (int)(current_picture->headerOffset()-abs_pos)+4; // Hier rein damit!

              buffer[offset]   = (uint8_t)(new_temp_ref >> 2);                       // Bit 10 - 2 von 10 Bit Tempref
              buffer[offset+1] = (uint8_t)(((new_temp_ref & 0x0003) << 6) +          // Bit 1 und 0 von 10 Bit Tempref
                  ((int)current_picture->picture_coding_type << 3) +                 // Bildtype auf Bit 5, 4 und 3
                  (current_picture->vbv_delay >> 13));                               // 3 Bit von VBVDelay
            }
          }
          current_index_pos++;
          current_header_list_pos = current_index_pos;
          current_object          = header_list->headerAt( current_header_list_pos);
        }
        else
        {
          // watermark Bytes zurück, dann setzen wir beim nächsten kopieren des Buffers richtig auf!
          stream_buffer->seekRelative( -watermark );
          bytes_processed -= watermark; // natürlich haben wir watermark Bytes weniger!
        }
      }
      //qDebug( "%sobject processed: %d",c_name,object_processed );
      //qDebug( "%sloop time: %d ms",c_name,loop_time.elapsed() );
    }while( !object_processed );


    // Jetzt können wir den Pufferinhalt schreiben, ggf. nicht komplett
    //qDebug( "%swrite data: processed: %d",c_name,bytes_processed );
    fs->directWrite( buffer, bytes_processed );

    // cancel action
    if ( ttAssigned(progress_bar) )
    {
      process += bytes_processed;
      if (progress_bar->setProgress( process ))
      {
        delete [] buffer;
        delete break_objects;
        delete [] time_code;

        return;
      }
    }


    count   -= bytes_processed;
    abs_pos += bytes_processed;

    //qDebug( "%snew loop: processed: %d / count: %lld / abs pos: %lld",c_name,bytes_processed,count, abs_pos );

    if ( bytes_processed <= 0 )
    {
      log->warningMsg( c_name, "bytes_processed: %d",bytes_processed );
      count = -1;
    }
  } //while(count > 0)

  log->infoMsg(c_name, "Number of break_objects: %d", break_objects->count());

  // noch was zu encoden ?
  if ( break_objects->count() > 0 )
  {
    TTBreakObject* current_break = (TTBreakObject*)break_objects->pop();  

    if ( current_break->copyStart() != -1 )
      encodePart( current_break->copyStart(), current_break->copyStop(), cr, fs);
  }

  delete [] buffer;
  delete break_objects;
  delete [] time_code;

  int proc_elapsed     = proc_time.elapsed();
  double transfer_rate = ((end_object->headerOffset()-start_object->headerOffset())/1024.0/1024.0)/((double)proc_elapsed/1000.0);
  log->infoMsg( c_name, "elapsed time: %d ms / %lf Mb/s",proc_elapsed,transfer_rate );
  log->infoMsg( c_name, "outer loop: %ld / inner loop: %ld",outer_loop_count, inner_loop_count );
}

/*! ////////////////////////////////////////////////////////////////////////////
 * EncodePart
 * Decode and encode the part between start and end
 * Keep in mind, that we must encode one extra frame because the transfer
 * objects method works in an [a,e[ intervall!
 */
void TTMpeg2VideoStream::encodePart(int start, int end, 
                                    TTCutParameter* cr, TTFileBuffer* cut_stream)
{
#if defined (ENCODE_INFO)
  log->infoMsg(c_name, "-------------------------------------------------");
  log->infoMsg(c_name, "Encode part");
  log->infoMsg(c_name, "Start: %d / end: %d", start, end);
#endif

  QFileInfo    new_file_info;
  TTAVIWriter* avi_writer = new TTAVIWriter( NULL );

  // save current cut parameter
  bool save_last_call         = cr->last_call;
  bool save_write_max_bitrate = cr->write_max_bitrate;

  // no sequence end code
  cr->last_call         = false;
  cr->write_max_bitrate = false;

  // decode the part to avi
  avi_writer->initAVIWriter( (TTVideoStream*)this );
  avi_writer->writeAVI( start, end );
  avi_writer->closeAVI();

  // use the sequence header according to current picture for information about
  // frame size (width x height) and aspect ratio
  TTSequenceHeader* seq_head  = this->currentSequenceHeader();

  QDir    temp_dir( TTCut::tempDirPath );
  QString avi_out_file   = "encode.avi";
  QString mpeg2_out_file = "encode";          // extension is added by transcode (!)
  new_file_info.setFile( temp_dir, avi_out_file );
  
  TTPicturesHeader* start_pic = header_list->pictureHeaderAt( start );
  bool progressive_frame      = start_pic->progressive_frame;

  if ( progressive_frame )
    log->infoMsg(c_name, "Progressive Frame: %d", progressive_frame);

  TTEncodeParameter enc_par;
  enc_par.avi_input_finfo.setFile( temp_dir, avi_out_file );
  enc_par.mpeg2_output_finfo.setFile( temp_dir, mpeg2_out_file );
  enc_par.video_width        = seq_head->horizontal_size_value;
  enc_par.video_height       = seq_head->vertical_size_value;
  enc_par.video_aspect_code  = seq_head->aspect_ratio_information;
  enc_par.video_bitrate      = seq_head->bitRateKbit();

#if defined(ENCODE_INFO)
  log->infoMsg(c_name, "current sequence information:");
  log->infoMsg(c_name, "---------------------------------------");
  log->infoMsg(c_name, "width: %d x height: %d", enc_par.video_width,       enc_par.video_height);
  log->infoMsg(c_name, "aspect code: %d/%s",     enc_par.video_aspect_code, TTCut::toAscii(seq_head->aspectRatioText()));
  log->infoMsg(c_name, "frame rate : %s",        TTCut::toAscii(seq_head->frameRateText()));
  log->infoMsg(c_name, "bitrate    : %f Kbit",   seq_head->bitRateKbit());
  log->infoMsg(c_name, "---------------------------------------");
#endif

  if ( ttAssigned( progress_bar ) )
    progress_bar->hideBar();

  TTTranscodeProvider* transcode_prov = new TTTranscodeProvider( );
  transcode_prov->setParameter( enc_par );

  // encode the part with transcode
  if ( transcode_prov->encodePart() )
  {
    new_file_info.setFile( temp_dir, "encode.m2v" );
    TTMpeg2VideoStream* new_mpeg_stream = new TTMpeg2VideoStream( new_file_info );

    int header_count = new_mpeg_stream->createHeaderList(); 
    int index_count  = new_mpeg_stream->createIndexList();
    new_mpeg_stream->indexList()->sortDisplayOrder();

    // just to supress compiler warning about unused variables
#if not defined(ENCODE_INFO)
    header_count = 0;
    index_count  = 0;
#endif
#if defined(ENCODE_INFO)
    log->infoMsg(c_name, "header list created: header: %d / index: %d", header_count, index_count);
    log->infoMsg(c_name, "cut: 0 - %d",end-start );
#endif

    // insert encoded part into the target cut stream
    new_mpeg_stream->cut( cut_stream, 0, end-start, cr );

    delete new_mpeg_stream;
  }
  else
  {
    log->errorMsg(c_name, "Error in transcode part (!)");
  }

  // remove temporary files
  QString rm_cmd = "rm ";
  rm_cmd        += new_file_info.absolutePath();
  rm_cmd        += "/encode.*";

  system( rm_cmd.toAscii().data() );

  delete avi_writer;
  delete transcode_prov;

  if ( ttAssigned( progress_bar ) )
    progress_bar->showBar();

  // set cut parameter back
  cr->last_call         = save_last_call;
  cr->write_max_bitrate = save_write_max_bitrate;

#if defined (ENCODE_INFO)
  log->infoMsg(c_name, "-------------------------------------------------");
#endif
}
