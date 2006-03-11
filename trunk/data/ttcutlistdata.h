/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / ttcut.tritime.org                       */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttcutlistdata.h                                                 */
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

#ifndef TTCUTLISTDATA_H
#define TTCUTLISTDATA_H

#include "../common/ttcut.h"
#include "../common/ttmessagelogger.h"
#include "../avstream/ttavheader.h"
#include "../avstream/ttavstream.h"
#include "../avstream/ttmpeg2videostream.h"
#include "../gui/ttcutproject.h"

#include <QList>

class TTMpeg2VideoStream;
class TTCutListData;
class TTCutProject;
class QString;
class QFileInfo;

class TTCutListDataItem
{
  friend class TTCutListData;
  
  public:
    TTCutListDataItem();
    TTCutListDataItem(int cutIn, QTime cutInTime, int cutOut, QTime cutOutTime, QTime lengthTime, off64_t lengthBytes);

    int   getCutInIndex() const;
    int   getCutOutIndex() const;
    int   getCutInFrameType() const;
    int   getCutOutFrameType() const;
    QTime getCutInTime() const;
    QTime getCutOutTime() const;
    
  private:
    int     cutInIndex;
    int     cutOutIndex;
    int     cutInFrameType;
    int     cutOutFrameType;
    QTime   cutInTime;
    QTime   cutOutTime;
    QTime   cutLengthTime;
    off64_t cutLengthBytes;
};


class TTCutListData
{
  public:
    TTCutListData(TTMpeg2VideoStream* sv);
    ~TTCutListData();

    TTMpeg2VideoStream* videoStream();
    int addItem(int cutInIndex, int cutOutIndex);
    int addCutPosition(int cutInIndex, int cutOutIndex, int order);
    int updateItem( int index, int cutInIndex, int cutOutIndex);
    int cutInPos(int index);
    int cutInPosAt(int index);
    int cutOutPos(int index);
    int cutOutPosAt(int index);
    void setCutOutPosAt(int index, int cutOut);
    QString streamFileName();
    QString cutInPosString(int index);
    QString cutOutPosString(int index);
    QString cutLengthString(int index);
    const TTCutListDataItem& at(int index);
    int count();
    void deleteAll();
    void removeAt(int index);
    void swap(int a, int b);
    void print();
    void writeToProject(TTCutProject* prj);

  private:
    QList<TTCutListDataItem> data;
    TTMessageLogger*    log;
    TTMpeg2VideoStream* mpegStream;
};
#endif
