
#include <QtGui>
#include <QPixmap>

#include "ttcutmainwindow.h"
#include "ttcutavcutdlg.h"
#include "ttprogressbar.h"

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


const char oName[] = "TTCutMainWindow";

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


  // Message logger instance
  log = TTMessageLogger::getInstance();

  // Settings
  settings = new TTCutSettings();
  settings->readSettings();
  
  // Audio list
  audioList = new TTAudioListData();

  // no navigation
  navigationEnabled( false );
  
  // Signal and slot connections
  // 
  // Connect signals from main menu
  // --------------------------------------------------------------------------
  connect(actionFileNew,          SIGNAL(activated()), SLOT(onFileNew()));
  connect(actionFileOpen,         SIGNAL(activated()), SLOT(onFileOpen()));
  connect(actionFileSave,         SIGNAL(activated()), SLOT(onFileSave()));
  connect(actionFileSaveAs,       SIGNAL(activated()), SLOT(onFileSaveAs()));
  connect(actionRecentProjects,   SIGNAL(activated()), SLOT(onFileRecent()));
  connect(actionExit,             SIGNAL(activated()), SLOT(onFileExit()));
  connect(actionSaveCurrentFrame, SIGNAL(activated()), SLOT(onActionSave()));
  connect(actionSettings,         SIGNAL(activated()), SLOT(onActionSettings()));
  connect(actionAbout,            SIGNAL(activated()), SLOT(onHelpAbout()));

  // Connect signals from video and audio info
  // --------------------------------------------------------------------------
  connect(videoFileInfo,          SIGNAL(fileOpened(QString)), SLOT(onReadVideoStream(QString)));
  connect(audioFileInfo,          SIGNAL(fileOpened(QString)), SLOT(onReadAudioStream(QString)));

  // Connect signals from navigation widget
  // --------------------------------------------------------------------------
  connect(navigation, SIGNAL(prevIFrame()),     currentFrame, SLOT(onPrevIFrame()));
  connect(navigation, SIGNAL(nextIFrame()),     currentFrame, SLOT(onNextIFrame()));
  connect(navigation, SIGNAL(prevPFrame()),     currentFrame, SLOT(onPrevPFrame()));
  connect(navigation, SIGNAL(nextPFrame()),     currentFrame, SLOT(onNextPFrame()));
  connect(navigation, SIGNAL(prevBFrame()),     currentFrame, SLOT(onPrevBFrame()));
  connect(navigation, SIGNAL(nextBFrame()),     currentFrame, SLOT(onNextBFrame()));
  connect(navigation, SIGNAL(setCutOut(int)),   currentFrame, SLOT(onSetCutOut(int)));
  connect(navigation, SIGNAL(setCutOut(int)),   cutOutFrame,  SLOT(onGotoCutOut(int)));
  connect(navigation, SIGNAL(setCutIn(int)),    currentFrame, SLOT(onSetCutIn(int)));
  connect(navigation, SIGNAL(gotoCutIn(int)),   currentFrame, SLOT(onGotoCutIn(int)));
  connect(navigation, SIGNAL(gotoCutOut(int)),  currentFrame, SLOT(onGotoCutOut(int)));
  connect(navigation, SIGNAL(addCutRange(int, int)), cutList,   SLOT(onAddEntry(int, int)));
  connect(navigation, SIGNAL(gotoMarker(int)),  currentFrame, SLOT(onGotoMarker(int)));

  // Connect signal from video slider
  // --------------------------------------------------------------------------
  connect(slider, SIGNAL(valueChanged(int)), SLOT(onVideoSliderChanged(int)));

  // Connect signals from cut-out frame widget
  // --------------------------------------------------------------------------
  connect(cutOutFrame, SIGNAL(equalFrameFound(int)), currentFrame, SLOT(onGotoFrame(int)));

  // Connect signals from current frame widget
  // --------------------------------------------------------------------------
  connect(currentFrame, SIGNAL(newFramePos(int)), SLOT(onNewFramePos(int))); 

  // Connect signals from cut list widget
  // --------------------------------------------------------------------------
  connect(cutList, SIGNAL(entrySelected(int)), cutOutFrame, SLOT(onGotoCutOut(int)));
  connect(cutList, SIGNAL(entryEdit(const TTCutListDataItem&)), navigation, SLOT(onEditCut(const TTCutListDataItem&)));
  connect(cutList, SIGNAL(gotoCutIn(int)),  currentFrame, SLOT(onGotoFrame(int)));
  connect(cutList, SIGNAL(gotoCutOut(int)), currentFrame, SLOT(onGotoFrame(int)));
  connect(cutList, SIGNAL(previewCut(int)),    SLOT(onPreviewCut(int)));
  connect(cutList, SIGNAL(audioVideoCut(int)), SLOT(onAudioVideoCut(int)));
  connect(cutList, SIGNAL(audioCut(int)),      SLOT(onAudioCut(int)));
}

