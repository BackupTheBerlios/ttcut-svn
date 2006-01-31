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


#include "ttmpegaudiostream.h"

const char c_name[] = "MPEGAUDIOSTR  : ";

// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// *** TTMPEGAudioStream
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// constructor
// -----------------------------------------------------------------------------
TTMPEGAudioStream::TTMPEGAudioStream()
  : TTAudioStream()
{

}

// constructor with file info and start position
// -----------------------------------------------------------------------------
TTMPEGAudioStream::TTMPEGAudioStream( const QFileInfo &f_info, int s_pos )
  : TTAudioStream( f_info, s_pos)
{

}

// search next sync byte in stream
// -----------------------------------------------------------------------------
void TTMPEGAudioStream::searchNextSyncByte()
{
  uint8_t  byte1;
  uint8_t  byte2;
  uint16_t sync_word;

  stream_buffer->readByte( byte2 );

  while (!stream_buffer->streamEOF() )
  {
    byte1 = byte2;
    stream_buffer->readByte( byte2 );

    sync_word = (byte1<<8) + byte2;

    if ((sync_word & 0xffe0) == 0xffe0)
    {
      stream_buffer->seekRelative(-1);
      break;
    }
  }
}

// parse mpeg audio header data
// -----------------------------------------------------------------------------
void TTMPEGAudioStream::parseAudioHeader( uint8_t* data, int offset, TTMpegAudioHeader* audio_header )
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
    qDebug( "%serror parsing audio header (!)",c_name );
    frame_length = 0;
    frame_time   = (double)0.0;
    break;
  }

  if (frame_length > 0)
  {
    //qDebug( "%sbit rate: %d",c_name,audio_header->bitRate() );
    if ( audio_header->bitRate() > 0 )
    {
      audio_header->frame_length = (int)frame_length;
      audio_header->frame_time   = (double)(audio_header->frame_length * 8000.0) / (double)audio_header->bitRate();
      frame_time                 = audio_header->frame_time;
      //qDebug( "%sframe length/time: %d / %lf", c_name,frame_length,frame_time );
    }
    else
    {
      qDebug( "%sbitrate error: bitrate is 0 (!)",c_name );
      audio_header->frame_length = (int)0;
      audio_header->frame_time   = (double)0.0;
    }
  }
}


// read one audio header from stream
// -----------------------------------------------------------------------------
void TTMPEGAudioStream::readAudioHeader( TTMpegAudioHeader* audio_header )
{
  uint8_t* data = new uint8_t[3];

  // read 3 byte from stream
  stream_buffer->readArray( data, 3 );

  // audio header offset
  audio_header->setHeaderOffset( stream_buffer->currentOffset() - 4 );

  // parse current audio header and fill header struct
  parseAudioHeader( data, 0, audio_header );
}

// create the audio header list
// -----------------------------------------------------------------------------
int TTMPEGAudioStream::createHeaderList( )
{
  TTMpegAudioHeader* audio_header;
  TTMpegAudioHeader* prev_audio_header;

  // if file already parsed return
  if ( stream_parsed )
  {
    if ( ttAssigned( header_list ) )
      return header_list->count();
    else
      return (int)0;
  }

  // open the audio stream
  // open the audio stream
  if ( !openStream() )  
  {
    //qDebug( "%scannot open audio stream: %s",c_name,stream_info->filePath().toAscii() );
    return (int)0;
  }

  // stream length
  stream_length_bytes = stream_buffer->streamLength();

  // create new audio header list
  header_list = new TTAudioHeaderList( 1000 );

  // Fuer defekte Streams, auf die Startposition positionieren
  stream_buffer->seekAbsolute( (off64_t)start_pos );

  if ( ttAssigned( progress_bar ) )
  {
    progress_bar->setActionText( "Create audio header list." );
    progress_bar->setTotalSteps( stream_buffer->streamLength() );
  }

  try
  {
    while ( !stream_buffer->streamEOF() )  
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
	// prvious frame header
	prev_audio_header = (TTMpegAudioHeader*)header_list->at(header_list->count()-1);
	
	// absolute frame time for current frame in msec
	audio_header->abs_frame_time = prev_audio_header->abs_frame_time+
	  prev_audio_header->frame_time;
      }
      
      // add audio header to header list
      header_list->add( audio_header );
      
      stream_buffer->seekRelative( audio_header->frame_length-4 );

      if ( ttAssigned(progress_bar) )
	progress_bar->setProgress( stream_buffer->currentOffset() );

    }
    progress_bar->setComplete();
  }
  catch ( TTStreamEOFException )
  {
  }
  
  // close the stream
  closeStream();

  // TODO: looking for interesting stream points, like mode changes etc.  
  
  //qDebug( "%sheader list created: %d",c_name,header_list->count() );
  
  return header_list->count();
}

void TTMPEGAudioStream::cut( TTFileBuffer* cut_stream, int start, int end,__attribute__ ((unused)) TTCutParameter* cp )
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


void TTMPEGAudioStream::cut( TTFileBuffer* cut_stream, TTAVCutList* cut_list )
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
      action_string.sprintf( "Audio cut: %d/%ld-%ld",i+1,start_pos,end_pos );
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
QString TTMPEGAudioStream::streamExtension()
{
  QString extension = ".mp2";

  if ( ttAssigned( stream_info) )
    return stream_info->suffix().toLower();
  else
    return extension;
}


QString TTMPEGAudioStream::absStreamTime()
{
  if ( ttAssigned( header_list ) )
  {
    if ( header_list->count() > (int)0 )
    {
      TTMpegAudioHeader* audio_header = (TTMpegAudioHeader*)header_list->audioHeaderAt( header_list->count()-1 );

      return ttMsecToTimeD( audio_header->abs_frame_time ).toString("hh:mm:ss.zzz" );
    }
  }
  // TODO: default return value
  return QString("");
}


int TTMPEGAudioStream::searchIndex( double s_time )
{
  if ( ttAssigned( header_list ) )
    return header_list->searchTimeIndex( s_time );
  else
    return -1;
}
