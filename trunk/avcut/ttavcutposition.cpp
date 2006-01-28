/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttavcutposition.cpp                                             */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 02/23/2005 */
/* MODIFIED: b. altendorf                                    DATE: 06/20/2005 */
/* MODIFIED: b. altendorf                                    DATE: 06/22/2005 */
/* MODIFIED: b. altendorf                                    DATE: 08/13/2005 */
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

//#define TTAVCUTLIST_DEBUG
//#define TTCUTPARAMETER_DEBUG

const char c_name[] = "TTAVCUTLIST   : ";

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
TTAVCutPosition::TTAVCutPosition( int c_in, int c_out, int c_order)
{
  cut_in_position  = c_in;
  cut_out_position = c_out;
  cut_in_offset    = (off64_t)0;
  cut_out_offset   = (off64_t)0;
  cut_order        = c_order;
}

TTAVCutPosition::TTAVCutPosition( int c_in,  off64_t c_in_off,
				  int c_out, off64_t c_out_off, int c_order )
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
void TTAVCutPosition::setCutIn( int c_in )
{
  cut_in_position = c_in;
}

// set items cut-out position
// -----------------------------------------------------------------------------
void TTAVCutPosition::setCutOut( int c_out )
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
void TTAVCutPosition::setCutOrder( int c_order )
{
  cut_order = c_order;
}

// get the cut-in position
// -----------------------------------------------------------------------------
int TTAVCutPosition::cutIn()
{
  return cut_in_position;
}

// get the cut-out position
// -----------------------------------------------------------------------------
int TTAVCutPosition::cutOut()
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
int TTAVCutPosition::cutOrder()
{
  return cut_order;
}

// global compare function
// ------------------------------------------------------------------------------
bool compareCutOrder( TTAVCutPosition* pos_1, TTAVCutPosition* pos_2 )
{
  if ( pos_1->cutOrder() < pos_2->cutOrder() )
    return true;
  else
    return false;
}

  
// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// TTAVCutList
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// constructor
// -----------------------------------------------------------------------------
TTAVCutList::TTAVCutList( int size )
{
  actual_size  = size;
}

// default destructor
// -----------------------------------------------------------------------------
TTAVCutList::~TTAVCutList()
{
  int i;

  for ( i = 0; i < size(); i++ )
  {
    TTAVCutPosition* c_pos = at(i);
    delete c_pos;
  }
  clear();
}

// add a new cut position item to the list
// -----------------------------------------------------------------------------
int TTAVCutList::addCutPosition( int c_in, int c_out, int c_order )
{
  cut_position = new TTAVCutPosition( c_in, c_out, c_order );

#if defined(TTAVCUTLIST_DEBUG)
  qDebug( "%sappend item: %ld",c_name,cut_position );
#endif
  append( cut_position );

  return count();
}

int TTAVCutList::addCutPosition( int c_in, off64_t c_in_off,
				 int c_out, off64_t c_out_off, int c_order )
{
  cut_position = new TTAVCutPosition( c_in, c_in_off, 
				      c_out, c_out_off,c_order );

#if defined(TTAVCUTLIST_DEBUG)
  qDebug( "%sappend item: %ld",c_name,cut_position );
#endif
  append( cut_position );

  return count();
}

// add a new cut position item to the list
// -----------------------------------------------------------------------------
int TTAVCutList::addCutPosition( TTAVCutPosition* c_pos )
{
  cut_position = new TTAVCutPosition( c_pos->cutIn(), c_pos->cutOut(), c_pos->cutOrder() );

#if defined(TTAVCUTLIST_DEBUG)
  qDebug( "%sappend item: %ld",c_name,cut_position );
#endif
  append( cut_position );

  return count();
}


// get items cut-in index
// -----------------------------------------------------------------------------
int TTAVCutList::cutInAt( int i_pos )
{
  if ( i_pos < count() )
    return at( i_pos )->cutIn();
  else
    return (int)0;
}

// get items cut-out index
// -----------------------------------------------------------------------------
int TTAVCutList::cutOutAt( int i_pos )
{
  if ( i_pos < count() )
    return at( i_pos )->cutOut();
  else
    return (int)0;
}

// get items cut-in index
// -----------------------------------------------------------------------------
off64_t TTAVCutList::cutInOffsetAt( int i_pos )
{
  if ( i_pos < count() )
    return at( i_pos )->cutInOffset();
  else
    return (off64_t)0;
}

// get items cut-out index
// -----------------------------------------------------------------------------
off64_t TTAVCutList::cutOutOffsetAt( int i_pos )
{
  if ( i_pos < count() )
    return at( i_pos )->cutOutOffset();
  else
    return (off64_t)0;
}

