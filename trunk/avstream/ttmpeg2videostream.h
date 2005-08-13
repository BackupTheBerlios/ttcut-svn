/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttmpeg2videostream.h                                            */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 05/12/2005 */
/* MODIFIED: b. altendorf                                    DATE: 08/13/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTMPEG2VIDEOSTREAM
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

#ifndef TTMPEG2VIDEOSTREAM_H
#define TTMPEG2VIDEOSTREAM_H

#include "ttavstream.h"
#include "ttmpeg2videoheader.h"
#include "ttaviwriter.h"

#include "../avcut/ttavcutposition.h"
#include "../extern/tttranscode.h"

#include <qstring.h>
#include <qfileinfo.h>
#include <q3ptrstack.h>

// -----------------------------------------------------------------------------
// TTMpeg2VideoStream
// -----------------------------------------------------------------------------
class TTMpeg2VideoStream : public TTVideoStream
{
 public:
  TTMpeg2VideoStream();
  TTMpeg2VideoStream( const QFileInfo &f_info );

  void makeSharedCopy( TTMpeg2VideoStream* v_stream );

  int createHeaderList();
  int createIndexList();

  TTSequenceHeader* currentSequenceHeader();
  TTGOPHeader*      currentGOPHeader();
  TTPicturesHeader* currentPictureHeader();
  TTSequenceHeader* sequenceHeaderAt( int index );
  TTGOPHeader*      GOPHeaderAt( int index );
  TTPicturesHeader* pictureHeaderAt( int index );
  TTVideoHeader*    getPrevMpeg2Object( TTVideoHeader* current );
  TTVideoHeader*    getNextMpeg2Object( TTVideoHeader* current );

  bool isCutInPoint( int pos );
  bool isCutOutPoint( int pos );

  void cut( TTFileBuffer* cut_stream, int start, int end, TTCutParameter* cp );
  void cut( TTFileBuffer* cut_stream, TTAVCutList* cut_list );
  void transferMpegObjects( TTFileBuffer* fs,
			    TTVideoHeader* start_object,
			    TTVideoHeader* end_object,
			    TTCutParameter* cr );

  void encodePart( int start, int end, TTCutParameter* cr, TTFileBuffer* cut_stream );
 protected:
  bool openStream();
  bool closeStream();
  bool createHeaderListFromIdd();
  bool createHeaderListFromMpeg2();
  void writeIDDFile( );
  void readIDDHeader();

 protected:
  TTFileBuffer* mpeg2_stream;
  TTFileBuffer* idd_stream;
  long*         stream_order_list;
  int           idd_file_version;
};

#endif //TTDTSAUDIOSTREAM_H
