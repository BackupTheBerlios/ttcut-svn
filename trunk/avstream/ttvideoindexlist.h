/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttvideoindexlist.h                                              */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 05/12/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTVIDEOINDEXLIST
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

#ifndef TTVIDEOINDEXLIST_H
#define TTVIDEOINDEXLIST_H

#include "ttheaderlist.h"
#include "ttmpeg2videoheader.h"

// -----------------------------------------------------------------------------
// TTVideoIndexList: List of pointers to TTFrameIndex 
// -----------------------------------------------------------------------------
class TTVideoIndexList : public TTHeaderList
{
 public:
  TTVideoIndexList( uint size );

  TTVideoIndex* videoIndexAt( uint index );

  void sortStreamOrder();
  void sortDisplayOrder();
  void setStreamOrder();
  void setDisplayOrder();
  bool streamOrder();
  bool displayOrder();

  uint    streamOrder( uint index );
  uint    displayOrder( uint index );
  uint    headerListIndex( uint index );
  int     pictureCodingType( uint index );
  uint    sequenceIndex( uint index );
  long    gopNumber( uint index );
  off64_t frameSize( uint index );
  //uint    streamOrderIndex( uint index );

  void numIFramesPlus();
  void numPFramesPlus();
  void numBFramesPlus();
  long numIFrames();
  long numPFrames();
  long numBFrames();

  int* stream_order_list;

 protected:
#ifdef __WIN32
  int compareItems( QCollection::Item Item1, QCollection::Item Item2 );
#else
  int compareItems( Q3PtrCollection::Item Item1, Q3PtrCollection::Item Item2 );
#endif
  void swapOrder();

 protected:
  int  current_order;
  long num_i_frames;
  long num_p_frames;
  long num_b_frames;
};



#endif //TTVIDEOINDEXLIST_H
