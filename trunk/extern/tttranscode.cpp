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

//#define TTTRANSCODE_DEBUG

#if defined (TTTRANSCODE_DEBUG)
const char c_name[] = "TTTRANSCODE   : ";
#endif

TTTranscodeProvider::TTTranscodeProvider( )
  : TTProcessForm( TTCut::mainWindow )
{
  QString str_head = "starting encoder >>>transcode -y ffmpeg<<<";

  str_command = "transcode";

  setModal( true );
  addLine( str_head );
  show();  

  qApp->processEvents();
}


TTTranscodeProvider::~TTTranscodeProvider()
{
#if defined (TTTRANSCODE_DEBUG)
  qDebug( "%sdestructor call: close window",c_name );
#endif

  close();

  qApp->processEvents();
}

void TTTranscodeProvider::setParameter( TTEncodeParameter& enc_par )
{
#if defined (TTTRANSCODE_DEBUG)
  qDebug( "%s----------------------------------------------------",c_name );
  qDebug( "%stranscode parameter:",c_name );
  qDebug( "%s----------------------------------------------------",c_name );
  qDebug( "%savi-file    : %s",c_name,enc_par.avi_input_finfo.absoluteFilePath().ascii() );
  qDebug( "%smpeg-file   : %s",c_name,enc_par.mpeg2_output_finfo.absoluteFilePath().ascii() );
  qDebug( "%swidhtxheight: %dx%d",c_name,enc_par.video_width,enc_par.video_height );
  qDebug( "%saspect-code : %d",c_name,enc_par.video_aspect_code );
  qDebug( "%sbitrate     : %f",c_name,enc_par.video_bitrate );
  qDebug( "%s----------------------------------------------------",c_name );
#endif

  //transcode -i encode.avi --pre_clip 0 -y ffmpeg --export_prof dvd-pal --export_asr 2 -o encode
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
		    << "2"
		    << "-o"
		    << enc_par.mpeg2_output_finfo.absoluteFilePath();
}

bool TTTranscodeProvider::encodePart( )    
{
  // create the process object for transcode
  transcode_proc = new QProcess();

  // read both channels: stderr and stdout
  transcode_proc->setReadChannelMode( QProcess::MergedChannels );

  // signal and slot connection
  connect( transcode_proc, SIGNAL(readyRead()),this,SLOT(transcodeReadOut()) );
  connect( transcode_proc, SIGNAL(started()),this,SLOT(transcodeStarted()) );
  connect( transcode_proc, SIGNAL(finished(int)),this,SLOT(transcodeFinish(int)) );
  connect( transcode_proc, SIGNAL(error(QProcess::ProcessError)), this, SLOT(transcodeError(QProcess::ProcessError)));
  connect( transcode_proc, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(transcodeState(QProcess::ProcessState)));

#if defined (TTTRANSCODE_DEBUG)
  qDebug( "%sstarting transcode...",c_name );
#endif

  transcode_proc->start( str_command, strl_command_line );
  
  if ( !transcode_proc->waitForStarted() )
  {
#if defined (TTTRANSCODE_DEBUG)
    qDebug( "%serror in waitForStarted: %d",c_name,transcode_proc->state() );
#endif

    delete transcode_proc;

    return false;
  }

  while ( transcode_proc->state() == QProcess::Running )
    qApp->processEvents();

  //  if ( !transcode_proc->waitForFinished() )
  //{
  //#if defined (TTTRANSCODE_DEBUG)
    //qDebug( "%serror in waitForFinished: %d",c_name,transcode_proc->state());
  //#endif

    //delete transcode_proc;

    //return false;
  //}

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
  qDebug( "%sexit encodePart",c_name );
#endif
  return true;
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
	i_pos = 0;
      }
    }
  }
  else
  {
    line = "transcode finished... done(0)";
    addLine( line );
  }
}

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
      i_pos = 0;
    }
  }
}

void TTTranscodeProvider::transcodeFinish( int e_code )
{
#if defined (TTTRANSCODE_DEBUG)
  qDebug( "%stranscode finish: %d",c_name,e_code );
#endif

  exit_code = e_code;
}


void TTTranscodeProvider::transcodeError( QProcess::ProcessError proc_error )
{
#if defined (TTTRANSCODE_DEBUG)
  qDebug( "%serror: %d",c_name, proc_error );
#endif
}


void TTTranscodeProvider::transcodeState( QProcess::ProcessState proc_state )
{
#if defined (TTTRANSCODE_DEBUG)
  qDebug( "%sstate changed: %d",c_name, proc_state );
#endif
}
