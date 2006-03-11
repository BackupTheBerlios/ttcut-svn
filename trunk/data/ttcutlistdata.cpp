/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / ttcut.tritime.org                       */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttcutlistdata.cpp                                               */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 02/26/2006 */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTCUTLISTDATA
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

#include "ttcutlistdata.h"

#include <QFileInfo>
#include <QString>

const char oName[] = "TTCutListData";
/*!
 * This class represent an item in the list
 */
TTCutListDataItem::TTCutListDataItem()
{
}

TTCutListDataItem::TTCutListDataItem(int cutIn, QTime cInTime, int cutOut, QTime cOutTime, QTime lengthTime, off64_t lengthBytes)
{
  cutInIndex     = cutIn;
  cutOutIndex    = cutOut;
  cutInTime      = cInTime;
  cutOutTime     = cOutTime;
  cutLengthTime  = lengthTime;
  cutLengthBytes = lengthBytes;
}

int TTCutListDataItem::getCutInIndex()const
{
  return cutInIndex;
}

int TTCutListDataItem::getCutOutIndex() const
{
  return cutOutIndex;
}

int TTCutListDataItem::getCutInFrameType() const
{
  return cutInFrameType;
}

int TTCutListDataItem::getCutOutFrameType() const
{
  return cutOutFrameType;
}

QTime TTCutListDataItem::getCutInTime() const
{
  return cutInTime;
}

QTime TTCutListDataItem::getCutOutTime() const
{
  return cutOutTime;
}

/*!
 * This class represents the audio list object
 */
TTCutListData::TTCutListData(TTMpeg2VideoStream* sv)
{
  log = TTMessageLogger::getInstance();

  mpegStream = sv;
}

TTCutListData::~TTCutListData()
{
}

TTMpeg2VideoStream* TTCutListData::videoStream()
{
  return mpegStream;
}

int TTCutListData::addItem(int cutInIndex, int cutOutIndex)
{
  QTime cutInTime     = mpegStream->frameTime(cutInIndex);
  QTime cutOutTime    = mpegStream->frameTime(cutOutIndex);
  int numFrames       = cutOutIndex-cutInIndex;
  if ( numFrames < 0 ) numFrames *= -1;
  QTime cutLengthTime = ttFramesToTime( numFrames, mpegStream->frameRate() );  
  off64_t lengthBytes = mpegStream->frameOffset(cutOutIndex) - mpegStream->frameOffset(cutInIndex);
  if ( lengthBytes < 0 ) lengthBytes *= -1;

  //log->infoMsg(oName, "Insert cut position");
  //log->infoMsg(oName, "------------------------------------------");
  //log->infoMsg(oName, "Cut-In : %s (%d)",  qPrintable(cutInTime.toString("hh:mm:ss.zzz")), cutInIndex);
  //log->infoMsg(oName, "Cut-Out: %s (%d)",  qPrintable(cutOutTime.toString("hh:mm:ss.zzz")), cutOutIndex);
  //log->infoMsg(oName, "Length : %s (%lld)",qPrintable(cutLengthTime.toString("hh:mm:ss.zzz")), lengthBytes);
  //log->infoMsg(oName, "------------------------------------------");

  TTCutListDataItem item(cutInIndex, cutInTime, cutOutIndex, cutOutTime, cutLengthTime, lengthBytes);
  item.cutInFrameType = mpegStream->frameType(cutInIndex);
  item.cutOutFrameType = mpegStream->frameType(cutOutIndex);
  data.append(item);

  // return current index
  return data.count()-1;
}

int TTCutListData::addCutPosition(int cutInIndex, int cutOutIndex, __attribute__((unused))int order)
{
  return addItem(cutInIndex, cutOutIndex);
}

