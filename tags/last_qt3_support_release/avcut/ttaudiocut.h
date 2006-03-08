/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttaudiocut.h                                                    */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 02/23/2005 */
/* MODIFIED: b. altendorf                                    DATE: 03/01/2005 */
/* MODIFIED: b. altendorf                                    DATE: 03/31/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TAUDIOSCHNITT
// ----------------------------------------------------------------------------

/*----------------------------------------------------------------------------*/
/* Die Klasse TAudioSchnitt entstammt dem Projekt "MPEG2Schnitt" von Martin   */
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

#ifndef TTAUDIOCUT_H
#define TTAUDIOCUT_H

#include <qstring.h>

#include "../gui/ttcut.h"
#include "../gui/ttprogressbar.h"

#include "../compat/theader.h"
#include "../compat/tmpegaudio.h"
#include "../compat/tdateipuffer.h"
#include "../compat/ttcutposition.h"

//bool VideoSchnittAnhalten;

// --------------------------------------------------------------------------------
// TAudioschnitt: Class
// --------------------------------------------------------------------------------
class TAudioSchnitt
{
 public:
   TAudioSchnitt( TTProgressBar* pBar = NULL );
   ~TAudioSchnitt();
   
   void setProgressBar( TTProgressBar* pbar );
   void IndexDateiSpeichern(QString Name, TListe* Liste);
   void ZielDateiOeffnen(QString ZielDatei);
   void QuellDateiOeffnen(TSchnittPunkt* SchnittPunkt);
   void QuellDateiSchliessen();
   void ZielDateiSchliessen();
   int  Schneiden(TTVideoCutList* SchnittListe);
   void KopiereSegment(long AnfangsIndex, long EndIndex);

   void createIndexFile( bool index );
   
 private:
   TTProgressBar* progressBar;
   QString        DateiName;                  //STRING
   QString        ZielDateiName;              //STRING
   QString        indexFileName;
   TDateiPuffer*  DateiStream;                //TDateiPuffer
   TDateiPuffer*  SpeicherStream;             //TDateiPuffer
   TListe*        Liste;                      //TListe
   TListe*        AudioListe;                 //TListe
   TListe*        NeueListe;                  //TListe
   bool           Anhalten;                   //Boolean
   bool           IndexDatei_erstellen;       //Boolean
   QString        TextString;                 //STRING
   TAudioHeader*  AudioHeader;                //TAudioHeader
   int            Versatz;                    //Integer
};

#endif //TTAUDIOCUT_H







