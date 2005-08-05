/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttaudiocut.cpp                                                  */
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

#include "ttaudiocut.h"


// construct object
// -----------------------------------------------------------------------------
TAudioSchnitt::TAudioSchnitt( TTProgressBar* pBar )
{
  progressBar    = pBar;
  DateiName      = " ";
  indexFileName  = " ";
  DateiStream    = NULL;
  SpeicherStream = NULL;
  AudioListe     = new TListe(1000);
  NeueListe      = new TListe(1000);
  Anhalten       = false;
  AudioHeader    = new TAudioHeader();
  IndexDatei_erstellen = TTCut::createCutIDD;
}

// destruct object
// -----------------------------------------------------------------------------
TAudioSchnitt::~TAudioSchnitt()
{
   if ( DateiStream != NULL )		
   {
      delete DateiStream;
      DateiStream = NULL;
   }

   if ( SpeicherStream != NULL )
   {
      delete SpeicherStream;
      SpeicherStream = NULL;
   }

   if ( AudioListe != NULL )
   {
      AudioListe->Loeschen();
      delete AudioListe;
   }

   if ( NeueListe != NULL )
   {
      if ( IndexDatei_erstellen )
	 IndexDateiSpeichern( ZielDateiName, NeueListe );

      NeueListe->Loeschen();
      delete NeueListe;
   }

   if ( AudioHeader != NULL )
      delete AudioHeader;
}

void TAudioSchnitt::setProgressBar( TTProgressBar* pBar )
{
  progressBar = pBar;
}

void TAudioSchnitt::QuellDateiOeffnen( TSchnittPunkt* SchnittPunkt )
{
   TMpegAudio* MpegAudio;  // TMpegAudio Class

   if ( DateiName != SchnittPunkt->AudioName )
   {
     if ( DateiStream != NULL )
     {
       delete DateiStream;
       DateiStream = NULL;
     }

     MpegAudio = new TMpegAudio();

     //qDebug("Meldung 205: Oeffne Audio-Quelldatei: %s",SchnittPunkt->AudioName.ascii());

     if ( MpegAudio->DateiOeffnen( SchnittPunkt->AudioName ) > -1 )
     {
       // AudioHeader->AudioTyp = MpegAudio->Audiotyplesen;
       // Der Audiotyp und weitere Informationen
       MpegAudio->DateiInformationLesen( AudioHeader );

       // AudioHeader ist in Ordnung
       if ( AudioHeader->FrameLaenge > 0 )
       {
	 //qDebug("Meldung 206: AudioHeader ist ok: %d",AudioHeader->FrameLaenge);

	 if ( SchnittPunkt->AudioListe == NULL )
	 {
	   //qDebug("Meldung 207: ListeErzeugen");
	   AudioListe->Loeschen();
	   MpegAudio->Listeerzeugen(AudioListe, NULL, Versatz);
	   Liste = AudioListe;
	 }
	 else
	   Liste = SchnittPunkt->AudioListe;
       }
       else
       {
	 qDebug("Meldung 131: SchnittPunkt.AudioName: Fehler im ersten Header der Audiodatei");
	 qDebug("Meldung 123: ZielDateiName: Die ZielDatei wird wieder geloescht");
       }
     }
     else
     {
       qDebug("Meldung 114: SchnittPunkt.AudioName: Die Datei  laesst sich nicht oeffnen.");
       qDebug("Meldung 124: ZielDateiName: Es wird keine (vollstaendige) Datei %s erzeugt.",ZielDateiName.ascii());
     }

     delete MpegAudio;

     DateiStream = new TDateiPuffer( SchnittPunkt->AudioName, fmOpenRead );
     DateiStream->PufferFreigeben();

     DateiName  = SchnittPunkt->AudioName;
   }
}

void TAudioSchnitt::QuellDateiSchliessen()
{
   if ( DateiStream != NULL )
   {
      delete DateiStream;
      DateiStream = NULL;
   }

   DateiName = " ";

   Liste->Loeschen();
}

