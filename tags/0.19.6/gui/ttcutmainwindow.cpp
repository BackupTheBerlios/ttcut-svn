/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / ttcut.tritime.org                       */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttcutmainwindow.cpp                                             */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 02/26/2006 */
/* MODIFIED: b. altendorf                                    DATE: 04/12/2007 */
/* MODIFIED: b. altendorf                                    DATE: 04/30/2007 */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTCUTMAINWINDOW
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

#include <QtGui>
#include <QPixmap>

#include "ttcutmainwindow.h"
#include "ttcutavcutdlg.h"
#include "ttprogressbar.h"
#include "ttcutaboutdlg.h"

#include "../ui//pixmaps/downarrow_18.xpm"
#include "../ui/pixmaps/uparrow_18.xpm"
#include "../ui/pixmaps/cancel_18.xpm"
#include "../ui/pixmaps/fileopen_24.xpm"
#include "../ui/pixmaps/filenew_16.xpm"
#include "../ui/pixmaps/fileopen_16.xpm"
#include "../ui/pixmaps/filesave_16.xpm"
#include "../ui/pixmaps/filesaveas_16.xpm"
#include "../ui/pixmaps/saveimage_16.xpm"
#include "../ui/pixmaps/settings_16.xpm"
#include "../ui/pixmaps/settings_18.xpm"
#include "../ui/pixmaps/exit_16.xpm"
#include "../ui/pixmaps/play_18.xpm"
#include "../ui/pixmaps/stop_18.xpm"
#include "../ui/pixmaps/search_18.xpm"
#include "../ui/pixmaps/preview_18.xpm"
#include "../ui/pixmaps/chapter_18.xpm"
#include "../ui/pixmaps/cutav_18.xpm"
#include "../ui/pixmaps/cutaudio_18.xpm"
#include "../ui/pixmaps/goto_18.xpm"
#include "../ui/pixmaps/note_18.xpm"
#include "../ui/pixmaps/clock_16.xpm"
#include "../ui/pixmaps/apply_18.xpm"
#include "../ui/pixmaps/addtolist_18.xpm"
#include "../ui/pixmaps/fileclose_18.xpm"

#include <QStringList>
#include <QString>

const char oName[] = "TTCutMainWindow";

/* /////////////////////////////////////////////////////////////////////////////
 * Application main window constructor
 */
