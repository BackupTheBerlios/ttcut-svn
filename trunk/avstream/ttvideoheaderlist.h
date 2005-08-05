/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttvideoheaderlist.h                                             */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 05/12/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTVIDEOHEADERLIST
// ----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Overview
// -----------------------------------------------------------------------------
//
//               +- TTAudioHeaderList 
//               | 
//               +- TTAudioIndexList
// TTHeaderList -|
//               +- TTVideoHeaderList
//               |
//               +- TTVideoIndexList
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

#ifndef TTVIDEOHEADERLIST_H
#define TTVIDEOHEADERLIST_H

#include "ttheaderlist.h"

#ifdef __WIN32
#include "ttwfilebuffer.h"
#include <qvector.h>
#include <qcollection.h>
#else
#include "ttfilebuffer.h"
#include <q3ptrvector.h>
#include <q3ptrcollection.h>
#endif

#include "ttmpeg2videoheader.h"


class TTSequenceHeader;
class TTPicturesHeader;
class TTGOPHeader;


// -----------------------------------------------------------------------------
// TTVideoHeaderList: Pointer list MPEG2 header objects
// -----------------------------------------------------------------------------
class TTVideoHeaderList : public TTHeaderList
{
 public:
  TTVideoHeaderList( uint size );

  uint8_t           headerTypeAt( uint index );
  TTVideoHeader*    headerAt( uint index );
  TTSequenceHeader* sequenceHeaderAt( uint index );
  TTPicturesHeader* pictureHeaderAt( uint index );
  TTGOPHeader*      gopHeaderAt( uint index );

  int headerIndex( TTVideoHeader* current );

  long createHeaderList( TTFileBuffer* mpeg2_stream );

  long readIndexFile( TTFileBuffer* idd_stream );
  long writeIndexFile( TTFileBuffer* idd_stream );
  bool checkIndexFile( TTFileBuffer* idd_stream, TTFileBuffer* mpeg2_stream );

  void numSequencePlus();
  void numPicturePlus();
  void numGopPlus();
  void numSequenceEndPlus();
  long numSequenceHeader();
  long numPictureHeader();
  long numGopHeader();
  long numSequenceEndHeader();

 protected:
#ifdef __win32
  int compareItems(QCollection::Item Item1, QCollection::Item Item2);
#else
  int compareItems(Q3PtrCollection::Item Item1, Q3PtrCollection::Item Item2);
#endif

 private:
  long num_sequence_header;
  long num_picture_header;
  long num_gop_header;
  long num_sequence_end_header;
};

#endif //TTVIDEOHEADERLIST_H
