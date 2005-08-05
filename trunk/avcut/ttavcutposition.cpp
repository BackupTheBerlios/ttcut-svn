/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttavcutposition.cpp                                             */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 02/23/2005 */
/* MODIFIED: b. altendorf                                    DATE: 06/20/2005 */
/* MODIFIED: b. altendorf                                    DATE: 06/22/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTAVCUTPOSITION
// *** TTAVCUTLIST
// ----------------------------------------------------------------------------

/*----------------------------------------------------------------------------*/
/* Die Klasse TSchnittPunkt entstammt dem Projekt "MPEG2Schnitt" von Martin   */
/* Dienert und wurde von mir nach C++ portiert und an mein Projekt angepasst. */
/* Copyright (C) 2003  Martin Dienert                                         */
/* Homepage: http:www.mdienert.de/mpeg2schnitt/                               */
/* E-Mail:   m.dienert@gmx.de                                                 */
/* Martin Dienert ist nicht verantwortlich fuer diese Quellen und leistet     */
/* diesbezueglich auch keinen Support.                                        */
/*----------------------------------------------------------------------------*/

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


#include "ttavcutposition.h"


// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// TTAVCutPosition
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// items default constructor
// -----------------------------------------------------------------------------
TTAVCutPosition::TTAVCutPosition()
{
  cut_in_position  = 0;
  cut_out_position = 0;
  cut_order        = 0;
}

// constructor with cut position and cut order
// -----------------------------------------------------------------------------
TTAVCutPosition::TTAVCutPosition( uint c_in, uint c_out, uint c_order)
{
  cut_in_position  = c_in;
  cut_out_position = c_out;
  cut_in_offset    = (off64_t)0;
  cut_out_offset   = (off64_t)0;
  cut_order        = c_order;
}

TTAVCutPosition::TTAVCutPosition( uint c_in,  off64_t c_in_off,
				  uint c_out, off64_t c_out_off, uint c_order )
{
  cut_in_position  = c_in;
  cut_out_position = c_out;
  cut_in_offset    = c_in_off;
  cut_out_offset   = c_out_off;
  cut_order        = c_order;
}

// defualt destructor
// -----------------------------------------------------------------------------
TTAVCutPosition::~TTAVCutPosition()
{

}

// set items cut-in position
// -----------------------------------------------------------------------------
void TTAVCutPosition::setCutIn( uint c_in )
{
  cut_in_position = c_in;
}

// set items cut-out position
// -----------------------------------------------------------------------------
void TTAVCutPosition::setCutOut( uint c_out )
{
  cut_out_position = c_out;
}

// set items cut-in offset (bytes)
// -----------------------------------------------------------------------------
void TTAVCutPosition::setCutInOffset( off64_t c_in )
{
  cut_in_offset = c_in;
}

// set items cut-out offset (bytes)
// -----------------------------------------------------------------------------
void TTAVCutPosition::setCutOutOffset( off64_t c_out )
{
  cut_out_offset = c_out;
}

// set items cut order
// -----------------------------------------------------------------------------
void TTAVCutPosition::setCutOrder( uint c_order )
{
  cut_order = c_order;
}

// get the cut-in position
// -----------------------------------------------------------------------------
uint TTAVCutPosition::cutIn()
{
  return cut_in_position;
}

// get the cut-out position
// -----------------------------------------------------------------------------
uint TTAVCutPosition::cutOut()
{
  return cut_out_position;
}

// get the cut-in offset (bytes)
// -----------------------------------------------------------------------------
off64_t TTAVCutPosition::cutInOffset()
{
  return cut_in_offset;
}

// get the cut-out offset (bytes)
// -----------------------------------------------------------------------------
off64_t TTAVCutPosition::cutOutOffset()
{
  return cut_out_offset;
}

// get the cut order
// -----------------------------------------------------------------------------
uint TTAVCutPosition::cutOrder()
{
  return cut_order;
}


// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// TTAVCutList
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// constructor
// -----------------------------------------------------------------------------
TTAVCutList::TTAVCutList( uint size )
{
  actual_size  = size;
  ins_position = 0;

  // resize pointer list
  resize( actual_size );
}

// default destructor
// -----------------------------------------------------------------------------
TTAVCutList::~TTAVCutList()
{

}

// add a new cut position item to the list
// -----------------------------------------------------------------------------
uint TTAVCutList::addCutPosition( uint c_in, uint c_out, uint c_order )
{
  if ( ins_position >= actual_size )
  {
    actual_size += 10;
    if ( !resize( actual_size ) )
    {
      return size();
    }
  }

  cut_position = new TTAVCutPosition( c_in, c_out, c_order );

  insert( ins_position, cut_position );

  ins_position++;

  return count();
}

uint TTAVCutList::addCutPosition( uint c_in, off64_t c_in_off,
				  uint c_out, off64_t c_out_off, uint c_order )
{
  if ( ins_position >= actual_size )
  {
    actual_size += 10;
    if ( !resize( actual_size ) )
    {
      return size();
    }
  }

  cut_position = new TTAVCutPosition( c_in, c_in_off, 
				      c_out, c_out_off,c_order );

  insert( ins_position, cut_position );

  ins_position++;

  return count();
}

// add a new cut position item to the list
// -----------------------------------------------------------------------------
uint TTAVCutList::addCutPosition( TTAVCutPosition* c_pos )
{
  if ( ins_position >= actual_size )
    {
      actual_size += 10;
      if ( !resize( actual_size ) )
	{
	  return size();
	}
    }
  
  cut_position = new TTAVCutPosition( c_pos->cutIn(), c_pos->cutOut(), c_pos->cutOrder() );

  insert( ins_position, cut_position );

  ins_position++;

  return count();
}


