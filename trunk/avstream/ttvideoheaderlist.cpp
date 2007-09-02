/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttvideoheaderlist.cpp                                           */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 05/12/2005 */
/* MODIFIED: b. altendorf                                    DATE: 08/29/2007 */
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

bool videoHeaderListCompareItems( TTAVHeader* head_1, TTAVHeader* head_2 );

/*! ////////////////////////////////////////////////////////////////////////////
 * Constructor
 */
TTVideoHeaderList::TTVideoHeaderList( int size )
  :TTHeaderList( size )
{
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Returns the header type at header list index position
 */
uint8_t TTVideoHeaderList::headerTypeAt( int index )
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

/*! ////////////////////////////////////////////////////////////////////////////
 * Returns the TTVideoHeader at header list index position
 */
TTVideoHeader* TTVideoHeaderList::headerAt( int index )
{
  try
  {
    checkIndexRange( index );

    return (TTVideoHeader*)at( index );
  }
  catch ( TTListIndexException )
  {
    if ( index < 0 )
      return (TTVideoHeader*)at( 0 );

    if ( index >= count() )
      return (TTVideoHeader*)at( count()-1 );

    return NULL;
  }
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Returns the TTSequenceHeader at header list index position
 */
TTSequenceHeader* TTVideoHeaderList::sequenceHeaderAt( int index )
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

/*! ////////////////////////////////////////////////////////////////////////////
 * Returns the TTPicturesHeader at header list index position
 */
TTPicturesHeader* TTVideoHeaderList::pictureHeaderAt( int index )
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

/*! ////////////////////////////////////////////////////////////////////////////
 * Returns the TTGOPHeader at header list index position
 */
TTGOPHeader* TTVideoHeaderList::gopHeaderAt( int index )
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

/*! ////////////////////////////////////////////////////////////////////////////
 * Returns the index position of the current TTVideoHeader
 */
int TTVideoHeaderList::headerIndex( TTVideoHeader* current )
{
  return indexOf( (TTVideoHeader*)current );
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Create the video header list from the given mpeg2 video stream
 */
long TTVideoHeaderList::createHeaderList( __attribute__ ((unused))TTFileBuffer* mpeg2_stream )
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

/*! ////////////////////////////////////////////////////////////////////////////
 * Read an idd-index file from idd_stream and create a header list from it
 */
long TTVideoHeaderList::readIndexFile( __attribute__ ((unused))TTFileBuffer* idd_stream )
{
  return 0;
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Write an idd-index file from current header list
 */
long TTVideoHeaderList::writeIndexFile( __attribute__ ((unused))TTFileBuffer* idd_stream )
{
  return 0;
}

/*! ////////////////////////////////////////////////////////////////////////////
 * Check an idd-index file against an mpeg2 stream
 */
bool TTVideoHeaderList::checkIndexFile( __attribute__ ((unused))TTFileBuffer* idd_stream, 
					__attribute__ ((unused))TTFileBuffer* mpeg2_stream )
{
  return false;
}

/*! ///////////////////////////////////////////////////////////////////////////
 * Sort the header list by header offset
 */
void TTVideoHeaderList::sort()
{
  qSort( begin(), end(), videoHeaderListCompareItems );
}

/*! ////////////////////////////////////////////////////////////////////////////
 * compare routine for sort
 */
bool videoHeaderListCompareItems( TTAVHeader* head_1, TTAVHeader* head_2 )
{
  if ( head_1->headerOffset() < head_2->headerOffset() )
    return true;
  else
    return false;
}
