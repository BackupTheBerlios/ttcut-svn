/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : tttranscode.h                                                   */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 08/07/2005 */
/* MODIFIED: b. altendorf                                    DATE: 03/18/2006 */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTTRANSCODE
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

#ifndef TTTRANSCODE_H
#define TTTRASNCODE_H

#include "../common/ttmessagelogger.h"
#include "../common/ttcut.h"
#include "../gui/ttprocessform.h"
#include "ttencodeparameter.h"

#include <QFileInfo>
#include <QProcess>
#include <QCloseEvent>

class TTVideoStream;
class TTEncodeParameter;

class TTTranscodeProvider : public QObject
{
  Q_OBJECT

  public:
    TTTranscodeProvider(TTEncodeParameter& enc_par);
    ~TTTranscodeProvider();

    bool encodePart(TTVideoStream* vStream, int start, int end);

  public slots:
    void onProcReadOut();
    void onProcStarted();
    void onProcFinished(int exit_code, QProcess::ExitStatus );
    void onProcError(QProcess::ProcessError proc_error);
    void onProcStateChanged(QProcess::ProcessState proc_state);
    void onProcKill();

  signals:
    void processOutput(const QString& msg);

  protected:
    void buildCommandLine();
    void writeAVIFile(TTVideoStream* vs, int start, int end);
    void connectSignals(QProcess* proc);
    void procOutput();

  private:
    TTMessageLogger*  log;
    TTEncodeParameter enc_par;
    QProcess*         proc;
    QString           str_command;
    QStringList       strl_command_line;
    int               exit_code;
    bool              transcode_success;
};

#endif //TTTRANSCODE_H  
   