TTCutMainWindow::TTCutMainWindow() 
: QMainWindow()
{

  // setup Qt Designer UI
  setupUi( this );

  // images
  // --------------------------------------------------------------------------
  TTCut::imgDownArrow  = new QPixmap( downarrow_18_xpm );
  TTCut::imgUpArrow    = new QPixmap( uparrow_18_xpm );
  TTCut::imgDelete     = new QPixmap( cancel_18_xpm );
  TTCut::imgFileOpen24 = new QPixmap( fileopen_24_xpm );
  TTCut::imgFileNew    = new QPixmap( filenew_16_xpm );
  TTCut::imgFileOpen   = new QPixmap( fileopen_16_xpm );
  TTCut::imgFileSave   = new QPixmap( filesave_16_xpm );;
  TTCut::imgFileSaveAs = new QPixmap( filesaveas_16_xpm );
  TTCut::imgSaveImage  = new QPixmap( saveimage_16_xpm );
  TTCut::imgSettings   = new QPixmap( settings_16_xpm );
  TTCut::imgSettings18 = new QPixmap( settings_18_xpm );
  TTCut::imgExit       = new QPixmap( exit_16_xpm );
  TTCut::imgPlay       = new QPixmap( play_18_xpm );
  TTCut::imgStop       = new QPixmap( stop_18_xpm );
  TTCut::imgSearch     = new QPixmap( search_18_xpm );
  TTCut::imgChapter    = new QPixmap( chapter_18_xpm );
  TTCut::imgPreview    = new QPixmap( preview_18_xpm );
  TTCut::imgCutAV      = new QPixmap( cutav_18_xpm );
  TTCut::imgCutAudio   = new QPixmap( cutaudio_18_xpm );
  TTCut::imgGoTo       = new QPixmap( goto_18_xpm );
  TTCut::imgMarker     = new QPixmap( note_18_xpm );
  TTCut::imgClock      = new QPixmap( clock_16_xpm );
  TTCut::imgApply      = new QPixmap( apply_18_xpm );
  TTCut::imgAddToList  = new QPixmap( addtolist_18_xpm );
  TTCut::imgFileClose  = new QPixmap( fileclose_18_xpm );

  setFocusPolicy(Qt::StrongFocus);

  // Message logger instance
  log = TTMessageLogger::getInstance();
  
  // Get the current Qt version at runtime
  log->infoMsg(oName, "TTCut-Version: %s", qPrintable(TTCut::versionString));
  log->infoMsg(oName, "Qt-Version:    %s", qVersion());

#if QT_VERSION < 0x040100
  // TODO: Show message box and abort session
  log->errorMsg(oName, "Qt-Version >= 4.1.0 required");
#endif

  // Settings
  TTCut::recentFileList.clear();
  settings = new TTCutSettings();
  settings->readSettings();
  log->enableLogFile(TTCut::createLogFile);
  log->setLogModeConsole(TTCut::logModeConsole);
  log->setLogModeExtended(TTCut::logModeExtended);

 
  // Audio list
  audioList = new TTAudioListData();
  audioFileInfo->setListData(audioList);

  // Mux list and mplex provider (later by plugin)
  muxListData   = new TTMuxListData();
  mplexProvider = new TTMplexProvider();

  // no navigation
  navigationEnabled( false );
 
  // init
  cutListData            = NULL;
  mpegStream             = NULL;
  TTCut::isVideoOpen     = false;
  TTCut::projectFileName = "";
  
  // Signal and slot connections
  // 
  // Connect signals from main menu
  // --------------------------------------------------------------------------
  connect(actionOpenVideo,        SIGNAL(triggered()), videoFileInfo, SLOT(onFileOpen()));
  connect(actionOpenAudio,        SIGNAL(triggered()), audioFileInfo, SLOT(onFileOpen()));
  connect(actionFileNew,          SIGNAL(triggered()), SLOT(onFileNew()));
  connect(actionFileOpen,         SIGNAL(triggered()), SLOT(onFileOpen()));
  connect(actionFileSave,         SIGNAL(triggered()), SLOT(onFileSave()));
  connect(actionFileSaveAs,       SIGNAL(triggered()), SLOT(onFileSaveAs()));
  connect(actionExit,             SIGNAL(triggered()), SLOT(onFileExit()));
  connect(actionSaveCurrentFrame, SIGNAL(triggered()), SLOT(onActionSave()));
  connect(actionSettings,         SIGNAL(triggered()), SLOT(onActionSettings()));
  connect(actionAbout,            SIGNAL(triggered()), SLOT(onHelpAbout()));

  // recent files
  for (int i = 0; i < MaxRecentFiles; ++i) {
    recentFileAction[i] = new QAction(this);
    recentFileAction[i]->setVisible(false);
    menuRecentProjects->addAction(recentFileAction[i]);
    connect(recentFileAction[i], SIGNAL(triggered()), SLOT(onFileRecent()));
  }
 
  updateRecentFileActions();

  // Connect signals from video and audio info
  // --------------------------------------------------------------------------
  connect(videoFileInfo,          SIGNAL(fileOpened(QString)), SLOT(onReadVideoStream(QString)));
  connect(audioFileInfo,          SIGNAL(fileOpened(QString)), SLOT(onReadAudioStream(QString)));

  // Connect signals from navigation widget
  // --------------------------------------------------------------------------
  connect(navigation, SIGNAL(prevIFrame()),      currentFrame, SLOT(onPrevIFrame()));
  connect(navigation, SIGNAL(nextIFrame()),      currentFrame, SLOT(onNextIFrame()));
  connect(navigation, SIGNAL(prevPFrame()),      currentFrame, SLOT(onPrevPFrame()));
  connect(navigation, SIGNAL(nextPFrame()),      currentFrame, SLOT(onNextPFrame()));
  connect(navigation, SIGNAL(prevBFrame()),      currentFrame, SLOT(onPrevBFrame()));
  connect(navigation, SIGNAL(nextBFrame()),      currentFrame, SLOT(onNextBFrame()));
  connect(navigation, SIGNAL(setCutOut(int)),    currentFrame, SLOT(onSetCutOut(int)));
  connect(navigation, SIGNAL(setCutOut(int)),    cutOutFrame,  SLOT(onGotoCutOut(int)));
  connect(navigation, SIGNAL(setCutIn(int)),     currentFrame, SLOT(onSetCutIn(int)));
  connect(navigation, SIGNAL(gotoCutIn(int)),    currentFrame, SLOT(onGotoCutIn(int)));
  connect(navigation, SIGNAL(gotoCutOut(int)),   currentFrame, SLOT(onGotoCutOut(int)));
  connect(navigation, SIGNAL(addCutRange(int, int)), cutList,   SLOT(onAddEntry(int, int)));
  connect(navigation, SIGNAL(gotoMarker(int)),   currentFrame, SLOT(onGotoMarker(int)));
  connect(navigation, SIGNAL(moveNumSteps(int)), currentFrame, SLOT(onMoveNumSteps(int)));
  connect(navigation, SIGNAL(moveToHome()),      currentFrame, SLOT(onMoveToHome()));
  connect(navigation, SIGNAL(moveToEnd()),       currentFrame, SLOT(onMoveToEnd()));

  // Connect signal from video slider
  // --------------------------------------------------------------------------
  connect(streamNavigator, SIGNAL(sliderValueChanged(int)), SLOT(onVideoSliderChanged(int)));

  // Connect signals from cut-out frame widget
  // --------------------------------------------------------------------------
  connect(cutOutFrame, SIGNAL(equalFrameFound(int)), currentFrame, SLOT(onGotoFrame(int)));
  connect(cutOutFrame, SIGNAL(newCutOutFramePos(int)), cutList,    SLOT(onEditCutOut(int)));

  // Connect signals from current frame widget
  // --------------------------------------------------------------------------
  connect(currentFrame, SIGNAL(newFramePosition(int)), SLOT(onNewFramePos(int))); 

  // Connect signals from cut list widget
  // --------------------------------------------------------------------------
  connect(cutList, SIGNAL(entrySelected(int)), cutOutFrame,     SLOT(onGotoCutOut(int)));
  connect(cutList, SIGNAL(entryEdit(const TTCutListDataItem&)), 
                                              navigation,       SLOT(onEditCut(const TTCutListDataItem&)));
  connect(cutList, SIGNAL(gotoCutIn(int)),     currentFrame,    SLOT(onGotoFrame(int)));
  connect(cutList, SIGNAL(gotoCutOut(int)),    currentFrame,    SLOT(onGotoFrame(int)));
  connect(cutList, SIGNAL(refreshDisplay()),   streamNavigator, SLOT(onRefreshDisplay()));
  connect(cutList, SIGNAL(previewCut(int)),                     SLOT(onPreviewCut(int)));
  connect(cutList, SIGNAL(audioVideoCut(int)),                  SLOT(onAudioVideoCut(int)));
  connect(cutList, SIGNAL(audioCut(int)),                       SLOT(onAudioCut(int)));
}

