/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttcutpreview.cpp                                                */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 03/13/2005 */
/* MODIFIED: b. altendorf                                    DATE: 03/12/2006 */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTCUTPREVIEW
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

#include "ttcutpreview.h"

#include <QApplication>
#include <QDir>

const char c_name[] = "TTCutPreview";

// -----------------------------------------------------------------------------
// TTCutPreview constructor
// -----------------------------------------------------------------------------
TTCutPreview::TTCutPreview(QWidget* parent, int prevW, int prevH)
  : QDialog(parent)
{
  setupUi(this);

  // set the objects name
  setObjectName(c_name);

  // message logger instance
  log = TTMessageLogger::getInstance();

  // set desired video width x height
  previewWidth  = prevW;
  previewHeight = prevH;

  // initialize some member
  mplayerProc = NULL;
  mplexProc   = NULL;
  isPlaying   = false;

  // set the frame size according the video size
  videoFrame->setMinimumSize( QSize( previewWidth, previewHeight ) );
  videoFrame->setMaximumSize( QSize( previewWidth, previewHeight ) );

  cbCutPreview->setEditable( false );
  cbCutPreview->setMinimumSize( 160, 20 );
  cbCutPreview->setInsertPolicy( QComboBox::InsertAfterCurrent );

  // connect signals and slots
  connect( cbCutPreview, SIGNAL( highlighted(int) ), SLOT( selectCut(int) ) );
  connect( pbPlay,       SIGNAL( clicked() ),        SLOT( playPreview() ) );
  connect( pbStop,       SIGNAL( clicked() ),        SLOT( stopPreview() ) );
  connect( pbExit,       SIGNAL( clicked() ),        SLOT( exitPreview() ) );
}

// -----------------------------------------------------------------------------
// Destroys the object and frees any allocated resources
// -----------------------------------------------------------------------------
TTCutPreview::~TTCutPreview()
{
  if ( ttAssigned( preview_cut_list ) )
    delete preview_cut_list;
}

// -----------------------------------------------------------------------------
// Init preview parameter
// -----------------------------------------------------------------------------
void TTCutPreview::initPreview( TTVideoStream* v_stream, TTAudioStream* a_stream, TTCutListData* c_list )
{
  video_stream = v_stream;
  audio_stream = a_stream;
  avcut_list   = c_list;
}