// Signals from the application menu
// ----------------------------------------------------------------------------

//! Menu "File new" action
void TTCutMainWindow::onFileNew()
{
}

//! Menu "File open" action
void TTCutMainWindow::onFileOpen()
{
}

//! Menu "File save" action
void TTCutMainWindow::onFileSave()
{
}


//! Menu "File save as" action
void TTCutMainWindow::onFileSaveAs()
{
}

//! Menu "Recent files..." action
void TTCutMainWindow::onFileRecent()
{
}

//! Menu "Exit" action
void TTCutMainWindow::onFileExit()
{
  // If project file open and has changes ask for save changes

  // Save application setting
  if ( ttAssigned(settings) ) {
    settings->writeSettings();
    delete settings;
  }

  // Quit TTCut
  qApp->quit();
}


//! Menu "Save current frame" action
void TTCutMainWindow::onActionSave()
{
  currentFrame->saveCurrentFrame();
}

//! Menu "Settings" action
void TTCutMainWindow::onActionSettings()
{
  TTCutSettingsDlg* settingsDlg = new TTCutSettingsDlg( this );
  settingsDlg->exec();

  delete settingsDlg;
}

//! Menu "About" action
void TTCutMainWindow::onHelpAbout()
{
}

// Signals from the video info widget
// ----------------------------------------------------------------------------

//! Signal from open video action
void TTCutMainWindow::onReadVideoStream(QString fName)
{
  TTVideoType*   videoType;
  TTProgressBar* progressBar;

  log->infoMsg(oName, "Read video stream: %s", qPrintable(fName));

  // Close current project
  if(TTCut::isVideoOpen){
    closeProject();
  }

  videoType = new TTVideoType( fName );

  // MPEG2 video
  if( videoType->avStreamType() == TTAVTypes::mpeg2_demuxed_video ){

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
      log->warningMsg(oName, "no header list created");
    }

    if(numIndex > 0){

      // Videostream successfully openend, header and indexlists created
      progressBar->hide();
      TTCut::isVideoOpen = true;
      mpegStream->indexList()->sortDisplayOrder();

      videoFileInfo->setVideoInfo( mpegStream );
        
      QString audioName = audioFromVideoName(fName);

      if ( !audioName.isEmpty() ) {
        onReadAudioStream(audioName);

        if ( TTCut::numAudioTracks ==  0 ) {
          // show open file dialog
        }
      }

      // initialize cut list data object
      cutListData = new TTCutListData( mpegStream );
      cutList->setListData( cutListData );

      // show video stream in current frame and prepare cut-out frame preview
      currentFrame->initVideoStream( mpegStream );
      cutOutFrame->initVideoStream( mpegStream );

      slider->setMinimum( 0 );
      slider->setMaximum( mpegStream->frameCount()-1 );
      onNewFramePos( 0 );

      navigationEnabled( true );

    } else {
      log->warningMsg(oName, "no index list created");
      delete mpegStream;
      mpegStream = (TTMpeg2VideoStream*)NULL;
    }

    mpegStream->setProgressBar((TTProgressBar*)NULL);    
    delete progressBar;

  }else{
    log->warningMsg(oName, "wrong video type for file: %s", qPrintable(fName));
  }

  delete videoType;
}