/* /////////////////////////////////////////////////////////////////////////////
 * Destructor
 */
TTCutMainWindow::~TTCutMainWindow()
{
  if (settings    != 0) delete settings;
  if (audioList   != 0) delete audioList;
  if (muxListData != 0) delete muxListData;
  if (cutListData != 0) delete cutListData;
}

/* /////////////////////////////////////////////////////////////////////////////
 * Signals from the application menu
 */
void TTCutMainWindow::keyPressEvent(QKeyEvent* e)
{
  navigation->keyPressEvent(e);
}

/* /////////////////////////////////////////////////////////////////////////////
 * Menu "File new" action
 */
void TTCutMainWindow::onFileNew()
{
  if (TTCut::isVideoOpen) 
  {
    if (ttAssigned(cutListData) && cutListData->count() > 0) 
    {
      //TODO: Ask for saving changes
      log->infoMsg(oName, "TODO: Ask for saving changes in project");
    }
    closeProject();
  }
}

/* /////////////////////////////////////////////////////////////////////////////
 * Menu "File open" action
 */
void TTCutMainWindow::onFileOpen()
{
  QString fn = QFileDialog::getOpenFileName(this,
      tr("Open project-file"),
      TTCut::lastDirPath,
      "Project(*.prj)");

  if (!fn.isEmpty()) {
    openProjectFile(fn);
  }
}

/* /////////////////////////////////////////////////////////////////////////////
 * Menu "File save" action
 */
void TTCutMainWindow::onFileSave()
{
  TTCutProject* projectFile;

  // no video open
  if (!TTCut::isVideoOpen) {
    log->warningMsg(oName, "no video open");
    return;
  }

  // Ask for file name
  if (TTCut::projectFileName.isEmpty()) 
  {

    TTCut::projectFileName = ttChangeFileExt(mpegStream->fileName(), "prj");
    QFileInfo prjFileInfo(QDir(TTCut::lastDirPath), TTCut::projectFileName);

    TTCut::projectFileName = QFileDialog::getSaveFileName(this,
        tr("Save project-file"),
        prjFileInfo.absoluteFilePath(),
        "Project(*.prj)");

    if (TTCut::projectFileName.isEmpty())
      return;
  }

  // append project file extension
  QFileInfo fInfo(TTCut::projectFileName);

  if (fInfo.suffix().isEmpty())
  {
    TTCut::projectFileName.append(".prj");
  }

  try {
    projectFile = new TTCutProject(TTCut::projectFileName, QIODevice::WriteOnly);
  } catch (TTCutProjectOpenException) {
    log->errorMsg(oName, "error save project file: %s", TTCut::toAscii(TTCut::projectFileName));
    return;
  }

  projectFile->clearFile();

  // write video file section
  projectFile->writeVideoSection(true);
  projectFile->writeVideoFileName(mpegStream->fileInfo()->absoluteFilePath());
  projectFile->writeVideoSection(false);

  // write audio files section
  audioList->writeToProject(projectFile);

  // write cut section
  cutListData->writeToProject(projectFile);

  // close project file
  delete projectFile;
}