// -----------------------------------------------------------------------------
// Create the preview clips
// -----------------------------------------------------------------------------
void TTCutPreview::createPreview( int c_index )
{
  int            i, iPos;
  QString        preview_video_name;
  QString        preview_audio_name;
  QString        preview_mplex_name;
  QFileInfo      preview_video_info;
  QFileInfo      preview_audio_info;
  QFileInfo      preview_mplex_info;
  QString        selectionString;
  QString        mplex_command;
  QTime          preview_time;
  uint           preview_frames;
  uint           start_index;
  uint           end_index;
  TTCutListData* temp_cut_list;

  preview_time.setHMS(0,0,0);
  preview_time   = preview_time.addSecs( TTCut::cutPreviewSeconds );
  preview_frames = ttTimeToFrames( preview_time, video_stream->frameRate() ) / 2;

  //qDebug( "%s-----------------------------------------------",c_name );
  //qDebug( "%s>>> create cut preview",c_name );
  //qDebug( "%s-----------------------------------------------",c_name );
  //qDebug( "%scut index    : %d",c_name,c_index);

  // create the cut list for the preview clips
  createCutPreviewList();

  // number of list entrys
  num_cut_list = preview_cut_list->count();
  num_preview  = num_cut_list/2+1;

  //qDebug( "%snum preview  : %d",c_name,num_preview);

  // ---------------------------------------------------------------------
  // Create video and audio preview clips
  // ---------------------------------------------------------------------
  for ( i = 0; i < num_preview; i++ )
  {
    //qDebug( "%s-----------------------------------------------",c_name );
    //qDebug( "%scut index  : %d / %d",c_name,i,cut_index);
    temp_cut_list = new TTCutListData(avcut_list->videoStream());

    if ( c_index > 0 )
    {
      preview_video_name.sprintf("preview_%03d.m2v",i+1-c_index);
      preview_audio_name.sprintf("preview_%03d.mpa",i+1-c_index);
    }
    else
    {
      preview_video_name.sprintf("preview_%03d.m2v",i+1);
      preview_audio_name.sprintf("preview_%03d.mpa",i+1);
    }

    // add temporary path information
    preview_video_info.setFile( QDir(TTCut::tempDirPath), preview_video_name );
    preview_audio_info.setFile( QDir(TTCut::tempDirPath), preview_audio_name );

    preview_video_name = preview_video_info.absoluteFilePath();
    preview_audio_name = preview_audio_info.absoluteFilePath();

    // first cut-in
    if ( i == 0 )
    {
      //qDebug( "%s(A)first cut %d      : %d",c_name,i,i);

      temp_cut_list->deleteAll();

      temp_cut_list->addCutPosition( preview_cut_list->cutInPosAt(i), preview_cut_list->cutOutPosAt(i), i );

      if ( c_index == i || c_index+1 == i || c_index < 0 )
      {
        selectionString.sprintf( "Start: %s", qPrintable(ttFramesToTime(preview_cut_list->cutInPosAt(i), 
                                                         video_stream->frameRate()).toString("hh:mm:ss")));
        cbCutPreview->addItem( selectionString );
      }
    }

    // cut i-i
    if ( num_preview > 1 && i > 0 && i < num_preview-1 )
    {
      iPos = (i-1)*2+1;
      //qDebug("%s(X)cut %d-%d, i pos   : %d",c_name,i,i+1,iPos);

      temp_cut_list->deleteAll();

      temp_cut_list->addCutPosition( preview_cut_list->cutInPosAt(iPos), preview_cut_list->cutOutPosAt(iPos), i );
      temp_cut_list->addCutPosition( preview_cut_list->cutInPosAt(iPos+1), preview_cut_list->cutOutPosAt(iPos+1), i );

      if ( c_index == i || c_index+1 == i || c_index < 0 )
      {
        start_index = preview_cut_list->cutOutPosAt( iPos );
        end_index   = preview_cut_list->cutInPosAt( iPos+1 );
        selectionString.sprintf( "Cut %d-%d: %s - %s",i,i+1,
            qPrintable(ttFramesToTime( start_index, video_stream->frameRate()).toString("hh:mm:ss")),
            qPrintable(ttFramesToTime( end_index, video_stream->frameRate()).toString("hh:mm:ss")));
        cbCutPreview->addItem( selectionString );
      }
    }

    //last cut out
    if ( i == num_preview-1 )
    {
      iPos = (i-1)*2+1;
      //qDebug("%s(E)last cut, i pos    : %d",c_name,iPos);
      temp_cut_list->deleteAll();

      temp_cut_list->addCutPosition( preview_cut_list->cutInPosAt(iPos), preview_cut_list->cutOutPosAt(iPos), i );

      if ( c_index == i || c_index+1 == i || c_index < 0 )
      {
        selectionString.sprintf( "End: %s", 
            qPrintable(ttFramesToTime(preview_cut_list->cutOutPosAt(iPos), 
                video_stream->frameRate()).toString("hh:mm:ss")));
        cbCutPreview->addItem( selectionString );
      }
    }

    // create cut
    if ( c_index == i || c_index+1 == i || c_index < 0 )
    {
      //qDebug( "%screate preview cut: %d - %s",c_name,i,preview_video_name.ascii());

      progress_bar = new TTProgressBar( this );
      progress_bar->show();
      qApp->processEvents();

      video_stream->setProgressBar( progress_bar );
      video_cut_stream = new TTFileBuffer( qPrintable(preview_video_name), fm_open_write );

      if ( TTCut::numAudioTracks > 0 )
      {
        audio_stream->setProgressBar( progress_bar );
        audio_cut_stream = new TTFileBuffer( qPrintable(preview_audio_name), fm_open_write );
      }

      video_stream->cut( video_cut_stream, temp_cut_list );

      if ( TTCut::numAudioTracks > 0 )
        audio_stream->cut( audio_cut_stream, temp_cut_list );

      delete progress_bar;

      delete video_cut_stream;  

      if ( TTCut::numAudioTracks > 0 )
        delete audio_cut_stream;
    }
    delete temp_cut_list;
    // next video preview clip
  }
  // end cut video/audio preview clips

  // multiplex video and audio files give a better result because
  // video and audio synchronisation is much better
  progress_bar = new TTProgressBar( this );
  progress_bar->show();
  progress_bar->setActionText( "Mplexing preview cuts..." );
  progress_bar->setTotalSteps( num_preview, 1 );

  for ( i = 0; i < num_preview; i++ )
  {
    preview_video_name.sprintf("preview_%03d.m2v",i+1);
    preview_audio_name.sprintf("preview_%03d.mpa",i+1);
    preview_mplex_name.sprintf("preview_%03d.mpg",i+1);

    // add temporary path information
    preview_video_info.setFile( QDir(TTCut::tempDirPath), preview_video_name );
    preview_audio_info.setFile( QDir(TTCut::tempDirPath), preview_audio_name );
    preview_mplex_info.setFile( QDir(TTCut::tempDirPath), preview_mplex_name );

    preview_video_name = preview_video_info.absoluteFilePath();
    preview_audio_name = preview_audio_info.absoluteFilePath();
    preview_mplex_name = preview_mplex_info.absoluteFilePath();

    // have we audio tracks
    if ( TTCut::numAudioTracks > 0 )
    {
      mplex_command  = "mplex -f 8 -o ";
      mplex_command += preview_mplex_name;
      mplex_command += " ";
      mplex_command += preview_video_name;
      mplex_command += " ";
      mplex_command += preview_audio_name;
      mplex_command += " ";
      mplex_command += "2>/dev/null";

      system( qPrintable(mplex_command) );
    }
    else
    {
      mplex_command  = "mv ";
      mplex_command += preview_video_name;
      mplex_command += " ";
      mplex_command += preview_mplex_name;
      mplex_command += " ";
      mplex_command += "2>/dev/null";

      system( qPrintable(mplex_command) );
    }
    log->infoMsg(c_name, "mplex command: %s", qPrintable(mplex_command));
    progress_bar->setProgress( i+1 );
  }
  progress_bar->setComplete();
  delete progress_bar;
  delete preview_cut_list;

  //qDebug( "%s-----------------------------------------------",c_name );

  // set the current cut preview to the first cut clip
  preview_video_name.sprintf("preview_001.mpg");
  preview_video_info.setFile( QDir(TTCut::tempDirPath), preview_video_name );

  current_video_file = preview_video_info.absoluteFilePath();
}


