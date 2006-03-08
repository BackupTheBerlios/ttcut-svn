/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttavtypes.h                                                     */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 05/14/2005 */
/* MODIFIED: b. altendorf                                    DATE: 06/10/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTAVTYPES
// *** TTAUDIOTYPES
// *** TTVIDEOTYPES
// ----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Overview
// -----------------------------------------------------------------------------
//
//            +- TTAudioType
//            |               
// TTAVTypes -|               
//            |
//            +- TTVideoType
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

#ifndef TTAVTYPES_H
#define TTAVTYPES_H

#include "ttcommon.h"

#ifdef __WIN32
#include "ttwfilebuffer.h"
#else
#include "ttfilebuffer.h"
#endif

#include "ttmemorybuffer.h"

class QString;
class QFileInfo;

class TTAudioStream;
class TTVideoStream;

// -----------------------------------------------------------------------------
// *** TTAVTypes: Abstract base class for AV stream types
// -----------------------------------------------------------------------------
class TTAVTypes
{
 public:
  TTAVTypes( QString f_name );
  virtual ~TTAVTypes();

  // known AV stream types
  enum AVStreamType
  {
    mpeg_audio,
    ac3_audio,
    pcm_audio,
    dts16_audio,
    dts14_audio,
    mpeg2_demuxed_video,
    mpeg2_mplexed_video,
    unknown
  };

  virtual QFileInfo&   avStreamInfo();
  virtual AVStreamType avStreamType();
  virtual off64_t      typeHeaderOffset();
  virtual long         typeHeaderLength();


 protected:
  QFileInfo*      av_stream_info;
  bool            av_stream_exists;
  TTFileBuffer*   av_stream;
  AVStreamType    av_stream_type;
  off64_t         type_header_offset;
  long            type_header_length;
  TTMemoryBuffer* mem_buffer;
};


// -----------------------------------------------------------------------------
// *** TTAudioType: Audio-stream type
// -----------------------------------------------------------------------------
class TTAudioType : public TTAVTypes
{
 public:
  TTAudioType( QString f_name );
  ~TTAudioType();

  TTAudioStream* createAudioStream();

 protected:
  void getAudioStreamType();

 private:
  int      start_pos;
  uint8_t  frame_size_code;
  uint16_t sample_rate;
  uint16_t frame_len;
};


// -----------------------------------------------------------------------------
// *** TTVideoType: Video stream type
// -----------------------------------------------------------------------------
class TTVideoType : public TTAVTypes
{
 public:
  TTVideoType( QString f_name );
  ~TTVideoType();

  TTVideoStream* createVideoStream();

 protected:
  void getVideoStreamType();
};

#endif //TTAVTYPES_H


