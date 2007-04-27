/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttvideoindexlist.h                                              */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 05/12/2005 */
/* MODIFIED: b. altendorf                                    DATE: 08/13/2005 */
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
  TTVideoIndexList( int size );

  TTVideoIndex* videoIndexAt( int index );

  void sortStreamOrder();
  void sortDisplayOrder();
  void setStreamOrder();
  void setDisplayOrder();
  bool streamOrder();
  bool displayOrder();

  int     streamOrder( int index );
  int     displayOrder( int index );
  int     headerListIndex( int index );
  int     pictureCodingType( int index );
  int     sequenceIndex( int index );
  long    gopNumber( int index );
  off64_t frameSize( int index );
  //int   streamOrderIndex( int index );

  void numIFramesPlus();
  void numPFramesPlus();
  void numBFramesPlus();
  long numIFrames();
  long numPFrames();
  long numBFrames();

  int* stream_order_list;

 protected:
  void swapOrder();
  void sort();

 protected:
  int  current_order;
  long num_i_frames;
  long num_p_frames;
  long num_b_frames;
};
#endif //TTVIDEOINDEXLIST_H
