/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttavtypes.cpp                                                   */
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

#include "ttavtypes.h"

#include "ttac3audiostream.h"
#include "ttac3audioheader.h"
#include "ttmpegaudiostream.h"
#include "ttmpegaudioheader.h"
#include "ttmpeg2videostream.h"
#include "../common/ttexception.h"

#include <QString>
#include <qfileinfo.h>

const char c_name [] = "TTAVTYPES     : ";

// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// TTAVTypes: Base class for AV stream types
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// construct a TTAVTypes object
// -----------------------------------------------------------------------------
TTAVTypes::TTAVTypes( QString f_name )
{
  av_stream_info     = new QFileInfo( f_name );
  av_stream_exists   = av_stream_info->exists();
  av_stream_type     = unknown;
  av_stream          = NULL;
  type_header_offset = 0;
  type_header_length = 0;

  if (!av_stream_info->exists()) {
	  qDebug("throw IOException in AVTypes....");
	  QString msg = QString("AV stream %1 does not exist").arg(f_name);
	  throw TTIOException(msg);
  }
}

// destructor
// -----------------------------------------------------------------------------
TTAVTypes::~TTAVTypes()
{
  if (av_stream_info != NULL) {
    delete av_stream_info;
  }


  if ( av_stream != NULL ) {
    //FIXME: Why here is a file buffer????
    av_stream->close();
    delete av_stream;
  }
}

// return current stream QFileInfo
// -----------------------------------------------------------------------------
QFileInfo& TTAVTypes::avStreamInfo()
{
  return *av_stream_info;
}

// return current stream type
// -----------------------------------------------------------------------------
TTAVTypes::AVStreamType TTAVTypes::avStreamType()
{
  return av_stream_type;
}

// return stream type header offset
// -----------------------------------------------------------------------------
quint64 TTAVTypes::typeHeaderOffset()
{
  return type_header_offset;
}

// return stream type header length
// -----------------------------------------------------------------------------
long TTAVTypes::typeHeaderLength()
{
  return type_header_length;
}


// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// *** TTAudioType: Audio-stream type
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// construct TTAudioType object
// -----------------------------------------------------------------------------
TTAudioType::TTAudioType( QString f_name )
  : TTAVTypes( f_name )
{
  start_pos = 0;

  // if audio file exists get audio stream type	
  if ( av_stream_exists )
  {
    getAudioStreamType();
  }
}

// destructor
// -----------------------------------------------------------------------------
 TTAudioType::~TTAudioType()
{

}

// create audio stream object according to stream type
// -----------------------------------------------------------------------------
TTAudioStream* TTAudioType::createAudioStream()
{
  switch ( av_stream_type )
  {
  case ac3_audio:
    return new TTAC3AudioStream( *av_stream_info, start_pos );
  case mpeg_audio:
    return new TTMPEGAudioStream( *av_stream_info, start_pos );
 
  default:
    qDebug( "%sunknown audio stream",c_name );
    return (TTAudioStream*)NULL;
  }
}

// evaluate audio stream and estimate audio stream type
// -----------------------------------------------------------------------------
void TTAudioType::getAudioStreamType()
{
  int     count  = 0;
  quint8* buffer = new quint8[65536];
  quint16 sync_word;

#if defined(TTAVTYPES_DEBUG)
  qDebug( "%s-----------------------------------------------",c_name );
  qDebug( "%s>>> get audio stream type",c_name );
  qDebug( "%s-----------------------------------------------",c_name );
#endif

  av_stream_type  = unknown;
  start_pos       = 0;

  // open audio-stream for reading
  // FIXME: av_stream seems to be local
  av_stream = new TTFileBuffer(av_stream_info->filePath(), QIODevice::ReadOnly );

  // read buffer from stream
  count = av_stream->readByte( buffer, 65536 );

  // --------------------------------------------------------------------------
  // analyze buffer
  // --------------------------------------------------------------------------
  for ( start_pos = 0; start_pos < count-1; start_pos++ )
  {
    sync_word = (uint)((buffer[start_pos]<<8)+buffer[start_pos+1]);

    //qDebug( "%s>>> sync_word: %04x",c_name,sync_word );

    // AC3 audio type
    // -------------------------------------------------------------------------
    if ( sync_word == 0x0b77 )
    {
      //qDebug( "%sfound AC3 sync word: %d",c_name,start_pos );

      frame_size_code = buffer[start_pos+4] & 0x3F;
      sample_rate     = (buffer[start_pos+4] & 0xC0)>>6;
      frame_len       = 2*AC3FrameLength[sample_rate][frame_size_code];

      //qDebug( "%sAC3 frame size code: %d",c_name,frame_size_code );
      //qDebug( "%sAC3 sample rate    : %d",c_name,sample_rate );
      //qDebug( "%sAC3 frame len      : %d",c_name,frame_len );

      if ( frame_len > 0           && 
          start_pos+frame_len+1 < count && 
          (buffer[start_pos+frame_len]<<8)+buffer[start_pos+1+frame_len] == 0x0b77 )
      {
        av_stream_type = ac3_audio;
        break;
      }
    }

    // MPEG audio type
    // -------------------------------------------------------------------------
    if ( (sync_word & 0xffe0) == 0xffe0 )
    {
      //qDebug( "%sFound MPEG audio sync word: %d",c_name,start_pos );

      TTMpegAudioHeader* mpeg_header = new TTMpegAudioHeader();
      TTMPEGAudioStream* mpeg_stream = new TTMPEGAudioStream(*av_stream_info, 0);

      mpeg_stream->parseAudioHeader( buffer, start_pos+1, mpeg_header );

      //qDebug( "%sframe length   : %d",c_name,mpeg_stream->frameLength() );
      //qDebug( "%ssync word      : %04x",c_name,((buffer[start_pos+mpeg_stream->frameLength()]<<8)+buffer[start_pos+1+mpeg_stream->frameLength()]) & 0xFFE0);

      if ( mpeg_header->frameLength() > 0 && 
          start_pos+mpeg_header->frameLength()+1 < count && 
          // next sync_word is MPEG audio
          (((buffer[start_pos+mpeg_header->frameLength()]<<8)+buffer[start_pos+1+mpeg_header->frameLength()]) & 0xFFE0) == 0xFFE0 )
      {
        //qDebug( "%sfound MPEG audio...",c_name );
        av_stream_type = mpeg_audio;

        if (mpeg_stream != NULL)
          delete mpeg_stream;

        if (mpeg_header != NULL)
          delete mpeg_header;
        
        break;
      }
    }
  }

  if (av_stream != NULL) {
    av_stream->close();
    delete av_stream;
    av_stream = NULL;
  }

  delete [] buffer;

  //qDebug( "%s-----------------------------------------------",c_name );
}

// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// *** TTVideoType: Video stream type
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// construct TTVideoType object
// -----------------------------------------------------------------------------
  TTVideoType::TTVideoType( QString f_name )
: TTAVTypes( f_name )
{
  // if video file exists get video stream type
  if ( av_stream_exists )
  {
    getVideoStreamType();
  }
}

// destructor
// -----------------------------------------------------------------------------
TTVideoType::~TTVideoType()
{

}

// create video stream object according to video stream type
// -----------------------------------------------------------------------------
TTVideoStream* TTVideoType::createVideoStream()
{
  return (av_stream_exists)
      ? new TTMpeg2VideoStream(*av_stream_info)
      : (TTVideoStream*)NULL;
}

// evaluate video stream and estimate video stream type
// currently we use only mpeg2 video, so this is a simple task
void TTVideoType::getVideoStreamType()
{
  av_stream_type = mpeg2_demuxed_video;
}




