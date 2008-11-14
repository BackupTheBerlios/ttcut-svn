/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttmpegaudiostream.cpp                                           */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 05/12/2005 */
/* MODIFIED: b. altendorf                                    DATE: 06/11/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTMPEGAUDIOSTREAM
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

// -----------------------------------------------------------------------------
// TODO * video frame rate is here constant 25.0 fps (!) use variabel framerate
// TODO * check for audio delay
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


#include "ttmpegaudiostream.h"
#include "../data/ttcutlistdata.h"
#include "../data/ttavdata.h"

const char c_name[] = "MPEGAUDIOSTR";

// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// *** TTMPEGAudioStream
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// constructor with file info and start position
// -----------------------------------------------------------------------------
TTMPEGAudioStream::TTMPEGAudioStream( const QFileInfo &f_info, int s_pos )
  : TTAudioStream( f_info, s_pos)
{
  log = TTMessageLogger::getInstance();
}

TTMPEGAudioStream::~TTMPEGAudioStream()
{
}

TTAVTypes::AVStreamType TTMPEGAudioStream::streamType() const
{
  return TTAVTypes::mpeg_audio;
}

// search next sync byte in stream
// -----------------------------------------------------------------------------
void TTMPEGAudioStream::searchNextSyncByte()
{
  quint8  byte1;
  quint8  byte2;
  quint16 sync_word;

  stream_buffer->readByte( byte2 );

  while (!stream_buffer->atEnd() )
  {
    byte1 = byte2;
    stream_buffer->readByte( byte2 );

    sync_word = (byte1<<8) + byte2;

    if ((sync_word & 0xffe0) == 0xffe0)
    {
      stream_buffer->seekBackward(1);
      break;
    }
  }
}

// parse mpeg audio header data
// -----------------------------------------------------------------------------
void TTMPEGAudioStream::parseAudioHeader( quint8* data, int offset, TTMpegAudioHeader* audio_header )
{
  audio_header->version            = (data[offset] & 0x18) >> 3;
  audio_header->layer              = (data[offset] & 0x06) >> 1;
  audio_header->protection_bit     = (data[offset] & 0x01) == 1;

  audio_header->bitrate_index      = (data[offset+1] & 0xf0) >> 4;
  audio_header->sampling_frequency = (data[offset+1] & 0x0c) >> 2;
  audio_header->padding_bit        = (data[offset+1] & 0x02) == 2;
  audio_header->private_bit        = (data[offset+1] & 0x01) == 1;

  audio_header->mode               = (data[offset+2] & 0xc0) >> 6;
  audio_header->mode_extension     = (data[offset+2] & 0x30) >> 4;
  audio_header->copyright          = (data[offset+2] & 0x08) == 8;
  audio_header->original_home      = (data[offset+2] & 0x04) == 4;
  audio_header->emphasis           = (data[offset+2] & 0x03);

  switch (audio_header->version)
  {
  case 3: // Mpeg 1
    if (audio_header->layer == 3) // Layer I
      frame_length = (int)trunc((12*audio_header->bitRate()/audio_header->sampleRate()+audio_header->padding_bit)*4);
    else // Layer II, Layer III
      frame_length = (int)trunc(144*audio_header->bitRate()/audio_header->sampleRate()+audio_header->padding_bit);
    break;
  case 0: // Mpeg 2.5
  case 2: // Mpeg 2
    if (audio_header->layer==3) // Layer I
      frame_length = (int)trunc((6*audio_header->bitRate()/audio_header->sampleRate()+audio_header->padding_bit)*4);
    else // Layer II, Layer III
      frame_length = (int)trunc(72*audio_header->bitRate()/audio_header->sampleRate()+audio_header->padding_bit);
    break;
  default:
    log->errorMsg(c_name, "error parsing audio header (!)");
    frame_length = 0;
    frame_time   = (double)0.0;
    break;
  }

  if (frame_length > 0)
  {
    //log->infoMsg(c_name, "bit rate: %d",audio_header->bitRate() );
    if ( audio_header->bitRate() > 0 )
    {
      audio_header->frame_length = (int)frame_length;
      audio_header->frame_time   = (double)(audio_header->frame_length * 8000.0) / (double)audio_header->bitRate();
      frame_time                 = audio_header->frame_time;
      //log->infoMsg(c_name, "frame length/time: %d / %lf", frame_length, frame_time);
    }
    else
    {
      log->errorMsg(c_name, "bitrate error: bitrate is 0 (!)");
      audio_header->frame_length = (int)0;
      audio_header->frame_time   = (double)0.0;
    }
  }
}


// read one audio header from stream
// -----------------------------------------------------------------------------
void TTMPEGAudioStream::readAudioHeader( TTMpegAudioHeader* audio_header )
{
  quint8* data = new quint8[3];

  // read 3 byte from stream
  stream_buffer->readByte( data, 3 );

  // audio header offset
  audio_header->setHeaderOffset( stream_buffer->position() - 4 );

  // parse current audio header and fill header struct
  parseAudioHeader( data, 0, audio_header );

  delete []data;
}

// create the audio header list
// -----------------------------------------------------------------------------
int TTMPEGAudioStream::createHeaderList( )
{
  TTMpegAudioHeader* audio_header;
  TTMpegAudioHeader* prev_audio_header;

  // create new audio header list
  header_list = new TTAudioHeaderList( 1000 );

  // Fuer defekte Streams, auf die Startposition positionieren
  stream_buffer->seekAbsolute( (quint64)start_pos );

  emit progressChanged(TTProgressBar::Init, "Create audio-header list", stream_buffer->size());

  try
  {
    emit progressChanged(TTProgressBar::Start, "Create audio-header list", 0);

    while ( !stream_buffer->atEnd() )
    {
      searchNextSyncByte();
      audio_header = new TTMpegAudioHeader();

      // read and parse current audio header
      readAudioHeader( audio_header );

      // claculate the absolute frame time
      // -----------------------------------------------------------------------
      // first audio header: abs_frame_time = 0.0 (msec)
      if ( header_list->count() == 0 )
        audio_header->abs_frame_time = (double)0.0;
      else
      {
        // previous frame header
        prev_audio_header = (TTMpegAudioHeader*)header_list->at(header_list->count()-1);

        // absolute frame time for current frame in msec
        audio_header->abs_frame_time = prev_audio_header->abs_frame_time+
          prev_audio_header->frame_time;
      }

      // add audio header to header list
      header_list->add( audio_header );
      stream_buffer->seekRelative( audio_header->frame_length-4 );

      emit progressChanged(TTProgressBar::Step, "Create audio-header list", stream_buffer->position());
    }

    emit progressChanged(TTProgressBar::Finished, "Audio-header list created", stream_buffer->position());
  }
  catch (TTFileBufferException)
  {
  }

#if defined MPEGAUDIO_DEBUG
  log->infoMsg(c_name, "header list created: %d", header_list->count());
  log->infoMsg(c_name, "abs stream length:   %s", qPrintable(sreamLenghtTime().toString("hh:mm:ss")));
#endif

  return header_list->count();
}

void TTMPEGAudioStream::cut(int start, int end, TTCutParameter* cp)
{
  quint64 start_offset;
  quint64 end_offset;

  start_offset = header_list->audioHeaderAt(start)->headerOffset();
  end_offset   = header_list->audioHeaderAt(end)->headerOffset()-1;

  copySegment(cp->getTargetStreamBuffer(), start_offset, end_offset );
}


void TTMPEGAudioStream::cut( TTFileBuffer* cut_stream, TTCutListData* cut_list )
{
  TTCutParameter* cut_param = new TTCutParameter(cut_stream);
  long    start_pos;
  long    end_pos;
  QString action_string;
  float   video_frame_length;
  long    audio_start_index;
  long    audio_end_index;
  float   audio_start_time;
  float   audio_end_time;
  float   local_audio_offset = 0.0;
  int     audio_index;
  QList<TTAVStream*> connectedToProgressBar;
  connectedToProgressBar.append(this);

#if defined MPEGAUDIO_DEBUG
  log->infoMsg(c_name, "-----------------------------------------------");
  log->infoMsg(c_name, ">>> cut audio stream                           ");
  log->infoMsg(c_name, "-----------------------------------------------");
  log->infoMsg(c_name, "entries in cut list: %d", cut_list->count());
  log->infoMsg(c_name, "target stream      : %s", qPrintable(cut_stream->fileName()));
#endif

  audio_index = cut_list->avData(0)->indexOfAudioStream( this );
  for (int i = 0; i < cut_list->count(); i++)
  {
    if (i == 0)
      cut_param->firstCall();

    start_pos = cut_list->cutInPosAt( i );
    end_pos   = cut_list->cutOutPosAt( i );
    TTAudioStream* pAudioStream = cut_list->avData(i)->audioStream(audio_index);

    if ( -1 == connectedToProgressBar.indexOf(pAudioStream) ) {
      connect( pAudioStream, SIGNAL(progressChanged(TTProgressBar::State, const QString&, quint64)),
               this, SIGNAL(progressChanged(TTProgressBar::State, const QString&, quint64)) );
      connectedToProgressBar.append(pAudioStream);
    }

    //qDebug( "%sstart / end  : %d / %d",c_name,start_pos,end_pos );
    //search
    TTMpegAudioHeader* audio_header = (TTMpegAudioHeader*)pAudioStream->headerList()->audioHeaderAt(0);
    frame_time = audio_header->frame_time;

    video_frame_length = 1000.0 / 25.0; //TODO: replace with fps

    //qDebug( "%slocal audio offset: %f",c_name,local_audio_offset );

    audio_start_time  = ((float)start_pos*video_frame_length+local_audio_offset)/frame_time;
    audio_start_index = ((long)round(audio_start_time) >= 0)
          ? (long)round(audio_start_time)
          : 0;
    local_audio_offset = ((float)start_pos*video_frame_length) - (float)audio_start_index*frame_time+local_audio_offset;
    audio_end_time     = (((float)(end_pos+1)*video_frame_length-local_audio_offset)/frame_time-1.0);
    audio_end_index    = ((long)round(audio_end_time) < pAudioStream->headerList()->count())
          ? (long)round(audio_end_time)
          : pAudioStream->headerList()->count()-1;
    local_audio_offset = ((float)(audio_end_index+1)*frame_time)-
      ((float)(end_pos+1)*video_frame_length)+local_audio_offset;

#if defined MPEGAUDIO_DEBUG
    log->infoMsg( c_name, "audio frame length: %d",frame_length );
    log->infoMsg( c_name, "audio frame time  : %f",frame_time );
    log->infoMsg( c_name, "start - end       : %d | %d - %d",i+1,audio_start_index,audio_end_index );
    log->infoMsg( c_name, "local audio offset: %f", local_audio_offset);
    log->infoMsg( c_name, "audio start/end   : %f/%f",audio_start_time,audio_end_time );
    log->infoMsg( c_name, "video length      : %f",(end_pos-start_pos+1)*video_frame_length );
    log->infoMsg( c_name, "audio length      : %f",(audio_end_index-audio_start_index+1)*frame_time );
#endif

    pAudioStream->cut(audio_start_index, audio_end_index, cut_param);

    if ( i == cut_list->count()-1 )
      cut_param->lastCall();
  }
  delete cut_param;
}

// return the stream extension
// -----------------------------------------------------------------------------
QString TTMPEGAudioStream::streamExtension()
{
  return (ttAssigned( stream_info))
        ? stream_info->suffix().toLower()
        : ".mp2";
}


QTime TTMPEGAudioStream::streamLengthTime()
{
  QTime result(0, 0, 0);

  if (header_list == NULL || header_list->count() == 0)
    return result;

  TTMpegAudioHeader* audio_header = (TTMpegAudioHeader*)header_list->audioHeaderAt( header_list->count()-1 );
  result = ttMsecToTimeD( audio_header->abs_frame_time );

  return result;
}


int TTMPEGAudioStream::searchIndex( double s_time )
{
  return (ttAssigned( header_list ))
      ? header_list->searchTimeIndex( s_time )
      : -1;
}
