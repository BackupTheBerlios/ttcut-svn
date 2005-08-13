/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttavstream.cpp                                                  */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 05/12/2005 */
/* MODIFIED: b. altendorf                                    DATE: 06/02/2005 */
/* MODIFIED: b. altendorf                                    DATE: 06/10/2005 */
/* MODIFIED: b. altendorf                                    DATE: 08/13/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTAVSTREAM (abstract)
// TTAUDIOSTREAM
// TTVIDEOSTREAM
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

#if defined(__INTEL_COMPILER)
#pragma warning(disable:981) // operands are evaluated in unspecified order
#pragma warning(disable:177) // variable "..." was declared but never referenced
#pragma warning(disable:869) // parameter "..." was never referenced
#pragma warning(disable:186) // pointless comparison...
#endif

#include "ttavstream.h"

#include <qstring.h>
#include <qfileinfo.h>


// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// *** TTAVStream: Abstract base class for all AV stream objects
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

const char c_name[] = "TTAVStream    : ";

// construct TTAVStream object
// -----------------------------------------------------------------------------
TTAVStream::TTAVStream()
{
  stream_info   = (QFileInfo*)NULL;
  stream_buffer = (TTFileBuffer*)NULL;
  stream_mode   = 0;
  stream_type   = TTAVTypes::unknown;
  stream_open   = false;
  stream_parsed = false;
  progress_bar  = (TTProgressBar*)NULL;
}

// construct TTAVStream object with file info
// -----------------------------------------------------------------------------
TTAVStream::TTAVStream( const QFileInfo &f_info )
{
  stream_info   = new QFileInfo( f_info );

  // check if stream exists
  if ( !stream_info->exists() )
  {
    delete stream_info;
    stream_info = (QFileInfo*)NULL;
  }

  stream_buffer = (TTFileBuffer*)NULL;
  stream_mode   = 0;
  stream_type   = TTAVTypes::unknown;
  stream_open   = false;
  stream_parsed = false;
  progress_bar  = (TTProgressBar*)NULL;
}

// destructor
// -----------------------------------------------------------------------------
TTAVStream::~TTAVStream()
{
  if ( ttAssigned(stream_info) )
    delete stream_info;

  if ( ttAssigned(stream_buffer) )
    delete stream_buffer;
}

// -----------------------------------------------------------------------------
// methods common for all stream types
// -----------------------------------------------------------------------------

bool TTAVStream::openStream()
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
    stream_buffer = new TTFileBuffer( stream_info->filePath().ascii(), fm_open_read );

    if ( ttAssigned( stream_buffer ) )
    {
      stream_open         = true;
      stream_mode         = fm_open_read;
      b_result            = true;
      stream_length_bytes = stream_buffer->streamLength();
    }
  }
  return b_result;
}

bool TTAVStream::closeStream()
{
  bool b_result = false;

  if ( stream_open )
  {
    if ( ttAssigned( stream_buffer ) )
    {
      stream_buffer->closeFile();
      delete stream_buffer;

      stream_open   = false;
      stream_mode   = 0;
      stream_buffer = (TTFileBuffer*)NULL;

      b_result      = true;
    }
  }

  return b_result;
}


// open source stream
// -----------------------------------------------------------------------------
void TTAVStream::openSource( QString f_name, bool use_buffer )
{

}

// close source stream
// -----------------------------------------------------------------------------
void TTAVStream::closeSource()
{

}

// set stream file name and create file info (stream_info) object
// -----------------------------------------------------------------------------
void TTAVStream::setFileName( const QString &f_name )
{
  if ( ttAssigned(stream_info) )
    stream_info->setFile( f_name );
  else
    stream_info = new QFileInfo( f_name );
}

// return the stream file name
// -----------------------------------------------------------------------------
QString TTAVStream::fileName()
{
  return stream_info->fileName();
}

// return the stream file name including absolute path
// -----------------------------------------------------------------------------
QString TTAVStream::filePath()
{
  return stream_info->filePath();
}

// return the file extension
// -----------------------------------------------------------------------------
QString TTAVStream::streamExtension()
{
  return stream_info->extension( false );
}

// set stream file info
// -----------------------------------------------------------------------------
void TTAVStream::setFileInfo( const QFileInfo &f_info )
{
  if ( ttAssigned(stream_info) )
  {
    delete stream_info;
    stream_info = new QFileInfo( f_info );
  }
  else
    stream_info = new QFileInfo( f_info );
}

// return stream file info
// -----------------------------------------------------------------------------
QFileInfo* TTAVStream::fileInfo()
{
  if ( ttAssigned(stream_info) )
    return stream_info;
  else
    return NULL;
}

// return pointer to current stream buffer object
// -----------------------------------------------------------------------------
TTFileBuffer* TTAVStream::streamBuffer()
{
  return stream_buffer;
}

// return current stream length in bytes
// -----------------------------------------------------------------------------
off64_t TTAVStream::streamLengthByte()
{
  return stream_length_bytes;
}

// return current stream typ
// -----------------------------------------------------------------------------
TTAVTypes::AVStreamType TTAVStream::streamType()
{
  return stream_type;
}

// set pointer to a TTProgressBar object
// -----------------------------------------------------------------------------
void TTAVStream::setProgressBar( TTProgressBar* p_bar )
{
  progress_bar = p_bar;
}

// copy a segment (start, end) from source- to target-stream
// -----------------------------------------------------------------------------
void TTAVStream::copySegment( TTFileBuffer* cut_stream, off64_t start_adr, off64_t end_adr )
{
  off64_t  progress;
  uint8_t* buffer      = new uint8_t[65536];
  off64_t  count       = end_adr-start_adr+1;
  off64_t  total_steps = count;

  //qDebug( "%scopy segment: %lld/%lld - %lld",c_name,count,start_adr,end_adr );
  //qDebug( "%scopy segment: cut stream offset: %lld",c_name,cut_stream->currentOffset() );

  stream_buffer->seekAbsolute( start_adr );

  //qDebug( "%scopy segment: new Position: %lld",c_name,stream_buffer->currentOffset() );

  if ( ttAssigned( progress_bar ) )
  {
    progress_bar->resetProgress();
    progress_bar->setTotalSteps( count );
  }

  while( count > 65536 )  
  {
    progress = end_adr-start_adr+1-count;

    stream_buffer->readArray( buffer, (int)65536 );
    cut_stream->directWrite( buffer, (off64_t)65536 );

    count -= 65536;
    //qDebug( "%scopy segment: count: %lld",c_name,count );
    //qDebug( "%scopy segment: cut stream offset: %lld",c_name,cut_stream->currentOffset() );

    if ( ttAssigned(progress_bar) )
    {
      progress_bar->setProgress( progress );
    }
  }

  //qDebug( "%scopy segment: read remaining data: %lld",c_name,count );

  stream_buffer->readArray( buffer, (int)count );
  cut_stream->directWrite( buffer, count );

  //qDebug( "%scopy segment: cut stream offset: %lld",c_name,cut_stream->currentOffset() );

  delete [] buffer;
}


// -----------------------------------------------------------------------------
// virtual methods, have to be overwritten in subclasses
// -----------------------------------------------------------------------------

// create index list
// -----------------------------------------------------------------------------
void TTAVStream::createIndex()
{

}

// create header list
// -----------------------------------------------------------------------------
int TTAVStream::createHeaderList()
{
  return (int)0;
}

// create index list
// -----------------------------------------------------------------------------
int TTAVStream::createIndexList()
{
  return (int)0;
}

// cut method
// -----------------------------------------------------------------------------
void TTAVStream::cut( TTFileBuffer* cut_stream, int start, int end, TTCutParameter* cp )
{

}

void TTAVStream::cut( TTFileBuffer* cut_stream, TTAVCutList* cut_list )
{

}

// is cut-in possible
// -----------------------------------------------------------------------------
bool TTAVStream::isCutInPoint( int pos )
{
  return false;
}

// is cut-out possible
// -----------------------------------------------------------------------------
bool TTAVStream::isCutOutPoint( int pos )
{
  return false;
}

// calculate stream length between start and end position
// -----------------------------------------------------------------------------
long TTAVStream::calculateLength( int start, int end )
{
  return (long)0;
}


// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// *** TTAudioStream: Base class for all audio streams
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// constructor
// -----------------------------------------------------------------------------
TTAudioStream::TTAudioStream()
  : TTAVStream()
{
  start_pos = 0;
}


// constructor with file info and start position
// -----------------------------------------------------------------------------
TTAudioStream::TTAudioStream( const QFileInfo &f_info, int s_pos )
  : TTAVStream( f_info )
{
  start_pos = s_pos;

  index_list  = NULL;
  header_list = NULL;
}

TTAudioStream::~TTAudioStream()
{
  qDebug( "TTAUDIOSTREAM : destructor call" );

  if ( ttAssigned( header_list ) )
  delete header_list;

  if( ttAssigned( index_list ) )
  delete index_list;
}

// cut-in always possible; video 
// -----------------------------------------------------------------------------
bool TTAudioStream::isCutInPoint( int pos )
{
  return true;
}

// cut-out always possible; video
// -----------------------------------------------------------------------------
bool TTAudioStream::isCutOutPoint( int pos )
{
  return true;
}


// return pointer to current header list
// -----------------------------------------------------------------------------
TTAudioHeaderList* TTAudioStream::headerList()
{
  return header_list;
}

// return pointer to current index list
// -----------------------------------------------------------------------------
TTAudioIndexList* TTAudioStream::indexList()
{
  return index_list;
}

// set new header list from extern
// -----------------------------------------------------------------------------
void TTAudioStream::setHeaderList( TTAudioHeaderList* h_list )
{
  header_list = h_list;
}

// set new index list from extern
// -----------------------------------------------------------------------------
void TTAudioStream::setIndexList( TTAudioIndexList* i_list )
{
  index_list = i_list;
}

TTAudioHeader* TTAudioStream::headerAt( int index )
{
  return header_list->audioHeaderAt(index);
}

// return the total count of samples in current audio stream
// -----------------------------------------------------------------------------
int TTAudioStream::sampleCount()
{
  return 0;
}

// return the current audio stream length in ??? (seconds)
// -----------------------------------------------------------------------------
double TTAudioStream::length()
{
  return 0;
}


// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// *** TTVideoStream: Base class for all video streams
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// constructor
// -----------------------------------------------------------------------------
TTVideoStream::TTVideoStream()
  : TTAVStream()
{
  header_list          = NULL;
  index_list           = NULL;
  num_header           = 0;
  num_index            = 0;
  current_index        = 0;
  previous_index       = 0;
  current_marker_index = 0;
  prev_marker_index    = 0;
}

// constructor with file info
// -----------------------------------------------------------------------------
TTVideoStream::TTVideoStream( const QFileInfo &f_info )
  : TTAVStream( f_info )
{
  header_list          = NULL;
  index_list           = NULL;
  num_header           = 0;
  num_index            = 0;
  current_index        = 0;
  previous_index       = 0;
  current_marker_index = 0;
  prev_marker_index    = 0;
}

TTVideoStream::~TTVideoStream()
{
  qDebug( "TTVIDEOSTREAM : destructor call" );

  if ( ttAssigned( header_list ) )
    delete header_list;

  if ( ttAssigned( index_list ) )
    delete index_list;
}

// return pointer to current header list
// -----------------------------------------------------------------------------
TTVideoHeaderList* TTVideoStream::headerList()
{
  return header_list;
}


// return pointer to current index list
// -----------------------------------------------------------------------------
TTVideoIndexList* TTVideoStream::indexList()
{
  return index_list;
}


// set pointer to header list
// -----------------------------------------------------------------------------
void TTVideoStream::setHeaderList( TTVideoHeaderList* h_list )
{
  header_list = h_list;
}


// set pointer to index list
// -----------------------------------------------------------------------------
void TTVideoStream::setIndexList( TTVideoIndexList* i_list )
{
  index_list = i_list;
}


int TTVideoStream::frameCount()
{
  if ( ttAssigned( index_list ) )
    return index_list->count();
  else
    return (int)0;
}

float TTVideoStream::frameRate()
{
  int sequence_index;
  TTSequenceHeader* current_sequence;

  sequence_index = index_list->sequenceIndex( current_index );
  current_sequence = header_list->sequenceHeaderAt( sequence_index );

  return current_sequence->frameRateValue();
}

float TTVideoStream::bitRate()
{
  int sequence_index;
  TTSequenceHeader* current_sequence;

  sequence_index = index_list->sequenceIndex( current_index );
  current_sequence = header_list->sequenceHeaderAt( sequence_index );

  return current_sequence->bitRateKbit();
}

int TTVideoStream::currentFrameType()
{
  if ( ttAssigned( index_list ) )
    return index_list->pictureCodingType( current_index );
  else
    return (int)0;
}

QTime TTVideoStream::currentFrameTime()
{
  return ttFramesToTime( current_index, frameRate() );
}

off64_t TTVideoStream::currentFrameOffset()
{
  int    h_index;
  off64_t offset;
  TTPicturesHeader* current_picture;

  if ( ttAssigned( index_list ) && ttAssigned( header_list ) )
  {
    h_index = index_list->headerListIndex( current_index );
    current_picture = header_list->pictureHeaderAt( h_index );
    offset = current_picture->headerOffset();
  }
  else
    offset = (off64_t)0;

  return offset;
}

int TTVideoStream::frameType( int i_pos )
{
  return index_list->pictureCodingType( i_pos );
}

QTime TTVideoStream::frameTime( int i_pos )
{
  return ttFramesToTime( i_pos, frameRate() );
}

off64_t TTVideoStream::frameOffset( int i_pos )
{
  int    h_index;
  off64_t offset;
  TTPicturesHeader* current_picture;

  if ( ttAssigned( index_list ) && ttAssigned( header_list ) )
  {
    h_index = index_list->headerListIndex( i_pos );
    current_picture = header_list->pictureHeaderAt( h_index );
    offset = current_picture->headerOffset();
  }
  else
    offset = (off64_t)0;

  return offset;
}

// -----------------------------------------------------------------------------
// navigating in stream by index- and header-list
// -----------------------------------------------------------------------------

// return current selected index in index-list
// -----------------------------------------------------------------------------
int TTVideoStream::currentIndex()
{
  return current_index;
}

// set the current index in index list
// -----------------------------------------------------------------------------
int TTVideoStream::setCurrentIndex( int index )
{
  previous_index = current_index;
  current_index  = index;

  return previous_index;
}

// return the previous index
// -----------------------------------------------------------------------------
int TTVideoStream::previousIndex()
{
  return previous_index;
}

// return marker index position
// -----------------------------------------------------------------------------
int TTVideoStream::markerIndex()
{
  return current_marker_index;
}

// set marker index position
// -----------------------------------------------------------------------------
int TTVideoStream::setMarkerIndex( int index )
{
  prev_marker_index    = current_marker_index;
  current_marker_index = index;

  return prev_marker_index;
}

// move to specified index position (index and frame coding type)
// -----------------------------------------------------------------------------
int TTVideoStream::moveToIndexPos( int index, int f_type )
{
  int j;

  if ( index_list->streamOrder() )
    {
      return moveToIndexPosSO( index, f_type );
    }

  //qDebug("move to index pos: %d / %d / %d",index,f_type,num_index);

  // search a specific frame type in video index list
  // ---------------------------------------------------------------------------
  if ( f_type > 0 )
  {
    // search forward
    if ( index > current_index && index < num_index )
    {
      j = 1;
      do
      {
	if ( index+j >= num_index ) break;

	video_index = index_list->videoIndexAt( index+j );
	j++;
      } while ( video_index->picture_coding_type != f_type );

      index += j-1;
    }
    // search backward
    else
    {
      j = -1;
      do
      {
	// CORRECT:  Warnung: comparison of unsigned expression < 0 is always false
	if ( index+j < 0 ) break;

	video_index = index_list->videoIndexAt( index+j );
	j--;
      } while ( video_index->picture_coding_type != f_type );

      index += j+1;
    }
  }

  // index in range ??? When not, correct
  // ---------------------------------------------------------------------------
  // index greater equal num_index
  if ( index >= num_index )
    index = num_index-1;

  // index ok, set new current position
  // ---------------------------------------------------------------------------
  previous_index = current_index;
  current_index  = index;

  return current_index;
}

// move to index pos "index" in stream order
// -----------------------------------------------------------------------------
int TTVideoStream::moveToIndexPosSO( int index, int f_type )
{
  int  j;
  int index_bak = index;
  int so_index;

  //qDebug("move to index pos in stream order: %d / %d / %d",index,f_type,num_index);

  // search a specific frame type in video index list
  // ---------------------------------------------------------------------------
  if ( f_type > 0 )
  {
    // search forward
    if ( index > current_index && index < num_index )
    {
      j = 1;
      do
      {
	if ( index+j >= num_index ) break;

	so_index    = index_list->stream_order_list[index+j];
	video_index = index_list->videoIndexAt( so_index );
	j++;
      } while ( video_index->picture_coding_type != f_type );

      index += j-1;
    }
    // search backward
    else
    {
      j = -1;
      do
      {
	if ( index+j < 0 ) break;

	so_index    = index_list->stream_order_list[index+j];
	video_index = index_list->videoIndexAt( so_index );
	j--;
      } while ( video_index->picture_coding_type != f_type );

      index += j+1;
    }
  }

  so_index = index_list->stream_order_list[index];

  // index in range ??? When not, correct
  // ---------------------------------------------------------------------------
  // index greater equal num_index
  if ( so_index >= num_index )
    so_index = num_index-1;

  // index ok, set new current position
  // ---------------------------------------------------------------------------
  previous_index = current_index;
  current_index  = index;

  return so_index;
}

// move to index by abs. time and frame coding type
// -----------------------------------------------------------------------------
int TTVideoStream::moveToIndexPos( const QTime& f_time, int f_type )
{
  long  index;
  int  ret_index  = current_index;

  index     = ttTimeToFrames( f_time, frameRate() );
  ret_index = moveToIndexPos( index, f_type );

  return ret_index;
}

// -----------------------------------------------------------------------------
// positioning methods for convenience
// -----------------------------------------------------------------------------

// goto next frame
// -----------------------------------------------------------------------------
int TTVideoStream::moveToNextFrame( int f_type )
{
  int ret_index = current_index;
  int index     = current_index+1;

  ret_index = moveToIndexPos( index, f_type );

  return ret_index;
}

// goto previous frame
// -----------------------------------------------------------------------------
int TTVideoStream::moveToPrevFrame( int f_type )
{
  int ret_index = current_index;
  int index     = current_index;

  if ( index > (long)0 )
    {
      index--;
      ret_index = moveToIndexPos( index, f_type );
    }

  return ret_index;
}

// goto next intra-frame (I)
// -----------------------------------------------------------------------------
int TTVideoStream::moveToNextIFrame()
{
  int ret_index = current_index;
  long iPos;
  long iFound;

  iPos   = current_index;
  iFound = -1;

  video_index = index_list->videoIndexAt( iPos );

  if ( video_index->picture_coding_type == 1 ) iPos++;

  while( iPos < (long)num_index-1 && iFound < 0 )
  {
    video_index = index_list->videoIndexAt(iPos);

    if ( video_index->picture_coding_type == 1 )
      iFound = iPos;

    iPos++;
  }

  if ( iPos != iFound && iFound >= 0 )
  {
    previous_index = current_index;
    current_index  = iFound;
  }

  return current_index;
}

// goto previous intra-frame (I)
// -----------------------------------------------------------------------------
int TTVideoStream::moveToPrevIFrame()
{
  long        iPos;
  long        iFound;

  iPos   = current_index;
  iFound = -1;

  video_index = index_list->videoIndexAt(iPos);
  if ( video_index->picture_coding_type == 1 ) iPos--;

  while( iPos >= 0 && iFound < 0 )
  {
    video_index = index_list->videoIndexAt(iPos);

    if ( video_index->picture_coding_type == 1 )
      iFound = iPos;

    iPos--;
  }

  if ( iPos != iFound && iFound >= 0 )
  {
    previous_index = current_index;
    current_index  = iFound;
  }

  return current_index;
}

// goto next predicted-frame (P)
// -----------------------------------------------------------------------------
int TTVideoStream::moveToNextPFrame()
{
  long        iPos;
  long        iFound;

  iPos   = current_index;
  iFound = -1;

  video_index = index_list->videoIndexAt( iPos );

  if ( video_index->picture_coding_type == 2 ) iPos++;

  while( iPos < (long)num_index-1 && iFound < 0 )
  {
    video_index = index_list->videoIndexAt(iPos);

    if ( video_index->picture_coding_type == 2 )
      iFound = iPos;

    iPos++;
  }

  if ( iPos != iFound && iFound >= 0 )
  {
    previous_index = current_index;
    current_index  = iFound;
  }

  return current_index;
}

// goto previous predicted-frame (P)
// -----------------------------------------------------------------------------
int TTVideoStream::moveToPrevPFrame()
{
  long        iPos;
  long        iFound;

  iPos   = current_index;
  iFound = -1;

  video_index = index_list->videoIndexAt(iPos);
  if ( video_index->picture_coding_type == 2 ) iPos--;

  while( iPos >= 0 && iFound < 0 )
  {
    video_index = index_list->videoIndexAt(iPos);

    if ( video_index->picture_coding_type == 2 )
      iFound = iPos;

    iPos--;
  }

  if ( iPos != iFound && iFound >= 0 )
  {
    previous_index = current_index;
    current_index  = iFound;
  }

  return current_index;
}

// goto next P- or I-frame, whatever comes first
// -----------------------------------------------------------------------------
int TTVideoStream::moveToNextPIFrame()
{
  long        iPos;
  long        iFound, iFoundP, iFoundI;

  // search previous P-frame
  iPos    = current_index;
  iFoundP = -1;

  video_index = index_list->videoIndexAt(iPos);
  if ( video_index->picture_coding_type == 2 ) iPos++;

  while( iPos >= 0 && iFoundP < 0 )
  {
    video_index = index_list->videoIndexAt(iPos);

    if ( video_index->picture_coding_type == 2 )
      iFoundP = iPos;

    iPos++;
  }

  // search previous I-frame
  iPos    = current_index;
  iFoundI = -1;

  // search previous P-frame
  video_index = index_list->videoIndexAt(iPos);
  if ( video_index->picture_coding_type == 1 ) iPos++;

  while( iPos >= 0 && iFoundI < 0 )
  {
    video_index = index_list->videoIndexAt(iPos);

    if ( video_index->picture_coding_type == 1 )
      iFoundI = iPos;

    iPos++;
  }

  if ( iFoundP < iFoundI )
    iFound = iFoundP;
  else
    iFound = iFoundI;

  if ( iPos != iFound && iFound >= 0 )
  {
    previous_index = current_index;
    current_index  = iFound;
  }

  return current_index;
}

// goto previous P- or I-frame
// -----------------------------------------------------------------------------
int TTVideoStream::moveToPrevPIFrame()
{
  long        iPos;
  long        iFound, iFoundP, iFoundI;

  // search previous P-frame
  iPos    = current_index;
  iFoundP = -1;

  video_index = index_list->videoIndexAt(iPos);
  if ( video_index->picture_coding_type == 2 ) iPos--;

  while( iPos >= 0 && iFoundP < 0 )
  {
    video_index = index_list->videoIndexAt(iPos);

    if ( video_index->picture_coding_type == 2 )
      iFoundP = iPos;

    iPos--;
  }

  // search previous I-frame
  iPos    = current_index;
  iFoundI = -1;

  // search previous P-frame
  video_index = index_list->videoIndexAt(iPos);
  if ( video_index->picture_coding_type == 1 ) iPos--;

  while( iPos >= 0 && iFoundI < 0 )
  {
    video_index = index_list->videoIndexAt(iPos);

    if ( video_index->picture_coding_type == 1 )
      iFoundI = iPos;

    iPos--;
  }

  if ( iFoundP > iFoundI )
    iFound = iFoundP;
  else
    iFound = iFoundI;

  if ( iPos != iFound && iFound >= 0 )
  {
    previous_index = current_index;
    current_index  = iFound;
  }

  return current_index;
}
