/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : tttranscode.h                                                   */
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

#ifndef TTTRANSCODE_H
#define TTTRASNCODE_H

#include "../common/ttmessagelogger.h"
#include "../common/ttcut.h"
#include "../gui/ttprocessform.h"

#include <QFileInfo>
#include <QProcess>

class TTEncodeParameter;

class TTTranscodeProvider : public TTProcessForm
{
  Q_OBJECT

 public:
  TTTranscodeProvider();
  ~TTTranscodeProvider();

  void setParameter( TTEncodeParameter& enc_par );
  bool encodePart( );

  public slots:
  void transcodeReadOut();
  void transcodeStarted();
  void transcodeFinish(int exit_code, QProcess::ExitStatus );
  void transcodeError(QProcess::ProcessError proc_error);
  void transcodeState(QProcess::ProcessState proc_state);
  void transcodeKill();

 private:
  TTMessageLogger* log;
  QProcess*        transcode_proc;
  QString          str_command;
  QStringList      strl_command_line;
  int              exit_code;
  bool             transcode_success;
};


class TTEncodeParameter
{
 public:
  TTEncodeParameter(){};
  ~TTEncodeParameter(){};

  QFileInfo avi_input_finfo;
  QFileInfo mpeg2_output_finfo;
  int       video_width;
  int       video_height;
  float     video_fps;
  int       video_aspect_code;
  float     video_bitrate;
  float     video_max_bitrate;
};

#endif
   
