/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : tttranscode.cpp                                                 */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 08/07/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTTRANSCODE
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

//#define TTTRANSCODE_DEBUG

const char c_name[] = "TTTRANSCODE";

// Create the process form for displaying the output of the encode
TTTranscodeProvider::TTTranscodeProvider( )
  : TTProcessForm( TTCut::mainWindow )
{
  log = TTMessageLogger::getInstance();
  log->infoMsg(c_name, "start transcode");
  
  QString str_head = "starting encoder >>>transcode -y ffmpeg<<<";

  str_command       = "transcode";
  transcode_success = false;

  setModal( true );
  addLine( str_head );
  show();  

  qApp->processEvents();
}

// clean up used resources
TTTranscodeProvider::~TTTranscodeProvider()
{
#if defined (TTTRANSCODE_DEBUG)
  qDebug( "%sdestructor call: close window",c_name );
#endif

  close();

  qApp->processEvents();
}

// parameter for the encoder
void TTTranscodeProvider::setParameter( TTEncodeParameter& enc_par )
{
#if defined (TTTRANSCODE_DEBUG)
  log->infoMsg( c_name, "----------------------------------------------------" );
  log->infoMsg( c_name, "transcode parameter:" );
  log->infoMsg( c_name, "----------------------------------------------------" );
  log->infoMsg( c_name, "avi-file    : %s",enc_par.avi_input_finfo.absoluteFilePath().ascii() );
  log->infoMsg( c_name, "mpeg-file   : %s",enc_par.mpeg2_output_finfo.absoluteFilePath().ascii() );
  log->infoMsg( c_name, "widhtxheight: %dx%d",enc_par.video_width,enc_par.video_height );
  log->infoMsg( c_name, "aspect-code : %d",enc_par.video_aspect_code );
  log->infoMsg( c_name, "bitrate     : %f",enc_par.video_bitrate );
  log->infoMsg( c_name, "----------------------------------------------------" );
#endif

  //transcode -i encode.avi --pre_clip 0 -y ffmpeg --export_prof dvd-pal --export_asr 2 -o encode
  QString str_aspect;
  str_aspect.sprintf("%d",enc_par.video_aspect_code );

  strl_command_line.clear();

  strl_command_line << "-i"
		    << enc_par.avi_input_finfo.absoluteFilePath()
		    << "--pre_clip"
		    << "0"
		    << "-y"
		    << "ffmpeg"
		    << "--export_prof"
		    << "dvd-pal"
		    << "--export_asr"
		    << str_aspect
		    << "-o"
		    << enc_par.mpeg2_output_finfo.absoluteFilePath();

  log->infoMsg(c_name, strl_command_line.join(" "));
}

// create encoder process and start it
bool TTTranscodeProvider::encodePart( )    
{
  int update = 100;

  
  // create the process object for transcode
  log->debugMsg( c_name, "before instantiate QProcess ;-)" );
  transcode_proc = new QProcess();
  
  // read both channels: stderr and stdout
  log->debugMsg( c_name, "set read channel mode ..." );
  transcode_proc->setReadChannelMode( QProcess::MergedChannels );

  // signal and slot connection
  log->debugMsg( c_name, "set signal and slot connection..." );
  connect( transcode_proc, SIGNAL(readyRead()),this,SLOT(transcodeReadOut()) );
  connect( transcode_proc, SIGNAL(started()),this,SLOT(transcodeStarted()) );
  connect( transcode_proc, SIGNAL(finished(int, QProcess::ExitStatus)),this,SLOT(transcodeFinish(int)) );
  connect( transcode_proc, SIGNAL(error(QProcess::ProcessError)), this, SLOT(transcodeError(QProcess::ProcessError)));
  connect( transcode_proc, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(transcodeState(QProcess::ProcessState)));


  // start the process; if successfully started() was emitted otherwise error()
  log->infoMsg( c_name, "start transcode process" );
  transcode_proc->start( str_command, strl_command_line );
  
  if ( !transcode_proc->waitForStarted() )
  {
    log->debugMsg( c_name, "error in waitForStarted: %d",transcode_proc->state() );

    delete transcode_proc;

    return transcode_success;
  }

  log->debugMsg( c_name, "start emergency timer..." );
  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(transcodeKill()));
  timer->start(60000);
  
  transcode_success = true;

  while ( transcode_proc->state() == QProcess::Running )
  {
    update--;
    if ( update == 0 )
    {
      qApp->processEvents();
      update = 100;
    }
  }

  if ( transcode_proc->state() == QProcess::Running )
  {
#if defined (TTTRANSCODE_DEBUG)
    qDebug( "%stry to kill the process...",c_name );
#endif
    transcode_proc->kill();
  }

