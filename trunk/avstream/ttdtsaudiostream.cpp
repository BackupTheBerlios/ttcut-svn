/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttdtsaudiostream.cpp                                            */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 05/12/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTDTSAUDIOSTREAM (abstract)
// TTDTS14AUDIOSTREAM
// TTDTS16AUDIOSTREAM
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


#include "ttdtsaudiostream.h"

// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// *** TTDTSAudioStream: Base class for DTS audio stream objects
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// default constructor
// -----------------------------------------------------------------------------
TTDTSAudioStream::TTDTSAudioStream()
  : TTAudioStream()
{

}

// constructor with file info and start position
// -----------------------------------------------------------------------------
TTDTSAudioStream::TTDTSAudioStream( const QFileInfo &f_info, int s_pos )
  : TTAudioStream( f_info, s_pos )
{

}

// return stream extension
// -----------------------------------------------------------------------------
QString TTDTSAudioStream::streamExtension()
{
  QString extension = ".dts";

  return extension;
}

// create header list
// -----------------------------------------------------------------------------
uint TTDTSAudioStream::createHeaderList( )
{
  TTDTSAudioHeader* audio_header;
  TTDTSAudioHeader* prev_audio_header;

  // if stream already parsed return
  if ( stream_parsed && ttAssigned( header_list ) )
    return header_list->count();

  header_list = new TTAudioHeaderList( 1000 );

  // for corrupt stream seek to calculated start position
  stream_buffer->seekRelative( start_pos);

  while ( !stream_buffer->streamEOF() )
  {
    searchNextSyncWord();

    audio_header = new TTDTSAudioHeader( audio_mode );

    readAudioHeader( audio_header );

    if ( header_list->count() == 0 )
      audio_header->abs_frame_time = 0;
    else
    {
      prev_audio_header = (TTDTSAudioHeader*)header_list->at(header_list->count()-1);
      audio_header->abs_frame_time = prev_audio_header->abs_frame_time
	+prev_audio_header->frame_time;
    }

    header_list->add( audio_header);
  }

  // den Stream nach interessanten Punkten absuchen lassen:
  // base.CreateIndex(FileName);

  return header_list->count();
}

// assign header values from stream data
// -----------------------------------------------------------------------------
void TTDTSAudioStream::assignValues( TTDTSAudioHeader* audio_header, uint8_t* source, int start_pos )
{
  audio_header->FTYPE =(source[start_pos]&0x80)!=0;
  audio_header->SHORT =(uint8_t)((source[start_pos]&0x7c)>>2);
  audio_header->CPF   =(source[start_pos]&0x02)!=0;
  audio_header->NBLKS =(uint8_t)(((source[start_pos]&0x01)<<6) | ((source[start_pos+1]&0xfc)>>2));
  audio_header->FSIZE =((source[start_pos+1]&0x03)<<12) | (source[start_pos+2]<<4) | ((source[start_pos+3]&0xf0)>>4);
  audio_header->AMODE =(uint8_t)(((source[start_pos+3]&0x0f)<<2)|((source[start_pos+4]&0xc0)>>6));
  audio_header->SFREQ =(uint8_t)((source[start_pos+4]&0x3c)>>2);
  audio_header->RATE  =(uint8_t)(((source[start_pos+4]&0x03)<<3)|((source[start_pos+5]&0xe0)>>5));

  audio_header->frame_length = audio_header->FSIZE+1;
  audio_header->frame_time   = 1000*((audio_header->NBLKS+1)<<5)/audio_header->sample_rate;
}

// transform header data from DTS14 to DTS16
// -----------------------------------------------------------------------------
uint8_t* TTDTSAudioStream::fourteenToSixteen( uint8_t* org, int start, int word_count)
{
  int      bitp = 0;
  uint32_t buffer_word = 0;
  uint16_t write_word;

  TTMemoryBuffer* mem_buffer = new TTMemoryBuffer( org, start, 2*word_count );
  TTMemoryBuffer* mem_buffer_result = new TTMemoryBuffer( 2*word_count );

  for ( int i = 0; i < word_count; i++ )
  {
    buffer_word = (buffer_word<<14)|(uint32_t)(mem_buffer->readUInt16()&0x3fff);
    bitp += 14;

    if ( bitp >= 16 )
    {
      buffer_word<<=32-bitp;
      write_word = (uint16_t)(buffer_word>>16);
      mem_buffer_result->writeByte((uint8_t)(write_word/256));
      mem_buffer_result->writeByte((uint8_t)(write_word%256));
      buffer_word &= 65536; //UInt16.MaxValue;
      bitp -= 16;
      buffer_word>>=16-bitp;
    }
  }
  if ( bitp != 0 )
  {
    mem_buffer_result->writeByte((uint8_t)(buffer_word/256));
    mem_buffer_result->writeByte((uint8_t)(buffer_word%256));
  }
  return mem_buffer_result->toArray();
}

