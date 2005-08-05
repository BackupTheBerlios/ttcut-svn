/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttac3audiostream.cpp                                            */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 05/12/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTAC3AUDIOSTREAM
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


#include "ttac3audiostream.h"

const char c_name [] = "AC3STREAM     : ";

// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// *** TTAC3AudioStream: AC3 audio stream class
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// constructor
// -----------------------------------------------------------------------------
TTAC3AudioStream::TTAC3AudioStream()
  : TTAudioStream()
{

}

// constructor with file info and start position
// -----------------------------------------------------------------------------
TTAC3AudioStream::TTAC3AudioStream( const QFileInfo &f_info, int s_pos )
  : TTAudioStream( f_info, s_pos )
{

}

// search the next sync byte in stream
// -----------------------------------------------------------------------------
void TTAC3AudioStream::searchNextSyncByte()
{
  uint8_t  byte1;
  uint8_t  byte2;
  uint16_t sync_word;

  stream_buffer->readByte( byte2 );

  while ( !stream_buffer->streamEOF() )
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
  uint8_t* daten = new uint8_t[6];
  uint16_t stuff;
  
  stream_buffer->readArray( daten, 6 );

  audio_header->setHeaderOffset( stream_buffer->currentOffset() - 8 ); // +Syncwort

  audio_header->crc1            = daten[0]<<8+daten[1];
  audio_header->fscod           = (uint8_t)((daten[2]&0xc0)>>6);
  audio_header->frmsizecod      = (uint8_t)(daten[2]&0x3f);
  audio_header->syncframe_words = AC3FrameLength[audio_header->fscod][audio_header->frmsizecod];
  audio_header->frame_length    = audio_header->syncframe_words*2;
  audio_header->frame_time      = 1000.0*((double)audio_header->syncframe_words*16.0)/(double)audio_header->bitRate();
  audio_header->bsid            = (uint8_t)(daten[3]>>3);
  audio_header->bsmod           = (uint8_t)(daten[3] & 0x07);
  audio_header->acmod           = (uint8_t)(daten[4]>>5);

  //qDebug( "%sbit rate    : %d",c_name,audio_header->bitRate() );
  //qDebug( "%sframe length: %d",c_name,audio_header->frame_length );
  frame_length = audio_header->frame_length;
  frame_time   = audio_header->frame_time;

  stuff = (uint16_t)((daten[4]<<8)+daten[5]);
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
uint TTAC3AudioStream::createHeaderList()
{
  TTAC3AudioHeader* audio_header;
  TTAC3AudioHeader* prev_audio_header;

  //qDebug( "%screate header list...",c_name );

  // stream already parsed return
  if ( stream_parsed )
  {
    if ( ttAssigned( header_list ) )
      return header_list->count();
    else
      return (uint)0;
  }

  // open the audio stream
  if ( !openStream() )
  {
    qDebug( "%scannot open audio stream: %s",c_name,stream_info->filePath().ascii() );
    return (uint)0;
  }

  header_list = new TTAudioHeaderList( 1000 );

  // seek to start pos for corrupt streams  
  stream_buffer->seekRelative( start_pos );
    
  try
  {
    while (!stream_buffer->streamEOF())
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
  catch ( TTStreamEOFException )
  {

  }

  // den Stream nach interessanten Punkten absuchen lassen:
  // base.CreateIndex(FileName);

  // close the audio stream
  closeStream();

  //qDebug( "%sheader list created: %d",c_name,header_list->count() );

  return header_list->count();
}

void TTAC3AudioStream::cut( TTFileBuffer* cut_stream, uint start, uint end, TTCutParameter* cp )
{
  off64_t start_offset;
  off64_t end_offset;

  start_offset = header_list->audioHeaderAt(start)->headerOffset();
  end_offset   = header_list->audioHeaderAt(end)->headerOffset()-1;

  //qDebug( "%scopy segment: %lld - %lld",c_name,start_offset,end_offset );

  openStream();
  copySegment( cut_stream, start_offset, end_offset );
  closeStream();
}

void TTAC3AudioStream::cut( TTFileBuffer* cut_stream, TTAVCutList* cut_list )
{
  int i;
  TTCutParameter* cut_param = new TTCutParameter();
  long    start_pos;
  long    end_pos;
  QString action_string;
  float   video_frame_length;
  long    audio_start_index;
  long    audio_end_index;
  float   audio_start_time;
  float   audio_end_time;
  float   local_audio_offset = 0.0;

  //qDebug( "%s-----------------------------------------------",c_name );
  //qDebug( "%s>>> cut audio stream                           ",c_name );
  //qDebug( "%s-----------------------------------------------",c_name );
  //qDebug( "%sentries in cut list: %d",c_name,cut_list->count() );
  //qDebug( "%starget stream      : %s",c_name,cut_stream->fileName() );

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
      action_string.sprintf( "Audio cut: %d/%d-%d",i+1,start_pos,end_pos );
      progress_bar->setActionText( action_string );
    }

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

    local_audio_offset = ((float)(audio_end_index+1)*frame_time)-
      ((float)(end_pos+1)*video_frame_length)+local_audio_offset;

    //qDebug( "%saudio frame length: %d",c_name,frame_length );
    //qDebug( "%saudio frame time  : %f",c_name,frame_time );
    //qDebug( "%sstart - end       : %d | %d - %d",c_name,i+1,audio_start_index,audio_end_index );

    //qDebug( "%saudio start/end   : %f/%f",c_name,audio_start_time,audio_end_time );
    //qDebug( "%svideo length      : %f",c_name,(end_pos-start_pos+1)*video_frame_length );
    //qDebug( "%saudio length      : %f",c_name,(audio_end_index-audio_start_index+1)*frame_time );

    cut( cut_stream, audio_start_index, audio_end_index, cut_param );
  }
  //qDebug( "%s-----------------------------------------------",c_name );
  delete cut_param;
}


// return the stream extension
// -----------------------------------------------------------------------------	
QString TTAC3AudioStream::streamExtension()
{
  QString extension = ".ac3";

  return extension;
}


QString TTAC3AudioStream::absStreamTime()
{
  if ( ttAssigned( header_list ) )
  {
    if ( header_list->count() > (uint)0 )
    {
      TTAC3AudioHeader* audio_header = (TTAC3AudioHeader*)header_list->audioHeaderAt( header_list->count()-1 );

      return ttMsecToTimeD( audio_header->abs_frame_time ).toString("hh:mm:ss.zzz" );
    }
  }
}
