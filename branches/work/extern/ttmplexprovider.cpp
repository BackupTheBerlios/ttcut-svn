/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttmplexprovider.cpp                                             */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 03/11/2006 */
/* MODIFIED: b. altendorf                                    DATE: 04/11/2007 */
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

#include "ttmplexprovider.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

#define EVENT_LOOP_INTERVALL 10

/**
 *Usage: mplex [params] -o <output filename pattern> <input file>... 
 *%d in the output file name is by segment count
 *where possible params are:
 *--verbose|-v num
 *  Level of verbosity. 0 = quiet, 1 = normal 2 = verbose/debug
 *--format|-f fmt
 *  Set defaults for particular MPEG profiles
 *  [0 = Generic MPEG1, 1 = VCD, 2 = user-rate VCD, 3 = Generic MPEG2,
 *   4 = SVCD, 5 = user-rate SVCD
 *   6 = VCD Stills, 7 = SVCD Stills, 8 = DVD with NAV sectors, 9 = DVD]
 *--mux-bitrate|-r num
 *  Specify data rate of output stream in kbit/sec
 *  (default 0=Compute from source streams)
 *--video-buffer|-b num [, num...] 
 *  Specifies decoder buffers size in kB.  [ 20...2000]
 *--lpcm-params | -L samppersec:chan:bits [, samppersec:chan:bits]
 *--mux-limit|-l num
 *  Multiplex only num seconds of material (default 0=multiplex all)
 *--sync-offset|-O num ms|s|mpt
 *  Specify offset of timestamps (video-audio) in mSec
 *--sector-size|-s num
 *  Specify sector size in bytes for generic formats [256..16384]
 *--vbr|-V
 *  Multiplex variable bit-rate video
 *--packets-per-pack|-p num
 *  Number of packets per pack generic formats [1..100]
 *--system-headers|-h
 *  Create System header in every pack in generic formats
 *--max-segment-size|-S size
 *  Maximum size of output file(s) in Mbyte (default: 0) (no limit)
 *--split-segment|-M
 *  Simply split a sequence across files rather than building run-out/run-in
 *--workaround|-W workaround [, workaround ]
 *--help|-?
 *  Print this lot out!
 */

const char cName[] = "TTMplexProvider";

/* /////////////////////////////////////////////////////////////////////////////
 * Constructor
 */
TTMplexProvider::TTMplexProvider() : TTProcessForm(TTCut::mainWindow)
{
  // message logger instance
  log = TTMessageLogger::getInstance();

  mplexSuccess = false;

  if (TTCut::muxPause)
  {
    setModal(true);
    connect(this, SIGNAL(btnCancelClicked()), this, SLOT(onButtonOkClicked()));
  }
  else
    setModal(false);
}

void TTMplexProvider::onButtonOkClicked()
{
  close();
}

/* /////////////////////////////////////////////////////////////////////////////
 * Clean up
 */
TTMplexProvider::~TTMplexProvider()
{
}

/* /////////////////////////////////////////////////////////////////////////////
 * Write mux data in muxscript
 * Theire should be only one muxscript for one session. Purpose of the muxscript
 * is batch-muxing all cutted videos from the current session, after(!) quitting
 * TTCut.
 */
