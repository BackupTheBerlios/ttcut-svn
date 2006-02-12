/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : tvideoschnitt.cpp                                               */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 02/23/2005 */
/* MODIFIED: b. altendorf                                    DATE: 03/01/2005 */
/* MODIFIED: b. altendorf                                    DATE: 03/21/2005 */
/* MODIFIED: b. altendorf                                    DATE: 03/31/2005 */
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

// C++ stream I/O header files
// -----------------------------------------------------------------------------
#include <iostream>
#include <iomanip>
using namespace std;

#include "tvideoschnitt.h"

#define TWOGIG 2294967296
// construct object
// -----------------------------------------------------------------------------
TVideoSchnitt::TVideoSchnitt( TTProgressBar* pBar )
{
  progressBar          = pBar;
  DateiName            = " ";
  ZielDateiName        = " ";
  DateiStream          = NULL;
  SpeicherStream       = NULL;
  VideoListe           = new TListe(1000);
  VideoIndexListe      = new TListe(1000);
  NeueListe            = new TListe(1000);
  Anhalten             = false;
  Timecode_korrigieren = TTCut::correctCutTimeCode;
  Bitrate_korrigieren  = TTCut::correctCutBitRate;;
  IndexDatei_erstellen = TTCut::createCutIDD;
  SequenzHeader        = new TSequenzHeader();
  BildHeader           = new TBildHeader();
}

// destruct object
// -----------------------------------------------------------------------------
TVideoSchnitt::~TVideoSchnitt()
{
  // Wenn ein DateiStream geoeffnet ist freigeben
  if ( ttAssigned(DateiStream) )
  {
    delete DateiStream;
    DateiStream = NULL;
  }

  // Wenn ein SpeicherStream geoeffnet ist freigeben
  if ( ttAssigned(SpeicherStream) )
  {
    delete SpeicherStream;
    SpeicherStream = NULL;
  }

  // Videoliste freigeben
  if ( ttAssigned(VideoListe) )
  {
    VideoListe->Loeschen();
    delete VideoListe;
  }

  // Video Indexliste freigeben
  if ( ttAssigned(VideoIndexListe) )
  {
    VideoIndexListe->Loeschen();
    delete VideoIndexListe;
  }

  if ( ttAssigned(NeueListe) )
  {
    if ( IndexDatei_erstellen )
      IndexDateiSpeichern(ZielDateiName, NeueListe);
    NeueListe->Loeschen();
    delete NeueListe;
  }

  // SequenzHeader freigeben
  if ( ttAssigned(SequenzHeader) )
    delete SequenzHeader;

  // BildHeader freigeben
  if ( ttAssigned(BildHeader) )
    delete BildHeader;
}

// set pointer to progress dialog
// -----------------------------------------------------------------------------
void TVideoSchnitt::setProgressBar( TTProgressBar* pBar )
{
  progressBar = pBar;
}

// save index-file
// -----------------------------------------------------------------------------
void TVideoSchnitt::IndexDateiSpeichern(QString Name, TListe* Liste)
{
  long          i;
  TDateiPuffer* ListenStream;
  THeaderKlein* Header;
  uint8_t       Puffer[4];

  if ( Liste->count() > 0 && !Name.isEmpty() )
  {
    ListenStream = new TDateiPuffer(ttChangeFileExt(Name, "idd"), fmCreate);

    Puffer[0] = int('i');   // Puffer mit 'idd' fuellen
    Puffer[1] = int('d');
    Puffer[2] = int('d');
    Puffer[3] = 2;          // Version der Indexdatei

    // Puffer an den Anfang der Datei schreiben
    ListenStream->SchreibenDirekt(Puffer, 4);

    for ( i = 0; i < Liste->count(); i++ )
    {
      Header = (THeaderKlein*)Liste->at(i);
      // 1 Byte
      ListenStream->SchreibenDirekt((uint8_t*)&Header->HeaderTyp, sizeof(Header->HeaderTyp));
      // 8 Byte`
      ListenStream->SchreibenDirekt((uint8_t*)&Header->Adresse,   sizeof(Header->Adresse));

      if ( Header->HeaderTyp == 0x00 )
      {
	// 2 Byte
	ListenStream->SchreibenDirekt((uint8_t*)&Header->TempRefer, sizeof(Header->TempRefer));
	// 1 Byte
	ListenStream->SchreibenDirekt((uint8_t*)&Header->BildTyp,   sizeof(Header->BildTyp));
      }
    }
    delete ListenStream;
  }
}

