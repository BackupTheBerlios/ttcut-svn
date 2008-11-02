/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttpcmaudiostream.cpp                                            */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 05/12/2005 */
/* MODIFIED: b. altendorf                                    DATE: 06/10/2005 */
/* MODIFIED: b. altendorf                                    DATE: 08/13/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTPCMAUDIOSTREAM
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


#include "ttpcmaudiostream.h"
	
const uint32_t TTPCMAudioStream::ChunkID       = 0x46464952;  // RIFF
const uint32_t TTPCMAudioStream::Format        = 0x45564157;  // WAVE
const uint32_t TTPCMAudioStream::Subchunk1ID   = 0x20746d66;  // 'fmt '
const uint32_t TTPCMAudioStream::Subchunk1Size = 16;          // length for PCM
const uint16_t TTPCMAudioStream::AudioFormat   = 1;           // PCM
const uint32_t TTPCMAudioStream::Subchunk2ID   = 0x61746164;  // 'data';

const char c_name [] = "PCMAUDIOSTREAM: ";

// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// TTPCMAudioStream
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// default constructor
// -----------------------------------------------------------------------------
TTPCMAudioStream::TTPCMAudioStream()
  : TTAudioStream()
{
  start_delay = 0;
}

// constructor with file info and start position
// -----------------------------------------------------------------------------
TTPCMAudioStream::TTPCMAudioStream( const QFileInfo &f_info, int s_pos )
  : TTAudioStream( f_info, s_pos )
{
  start_delay = 0;
}


// liefert (falls vorhanden) das erste Byte hinter der ChunkID!
// -----------------------------------------------------------------------------
int TTPCMAudioStream::findChunk( uint32_t chunk_name, uint8_t* buffer, int buf_size )
{
  uint32_t test_value;
  uint32_t offset;

  qDebug( "%sfind chunk...",c_name );

  TTMemoryBuffer* mem_buffer = new TTMemoryBuffer( buffer );
  
  if ( mem_buffer->readUInt32() != ChunkID ) // 'RIFF'
    return -1;

  mem_buffer->seek( 4, TTMemoryBuffer::current );

  if ( mem_buffer->readUInt32() != Format )  // 'WAVE'
    return -1;

  do
  {
    test_value = mem_buffer->readUInt32();

    if ( test_value != chunk_name ) // Ist das der gewuenschte Chunk?
    {
      offset = mem_buffer->readUInt32();
      mem_buffer->seek( offset, TTMemoryBuffer::current );
    }
  } while ( test_value != chunk_name || mem_buffer->currentPosition() >= buf_size );

  if ( mem_buffer->currentPosition() >= buf_size )
    return -1;
  else
    return (int)mem_buffer->currentPosition();
}

// create header list
// -----------------------------------------------------------------------------
int TTPCMAudioStream::createHeaderList()
{
  int len = 0;
  int pos;

  // if buffer already parsed return
  if ( stream_parsed && ttAssigned( header_list ) )
    return header_list->count();

  // WAV isn't packet oriented; header list wasn't created
  header_list = (TTAudioHeaderList*)NULL;

  uint8_t* temp_buffer = new uint8_t [65536];

  len = stream_buffer->readArray( temp_buffer, 0, 65536 );

  TTMemoryBuffer* mem_buffer = new TTMemoryBuffer( temp_buffer );

  pos = findChunk( Subchunk1ID, temp_buffer, len );

  mem_buffer->seek( pos, TTMemoryBuffer::begin );
  mem_buffer->readUInt32(); // Subchunk1Size
  mem_buffer->readUInt16(); // AudioFormat
  num_channels    = mem_buffer->readUInt16();
  sample_rate     = mem_buffer->readUInt32();
  byte_rate       = mem_buffer->readUInt32();
  block_align     = mem_buffer->readUInt16();
  bits_per_sample = mem_buffer->readUInt16();
  
  pos = findChunk( Subchunk2ID, temp_buffer, len );
  mem_buffer->seek( pos, TTMemoryBuffer::begin );
  subchunk2_size      = mem_buffer->readUInt32();
  data_start_position = (int)mem_buffer->currentPosition();

  // search interesting stream points
  // base.CreateIndex(FileName);

  return (int)0;;
}

void TTPCMAudioStream::cut(__attribute__ ((unused)) TTFileBuffer* cut_stream, __attribute__ ((unused))TTCutListData* cut_list )
{

}


// cut method
// -----------------------------------------------------------------------------
void TTPCMAudioStream::cut( TTFileBuffer* cut_stream, int start, int end, TTCutParameter* cp )
{
  int32_t length;

  if ( cp->first_call)
  {
    // Der 1. Schnitt muß den Header schreiben!
    TTMemoryBuffer* mem_buffer = new TTMemoryBuffer();

      mem_buffer->write(ChunkID);
      mem_buffer->write((uint32_t)0); // Länge des gesamten Files -8 Bytes
      mem_buffer->write(Format);
      mem_buffer->write(Subchunk1ID);
      mem_buffer->write(Subchunk1Size);
      mem_buffer->write(AudioFormat);
      mem_buffer->write(num_channels);
      mem_buffer->write(sample_rate);
      mem_buffer->write(byte_rate);
      mem_buffer->write(block_align);
      mem_buffer->write(bits_per_sample);
      mem_buffer->write(Subchunk2ID);
      mem_buffer->write((uint32_t)0); // Länge des 'data' Chunk
      mem_buffer->writeTo( cut_stream );
  }

  copySegment( cut_stream, getSample(start)->position,
	      getSample(end)->position+getSample(end)->frame_length-1);


  if (cp->last_call)
  {
    // Laenge im 'data' Segment anpassen
    length = (int32_t)(cut_stream->currentOffset()-44);

    TTMemoryBuffer* mem_buffer = new TTMemoryBuffer();

    cut_stream->seekAbsolute( 40 );
    mem_buffer->write(length); // Die Länge des DataChunk!
    mem_buffer->writeTo( cut_stream );
    cut_stream->seekAbsolute( 4 );
    mem_buffer->seek( 0, TTMemoryBuffer::begin );
    mem_buffer->write(length+36); // Die Länge des kompletten Chunks
    mem_buffer->writeTo( cut_stream );
  }
}

// return stream extension
// -----------------------------------------------------------------------------
QString TTPCMAudioStream::streamExtension()
{
  QString str_extension = ".wav";

  return str_extension;
}

// return sample at index position
// -----------------------------------------------------------------------------
TTPCMAudioHeader* TTPCMAudioStream::getSample( int index )
{
  if ( (long)index < (long)0 )
    index = (long)0;

  if ( index >= samples_count )
    index = samples_count-1;

  return (TTPCMAudioHeader*)NULL;
  //  return new TTPCMAudioHeader( data_start_position+index*block_align,
  //			       block_align,
  //			       start_delay+(1000*index)/sample_rate,
  //			       num_channels,byte_rate,sample_rate );
}

// return sample index position for time
// -----------------------------------------------------------------------------
int TTPCMAudioStream::getLessOrSame( float time )
{
  int pos = (int)((time-start_delay)*sample_rate/1000);

  if ( pos < samples_count)
    return pos;
  else
    return samples_count-1;
}

// return total count of samples in stream
// -----------------------------------------------------------------------------
int TTPCMAudioStream::samplesCount()
{
  return (int)(subchunk2_size/block_align);
}

// calculate the stream length
// -----------------------------------------------------------------------------
long TTPCMAudioStream::calculateLength( __attribute__ ((unused))int start, __attribute__ ((unused))int end)
{
  //return base.CalculateLength(start,end)+44; // 44 Bytes mehr fuer den WAV-Header
  return (long)0;
}

// WAV hat keine Moeglichkeit das Format zu wechseln!, daher:
// -----------------------------------------------------------------------------
void TTPCMAudioStream::parseForInterestingStreamPoints()
{

}


float TTPCMAudioStream::length()
{
  return getSample(samples_count-1)->abs_frame_end_time-getSample(0)->abs_frame_time;
}

int TTPCMAudioStream::startDelay()
{
  return start_delay;
}

int TTPCMAudioStream::setStartDelay( int s_delay )
{
  start_delay = s_delay;
  parseForInterestingStreamPoints();

  return (int)0;
}