// get items cut-in index
// -----------------------------------------------------------------------------
uint TTAVCutList::cutInAt( uint i_pos )
{
  if ( i_pos < count() )
    return at( i_pos )->cutIn();
  else
    return (uint)0;
}

// get items cut-out index
// -----------------------------------------------------------------------------
uint TTAVCutList::cutOutAt( uint i_pos )
{
  if ( i_pos < count() )
    return at( i_pos )->cutOut();
  else
    return (uint)0;
}

// get items cut-in index
// -----------------------------------------------------------------------------
off64_t TTAVCutList::cutInOffsetAt( uint i_pos )
{
  if ( i_pos < count() )
    return at( i_pos )->cutInOffset();
  else
    return (off64_t)0;
}

// get items cut-out index
// -----------------------------------------------------------------------------
off64_t TTAVCutList::cutOutOffsetAt( uint i_pos )
{
  if ( i_pos < count() )
    return at( i_pos )->cutOutOffset();
  else
    return (off64_t)0;
}

// get items cut order
// -----------------------------------------------------------------------------
uint TTAVCutList::cutOrderAt( uint i_pos )
{
  if ( i_pos < count() )
    return at( i_pos )->cutOrder();
  else
    return (uint)0;
}

// set items cut order
// -----------------------------------------------------------------------------
uint TTAVCutList::setCutOrderAt( uint i_pos, uint c_order )
{
  uint old_order;

  if ( i_pos < count() )
  {
    old_order = cutOrderAt( i_pos );
    at( i_pos )->setCutOrder( c_order );

    return old_order;
  }
  else
    return (uint)0;
}

// return the pointer to entry at position "i_pos"
// -----------------------------------------------------------------------------
TTAVCutPosition* TTAVCutList::entryAt( uint i_pos )
{
  if ( i_pos < count() )
    return at( i_pos );
  else
    return (TTAVCutPosition*)NULL;
}

// return the pointer to entry with cut order "c_order"
// -----------------------------------------------------------------------------
TTAVCutPosition* TTAVCutList::entryAtOrder( uint c_order )
{
  int   found_index;
  const TTAVCutPosition* temp_pos = new TTAVCutPosition( 0, 0, c_order );

  // use bsearch
  found_index = bsearch( temp_pos );

  // temporary cut position object not longer needed; delete it
  delete temp_pos;

  //qDebug( "entry at order: %d / found_index: %d",c_order,found_index );

  // valid index position;
  if ( found_index >= 0 && found_index < size() )
    // found temp_pos in list; return pointer to item
    return at( found_index );
  else
    // temp_pos could not be found in list; return NULL
    return (TTAVCutPosition*)NULL;
}

// clear the list and resize it to initial size
// -----------------------------------------------------------------------------
void TTAVCutList::deleteAll()
{
  ins_position = 0;
  actual_size  = 10;

  clear();

  resize( actual_size );
}

// removes entry at position "i_pos" from list and adjust the cut order
// from the following entries
// -----------------------------------------------------------------------------
uint TTAVCutList::removeEntryAt( uint i_pos )
{
  int  i;
  uint c_order;

  if ( i_pos < count() )
  {
    // sort the list
    sort();

    // actualize sort order after deletet item
    for ( i = i_pos+1; i < count(); i++ )
      setCutOrderAt( i, i-1 );

    // remove the requested item from the list
    remove( i_pos );

    // sort the list
    sort();

    // new insert position for add
    ins_position = count();
  }

  // return current number of items in list
  return count();
}

uint TTAVCutList::removeEntryAtOrder( uint c_order )
{
  int i;
  int f_index; 

  // sort the list
  sort();
 
  //qDebug( "remove entry at: %d",c_order );

  TTAVCutPosition* temp_pos = entryAtOrder( c_order );
  
  // search the item position
  f_index = bsearch( temp_pos );

  return removeEntryAt( f_index );
}

// sort items in the list in ascending cut order
// -----------------------------------------------------------------------------
void TTAVCutList::sortCutOrder()
{
  sort();
}

// compare the cut order of two items; needed for bsearch and sort
// -----------------------------------------------------------------------------
int TTAVCutList::compareItems( Q3PtrCollection::Item Item1, Q3PtrCollection::Item Item2 )
{
  int result = 0;

  TTAVCutPosition* pos1;
  TTAVCutPosition* pos2;

  pos1 = (TTAVCutPosition*)Item1;
  pos2 = (TTAVCutPosition*)Item2;

  // the values for the cut order of two items are compared
  if ( pos1->cutOrder() < pos2->cutOrder() )
    result = -1;
  if ( pos1->cutOrder() == pos2->cutOrder() )
    result = 0;
  if ( pos1->cutOrder() > pos2->cutOrder() )
    result = 1;

  return result;
}



// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// *** TTCutParameter
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////
TTCutParameter::TTCutParameter()
{
  pictures_written            = (uint)0;
  first_call                  = true;
  last_call                   = false;
  write_max_bitrate           = false;
  write_sequence_end_code     = false;
  create_dvd_compliant_stream = false;

  result_header_list = (TTVideoHeaderList*)NULL;
}

bool TTCutParameter::writeSequenceEndCode()
{
  return write_sequence_end_code;
}

bool TTCutParameter::writeMaxBitrate()
{
  return write_max_bitrate;
}


bool TTCutParameter::createDVDCompilantStream()
{
  return create_dvd_compliant_stream;
}  