/* /////////////////////////////////////////////////////////////////////////////
 * Menu "File save as" action
 */
void TTCutMainWindow::onFileSaveAs()
{
  if (!TTCut::isVideoOpen)
    return;

  TTCut::projectFileName = ttChangeFileExt(mpegStream->fileName(), "prj");
  QFileInfo prjFileInfo(QDir(TTCut::lastDirPath), TTCut::projectFileName);

  TTCut::projectFileName = QFileDialog::getSaveFileName( this,
      tr("Save project-file as"),
      prjFileInfo.absoluteFilePath(),
      "Project(*.prj)" );

  if (!TTCut::projectFileName.isEmpty())
  {
    QFileInfo fInfo(TTCut::projectFileName);
    TTCut::lastDirPath = fInfo.absolutePath();
 
    onFileSave();
  }
}


/* /////////////////////////////////////////////////////////////////////////////
 * Menu "Recent files..." action
 */
void TTCutMainWindow::onFileRecent()
{
  QAction* action = qobject_cast<QAction*>(sender());

  if (action) 
  {
    //log->infoMsg(oName, "open recent project file: %s", TTCut::toAscii(action->data().toString()));
    openProjectFile(action->data().toString());
  }
}


/* /////////////////////////////////////////////////////////////////////////////
 * Menu "Exit" action
 */
void TTCutMainWindow::onFileExit()
{
  close();

  qApp->quit();
}

/* /////////////////////////////////////////////////////////////////////////////
 * React to the application window close event
 * - save settings
 * - ask for saving changes
 * - close the project
 */
void TTCutMainWindow::closeEvent(QCloseEvent* event)
{
  // If project file open and has changes ask for save changes
  if (TTCut::isVideoOpen) 
  {
    if (ttAssigned(cutListData) && cutListData->count() > 0) 
    {
      log->infoMsg(oName, "TODO: Ask for saving changes in project");
    }
  }

  closeProject();
  
  if ( ttAssigned(settings) ) 
    settings->writeSettings();

  event->accept();
}

/* /////////////////////////////////////////////////////////////////////////////
 * Menu "Save current frame" action
 */
void TTCutMainWindow::onActionSave()
{
  currentFrame->saveCurrentFrame();
}

/* /////////////////////////////////////////////////////////////////////////////
 * Menu "Settings" action
 */
void TTCutMainWindow::onActionSettings()
{
  TTCutSettingsDlg* settingsDlg = new TTCutSettingsDlg( this );
  settingsDlg->exec();

  log->enableLogFile(TTCut::createLogFile);
  log->setLogModeConsole(TTCut::logModeConsole);
  log->setLogModeExtended(TTCut::logModeExtended);

  if ( ttAssigned(settings) ) 
    settings->writeSettings();

  delete settingsDlg;
}

/* /////////////////////////////////////////////////////////////////////////////
 * Menu "About" action
 */
void TTCutMainWindow::onHelpAbout()
{
  TTCutAboutDlg about(this);
  about.exec();
}

/* /////////////////////////////////////////////////////////////////////////////
 * Signals from the video info widget
 */

/* /////////////////////////////////////////////////////////////////////////////
 * Signal from open video action
 */
void TTCutMainWindow::onReadVideoStream(QString fName)
{
  if (openVideoStream(fName) <= 0) 
  {
    return;
  }

  initStreamNavigator();

  QFileInfoList audioInfoList = audioFromVideoName(fName);

  if (audioInfoList.count() == 0)
  {
    TTCut::numAudioTracks = 0;
    audioFileInfo->onFileOpen();
    return;
  }

  for (int i = 0; i < audioInfoList.count(); i++)
    onReadAudioStream(audioInfoList.at(i).absoluteFilePath());
}

/* /////////////////////////////////////////////////////////////////////////////
 * Signals from the audio list view widget
 */

/* /////////////////////////////////////////////////////////////////////////////
 * Signal from open audio action
 */
void TTCutMainWindow::onReadAudioStream(QString fName)
{
  openAudioStream(fName);
}


/* /////////////////////////////////////////////////////////////////////////////
 * Video slider position changed signal
 */
void TTCutMainWindow::onVideoSliderChanged(int sPos)
{
  if( sliderUpdateFrame && TTCut::isVideoOpen ) {
    if( TTCut::fastSlider )
      currentFrame->onGotoFrame( sPos, 1 );
    else
      currentFrame->onGotoFrame( sPos, 0 );

    videoFileInfo->setVideoInfo( mpegStream );
    navigation->checkCutPosition( mpegStream );
  }
  sliderUpdateFrame = true;
}

