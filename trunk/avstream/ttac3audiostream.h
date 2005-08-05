/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttac3audiostream.h                                              */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 05/12/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTAC3AUDIOSTREAM
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

#ifndef TTAC3AUDIOSTREAM_H
#define TTAC3AUDIOSTREAM_H

#include "ttavstream.h"
#include "ttac3audioheader.h"

// -----------------------------------------------------------------------------
// *** TTAC3AudioStream
// -----------------------------------------------------------------------------
class TTAC3AudioStream : public TTAudioStream
{
 public:
  TTAC3AudioStream();
  TTAC3AudioStream( const QFileInfo &f_info, int s_pos=0 );

  void searchNextSyncByte();
  void readAudioHeader( TTAC3AudioHeader* audio_header );

  void cut( TTFileBuffer* cut_stream, uint start, uint end, TTCutParameter* cp );
  void cut( TTFileBuffer* cut_stream, TTAVCutList* cut_list );

  uint createHeaderList();
  QString streamExtension();
  QString absStreamTime();
};

#endif //TTAC3AUDIOSTREAM_H
