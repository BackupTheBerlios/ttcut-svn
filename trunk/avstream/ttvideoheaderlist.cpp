/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttvideoheaderlist.cpp                                           */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 05/12/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTVIDEOHEADERLIST
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

#include "ttvideoheaderlist.h"


// construct object
TTVideoHeaderList::TTVideoHeaderList( uint size )
  :TTHeaderList( size )
{
  num_sequence_header     = 0;
  num_picture_header      = 0;
  num_gop_header          = 0;
  num_sequence_end_header = 0;
}

uint8_t TTVideoHeaderList::headerTypeAt( uint index )
{
  try
  {
    checkIndexRange( index );

    return at( index )->headerType();
  }
  catch ( TTListIndexException )
  {
    return (uint8_t)0;
  }
}

TTVideoHeader* TTVideoHeaderList::headerAt( uint index )
{
  try
  {
    checkIndexRange( index );

    return (TTVideoHeader*)at( index );
  }
  catch ( TTListIndexException )
  {
    qDebug("headerAt: index exception (!)");

    if ( (long)index < 0 )
      return (TTVideoHeader*)at( 0 );

    if ( index > count()-1 )
      return (TTVideoHeader*)at( count()-1 );
  }
}

TTSequenceHeader* TTVideoHeaderList::sequenceHeaderAt( uint index )
{
  int i;

  try
  {
    checkIndexRange( index );

    if ( at(index)->headerType() == TTMpeg2VideoHeader::sequence_start_code )
    {
      //qDebug( "bingo, sequence found for index %d",index );
      return (TTSequenceHeader*)at( index );
    }
    else
    {
      i = index-1;
      while ( at(i)->headerType() != TTMpeg2VideoHeader::sequence_start_code && i > 0 )
	i--;

      index = i;

      if ( at(index)->headerType() == TTMpeg2VideoHeader::sequence_start_code )
	return (TTSequenceHeader*)at( index );
      else
      {
	qDebug( "no sequence found for index: %d",index );
	return NULL;
      }
    }
  }
  catch ( TTListIndexException )
  {
    return NULL;
  }
}


TTPicturesHeader* TTVideoHeaderList::pictureHeaderAt( uint index )
{
  try
  {
    checkIndexRange( index );
    return (TTPicturesHeader*)at( index );
  }
  catch ( TTListIndexException )
  {
    return NULL;
  }
}


TTGOPHeader* TTVideoHeaderList::gopHeaderAt( uint index )
{
  try
  {
    checkIndexRange( index );
    return (TTGOPHeader*)at( index );
  }
  catch ( TTListIndexException )
  {
    return NULL;
  }
}

int TTVideoHeaderList::headerIndex( TTVideoHeader* current )
{
  return bsearch( (TTVideoHeader*)current );
}

// Create the header list from mpeg2 stream
long TTVideoHeaderList::createHeaderList( TTFileBuffer* mpeg2_stream )
{
  return 0;
}


// -----------------------------------------------------------------------------
// IDD-Index file operations
// -----------------------------------------------------------------------------
// Aufbau der Indexdatei Video:
// -----------------------------------------------------------------------------
//
//   3 Byte Zeichenkette 'idd' (Indexdatei)
//   1 Byte Versionsnummer
// 
//   Wiederholen bis Dateiende
//   -------------------------------------------------------------
//     1 Byte Headertype       ($B3-Sequenzheader, 
//                              $B8-Gruppenheader, 
//                              $00-Bildheader)
//     8 Byte (Int64) Adresse des Headers in der Datei 
//                    (inclusive 4 Byte Startcode $00 00 01 xx)
//     >> Wenn Bildheader dann
//        2 Byte (Wort) temporaere Referenz
//        1 Byte        Bildtype (1-IFrame, 2-PFrame, 3-BFrame)
//   -------------------------------------------------------------
//   Wiederholen Ende
//
//   1 Byte HeaderType         ($B7-Sequenzendcode)
//   8 Byte Adresse            (wird zum kopieren des letzten Bildes gebraucht)
// -----------------------------------------------------------------------------

// Read an idd-index file from idd_stream and create a header list from it
long TTVideoHeaderList::readIndexFile( TTFileBuffer* idd_stream )
{
return 0;
}


// Write an idd-index file from current header list
long TTVideoHeaderList::writeIndexFile( TTFileBuffer* idd_stream )
{
return 0;
}


// Check an idd-index file against an mpeg2 stream
bool TTVideoHeaderList::checkIndexFile( TTFileBuffer* idd_stream, 
					TTFileBuffer* mpeg2_stream )
{
return false;
}


void TTVideoHeaderList::numSequencePlus()
{
  num_sequence_header++;
}


void TTVideoHeaderList::numPicturePlus()
{
  num_picture_header++;
}


void TTVideoHeaderList::numGopPlus()
{
  num_gop_header++;
}


void TTVideoHeaderList::numSequenceEndPlus()
{
  num_sequence_end_header++;
}


long TTVideoHeaderList::numSequenceHeader()
{
  return num_sequence_header;
}


long TTVideoHeaderList::numPictureHeader()
{
  return num_picture_header;
}


long TTVideoHeaderList::numGopHeader()
{
  return num_gop_header;
}


long TTVideoHeaderList::numSequenceEndHeader()
{
  return num_sequence_end_header;
}

// compare routine for sort
#ifdef __WIN32
int TTVideoHeaderList::compareItems( QCollection::Item Item1, QCollection::Item Item2 )
#else
int TTVideoHeaderList::compareItems( Q3PtrCollection::Item Item1, Q3PtrCollection::Item Item2 )
#endif
{
  int result = 0;

  TTMpeg2VideoHeader* index1;
  TTMpeg2VideoHeader* index2;

  index1 = (TTMpeg2VideoHeader*)Item1;
  index2 = (TTMpeg2VideoHeader*)Item2;

  // the values for header offset from two item are compared
  if ( index1->headerOffset() < index2->headerOffset() )
    result = -1;
  if ( index1->headerOffset() == index2->headerOffset() )
    result = 0;
  if ( index1->headerOffset() > index2->headerOffset() )
    result = 1;

  return result;
}