// get items cut order
// -----------------------------------------------------------------------------
int TTAVCutList::cutOrderAt( int i_pos )
{
  if ( i_pos < count() )
    return at( i_pos )->cutOrder();
  else
    return (int)0;
}

// set items cut order
// -----------------------------------------------------------------------------
int TTAVCutList::setCutOrderAt( int i_pos, int c_order )
{
  int old_order;

  if ( i_pos < count() )
  {
    old_order = cutOrderAt( i_pos );
    at( i_pos )->setCutOrder( c_order );

    return old_order;
  }
  else
    return (int)0;
}

// return the pointer to entry at position "i_pos"
// -----------------------------------------------------------------------------
TTAVCutPosition* TTAVCutList::entryAt( int i_pos )
{
  if ( i_pos < count() )
    return at( i_pos );
  else
    return (TTAVCutPosition*)NULL;
}

// return the pointer to entry with cut order "c_order"
// -----------------------------------------------------------------------------
TTAVCutPosition* TTAVCutList::entryAtOrder( int c_order )
{
  // UNUSED: const TTAVCutPosition* temp_pos = new TTAVCutPosition( 0, 0, c_order );

  sort();

  if ( c_order < count() )
    return at( c_order );
  else
    return (TTAVCutPosition*)NULL;
}

// clear the list and resize it to initial size
// -----------------------------------------------------------------------------
void TTAVCutList::deleteAll()
{
  int i;
  ins_position = 0;
  actual_size  = 10;

  for ( i = 0; i < size(); i++ )
  {
#if defined(TTAVCUTLIST_DEBUG)
    qDebug( "%sdelete all: remove entry: %ld",c_name,at(i) );
#endif
    TTAVCutPosition* c_pos = at(i);
    delete c_pos;
    this[i] = 0;
  }
  clear();
}

// removes entry at position "i_pos" from list and adjust the cut order
// from the following entries
// -----------------------------------------------------------------------------
int TTAVCutList::removeEntryAt( int i_pos )
{
  int  i;
  // UNUSED: int c_order;

  if ( i_pos < count() )
  {
    // sort the list
    sort();

    // actualize sort order after deletet item
    for ( i = i_pos+1; i < count(); i++ )
      setCutOrderAt( i, i-1 );

    // remove the requested item from the list
#if defined(TTAVCUTLIST_DEBUG)
    qDebug( "%sremove entry: %ld",c_name,at(i_pos) );
#endif
    delete at( i_pos );
    remove( i_pos );

    // sort the list
    sort();

    // new insert position for add
    ins_position = count();
  }

  // return current number of items in list
  return count();
}

int TTAVCutList::removeEntryAtOrder( int c_order )
{
  int i;
  // UNUSED: int f_index; 

  // sort the list
  sort();
  
  // actualize sort order after deletet item
  for ( i = 0; i < size(); i++ )
  {
    TTAVCutPosition* cut_pos = at(i);
    
    if ( cut_pos->cutOrder() == c_order )
    {
      // remove the requested item from the list
#if defined(TTAVCUTLIST_DEBUG)
      qDebug( "%sremove entry at order: %ld",c_name,at(i) );
#endif
      delete at( i );
      remove( i );
      
      // sort the list
      sort();
    }
  }
  // return current number of items in list
  return count();
}

// sort items in the list in ascending cut order
// -----------------------------------------------------------------------------
void TTAVCutList::sortCutOrder()
{
  sort();
}

void TTAVCutList::sort()
{
  qSort( begin(), end(), compareCutOrder );
}


// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// *** TTCutParameter
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

const char c_name_3[] = "TTCUTPARAMETER: ";

TTCutParameter::TTCutParameter()
{
#if defined(TTCUTPARAMETER_DEBUG)
  qDebug( "%screate cut parameter object",c_name_3 );
#endif
  pictures_written            = 0;
  first_call                  = true;
  last_call                   = false;
  write_max_bitrate           = false;
  write_sequence_end_code     = false;
  create_dvd_compliant_stream = false;

  result_header_list = (TTVideoHeaderList*)NULL;
}

TTCutParameter::~TTCutParameter()
{
#if defined(TTCUTPARAMETER_DEBUG)
  qDebug( "%sdelete cut parameter object",c_name_3 );
#endif
  if ( ttAssigned( result_header_list ) )
  {
#if defined(TTCUTPARAMETER_DEBUG)
    qDebug( "%sdelete header list",c_name_3 );
#endif
    delete result_header_list;
  }
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

