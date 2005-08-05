/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : tvideoschnitt.h                                                 */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 02/23/2005 */
/* MODIFIED: b. altendorf                                    DATE: 03/01/2005 */
/* MODIFIED: b. altendorf                                    DATE: 03/21/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TVIDEOSCHNITT
// ----------------------------------------------------------------------------

/*----------------------------------------------------------------------------*/
/* Die Klasse TVideoSchnitt entstammt dem Projekt "MPEG2Schnitt" von Martin   */
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

#ifndef TTVIDEOSCHNITT_H
#define TTVIDEOSCHNITT_H

#include <qstring.h>

#include "../gui/ttcut.h"
#include "../gui/ttprogressbar.h"

#include "../compat/tdateipuffer.h"
#include "../compat/tmpeg2header.h"
#include "../compat/theader.h"
#include "../compat/ttcutposition.h"


// TVideoschnitt: Class
// --------------------------------------------------------------------------------
class TVideoSchnitt
{
public:
   TVideoSchnitt( TTProgressBar* pBar = NULL );
   ~TVideoSchnitt();

    void setProgressBar( TTProgressBar* pBar );
    void IndexDateiSpeichern(QString Name, TListe* Liste);
    void ZielDateiOeffnen(QString ZielDatei);
    void QuellDateiOeffnen(TSchnittPunkt* SchnittPunkt);
    void ZielDateiSchliessen();
    void QuellDateiSchliessen();
    int  Schneiden(TTVideoCutList* SchnittListe);
    void KopiereSegment(bool geaendert, long AnfangsIndex, long EndIndex, float Framerate, bool anzeigen);
    void SequenzEndeAnhaengen();

    long BildSuchen( int frameTyp, long fromIndex, TListe* index );

    TTProgressBar*  progressBar;
    int64           AdressDiff;                //Int64;
    long            BildZaehler;               //LongInt;
    int             BildDiff;                  //Word;
    QString         DateiName;                 //STRING;
    QString         ZielDateiName;             //STRING;
    TDateiPuffer*   DateiStream;               //TDateiPuffer;
    TDateiPuffer*   SpeicherStream;            //TDateiPuffer;
    TListe*         Liste;
    TListe*         IndexListe;
    TListe*         VideoListe;
    TListe*         VideoIndexListe;
    TListe*         NeueListe;                 //TListe;
    bool            Anhalten;                  //Boolean;
    bool            Timecode_korrigieren;
    bool            Bitrate_korrigieren;       //Boolean;
    bool            IndexDatei_erstellen;      //Boolean;
    bool            VideoSchnittAnhalten;
    QString         TextString;                //STRING;
    TSequenzHeader* SequenzHeader;             //TSequenzHeader;
    TBildHeader*    BildHeader;                //TBildHeader;
    bool            ersterSequenzHeader;       //Boolean;
    bool            erstesBild;
};

#endif //TVIDEOSCHNITT_H
