/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttdtsaudioheader.h                                              */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 05/12/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTDTSAUDIOHEADER
// *** TTDTS14AUDIOHEADER
// *** TTDTS16AUDIOHEADER
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
//                                                   +- TTPicturesHeader
//                                                   |
//                                                   +- TTGOPHeader
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

#include "ttdtsaudioheader.h"

#include <QString>

TTDTSAudioHeader::TTDTSAudioHeader()
  : TTAudioHeader()
{
  audio_mode = TTAVTypes::unknown;
}

TTDTSAudioHeader::TTDTSAudioHeader( TTAVTypes::AVStreamType a_mode )
  : TTAudioHeader()
{
  audio_mode = a_mode;
}


QString TTDTSAudioHeader::description()
{
  QString str_desc;

  if ( audio_mode == TTAVTypes::dts14_audio )
    str_desc = "DTS core 14";
  else
    str_desc = "DTS core 16";

  return str_desc;
}

QString TTDTSAudioHeader::mode()
{
  QString str_mode;

  //return string.Format("{0} ({1})",dts_acmod[AMODE],dts_channels[AMODE]);

  return str_mode;
}

int TTDTSAudioHeader::bitrate()
{
  return dts_bitrate_index[RATE];
}

int TTDTSAudioHeader::samplerate()
{
  return dts_frequency_index[SFREQ];
}


// -----------------------------------------------------------------------------
// TTDTS14AudioHeader
// -----------------------------------------------------------------------------
TTDTS14AudioHeader::TTDTS14AudioHeader()
  : TTDTSAudioHeader()
{

}

QString& TTDTS14AudioHeader::descString()
{
  return *str_description;
}


QString& TTDTS14AudioHeader::modeString()
{
  return *str_mode;
}


int TTDTS14AudioHeader::bitRate()
{
  return 0;
}


int TTDTS14AudioHeader::sampleRate()
{
  return 0;
}



// -----------------------------------------------------------------------------
// TTDTS16AudioHeader
// -----------------------------------------------------------------------------
TTDTS16AudioHeader::TTDTS16AudioHeader()
  : TTDTSAudioHeader()
{

}

QString& TTDTS16AudioHeader::descString()
{
  return *str_description;
}


QString& TTDTS16AudioHeader::modeString()
{
  return *str_mode;
}


int TTDTS16AudioHeader::bitRate()
{
  return 0;
}


int TTDTS16AudioHeader::sampleRate()
{
  return 0;
}