void TAudioSchnitt::IndexDateiSpeichern(QString Name, TListe* Liste)
{
   long               I;
   TDateiPuffer*      ListenStream;
   TAudioHeaderKlein* Header;
   uint8_t            Puffer[4];
   uint8_t            byte8[8];

   if ( Liste->count() > 0 && Name != " " )
   {
      indexFileName = ttAddFileExt( Name, "idd" );
      ListenStream = new TDateiPuffer(indexFileName, fmCreate);

      Puffer[0] = int('i');      // Puffer mit 'idd' fuellen
      Puffer[1] = int('d');
      Puffer[2] = int('d');
      Puffer[3] = 3;             // Version der Indexdatei

      // Puffer an den Anfang der Datei schreiben
      ListenStream->SchreibenDirekt(Puffer, 4);

      // Audioversatz in die Indexdatei schreiben
      //Integer(Puffer) = Versatz;
      //ListenStream->SchreibenDirekt(Puffer, 2);

      for ( I = 0; I < Liste->count(); I++ )
      {
	 Header = (TAudioHeaderKlein*)Liste->at(I);
	 // 8 Byte
	 byte8[0] = Header->Adresse;
	 ListenStream->SchreibenDirekt((uint8_t*)Header->Adresse, sizeof(Header->Adresse));
      }
      delete ListenStream;
  }
}

void TAudioSchnitt::ZielDateiOeffnen( QString ZielDatei )
{
  if ( ZielDateiName != ZielDatei )
  {
    // neuer SpeicherStream wird in "Schneiden" angelegt
    if ( SpeicherStream != NULL )
      delete SpeicherStream;

    if ( IndexDatei_erstellen )
      IndexDateiSpeichern( ZielDateiName, NeueListe );

    NeueListe->Loeschen();
    ZielDateiName = ZielDatei;

    //qDebug("Meldung 214: Neue Audio-Zieldatei oeffnen: %s",ZielDateiName.ascii());
  }
}

void TAudioSchnitt::ZielDateiSchliessen()
{
   if ( SpeicherStream != NULL )
   {
      delete SpeicherStream;
      SpeicherStream = NULL;
   }

   if ( IndexDatei_erstellen )
      IndexDateiSpeichern(ZielDateiName, NeueListe);

   NeueListe->Loeschen();
   ZielDateiName = " ";
}

