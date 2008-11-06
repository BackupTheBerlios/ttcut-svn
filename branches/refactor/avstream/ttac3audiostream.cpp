/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttac3audiostream.cpp                                            */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 05/12/2005 */
/* MODIFIED: b. altendorf                                    DATE: 08/13/2005 */
/* MODIFIED: b. altendorf                                    DATE: 04/24/2007 */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTAC3AUDIOSTREAM
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


#include "ttac3audiostream.h"

#include "../data/ttcutlistdata.h"

const char c_name [] = "AC3STREAM";

// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// *** TTAC3AudioStream: AC3 audio stream class
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// constructor with file info and start position
// -----------------------------------------------------------------------------
  TTAC3AudioStream::TTAC3AudioStream( const QFileInfo &f_info, int s_pos )
: TTAudioStream( f_info, s_pos )
{
  log = TTMessageLogger::getInstance();
}

TTAC3AudioStream::~TTAC3AudioStream()
{
}

TTAVTypes::AVStreamType TTAC3AudioStream::streamType() const
{
  return TTAVTypes::ac3_audio;
}

// search the next sync byte in stream
// -----------------------------------------------------------------------------
void TTAC3AudioStream::searchNextSyncByte()
{
  quint8  byte1;
  quint8  byte2;
  quint16 sync_word;

  stream_buffer->readByte( byte2 );

  while ( !stream_buffer->atEnd() )
  {
    byte1 = byte2;

    stream_buffer->readByte( byte2 );

    sync_word = (byte1<<8) + byte2;

    if ( sync_word == 0x0B77 )
      break;
  }
}

// read audio header information from stream
// -----------------------------------------------------------------------------
void TTAC3AudioStream::readAudioHeader( TTAC3AudioHeader* audio_header)
{
  quint8* daten = new quint8[6];
  quint16 stuff;

  stream_buffer->readByte(daten, 6 );

  audio_header->setHeaderOffset( stream_buffer->position() - 8 ); // +Syncwort

  audio_header->crc1            = daten[0]<<8+daten[1];
  audio_header->fscod           = (quint8)((daten[2]&0xc0)>>6);
  audio_header->frmsizecod      = (quint8)(daten[2]&0x3f);
  audio_header->syncframe_words = AC3FrameLength[audio_header->fscod][audio_header->frmsizecod];
  audio_header->frame_length    = audio_header->syncframe_words*2;
  audio_header->frame_time      = 1000.0*((double)audio_header->syncframe_words*16.0)/(double)audio_header->bitRate();
  audio_header->bsid            = (quint8)(daten[3]>>3);
  audio_header->bsmod           = (quint8)(daten[3] & 0x07);
  audio_header->acmod           = (quint8)(daten[4]>>5);

  //qDebug( "%sbit rate    : %d",c_name,audio_header->bitRate() );
  //qDebug( "%sframe length: %d",c_name,audio_header->frame_length );
  frame_length = audio_header->frame_length;
  frame_time   = audio_header->frame_time;

  stuff = (quint16)((daten[4]<<8)+daten[5]);
  stuff <<= 3;

  if ((audio_header->acmod & 0x01)!=0 && audio_header->acmod!=1)
    stuff<<=2;
  if ((audio_header->acmod & 0x04)!=0)
    stuff<<=2;
  if (audio_header->acmod == 0x02)
    stuff<<=2;

  audio_header->lfeon=(stuff&0x8000)!=0;


}

// create the header list
// -----------------------------------------------------------------------------
int TTAC3AudioStream::createHeaderList()
{
  TTAC3AudioHeader* audio_header;
  TTAC3AudioHeader* prev_audio_header;

  //qDebug( "%screate header list...",c_name );

  header_list = new TTAudioHeaderList( 1000 );

  // seek to start pos for corrupt streams  
  stream_buffer->seekRelative( start_pos );

  try
  {
    while (!stream_buffer->atEnd())
    {
      searchNextSyncByte();

      audio_header = new TTAC3AudioHeader();

      readAudioHeader( audio_header );

      if ( header_list->count() == 0 )
        audio_header->abs_frame_time = 0.0;
      else
      {
        prev_audio_header = (TTAC3AudioHeader*)header_list->at(header_list->count()-1);
        audio_header->abs_frame_time = prev_audio_header->abs_frame_time
          +prev_audio_header->frame_time;
      }

      //qDebug( "%sabs frame time: %lf - %lf",c_name,audio_header->frame_time,audio_header->abs_frame_time );

      header_list->add( audio_header );

      stream_buffer->seekRelative(audio_header->syncframe_words*2-8);
}

  }
  catch (TTFileBufferException)
  {

  }

  // den Stream nach interessanten Punkten absuchen lassen:
  // base.CreateIndex(FileName);

 
  //qDebug( "%sheader list created: %d",c_name,header_list->count() );

  return header_list->count();
}