/* /////////////////////////////////////////////////////////////////////////////
 * Signals from the current frame widget
 */

/* /////////////////////////////////////////////////////////////////////////////
 * New current frame position
 */
void TTCutMainWindow::onNewFramePos(int newPos)
{
  sliderUpdateFrame = false;
  streamNavigator->slider()->setValue( newPos );
  videoFileInfo->setVideoInfo( mpegStream );
  navigation->checkCutPosition( mpegStream );
}

/* /////////////////////////////////////////////////////////////////////////////
 * Signals from the cut list widget
 */

/* /////////////////////////////////////////////////////////////////////////////
 * Create cut preview for current cut list
 */
void TTCutMainWindow::onPreviewCut(int index)
{
  TTAudioStream* audioStream = NULL;

  if (TTCut::isVideoOpen && cutListData->count() > 0) {

    if (audioList->count() > 0){
      audioStream = audioList->audioStreamAt(0);
    }    

    // create preview dialog frame
    TTCutPreview* cutPreview = new TTCutPreview( this );

    // set video/audio index- and header lists
    cutPreview->initPreview(mpegStream, audioStream, cutListData);
    cutPreview->createPreview(index);

    // execute modal dialog frame
    cutPreview->exec();

    if ( ttAssigned(settings) ) 
      settings->writeSettings();
  }
}

/* /////////////////////////////////////////////////////////////////////////////
 * Do video and audio cut
 */
void TTCutMainWindow::onAudioVideoCut(__attribute__ ((unused))int index, bool cutAudioOnly)
{
  QString        AudioDateiEndung;
  QString        HString;
  int            AudioAnzahl;
  int            muxIndex = 0;
  int            list_pos = 0;
  QString        videoCutName;
  QString        audio_cut_name;
  QString        audio_number;
  QFileInfo      video_cut_file_info;
  QFileInfo      audio_cut_file_info;
  uint           len1, len2, len;
  TTFileBuffer*  video_cut_stream;
  TTFileBuffer*  audio_cut_stream;
  TTAudioStream* current_audio_stream;
  TTProgressBar* progress_bar;

  // no video stream open or no cut sequences defined; exit
  if ( !TTCut::isVideoOpen || cutListData->count() == 0 )
    return;

  if ( ttAssigned(settings) ) 
      settings->writeSettings();
 
  // --------------------------------------------------------------------------
  // compose video cut name from video file name
  // --------------------------------------------------------------------------
  // get video file file-extension
  QString sExt = mpegStream->fileInfo()->suffix();

  // remove the extension
  len1 = sExt.length();
  len2 = mpegStream->fileInfo()->fileName().length();

  len  = len2 - len1 - 1;

  videoCutName = mpegStream->fileInfo()->fileName();
  videoCutName.truncate(len);

  // append new cut name
  videoCutName += "_cut.m2v";

  // set global cut video name
  TTCut::cutVideoName = videoCutName;

  // --------------------------------------------------------------------------
  // start dialog for cut options
  // --------------------------------------------------------------------------
  TTCutAVCutDlg*  cutAVDlg = new TTCutAVCutDlg(this);


  // user cancel; exit
  if ( cutAVDlg->exec() == 1 )
  {
    delete cutAVDlg;
    return;
  }

  // dialog exit with start
  // --------------------------------------------------------------------------
  delete cutAVDlg;

  // set new video cut name
  videoCutName = TTCut::cutVideoName;

  video_cut_file_info.setFile ( QDir(TTCut::cutDirPath), videoCutName );
  videoCutName = video_cut_file_info.absoluteFilePath();

  // if file exists delete it
  if (video_cut_file_info.exists()) {
    // TODO: User warning about deleting file
    log->warningMsg(oName, "deleting existing video cut file: %s", TTCut::toAscii(videoCutName));
    QFile tempFile(videoCutName);
    tempFile.remove();
    tempFile.close();
  }

  // --------------------------------------------------------------------------
  // Cut video-file
  // --------------------------------------------------------------------------

  // cut only audio ???
  if ( !cutAudioOnly )
  {
    //qDebug("Meldung125: Die Videodaten werden in der Datei %s gespeichert.",videoCutName.ascii());

    progress_bar = new TTProgressBar( this );
    progress_bar->show();
    qApp->processEvents();

     video_cut_stream = new TTFileBuffer(videoCutName.toUtf8().constData(), fm_open_write );

    mpegStream->setProgressBar( progress_bar );

    mpegStream->cut( video_cut_stream, cutListData );

    // video cut canceled
    if (progress_bar->isCanceled())
    {
      QFile videoFile(videoCutName);
      videoFile.remove();

      // clean up
      delete progress_bar;
      delete video_cut_stream;

      return;
    }

    muxIndex = muxListData->addItem(QString::fromUtf8(video_cut_stream->fileName()));

    delete progress_bar;
    delete video_cut_stream;
  } // Ende Videoschnitt

  // --------------------------------------------------------------------------
  // Cut audio-file
  // --------------------------------------------------------------------------

  AudioAnzahl = audioList->count();

  //qDebug( "%sEs wird(werden) %d Audiodatei(en) erzeugt.",c_name,AudioAnzahl );

  list_pos = 0;

  while ( AudioAnzahl > 0 )
  {
    current_audio_stream = audioList->audioStreamAt( list_pos );

    //qDebug( "%s: current audio stream: %s", oName, qPrintable(current_audio_stream->fileName()));

    // Quick and ugly: make it better ;-)
    // ------------------------------------------------------------------------
    audio_cut_file_info = video_cut_file_info;

    audio_cut_name = audio_cut_file_info.completeBaseName();
    audio_number.sprintf( "_%03d.",list_pos+1 );
    audio_cut_name += audio_number;
    audio_cut_file_info.setFile( current_audio_stream->fileName() );
    audio_cut_name += audio_cut_file_info.suffix();
    audio_cut_file_info.setFile( QDir(TTCut::cutDirPath), audio_cut_name );
    audio_cut_name = audio_cut_file_info.absoluteFilePath();
    // ------------------------------------------------------------------------

    //qDebug( "%s: audio cut file: %s", oName, qPrintable(audio_cut_name));

    // audio file exists
    if (audio_cut_file_info.exists()) {
      // TODO: Warning about deleting file
      log->warningMsg(oName, "deleting existing audio cut file: %s", audio_cut_name.toLatin1().constData());
      QFile tempFile(audio_cut_name);
      tempFile.remove();
      tempFile.close();
    }

    progress_bar = new TTProgressBar( this );
    progress_bar->show();
    qApp->processEvents();

    current_audio_stream->setProgressBar( progress_bar );

    audio_cut_stream = new TTFileBuffer(qPrintable(audio_cut_name), fm_open_write );

    current_audio_stream->cut( audio_cut_stream, cutListData );

    // audio cut canceled
    if (progress_bar->isCanceled())
    {
      QFile audioFile(audio_cut_name);
      audioFile.remove();

      // clean up
      delete progress_bar;
      delete current_audio_stream;

      return;
    }

    if (!cutAudioOnly)
      muxListData->appendAudioName(muxIndex, QString::fromUtf8(audio_cut_stream->fileName()));

    delete progress_bar;
    delete audio_cut_stream;

    list_pos++;
    AudioAnzahl--;
  }
  // Ende Audioschnitt

  if (cutAudioOnly)
    return;

  // mux list / direct mux
  // muxListData->print();
  if (TTCut::muxMode == 1)
    mplexProvider->writeMuxScript(muxListData);
  else
    mplexProvider->mplexPart(muxListData, muxListData->count()-1);
}