// read audio header (virtual)
// -----------------------------------------------------------------------------
void TTDTSAudioStream::readAudioHeader( TTDTSAudioHeader* audio_header )
{

}

// search next sync word (virtual)
// -----------------------------------------------------------------------------
void TTDTSAudioStream::searchNextSyncWord()
{

}


// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// *** TTDTS14AudioStream
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// constructor
// -----------------------------------------------------------------------------
TTDTS14AudioStream::TTDTS14AudioStream()
  : TTDTSAudioStream()
{

}

// constructor with file info and start position
// -----------------------------------------------------------------------------
TTDTS14AudioStream::TTDTS14AudioStream( const QFileInfo &f_info, int s_pos )
  : TTDTSAudioStream( f_info, s_pos )
{

}

// read header data from stream
// -----------------------------------------------------------------------------
void TTDTS14AudioStream::readAudioHeader( TTDTSAudioHeader* audio_header )
{
  uint8_t* ar = new uint8_t[14];

  // seek 6 byte backward because of syncword
  stream_buffer->seekRelative(-6);
  audio_header->setHeaderOffset( stream_buffer->currentOffset() );

  stream_buffer->readArray( ar,14 );

  assignValues( audio_header, fourteenToSixteen( ar,0,7 ), 4 );

  audio_header->frame_length = audio_header->frame_length*16/14;

  stream_buffer->seekRelative( audio_header->frame_length-14);

  delete [] ar;
}

// search next sync word
// -----------------------------------------------------------------------------
void TTDTS14AudioStream::searchNextSyncWord()
{
  uint8_t* int14 = new uint8_t[6];
  uint8_t* int16;

  stream_buffer->readArray( int14, 6 );

  while ( !stream_buffer->streamEOF() )
  {
    int16 = fourteenToSixteen( int14,0,3 );

    if ( int16[0] == 0x7f && int16[1] == 0xfe && int16[2] == 0x80 && int16[3] == 0x01 )
      break;

    for ( int i = 0; i < 5; ++i )
      int14[i]=int14[i+1];

    stream_buffer->readByte(int14[5]);
  }

  delete [] int14;	
}



// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// *** TTDTS16AudioStream
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// constructor
// -----------------------------------------------------------------------------
TTDTS16AudioStream::TTDTS16AudioStream()
  : TTDTSAudioStream()
{

}

// constructor with file info and start position
// -----------------------------------------------------------------------------
TTDTS16AudioStream::TTDTS16AudioStream( const QFileInfo &f_info, int s_pos )
  : TTDTSAudioStream( f_info, s_pos )
{

}

// read header data from stream
// -----------------------------------------------------------------------------
void TTDTS16AudioStream::readAudioHeader( TTDTSAudioHeader* audio_header )
{
  uint8_t* ar = new uint8_t[6];

  stream_buffer->readArray( ar,6 );

  audio_header->setHeaderOffset( stream_buffer->currentOffset() - 10 ); // +Syncword

  assignValues( audio_header, ar, 0);

  stream_buffer->seekRelative( audio_header->frame_length-10 );

  delete [] ar;
}

// search next sync word
// -----------------------------------------------------------------------------
void TTDTS16AudioStream::searchNextSyncWord()
{
  uint64_t sync = 0;
  uint8_t  byte1;

  for ( int i = 0; i < 4 && !stream_buffer->streamEOF(); i++ )
  {
    stream_buffer->readByte( byte1 );
    sync=(sync<<8)|byte1;
  }

  while ( !stream_buffer->streamEOF() )
  {
    if ( sync == 0x7ffe8001 )
      break;

    stream_buffer->readByte( byte1 );
    sync=(sync<<8)|byte1;
  }
}


