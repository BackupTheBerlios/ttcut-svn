/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttheaderlist.cpp                                                */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 05/12/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTHEADERLIST
// ----------------------------------------------------------------------------

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

#include "ttheaderlist.h"

// construct a header list object
TTHeaderList::TTHeaderList( uint size )
{
  initial_size = size;
  actual_size  = size;
  ins_pos      = 0;

  resize( size );
}

// ad an header to the header list
void TTHeaderList::add( TTAVHeader* header )
{
  if ( ins_pos >= actual_size )
  {
    actual_size += 1000;
    if ( !resize( actual_size ) )
    {
      return;
    }
  }

  insert(ins_pos, header);

  ins_pos++;
}

// remove all items from the header list
void TTHeaderList::deleteAll()
{
  uint i;

  for ( i = 0; i < count(); i++ )
  {
    if ( !remove( i ) )
    {
      qDebug( "TTHeaderList: cannot remove item: %ud",i );
    }
  }

  ins_pos = 0;

  resize( initial_size );
}

void TTHeaderList::checkIndexRange( uint index )
{
  //qDebug("check index: %d / %d",index,count() );
  if ( (int)index < 0 || index >= count() )
    throw TTListIndexException();
}