void TTCutMainWindow::onAudioCut(__attribute__ ((unused))int index)
{
  onAudioVideoCut(index, true);
}

/* /////////////////////////////////////////////////////////////////////////////
 * Service methods
 */

/* /////////////////////////////////////////////////////////////////////////////
 * open video and audio stream and create according index and header lists
 */
void TTCutMainWindow::createAVStreams(QString videoFile, QString audioFile)
{
  log->infoMsg(oName, "Create video stream: %s", TTCut::toAscii(videoFile));
  log->infoMsg(oName, "Create audio stream: %s", TTCut::toAscii(audioFile));
}

/* /////////////////////////////////////////////////////////////////////////////
 * Search for audiofiles acording to the video file name
 * Valid audio extensions are: mpa, mp2, ac3
 */
QFileInfoList TTCutMainWindow::audioFromVideoName(QString videoFile)
{
  QString       videoBaseName; 
  QFileInfo     videoFileInfo(videoFile);
  QStringList   audioFilters;
  QDir          audioDir(videoFileInfo.absoluteDir());
  QFileInfoList audioInfoList;

  //  video file name without extension
  videoBaseName = videoFileInfo.completeBaseName();

  audioFilters << videoBaseName+"*"+".mpa"
               << videoBaseName+"*"+".mp2"
               << videoBaseName+"*"+".ac3";

  audioDir.setNameFilters(audioFilters);
  audioDir.setFilter(QDir::Files);

  return audioDir.entryInfoList();
}

/* /////////////////////////////////////////////////////////////////////////////
 * Close current project or video file
 */
