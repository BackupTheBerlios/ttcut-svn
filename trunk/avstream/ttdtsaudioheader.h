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

#ifndef TTDTSAUDIOHEADER_H
#define TTDTSAUDIOHEADER_H

#include "ttavtypes.h"
#include "ttavheader.h"

class QString;


__attribute__ ((unused))static int dts_frequency_index[16] =
{ 
  0,     8000,  16000, 32000, 64000, 128000, 
  11025, 22050, 44100, 88200, 176400,
  12000, 24000, 48000, 96000, 192000 
}; 

__attribute__ ((unused))static int dts_bitrate_index[32] =
{ 
  32000,   56000,   64000,   96000,   112000, 128000, 
  192000,  224000,  256000,  320000,  384000, 
  448000,  512000,  576000,  640000,  768000, 
  896000,  1024000, 1152000, 1280000, 1344000, 
  1408000, 1411200, 1472000, 1536000, 1920000, 
  2048000, 3072000, 3840000, 4096000, 0, 0 
}; 

__attribute__ ((unused))static char* dts_acmod[64] =
{ 
  "1","DM","2/0","2/0", 
  "2/0","3/0","2.1/0","3.1/0", 
  "2/2","3/2","2/2/2","2/2/2", 
  "3/2/2","3.1/2/2","","", 
  "","","","","","","","","","","","","","","","", 
  "","","","","","","","","","","","","","","","", 
  "","","","","","","","","","","","","","","","" 
}; 

__attribute__ ((unused))static int dts_channels[64] =
{ 
  1,2,2,2, 2,3,3,4, 4,5,6,6, 7,8,0,0, 
  0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
  0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
  0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 
}; 


// -----------------------------------------------------------------------------
// TTDTSAudioHeader (abstract)
// -----------------------------------------------------------------------------
class TTDTSAudioHeader : public TTAudioHeader
{
public:
  TTDTSAudioHeader();
  TTDTSAudioHeader( TTAVTypes::AVStreamType a_mode );

  QString description();
  QString mode();
  int     bitrate();
  int     samplerate();

  bool     FTYPE;
  uint8_t  SHORT;
  bool     CPF;
  uint8_t  NBLKS;
  uint16_t FSIZE;
  uint8_t  AMODE;
  uint8_t  SFREQ;
  uint8_t  RATE;

 protected:
  TTAVTypes::AVStreamType audio_mode;
  
};


// -----------------------------------------------------------------------------
// TTDTS14AudioHeader
// -----------------------------------------------------------------------------
class TTDTS14AudioHeader : public TTDTSAudioHeader
{
public:
  TTDTS14AudioHeader();

  QString& descString();
  QString& modeString();
  int      bitRate();
  int      sampleRate();

private:

};


// -----------------------------------------------------------------------------
// TTDTS16AudioHeader
// -----------------------------------------------------------------------------
class TTDTS16AudioHeader : public TTDTSAudioHeader
{
public:
  TTDTS16AudioHeader();

  QString& descString();
  QString& modeString();
  int      bitRate();
  int      sampleRate();

private:

};

#endif //TTDTSAUDIOHEADER_H