void TTMplexProvider::writeMuxScript(TTMuxListData* muxData)
{
  QStringList mplexCmd;
  QString     outputPath;
  QString     outputFile;
  QFileInfo   outputFileInfo;
  QString     videoFile;
  QString     audioFile;
  QFileInfo   muxInfo(QDir(TTCut::cutDirPath), "muxscript.sh" );
  QFile       muxFile(muxInfo.absoluteFilePath());

  if (muxInfo.exists())
    muxFile.remove();

  muxFile.open(QIODevice::WriteOnly | QIODevice::Text);

  QTextStream muxOutStream(&muxFile);

  muxOutStream << "# TTCut - Mplex script ver. 0.10.2" << "\n";
  muxOutStream << "#!/bin/sh" << "\n";
  muxOutStream << "#\n";

  for (int i=0; i < muxData->count(); i++) 
  {
    mplexCmd.clear();

    mplexCmd   << "mplex"
               << "-f8"
               << "-o";

    outputFileInfo.setFile(muxData->videoFileAt(i));

    outputPath = TTCut::muxOutputPath;

    if (outputPath.isNull() || outputPath.isEmpty())
      outputPath = outputFileInfo.absolutePath();

    if (outputPath.lastIndexOf("/") < outputPath.length()-1)
      outputPath += "/";

    // video output file name
    outputFile = "\""
               + outputPath
               + ttChangeFileExt(muxData->videoFileNameAt(i), "mpg")
               + "\"";

    mplexCmd   << outputFile.toLatin1().constData();
    
    // video input file name
    videoFile  = "\""
               + muxData->videoFileAt(i)
               + "\"";

    mplexCmd   << videoFile.toLatin1().constData();

    // audio file names
    for (int j=0; j < muxData->numAudioFilesAt(i); j++) 
    {
      audioFile = "\""
                + muxData->audioFileAt(i, j)
                + "\"";
      mplexCmd  << audioFile.toLatin1().constData();
    }

    // join all items in string list together separated by spaces and write
    // the resulting string to the output buffer
    muxOutStream << mplexCmd.join(" ") << "\n";
    muxOutStream << "#\n";
  }

  muxFile.flush();
  muxFile.close();

  // make muxscript executeable by the owner/user
  bool result = muxFile.setPermissions(
        QFile::ReadOwner|QFile::WriteOwner|QFile::ExeOwner|
        QFile::ReadUser |QFile::WriteUser |QFile::ExeUser |
        QFile::ReadGroup|QFile::ExeGroup);

  if (!result)
    log->errorMsg(cName, "Cannot modify permissions of the muxscript!");
}

/* /////////////////////////////////////////////////////////////////////////////
 * Call mplex to mux current stream.
 * Pupose is to mux the current (cutted) video after finishing the cut.
 */
bool TTMplexProvider::mplexPart(TTMuxListData* muxData, int index)
{
  QString        mplexCmd;
  QStringList    mplexArgs;
  QString        outputFile;
  QFileInfo      outputFileInfo;
  QString        outputPath;
  QString        videoFile;
  QString        audioFile;
  int  update  = EVENT_LOOP_INTERVALL;
  mplexSuccess = false;
  proc         = new QProcess();
  proc->setReadChannelMode( QProcess::MergedChannels );

  showOkButton(true);
  enableButton(false);
  show();
  qApp->processEvents();
  
  // signal and slot connection for QProcess
  connect(proc, SIGNAL(error(QProcess::ProcessError)),        SLOT(onProcError(QProcess::ProcessError)));
  connect(proc, SIGNAL(finished(int, QProcess::ExitStatus)),  SLOT(onProcFinished(int, QProcess::ExitStatus)));
  connect(proc, SIGNAL(readyRead()),                          SLOT(onProcReadOut()));
  connect(proc, SIGNAL(started()),                            SLOT(onProcStarted()));
  connect(proc, SIGNAL(stateChanged(QProcess::ProcessState)), SLOT(onProcStateChanged(QProcess::ProcessState)));

  mplexCmd   = "mplex";

  // initialize  mplex argument string list
  mplexArgs.clear();
  mplexArgs  << "-f8"
             << "-o";

  // video output file name
  outputFileInfo.setFile(muxData->videoFileAt(index));
  outputPath = TTCut::muxOutputPath;

  if (outputPath.isNull() || outputPath.isEmpty())
     outputPath = outputFileInfo.absolutePath();

  if (outputPath.lastIndexOf("/") < outputPath.length()-1)
    outputPath += "/";

  outputFile = outputPath + ttChangeFileExt(muxData->videoFileNameAt(index), "mpg");

  mplexArgs  << outputFile.toLatin1().constData();
    
  // video input file name
  videoFile  = muxData->videoFileAt(index);

  mplexArgs  << videoFile.toLatin1().constData();

  // audio file names
  for (int i=0; i < muxData->numAudioFilesAt(index); i++)
  {
    audioFile = muxData->audioFileAt(index, i);
    mplexArgs << audioFile.toLatin1().constData();
  }

  // info message in logfile
  log->infoMsg(cName, "Mplex command string: %s", mplexArgs.join(" ").toLatin1().constData());

  proc->start(mplexCmd, mplexArgs);

  // just a very simple event loop ;-)
  // we have to wait until the process has finished
  while (proc->state() == QProcess::Starting ||
         proc->state() == QProcess::Running     ) 
  {
    update--;
    if ( update == 0 ) 
    {
      qApp->processEvents();
      update = EVENT_LOOP_INTERVALL;
    }
  }

  return mplexSuccess;
}