void TTCutPreview::createCutPreviewList( )
{
  int    list_count;
  int    current_entry;
  int    i;
  QTime  preview_time;
  long   preview_frames;
  int    frame_type;
  long   start_index;
  long   end_index;

  // create new cut list for the preview clips
  preview_cut_list = new TTCutListData(avcut_list->videoStream());

  current_entry = 0;
  list_count    = 0;

  preview_time.setHMS(0,0,0);
  preview_time   = preview_time.addSecs( TTCut::cutPreviewSeconds );
  preview_frames = ttTimeToFrames( preview_time, video_stream->frameRate() ) / 2;

  //qDebug( "%s-----------------------------------------------",c_name );
  //qDebug( "%s>>> create cut preview list",c_name );
  //qDebug( "%s-----------------------------------------------",c_name );
  //qDebug( "%spreview frames : %d",c_name,preview_frames);
  //qDebug( "%savcutlist count: %d",c_name,avcut_list->count());

  for ( i = 0; i < avcut_list->count(); i++ )
  {
    start_index = avcut_list->cutInPosAt( i );
    end_index   = start_index + preview_frames;

    if ( end_index >= video_stream->frameCount() )
      end_index = video_stream->frameCount()-1;

    // cut should end at an I-frame or P-frame
    frame_type = video_stream->frameType( end_index );
    //qDebug("%sframe type at end: %ld - %d",c_name,end_index,frame_type);
    while ( frame_type == 3 &&
        end_index < video_stream->frameCount()-1 )
    {
      end_index++;
      frame_type = video_stream->frameType( end_index );
    }

    //qDebug("%screate cut list entry: %ld - %d: %d",c_name,start_index,end_index,preview_frames);

    preview_cut_list->addCutPosition( start_index, end_index, list_count );

    list_count++;

    end_index   = avcut_list->cutOutPosAt( i );
    start_index = end_index - preview_frames;

    if ( start_index < 0 )
      start_index = 0;

    // cut should start at an I-frame
    frame_type = video_stream->frameType( start_index );
    //qDebug("%sframe type at start: %ld - %d",c_name,start_index,frame_type);
    while ( frame_type != 1 && start_index > 0 )
    {
      start_index--;
      frame_type = video_stream->frameType( start_index );
    }

    //qDebug("%screate cut list entry: %ld - %d: %d",c_name,start_index,end_index,preview_frames);

    preview_cut_list->addCutPosition( start_index, end_index, list_count );

    list_count++;
    current_entry++;
  }
  //qDebug( "%spreview count: %d",c_name,preview_cut_list->count());
  //qDebug( "%s-----------------------------------------------",c_name );
}