#if defined (TTTRANSCODE_DEBUG)
  qDebug( "%sdelete process: %d",c_name,transcode_proc->state() );
#endif
  delete transcode_proc;

#if defined (TTTRANSCODE_DEBUG)
  qDebug( "%sexit encodePart: %d",c_name,transcode_success );
#endif
  return transcode_success;
}


void TTTranscodeProvider::transcodeReadOut()
{
  char       temp_str[101];
  int        i, i_pos;
  QString    line;
  QByteArray ba;

    
  if ( transcode_proc->state() == QProcess::Running )
  {
    ba = transcode_proc->readAll();

    log->debugMsg( c_name, "transcodeReadOut: %d", ba.size() );

    i_pos = 0;

    for ( i = 0; i < ba.size(); ++i) 
    {
      if ( ba.at(i) != '\n' && i_pos < 100)
      {
        temp_str[i_pos] = ba.at(i);
        i_pos++;
      }
      else
      {
        temp_str[i_pos] = '\0';
        line = temp_str;
        addLine( line );
        log->infoMsg(c_name, line);
        i_pos = 0;
      }
    }
  }
  else
  {
    line = "transcode finished... done(0)";
    addLine( line );
    log->infoMsg(c_name, line);
  }
}

// process successfully started
void TTTranscodeProvider::transcodeStarted()
{
  char       temp_str[101];
  int        i, i_pos;
  QString    line;
  QByteArray ba;

#if defined (TTTRANSCODE_DEBUG)
  qDebug( "%stranscode read std out ...",c_name );
#endif

  ba = transcode_proc->readAll();

  log->debugMsg( c_name, "byte array length: %d", ba.size() );
  
  i_pos = 0;

  for ( i = 0; i < ba.size(); ++i) 
  {
    if ( ba.at(i) != '\n' && i_pos < 100)
    {
      temp_str[i_pos] = ba.at(i);
      i_pos++;
    }
    else
    {
      temp_str[i_pos] = '\0';
      line = temp_str;
      addLine( line );
      log->infoMsg(c_name, line);
      i_pos = 0;
    }
  }
}

void TTTranscodeProvider::transcodeFinish( int e_code, QProcess::ExitStatus e_status )
{
  log->debugMsg( c_name, "transcode finish: %d/%d", e_code, e_status );

  exit_code = e_code;
}

// process error
void TTTranscodeProvider::transcodeError( __attribute__ ((unused))QProcess::ProcessError proc_error )
{
  log->errorMsg( c_name, "error: %d", proc_error );
}

// process state changed
void TTTranscodeProvider::transcodeState( __attribute__ ((unused))QProcess::ProcessState proc_state )
{
#if defined (TTTRANSCODE_DEBUG)
  qDebug( "%sstate changed: %d",c_name, proc_state );
#endif
}


void TTTranscodeProvider::transcodeKill( )
{
#if defined (TTTRANSCODE_DEBUG)
  qDebug( "%stranscode kill",c_name );
#endif

  qDebug( "%stime-out-event forces transcode to terminate (!)",c_name );

  transcode_success = false;
  transcode_proc->kill();
}
