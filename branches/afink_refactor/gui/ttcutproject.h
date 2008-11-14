/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttcutproject.h                                                  */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 08/14/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTCUTPROJECT
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

#ifndef TTCUTPROJECT_H
#define TTCUTPROJECT_H

#include <QFile>

class QTextStream;
class QString;

// TTCutProject
// ------------------------------------------------------------------------------
class TTCutProject
{
 public:
  TTCutProject( QString& prj_file_name, QIODevice::OpenModeFlag mode=QIODevice::ReadOnly );
  ~TTCutProject();

  void clearFile();
  void writeVideoSection( bool start=true );
  void writeVideoFileName( const QString& video_file_name );
  void writeAudioSection( bool start=true );
  void writeAudioFileName( const QString& audio_file_name, int videoIndex );
  void writeCutSection( bool start=true );
  void writeCutEntry( int cut_in, int cut_out, int videoIndex );

  bool seekToVersionSection();
  bool readFileVersion( int& version );
  bool seekToVideoSection();
  bool readVideoFileName( QString& video_file_name );
  bool seekToAudioSection();
  bool readAudioFileName( QString& audio_file_name, int& videoIndex );
  bool seekToCutSection();
  bool readCutEntry( int& cut_in, int& cut_out, int& videoIndex );

 private:
  QFile* project_file;
  QTextStream* io_stream;
};

// Exceptions
// ------------------------------------------------------------------------------
class TTCutProjectOpenException{};

#endif
