/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : tttranscode.cpp                                                 */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 08/07/2005 */
/* MODIFIED: b. altendorf                                    DATE: 03/18/2006 */
/* MODIFIED: b. altendorf                                    DATE: 04/18/2007 */
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

#include "tttranscode.h"

#include <QTimer>
#include <QTextStream>

//#define TTTRANSCODE_DEBUG

#define EVENT_LOOP_INTERVALL 1

const char c_name[] = "TTTranscodeProvider";

/* ////////////////////////////////////////////////////////////////////////////
 * Create the process form for displaying the output of the encode
 */
TTTranscodeProvider::TTTranscodeProvider( )
  : TTProcessForm( TTCut::mainWindow )
{
  // message logger instance
  log = TTMessageLogger::getInstance();
  
  QString str_head = "starting encoder >>>transcode -y ffmpeg<<<";

  str_command       = "transcode";
  transcode_success = false;

  setModal( true );
  addLine( str_head );
  show();  
  qApp->processEvents();
}

/* /////////////////////////////////////////////////////////////////////////////
 * Clean up used resources
 */
TTTranscodeProvider::~TTTranscodeProvider()
{
  close();
  qApp->processEvents();
}

/* /////////////////////////////////////////////////////////////////////////////
 * Parameter for the encoder
 */
void TTTranscodeProvider::setParameter( TTEncodeParameter& enc_par )
{
#if defined (TTTRANSCODE_DEBUG)
  log->debugMsg( c_name, "----------------------------------------------------" );
  log->debugMsg( c_name, "transcode parameter:" );
  log->debugMsg( c_name, "----------------------------------------------------" );
  log->debugMsg( c_name, "avi-file    : %s",    qPrintable(enc_par.avi_input_finfo.absoluteFilePath()) );
  log->debugMsg( c_name, "mpeg-file   : %s",    qPrintable(enc_par.mpeg2_output_finfo.absoluteFilePath()) );
  log->debugMsg( c_name, "widhtxheight: %dx%d", enc_par.video_width,enc_par.video_height );
  log->debugMsg( c_name, "aspect-code : %d",    enc_par.video_aspect_code );
  log->debugMsg( c_name, "bitrate     : %f",    enc_par.video_bitrate );
  log->debugMsg( c_name, "----------------------------------------------------" );
#endif

  //transcode -i encode.avi --pre_clip 0 -y ffmpeg --export_prof dvd-pal --export_asr 2 -o encode
  QString str_aspect;
  str_aspect.sprintf("%d",enc_par.video_aspect_code );

  QString str_format;
  str_format.sprintf("%dx%d", enc_par.video_width, enc_par.video_height);

  QString str_bitrate;
  str_bitrate.sprintf("%f", enc_par.video_bitrate);

  strl_command_line.clear();

  strl_command_line << "-i"
		    << enc_par.avi_input_finfo.absoluteFilePath()
		    << "--pre_clip"
		    << "0"
		    << "-y"
		    << "mpeg2enc"
        << "-F"
        << "8,\"-v 1 -q 3\""
		    << "--export_prof"
		    << "dvd-pal"
		    << "--export_asr"
		    << str_aspect
		    << "-o"
		    << enc_par.mpeg2_output_finfo.absoluteFilePath();

  //log->infoMsg(c_name, strl_command_line.join(" "));
}

/* /////////////////////////////////////////////////////////////////////////////
 * Create encoder process and start it
 */
bool TTTranscodeProvider::encodePart( )    
{
  int update        = EVENT_LOOP_INTERVALL;     //update intervall for local event loop
  transcode_success = false;      
  
  // create the process object for transcode
  proc = new QProcess();
  
  // read both channels: stderr and stdout
  proc->setReadChannelMode( QProcess::MergedChannels );

  // signal and slot connection
  connect(proc, SIGNAL(error(QProcess::ProcessError)),       SLOT(onProcError(QProcess::ProcessError)));
  connect(proc, SIGNAL(finished(int, QProcess::ExitStatus)), SLOT(onProcFinished(int, QProcess::ExitStatus)));  
  connect(proc, SIGNAL(readyRead()),                         SLOT(onProcReadOut()) );
  connect(proc, SIGNAL(started()),                           SLOT(onProcStarted()) );
  connect(proc, SIGNAL(stateChanged(QProcess::ProcessState)),SLOT(onProcStateChanged(QProcess::ProcessState)));

  // start the process; if successfully started() was emitted otherwise error()
  proc->start(str_command, strl_command_line);
  
  // we must wait until the process has finished
  while (proc->state() == QProcess::Starting ||
         proc->state() == QProcess::Running     ) {
    update--;
    if ( update == 0 ) {
      qApp->processEvents();
      update = EVENT_LOOP_INTERVALL;
    }
  }

  qApp->processEvents();
  
  delete proc;
  proc = NULL;

  return transcode_success;
}