/* /////////////////////////////////////////////////////////////////////////////
 * Cut method
 */
void TTAC3AudioStream::cut(int start, int end, TTCutParameter* cp)
{
  quint64 start_offset;
  quint64 end_offset;

  start_offset = header_list->audioHeaderAt(start)->headerOffset();
  end_offset   = header_list->audioHeaderAt(end)->headerOffset()-1;

  //log->debugMsg(c_name, "copy segment: %lld - %lld", start_offset,end_offset );

  //openStream();
  copySegment(cp->getTargetStreamBuffer(), start_offset, end_offset );
  //closeStream();
}

/* /////////////////////////////////////////////////////////////////////////////
 * Cut AC3 audio stream
 */
void TTAC3AudioStream::cut( TTFileBuffer* cut_stream, TTCutListData* cut_list )
{
  int i;
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

#if defined(AC3STREAM_DEBUG)
  log->debugMsg(c_name, "-----------------------------------------------");
  log->debugMsg(c_name, ">>> cut audio stream                           ");
  log->debugMsg(c_name, "-----------------------------------------------");
  log->debugMsg(c_name, "entries in cut list: %d", cut_list->count());
  log->debugMsg(c_name, "target stream      : %s", cut_stream->fileName());
#endif

  for ( i = 0; i < cut_list->count(); i++ )
  {
    if (i == 0)
      cut_param->firstCall();

    start_pos = cut_list->cutInPosAt( i );
    end_pos   = cut_list->cutOutPosAt( i );


    //qDebug( "%sstart / end  : %d / %d",c_name,start_pos,end_pos );
    //search 
    video_frame_length = 1000.0 / 25.0; //TODO: replace with fps

    //qDebug( "%slocal audio offset: %f",c_name,local_audio_offset );

    audio_start_time = ((float)start_pos*video_frame_length+local_audio_offset)/frame_time;
    audio_start_index = (long)round(audio_start_time);

    if ( (int)audio_start_index < 0 )
      audio_start_index = 0;

    local_audio_offset = ((float)start_pos*video_frame_length)-
      (float)audio_start_index*frame_time+local_audio_offset;

    audio_end_time   = (((float)(end_pos+1)*video_frame_length-local_audio_offset)/frame_time-1.0);
    audio_end_index  = (long)round(audio_end_time);

    if (audio_end_index >= header_list->count())
      audio_end_index = header_list->count()-1;

    local_audio_offset = ((float)(audio_end_index+1)*frame_time)-
      ((float)(end_pos+1)*video_frame_length)+local_audio_offset;

#if defined(AC3STREAM_DEBUG)
    log->debugMsg(c_name, "audio frame length: %d", frame_length );
    log->debugMsg(c_name, "audio frame time  : %f", frame_time );
    log->debugMsg(c_name, "start - end       : %d | %d - %d",i+1, audio_start_index,audio_end_index );
    log->debugMsg(c_name, "saudio start/end   : %f/%f", audio_start_time,audio_end_time );
    log->debugMsg(c_name, "video length      : %f", (end_pos-start_pos+1)*video_frame_length );
    log->debugMsg(c_name, "audio length      : %f", (audio_end_index-audio_start_index+1)*frame_time );
#endif

    cut(audio_start_index, audio_end_index, cut_param);

    if (i == cut_list->count()-1)
      cut_param->lastCall();
  }

  delete cut_param;
}


// return the stream extension
// -----------------------------------------------------------------------------	
QString TTAC3AudioStream::streamExtension()
{
  QString extension = ".ac3";

  return extension;
}


QTime TTAC3AudioStream::streamLengthTime()
{
  QTime result(0, 0, 0);

  if (header_list == NULL || header_list->count() == 0)
    return result;

  TTAC3AudioHeader* audio_header = (TTAC3AudioHeader*)header_list->audioHeaderAt( header_list->count()-1 );
  result = ttMsecToTimeD(audio_header->abs_frame_time);

  return result;
}
