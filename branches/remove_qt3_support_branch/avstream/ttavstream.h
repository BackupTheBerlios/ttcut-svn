/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttavstream.h                                                    */
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

#ifndef TTAVSTREAM_H
#define TTAVSTREAM_H

#include "ttcommon.h"

#ifdef __WIN32
#include "ttwfilebuffer.h"
#else
#include "ttfilebuffer.h"
#endif

#include "../data/ttcutparameter.h"
#include "../common/ttmessagelogger.h"
#include "../gui/ttprogressbar.h"

#include "ttavtypes.h"
#include "ttaudioheaderlist.h"
#include "ttaudioindexlist.h"
#include "ttvideoheaderlist.h"
#include "ttvideoindexlist.h"
#include "ttmemorybuffer.h"

#include <qdatetime.h>

//class TTCutParameter;
class TTCutListData;
class TTAVTypes;
class QString;
class QFileInfo;
class TTProgressBar;
class TTCutParameter;

// -----------------------------------------------------------------------------
// *** TTAVStream: Abstract class TTAVStream
// -----------------------------------------------------------------------------
class TTAVStream
{
public:
  TTAVStream();
  TTAVStream( const QFileInfo &f_info );
  virtual ~TTAVStream();

  virtual void setFileName( const QString &f_name );
  virtual QString fileName();
  virtual QString filePath();
  virtual void setFileInfo( const QFileInfo &f_info );
  virtual QFileInfo* fileInfo();
  virtual TTFileBuffer* streamBuffer();
  virtual off64_t streamLengthByte();
  virtual TTAVTypes::AVStreamType streamType();

  virtual bool openStream();
  virtual bool closeStream();

  virtual void setProgressBar( TTProgressBar* p_bar );

  virtual void createIndex();
  virtual int createHeaderList();
  virtual int createIndexList();
  virtual void cut( TTFileBuffer* cut_stream, int start, int end, TTCutParameter* cp );
  virtual void cut( TTFileBuffer* cut_stream, TTCutListData* cut_list );
  virtual bool isCutInPoint( int pos );
  virtual bool isCutOutPoint( int pos );
  virtual QString streamExtension();
  virtual void copySegment( TTFileBuffer* cut_stream, off64_t start_adr, off64_t end_adr );

  virtual void openSource( QString f_name, bool use_buffer );
  virtual void closeSource();

  virtual long calculateLength( int start, int end );

protected:
  QFileInfo*     stream_info;
  TTFileBuffer*  stream_buffer;
  int            stream_mode;
  off64_t        stream_length_bytes;
  TTAVTypes::AVStreamType stream_type;
  bool           stream_open;
  bool           stream_parsed;
  TTProgressBar* progress_bar;
  TTMessageLogger* log;
};


// -----------------------------------------------------------------------------
// *** TTAudioStream: Class TTAudioStream
// -----------------------------------------------------------------------------
class TTAudioStream : public TTAVStream
{
public:
  TTAudioStream();
  TTAudioStream( const QFileInfo &f_info, int s_pos=0);
  virtual ~TTAudioStream();

  // header an index list
  TTAudioHeaderList* headerList();
  TTAudioIndexList* indexList();
  void setHeaderList( TTAudioHeaderList* h_list );
  void setIndexList( TTAudioIndexList* i_list );

  TTAudioHeader* headerAt( int index );

  // virtual cut methods
  virtual bool isCutInPoint( int pos );
  virtual bool isCutOutPoint( int pos );

  // stream properties common for all audio stream types
  int sampleCount();
  double length();
  virtual QString absStreamTime(){return "";};

protected:
  // header and index list
  TTAudioHeaderList* header_list;
  TTAudioIndexList*  index_list;

  // audio_delay > 0: audio starts before video (in ms)
  // audio_delay < 0: audio starts after  video (in ms)
  int    audio_delay;
  int    start_pos;
  int    samples_count;
  int    frame_length;
  double frame_time;
};


// -----------------------------------------------------------------------------
// *** TTVideoStream: Class TTVideoStream
// -----------------------------------------------------------------------------
class TTVideoStream : public TTAVStream
{
 public:
  TTVideoStream();
  TTVideoStream( const QFileInfo &f_info );
  virtual ~TTVideoStream();

  // header- and index-list
  TTVideoHeaderList* headerList();
  TTVideoIndexList* indexList();
  void setHeaderList( TTVideoHeaderList* h_list );
  void setIndexList( TTVideoIndexList* i_list );

  int    frameCount();
  float   frameRate();
  float   bitRate();
  int     currentFrameType();
  QTime   currentFrameTime();
  off64_t currentFrameOffset();
  int     frameType( int i_pos );
  QTime   frameTime( int i_pos );
  off64_t frameOffset( int i_pos );

  // navigation in header-list

  // navigation in index-list
  int currentIndex();
  int setCurrentIndex( int index );
  int previousIndex();
  int markerIndex();
  int setMarkerIndex( int index );
  int moveToIndexPos( int index, int f_type=0 );
  int moveToIndexPos( const QTime& f_time, int f_type=0 );
  int moveToIndexPosSO( int index, int f_type=0 );
  int moveToNextFrame( int f_type=0 );
  int moveToPrevFrame( int f_type=0 );
  int moveToNextIFrame();
  int moveToPrevIFrame();
  int moveToNextPFrame();
  int moveToPrevPFrame();
  int moveToNextPIFrame();
  int moveToPrevPIFrame();

protected:
  // List-objects
  TTVideoHeaderList* header_list;
  TTVideoIndexList*  index_list;
  int               num_header;
  int               num_index;

  // Navigation
  TTVideoIndex*      video_index;
  int               current_index;
  int               previous_index;
  int               current_marker_index;
  int               prev_marker_index;

  // intern
  float              frame_rate;
  float              bit_rate;
};

#endif //TTAVSTREAM_H