void TTCutMainWindow::closeProject()
{
  if (TTCut::isVideoOpen) 
  {
    videoFileInfo->resetVideoInfo();
    audioFileInfo->clearList();
    audioList->deleteAll();
    cutListData->deleteAll();
    cutList->clearList();
    streamNavigator->slider()->setValue(0);
    navigationEnabled(false);
    currentFrame->closeVideoStream();
    cutOutFrame->closeVideoStream();
    TTCut::isVideoOpen     = false;
    TTCut::projectFileName = "";

    delete cutListData;
    cutListData = 0;

    if (mpegStream != 0)
    {
      delete mpegStream;
      mpegStream = 0;
    }
  }
}

/* /////////////////////////////////////////////////////////////////////////////
 * Enable or disable navigation
 */
void TTCutMainWindow::navigationEnabled( bool enabled )
{
  cutOutFrame->controlEnabled( enabled );
  currentFrame->controlEnabled( enabled );
  navigation->controlEnabled( enabled );
  streamNavigator->controlEnabled( enabled );
}

/* /////////////////////////////////////////////////////////////////////////////
 * Open project file
 */
bool TTCutMainWindow::openProjectFile(QString fName)
{
  bool result = false;
  TTCutProject* projectFile = 0;

  TTCut::projectFileName = fName;

  closeProject();

  // error opening project file
  try 
  {
    projectFile = new TTCutProject(fName, QIODevice::ReadOnly);

    QFileInfo fInfo(fName );
    TTCut::lastDirPath = fInfo.absolutePath();
  } catch (TTCutProjectOpenException) 
  {
    log->errorMsg(oName, "error open project file: %s", TTCut::toAscii(fName));
    delete projectFile;
    return result;
  }

  // open and read video file
  projectFile->seekToVideoSection();
  QString videoFileName;
  if (projectFile->readVideoFileName(videoFileName)) {
    openVideoStream(videoFileName);
  }

  if (!TTCut::isVideoOpen) {
    log->errorMsg(oName, "error open video file: %s", TTCut::toAscii(videoFileName));
    return result;
  }

  // open and read audio files
  projectFile->seekToAudioSection();
  QString audioFileName;
  while (projectFile->readAudioFileName(audioFileName)) {
    openAudioStream(audioFileName);
  }

  if (audioList->count() == 0) {
    log->warningMsg(oName, "no audio files in project: %s", TTCut::toAscii(fName));
    audioFileInfo->onFileOpen();
  }

  // read cut positions
  projectFile->seekToCutSection();
  cutListData = new TTCutListData(mpegStream);
  cutList->setListData(cutListData);
  int cutInPos;
  int cutOutPos;
  while (projectFile->readCutEntry(cutInPos, cutOutPos)) {
    cutList->onAddEntry(cutInPos, cutOutPos);
  }

  initStreamNavigator();

  // add project file to recent file list
  insertRecentFile(fName);

  // remove project file
  delete projectFile;

  result = true;

  return result;
}

/* /////////////////////////////////////////////////////////////////////////////
 * Open video stream and create video index and header lists
 */
int TTCutMainWindow::openVideoStream(QString fName)
{
  int result = -1;
  TTVideoType*   videoType;
  TTProgressBar* progressBar;
  QFileInfo fInfo(fName);

  if (!fInfo.exists()) {
    log->errorMsg(oName, "Video file doesn't exists: %s", TTCut::toAscii(fName));
    return result;
  }

  log->infoMsg(oName, "Read video stream: %s", TTCut::toAscii(fName));

  // Close current project
  if(TTCut::isVideoOpen){
    closeProject();
  }

  videoType = new TTVideoType( fName );

  // MPEG2 video
  if(!videoType->avStreamType() == TTAVTypes::mpeg2_demuxed_video) {
    log->errorMsg(oName, "wrong video type: %s", TTCut::toAscii(fName));
    return result;
  }

  mpegStream = (TTMpeg2VideoStream*)videoType->createVideoStream();

  // init progress bar
  progressBar = new TTProgressBar( this );
  mpegStream->setProgressBar( progressBar );
  progressBar->show();
  qApp->processEvents();

  // create header- and index-list for mpeg stream
  int numHeader = mpegStream->createHeaderList();
  int numIndex  = 0;

  if( numHeader > 0 ){
    numIndex = mpegStream->createIndexList(); 
  }else{
    log->errorMsg(oName, "no header list created");
    delete progressBar;
    delete videoType;
    mpegStream->setProgressBar((TTProgressBar*)NULL);    
    mpegStream = (TTMpeg2VideoStream*)NULL;
    return result;
  }

  if (numIndex == 0) {
    log->errorMsg(oName, "no index list created");
    delete progressBar;
    delete videoType;
    mpegStream->setProgressBar((TTProgressBar*)NULL);    
    mpegStream = (TTMpeg2VideoStream*)NULL;
    return result;
  }

  result = numIndex;

  // Videostream successfully openend, header and indexlists created
  progressBar->hide();
  TTCut::isVideoOpen = true;
  mpegStream->indexList()->sortDisplayOrder();

  videoFileInfo->setVideoInfo( mpegStream );

  // show video stream in current frame and prepare cut-out frame preview
  currentFrame->initVideoStream( mpegStream );
  cutOutFrame->initVideoStream( mpegStream );

  onNewFramePos( 0 );

  mpegStream->setProgressBar((TTProgressBar*)NULL);    

  delete progressBar;
  delete videoType;

  return result;
}