int TAudioSchnitt::Schneiden( TTVideoCutList* SchnittListe )
{
   int                Result;
   long               I;
   TSchnittPunkt*     Schnittpunkt;
   long               AnfangsIndex, EndIndex;
   long               iProgress;
   QString            actionText;
   TMpegAudio*        MpegAudio;
   TAudioHeader*      AudioNULLHeader;
   TAudioHeaderKlein* AudioHeaderKlein;
   bool               Audioschneiden;
   float              Bildlaenge;
   float              Audioversatz;
   QString            DateiName;

   float fAnfangsIndex;
   float fEndIndex;

   Result = 0;
   I      = 0;

   Audioversatz = 0;
   Audioschneiden = true;

   // test
   Versatz = 0;
   // Videoschnitt wurde angehalten
   //if ( VideoSchnittAnhalten )
   //{
   // VideoSchnittAnhalten = false;
   // return Result;
   //}

   AudioNULLHeader = new TAudioHeader();

   // AudioHeader in der Schnittliste suchen
   // --------------------------------------------------------------------------
   do
   {
      // X. Schnitt
      Schnittpunkt = (TSchnittPunkt*)SchnittListe->at(I);

      if ( !Schnittpunkt->AudioName.isEmpty() )
      {
	 // Ist mindestens ein AudioHeader
	 // vorhanden wird Audio geschnitten
	 Audioschneiden = true;
	 MpegAudio      = new TMpegAudio();

         // Audiodatei muss geoeffnet sein
	 if ( MpegAudio->DateiOeffnen( Schnittpunkt->AudioName ) == 0 )
	 {
	    // Den Audiotyp und weitere Informationen ueber die
	    // erste gefundene Audiodatei lesen.
	    MpegAudio->DateiInformationLesen( AudioNULLHeader );
	    //if ( AudioNULLHeader.FrameLaenge = 0 )
	    //Audioschneiden = false;
	 }
	 // Audiodatei laesst sich nicht oeffnen,
	 else
	    Audioschneiden = false;

	 // also kein Audio schneiden.
	 delete MpegAudio;
      }
      else
	 Audioschneiden = false;

      I++;
   }
   while ( Audioschneiden && I < SchnittListe->count());
   // --------------------------------------------------------------------------

   if ( Audioschneiden )
   {
      if ( ttAssigned(progressBar) )
      {
	 progressBar->setActionText( "Cut audio" );
      }

      // Abbruch durch Anwender
      Result = -1;

      //qDebug("Meldung 224: Oeffne Ziel-Audiostream %s",ZielDateiName.ascii());

      SpeicherStream = new TDateiPuffer( ZielDateiName, fmCreate );

      // ZielDatei nicht geoeffnet
      if ( SpeicherStream->DateiEnde )
      {
	 Anhalten = true;
	 Result   = -2;
      }
      else
	 Anhalten = false;

      I = 0;
      // ---------------------------------------------------------------
      while ( I < SchnittListe->count() && !Anhalten )
      {
	// X. Schnitt
	Schnittpunkt = (TSchnittPunkt*)SchnittListe->at(I);
	
	//qDebug("Meldung 230: Schneide Audio Schnitt %d - %s",I,Schnittpunkt->AudioName.ascii());

	if ( ttAssigned(progressBar) )
	{
	   actionText.sprintf("audio cut %d - %s",I+1,Schnittpunkt->AudioName.ascii());
	   progressBar->setActionText( actionText );
	}

	if ( !Schnittpunkt->AudioName.isEmpty() )
	{
	  QuellDateiOeffnen( Schnittpunkt );

	  //qDebug("Meldung 232: Audio-Quelldatei offen: %d",Liste->count());

	  // Quelldatei nicht geoeffnet oder keine Liste
	  if ( Liste->count() == 0 )
	  {
	    Anhalten = true;
	    Result   = -3;
	  }
	  else
	  {
	    // FrameRate ist 0
	    if ( Schnittpunkt->FrameRate == 0 )
	    {
	      Anhalten = true;
	      Result   = -4;
	    }
	    else
	    {
	      Bildlaenge = 1000.0 / Schnittpunkt->FrameRate;
	
	      qDebug("Beginn Audioberechnung");
	      qDebug("Bildlaenge         : %f",Bildlaenge);
	      qDebug("Audioframelaenge   : %f",AudioHeader->Framezeit);
	      qDebug("AudioOffset        : %d",Schnittpunkt->AudioOffset);
	      qDebug("Audioversatz 1     : %f",Audioversatz);
	      qDebug("Schnittpunkt Anfang: %f",Schnittpunkt->Anfang*Bildlaenge);

	      fAnfangsIndex = (Schnittpunkt->Anfang * Bildlaenge +
			       Audioversatz - Versatz - Schnittpunkt->AudioOffset) /
		               AudioHeader->Framezeit;

	      AnfangsIndex = (long)round(fAnfangsIndex);

	      //qDebug("AnfangsIndex: %f | %d ",fAnfangsIndex,AnfangsIndex);

	      // AnfangsIndex darf nicht kleiner NULL sein.
	      if ( AnfangsIndex < 0 )
		AnfangsIndex = 0;
		
	      //qDebug("AnfangsIndex: ' + IntToStr(AnfangsIndex) + ' Frames');

	      Audioversatz = (Schnittpunkt->Anfang * Bildlaenge) -
		             (AnfangsIndex * AudioHeader->Framezeit) +
			     Audioversatz - Versatz - Schnittpunkt->AudioOffset;

	      //qDebug("Audioversatz 2: '+ FloatToStr(Audioversatz) + ' ms');
	      //qDebug("Schnittpunkt Ende+1:'+FloatToStr((Schnittpunkt.Ende+1)*Bildlaenge)+'ms');

	      fEndIndex = (((Schnittpunkt->Ende + 1) * Bildlaenge) -
			   Audioversatz - Versatz - Schnittpunkt->AudioOffset) /
		           AudioHeader->Framezeit - 1;

	      EndIndex = (long)round(fEndIndex);

	      //qDebug("EndIndex: %f | %d ",fEndIndex,EndIndex);

            Audioversatz = ((EndIndex + 1) * AudioHeader->Framezeit) -
	      ((Schnittpunkt->Ende + 1) * Bildlaenge) + Audioversatz + Versatz + Schnittpunkt->AudioOffset;

	      if ( !Anhalten )
		KopiereSegment(AnfangsIndex, EndIndex);
	    }
	  }
	}
	else
	{
	  ; // fuer leeres Audio vorgesehen
	}
	I++;
      }
      AudioHeaderKlein = new TAudioHeaderKlein;

      AudioHeaderKlein->Adresse = SpeicherStream->AktuelleAdr();
      NeueListe->Add(AudioHeaderKlein);
   }

   if ( Anhalten )
   {
     DateiName = ZielDateiName;
     NeueListe->Loeschen();
     ZielDateiSchliessen();

     if ( ttFileExists(DateiName) )
       ttDeleteFile(DateiName);
   }
   else
     Result = 0;

   delete AudioNULLHeader;

   //qDebug("Meldung 240: Audio-Schneiden Result: %d",Result);

   return Result;
}


