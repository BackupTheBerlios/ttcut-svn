/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttmpeg2videostream.cpp                                          */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 05/12/2005 */
/* MODIFIED: b. altendorf                                    DATE: 06/05/2005 */
/* MODIFIED: b. altendorf                                    DATE: 08/13/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTMPEG2VIDEOSTREAM
// ----------------------------------------------------------------------------

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

//#define TTMPEG2VIDEOSTREAM_DEBUG

#include "ttmpeg2videostream.h"

#include <QDir>
#include <QStack>

const char c_name[] = "MPEG2STREAM";

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


  TTMpeg2VideoStream::TTMpeg2VideoStream( const QFileInfo &f_info )
    : TTVideoStream( f_info )
  {
    log = TTMessageLogger::getInstance();

    stream_type   = TTAVTypes::mpeg2_demuxed_video;
    mpeg2_stream  = NULL;
    header_list   = NULL;
    index_list    = NULL;
  }

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

  // Create the mpeg2 header-list
  int TTMpeg2VideoStream::createHeaderList()
  {
    QString   idd_stream_name;
    QFileInfo idd_stream_info;
    bool      header_list_exists = false;
    int      u_result = 0;

    //qDebug("%screate header-list: %s",c_name,stream_info->filePath().ascii());

    // open the mpeg2 stream
    if ( !openStream() )
    {
      qDebug("%scould not open stream", c_name ); //%s",c_name,stream_info->filePath().ascii());
      u_result = 0;
      return u_result;
    }

    // new video-header-list
    // ---------------------------------------------------------------------------
    header_list = new TTVideoHeaderList( 2000 );

    // ---------------------------------------------------------------------------
    // Mpeg2Schnitt idd-file
    // ---------------------------------------------------------------------------
    // create Mpeg2Schnitt idd-stream name
    if ( TTCut::readVideoIDD )
    {
      idd_stream_name = ttChangeFileExt( stream_info->filePath(), "idd" );

      log->infoMsg(c_name, "look for idd-file: %s",idd_stream_name.ascii());

      // check for Mpeg2Schnitt idd-stream in current directory
      idd_stream_info.setFile( idd_stream_name );

      if ( idd_stream_info.exists() )
        {
    idd_stream         = new TTFileBuffer( idd_stream_name, fm_open_read );

    header_list_exists = createHeaderListFromIdd();

    // we can delete the idd-stream
    idd_stream->closeFile();
    delete idd_stream;
        }
    }

    // ---------------------------------------------------------------------------
    // Parse the mpeg2 stream for header
    // ---------------------------------------------------------------------------
    if ( !header_list_exists )
    {
      header_list_exists = createHeaderListFromMpeg2();
    }
      
    // mpeg2 header list successfully created ???
    // ---------------------------------------------------------------------------
    if ( header_list_exists )
    {
      num_header = header_list->count();
      u_result   = num_header;
    }
    else
    {
      delete header_list;
      header_list = NULL;
      num_header  = 0;
    }

    log->infoMsg(c_name, "header-list created: %d",u_result);
    return u_result;
  }


  // Create the mpeg2 index-list
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
    qDebug("%sindex list created: %d/%d",c_name,num_index,index_list->size() );
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

  TTSequenceHeader* TTMpeg2VideoStream::currentSequenceHeader()
  {
    int    header_index;
    uint8_t header_type;

    header_index    = index_list->headerListIndex( currentIndex() );

    header_type     = header_list->at( header_index )->headerType();
    
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
      log->warningMsg(c_name, "no sequence header found (!): %d", header_index );
      return NULL;
    }
  }


  TTGOPHeader* TTMpeg2VideoStream::currentGOPHeader()
  {
    int    header_index;
    uint8_t header_type;

    header_index    = index_list->headerListIndex( currentIndex() );

    header_type     = header_list->at( header_index )->headerType();
    
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
      qDebug( "%sno group header found (!): %d",c_name,header_index );
      return NULL;
    }
  }


  TTPicturesHeader* TTMpeg2VideoStream::currentPictureHeader()
  {
    int    header_index;
    uint8_t header_type;

    header_index    = index_list->headerListIndex( currentIndex() );

    header_type     = header_list->at( header_index )->headerType();
    
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
      qDebug( "%sno picture header found (!): %d",c_name,header_index );
      return NULL;
    }
  }

  TTSequenceHeader* TTMpeg2VideoStream::sequenceHeaderAt( int index )
  {
    int    header_index;
    uint8_t header_type;

    header_index    = index_list->headerListIndex( index );
    header_type     = header_list->at( header_index )->headerType();
    
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
      qDebug( "%sno sequence header found (!): %d",c_name,header_index );
      return NULL;
    }
  }

  TTGOPHeader* TTMpeg2VideoStream::GOPHeaderAt( int index )
  {
    int    header_index;
    uint8_t header_type;

    header_index    = index_list->headerListIndex( index );
    header_type     = header_list->at( header_index )->headerType();
    
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
      log->warningMsg(c_name, "no group header found (!): %d", header_index );
      return NULL;
    }
  }

  TTPicturesHeader* TTMpeg2VideoStream::pictureHeaderAt( int index )
  {
    int    header_index;
    uint8_t header_type;

    header_index    = index_list->headerListIndex( index );
    header_type     = header_list->at( header_index )->headerType();
    
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
      qDebug( "%sno picture header found (!): %d",c_name,header_index );
      return NULL;
    }
  }

  TTVideoHeader* TTMpeg2VideoStream::getPrevMpeg2Object( TTVideoHeader* current )
  {
    int found_index;

    found_index = header_list->indexOf( (TTAVHeader*)current );

    if ( found_index-1 < 0 )
      return (TTVideoHeader*)NULL;
    else
      return (TTVideoHeader*)header_list->headerAt( (int)found_index-1 );
  }

  TTVideoHeader* TTMpeg2VideoStream::getNextMpeg2Object( TTVideoHeader* current )
  {
    int found_index;

    found_index = header_list->indexOf( (TTAVHeader*)current );

    if ( found_index < 0 || found_index+1 >= header_list->count() )
      return (TTVideoHeader*)NULL;
    else
      return (TTVideoHeader*)header_list->headerAt( (int)found_index+1 );
  }

  // Open the mpeg2 video stream
  // -----------------------------------------------------------------------------
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
      mpeg2_stream = new TTFileBuffer( stream_info->filePath().ascii(), fm_open_read );

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

  bool TTMpeg2VideoStream::closeStream()
  {
    bool b_result = false;

    if ( stream_open )
    {
      if ( ttAssigned( mpeg2_stream ) )
      {
        mpeg2_stream->closeFile();
        delete mpeg2_stream;
        
        stream_open   = false;
        stream_mode   = 0;
        stream_buffer = (TTFileBuffer*)NULL;
        mpeg2_stream  = (TTFileBuffer*)NULL;

        b_result      = true;
      }
    }

    return b_result;
  }

  // create the mpeg2 header-list from Mpeg2Schnitt idd-file
  // -----------------------------------------------------------------------------
  bool TTMpeg2VideoStream::createHeaderListFromIdd()
  {
    bool     b_result = false;
    uint8_t  buffer4[4];
    uint8_t  buffer8[4];
    long*    byte4;
    off64_t* byte8;

    byte4 = (long*)&buffer4;
    byte8 = (off64_t*)&buffer8;

    idd_file_version = 0;

    if ( idd_stream->readArray( buffer4, 4 ) )
    {
      // idd index file ?
      if ( buffer4[0] == int('i') &&
     buffer4[1] == int('d') &&
     buffer4[2] == int('d')    )
        idd_file_version = buffer4[3];

      //qDebug("%sIDD version: %d",c_name,idd_file_version);

      readIDDHeader();

      if ( header_list->count() > 0 )
        b_result = true;
    }
    return b_result;
  }


  // create the header-list by parsing the mpeg2 stream
  // -----------------------------------------------------------------------------
  bool TTMpeg2VideoStream::createHeaderListFromMpeg2()
  {
    bool                b_result = true;
    bool                b_cancel = false;
    uint8_t             header_type;
    TTMpeg2VideoHeader* new_header;

    if ( ttAssigned( progress_bar ) )
    {
      progress_bar->setActionText( "Create video header list." );
      progress_bar->setTotalSteps( mpeg2_stream->streamLength() );
      progress_bar->setProgress( 1 );
    }

    // ---------------------------------------------------------------------------
    // create the video header list
    // ---------------------------------------------------------------------------
    try
    {
      // go through the mpeg2 stream until EOF was reached
      while( !mpeg2_stream->streamEOF() && !b_cancel )
      {
        header_type = 0xFF;
      
        // search next header (start code)
        while ( header_type != TTMpeg2VideoHeader::picture_start_code  &&
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
    break;
        case TTMpeg2VideoHeader::group_start_code:
    new_header = new TTGOPHeader();
    header_list->numGopPlus();
    break;
        case TTMpeg2VideoHeader::sequence_end_code:
    header_list->numSequenceEndPlus();
    break;
        }

#ifdef __TTMPEG2
        //qDebug("Header type: %02x",header_type);
#endif
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

    if ( header_list->count() > 0 && TTCut::createVideoIDD ) 
    {
      writeIDDFile();
    }

#if defined(TTMPEG2VIDEOSTREAM_DEBUG)
    qDebug( "%sheader list count: %d/%d",c_name,header_list->count(),header_list->size() );
#endif

    if ( b_cancel )
      b_result = false;

    return b_result;
  }


  // write MPEG2Schnit *.idd file
  // -----------------------------------------------------------------------------
  void TTMpeg2VideoStream::writeIDDFile( )
  {
    QString             idd_stream_name;
    QFileInfo           idd_stream_info;
    //TTMpeg2VideoHeader* mpeg2_header;
    TTPicturesHeader*   current_picture;
    int                index = 0;
    //int                 i;
    uint8_t             header_type;
    off64_t             offset;
    int16_t            temp_ref;
    uint8_t             coding_type;
    uint8_t             buffer[8];


    // create Mpeg2Schnitt idd-stream name
    idd_stream_name = ttChangeFileExt( stream_info->filePath(), "idd" );

    //qDebug( "%screate idd-file: %s",c_name,idd_stream_name.ascii() );

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
    idd_stream         = new TTFileBuffer( idd_stream_name, fm_open_write );

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

    progress_bar->setComplete();

    // write sequence end header type
    buffer[0] = 0xB7;
    idd_stream->directWrite(buffer, 1);  // 1 Byte

    // write last file offset
    offset      = header_list->at(index-1)->headerOffset();
    idd_stream->directWriteUInt64( offset );  // 8 Byte
  }


  // read MPEG2Schnitt *.idd file (also created by projectX)
  // -----------------------------------------------------------------------------
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
    progress_bar->resetProgress();
    progress_bar->setTotalSteps( idd_stream->streamLength() );
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
	progress_bar->setProgress( idd_stream->currentOffset() );

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
    progress_bar->setComplete();
  }
  catch (...)
  {
  }  
}


bool TTMpeg2VideoStream::isCutInPoint( int pos )
{
  if ( TTCut::encoderMode )
    return true;

  int frame_type = index_list->pictureCodingType( pos );

  if ( frame_type == 1 )
    return true;
  else
    return false;
}


bool TTMpeg2VideoStream::isCutOutPoint( int pos )
{
  if ( TTCut::encoderMode )
    return true;

  int frame_type = index_list->pictureCodingType( pos );

  if ( frame_type == 1  || frame_type == 2 )
    return true;
  else
    return false;
}



void TTMpeg2VideoStream::cut( TTFileBuffer* cut_stream, TTAVCutList* cut_list )
{
  int i;
  TTCutParameter* cut_param = new TTCutParameter();
  int start_pos;
  int end_pos;
  QString action_string;

#if defined (TTMPEG2VIDEOSTREAM_DEBUG)
  qDebug( "%s-----------------------------------------------",c_name );
  qDebug( "%s>>> cut video stream",c_name );
  qDebug( "%s-----------------------------------------------",c_name );
  qDebug( "%sentries in cut list: %d",c_name,cut_list->count() );
  qDebug( "%starget stream      : %s",c_name,cut_stream->fileName() );
#endif

  for ( i = 0; i < cut_list->count(); i++ )
  {
    if ( i == 0 )
      cut_param->first_call = true;
    else
      cut_param->first_call = false;

    if ( i == cut_list->count()-1 )
      cut_param->last_call = true;

    start_pos = cut_list->cutInAt( i );
    end_pos   = cut_list->cutOutAt( i );

    if ( ttAssigned( progress_bar ) )
    {
      action_string.sprintf( "Video cut: %d/%d-%d",i+1,start_pos,end_pos );
      progress_bar->setActionText( action_string );
    }

#if defined (TTMPEG2VIDEOSTREAM_DEBUG)
    qDebug( "%sstart - end        : %d | %d - %d",c_name,i+1,start_pos,end_pos );
#endif

    cut( cut_stream, start_pos, end_pos, cut_param );
  }

#if defined (TTMPEG2VIDEOSTREAM_DEBUG)
  qDebug( "%s-----------------------------------------------",c_name );
#endif

  delete cut_param;
}


void TTMpeg2VideoStream::cut( TTFileBuffer* fs, int start, int end, TTCutParameter* cr)
{
  int             current_index_list_pos;
  int             current_header_list_pos;
  int             start_object_index;
  int             end_object_index;
  TTVideoHeader*  current_header;
  TTVideoHeader*  start_object;
  TTVideoHeader*  end_object;
  int             temp_end;
  uint8_t         seq_end[4];

#if defined (TTMPEG2VIDEOSTREAM_DEBUG)
  qDebug( "%s-----------------------------------------------",c_name );
  qDebug( "%s>>> cut video stream",c_name );
  qDebug( "%s-----------------------------------------------",c_name );
  qDebug( "%starget stream      : %s",c_name,fs->fileName() );
  qDebug( "%sstart: %ld | end: %ld | count: %ld",c_name,start,end,index_list->count() );
#endif

  // open source mpeg2-stream for reading
  openStream();
    
  // Beim ersten Aufruf einen neuen IndexContainer erzeugen
  if ( cr->first_call && cr->result_header_list == (TTVideoHeaderList*)NULL )
  {
    //qDebug( "%s+++ first call to cut",c_name );
    cr->result_header_list = new TTVideoHeaderList( 1000 );
  }

  // ---------------------------------------------------------------------------
  // investigate start object for copy sequence
  // ---------------------------------------------------------------------------
  current_index_list_pos = start;

  // Bildtyp des Start-Bildes ermitteln; aus Indexliste (!)
  // Ist dies kein I-Frame, temporäres AVI erzeugen und kodieren lassen
  if ( index_list->pictureCodingType( start ) != 1 )
  {
    temp_end = start+1;

    // Alle Bilder bis zum nächsten I-Frame mitnehmen
    while ( index_list->pictureCodingType( temp_end ) != 1 )
    {
      //qDebug( "%ssearch I-frame at %d, frame type: %d",c_name,temp_end,index_list->pictureCodingType( temp_end ) );
      temp_end++;
    }
    
    // encoden lassen
    encodePart( start, temp_end, cr, fs);
    
    // auf das nächste Picture in der Indexliste positionieren, 
    // damit der Rest kopiert werden kann.
    current_index_list_pos = temp_end;
  }
  
  // Aktuelle Position in der Headerliste
  current_header_list_pos = index_list->headerListIndex( current_index_list_pos ); 
    
  // bei Mpeg2 liegt vor dem GOP zwingend eine Sequence
  current_header_list_pos -= 2;
  current_header = (TTVideoHeader*)header_list->headerAt( current_header_list_pos );

  // aktueller Header ist kein Sequence-Header...
  if ( current_header->headerType() != TTMpeg2VideoHeader::sequence_start_code )
  {
    log->warningMsg(c_name, "no sequence at cut start....");

    // Erste (und einzige) Sequence aus dem Stream kopieren
    TTSequenceHeader* m2o  = header_list->sequenceHeaderAt( current_header_list_pos );
    TTVideoHeader*    next = header_list->headerAt( header_list->headerIndex( (TTVideoHeader*)m2o )+1 );

    // Suche nach einem Sequence-Header vor aktueller Position erfolgreich ???
    if ( ttAssigned( m2o ) && ttAssigned( next ) )
    {
      // kopiere den Sequence-Header in den Stream
      copySegment( fs, m2o->headerOffset(), next->headerOffset()-1 );

      if ( cr->max_bitrate < m2o->bit_rate_value )
	cr->max_bitrate = m2o->bit_rate_value;

      TTSequenceHeader* newSequence = new TTSequenceHeader( );
      
      newSequence->setHeaderOffset( (off64_t)0 );
      newSequence->aspect_ratio_information = m2o->aspect_ratio_information;
      
      if ( ttAssigned( cr->result_header_list ) )
	cr->result_header_list->add( newSequence );
      
      // Auf GOP positionieren
      current_header_list_pos += 1;
    }
    // Keine Sequence gefunden
    else
    {
      log->warningMsg(c_name, "no sequence found in stream (?)");
    }
  }
   
  // --------------------------------------------------------------------------- 
  // start object for copy sequence (copy sequence starts at sequence-header or
  // at GOP-Header if we have must correct the sequence; see above)
  // --------------------------------------------------------------------------- 
  start_object_index = current_header_list_pos;
  start_object = header_list->headerAt( start_object_index );
    
  // ---------------------------------------------------------------------------
  // investigate end object for copy sequence
  // ---------------------------------------------------------------------------
  current_index_list_pos = end;
  temp_end               = end;

  // solange dies kein I- oder P-Frame ist...
  while( index_list->pictureCodingType( temp_end ) == 3 )
    temp_end--;
    
  // Endeobjekt ist ein I- oder P-Frame
  current_index_list_pos  = temp_end;
  current_header_list_pos = index_list->headerListIndex( current_index_list_pos );     

  // die folgenden B-Frames gehören noch dazu (nach stream-order)
  if ( current_header_list_pos < header_list->count()-2 )
  {
    do
    {
      current_header_list_pos++;
      current_header = header_list->headerAt( current_header_list_pos );
    } while ( current_header->headerType() == TTMpeg2VideoHeader::picture_start_code && 
	      ((TTPicturesHeader*)current_header)->picture_coding_type == 3 &&
	      current_header_list_pos < header_list->count()-1 );
  }
    
  // ---------------------------------------------------------------------------
  // end object for copy sequence
  // ---------------------------------------------------------------------------
  end_object_index = current_header_list_pos;
  end_object = header_list->headerAt( end_object_index );

  // transfer the copy sequence to destination stream
#if defined (TTMPEG2VIDEOSTREAM_DEBUG)
  qDebug( "%s>>> transfer objects: %d - %d",c_name,header_list->headerIndex(start_object),header_list->headerIndex(end_object) );
#endif
  transferMpegObjects( fs, start_object, start_object_index,
		       end_object, end_object_index, cr );
    
  // liegt das gewünschte Ende auf einem B-Frame so muss dieser
  // Bereich neu encodiert werden.
  //qDebug( "%stemp end: %d, end: %d",c_name,temp_end,end );
  if ( temp_end != end )
    encodePart( temp_end+1, end, cr, fs );
    
  // last call to cut; write sequence end code
  if ( cr->last_call )
  {
    //qDebug( "%s--- last call to cut",c_name );
    // Endesequenz anhängen
    TTSequenceEndHeader* se = new TTSequenceEndHeader();
    se->setHeaderOffset( fs->currentOffset() );

    if ( ttAssigned( cr->result_header_list ) )
      cr->result_header_list->add( se );

    // um die Anzahl der Bilder in den GOPs festzustellen
    //cr.result_header_list.IndexList.CreatePictureDecoderOrder(); 
       
    //if ( cr->writeSequenceEndCode() ) // auch schreiben?
    seq_end[0] = 0x00;
    seq_end[1] = 0x00;
    seq_end[2] = 0x01;
    seq_end[3] = 0xb7;

    fs->directWrite( seq_end, 4);
    //if ( cr->writeMaxBitrate() )
    //  writeMaxBitrate( fs, cr->max_bitrate );
  }
  // close source mpeg2-stream
  closeStream();
}


// Kopiert das Intervall von [startObject,endObject[ !
void TTMpeg2VideoStream::transferMpegObjects( TTFileBuffer* fs,
					      TTVideoHeader* start_object,
					      int start_object_index,
					      TTVideoHeader* end_object,
					      __attribute__ ((unused))int end_object_index,
					      TTCutParameter* cr )
{
  uint8_t*          buffer  = new uint8_t[65536];
  off64_t           count   = end_object->headerOffset()-start_object->headerOffset();
  off64_t           abs_pos = start_object->headerOffset();
  int               process = 0;
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

  QStack<TTBreakObject*>* break_objects = new QStack<TTBreakObject*>; 
  
  __attribute__ ((unused))TTVideoHeader* newMpeg2Object = (TTVideoHeader*)NULL;
  
  current_index_pos = start_object_index;

  QTime proc_time;
  proc_time.start();

  //#if defined (TTMPEG2VIDEOSTREAM_DEBUG)
  log->infoMsg( c_name, "----------------------------------------------------" );
  log->infoMsg( c_name, "transferMpegObjects" );
  log->infoMsg( c_name, "start: %lld / end: %lld / count: %lld",start_object->headerOffset(),end_object->headerOffset(),count );
  //#endif

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
	    qDebug( "%s>>> remove unwanted objects: %d",c_name, break_objects->count() );
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
	    
	    // Müssen neue temporärere Referenzen geschrieben werden?
	    if ( temp_ref_delta != 0) 
	    {
	      short new_temp_ref = (short)(current_picture->temporal_reference-temp_ref_delta);
	      int   offset       = (int)(current_picture->headerOffset()-abs_pos)+4; // Hier rein damit!

	      buffer[offset]   = (uint8_t)(new_temp_ref >> 2);                            // Bit 10 - 2 von 10 Bit Tempref
	      buffer[offset+1] = (uint8_t)(((new_temp_ref & 0x0003) << 6) +               // Bit 1 und 0 von 10 Bit Tempref
					   ((int)current_picture->picture_coding_type << 3) +   // Bildtype auf Bit 5, 4 und 3
					   (current_picture->vbv_delay >> 13));                 // 3 Bit von VBVDelay
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

    if ( ttAssigned(progress_bar) )
    {
    process += bytes_processed;
    progress_bar->setProgress( process );
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


void TTMpeg2VideoStream::encodePart( int start, int end, TTCutParameter* cr, TTFileBuffer* cut_stream )
{
//#if defined (TTMPEG2VIDEOSTREAM_DEBUG)
  log->infoMsg(c_name, "---------------------------------------------");
  log->infoMsg(c_name, "encode part: start: %d -- end: %d",start,end );
  log->infoMsg(c_name, "---------------------------------------------");
//#endif

  QFileInfo new_file_info;

  TTAVIWriter* avi_writer = new TTAVIWriter( NULL );

  // save current cut parameter
  bool save_last_call         = cr->last_call;
  bool save_write_max_bitrate = cr->write_max_bitrate;

  // never write sequence end code (!)
  cr->last_call = false;
  cr->write_max_bitrate = false;

  avi_writer->initAVIWriter( (TTVideoStream*)this );

  avi_writer->writeAVI( start, end );

  avi_writer->closeAVI();
 
  // transcode object
  TTSequenceHeader* seq_head  = header_list->sequenceHeaderAt( 0 );
  TTEncodeParameter enc_par;
  QDir temp_dir( TTCut::tempDirPath );
  QString avi_out_file = "encode.avi";
  QString mpeg2_out_file = "encode";
  new_file_info.setFile( temp_dir, "encode.avi" );

  enc_par.avi_input_finfo.setFile( temp_dir, avi_out_file );
  enc_par.mpeg2_output_finfo.setFile( temp_dir, mpeg2_out_file );
  enc_par.video_width        = seq_head->horizontal_size_value;
  enc_par.video_height       = seq_head->vertical_size_value;
  enc_par.video_aspect_code  = seq_head->aspect_ratio_information;
  enc_par.video_bitrate      = seq_head->bitRateKbit();

  if ( ttAssigned( progress_bar ) )
    progress_bar->hideBar();

  TTTranscodeProvider* transcode_prov = new TTTranscodeProvider( );
  
  transcode_prov->setParameter( enc_par );

  if ( transcode_prov->encodePart() )
  {
//#if defined (TTMPEG2VIDEOSTREAM_DEBUG)
    log->infoMsg(c_name, "transcode exit succesfully");
//#endif

    new_file_info.setFile( temp_dir, "encode.m2v" );
    TTMpeg2VideoStream* new_mpeg_stream = new TTMpeg2VideoStream( new_file_info );
    
    new_mpeg_stream->createHeaderList(); 
    new_mpeg_stream->createIndexList();
    new_mpeg_stream->indexList()->sortDisplayOrder();
    
//#if defined (TTMPEG2VIDEOSTREAM_DEBUG)
    log->infoMsg(c_name, "cut: 0 - %d",end-start );
//#endif

    new_mpeg_stream->cut( cut_stream, 0, end-start, cr );

//#if defined (TTMPEG2VIDEOSTREAM_DEBUG)
    log->infoMsg(c_name, "---------------------------------------------");
//#endif
    
    delete new_mpeg_stream;
  }
  else
  {
    log->errorMsg(c_name, "error in transcode part (!)");
  }

  // remove temporary file
  QString rm_cmd  = "rm ";
  rm_cmd         += new_file_info.absolutePath();
  rm_cmd         += "/encode.*";
  
  system( rm_cmd.ascii() );
  
  delete transcode_prov;

  if ( ttAssigned( progress_bar ) )
    progress_bar->showBar();

  // set cut parameter back
  cr->last_call         = save_last_call;
  cr->write_max_bitrate = save_write_max_bitrate;
}
