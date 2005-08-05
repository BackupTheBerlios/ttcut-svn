/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttaudioheaderlist.cpp                                           */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 05/12/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTAUDIOHEADERLIST
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

#include "ttaudioheaderlist.h"

TTAudioHeaderList::TTAudioHeaderList( uint size )
  : TTHeaderList( size )
{

}

TTAudioHeader* TTAudioHeaderList::audioHeaderAt( uint index )
{
  try
  {
    checkIndexRange( index );
    
    return (TTAudioHeader*)at( index );
  }
  catch ( TTListIndexException )
  {
    return NULL;
  }
}


uint TTAudioHeaderList::searchTimeIndex( double s_time )
{
  uint           index;
  uint           abs_time = 0;
  uint           search_time = (uint)s_time*1000;
  TTAudioHeader* audio_header;

  index = 0;

  do
  {
    audio_header = (TTAudioHeader*)at(index);
    abs_time = (uint)(audio_header->abs_frame_time*1000);
    qDebug( "abs time: %d / %d",abs_time, search_time );
    index++;
  }
  while ( abs_time <= search_time );

  qDebug( "found index: %d",index-2 );

  return index-2;
}


// needed by sort routine and bsearch
#ifdef __WIN32
int TTAudioHeaderList::compareItems( QCollection::Item Item1, QCollection::Item Item2 )
#else
int TTAudioHeaderList::compareItems( Q3PtrCollection::Item Item1, Q3PtrCollection::Item Item2 )
#endif
{
  int result = 0;

  TTAudioHeader* index1;
  TTAudioHeader* index2;

  index1 = (TTAudioHeader*)Item1;
  index2 = (TTAudioHeader*)Item2;

  // the values for the display order of two items are compared
  if ( (uint)index1->abs_frame_time*1000 < (uint)index2->abs_frame_time*1000 )
    result = -1;
  if ( (uint)index1->abs_frame_time*1000 == (uint)index2->abs_frame_time*1000 )
    result = 0;
  if ( (uint)index1->abs_frame_time*1000 > (uint)index2->abs_frame_time*1000 )
    result = 1;

  return result;
}
