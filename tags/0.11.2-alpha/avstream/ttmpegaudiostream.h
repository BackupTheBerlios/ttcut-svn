/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttmpegaudiostream.h                                             */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 05/12/2005 */
/* MODIFIED: b. altendorf                                    DATE: 06/11/2005 */
/* MODIFIED: b. altendorf                                    DATE: 08/13/2005 */
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

#ifndef TTMPEGAUDIOSTREAM_H
#define TTMPEGAUDIOSTREAM_H

#include "ttavstream.h"
#include "ttmpegaudioheader.h"

// -----------------------------------------------------------------------------
// TTMPEGAudioStream
// -----------------------------------------------------------------------------
class TTMPEGAudioStream : public TTAudioStream
{
 public:
  TTMPEGAudioStream();
  TTMPEGAudioStream( const QFileInfo &f_info, int s_pos=0 );

  void searchNextSyncByte();
  void parseAudioHeader( uint8_t* data, int offset, TTMpegAudioHeader* audio_header );

  void cut( TTFileBuffer* cut_stream, int start, int end, TTCutParameter* cp );
  void cut( TTFileBuffer* cut_stream, TTCutListData* cut_list );

  void    readAudioHeader( TTMpegAudioHeader* audio_header );
  int     createHeaderList( );
  QString streamExtension();
  QString absStreamTime();
  int     searchIndex( double s_time );
};

#endif //TTMPEGAUDIOSTREAM_H