//! Update an existing cut list data item
int TTCutListData::updateItem( int index, int cutInIndex, int cutOutIndex)
{
  QTime cutInTime     = mpegStream->frameTime(cutInIndex);
  QTime cutOutTime    = mpegStream->frameTime(cutOutIndex);
  int numFrames       = cutOutIndex-cutInIndex;
  if ( numFrames < 0 ) numFrames *= -1;
  QTime cutLengthTime = ttFramesToTime( numFrames, mpegStream->frameRate() );  
  off64_t lengthBytes = mpegStream->frameOffset(cutOutIndex) - mpegStream->frameOffset(cutInIndex);
  if ( lengthBytes < 0 ) lengthBytes *= -1;

  //log->infoMsg(oName, "Update cut position at index: %d", index);
  //log->infoMsg(oName, "------------------------------------------");
  //log->infoMsg(oName, "Cut-In : %s (%d)",  qPrintable(cutInTime.toString("hh:mm:ss.zzz")), cutInIndex);
  //log->infoMsg(oName, "Cut-Out: %s (%d)",  qPrintable(cutOutTime.toString("hh:mm:ss.zzz")), cutOutIndex);
  //log->infoMsg(oName, "Length : %s (%lld)",qPrintable(cutLengthTime.toString("hh:mm:ss.zzz")), lengthBytes);
  //log->infoMsg(oName, "------------------------------------------");

  //TTCutListDataItem item = at(index);
  data[index].cutInIndex      = cutInIndex;
  data[index].cutInTime       = cutInTime;
  data[index].cutOutIndex     = cutOutIndex;
  data[index].cutOutTime      = cutOutTime;
  data[index].cutLengthTime   = cutLengthTime;
  data[index].cutLengthBytes  = lengthBytes;
  data[index].cutInFrameType  = mpegStream->frameType(cutInIndex);
  data[index].cutOutFrameType = mpegStream->frameType(cutOutIndex);

  // return current item count
  return data.count();
}
 
int TTCutListData::cutInPos(int index)
{
  return data.at(index).cutInIndex;
}

int TTCutListData::cutInPosAt(int index)
{
  return cutInPos(index);
}

int TTCutListData::cutOutPos(int index)
{
  return data.at(index).cutOutIndex;
}

int TTCutListData::cutOutPosAt(int index)
{
  return cutOutPos(index);
}

void TTCutListData::setCutOutPosAt(int index, int cutOut)
{
  data[index].cutOutIndex = cutOut;
}

QString TTCutListData::streamFileName()
{
  return mpegStream->fileName();
}

QString TTCutListData::cutInPosString(int index)
{
  QString result;

  TTCutListDataItem item = data.at(index);
  result.sprintf("%s (%d)", qPrintable(item.cutInTime.toString("hh:mm:ss")), item.cutInIndex);

  //log->infoMsg(oName, "cut-in: %s", qPrintable(result));
  return result;
}

QString TTCutListData::cutOutPosString(int index)
{
  QString result;

  TTCutListDataItem item = data.at(index);
  result.sprintf("%s (%d)", qPrintable(item.cutOutTime.toString("hh:mm:ss")), item.cutOutIndex);

  //log->infoMsg(oName, "cut-out: %s", qPrintable(result));
  return result;
}

QString TTCutListData::cutLengthString(int index)
{
  QString result;

  TTCutListDataItem item = data.at(index);
  result.sprintf("%s (%8.2lf MB)", qPrintable(item.cutLengthTime.toString("hh:mm:ss")), item.cutLengthBytes/1024.0/1024.0);

  //log->infoMsg(oName, "cut length: %s", qPrintable(result));
  return result;
}

//! Print the contentn if the cut list data for debug purpose
void TTCutListData::print()
{
  log->infoMsg(oName, "cut-list data");
  log->infoMsg(oName, "----------------------------------------");

  for(int i=0; i < data.count(); i++) {
    log->infoMsg(oName, "cut-in/cut-out: %8d | %8d", data.at(i).cutInIndex, data.at(i).cutOutIndex);
  }
}

//! Get data item at position index
const TTCutListDataItem& TTCutListData::at(int index)
{
  return data.at(index);
}

//! Return the number of items in data list
int TTCutListData::count()
{
  return data.count();
}

void TTCutListData::deleteAll()
{
  data.clear();
}

//! Remove entry at index from data list
void TTCutListData::removeAt(int index)
{
  data.removeAt(index);
}

//! Swap two entries in data list
void TTCutListData::swap(int a, int b)
{
  data.swap(a, b);
}


//! Write the cut list data to project file
void TTCutListData::writeToProject(TTCutProject* prj)
{
  int    start_pos;
  int    end_pos;

  prj->writeCutSection( true );

  // walk through the av cut list
  for (int i = 0; i < data.count(); i++ ) {
    start_pos = data[i].cutInIndex;
    end_pos   = data[i].cutOutIndex;

    prj->writeCutEntry( start_pos, end_pos );
  }
  prj->writeCutSection( false );
}