void TAudioSchnitt::KopiereSegment(long AnfangsIndex, long EndIndex)
{
   off64_t  Menge;
   off64_t  Groesse;
   off64_t  aktAdresse;
   off64_t  AnfAdrDatei;
   off64_t  AnfAdrSpeicher;
   uint8_t* Puffer;
   long     I;
   long     iProgress;
   QString  actionText;
   TAudioHeaderKlein* Listenpunkt;
   TAudioHeaderKlein* AudioHeaderKlein;

   //qDebug("Meldung 290: KopiereSegment: %ld - %ld",AnfangsIndex,EndIndex);

   if ( ttAssigned(progressBar) )
   {
      actionText.sprintf("copy segment: %ld - %ld",AnfangsIndex,EndIndex);
      progressBar->setActionText( actionText );
      progressBar->setProgress( 0 );
      iProgress = EndIndex-AnfangsIndex;
      progressBar->setTotalSteps( iProgress );
   }

   Groesse = 0;

   if ( EndIndex > (long)Liste->count() - 2 )        // EndIndex pruefen und eventuell
      EndIndex = (long)Liste->count() - 2;              // auf maximalen Wert setzen

   // AnfangsIndex pruefen, ist er groesser
   // als die Liste -1 nicht kopieren
   if ( AnfangsIndex < (long)Liste->count() - 1 )
   {
      // Startadresse der ZielDatei merken
      AnfAdrSpeicher = SpeicherStream->AktuelleAdr();

      // 1 MByte
      Puffer = new uint8_t[1048576];

      // Zaehler setzen zum Header suchen
      I           = AnfangsIndex;
      Listenpunkt = (TAudioHeaderKlein*)Liste->at(I);
      aktAdresse  = Listenpunkt->Adresse;            // Startadresse zum kopieren
      AnfAdrDatei = aktAdresse;                      // Startadresse der Quelldatei merken
      DateiStream->NeuePosition(aktAdresse);

      do
      {
	 // Header suchen der gerade noch in den Block passt
	 do
	 {
	    I++;
	    // am Ende der Liste kann man nicht auf den naechsten Header zugreifen
	    if ( I + 1 < (long)Liste->count() )
	       // die Endadresse eines Headers/Bildes ist die Anfangsadresse des naechsten Headers
	       Listenpunkt = (TAudioHeaderKlein*)Liste->at(I+1);
	 }
	 // der naechste Header passt nicht mehr in den Puffer
	 while (((Listenpunkt->Adresse - aktAdresse) < 1048576) &&
		// Ende des Abschnitts
		(I <= EndIndex));

	 Listenpunkt = (TAudioHeaderKlein*)Liste->at(I);

	 if ( !Anhalten )
	 {
	    Groesse = Listenpunkt->Adresse - aktAdresse;
	    Menge   = DateiStream->LesenDirekt(Puffer, Groesse);

	    if ( Menge < Groesse )
	    {
	       if ( Menge < 0 )
		  qDebug("Meldung114: Die Datei #Text1# laesst sich nicht oeffnen.");
	       else
		  qDebug("Meldung122: Die Datei #Text1# ist scheinbar zu kurz.");
	
	       Anhalten = true;
	    }
	 }
	 if ( !Anhalten )
	 {
	    Menge = SpeicherStream->SchreibenDirekt(Puffer, Groesse);

	    if ( ttAssigned(progressBar) )
	    {
	      iProgress = I-AnfangsIndex+1;
	      Anhalten  = progressBar->setProgress( iProgress );
	    }

	    if ( Menge < Groesse )
	    {
	      if ( SpeicherStream->AktuelleAdr() > (uint64_t)2294967296UL - (uint64_t)10 )
		 qDebug("Meldung82: Dateischreibfehler. Datei groesser 2 GByte; LARGE_FILE_SUPPORT vorhanden?");
	      else
		 qDebug("Meldung83: Dateischreibfehler. Festplatte voll? Menge: %lld",Menge);

	      Anhalten = true;
	    }
	 }

	 aktAdresse = Listenpunkt->Adresse;
      }
      while( (I <= EndIndex) && !Anhalten);

      delete [] Puffer;

      //  fuer alle kopierten Audioframes
      for ( I = AnfangsIndex; I <= EndIndex; I++ )
      {
	 AudioHeaderKlein = new TAudioHeaderKlein();          // einen neuen Header erzeugen
	 Listenpunkt = (TAudioHeaderKlein*)Liste->at(I);      // die Adresse neu berechnen
	 AudioHeaderKlein->Adresse = Listenpunkt->Adresse - AnfAdrDatei + AnfAdrSpeicher;
	 NeueListe->Add(AudioHeaderKlein);                    // und in die Liste speichern
      }
   }
}


void TAudioSchnitt::createIndexFile( bool index )
{
  IndexDatei_erstellen = index;
}		

