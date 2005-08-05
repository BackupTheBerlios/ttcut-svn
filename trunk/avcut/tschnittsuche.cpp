/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : tschnittsuche.cpp                                               */
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


#include "tschnittsuche.h"

TProfil::TProfil()
{
  INkorr  = 0;
  OUTkorr = 0;
  Laenge  = 1;
  Name    = " ";
}

bool TSchnittSuche::vergleicheArray(uint8_t* Array1, uint8_t* Array2)
{
   bool Result;
   int  I;
   int  ArrayLaenge;

   // Haben die Frames nicht die gleichen Eigenschaften ist
   // spaetestens nach dem 4. Byte Schluss
   ArrayLaenge = sizeof(Array1);

   if ( ArrayLaenge == sizeof(Array2) )
   {
      I = 0;

      while( I < ArrayLaenge )
      {
	 if ( Array1[I] == Array2[I] )
	    I++;
	 else
	    // Zum Erfuellen der Abbruchbedingung muuss
	    // I >= ArrayLaenge sein.
	    I = ArrayLaenge + 1;
      }

      // Nur wenn I = ArrayLaenge ist wurde die gesamte
      // Schleife erfolgreich durchlaufen.
      if ( I == ArrayLaenge )
	 Result = true;
      else
	 Result = false;
   }
   else
      Result = false;

   return Result;
}

// Schnittpunkte suchen durch binaer-Vergleich einzelner Audio-Frames
bool TSchnittSuche::SchnittpunkteSuchen(TMpegAudio* MpegAudio, TListe* Liste,
					uint8_t* vergleichsDatenIN,
					uint8_t* vergleichsDatenOUT,
					int SchnittLaenge,
					int INkorr, int OUTkorr)
{
   bool     Result;
   uint8_t* Puffer;
   int64    I, myIN;
   TSchnittPunkt* Schnittpunkt;
   bool     Anhalten = false;
   TAudioHeader* AudioHeader;
   int      Ergebnis;
   int      FrameLaenge;
   int64    Adressenzaehler;

   Result=false;

   // Audioframezaehler
   I = 0;

   // IN-Schnittpunkt
   myIN            = -1;
   Adressenzaehler = 0;
   AudioHeader     = new TAudioHeader();

   Ergebnis = MpegAudio->DateiInformationLesen(AudioHeader);

   if ( Ergebnis > -1 )
   {
     SchnittLaenge = (int)round((1000 / AudioHeader->Framezeit) * 60 * SchnittLaenge);
      FrameLaenge = AudioHeader->FrameLaenge;

      //TODO   SetLength(Puffer, FrameLaenge);
      Puffer = new uint8_t [FrameLaenge];

      do
      {
	 if ( (Ergebnis == 0) || (Ergebnis == 1) )
	 {
	    Ergebnis = MpegAudio->FrameLesen(FrameLaenge, Puffer, false);

	    if ( Ergebnis == 0 )
	    {
	       // noch kein moeglicher IN gefunden
	       if ( myIN < 0 )
	       {
		  if ( vergleicheArray(Puffer, vergleichsDatenIN) )
		     // Frame entspricht VergleichsFrame
		     myIN = I;
	       }
	       else
	       {
		  // schon moeglicher IN gefunden --> OUT suchen
		  if ( vergleicheArray(Puffer, vergleichsDatenOUT) )
		     // aktueller Frame = OUT ?
		     if ( (I - SchnittLaenge) > myIN )
		     {
			Schnittpunkt = new TSchnittPunkt();
			
			Schnittpunkt->Anfang = (long)round(myIN * (AudioHeader->Framezeit / 1000) * BilderProsek) + INkorr;
			Schnittpunkt->Ende   = (long)round(I * (AudioHeader->Framezeit / 1000) * BilderProsek) + OUTkorr;
			
			// Reserve zum Korrigieren (theoretisch koennten IN
			// und OUT nach Korrektur beide nach dem letzten
			// I-Frame liegen)
			if ( !((Schnittpunkt->Anfang < 0) ||
			       (Schnittpunkt->Anfang+100 > Schnittpunkt->Ende) ||
			       (Schnittpunkt->Ende > IndexListe->count())) )
			{
			  SchnittListe->Add((THeader*)Schnittpunkt);
			   Result = true;
			}
			else
			   delete Schnittpunkt;
			myIN = -1;
		     }
		  // ist der OUT auch ein IN ?, oder ein neuer IN,
		  // bevor OUT gefunden wurde
		  if ( vergleicheArray(Puffer, vergleichsDatenIN) )
		     myIN = I;
	       }
	       I++;
	    }
	 }
	 Adressenzaehler = Adressenzaehler + FrameLaenge;
	 //Anhalten = Fortschrittsfenster->Fortschrittsanzeige(Adressenzaehler);
      }
      while ( (Ergebnis < 0) || Anhalten );

   }
   delete AudioHeader;

   return Result;
}