// -----------------------------------------------------------------------------
// Combobox selection changed, so we had to select new files
// -----------------------------------------------------------------------------
void TTCutPreview::selectCut( int iCut )
{
  QString   preview_video_name;
  QFileInfo preview_video_info;

  preview_video_name.sprintf("preview_%03d.mpg",iCut+1);
  preview_video_info.setFile( QDir(TTCut::tempDirPath), preview_video_name );
  current_video_file = preview_video_info.absoluteFilePath();
}


// -----------------------------------------------------------------------------
// Play the selected cut preview clip
// -----------------------------------------------------------------------------
void TTCutPreview::playPreview()
{
  if ( !isPlaying )
  {
    log->debugMsg( c_name, "Start playing preview: %s",current_video_file.toAscii().data() );

    // try to grab the keyboard, prevents mplayer from receiving keyboard input
    grabKeyboard();

    // create mplayer process
    mplayerProc = new QProcess( );

    // play the current preview clip
    playMPlayer( current_video_file, current_audio_file );
  }
}


// -----------------------------------------------------------------------------
// Stop the current playing preview
// -----------------------------------------------------------------------------
void TTCutPreview::stopPreview()
{
  // release the keyboard, so other widgets can receive keyboard input
  releaseKeyboard();

  if ( !stopMPlayer() )
    qDebug( "No playing preview ???" );
}


// -----------------------------------------------------------------------------
// Exit the preview window
// -----------------------------------------------------------------------------
void TTCutPreview::exitPreview()
{
  QString   rm_command = "rm ";
  QString   file_name  = "preview*";
  QFileInfo file_info;

  if ( !isPlaying )
  {
    // clean up preview* files in temp directory
    file_info.setFile( QDir(TTCut::tempDirPath), file_name );
    rm_command += file_info.absoluteFilePath();
    rm_command += " 2>/dev/null";

    system( rm_command.toAscii().data());

    done( 0 );
  }
  else
  {
    if(stopMPlayer())
    {
      done(0);
    }
    else
      qDebug( "Cant't exit, mplayer still running ???" );
  }
}


// -----------------------------------------------------------------------------
// Read messages from mplayer process from stdout
// -----------------------------------------------------------------------------
void TTCutPreview::readFromStdout()
{
  char       temp_str[101];
  int        i, i_pos;
  QString    line;
  QByteArray ba;

  if(mplayerProc->state() == QProcess::Running)
  {
    ba = mplayerProc->readAll();

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
        log->infoMsg(c_name, line);
        i_pos = 0;
      }
    }
  }
  else
  {
    line = "mplayer finished... done(0)";
    isPlaying = false;
    log->infoMsg(c_name, line);
  }
}

