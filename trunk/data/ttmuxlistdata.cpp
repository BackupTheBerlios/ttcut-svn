/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / ttcut.tritime.org                       */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttmuxlistdata.cpp                                               */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 03/11/2006 */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTMUXOLISTDATA
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

#include "ttmuxlistdata.h"

#include "../avstream/ttavheader.h"

#include <QFileInfo>
#include <QString>
#include <QStringList>

const char oName[] = "TTMuxListData";

//! This class represent an item in the list
TTMuxListDataItem::TTMuxListDataItem()
{
}

TTMuxListDataItem::TTMuxListDataItem(QString video, QStringList audio)
{
  videoFileName  = video;
  audioFileNames = audio;
}

QString TTMuxListDataItem::getVideoName()
{
  return videoFileName;
}

QStringList TTMuxListDataItem::getAudioNames()
{
  return audioFileNames;
}

//! Construct the audio list data object
TTMuxListData::TTMuxListData()
{
  log = TTMessageLogger::getInstance();
}

//! Destruct object
TTMuxListData::~TTMuxListData()
{
}

int TTMuxListData::addItem(QString video)
{
  bool newItem = true;
  int index;

  for (int i=0; i < data.count(); i++) {
    if (data[i].videoFileName == video) {
      index   = i;
      newItem = false;
      break;
    }
  }

  if (newItem) {
    TTMuxListDataItem item;

    item.videoFileName = video;
    data.append(item);
  } else {
    data[index].videoFileName = video;
    data[index].audioFileNames.clear();
  }

  return data.count()-1;
}

int TTMuxListData::addItem(QString video, QString audio)
{
  bool newItem = true;
  int index;

  for (int i=0; i < data.count(); i++) {
    if (data[i].videoFileName == video) {
      index   = i;
      newItem = false;
      break;
    }
  }

  if (newItem) {
    TTMuxListDataItem item;

    item.videoFileName = video;
    item.audioFileNames.append(audio);
    data.append(item);
  } else {
    data[index].videoFileName = video;
    data[index].audioFileNames.clear();
    data[index].audioFileNames.append(audio);
  }

  return data.count()-1;
}

//! Add item to list
int TTMuxListData::addItem(QString video, QStringList audio)
{
  bool newItem = true;
  int index;

  for (int i=0; i < data.count(); i++) {
    if (data[i].videoFileName == video) {
      index   = i;
      newItem = false;
      break;
    }
  }

  if (newItem) {
    TTMuxListDataItem item;

    item.videoFileName  = video;
    item.audioFileNames = audio;
    data.append(item);
  } else {
    data[index].videoFileName = video;
    data[index].audioFileNames.clear();
    data[index].audioFileNames = audio;
  }

  return data.count()-1;
}

void TTMuxListData::appendAudioName(int index, QString audio)
{
  qDebug("append audio file: %s", qPrintable(audio));
  data[index].audioFileNames.append(audio);
}

//! Returns the data item at position index
TTMuxListDataItem& TTMuxListData::itemAt(int index)
{
  return data[index];
}

//! Returns the number of entries in list
int  TTMuxListData::count()
{
  return data.count();
}

//! Delete all item from list
void TTMuxListData::deleteAll()
{
  data.clear();
}

//! Remove item at position index from list
void TTMuxListData::removeAt(int index)
{
  data.removeAt(index);
}

//! Print the data list for debug purpose
void TTMuxListData::print()
{
  log->infoMsg(oName, "mux-list data");
  log->infoMsg(oName, "----------------------------------------");

  for(int i=0; i < data.count(); i++) {
    log->infoMsg(oName, "--------------------------------");
    log->infoMsg(oName, "video-file: %s", qPrintable(data[i].getVideoName()));
    QStringList audioNames = data[i].getAudioNames();
    for (int j=0; j < audioNames.size(); j++) {
      log->infoMsg(oName, "audio-file: %s", qPrintable(audioNames.at(j)));
    }
    log->infoMsg(oName, "--------------------------------");
  }
}

