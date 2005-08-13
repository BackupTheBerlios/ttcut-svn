/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttheaderlist.h                                                  */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 05/12/2005 */
/* MODIFIED: b. altendorf                                    DATE: 08/13/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTHEADERLIST
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

#ifndef TTHEADERLIST_H
#define TTHEADERLIST_H

#include "ttavheader.h"

#include <QVector>


// -----------------------------------------------------------------------------
// TTHeaderList: Pointer list for TTAVHeader objects
// -----------------------------------------------------------------------------
class TTHeaderList : public QVector<TTAVHeader*>
{
 public:
  TTHeaderList( int size );
  virtual ~TTHeaderList();

  virtual void add( TTAVHeader* header );
  virtual void deleteAll();
  virtual void sort();

 protected:
  virtual void checkIndexRange( int index );

 protected:
  int initial_size;
  int actual_size;
};

// -----------------------------------------------------------------------------
// Exceptions
// -----------------------------------------------------------------------------
class TTListIndexException
{
 public:
  TTListIndexException( long i ) : index(i){ qDebug("index: %ld",index);}
 long index;

};

#endif //TTHEADERLIST
