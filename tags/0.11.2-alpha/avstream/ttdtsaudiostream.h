/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttdtsaudiostream.h                                              */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 05/12/2005 */
/* MODIFIED: b. altendorf                                    DATE: 08/13/2005 */
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

#ifndef TTDTSAUDIOSTREAM_H
#define TTDTSAUDIOSTREAM_H

#include "ttavstream.h"
#include "ttdtsaudioheader.h"

// -----------------------------------------------------------------------------
// *** TTDTSAudioStream: Base class for DTS audio streams
// -----------------------------------------------------------------------------
class TTDTSAudioStream : public TTAudioStream
{
 public:
  TTDTSAudioStream();
  TTDTSAudioStream( const QFileInfo &f_info, int s_pos );

  QString  streamExtension();
  int      createHeaderList( );
  void     assignValues( TTDTSAudioHeader* audio_header, uint8_t* source, int start_pos );
  uint8_t* fourteenToSixteen( uint8_t* org, int start, int word_count);

  virtual void searchNextSyncWord();

  void readAudioHeader( TTDTSAudioHeader* audio_header );
  TTAVTypes::AVStreamType audio_mode;

};


// -----------------------------------------------------------------------------
// *** TTDTS14AudioStream
// -----------------------------------------------------------------------------
class TTDTS14AudioStream : public TTDTSAudioStream
{
 public:
  TTDTS14AudioStream();
  TTDTS14AudioStream( const QFileInfo &f_info, int s_pos );

  void readAudioHeader( TTDTSAudioHeader* audio_header );
  void searchNextSyncWord();
};



// -----------------------------------------------------------------------------
// *** TTDTS16AudioStream
// -----------------------------------------------------------------------------
class TTDTS16AudioStream : public TTDTSAudioStream
{
 public:
  TTDTS16AudioStream();
  TTDTS16AudioStream( const QFileInfo &f_info, int s_pos );

  void readAudioHeader( TTDTSAudioHeader* audio_header );
  void searchNextSyncWord();
};

#endif //TTDTSAUDIOSTREAM_H
