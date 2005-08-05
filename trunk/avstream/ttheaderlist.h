/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttheaderlist.h                                                  */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 05/12/2005 */
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

// Qt 2.x (Windows)
#ifdef __WIN32
#include <qvector.h>
#include <qcollection.h>
#else
#include <q3ptrvector.h>
#include <q3ptrcollection.h>
#endif

// -----------------------------------------------------------------------------
// TTHeaderList: Pointer list for TTAVHeader objects
// -----------------------------------------------------------------------------
#ifdef __WIN32
class TTHeaderList : public QVector<TTAVHeader>
#else
class TTHeaderList : public Q3PtrVector<TTAVHeader>
#endif
{
 public:
  TTHeaderList( uint size );

  virtual void add( TTAVHeader* header );
  virtual void deleteAll();

 protected:
  virtual void checkIndexRange( uint index );

 protected:
  uint initial_size;
  uint actual_size;
  uint ins_pos;
};

// -----------------------------------------------------------------------------
// Exceptions
// -----------------------------------------------------------------------------
class TTListIndexException{};

#endif //TTHEADERLIST
