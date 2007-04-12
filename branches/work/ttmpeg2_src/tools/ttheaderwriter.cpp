/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTMPEG2 2007                                                    */
/* FILE     : ttheaderwriter.cpp                                              */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 03/28/2007 */
/* MODIFIED:                                                 DATE:            */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTHEADERWRITER
// ----------------------------------------------------------------------------

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

#include "ttheaderwriter.h"

#include <QString>
#include <QFileInfo>
#include <QFileDialog>

/*!
 * Constructor
 */
TTHeaderWriter::TTHeaderWriter(TTVideoIndexList* indexList, TTVideoHeaderList* headerList)
{
  this->indexList  = indexList;
  this->headerList = headerList;
  frameCount       = indexList->count();
  headerCount      = headerList->count();
}

// ////////////////////////////////////////////////////////////////////////////
// Write the video header to file.
//
void TTHeaderWriter::writeHeaderListToFile(QString fileName)
{
  QFile data(fileName);

  if (!data.open(QFile::WriteOnly | QFile::Truncate)) 
  {
    return;
  }

  data.resize(0);
  QTextStream out(&data);

  out << "Headerlist for file: " << fileName << endl;
  out << endl;
  out << "Headers: " << headerList->count() << endl;
  out << "Frames : " << indexList->count()  << endl;
  out << endl;

  out << qSetFieldWidth(16)
      << left
      << "Offset" 
      << qSetFieldWidth(8) 
      << left 
      << "Type" 
      << qSetFieldWidth(8) 
      << left 
      << "Info" 
      << qSetFieldWidth(0)
      << endl;

  out << "--------------------------------------------------------------" << endl;

  for (int i = 0; i < headerList->count(); i++)
  {
    out << qSetFieldWidth(16)
        << left
        << headerList->at(i)->headerOffset()
        << qSetFieldWidth(8)
        << left
        << hex
        << headerList->at(i)->headerType()
        << qSetFieldWidth(8)
        << left
        << writeHeaderInfo((TTMpeg2VideoHeader*)headerList->at(i))
        << qSetFieldWidth(0)
        << endl;
  }

  out.flush();
}

// ////////////////////////////////////////////////////////////////////////////
// Write suplemental header information
//
QString TTHeaderWriter::writeHeaderInfo(TTMpeg2VideoHeader* header)
{
  switch (header->headerType())
  {
    case TTMpeg2VideoHeader::sequence_start_code:
      return sequenceInfo((TTSequenceHeader*)header);

    case TTMpeg2VideoHeader::picture_start_code:
      return pictureInfo((TTPicturesHeader*)header);

    case TTMpeg2VideoHeader::group_start_code:
      return gopInfo((TTGOPHeader*)header);

    default:
      return "---";
  }
}

// ////////////////////////////////////////////////////////////////////////////
// Sequence header information
//
QString TTHeaderWriter::sequenceInfo(TTSequenceHeader* sequence)
{
  QString info;

  info.sprintf("Sequence: %s %dx%d %6.2f kBit/s", 
      sequence->aspectRatioText().toAscii().data(),
      sequence->horizontalSize(),
      sequence->verticalSize(),
      sequence->bitRateKbit());

  return info;
}

// ////////////////////////////////////////////////////////////////////////////
// Picture header information
//
QString TTHeaderWriter::pictureInfo(TTPicturesHeader* picture)
{
  QString info;

  info.sprintf("Picture: [%s/%d]", 
      picture->codingTypeString().toAscii().data(),
      picture->temporal_reference);

  return info;
}

// ////////////////////////////////////////////////////////////////////////////
// GOP header information
//
QString TTHeaderWriter::gopInfo(TTGOPHeader* gop)
{
  QString info;
  QString gopType = "closed";

  if (!gop->closed_gop)
    gopType = "open";

  info.sprintf("GOP: [%s/%d]",
      gopType.toAscii().data(),
      gop->pictures_in_gop);

  return info;
}