/* /////////////////////////////////////////////////////////////////////////////
 * Open audio stream
 */
int TTCutMainWindow::openAudioStream(QString fName)
{
  int result = -1;
  TTAudioType*   audio_type;
  TTAudioStream* current_audio_stream;
  TTProgressBar* progress_bar;

  log->infoMsg(oName, "Read audio stream: %s", fName.toLatin1().constData());

  // get the stream type and create according stream-object
  audio_type   = new TTAudioType( fName );

  // create the audio stream object for the first audio file
  if ( audio_type->avStreamType() == TTAVTypes::mpeg_audio  ||
      audio_type->avStreamType() == TTAVTypes::ac3_audio   ||
      audio_type->avStreamType() == TTAVTypes::dts14_audio ||
      audio_type->avStreamType() == TTAVTypes::dts16_audio ||
      audio_type->avStreamType() == TTAVTypes::pcm_audio      ) {

    current_audio_stream = (TTAudioStream*)audio_type->createAudioStream();

    // set progress bar
    progress_bar = new TTProgressBar( this );
    current_audio_stream->setProgressBar( progress_bar );
    progress_bar->show();
    qApp->processEvents();

    // create header list for audio stream
    int num_header = current_audio_stream->createHeaderList();

    // error reading audio stream or user abort during operation
    if ( num_header == 0 && audio_type->avStreamType() != TTAVTypes::pcm_audio ||
        num_header == 1 && audio_type->avStreamType() == TTAVTypes::pcm_audio    ) {

      log->errorMsg( oName, "error reading audio stream; no header list (!)" );
      delete current_audio_stream;

    } else {

      // audio stream succesfully parsed; add item to list
      int curIndex = audioList->addItem( fName, current_audio_stream );
      audioFileInfo->addItem(audioList->itemAt(curIndex));
      audioList->print();

      result = curIndex;

      // first audio track loaded
      TTCut::numAudioTracks = audioList->count();
    }

    delete progress_bar;
    current_audio_stream->setProgressBar( (TTProgressBar*)NULL );

  } else {
    log->errorMsg(oName, "wrong audio type for file: %s", TTCut::toAscii(fName));
  }

  delete audio_type;
  return result;
}

/* /////////////////////////////////////////////////////////////////////////////
 * Intitialize stream navigator display and cut list data
 */
void TTCutMainWindow::initStreamNavigator()
{
  if (cutListData == 0)
  {
    cutListData = new TTCutListData(mpegStream);
    cutList->setListData(cutListData);
  }

  streamNavigator->setMinValue(0);
  streamNavigator->setMaxValue(mpegStream->frameCount()-1);
  streamNavigator->setCutListData(cutListData);

  navigationEnabled( true );
}

/* /////////////////////////////////////////////////////////////////////////////
 * Update recent file menu actions
 */
void TTCutMainWindow::updateRecentFileActions()
{
  int numRecentFiles = qMin(TTCut::recentFileList.size(), (int)MaxRecentFiles);

  for (int i = 0; i < numRecentFiles; ++i) {
    QString text = tr("&%1 %2").arg(i+1).
      arg(QFileInfo(TTCut::recentFileList[i]).fileName());
    recentFileAction[i]->setText(text);
    recentFileAction[i]->setData(TTCut::recentFileList[i]);
    recentFileAction[i]->setVisible(true);
  }

  for (int j = numRecentFiles; j < MaxRecentFiles; ++j) {
    recentFileAction[j]->setVisible(false);
  }
}

/* /////////////////////////////////////////////////////////////////////////////
 * Insert new file in recent file list
 */
void TTCutMainWindow::insertRecentFile(const QString& fName)
{
  TTCut::recentFileList.removeAll(fName);
  TTCut::recentFileList.prepend(fName);

  while (TTCut::recentFileList.size() > MaxRecentFiles) {
    TTCut::recentFileList.removeLast();
  }

  foreach (QWidget* widget, QApplication::topLevelWidgets()) {
    TTCutMainWindow* mainWin = qobject_cast<TTCutMainWindow*>(widget);
    if (mainWin) {
      mainWin->updateRecentFileActions();
    }
  }
}
