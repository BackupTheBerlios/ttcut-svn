/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttavheader.cpp                                                  */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 05/12/2005 */
/* MODIFIED: b. altendorf                                    DATE: 08/13/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTAVHEADER (abstract)
// TTAUDIOHEADER
// TTVIDEOHEADER
// ----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Overview
// -----------------------------------------------------------------------------
//
//                               +- TTAC3AudioHeader
//                               |
//                               +- TTMpegAudioHeader
//             +- TTAudioHeader -|                    +- TTDTS14AudioHeader
//             |                 +- TTDTSAudioHeader -|
//             |                 |                    +- TTDTS16AudioHeader
// TTAVHeader -|                 +- TTPCMAudioHeader
//             |
//             |                                     +- TTSequenceHeader
//             |                                     |
//             |                                     +- TTSequenceEndHeader
//             +- TTVideoHeader -TTMpeg2VideoHeader -|
//             |                                     +- TTPicturesHeader
//             |                                     |
//             |                                     +- TTGOPHeader
//             |
//             +- TTVideoIndex
//             |
//             +- TTBreakObject
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

#include "ttavheader.h"

#include <qstring.h>

// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// *** TTAVHeader
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// default constructor
// -----------------------------------------------------------------------------
TTAVHeader::TTAVHeader()
{
  str_description   = new QString( "unknown" );
  str_mode          = new QString( "unknown" );
  str_bit_rate      = new QString( "unknown" );
  str_sample_rate   = new QString( "unknown" );
  header_start_code = 0xFF;
  header_offset     = 0;
}

// destructor
// -----------------------------------------------------------------------------
TTAVHeader::~TTAVHeader()
{
  if ( ttAssigned( str_description ) )
    delete str_description;

  if ( ttAssigned( str_mode ) )
    delete str_mode;

  if ( ttAssigned( str_bit_rate ) )
    delete str_bit_rate;

  if ( ttAssigned( str_sample_rate ) )
    delete str_sample_rate;
}

// return header description string
// -----------------------------------------------------------------------------
QString& TTAVHeader::descString()
{
  return *str_description;
}

// return header mode string
// -----------------------------------------------------------------------------
QString& TTAVHeader::modeString()
{
  return *str_mode;
}

// return bit rate string
// -----------------------------------------------------------------------------
QString& TTAVHeader::bitRateString()
{
  return *str_bit_rate;
}

// return sample rate string
// -----------------------------------------------------------------------------
QString& TTAVHeader::sampleRateString()
{
  return *str_sample_rate;
}

// return header type (start code)
// -----------------------------------------------------------------------------
uint8_t TTAVHeader::headerType()
{
  return header_start_code;
}

// return header offset in bytes
// -----------------------------------------------------------------------------
off64_t TTAVHeader::headerOffset()
{
  return header_offset;
}

// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// *** TTAudioHeader
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// default constructor
// -----------------------------------------------------------------------------
TTAudioHeader::TTAudioHeader()
{
  position       = 0;
  frame_time     = 0.0;
  abs_frame_time = 0.0;
  frame_length   = 0;
}

// -----------------------------------------------------------------------------
// methods common for all audio header objects
// -----------------------------------------------------------------------------

// return bit rate
// -----------------------------------------------------------------------------
int TTAudioHeader::bitRate()
{
  return 0;
}

// return sample rate
// -----------------------------------------------------------------------------
int TTAudioHeader::sampleRate()
{
  return 0;
}


double TTAudioHeader::absFrameEndTime()
{
  return 0.0;
}


int TTAudioHeader::compareTo()
{
  return 0;
}

int TTAudioHeader::frameLength()
{
  return frame_length;
}


// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// *** TTVideoHeader
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// default constructor
// -----------------------------------------------------------------------------
TTVideoHeader::TTVideoHeader()
{

}

// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// *** TTVideoIndex
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////
/*
*bool TTVideoIndex::operator < (TTAVHeader* v_index)
*{
*  qDebug( "video index operator <" );
*
*  if ( display_order < ((TTVideoIndex*)v_index)->display_order )
*    return true;
*  else
*    return false;
*}
*/
// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// *** TTBreakObject
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////
TTBreakObject::TTBreakObject()
{
  stop_object    = (TTVideoHeader*)NULL;
  restart_object = (TTVideoHeader*)NULL;
  copy_start     = -1;
  copy_stopp     = -1;
}
