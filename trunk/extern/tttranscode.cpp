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


const char c_name[] = "TTTRANSCODE   : ";

TTTranscodeProvider::TTTranscodeProvider( )
  :QProcess()
{
  QString str_head = "starting encoder >>>transcode -y ffmpeg<<<";

  str_command = "transcode";
  
  setReadChannel( QProcess::StandardOutput );
  setReadChannelMode( QProcess::SeparateChannels );

  proc_view = new TTProcessForm( TTCut::mainWindow );
  proc_view->setModal( true );
  proc_view->addLine( str_head );
  proc_view->show();  

  connect( (QProcess*)this, SIGNAL(readyRead()),this,SLOT(transcodeReadOut()) );
  connect( (QProcess*)this, SIGNAL(started()),this,SLOT(transcodeStarted()) );
  connect( (QProcess*)this, SIGNAL(finished(int)),this,SLOT(transcodeFinish(int)) );
}


TTTranscodeProvider::~TTTranscodeProvider()
{
  qDebug( "%sterminating transcode process ...",c_name );
  //terminate();
  kill();
  proc_view->close();
  delete proc_view;
}

void TTTranscodeProvider::setParameter( TTEncodeParameter& enc_par )
{
  qDebug( "%s----------------------------------------------------",c_name );
  qDebug( "%stranscode parameter:",c_name );
  qDebug( "%s----------------------------------------------------",c_name );
  qDebug( "%savi-file    : %s",c_name,enc_par.avi_input_finfo.absoluteFilePath().ascii() );
  qDebug( "%smpeg-file   : %s",c_name,enc_par.mpeg2_output_finfo.absoluteFilePath().ascii() );
  qDebug( "%swidhtxheight: %dx%d",c_name,enc_par.video_width,enc_par.video_height );
  qDebug( "%saspect-code : %d",c_name,enc_par.video_aspect_code );
  qDebug( "%sbitrate     : %f",c_name,enc_par.video_bitrate );
  qDebug( "%s----------------------------------------------------",c_name );

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
  qDebug( "%sstarting transcode...",c_name );

  start( str_command, strl_command_line );

  if ( !waitForStarted() )
  {
    qDebug( "%serror starting transcode",c_name );
    return false;
  }

  if ( !waitForFinished(-1) )
  {
    qDebug( "%serror executing transcode",c_name );
    return false;
  }

  return true;
}


void TTTranscodeProvider::transcodeReadOut()
{
  char       temp_str[101];
  int        i, i_pos;
  QString    line;
  QByteArray ba;

  ba = readAll();

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
      proc_view->addLine( line );
      i_pos = 0;
    }
  }
}

void TTTranscodeProvider::transcodeStarted()
{
  char       temp_str[101];
  int        i, i_pos;
  QString    line;
  QByteArray ba;

  ba = readAll();

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
      proc_view->addLine( line );
      i_pos = 0;
    }
  }
}

void TTTranscodeProvider::transcodeFinish( int e_code )
{
  char       temp_str[101];
  int        i, i_pos;
  QString    line;
  QByteArray ba;

  ba = readAll();

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
      proc_view->addLine( line );
      i_pos = 0;
    }
  }

  closeReadChannel( QProcess::StandardOutput );
  close();
  exit_code = e_code;
}
