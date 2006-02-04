/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttcutpreview.cpp                                                */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 03/13/2005 */
/* MODIFIED: b. altendorf                                    DATE: 06/22/2005 */
/* MODIFIED: b. altendorf                                    DATE: 07/16/2005 */
/* MODIFIED: b. altendorf                                    DATE: 08/09/2005 */
/* MODIFIED: b. altendorf                                    DATE: 02/02/2006 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTCUTPREVIEW
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
#include <QProcess>
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QDir>

const char c_name[] = "TTCUTPREVIEW";

// -----------------------------------------------------------------------------
// TTCutPreview constructor
// -----------------------------------------------------------------------------
TTCutPreview::TTCutPreview( QWidget* parent, int prevW, int prevH,
    const char* name, bool modal, Qt::WFlags fl )
: QDialog( parent, name, modal, fl )
{
  // set widget's name
  if ( !name )
    setName( "TTCutPreview" );

  log = TTMessageLogger::getInstance();

  // set desired video widthxheight
  previewWidth  = prevW;
  previewHeight = prevH;

  // initialize some member
  mplayerProc = NULL;
  mplexProc   = NULL;
  isPlaying   = false;

  // main layout
  TTCutPreviewLayout = new QGridLayout( this, 1, 1, 11, 6, "TTCutPreviewLayout");

  // the video preview window
  videoFrame = new QFrame( this, "videoFrame" );
  videoFrame->setFrameShape( QFrame::StyledPanel );
  videoFrame->setFrameShadow( QFrame::Sunken );

  // set the frame size according the video size
  videoFrame->setMinimumSize( QSize( previewWidth, previewHeight ) );
  videoFrame->setMaximumSize( QSize( previewWidth, previewHeight ) );

  // add video preview window to main layout
  TTCutPreviewLayout->addWidget( videoFrame, 0, 0 );

  // button layout
  layout1 = new QHBoxLayout( 0, 0, 6, "layout1");

  // combo=box for the cut qualifier
  cbCutPreview = new QComboBox( FALSE, this, "cbCutPreview" );
  cbCutPreview->setMinimumSize( 160, 20 );
  layout1->addWidget( cbCutPreview );

  // play button
  pbPlay = new QPushButton( this, "pbPlay" );
  pbPlay->setIconSet( QIcon( *(TTCut::imgPlay) ) );
  layout1->addWidget( pbPlay );

  // stop button
  pbStop = new QPushButton( this, "pbStop" );
  pbStop->setIconSet( QIcon( *(TTCut::imgStop) ) );
  layout1->addWidget( pbStop );

  // spacer
  spacer1 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
  layout1->addItem( spacer1 );

  // exit preview button
  pbExit = new QPushButton( this, "pbExit" );
  pbExit->setIconSet( QIcon( *(TTCut::imgFileClose) ) );
  layout1->addWidget( pbExit );

  // add button layout to main layoput
  TTCutPreviewLayout->addLayout( layout1, 1, 0 );

  // set button and label text
  languageChange();

  // resize widget to minimum size
  setMinimumSize(640,480);
  resize( QSize(640, 480).expandedTo(minimumSizeHint()) );
  setMaximumSize( QSize(640, 480).expandedTo(minimumSizeHint()) );

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
// Sets the strings of the subwidgets using the current language.
// -----------------------------------------------------------------------------
void TTCutPreview::languageChange()
{
  // dialog caption
  setCaption( tr( "Cut Preview" ) );

  // button text
  pbPlay->setText( tr( "Play" ) );
  pbStop->setText( tr( "Stop" ) );
  pbExit->setText( tr( "Exit" ) );
}

// -----------------------------------------------------------------------------
// Init preview parameter
// -----------------------------------------------------------------------------
void TTCutPreview::initPreview( TTVideoStream* v_stream, TTAudioStream* a_stream, TTAVCutList* c_list )
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
  TTAVCutList*   temp_cut_list;

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

    temp_cut_list = new TTAVCutList( 2 );

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

      temp_cut_list->addCutPosition( preview_cut_list->entryAt( i ) );

      if ( c_index == i || c_index+1 == i || c_index < 0 )
      {
        selectionString.sprintf( "Start: %s", ttFramesToTime( preview_cut_list->cutInAt( i ), video_stream->frameRate() ).toString("hh:mm:ss").toAscii().data() );
        cbCutPreview->insertItem( selectionString );
      }
    }

    // cut i-i
    if ( num_preview > 1 && i > 0 && i < num_preview-1 )
    {
      iPos = (i-1)*2+1;
      //qDebug("%s(X)cut %d-%d, i pos   : %d",c_name,i,i+1,iPos);

      temp_cut_list->deleteAll();

      temp_cut_list->addCutPosition( preview_cut_list->entryAt( iPos ) );
      temp_cut_list->addCutPosition( preview_cut_list->entryAt( iPos+1 ) );

      if ( c_index == i || c_index+1 == i || c_index < 0 )
      {
        start_index = preview_cut_list->cutOutAt( iPos );
        end_index   = preview_cut_list->cutInAt( iPos+1 );
        selectionString.sprintf( "Cut %d-%d: %s - %s",i,i+1,
            ttFramesToTime( start_index, video_stream->frameRate() ).toString("hh:mm:ss").toAscii().data(),
            ttFramesToTime( end_index, video_stream->frameRate() ).toString("hh:mm:ss").toAscii().data() );
        cbCutPreview->insertItem( selectionString );
      }
    }

    //last cut out
    if ( i == num_preview-1 )
    {
      iPos = (i-1)*2+1;
      //qDebug("%s(E)last cut, i pos    : %d",c_name,iPos);
      temp_cut_list->deleteAll();

      temp_cut_list->addCutPosition( preview_cut_list->entryAt( iPos ) );

      if ( c_index == i || c_index+1 == i || c_index < 0 )
      {
        selectionString.sprintf( "End: %s", ttFramesToTime( preview_cut_list->cutOutAt( iPos ), video_stream->frameRate() ).toString("hh:mm:ss").toAscii().data() );
        cbCutPreview->insertItem( selectionString );
      }
    }

    // create cut
    if ( c_index == i || c_index+1 == i || c_index < 0 )
    {
      //qDebug( "%screate preview cut: %d - %s",c_name,i,preview_video_name.ascii());

      progress_bar = new TTProgressBar( this, "ProgressBar", true);
      progress_bar->show();
      qApp->processEvents();

      video_stream->setProgressBar( progress_bar );
      video_cut_stream = new TTFileBuffer( preview_video_name.toAscii().data(), fm_open_write );

      if ( TTCut::numAudioTracks > 0 )
      {
        audio_stream->setProgressBar( progress_bar );
        audio_cut_stream = new TTFileBuffer( preview_audio_name.toAscii().data(), fm_open_write );
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
  progress_bar = new TTProgressBar( this, "ProgressBar", true);
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

      system( mplex_command.toAscii().data() );
    }
    else
    {
      mplex_command  = "mv ";
      mplex_command += preview_video_name;
      mplex_command += " ";
      mplex_command += preview_mplex_name;
      mplex_command += " ";
      mplex_command += "2>/dev/null";

      system( mplex_command.toAscii().data() );
    }
    log->infoMsg(c_name, "mplex command: %s", mplex_command.toAscii().data());
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
  preview_cut_list = new TTAVCutList( 10 );

  // sort the av cut list
  avcut_list->sortCutOrder();

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
    start_index = avcut_list->cutInAt( i );
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
    
    end_index   = avcut_list->cutOutAt( i );
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
    //qDebug( "Start playing preview: %s",current_video_file.ascii() );

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
log->infoMsg(c_name, "mplayer output: %s", mplayerProc->readAllStandardOutput().constData());
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
      connect(mplayerProc, SIGNAL( finished(int) ),  SLOT( exitMPlayer(int) ) );

      isPlaying = true;

      return true;
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
void TTCutPreview::exitMPlayer(__attribute__ ((unused)) int e_code)
{
  //qDebug( "%sexit mplayer: exit code: %d",c_name,e_code );

  // delete the mplayer process
  delete mplayerProc;

  isPlaying = false;
}




