/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / ttcut.tritime.org                       */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttaudiolistdata.cpp                                             */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 02/26/2006 */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTAUDIOLISTDATA
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

#include "ttaudiolistdata.h"

#include "../avstream/ttavheader.h"

#include <QFileInfo>
#include <QString>


const char oName[] = "TTAudioListData";

//! This class represent an item in the list
TTAudioListDataItem::TTAudioListDataItem()
{
  audioStream = NULL;
}

TTAudioListDataItem::TTAudioListDataItem(QFileInfo fInfo, TTAudioStream* aStream)
{
  audioFileInfo = fInfo;
  audioStream   = aStream;
}

QFileInfo TTAudioListDataItem::getFileInfo()
{
  return audioFileInfo;
}

TTAudioStream* TTAudioListDataItem::getAudioStream()
{
  return audioStream;
}

QString TTAudioListDataItem::getFileName()
{
  return audioStream->fileName();
}

QString TTAudioListDataItem::getLength()
{
  return audioLength;
}

QString TTAudioListDataItem::getVersion()
{
  return audioVersion;
}

QString TTAudioListDataItem::getMode()
{
  return audioMode;
}

QString TTAudioListDataItem::getBitrate()
{
  return audioBitrate;
}

QString TTAudioListDataItem::getSamplerate()
{
  return audioSamplerate;
}


QString TTAudioListDataItem::getDelay()
{
  return audioDelay;
}


//! Construct the audio list data object
TTAudioListData::TTAudioListData()
{
  log = TTMessageLogger::getInstance();
}

//! Destruct object
TTAudioListData::~TTAudioListData()
{
}

//! Add item to list
int TTAudioListData::addItem(QString fName, TTAudioStream* aStream)
{
  TTAudioHeader* header;
  QFileInfo fInfo(fName);

  TTAudioListDataItem item(fInfo, aStream);

  // audio stream have constant properties, so we can use the first header
  header = aStream->headerAt(0);

  item.audioLength = QString("%1 (%2 MB)")
  			.arg(aStream->streamLengthTime().toString("hh:mm:ss.zzz"))
  			.arg((double)aStream->streamLengthByte()/1024.0/1024.0);
  //item.audioLength.sprintf("%s (%0.2lf MB)",
  //    qPrintable(aStream->streamLengthTime().toString("hh:mm:ss.zzz")),
  //    (double)aStream->streamLengthByte()/1024.0/1024.0);

  item.audioVersion    = header->descString();
  item.audioBitrate    = header->bitRateString();
  item.audioSamplerate = header->sampleRateString();
  item.audioMode       = header->modeString();

  // FIXME: use real delay value for audio delay
  item.audioDelay      = "0";

  data.append(item);

  // return current index position in data list
  return data.count()-1;
}

//! Returns the data item at position index
TTAudioListDataItem& TTAudioListData::itemAt(int index)
{
  return data[index];
}

//! Returns the audio stream object at position index
TTAudioStream* TTAudioListData::audioStreamAt(int index)
{
  return data[index].audioStream;
}

//! Returns the number of entries in list
int  TTAudioListData::count()
{
  return data.count();
}

//! Delete all item from list
void TTAudioListData::deleteAll()
{
  for (int i = 0; i < data.count(); i++)
  {
    if (data[i].audioStream != 0)
    {
      delete data[i].audioStream;
      data[i].audioStream = 0;
    }
  }

  data.clear();
}

//! Remove item at position index from list
void TTAudioListData::removeAt(int index)
{
  if ( data[index].audioStream != 0 )
  {
    delete data[index].audioStream;
    data[index].audioStream = 0;
  }
  data.removeAt(index);
}

//! Swap items at positions a and b
void TTAudioListData::swap(int a, int b)
{
  data.swap(a, b);
}

//! Print the data list for debug purpose
void TTAudioListData::print()
{
  log->infoMsg(oName, "audio-list data");
  log->infoMsg(oName, "----------------------------------------");

  for(int i=0; i < data.count(); i++) {
    log->infoMsg(oName, "audio-File: %s", TTCut::toAscii(data[i].audioStream->fileName()));
  }
}

//! Write audio list to projecfile
void TTAudioListData::writeToProject(TTCutProject* prj, int nVideoIndex)
{
  for (int i=0; i < data.count(); i++) {
    prj->writeAudioFileName( data[i].audioFileInfo.absoluteFilePath(), nVideoIndex );
  }
}

