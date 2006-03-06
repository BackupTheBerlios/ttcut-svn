/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttpcmaudiostream.h                                              */
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

#ifndef TTPCMAUDIOSTREAM_H
#define TTPCMAUDIOSTREAM_H

#include "ttavstream.h"
#include "ttpcmaudioheader.h"

// -----------------------------------------------------------------------------
// TTPCMAudioStream
// -----------------------------------------------------------------------------
class TTPCMAudioStream : public TTAudioStream
{
 public:
  TTPCMAudioStream();
  TTPCMAudioStream( const QFileInfo &f_info, int s_pos );

  int   findChunk( uint32_t chunk_name, uint8_t* buffer, int buf_size);
  int   createHeaderList();
  void  cut( TTFileBuffer* cut_stream, int start, int end, TTCutParameter* cp );  
  void  cut( TTFileBuffer* cut_stream, TTCutListData* cut_list );
  QString streamExtension();
  TTPCMAudioHeader* getSample( int index );
  int   getLessOrSame( float time );
  int   samplesCount();
  long  calculateLength( int start, int end );
  void  parseForInterestingStreamPoints();
  float length();
  int   startDelay();
  int   setStartDelay( int s_delay );

  static const uint32_t ChunkID;
  static const uint32_t Format;
  static const uint32_t Subchunk1ID;
  static const uint32_t Subchunk1Size;
  static const uint16_t AudioFormat;
  static const uint32_t Subchunk2ID;

  uint16_t num_channels;
  uint32_t sample_rate;
  uint32_t byte_rate;
  uint16_t block_align;
  uint16_t bits_per_sample;
  uint32_t subchunk2_size;

  int data_start_position;
  int start_delay;
};

#endif //TTPCMAUDIOSTREAM_H