/* /////////////////////////////////////////////////////////////////////////////
 * Reimplement the closeEvent to avoid closing the process window while the
 * process is in running state
 */
void TTTranscodeProvider::closeEvent(QCloseEvent *event)
{
  if (proc != NULL && proc->state() == QProcess::Running)
    event->ignore();
  else
    event->accept();
}

/* /////////////////////////////////////////////////////////////////////////////
 * This signal is emitted once every time new data is available for reading from 
 * the device.
 */
void TTTranscodeProvider::onProcReadOut()
{
  QString    line;
  QByteArray ba;

  ba = proc->readAll();

  QTextStream out(&ba);

  while (!out.atEnd()) {
    line = out.readLine();
    //log->infoMsg(c_name, "* %s", qPrintable(line));
    addLine(line);
  }
}

/* /////////////////////////////////////////////////////////////////////////////
 * This signal is emitted when the process has started; state() returns Running
 */
void TTTranscodeProvider::onProcStarted()
{
  QString    line;
  QByteArray ba;

  ba = proc->readAll();
  QTextStream out(&ba);

  line = out.readLine();
  //log->infoMsg(c_name, line);
  addLine( line );
}

/* /////////////////////////////////////////////////////////////////////////////
 * This signal is emitted when the process finishes
 */
void TTTranscodeProvider::onProcFinished(int e_code, QProcess::ExitStatus e_status)
{
  QString procMsg;

  switch (e_status) {
    case QProcess::NormalExit:
      //log->debugMsg(c_name, "The process ecxited normally: %d", e_code);
      procMsg = tr("Transcode exit normally ... done(0)");
      transcode_success = true;
      break;
    case QProcess::CrashExit:
      //log->debugMsg(c_name, "The process crashed: %d", e_code);
      procMsg = tr("Transcode crashed");
      transcode_success = false;
      break;
    default:
      //log->debugMsg(c_name, "Unknown process exit status (!): %d", e_code);
      procMsg = tr("unknown process exit status");
      transcode_success = false;
      break;
  }
  addLine(procMsg);
  exit_code = e_code;
}

/* /////////////////////////////////////////////////////////////////////////////
 * This signal is emitted when an error occurs with the process
 */
void TTTranscodeProvider::onProcError(QProcess::ProcessError proc_error)
{
  QString errorMsg;
  
  switch (proc_error) {
    case QProcess::FailedToStart:
      errorMsg = tr("The process failed to start.");
      break;
    case QProcess::Crashed:
      errorMsg = tr("The process crashed some time after starting successfully.");
      break;
    case QProcess::Timedout:
      errorMsg = tr("The last waitFor...() function timed out.");
      break;
    case QProcess::WriteError:
      errorMsg = tr("An error occured when attempting to write to the process.");
      break;
    case QProcess::ReadError:
      errorMsg = tr("An error occured when attempting to read from the process.");
      break;
    case QProcess::UnknownError:
    default:
      errorMsg = QString(tr("An unknown error occured: %1")).arg(proc_error);
      break;
  }
  addLine(errorMsg);
  //log->errorMsg(c_name, qPrintable(errorMsg));
  transcode_success = false;
}

/* /////////////////////////////////////////////////////////////////////////////
 * This signal is emitted whenever the state changed
 */
void TTTranscodeProvider::onProcStateChanged(QProcess::ProcessState proc_state)
{
  QString stateMsg;

  switch (proc_state) {
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

  //log->debugMsg(c_name, stateMsg);
  addLine(stateMsg);
}

/* /////////////////////////////////////////////////////////////////////////////
 * Kills the current process, causing it to exit immediately
 */
void TTTranscodeProvider::onProcKill( )
{
  //log->debugMsg(c_name, "Kill the current process, causing it to exit immediately.");
  transcode_success = false;
  proc->kill();
}