// open cut-target-file
// -----------------------------------------------------------------------------
void TVideoSchnitt::ZielDateiOeffnen(QString ZielDatei)
{
  if ( (ZielDateiName != ZielDatei) )
  {
    if ( ttAssigned(SpeicherStream) )
      delete SpeicherStream;

    SpeicherStream = new TDateiPuffer(ZielDatei, fmCreate);

    if ( IndexDatei_erstellen )
      IndexDateiSpeichern(ZielDateiName, NeueListe);

    NeueListe->Loeschen();
    ZielDateiName = ZielDatei;
  }
}

// close cut-target-file
// -----------------------------------------------------------------------------
void TVideoSchnitt::ZielDateiSchliessen()
{
  if ( ttAssigned(SpeicherStream) )
  {
    delete SpeicherStream;
    SpeicherStream = NULL;
  }

  if ( IndexDatei_erstellen )
    IndexDateiSpeichern(ZielDateiName, NeueListe);

  NeueListe->Loeschen();
  ZielDateiName = " ";
}

// open video-source-file
// -----------------------------------------------------------------------------
void TVideoSchnitt::QuellDateiOeffnen(TSchnittPunkt* SchnittPunkt)
{
  TMpeg2Header* MpegHeader;

  if ( DateiName != SchnittPunkt->VideoName )
  {
    if ( ttAssigned(DateiStream) )
    {
      delete DateiStream;
      DateiStream = NULL;
    }

    MpegHeader = new TMpeg2Header();

    MpegHeader->DateiOeffnen(SchnittPunkt->VideoName);
    MpegHeader->DateiInformationLesen(SequenzHeader, BildHeader);

    // neue Liste einlesen
    if ( !ttAssigned(SchnittPunkt->VideoListe) )
    {
      VideoListe->Loeschen();
      VideoIndexListe->Loeschen();
      MpegHeader->ListeErzeugen(VideoListe, VideoIndexListe);

      Liste      = VideoListe;
      IndexListe = VideoIndexListe;
    }
    else
    {
      Liste      = SchnittPunkt->VideoListe;
      IndexListe = SchnittPunkt->VideoIndexListe;
    }

    delete MpegHeader;

    DateiStream = new TDateiPuffer(SchnittPunkt->VideoName, fmOpenRead);

    DateiStream->PufferFreigeben();
    DateiName = SchnittPunkt->VideoName;
  }
}

// close source-video-file
// -----------------------------------------------------------------------------
void TVideoSchnitt::QuellDateiSchliessen()
{
  if ( ttAssigned(DateiStream) )
  {
    delete DateiStream;
    DateiStream = NULL;
  }

  DateiName = " ";

  VideoListe->Loeschen();
  VideoIndexListe->Loeschen();

  Liste      = NULL;
  IndexListe = NULL;
}

