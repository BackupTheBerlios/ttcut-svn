/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttac3audioheader.cpp                                            */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 05/12/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTAC3AUDIOHEADER
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
//             +- TTVideoHeader -TTMpeg2VideoHeader
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

#include "ttac3audioheader.h"

#include <QString>

const char c_name[] = "TTAC3HEADER   : ";

TTAC3AudioHeader::TTAC3AudioHeader()
  : TTAudioHeader()
{
  if ( ttAssigned( str_description ) )
    *str_description = "AC-3";
  else
    str_description = new QString( "AC-3" );

  if ( ttAssigned( str_mode ) )
    *str_mode = "unknown";
  else
    str_mode = new QString( "unknown" );
}


QString& TTAC3AudioHeader::descString()
{
  return *str_description;
}

QString& TTAC3AudioHeader::modeString()
{
  QString num_string;

  num_string.setNum(AC3AudioCodingMode[acmod]);
  
  //TODO: Question, is AC3 mode correct ???

  //qDebug( "%sAC3 mode: %d",c_name,acmod );

  *str_mode = AC3Mode[acmod];
  *str_mode += "-";
  *str_mode += num_string;
  if ( lfeon == 0 )
    *str_mode += ".0";
  else
    *str_mode += ".1";

  return *str_mode;
}

int TTAC3AudioHeader::bitRate()
{
  return 1000*AC3BitRate[frmsizecod];
}

QString& TTAC3AudioHeader::bitRateString()
{
  str_bit_rate->sprintf( "%d KBit/s", bitRate() );

  return *str_bit_rate;
}

int TTAC3AudioHeader::sampleRate()
{
  return AC3SampleRate[fscod];
}

QString& TTAC3AudioHeader::sampleRateString()
{
  str_sample_rate->sprintf( "%d",sampleRate() );
  
  return *str_sample_rate;
}

