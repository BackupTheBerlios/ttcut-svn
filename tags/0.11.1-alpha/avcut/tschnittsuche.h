/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : tschnittsuche.h                                                 */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 02/23/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TSCHNITTSUCHE
// ----------------------------------------------------------------------------

/*----------------------------------------------------------------------------*/
/* Die Klasse TSchnittSuche entstammt dem Projekt "MPEG2Schnitt" von Martin   */
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


#ifndef TSCHNITTSUCHE_H
#define TSCHNITTSUCHE_H

#include <qstring.h>

#include "../avstream/ttcommon.h"

#include "../compat/theader.h"
#include "../compat/tdateipuffer.h"
#include "../compat/tmpegaudio.h"
#include "../compat/ttcutposition.h"

class TProfil
{
 public:
  TProfil();
   int     INkorr;   //Integer
   int     OUTkorr;  //Integer
   int     Laenge;   //Integer
   QString Name; //String
};

class TSchnittSuche
{
public:
  bool vergleicheArray(uint8_t* Array1, uint8_t* Array2);
   void WertePruefen();
   void ProfileSpeichern();
   void ProfileLaden();
   bool SchnittpunkteSuchen(TMpegAudio* MpegAudio, TListe* Liste,
			    uint8_t* vergleichsdatenIN,
			    uint8_t* vergleichsdatenOUT,
			    int SchnittLaenge, 
			    int INkorr, int OUTkorr);
   
   
   QString AudioDateiName;
   TListe* SchnittListe;
   float   BilderProsek;     // BilderProsek;
   TListe* IndexListe;
   bool    ProfilDateiGeaendert;
};


//Profile int[3];

//digitale Stille: 192 KBit, 48 kHz, stereo

uint8_t DigitalSilence[576] =
  {
 0xFF, 0xFD, 0xA4, 0x00, 0x22, 0x22, 0x22, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x33, 0x6D,
 0xB6, 0xDB, 0x6D, 0xC4, 0x89, 0x24, 0x90, 0x00, 0x00, 0x00, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
 0xAA, 0xAA, 0xAA, 0xAA, 0xAF, 0xBE, 0xFB, 0xEF, 0xBE, 0xFB, 0xEF, 0xBE, 0xFB, 0xEF, 0xBE, 0xFB,
 0xEF, 0xBE, 0xFB, 0xEF, 0xBE, 0xFB, 0xEF, 0xBE, 0xFB, 0xEF, 0xBE, 0xFB, 0xEF, 0xBE, 0xFB, 0xEF,
 0xBE, 0xFB, 0xEF, 0xBE, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xF9, 0xB6, 0xF9, 0xB6,
 0xF9, 0xB6, 0xF9, 0xB6, 0xF9, 0xB6, 0xF9, 0xB6, 0xF9, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6,
 0xDB, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB, 0x6B, 0x63, 0xE7, 0xCD, 0x6B, 0x5A, 0xD6, 0xB6, 0xDB,
 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB, 0x7C, 0xDB, 0x7C, 0xDB, 0x7C, 0xDB, 0x7C, 0xDB, 0x7C, 0xDB,
 0x7C, 0xDB, 0x7C, 0xDB, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB,
 0x6D, 0xB5, 0xB1, 0xF3, 0xE6, 0xB5, 0xAD, 0x6B, 0x5B, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB, 0x6D,
 0xBE, 0x6D, 0xBE, 0x6D, 0xBE, 0x6D, 0xBE, 0x6D, 0xBE, 0x6D, 0xBE, 0x6D, 0xBE, 0x6D, 0xB6, 0xDB,
 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xDA, 0xD8, 0xF9, 0xF3, 0x5A,
 0xD6, 0xB5, 0xAD, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xDF, 0x36, 0xDF, 0x36, 0xDF, 0x36,
 0xDF, 0x36, 0xDF, 0x36, 0xDF, 0x36, 0xDF, 0x36, 0xDB, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB, 0x6D,
 0xB6, 0xDB, 0x6D, 0xB6, 0xDB, 0x6D, 0x6C, 0x7C, 0xF9, 0xAD, 0x6B, 0x5A, 0xD6, 0xDB, 0x6D, 0xB6,
 0xDB, 0x6D, 0xB6, 0xDB, 0x6F, 0x9B, 0x6F, 0x9B, 0x6F, 0x9B, 0x6F, 0x9B, 0x6F, 0x9B, 0x6F, 0x9B,
 0x6F, 0x9B, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6,
 0xB6, 0x3E, 0x7C, 0xD6, 0xB5, 0xAD, 0x6B, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB, 0x6D, 0xB7, 0xCD,
 0xB7, 0xCD, 0xB7, 0xCD, 0xB7, 0xCD, 0xB7, 0xCD, 0xB7, 0xCD, 0xB7, 0xCD, 0xB6, 0xDB, 0x6D, 0xB6,
 0xDB, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB, 0x5B, 0x1F, 0x3E, 0x6B, 0x5A, 0xD6,
 0xB5, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB, 0xE6, 0xDB, 0xE6, 0xDB, 0xE6, 0xDB, 0xE6,
 0xDB, 0xE6, 0xDB, 0xE6, 0xDB, 0xE6, 0xDB, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB,
 0x6D, 0xB6, 0xDB, 0x6D, 0xAD, 0x8F, 0x9F, 0x35, 0xAD, 0x6B, 0x5A, 0xDB, 0x6D, 0xB6, 0xDB, 0x6D,
 0xB6, 0xDB, 0x6D, 0xF3, 0x6D, 0xF3, 0x6D, 0xF3, 0x6D, 0xF3, 0x6D, 0xF3, 0x6D, 0xF3, 0x6D, 0xF3,
 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xD6, 0xC7,
 0xCF, 0x9A, 0xD6, 0xB5, 0xAD, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xF9, 0xB6, 0xF9,
 0xB6, 0xF9, 0xB6, 0xF9, 0xB6, 0xF9, 0xB6, 0xF9, 0xB6, 0xF9, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB, 0x6D,
 0xB6, 0xDB, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB, 0x6B, 0x63, 0xE7, 0xCD, 0x6B, 0x5A, 0xD6, 0xB6,
 0xDB, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB, 0x7C, 0xDB, 0x7C, 0xDB, 0x7C, 0xDB, 0x7C, 0xDB, 0x7C,
 0xDB, 0x7C, 0xDB, 0x7C, 0xDB, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6,
 0xDB, 0x6D, 0xB5, 0xB1, 0xF3, 0xE6, 0xB5, 0xAD, 0x6B, 0x5B, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB,
 0x6D, 0xBE, 0x6D, 0xBE, 0x6D, 0xBE, 0x6D, 0xBE, 0x6D, 0xBE, 0x6D, 0xBE, 0x6D, 0xBE, 0x6D, 0xB6,
 0xDB, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xDA, 0xD8, 0xF9, 0xF3,
 0x5A, 0xD6, 0xB5, 0xAD, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xDF, 0x36, 0xDF, 0x36, 0xDF,
 0x36, 0xDF, 0x36, 0xDF, 0x36, 0xDF, 0x36, 0xDF, 0x36, 0xDB, 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB,
 0x6D, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB, 0x6D, 0x6C, 0x7C, 0xF9, 0xAD, 0x6B, 0x5A, 0xD0, 0x00, 0x00
  };

#endif //TSCHNITTSUCHE_H