// video cut process
// -----------------------------------------------------------------------------
int TVideoSchnitt::Schneiden( TTVideoCutList* SchnittListe )
{
  int            Result;
  int            I, J;
  TSchnittPunkt* SchnittPunkt;
  TBildIndex*    BildIndex;
  THeaderKlein*  ListenPunkt;
  int            BildNr, BildNr2;
  int            AnfangsIndex;
  QString        DateiName;
  QString        actionText;

  Result      = 0;
  AdressDiff  = 0;
  BildDiff    = 0;
  BildZaehler = 0;

  VideoSchnittAnhalten = false;
  Anhalten             = false;
  ersterSequenzHeader  = true;
  erstesBild           = true;

  //qDebug("cut count: %d",SchnittListe->count());

  if ( ttAssigned(progressBar) )
  {
    progressBar->setActionText( "Cut video" );
  }

  I = 0;  // Schnittzaehler
  while (I < SchnittListe->count() && !Anhalten )
  {

    if ( ttAssigned(progressBar) )
    {
      actionText.sprintf( "cut %d", I+1 );
      progressBar->setActionText( actionText );
    }

    // X. Schnitt
    SchnittPunkt = (TSchnittPunkt*)SchnittListe->at(I);

    QuellDateiOeffnen( SchnittPunkt );

    // Anwender hat beim erstellen der Liste auf Abbrechen gedrueckt
    if ( Liste->count() == 0 )
    {
      Anhalten = true;
    }
    else
    {
      // naechstgelegenes I-Frame suchen
      SchnittPunkt->Anfang = BildSuchen(1, SchnittPunkt->Anfang, IndexListe);

      //index in der header-liste
      BildIndex = (TBildIndex*)IndexListe->at(SchnittPunkt->Anfang);

      // der Schnitt muss an einem I-Frame beginnen
      if ( BildIndex->BildTyp == 1 )
      {
	ListenPunkt = (THeaderKlein*)Liste->at((BildIndex->BildIndex - 2));
	
	if ( ListenPunkt->HeaderTyp == SequenceStartCode )
	{
	  AnfangsIndex = BildIndex->BildIndex - 2;
	  ListenPunkt  = (THeaderKlein*)Liste->at(BildIndex->BildIndex);

	  if ( ListenPunkt->HeaderTyp == BildStartCode )
	  {
	    // TempReferenz > 0 --> offene Gruppe
	    BildNr = ListenPunkt->TempRefer;
	
	    // Die naechsten Bilder (B-Frames)
	    // muessen entfernt werden.
	    if ( BildNr > 0 )
	    {
	      // Die Tempreferenze muessen in dieser Gruppe
	      // neu geschrieben werden.
	      ListenPunkt = (THeaderKlein*)Liste->at((BildIndex->BildIndex + 1));
	
	      if ( ListenPunkt->HeaderTyp == BildStartCode )
	      {
		if ( !Anhalten )
		  // von Sequenzheader bis Ende I-Frame kopieren und korrigieren
		  //(Tempreferenz und Timecode)
		  KopiereSegment(true, AnfangsIndex,
				 BildIndex->BildIndex, SchnittPunkt->FrameRate, false);
	      }
	      else
	      {
		// Ist die TempReferenz > 0 muss nach einem I-Frame noch
		// ein B-Frame kommen
		qDebug("Ist die TempReferenz > 0 muss nach einem I-Frame noch ein B-Frame kommen");
		Result = -4;
		return Result;
	      }
	      // B-Frames ueberspringen
	      ListenPunkt = (THeaderKlein*)Liste->at((BildIndex->BildIndex + BildNr + 1));
	
	      if ( ListenPunkt->HeaderTyp == BildStartCode )
	      {
		AnfangsIndex = BildIndex->BildIndex + BildNr + 1;
	      }
	      else
	      {
		// nach den "herrenlosen" B-Frames sollte eigentlich
		// ein P-Frame stehen
		qDebug("nach den >herrenlosen< B-Frames sollte eigentlich ein P-Frame stehen");
		Result = -4;
		return Result;
	      }
	    }
	    // naechstgelegenes I-, oder P-Frame suchen
	    SchnittPunkt->Ende = BildSuchen(2, SchnittPunkt->Ende, IndexListe);
	    BildIndex          = (TBildIndex*)IndexListe->at(SchnittPunkt->Ende);
	    ListenPunkt        = (THeaderKlein*)Liste->at(BildIndex->BildIndex);
	
	    if ( ListenPunkt->HeaderTyp == BildStartCode &&
		BildIndex->BildTyp < 3 )
	    {
	      // TempReferenz des letzten I- oder P-Frames
	      BildNr = ListenPunkt->TempRefer;
	      J = 1;
	      do
	      {
		// da die Bilder nicht in der angezeigten Reihenfolge
		// gespeichert sind muss des letzte B-Frame mit kleinerer
		// TempReferenz gesucht werden
		ListenPunkt = (THeaderKlein*)Liste->at(BildIndex->BildIndex + J);
		
		if ( ListenPunkt->HeaderTyp == BildStartCode )
		  BildNr2 = ListenPunkt->TempRefer;
		else
		  // Sicherer Schleifenabbruch am GOPEnde
		  BildNr2 = BildNr + 1;
		J++;
	      } while( (BildNr > BildNr2) &&
		      (BildIndex->BildIndex + J < (long)Liste->count()));
	      // die TempReferenz ist grosser als  die TempReferenz des
	      // letztes P- oder I-Frames
	      // oder das Ende der GOP ist erreicht
	
	      if ( !Anhalten )
		// Dateiteil kopieren und korrigieren (Tempreferenz und Timecode)
		KopiereSegment(false, AnfangsIndex, BildIndex->BildIndex + J - 2,
			       SchnittPunkt->FrameRate, false);
	    }
	    else
	      Result = -4;    // am Ende des Schnittes muss ein Bildheader stehen
	  }
	  else
	    Result = -3;       // der Schnitt muss mit einem Bildheader mit I-Frame beginnen
	}
	else
	  Result = -2;          // 2 Header vorm 1. I-Frame muss ein Sequenzheader stehen
      }
      else
	Result = -1;             // Der Schnitt muss an einem I-Frame beginnen
    }
    I++; // naechster Schnitt
  }

  SequenzEndeAnhaengen();

  if ( Anhalten || Result < 0 )     // Abbruch durch Anwender
  {
    DateiName = ZielDateiName;     // DateiName merken
    NeueListe->Loeschen();            // neue Liste loeschen, dadurch
    // wird keine *.idd Datei gespeichert
    ZielDateiSchliessen();           // erzeugte Datei schliessen

    //if ( ttFileExists(DateiName) )   // und loeschen
    //	 ttDeleteFile(DateiName);

    VideoSchnittAnhalten = true;   // globale Variable, sagt weiteren
    // Proceduren das Schluss ist
  }
  //qDebug("Meldung 440: Video-Schneiden: Result: %d",Result);
  return Result;
}