// /////////////////////////////////////////////////////////////////////////////
// Event Processing
// /////////////////////////////////////////////////////////////////////////////
//

/* /////////////////////////////////////////////////////////////////////////////
 * Reimplement the closeEvent to avoid closing the process window while the
 * process is in running state
 */
void TTMplexProvider::closeEvent(QCloseEvent *event)
{
  if (proc != NULL && proc->state() == QProcess::Running)
    event->ignore();
  else
    event->accept();
}

/* /////////////////////////////////////////////////////////////////////////////
 * This signal is emitted when an error occurs with the process
 */
void TTMplexProvider::onProcError(QProcess::ProcessError procError)
{
  qDebug("proc error: %d", procError);
}

/* /////////////////////////////////////////////////////////////////////////////
 * This signal is emitted once every time new data is available for reading from
 * the device
 */
void TTMplexProvider::onProcReadOut()
{
  procOutput();
}

/* /////////////////////////////////////////////////////////////////////////////
 * This signal is emitted when the process has started;
 * state() returns running
 */
void TTMplexProvider::onProcStarted()
{
  procOutput();
}

/* /////////////////////////////////////////////////////////////////////////////
 * This signal is emitted when the process finishes
 */
void TTMplexProvider::onProcFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
  if (exitStatus == QProcess::NormalExit)
  {
    if (TTCut::muxDeleteES)
      qDebug("Delete ES not implemented");
  }

  enableButton(true);
  mplexSuccess  = true;
  this->exitCode = exitCode;
}

/* /////////////////////////////////////////////////////////////////////////////
 * This signal is emitted whenever the state changed
 */
void TTMplexProvider::onProcStateChanged(QProcess::ProcessState procState)
{
  QString stateMsg;

  switch (procState) {
    case QProcess::NotRunning:
      stateMsg = "The process is not running.";
      break;
    case QProcess::Starting:
      stateMsg = "The process is starting, but program has not yet been invoked.";
      break;
    case QProcess::Running:
      stateMsg = "The process is running and is ready for reading and writing.";
      break;
    default:
      stateMsg = "Unknown process state!";
      break;
  }

  //log->debugMsg(cName, stateMsg);
  addLine(stateMsg);
}

/* /////////////////////////////////////////////////////////////////////////////
 * Kills the current process, causing it to exit immediately
 */
void TTMplexProvider::onProcKill()
{
  enableButton(true);
  mplexSuccess = false;
  proc->kill();
}

/* /////////////////////////////////////////////////////////////////////////////
 * Write the process output to process form
 */
void TTMplexProvider::procOutput()
{
  if (proc == NULL)
    return;

  QString    line;
  QByteArray ba;

  ba = proc->readAll();

  QTextStream out(&ba);

  while (!out.atEnd()) 
  {
    line = out.readLine();
    //log->infoMsg(cName, "* %s", TTCut::toAscii(line));
    addLine(line);
  }
} 

// /////////////////////////////////////////////////////////////////////////////
// Settings
// /////////////////////////////////////////////////////////////////////////////
//
//! Level of verbosity (--verbose|-v num)
int TTMplexProvider::createVerboseHash()
{
  verbose["quiet"]         = 0;
  verbose["normal"]        = 1;
  verbose["verbose/debug"] = 2;

  return verbose.size();
}

//! Set defaults for particular MPEG profiles (--format|-f fmt)
int TTMplexProvider::createFormatHash()
{
  format["Generic MPEG1"]        = 0;
  format["VCD"]                  = 1;
  format["user-rate VCD"]        = 2;
  format["Generic MPEG2"]        = 3;
  format["SVCD"]                 = 4;
  format["user-rate VCD"]        = 5;
  format["VCD Stills"]           = 6;
  format["SVCD Stills"]          = 7;
  format["DVD with NAV sectors"] = 8;
  format["DVD"]                  = 9;

  return format.size();
}
