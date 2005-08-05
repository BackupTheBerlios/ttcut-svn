/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttcutposition.h                                                 */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 02/23/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTCUTPOSITION
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

#ifndef TTCUTPOSITION_H
#define TTCUTPOSITION_H

#include <qdatetime.h>
#include <qptrvector.h>

#include "tmpeg2header.h"  // TListe
#include "theader.h"

typedef long long int64;

class  TSchnittPunkt
{
 public:
  TSchnittPunkt();
  ~TSchnittPunkt();

    QString VideoName;
    QString AudioName;
    long    Anfang;
    long    Ende;
    long    cutInFrame;
    long    cutOutFrame;
    QTime   cutInTime;
    QTime   cutOutTime;
    QTime   cutResultTime;
    int64   VideoGroesse;
    int64   AudioGroesse;
    float   FrameRate;
    int     AudioOffset;
    TListe* VideoListe;
    TListe* VideoIndexListe;
    TListe* AudioListe;
    int64   cutInOffset;
    int64   cutOutOffset;
};

class TTVideoCutList : public QPtrVector<TSchnittPunkt>
{
 public:
  TTVideoCutList(uint size);
  ~TTVideoCutList();

  void Add(TSchnittPunkt* cutPos);
  void Loeschen();

  uint actualSize;
  int insPos;
};


#endif //TTCUTPOSITION_H
