/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttmplexprovider.h                                               */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 03/11/2006 */
/* MODIFIED: b. altendorf                                    DATE: 04/18/2007 */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTMPLEXPROVIDER
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

#ifndef TTMPLEXPROVIDER_H
#define TTMPLEXPROVIDER_H

#include "../common/ttmessagelogger.h"
#include "../common/ttcut.h"
#include "../data/ttmuxlistdata.h"
#include "../gui/ttprocessform.h"

#include <QCloseEvent>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QHash>

class TTMuxListData;

class TTMplexProvider : public TTProcessForm
{
  Q_OBJECT

  public:
    TTMplexProvider();
    ~TTMplexProvider();

    void writeMuxScript(TTMuxListData* muxData);
    bool mplexPart(TTMuxListData* muxData, int index);

  protected:
    void procOutput();
    void deleteElementaryStreams();
    int  createVerboseHash();
    int  createFormatHash();

  public slots:
    void closeEvent(QCloseEvent *event);
    void onButtonOkClicked();
    void onProcError(QProcess::ProcessError procError);
    void onProcReadOut();
    void onProcStarted();
    void onProcFinished(int exitCode, QProcess::ExitStatus);
    void onProcStateChanged(QProcess::ProcessState procState);
    void onProcKill();

  private:
    TTMessageLogger*    log;
    QProcess*           proc;
    QString             commandStr;
    QStringList         commandStrList;
    int                 exitCode;
    bool                mplexSuccess;
    TTMuxListData*      currentMuxList;
    int                 currentIndex;
    QHash<QString, int> verbose;
    QHash<QString, int> format;
};


#endif //TTMPLEXPROVIDER
