/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005/2010 / www.tritime.org                    */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2008                                                      */
/* FILE     : ttavstream.cpp                                                  */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 05/12/2005 */
/* MODIFIED: b. altendorf                                    DATE: 06/01/2008 */
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
//                               +- TTMpegAudioStream
//             +- TTAudioStream -|
//             |                 +- TTAC3AudioStream
// TTAVStream -|
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

#include "../data/ttcutlistdata.h"
#include "../common/ttexception.h"

#include <QString>
#include <qfileinfo.h>

// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// TTAVStream: Abstract base class for all AV stream objects
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

const char c_name[] = "TTAVStream    : ";

/* /////////////////////////////////////////////////////////////////////////////
 * Construct TTAVStream object with file info
 * The file given by f_info must exist!
 */
TTAVStream::TTAVStream(const QFileInfo &f_info)
{
  log         = TTMessageLogger::getInstance();
  stream_info = new QFileInfo(f_info);

  // check if stream exists
  if (!stream_info->exists())
  {
    QString msg = QString("Stream does not exists: %1").arg(stream_info->filePath());
    throw TTIOException(msg);
  }

  stream_buffer = new TTFileBuffer(stream_info->filePath(), QIODevice::ReadOnly);

  if (!ttAssigned(stream_buffer)) {
    QString msg = QString("Error allocating buffer for: %1").arg(stream_info->filePath());
    throw TTIOException(msg);
  }

  stream_type = TTAVTypes::unknown;
}

/* /////////////////////////////////////////////////////////////////////////////
 * Destructor
 */
TTAVStream::~TTAVStream()
{
  if (ttAssigned(stream_buffer)) {
    stream_buffer->close();
    delete stream_buffer;
    stream_buffer = NULL;
  }

  if (ttAssigned(stream_info)) {
    delete stream_info;
    stream_info = NULL;
  }
}


// -----------------------------------------------------------------------------
// methods common for all stream types
// -----------------------------------------------------------------------------
/* /////////////////////////////////////////////////////////////////////////////
 * Get the current stream filename
 */
QString TTAVStream::fileName()
{
  return stream_info->fileName();
}

/* /////////////////////////////////////////////////////////////////////////////
 * Get the current stream full path (path + filename)
 */
QString TTAVStream::filePath()
{
  return stream_info->filePath();
}

/* /////////////////////////////////////////////////////////////////////////////
 * Get the current stream filename extension
 */
QString TTAVStream::fileExtension()
{
  return stream_info->suffix();
}


/* /////////////////////////////////////////////////////////////////////////////
 * Get the current stream length in bytes
 */
quint64 TTAVStream::streamLengthByte()
{
  return stream_buffer->size();
}

// copy a segment (start, end) from source- to target-stream
// -----------------------------------------------------------------------------
void TTAVStream::copySegment(TTFileBuffer* cut_stream, quint64 start_adr, quint64 end_adr)
{
  quint64 progress    = 0;
  quint64 buffer_size = 65536;
  quint8* buffer      = new quint8[buffer_size];
  quint64 count       = end_adr-start_adr+1;

  stream_buffer->seekAbsolute( start_adr );

  emit progressChanged(TTProgressBar::Init, "copy segment", count);

  while( count > buffer_size )
  {
    progress = end_adr-start_adr+1-count;

    stream_buffer->readByte(buffer, buffer_size );
    cut_stream->directWrite(buffer, (quint64)buffer_size );

    count -= buffer_size;
    emit progressChanged(TTProgressBar::Step, "copy segment", progress);
  }

  stream_buffer->readByte(buffer, count);
  cut_stream->directWrite(buffer, count);

  emit progressChanged(TTProgressBar::Finished, "copy segment", progress);

  delete []buffer;
}


// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// *** TTAudioStream: Base class for all audio streams
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////


// constructor with file info and start position
// -----------------------------------------------------------------------------
TTAudioStream::TTAudioStream(const QFileInfo &f_info, int s_pos)
  : TTAVStream(f_info)
{
  start_pos   = s_pos;
  header_list = 0;
}

TTAudioStream::~TTAudioStream()
{
  if (ttAssigned(header_list)) {
    header_list->deleteAll();
    delete header_list;
    header_list = NULL;
  }
}