// Signals from the audio list view widget
// ----------------------------------------------------------------------------

//! Signal from open audio action
void TTCutMainWindow::onReadAudioStream(QString fName)
{
  TTAudioType*   audio_type;
  TTAudioStream* current_audio_stream;
  TTProgressBar* progress_bar;

  log->infoMsg(oName, "Read audio stream: %s", qPrintable(fName));

  // get the strem type and create according stream-object
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
      int numTracks = audioList->addItem( fName, current_audio_stream );

      // first audio track loaded
      TTCut::numAudioTracks = numTracks;
    }

    delete progress_bar;
    current_audio_stream->setProgressBar( (TTProgressBar*)NULL );

  } else {
    log->errorMsg(oName, "wrong audio type for file: %s", qPrintable(fName));
  }

  delete audio_type;
}


// Video slider position changed signal
// ----------------------------------------------------------------------------

//! Video slider position changed
void TTCutMainWindow::onVideoSliderChanged(int sPos)
{
  if( sliderUpdateFrame && TTCut::isVideoOpen ) {
    if( TTCut::fastSlider )
      currentFrame->onGotoFrame( sPos, 1 );
    else
      currentFrame->onGotoFrame( sPos, 0 );

    videoFileInfo->setVideoInfo( mpegStream );
    navigation->checkCutPosition( mpegStream );

    // not good (!)qApp->processEvents();
  }
  sliderUpdateFrame = true;
}

// Signals from the navigation widget
// ----------------------------------------------------------------------------

//! Add specified cut range to cut list
void TTCutMainWindow::onAddCutRange(int cutIn, int cutOut)
{
  // FIXEM: Mybe not used (!)
}

// Signals from cut-out widget
// ----------------------------------------------------------------------------

//! Navigate to previous possible cut out position
void TTCutMainWindow::onPrevCutOutPos()
{
}

//! Navigate to next possible cut out position
void TTCutMainWindow::onNextCutOutPos()
{
}

//! Search for equal frame from current position
void TTCutMainWindow::onSearchFrame()
{
}

// Signals from the current frame widget
// ----------------------------------------------------------------------------

//! New current frame position
void TTCutMainWindow::onNewFramePos(int newPos)
{
  sliderUpdateFrame = false;
  slider->setValue( newPos );
  videoFileInfo->setVideoInfo( mpegStream );
  navigation->checkCutPosition( mpegStream );
}

// Signals from the cut list widget
// ----------------------------------------------------------------------------
void TTCutMainWindow::onPreviewCut(int index)
{
  TTAudioStream* audioStream = NULL;

  if (TTCut::isVideoOpen && cutListData->count() > 0) {

    if (audioList->count() > 0){
      audioStream = audioList->audioStream(0);
    }    

    // create preview dialog frame
    TTCutPreview* cutPreview = new TTCutPreview( this );

    // set video/audio index- and header lists
    cutPreview->initPreview(mpegStream, audioStream, cutListData);
    cutPreview->createPreview(index);

    // execute modal dialog frame
    cutPreview->exec();
  }
}

