/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / ttcut.tritime.org                       */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttaudiolistdata.h                                               */
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

#ifndef TTAUDIOLISTDATA_H
#define TTAUDIOLISTDATA_H

#include "../common/ttmessagelogger.h"
#include "../avstream/ttavheader.h"
#include "../avstream/ttavstream.h"
#include "../gui/ttcutproject.h"

#include <QList>

class TTAudiostream;
class TTCutProject;
class QString;
class QFileInfo;

class TTAudioListDataItem
{
  friend class TTAudioListData;

  public:
    TTAudioListDataItem();
    TTAudioListDataItem(QFileInfo fInfo, TTAudioStream* aStream);

    QFileInfo      getFileInfo();
    TTAudioStream* getAudioStream();
    QString        getFileName();
    QString        getLength();
    QString        getVersion();
    QString        getMode();
    QString        getBitrate();
    QString        getSamplerate();
    QString        getDelay();

  private:
    QFileInfo      audioFileInfo;
    TTAudioStream* audioStream;
    QString        audioLength;
    QString        audioVersion;
    QString        audioMode;
    QString        audioSamplerate;
    QString        audioBitrate;
    QString        audioDelay;
};


class TTAudioListData
{
  public:
    TTAudioListData();
    ~TTAudioListData();

    int  addItem(QString fName, TTAudioStream* aStream);
    TTAudioListDataItem& itemAt(int index);
    TTAudioStream* audioStreamAt(int index);
    int  count();
    void deleteAll();
    void removeAt(int index);
    void swap(int a, int b);
    void print();
    void writeToProject(TTCutProject* prj, int nVideoIndex);

  private:
    TTMessageLogger* log;
    QList<TTAudioListDataItem>data;

};

#endif //TTAUDIOLISTDATA_H
