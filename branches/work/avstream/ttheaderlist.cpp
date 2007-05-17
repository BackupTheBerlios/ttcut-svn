/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttheaderlist.cpp                                                */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 05/12/2005 */
/* MODIFIED: b. altendorf                                    DATE: 08/13/2005 */
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

//#define TTHEADERLIST_DEBUG

const char c_name[] = "TTHEADERLIST  : ";

static int number = 0;

// construct a header list object
TTHeaderList::TTHeaderList( int size )
{
  initial_size = size;
  actual_size  = size;

  qDebug("create Header list: %d",number);
  number++;
}

TTHeaderList::~TTHeaderList()
{
  int i;

//#if defined(TTHEADERLIST_DEBUG)
  qDebug( "%sdelete header list: %d / number: %d",c_name,size(), number );
  number--;
//#endif

  for ( i = 0; i < size(); i++ )
  {
    TTAVHeader* av_header = at(i);
    //qDebug("delete: %ld",at(i));
    delete av_header;
  }
  clear();
}

// ad an header to the header list
void TTHeaderList::add( TTAVHeader* header )
{
  //#if defined(TTHEADERLIST_DEBUG)
  //qDebug("%sadd header: %ld",c_name,header);
  //#endif
  append( header );
}

// remove all items from the header list
void TTHeaderList::deleteAll()
{
  int i;

  qDebug("delete all: %d", number);

  for ( i = 0; i < size(); i++ )
  {
    TTAVHeader* av_header = at(i);
    delete av_header;
  }
  clear();
}

void TTHeaderList::sort()
{
  qSort( begin(), end() );
}

void TTHeaderList::checkIndexRange( int index )
{
  //qDebug("check index: %d / %d",index,count() );
  if ( index < 0 || index >= size() )
    throw TTListIndexException(index);
}