void TTCutMainWindow::onAudioVideoCut(int index)
{
  QString        AudioDateiEndung;
  QString        HString;
  int            AudioAnzahl;
  int list_pos = 0;
  bool           nurAudioSchneiden;
  QString        videoCutName;
  QString        audio_cut_name;
  QString        audio_number;
  QFileInfo      video_cut_file_info;
  QFileInfo      audio_cut_file_info;
  uint len1, len2, len;
  TTFileBuffer* video_cut_stream;
  TTFileBuffer* audio_cut_stream;
  TTAudioStream* current_audio_stream;
  TTProgressBar* progress_bar;

  // no video stream open or no cut sequences defined; exit
  if ( !TTCut::isVideoOpen || cutListData->count() == 0 )
    return;

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

  // cut video and audio
  nurAudioSchneiden = false;

  // --------------------------------------------------------------------------
  // start dialog for cut options
  // --------------------------------------------------------------------------
  TTCutAVCutDlg*  cutAVDlg = new TTCutAVCutDlg( this, "CutAVDlg" );


  // user cancel; exit
  if ( cutAVDlg->exec() == 1 )
  {
    return;
  }

  // dialog exit with start
  // --------------------------------------------------------------------------
  // set new video cut name
  videoCutName = TTCut::cutVideoName;

  video_cut_file_info.setFile ( QDir(TTCut::cutDirPath), videoCutName );
  videoCutName = video_cut_file_info.absoluteFilePath();

  // --------------------------------------------------------------------------
  // Cut video-file
  // --------------------------------------------------------------------------

  // cut only audio ???
  if ( !nurAudioSchneiden )
  {
    //qDebug("Meldung125: Die Videodaten werden in der Datei %s gespeichert.",videoCutName.ascii());

    progress_bar = new TTProgressBar( this );
    progress_bar->show();
    qApp->processEvents();

    video_cut_stream = new TTFileBuffer(qPrintable(videoCutName), fm_open_write );

    mpegStream->setProgressBar( progress_bar );

    mpegStream->cut( video_cut_stream, cutListData );

    //qDebug("Meldung128: Das Schneiden der Datei %s ist beendet.",HString.ascii());
    delete progress_bar;
    delete video_cut_stream;
  }
  // Ende Videoschnitt

  // --------------------------------------------------------------------------
  // Cut audio-file
  // --------------------------------------------------------------------------

  AudioAnzahl = audioList->count();

  //qDebug( "%sEs wird(werden) %d Audiodatei(en) erzeugt.",c_name,AudioAnzahl );

  list_pos = 0;

  while ( AudioAnzahl > 0 )
  {
    current_audio_stream = audioList->audioStream( list_pos );

    //qDebug( "%scurrent audio stream: %s",c_name,current_audio_stream->fileName().ascii() );

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

    //qDebug( "%saudio cut file: %s",c_name,audio_cut_name.ascii() );

    progress_bar = new TTProgressBar( this );
    progress_bar->show();
    qApp->processEvents();

    current_audio_stream->setProgressBar( progress_bar );

    audio_cut_stream = new TTFileBuffer(qPrintable(audio_cut_name), fm_open_write );

    current_audio_stream->cut( audio_cut_stream, cutListData );

    delete progress_bar;
    delete audio_cut_stream;

    list_pos++;
    AudioAnzahl--;
  }
  // Ende Audioschnitt
}

void TTCutMainWindow::onAudioCut(int index)
{
}

// Service methods
// ----------------------------------------------------------------------------

//! open video and audio stream and create according index and header lists
// -----------------------------------------------------------------------------
void TTCutMainWindow::createAVStreams(QString videoFile, QString audioFile)
{
  log->infoMsg(oName, "Create video stream: %s", qPrintable(videoFile));
  log->infoMsg(oName, "Create audio stream: %s", qPrintable(audioFile));
}

//! Search a audiofile corresponding to videofile name
QString TTCutMainWindow::audioFromVideoName(QString videoFile)
{
  QString result = "";

  // look for extension mpa
  result = ttChangeFileExt( videoFile, "mpa" );
  QFileInfo fInfo( result );

  if( fInfo.exists() ){
    return result;
  }

  // look for extension mp2
  result = ttChangeFileExt( videoFile, "mp2" );
  fInfo.setFile( result );

  if( fInfo.exists() ){
    return result;
  }

  // look for extension ac3
  result = ttChangeFileExt( videoFile, "ac3" );
  fInfo.setFile( result );

  if( fInfo.exists() ){
    return result;
  }

  // no corresponding audio file found
  return result;
}

void TTCutMainWindow::closeProject()
{
}

void TTCutMainWindow::navigationEnabled( bool enabled )
{
  cutOutFrame->controlEnabled( enabled );
  currentFrame->controlEnabled( enabled );
  navigation->controlEnabled( enabled );
  slider->setEnabled( enabled );
}