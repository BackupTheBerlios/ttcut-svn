/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttcutproject.cpp                                                */
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

#include "ttcutproject.h"

#include <QTextStream>
#include <QString>
#include <QStringList>

//#define TTCUTPROJECT_DEBUG

const char c_name[] = "TTCUTPROJECT  : ";

TTCutProject::TTCutProject( QString& prj_file_name, QIODevice::OpenModeFlag mode )
{
#if defined(TTCUTPROJECT_DEBUG)
  qDebug( "%sopen project file: %s",c_name,prj_file_name.ascii() );
#endif

  project_file = new QFile( prj_file_name );
  io_stream    = new QTextStream( project_file );

  if (!project_file->open(mode | QIODevice::Text))
  {
    qDebug( "%serror open file (!)",c_name );
    throw( TTCutProjectOpenException() );
  }

  if ( mode == QIODevice::WriteOnly )
  {
    *io_stream << "TTCut - Projekt-File\n";
    *io_stream << "[version]\n";
    *io_stream << "1.0\n";
  }
}

TTCutProject::~TTCutProject()
{
  project_file->flush();
  project_file->close();

  delete io_stream;
  delete project_file;
}

void TTCutProject::clearFile()
{
  if ( project_file->exists() )
  {
    project_file->resize( 0 );
  }
}

void TTCutProject::writeVideoSection( bool start )
{
  if ( start )
    *io_stream << "[video]\n";
  else
    *io_stream << "\n";
}

void TTCutProject::writeVideoFileName( const QString& video_file_name )
{
  *io_stream << video_file_name << "\n";
}
void TTCutProject::writeAudioSection( bool start )
{
  if ( start )
    *io_stream << "[audio]\n";
  else
    *io_stream << "\n";
}

void TTCutProject::writeAudioFileName( const QString& audio_file_name )
{
  *io_stream << audio_file_name << "\n";
}

void TTCutProject::writeCutSection( bool start )
{
  if ( start )
    *io_stream << "[cut]\n";
  else
    *io_stream << "\n";
}

void TTCutProject::writeCutEntry( int cut_in, int cut_out )
{
  *io_stream << cut_in << ";" << cut_out << "\n";
}

bool TTCutProject::seekToVersionSection()
{
  if ( !project_file->seek( 0 ) )
  {
#if defined(TTCUTPROJECT_DEBUG)
    qDebug( "%serror in seek to video-section(!)",c_name );
#endif
    return false;
  }

  while( !io_stream->atEnd() && io_stream->readLine() != "[version]" )
    ;

  if ( !io_stream->atEnd() )
    return true;
  else
    return false;
}

bool TTCutProject::readFileVersion( int& version )
{
  QString line;

  if ( !io_stream->atEnd() )
  {
    line = io_stream->readLine();
    
    if ( !line.isEmpty() )
    {
      version = line.toInt();
      
#if defined(TTCUTPROJECT_DEBUG)
      qDebug( "%sfound file-version: %d",c_name,version );
#endif
      return true;
    }
  }
  return false;
}

bool TTCutProject::seekToVideoSection()
{
  if ( !project_file->seek( 0 ) )
  {
#if defined(TTCUTPROJECT_DEBUG)
    qDebug( "%serror in seek to video-section(!)",c_name );
#endif
    return false;
  }

  while( !io_stream->atEnd() && io_stream->readLine() != "[video]" )
    ;

  if ( !io_stream->atEnd() )
    return true;
  else
    return false;
}

bool TTCutProject::readVideoFileName( QString& video_file_name )
{
  QString line;

  if ( !io_stream->atEnd() )
  {
    line = io_stream->readLine();
    
    if ( !line.isEmpty() )
    {
      video_file_name = line;
      
#if defined(TTCUTPROJECT_DEBUG)
      qDebug( "%sfound video file: %s",c_name,video_file_name.ascii() );
#endif
      return true;
    }
  }
  return false;
}

bool TTCutProject::seekToAudioSection()
{
  if ( !project_file->seek( 0 ) )
  {
#if defined(TTCUTPROJECT_DEBUG)
    qDebug( "%serror in seek to audio-section(!)",c_name );
#endif
    return false;
  }

  while( !io_stream->atEnd() && io_stream->readLine() != "[audio]" )
    ;

  if ( !io_stream->atEnd() )
    return true;
  else
    return false;
}

bool TTCutProject::readAudioFileName( QString& audio_file_name )
{
  QString line;
  
  if ( !io_stream->atEnd() )
  {
    line = io_stream->readLine();
    
    if ( !line.isEmpty() )
    {
      audio_file_name = line;
      
#if defined(TTCUTPROJECT_DEBUG)
      qDebug( "%sfound audio file: %s",c_name,audio_file_name.ascii() );
#endif      
      return true;
    }
  }
  return false;
}

bool TTCutProject::seekToCutSection()
{
  if ( !project_file->seek( 0 ) )
  {
#if defined(TTCUTPROJECT_DEBUG)
    qDebug( "%serror in seek to cut-section(!)",c_name );
#endif
    return false;
  }

  while( !io_stream->atEnd() && io_stream->readLine() != "[cut]" )
    ;

  if ( !io_stream->atEnd() )
    return true;
  else
    return false;
}

bool TTCutProject::readCutEntry( int& cut_in, int& cut_out )
{
  QString     line;
  QStringList str_list;

  if ( !io_stream->atEnd() )
  {
    line = io_stream->readLine();
    
    if ( !line.isEmpty() )
    {
      str_list.clear();

      str_list = line.split( ";");
      
      cut_in  = str_list[0].toInt();
      cut_out = str_list[1].toInt();

#if defined(TTCUTPROJECT_DEBUG)
      qDebug( "%sfound cut entry: cut-in: %d / cut_out: %d",c_name,cut_in,cut_out );
#endif      
      return true;
    }
  }
  return false;
}

