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


#include "ttvideoindexlist.h"

const char c_name[] = "TTVIDEOINDEX  : ";

// construct a index list object
TTVideoIndexList::TTVideoIndexList( uint size )
  :TTHeaderList( size )
{
  current_order  = 0;  //0: stream order; 1: display order

  // initialize number of frames
  num_i_frames = 0;
  num_p_frames = 0;
  num_b_frames = 0;

  stream_order_list = NULL;
}

TTVideoIndex* TTVideoIndexList::videoIndexAt( uint index )
{
  try
  {
    //qDebug( "%svideo index at: %ld",c_name,index );
    checkIndexRange( index );
    return (TTVideoIndex*)at( index );
  }
  catch ( TTListIndexException )
  {
    qDebug("TTListIndexException");
    return NULL;
  }
}

// sort list items in stream order (default)
void TTVideoIndexList::sortStreamOrder()
{
  // list has to be in display order
  if ( current_order == 1 )
  {
    //qDebug("%ssort stream order...",c_name);

    // swap display and stream order
    swapOrder();
    // sort contents
    sort();
    // swap stream and display order
    swapOrder();

    // current order is stream order
    current_order = 0;
  }
}

// sort list items in display order
void TTVideoIndexList::sortDisplayOrder()
{
  //list has to be in stream order (the default)
  if ( current_order == 0 )
  {
    // sort contents
    //qDebug("%ssort display order...",c_name);
    sort();

    // current order is display order
    current_order = 1;
  }
}

void TTVideoIndexList::setStreamOrder()
{
  // current order is stream order
  current_order = 0;
}


void TTVideoIndexList::setDisplayOrder()
{
  // current order is display order
  current_order = 1;
}

bool TTVideoIndexList::streamOrder()
{
  if ( current_order == 0 )
    return true;
  else
    return false;
}

bool TTVideoIndexList::displayOrder()
{
  if ( current_order == 1 )
    return true;
  else
    return false;
}

// return the stream order of picture at index
uint TTVideoIndexList::streamOrder( uint index )
{
  try
  {
    return videoIndexAt(index)->stream_order;
  }
  catch ( TTListIndexException )
  {
    return (long) -1;
  }
}

// return the display order of picture at index
uint TTVideoIndexList::displayOrder( uint index )
{
  try
  {
    return videoIndexAt(index)->display_order;
  }
  catch ( TTListIndexException )
  {
    return (long) -1;
  }
}

// return the index in the header list of picture at index
uint TTVideoIndexList::headerListIndex( uint index )
{
  //qDebug( "%sheader list index: %ld",c_name,index );

  try
  {
    //qDebug( "%sheader index: %ld",c_name,videoIndexAt(index)->header_list_index );
    return videoIndexAt(index)->header_list_index;
  }
  catch ( TTListIndexException )
  {
    qDebug( "%sexception (!): %ld",c_name,index );
    return (uint)0;
  }
}

// return the picture_coding_type (I=1,P=2,B=3)
int  TTVideoIndexList::pictureCodingType( uint index )
{
  //qDebug( "%scoding type at: %d",c_name,index );

  try
  {
    return videoIndexAt(index)->picture_coding_type;
  }
  catch ( TTListIndexException )
  {
    return (long) -1;
  }
}

uint TTVideoIndexList::sequenceIndex( uint index )
{
  try
  {
    return videoIndexAt(index)->sequence_index;
  }
  catch ( TTListIndexException )
  {
    return (uint)0;
  }
}


long TTVideoIndexList::gopNumber( uint index )
{
  try
  {
    return videoIndexAt(index)->gop_number;
  }
  catch ( TTListIndexException )
  {
    return (uint)0;
  }
}

off64_t TTVideoIndexList::frameSize( uint index )
{
  try
  {
    return videoIndexAt(index)->picture_size;
  }
  catch ( TTListIndexException )
  {
    return (uint)0;
  }
}


void TTVideoIndexList::numIFramesPlus()
{
  num_i_frames++;
}

void TTVideoIndexList::numPFramesPlus()
{
  num_p_frames++;
}

void TTVideoIndexList::numBFramesPlus()
{
  num_b_frames++;
}

long TTVideoIndexList::numIFrames()
{
  return num_i_frames;
}

long TTVideoIndexList::numPFrames()
{
  return num_p_frames;
}

long TTVideoIndexList::numBFrames()
{
  return num_b_frames;
}


// needed by sort routine
#ifdef __WIN32
int TTVideoIndexList::compareItems( QCollection::Item Item1, QCollection::Item Item2 )
#else
int TTVideoIndexList::compareItems( Q3PtrCollection::Item Item1, Q3PtrCollection::Item Item2 )
#endif
{
  int result = 0;

  TTVideoIndex* index1;
  TTVideoIndex* index2;

  index1 = (TTVideoIndex*)Item1;
  index2 = (TTVideoIndex*)Item2;

  // the values for the display order of two items are compared
  if ( index1->display_order < index2->display_order )
    result = -1;
  if ( index1->display_order == index2->display_order )
    result = 0;
  if ( index1->display_order > index2->display_order )
    result = 1;

  return result;
}

// swap display and stream order and vice versa
void TTVideoIndexList::swapOrder()
{
  long temp_order;
  unsigned int i;

  try
  {
    for( i = 0; i < count(); i++ )
    {
      temp_order                     = videoIndexAt(i)->display_order;
      videoIndexAt(i)->display_order = videoIndexAt(i)->stream_order;
      videoIndexAt(i)->stream_order  = temp_order;
    }
  }
  catch ( TTListIndexException )
  {
    qDebug( "not allowed index in TTVideoIndexList::swapOrder" );
  }
}