// -----------------------------------------------------------------------------
// MPlayer interface
// -----------------------------------------------------------------------------

// start mplayer playing file <fileName>
// -----------------------------------------------------------------------------
bool TTCutPreview::playMPlayer( QString videoFile,__attribute__ ((unused)) QString audioFile )
{
  QString     str_cmd;
  QStringList mplayer_cmd;

  if ( ttAssigned( mplayerProc ) &&
      !isPlaying                   )
  {
    // Setup interface with MPlayer
    mplayer_cmd.clear();

    // ----------------------------------------------------------------------
    // slave-mode
    // ----------------------------------------------------------------------
    // Switches on slave mode, in which MPlayer works as a backend for other
    // programs. Instead of intercepting keyboard events, MPlayer will read
    // commands from stdin.
    // NOTE: See -input cmdlist for a list of slave commands and
    // DOCS/tech/slave.txt for their description.
    // ----------------------------------------------------------------------

    // Every argument must have it's own addArgument
    mplayer_cmd << "-slave"
      << "-identify"
      << "-quiet"
      << "-wid";

    str_cmd.sprintf( "%ld",(long)videoFrame->winId() );
    mplayer_cmd << str_cmd
      << "-geometry";

    str_cmd.sprintf( "%dx%d+0+0", previewWidth, previewHeight );
    mplayer_cmd << str_cmd
      << videoFile;


    // start the mplayer process
    if ( mplayerProc->state() != QProcess::Running )
    {
      log->infoMsg(c_name, "mplayer command: %s", mplayer_cmd.join(" ").toAscii().data());
      mplayerProc->start( "mplayer", mplayer_cmd );

      // signal and slot connection for the mplayer process
      // detect when mplayer has information ready for us
      connect(mplayerProc, SIGNAL( started() ), SLOT( mplayerStarted() ) );
      connect(mplayerProc, SIGNAL( readyRead() ),SLOT( readFromStdout() ) );
      connect(mplayerProc, SIGNAL( finisheded(int, QProcess::ExitStatus) ),  SLOT( exitMPlayer(int, QProcess::ExitStatus) ) );
      connect(mplayerProc, SIGNAL( error(QProcess::ProcessError) ), SLOT( errorMplayer(QProcess::ProcessError) ) );
      connect(mplayerProc, SIGNAL( stateChanged(QProcess::ProcessState) ), SLOT( stateChangedMplayer(QProcess::ProcessState) ) );

      isPlaying = true;

      return true;
    }
    else
    {
      log->infoMsg(c_name, "mplayer command: %s", mplayer_cmd.join(" ").toAscii().data());
      log->errorMsg(c_name, "error starting mplayer (!)");
    }
  }
  return false;
}

// stop mplayer playing
// -----------------------------------------------------------------------------
bool TTCutPreview::stopMPlayer()
{
  const QString strQuit = "quit\n";

  if ( isPlaying )
  {
    mplayerProc->write( "quit\n" );

    isPlaying = false;

    return true;
  }

  return false;
}

void TTCutPreview::mplayerStarted()
{
  log->infoMsg(c_name, "mplayer process started");
}

// exit mplayer process
// -----------------------------------------------------------------------------
void TTCutPreview::exitMPlayer(__attribute__ ((unused)) int e_code, QProcess::ExitStatus e_status)
{
  //qDebug( "%sexit mplayer: exit code: %d",c_name,e_code );

  log->infoMsg(c_name, "exit code %d / exit status %d", e_status);

  // delete the mplayer process
  delete mplayerProc;

  isPlaying = false;
}

void TTCutPreview::errorMplayer( QProcess::ProcessError error )
{
  log->errorMsg(c_name, "error: %d", error);

  delete mplayerProc;
  isPlaying = false;
}

void TTCutPreview::stateChangedMplayer( QProcess::ProcessState newState )
{
  log->infoMsg(c_name, "state changed: %d", newState);

  if(newState == QProcess::NotRunning){
    isPlaying = false;
  }
}



