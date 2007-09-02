/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttvideoindexlist.h                                              */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 05/12/2005 */
/* MODIFIED: b. altendorf                                    DATE: 08/29/2007 */
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

/*! ////////////////////////////////////////////////////////////////////////////
 * Compare function for sorting the video index list by display order
 */
bool compareFunc( TTAVHeader* index_1, TTAVHeader* index_2 )
{
  if ( ((TTVideoIndex*)index_1)->display_order < ((TTVideoIndex*)index_2)->display_order )
    return true;
  else
    return false;
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Construct a index list object
 */
TTVideoIndexList::TTVideoIndexList( int size )
  :TTHeaderList( size )
{
  current_order     = 0;  //0: stream order; 1: display order
  stream_order_list = NULL;
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Returns the TTVideoIndex object at index list position index
 */
TTVideoIndex* TTVideoIndexList::videoIndexAt( int index )
{
  try
  {
    checkIndexRange( index );
    return (TTVideoIndex*)at( index );
  }
  catch ( TTListIndexException )
  {
    qDebug("TTListIndexException");
    return NULL;
  }
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Sort list items in stream order (default)
 */
void TTVideoIndexList::sortStreamOrder()
{
  // list has to be in display order
  if ( current_order == 1 )
  {
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

/*! ////////////////////////////////////////////////////////////////////////////
 * Sort list items in display order
 */
void TTVideoIndexList::sortDisplayOrder()
{
  //list has to be in stream order (the default)
  if ( current_order == 0 )
  {
    // sort contents
    sort();

    // current order is display order
    current_order = 1;
  }
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Returns true if the index list is sorted in stream order
 */
bool TTVideoIndexList::isStreamOrder()
{
  if ( current_order == 0 )
    return true;
  else
    return false;
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Returns true if the index list is sorted in display order
 */
bool TTVideoIndexList::isDisplayOrder()
{
  if ( current_order == 1 )
    return true;
  else
    return false;
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Returns the stream order of the picture at index position
 */
int TTVideoIndexList::streamOrder( int index )
{
  try
  {
    return videoIndexAt(index)->stream_order;
  }
  catch ( TTListIndexException )
  {
    return (int)0;
  }
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Returns the display order of the picture at index positon
 */
int TTVideoIndexList::displayOrder( int index )
{
  try
  {
    return videoIndexAt(index)->display_order;
  }
  catch ( TTListIndexException )
  {
    return (int) 0;
  }
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Returns the index in video header list of picture at given index
 */
int TTVideoIndexList::headerListIndex( int index )
{
  try
  {
    return videoIndexAt(index)->header_list_index;
  }
  catch ( TTListIndexException )
  {
    qDebug( "%sexception (!): %d",c_name,index );
    return (int)0;
  }
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Returns the picture_coding_type (I=1,P=2,B=3) at index position
 */
int  TTVideoIndexList::pictureCodingType( int index )
{
  try
  {
    return videoIndexAt(index)->picture_coding_type;
  }
  catch ( TTListIndexException )
  {
    return (int) 0;
  }
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Returns the sequence index of picture at index position
 */
int TTVideoIndexList::sequenceIndex( int index )
{
  try
  {
    return videoIndexAt(index)->sequence_index;
  }
  catch ( TTListIndexException )
  {
    return (int)0;
  }
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Returns the gop number of picture at index position
 */
long TTVideoIndexList::gopNumber( int index )
{
  try
  {
    return videoIndexAt(index)->gop_number;
  }
  catch ( TTListIndexException )
  {
    return (long)0;
  }
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Swap display and stream order and vice versa
 */
void TTVideoIndexList::swapOrder()
{
  long temp_order;
  int  i;

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

/*! ////////////////////////////////////////////////////////////////////////////
 * Sort the video index list by display order
 */
void TTVideoIndexList::sort()
{
  qSort( begin(), end(), compareFunc );
}