void TVideoSchnitt::KopiereSegment(bool geaendert, long AnfangsIndex, long EndIndex, float Framerate, bool anzeigen)
{
  off64_t       Menge;
  off64_t       Groesse;
  off64_t       aktAdresse;
  off64_t       PufferAdr;
  off64_t       AnfAdrDatei;
  off64_t       AnfAdrSpeicher;
  long          iProgress;
  uint8_t*      Puffer;
  long          I, J;
  THeaderKlein* ListenPunkt;
  THeaderKlein* HeaderKlein;
  int           Tempreferenz;
  TTimeCode     Zeit;
  long          BitRate;
  float         fps = TTCut::frameRate;
  int           hour, minute, second, msecond;
  QString       actionText;

  uint64_t      vfatLimit = 2294967296UL;

  //test
  int festeBitRate = 0;
  bool D2VDatei_erstellen = TTCut::createD2V;

  Groesse = 0;

  if ( EndIndex > (long)Liste->count() - 2 )        // EndIndex pruefen und eventuell
    EndIndex = (long)Liste->count() - 2;            // auf maximalen Wert setzen

  if ( AnfangsIndex < (long)Liste->count() - 1 )    // AnfangsIndex pruefen, ist er groesser
  {                                                 // als die Liste -1 nicht kopieren
    AnfAdrSpeicher = SpeicherStream->AktuelleAdr(); // Startadresse der Zieldatei merken

    Puffer = new uint8_t [1048576];                 // 1 MByte

    I           = AnfangsIndex;                     // Zaehler setzen zum Header suchen
    J           = AnfangsIndex;                     // Zaehler setzen zum Header korrigieren
    ListenPunkt = (THeaderKlein*)Liste->at(I);
    aktAdresse  = ListenPunkt->Adresse;             // Startadresse zum kopieren
    AnfAdrDatei = aktAdresse;                       // Startadresse der Quelldatei merken

    //qDebug("KopiereSegment: Anfang: %ld | Ende: %ld | DateiStream-Pos: %lld",AnfangsIndex,
    //                                                                         EndIndex,aktAdresse);
    DateiStream->NeuePosition(aktAdresse);

    // progress bar
   if ( ttAssigned(progressBar) )
   {
      actionText.sprintf("copy segment: %ld - %ld | stream-Pos: %lld",AnfangsIndex,
			                                              EndIndex,aktAdresse);
      progressBar->resetProgress();
      progressBar->setActionText( actionText );
      iProgress = EndIndex-AnfangsIndex;
      progressBar->setTotalSteps( iProgress );
   }


    do
    {
      // Header suchen der gerade nooch in den Block passt
      do
      {
        I++;
	// am Ende der Liste kann man nicht auf den naechsten Header zugreifen
        if ( I + 1 < (long)Liste->count() )
	  // die Endadresse eines Headers/Bildes ist die Anfangsadresse des naechsten Headers
          ListenPunkt = (THeaderKlein*)Liste->at(I + 1);
      }
      while( (ListenPunkt->Adresse - aktAdresse) < 1048576 && // der naechste Header passt nicht mehr in den Puffer
	     (I <= EndIndex) );                               // Ende des Abschnitts

      ListenPunkt = (THeaderKlein*)Liste->at(I);

      if ( ListenPunkt->Adresse - aktAdresse > 1048576 )       // Fehler in alten Indexdateien
      {
        qDebug("Meldung84: Der zu kopierende Teil der Datei passt nicht in den Kopierpuffer.");
        Anhalten = true;
      }

      if ( !Anhalten )
      {
        Groesse = ListenPunkt->Adresse - aktAdresse;
        Menge   = DateiStream->LesenDirekt(Puffer, Groesse);

        if ( Menge < Groesse )
        {
          if ( Menge < 0 )
            qDebug("Meldung114: Dateiname, 'Die Datei 0xText1# laesst sich nicht oeffnen.");
          else
            qDebug("Meldung122: Die Datei ist scheinbar zu kurz.");
          Anhalten = true;
        }
      }

      // Abschnitt korrigieren, Indexdatei erstellen ---------------------------------------------
      if ( !Anhalten )
      {
        do
	{
          ListenPunkt = (THeaderKlein*)Liste->at(J);

          if ( IndexDatei_erstellen || D2VDatei_erstellen )    // Wenn eine neue Indexdatei erstellt werden soll,
          {
            HeaderKlein = new THeaderKlein();                 // neuen ListenPunkt erzeugen,
            HeaderKlein->HeaderTyp = ListenPunkt->HeaderTyp;     // Headertyp eintragen,
            HeaderKlein->Adresse   = ListenPunkt->Adresse - AnfAdrDatei + AnfAdrSpeicher; // neue Adresse berechnen,
            NeueListe->Add(HeaderKlein);                         // und zur Liste hinzufuegen.
          }
          else
            HeaderKlein = NULL;

          if ( ListenPunkt->HeaderTyp == BildStartCode )
          {
            BildZaehler++;                                   // Bildzaehler erhoehen

            if ( erstesBild  )                               // Ist das Bild das erste Bild im Abschnitt
            {                                                // muss eventuell die Tempreferenz neu geschrieben werden
              erstesBild = false;
              BildDiff = ListenPunkt->TempRefer;             // Bilddifferenz zum ersten kopierten Bild
            }

            if ( BildDiff > 0 )                              // TempReferenz neu schreiben
            {
              Tempreferenz = ListenPunkt->TempRefer - BildDiff;
              Puffer[ListenPunkt->Adresse - aktAdresse + 4] = Tempreferenz >> 2;
                                                                // Bit 10 - 2 von 10 Bit Tempref
              Puffer[ListenPunkt->Adresse - aktAdresse + 5] = ((Tempreferenz & 0x0003) << 6) |
		// Bit 1 und 0 von 10 Bit Tempref
	      ((Puffer[ListenPunkt->Adresse - aktAdresse + 5]) & 0x3F);
                                                                // Bildtype (Bit 5, 4 und 3) und 3 Bit von VBVDelay
            }

            if ( (IndexDatei_erstellen  || D2VDatei_erstellen) && HeaderKlein != NULL )
            {
              HeaderKlein->TempRefer = ListenPunkt->TempRefer - BildDiff;  // Tempreferenz eintragen
              HeaderKlein->BildTyp   = ListenPunkt->BildTyp;               // Bildtyp eintragen

              if ( D2VDatei_erstellen )
              {
                PufferAdr = ListenPunkt->Adresse - aktAdresse + 8;
		
                while( !((Puffer[PufferAdr]) == 0) && ((Puffer[PufferAdr + 1]) == 0) && ((Puffer[PufferAdr + 2]) == 1) )
		{
                  PufferAdr++;                                         // naechsten Startcode suchen
		}
		
		if ( Puffer[PufferAdr + 3] == 0xB5 && (Puffer[PufferAdr + 4] & 0xF0) == 0x80 )
                {                                                     // erweiterter Startcode mit "Picture coding extension"
                  PufferAdr += 7;
                  if ( ((Puffer[PufferAdr]) & 0x80) == 0x80 )         // oberstes Bild zuerst
                    HeaderKlein->BildTyp = HeaderKlein->BildTyp | 0x80;
                  if ( ((Puffer[PufferAdr]) & 0x02) == 0x02 )         // repeat_first_field
                    HeaderKlein->BildTyp = HeaderKlein->BildTyp | 0x40;
                }
              }
            }
          }
          if ( ListenPunkt->HeaderTyp == GruppenStartCode )
          {
            BildDiff = 0;                                        // Bilddifferenz zuruecksetzen

            if ( Timecode_korrigieren )
            {
	      //Timecode aus Bildzaehler berechnen
	      hour    = (int)trunc( BildZaehler / (3600 * fps));
	      minute  = (int)trunc((BildZaehler - hour * 3600 * fps) / (60 * fps));
	      second  = (int)trunc((BildZaehler - hour * 3600 * fps - minute * 60 * fps) / fps);
	      msecond = (int)trunc((BildZaehler - hour * 3600 * fps - minute * 60 * fps - second * fps) * 1000.0/fps);

              Puffer[ListenPunkt->Adresse - aktAdresse + 4] = ((Puffer[ListenPunkt->Adresse - aktAdresse + 4]) & 0x80) |
		// 1 Bit Dropframe
		((hour & 0x3F) << 2) |        // 5 Bit Stunde
		((minute & 0x30) >> 4);       // 2 Bit von Minute

              Puffer[ListenPunkt->Adresse - aktAdresse + 5] = ((Puffer[ListenPunkt->Adresse - aktAdresse + 5]) & 0x08) |
		// 1 Bit Marker
		((minute & 0x0F) << 4) |      // 4 Bit von Minute
		((second & 0x38) >> 3);       // 3 Bit von Sekunde
	
              Puffer[ListenPunkt->Adresse - aktAdresse + 6] = ((second & 0x07) << 5) |
		// 3 Bit von Sekunde
		((msecond & 0x3E) >> 1);      // 5 Bit von Bild
	
              Puffer[ListenPunkt->Adresse - aktAdresse + 7] = ((msecond & 0x01) << 7) |
		// 1 Bit von Bild
		((uint8_t)geaendert << 6) |   // wenn geaendert dann Closed Group
		((Puffer[ListenPunkt->Adresse - aktAdresse + 7]) & 0x40) ||
		// oder Orginalbit Closed Group
		((Puffer[ListenPunkt->Adresse - aktAdresse + 7]) & 0x20);
	      // Bit fuer broken Link
            }
          }

          if ( ListenPunkt->HeaderTyp == SequenceStartCode &&
	       Bitrate_korrigieren                         &&
	       ersterSequenzHeader )
          {
            ersterSequenzHeader = false;

            if ( festeBitRate != 0 )
              BitRate = (long)round(festeBitRate / 400);
            else
              BitRate = (long)round(SequenzHeader->Bitrate / 400);
	
            PufferAdr = ListenPunkt->Adresse - aktAdresse + 8;
            Puffer[PufferAdr] = (BitRate & 0x3FC00) >> 10;
            PufferAdr++;
            Puffer[PufferAdr] = (BitRate & 0x3FC) >> 2;
            PufferAdr++;
            Puffer[PufferAdr] = (Puffer[PufferAdr] & 0x3F) | (BitRate & 0x03) << 6;
	    PufferAdr++;
	
            if ( (Puffer[PufferAdr] & 0x02) == 2 )
	      PufferAdr += 64;
	
            if ( (Puffer[PufferAdr] & 0x01) == 1 )
              PufferAdr += 65;
            else
              PufferAdr++;
	
            if ( Puffer[PufferAdr]     == 0    &&
		 Puffer[PufferAdr + 1] == 0    &&
		 Puffer[PufferAdr + 2] == 1    &&
		 Puffer[PufferAdr + 3] == 0xB5 &&
		 (Puffer[PufferAdr + 4] & 0xF0) == 0x10 )
            {
              PufferAdr += 6;
              Puffer[PufferAdr] = (Puffer[PufferAdr] & 0xE0) | ((BitRate & 0x3E000000) >> 25);
              PufferAdr++;
              Puffer[PufferAdr] = (Puffer[PufferAdr] & 0x01) | ((BitRate && 0x01FC0000) >> 18);
	
            }
	  }
	  J++;
        }
        while (J <= I);
      }

      //---------------------------------------------------- Korrigieren, Indexdatei erstellen Ende
      if ( !Anhalten )
      {
        Menge = SpeicherStream->SchreibenDirekt(Puffer, Groesse);
	
	if ( ttAssigned(progressBar) )
	{
	  iProgress = I-AnfangsIndex+1;
	  Anhalten = progressBar->setProgress( iProgress );
	}
	
        if ( Menge < Groesse )
        {
          if ( SpeicherStream->AktuelleAdr() > vfatLimit - (uint64_t)10 )
	     qDebug("Meldung82: Dateischreibfehler. Datei groesser 2 GByte; LARGE_FILE_SUPPORT vorhanden?");
          else
	     qDebug("Meldung83: Dateischreibfehler. Festplatte voll?, Menge: %lld",Menge);

	  Anhalten = true;
        }
      }

      ListenPunkt = (THeaderKlein*)Liste->at(I);
      aktAdresse  = ListenPunkt->Adresse;
    }
    while( (I <= EndIndex) && !Anhalten);

    delete [] Puffer;
  }
}