// return pointer to current header list
// -----------------------------------------------------------------------------
TTAudioHeaderList* TTAudioStream::headerList()
{
  return header_list;
}

TTAudioHeader* TTAudioStream::headerAt( int index )
{
  return header_list->audioHeaderAt(index);
}

// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// *** TTVideoStream: Base class for all video streams
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// constructor with file info
// -----------------------------------------------------------------------------
TTVideoStream::TTVideoStream( const QFileInfo &f_info )
  : TTAVStream( f_info )
{
  header_list          = NULL;
  index_list           = NULL;
  current_index        = 0;
  current_marker_index = 0;
}

TTVideoStream::~TTVideoStream()
{
  if (ttAssigned( header_list)) {
    header_list->deleteAll();
    delete header_list;
  }

  if (ttAssigned( index_list)) {
    index_list->deleteAll();
    delete index_list;
  }
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

int TTVideoStream::frameCount()
{
  return (ttAssigned(index_list)) ? index_list->count() : 0;
}

/* /////////////////////////////////////////////////////////////////////////////
 * Return the frame rate from current sequence
 */
float TTVideoStream::frameRate()
{
  // the framerate is constant in one stream!
  TTSequenceHeader* sequence = header_list->firstSequenceHeader();

  return sequence->frameRateValue();
}

/* /////////////////////////////////////////////////////////////////////////////
 * Return the bitrate from current sequence
 */
float TTVideoStream::bitRate()
{
  TTSequenceHeader* seq = getSequenceHeader(current_index);

  return seq->bitRateKbit();
}

TTSequenceHeader* TTVideoStream::currentSequenceHeader()
{
  return getSequenceHeader(current_index);
}

TTSequenceHeader* TTVideoStream::getSequenceHeader(int pos)
{
  int pic_index = index_list->moveToPrevIndexPos(pos+1, 1);

  if (pic_index <= 0)
    return header_list->firstSequenceHeader();

  int head_index = index_list->headerListIndex(pic_index);

  while ((head_index >= 0) &&
         (header_list->headerTypeAt(head_index) != TTMpeg2VideoHeader::sequence_start_code)) {
      head_index--;
  }

  return (head_index > 0)
      ? header_list->sequenceHeaderAt(head_index)
      : header_list->firstSequenceHeader();
}

/*
 * Return the picture coding type from picture at 'current_index'
 * position
 */
int TTVideoStream::currentFrameType()
{
  return (ttAssigned(index_list))
      ? index_list->pictureCodingType(current_index)
      : 0;
}

QTime TTVideoStream::currentFrameTime()
{
  return ttFramesToTime( current_index, frameRate() );
}

quint64 TTVideoStream::currentFrameOffset()
{
  return frameOffset(current_index);
}

int TTVideoStream::frameType( int i_pos )
{
  return index_list->pictureCodingType( i_pos );
}

QTime TTVideoStream::frameTime( int i_pos )
{
  return ttFramesToTime( i_pos, frameRate() );
}

quint64 TTVideoStream::frameOffset( int i_pos )
{
  quint64 offset;

  if ( ttAssigned( index_list ) && ttAssigned( header_list ) )
  {
    int h_index = index_list->headerListIndex( i_pos );
    TTPicturesHeader* current_picture = header_list->pictureHeaderAt( h_index );
    offset = current_picture->headerOffset();
  }
  else
    offset = (quint64)0;

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
  int prev_marker_index = current_marker_index;
  current_marker_index  = index;

  return prev_marker_index;
}

// -----------------------------------------------------------------------------
// positioning methods for convenience
// -----------------------------------------------------------------------------

int TTVideoStream::moveToIndexPos(int pos, int f_type)
{
  int index = index_list->moveToIndexPos(pos, f_type);
  return (index >= 0) ? current_index=index : current_index;
}

// goto next frame
// -----------------------------------------------------------------------------
int TTVideoStream::moveToNextFrame( int f_type )
{
  int index = index_list->moveToNextIndexPos(current_index, f_type);
  return (index >= 0) ? current_index=index : current_index;
}

// goto previous frame
// -----------------------------------------------------------------------------
int TTVideoStream::moveToPrevFrame( int f_type )
{
  int index = index_list->moveToPrevIndexPos(current_index, f_type);
  return (index >= 0) ? current_index=index : current_index;
}

// goto next intra-frame (I)
// -----------------------------------------------------------------------------
int TTVideoStream::moveToNextIFrame()
{
  int index = index_list->moveToNextIndexPos(current_index, 1);
  return (index >= 0) ? current_index=index : current_index;
}

// goto previous intra-frame (I)
// -----------------------------------------------------------------------------
int TTVideoStream::moveToPrevIFrame()
{
  int index = index_list->moveToPrevIndexPos(current_index, 1);
  return (index >= 0) ? current_index=index : current_index;
}

// goto next predicted-frame (P)
// -----------------------------------------------------------------------------
int TTVideoStream::moveToNextPFrame()
{
  int index = index_list->moveToNextIndexPos(current_index, 2);
  return (index >= 0) ? current_index=index : current_index;
}

// goto previous predicted-frame (P)
// -----------------------------------------------------------------------------
int TTVideoStream::moveToPrevPFrame()
{
  int index = index_list->moveToPrevIndexPos(current_index, 2);
  return (index >= 0) ? current_index=index : current_index;
}

// goto next P- or I-frame, whatever comes first
// -----------------------------------------------------------------------------
int TTVideoStream::moveToNextPIFrame()
{
  int pos_i_frame = index_list->moveToNextIndexPos(current_index, 1);
  int pos_p_frame = index_list->moveToNextIndexPos(current_index, 2);

  int index = (pos_i_frame <= pos_p_frame) ? pos_i_frame : pos_p_frame;

  return (index >= 0) ? current_index=index : current_index;
}

// goto previous P- or I-frame
// -----------------------------------------------------------------------------
int TTVideoStream::moveToPrevPIFrame()
{
  int pos_i_frame = index_list->moveToPrevIndexPos(current_index, 1);
  int pos_p_frame = index_list->moveToPrevIndexPos(current_index, 2);

  int index = (pos_i_frame >= pos_p_frame) ? pos_i_frame : pos_p_frame;

  return (index >= 0) ? current_index=index : current_index;
}

// goto next frame where the video format changes (aspect ratio, or resolution, or fps)
// -----------------------------------------------------------------------------
int TTVideoStream::moveToNextFFrame()
{
  float   fps      = currentSequenceHeader()->frameRateValue();
  int     horiz    = currentSequenceHeader()->horizontalSize();
  int     vert     = currentSequenceHeader()->verticalSize();
  QString aspect   = currentSequenceHeader()->aspectRatioText();
  int     newIndex = current_index;
  while ( -1 != (newIndex=index_list->moveToNextIndexPos(newIndex, 1)) ) {
    TTSequenceHeader* newHeader = getSequenceHeader(newIndex);
    if ( newHeader->frameRateValue()  != fps   ||
         newHeader->horizontalSize()  != horiz ||
         newHeader->verticalSize()    != vert  ||
         newHeader->aspectRatioText() != aspect )
    {
      return current_index=newIndex;
    }
  }
  return current_index;
}

// goto prev frame where the video format changes (aspect ratio, or resolution, or fps)
// -----------------------------------------------------------------------------
int TTVideoStream::moveToPrevFFrame()
{
  float   fps      = currentSequenceHeader()->frameRateValue();
  int     horiz    = currentSequenceHeader()->horizontalSize();
  int     vert     = currentSequenceHeader()->verticalSize();
  QString aspect   = currentSequenceHeader()->aspectRatioText();
  int     newIndex = current_index;
  while ( -1 != (newIndex=index_list->moveToPrevIndexPos(newIndex, 1)) ) {
    TTSequenceHeader* newHeader = getSequenceHeader(newIndex);
    if ( newHeader->frameRateValue()  != fps   ||
         newHeader->horizontalSize()  != horiz ||
         newHeader->verticalSize()    != vert  ||
         newHeader->aspectRatioText() != aspect )
    {
      return current_index=newIndex;
    }
  }
  return current_index;
}