void TVideoSchnitt::SequenzEndeAnhaengen()
{
   long          Daten;
   THeaderKlein* HeaderKlein;

   // Wenn eine neue Indexdatei erstellt wurde,
   if ( IndexDatei_erstellen )
   {
      HeaderKlein = new THeaderKlein();                     // neuen ListenPunkt erzeugen,

      HeaderKlein->HeaderTyp = 0xB7;                         // Sequenzendecode eintragen,
      HeaderKlein->Adresse   = SpeicherStream->AktuelleAdr(); // Adresse eintragen,
      NeueListe->Add(HeaderKlein);                          // und zur Liste hinzufuegen.
   }

   Daten = 0xB7010000;
   SpeicherStream->SchreibenDirekt((uint8_t*)&Daten, 4);
}


long TVideoSchnitt::BildSuchen( int frameTyp, long fromIndex, TListe* index )
{
  TBildIndex* frameIndex;
  long        iPos;
  long        iFound;
  long        framePosition;
  long        numFrames;

  numFrames     = index->count();
  framePosition = fromIndex;
  iPos          = fromIndex;
  iFound        = -1;

  frameIndex = (TBildIndex*)index->at(iPos);

  //if ( frameIndex->BildTyp == frameTyp ) iPos++;

  while( iPos < numFrames-1 && iFound < 0 )
  {
    frameIndex = (TBildIndex*)index->at(iPos);

    if ( frameIndex->BildTyp == frameTyp )
      iFound = iPos;

    iPos++;
  }

  if ( iPos != iFound && iFound >= 0 )
    framePosition = iFound;

  return framePosition;
}


