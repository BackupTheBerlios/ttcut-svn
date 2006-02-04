/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttcutmainwnd.cpp                                                */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 02/23/2005 */
/* MODIFIED: b. altendorf                                    DATE: 03/01/2005 */
/* MODIFIED: b. altendorf                                    DATE: 03/19/2005 */
/* MODIFIED: b. altendorf                                    DATE: 04/01/2005 */
/* MODIFIED: b. altendorf                                    DATE: 06/22/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTCUTMAINWND
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


#include "ttcutmainwnd.h"

// Qt header
// -----------------------------------------------------------------------------
#include <qapplication.h>
#include <qvariant.h>
#include <qpushbutton.h>
#include <qscrollbar.h>
#include <q3header.h>
#include <q3listview.h>
#include <qlabel.h>
#include <q3frame.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <q3whatsthis.h>
#include <qaction.h>
#include <qmenubar.h>
#include <q3popupmenu.h>
#include <q3toolbar.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qfile.h>
#include <q3filedialog.h>
#include <q3process.h>
//Added by qt3to4:
#include <QWheelEvent>
#include <QFocusEvent>
#include <QGridLayout>
#include <QKeyEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QResizeEvent>
#include <QGroupBox>

#include "../pixmaps/downarrow_18.xpm"
#include "../pixmaps/uparrow_18.xpm"
#include "../pixmaps/cancel_18.xpm"
#include "../pixmaps/fileopen_24.xpm"
#include "../pixmaps/filenew_16.xpm"
#include "../pixmaps/fileopen_16.xpm"
#include "../pixmaps/filesave_16.xpm"
#include "../pixmaps/filesaveas_16.xpm"
#include "../pixmaps/saveimage_16.xpm"
#include "../pixmaps/settings_16.xpm"
#include "../pixmaps/settings_18.xpm"
#include "../pixmaps/exit_16.xpm"
#include "../pixmaps/play_18.xpm"
#include "../pixmaps/stop_18.xpm"
#include "../pixmaps/search_18.xpm"
#include "../pixmaps/preview_18.xpm"
#include "../pixmaps/chapter_18.xpm"
#include "../pixmaps/cutav_18.xpm"
#include "../pixmaps/cutaudio_18.xpm"
#include "../pixmaps/goto_18.xpm"
#include "../pixmaps/note_18.xpm"
#include "../pixmaps/clock_16.xpm"
#include "../pixmaps/apply_18.xpm"
#include "../pixmaps/addtolist_18.xpm"
#include "../pixmaps/fileclose_18.xpm"


TTCutMainWnd* mainWnd;

void updateCurrentPosition()
{
  mainWnd->refreshCurrentPosition();
}

const char* c_name = "TTCUTMAINWND  : ";

// -----------------------------------------------------------------------------
// Constructs a TTCutMainWnd as a child of 'parent', with the
// name 'name' and widget flags set to 'f'.
// -----------------------------------------------------------------------------
TTCutMainWnd::TTCutMainWnd( QWidget* parent, const char* name, Qt::WFlags fl )
  : QMainWindow( parent, fl )
    //    image7( (const char **) image7_data )
{
  int video_frame_width  = 240;
  int video_frame_height = 190;

  // initialization
  // --------------------------------------------------------------------------
  mainWnd = this;

  log = TTMessageLogger::getInstance();
  int mode = TTMessageLogger::CONSOLE & TTMessageLogger::SUMMARIZE;
  log->setLogMode(mode);
  log->infoMsg(c_name, "Starting TTCut ...");
  log->infoMsg(c_name, "videoFrame: %d/%d", video_frame_width, video_frame_height);

  // settings
  // ---------------------------------------------------------------------------
  settings = new TTCutSettings();
  settings->readSettings();

  isCutInPos        = false;
  isCutOutPos       = false;
  sliderUpdateFrame = true;
  firstCutOut       = true;
  current_cut_pos   = new TTAVCutPosition();
  markerPosition    = -1;

  // initialize
  // ---------------------------------------------------------------------------
  project_file_name.clear();


  //QT3if ( !name )
  //QT3  setName( "TTCutMainWnd" );

  (void)statusBar();

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

  // min widgets size
  // --------------------------------------------------------------------------
  //setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, sizePolicy().hasHeightForWidth() ) );
  //setMinimumSize( QSize( 800, 600 ) );
  //setMaximumSize( QSize( 1600, 1280 ) );

  // central widget
  // --------------------------------------------------------------------------
  //setCentralWidget( new QWidget( this, "qt_central_widget" ) );


  // --------------------------------------------------------------------------
  // menu actions
  // --------------------------------------------------------------------------

  // file-menu
  fileNewAction        = new QAction( this, "fileNewAction" );
  fileNewAction->setIconSet( QIcon( *(TTCut::imgFileNew) ) );
  fileOpenAction       = new QAction( this, "fileOpenAction" );
  fileOpenAction->setIconSet( QIcon( *(TTCut::imgFileOpen) ) );
  fileSaveAction       = new QAction( this, "fileSaveAction" );
  fileSaveAction->setIconSet( QIcon( *(TTCut::imgFileSave) ) );
  fileSaveAsAction     = new QAction( this, "fileSaveAsAction" );
  fileSaveAsAction->setIconSet( QIcon( *(TTCut::imgFileSaveAs) ) );
  fileExitAction       = new QAction( this, "fileExitAction" );
  fileExitAction->setIconSet( QIcon( *(TTCut::imgExit) ) );
  fileRecentAction     = new QAction( this, "fileRecentAction" );
  fileRecentAction     = new QAction( this, "fileRecentAction" );

  // actions-menu
  actionSaveAction     = new QAction( this, "editUndoAction" );
  actionSaveAction->setIconSet( QIcon( *(TTCut::imgSaveImage) ) );
  actionSettingsAction = new QAction( this, "editRedoAction" );
  actionSettingsAction->setIconSet( QIcon( *(TTCut::imgSettings) ) );

  // help-menu
  helpAboutAction      = new QAction( this, "helpAboutAction" );


  // --------------------------------------------------------------------------
  // menubar
  // --------------------------------------------------------------------------
  //MenuBar = new QMenuBar( this, "MenuBar" );

  // file-menu
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(fileNewAction);
  fileMenu->addAction(fileOpenAction);
  fileMenu->addAction(fileSaveAction);
  fileMenu->addAction(fileSaveAsAction);
  fileMenu->addSeparator();
  fileMenu->addAction(fileRecentAction);
  fileMenu->addSeparator();
  fileMenu->addAction(fileExitAction);


  // actions-menu
  actionMenu = menuBar()->addMenu(tr("&Action"));
  actionMenu->addAction(actionSaveAction);
  actionMenu->addSeparator();
  actionMenu->addAction(actionSettingsAction);

  // help-menu
  helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(helpAboutAction);


  // --------------------------------------------------------------------------
  // central layout
  // --------------------------------------------------------------------------
  setObjectName(QString::fromUtf8("TTCutMainWnd"));
  resize(QSize(954, 756).expandedTo(minimumSizeHint()));
  QSizePolicy sizePolicy((QSizePolicy::Policy)3, (QSizePolicy::Policy)3);
  sizePolicy.setHorizontalStretch(0);
  sizePolicy.setVerticalStretch(0);
  sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
  setSizePolicy(sizePolicy);
  setMinimumSize(QSize(900, 800));
  setBaseSize(QSize(924, 724));

  centralWidget = new QWidget(this);
  centralWidget->setObjectName(QString::fromUtf8("centralWidget"));

  setCentralWidget( centralWidget );
  TTCut::mainWindow = centralWidget;

  // ???-----------------------------------------------------------------------
  widget = new QWidget(centralWidget);
  widget->setObjectName(QString::fromUtf8("widget"));
  widget->setGeometry(QRect(9, 381, 1060, 331));
  // ???-----------------------------------------------------------------------

  gridLayout = new QGridLayout(centralWidget);
  gridLayout->setSpacing(6);
  gridLayout->setMargin(8);
  gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

  // --------------------------------------------------------------------------
  // video file section
  // --------------------------------------------------------------------------

  // group box for video file section
  // --------------------------------------------------------------------------
  gbVideoFile = new QGroupBox(centralWidget);
  gbVideoFile->setObjectName(QString::fromUtf8("gbVideoFile"));
  QSizePolicy sizePolicy16((QSizePolicy::Policy)5, (QSizePolicy::Policy)5);
  sizePolicy16.setHorizontalStretch(0);
  sizePolicy16.setVerticalStretch(0);
  sizePolicy16.setHeightForWidth(gbVideoFile->sizePolicy().hasHeightForWidth());
  gbVideoFile->setSizePolicy(sizePolicy16);
  gridLayout7 = new QGridLayout(gbVideoFile);
  gridLayout7->setSpacing(6);
  gridLayout7->setMargin(8);
  gridLayout7->setObjectName(QString::fromUtf8("gridLayout7"));

  // video file name
  // --------------------------------------------------------------------------
  laVideoFileName = new QLabel(gbVideoFile);
  laVideoFileName->setObjectName(QString::fromUtf8("laVideoFileName"));
  QSizePolicy sizePolicy29((QSizePolicy::Policy)1, (QSizePolicy::Policy)5);
  sizePolicy29.setHorizontalStretch(0);
  sizePolicy29.setVerticalStretch(0);
  sizePolicy29.setHeightForWidth(laVideoFileName->sizePolicy().hasHeightForWidth());
  laVideoFileName->setSizePolicy(sizePolicy29);
  gridLayout7->addWidget(laVideoFileName, 0, 1, 1, 1);

  tlVideoFileName = new QLabel(gbVideoFile);
  tlVideoFileName->setObjectName(QString::fromUtf8("tlVideoFileName"));
  gridLayout7->addWidget(tlVideoFileName, 0, 2, 1, 11);

  // VBV buffer size
  // --------------------------------------------------------------------------
  laVbvBuffer = new QLabel(gbVideoFile);
  laVbvBuffer->setObjectName(QString::fromUtf8("laVbvBuffer"));
  QSizePolicy sizePolicy17((QSizePolicy::Policy)1, (QSizePolicy::Policy)5);
  sizePolicy17.setHorizontalStretch(0);
  sizePolicy17.setVerticalStretch(0);
  sizePolicy17.setHeightForWidth(laVbvBuffer->sizePolicy().hasHeightForWidth());
  laVbvBuffer->setSizePolicy(sizePolicy17);  
  gridLayout7->addWidget(laVbvBuffer, 1, 11, 1, 1);
  
  tlVbvBuffer = new QLabel(gbVideoFile);
  tlVbvBuffer->setObjectName(QString::fromUtf8("tlVbvBuffer"));
  QSizePolicy sizePolicy20((QSizePolicy::Policy)1, (QSizePolicy::Policy)5);
  sizePolicy20.setHorizontalStretch(0);
  sizePolicy20.setVerticalStretch(0);
  sizePolicy20.setHeightForWidth(tlVbvBuffer->sizePolicy().hasHeightForWidth());
  tlVbvBuffer->setSizePolicy(sizePolicy20);  
  gridLayout7->addWidget(tlVbvBuffer, 1, 12, 1, 1);

  // bitrate
  // --------------------------------------------------------------------------
  laBitRate = new QLabel(gbVideoFile);
  laBitRate->setObjectName(QString::fromUtf8("laBitRate"));
  QSizePolicy sizePolicy18((QSizePolicy::Policy)1, (QSizePolicy::Policy)5);
  sizePolicy18.setHorizontalStretch(0);
  sizePolicy18.setVerticalStretch(0);
  sizePolicy18.setHeightForWidth(laBitRate->sizePolicy().hasHeightForWidth());
  laBitRate->setSizePolicy(sizePolicy18);  
  gridLayout7->addWidget(laBitRate, 1, 9, 1, 1);
  
  tlBitRate = new QLabel(gbVideoFile);
  tlBitRate->setObjectName(QString::fromUtf8("tlBitRate"));
  QSizePolicy sizePolicy21((QSizePolicy::Policy)1, (QSizePolicy::Policy)5);
  sizePolicy21.setHorizontalStretch(0);
  sizePolicy21.setVerticalStretch(0);
  sizePolicy21.setHeightForWidth(tlBitRate->sizePolicy().hasHeightForWidth());
  tlBitRate->setSizePolicy(sizePolicy21);  
  gridLayout7->addWidget(tlBitRate, 1, 10, 1, 1);

  // frame rate
  // --------------------------------------------------------------------------
  laFrameRate = new QLabel(gbVideoFile);
  laFrameRate->setObjectName(QString::fromUtf8("laFrameRate"));
  QSizePolicy sizePolicy19((QSizePolicy::Policy)1, (QSizePolicy::Policy)5);
  sizePolicy19.setHorizontalStretch(0);
  sizePolicy19.setVerticalStretch(0);
  sizePolicy19.setHeightForWidth(laFrameRate->sizePolicy().hasHeightForWidth());
  laFrameRate->setSizePolicy(sizePolicy19);
  gridLayout7->addWidget(laFrameRate, 1, 7, 1, 1);
  
  tlFrameRate = new QLabel(gbVideoFile);
  tlFrameRate->setObjectName(QString::fromUtf8("tlFrameRate"));
  QSizePolicy sizePolicy22((QSizePolicy::Policy)1, (QSizePolicy::Policy)5);
  sizePolicy22.setHorizontalStretch(0);
  sizePolicy22.setVerticalStretch(0);
  sizePolicy22.setHeightForWidth(tlFrameRate->sizePolicy().hasHeightForWidth());
  tlFrameRate->setSizePolicy(sizePolicy22);  
  gridLayout7->addWidget(tlFrameRate, 1, 8, 1, 1);
  
  // Aspect ratio
  // --------------------------------------------------------------------------
  laVideoAspect = new QLabel(gbVideoFile);
  laVideoAspect->setObjectName(QString::fromUtf8("laVideoAspect"));
  QSizePolicy sizePolicy26((QSizePolicy::Policy)1, (QSizePolicy::Policy)5);
  sizePolicy26.setHorizontalStretch(0);
  sizePolicy26.setVerticalStretch(0);
  sizePolicy26.setHeightForWidth(laVideoAspect->sizePolicy().hasHeightForWidth());
  laVideoAspect->setSizePolicy(sizePolicy26);
  gridLayout7->addWidget(laVideoAspect, 1, 5, 1, 1);

  tlVideoAspect = new QLabel(gbVideoFile);
  tlVideoAspect->setObjectName(QString::fromUtf8("tlVideoAspect"));
  QSizePolicy sizePolicy23((QSizePolicy::Policy)1, (QSizePolicy::Policy)5);
  sizePolicy23.setHorizontalStretch(0);
  sizePolicy23.setVerticalStretch(0);
  sizePolicy23.setHeightForWidth(tlVideoAspect->sizePolicy().hasHeightForWidth());
  tlVideoAspect->setSizePolicy(sizePolicy23);
  gridLayout7->addWidget(tlVideoAspect, 1, 6, 1, 1);
  
  // Resolution
  // --------------------------------------------------------------------------
  laResolution = new QLabel(gbVideoFile);
  laResolution->setObjectName(QString::fromUtf8("laResolution"));
  QSizePolicy sizePolicy27((QSizePolicy::Policy)1, (QSizePolicy::Policy)5);
  sizePolicy27.setHorizontalStretch(0);
  sizePolicy27.setVerticalStretch(0);
  sizePolicy27.setHeightForWidth(laResolution->sizePolicy().hasHeightForWidth());
  laResolution->setSizePolicy(sizePolicy27);
  gridLayout7->addWidget(laResolution, 1, 3, 1, 1);

  tlResolution = new QLabel(gbVideoFile);
  tlResolution->setObjectName(QString::fromUtf8("tlResolution"));
  QSizePolicy sizePolicy24((QSizePolicy::Policy)1, (QSizePolicy::Policy)5);
  sizePolicy24.setHorizontalStretch(0);
  sizePolicy24.setVerticalStretch(0);
  sizePolicy24.setHeightForWidth(tlResolution->sizePolicy().hasHeightForWidth());
  tlResolution->setSizePolicy(sizePolicy24);
  gridLayout7->addWidget(tlResolution, 1, 4, 1, 1);
  
  // Video length
  // --------------------------------------------------------------------------
  laVideoLen = new QLabel(gbVideoFile);
  laVideoLen->setObjectName(QString::fromUtf8("laVideoLen"));
  QSizePolicy sizePolicy28((QSizePolicy::Policy)1, (QSizePolicy::Policy)5);
  sizePolicy28.setHorizontalStretch(0);
  sizePolicy28.setVerticalStretch(0);
  sizePolicy28.setHeightForWidth(laVideoLen->sizePolicy().hasHeightForWidth());
  laVideoLen->setSizePolicy(sizePolicy28);
  gridLayout7->addWidget(laVideoLen, 1, 1, 1, 1);

  tlVideoLen = new QLabel(gbVideoFile);
  tlVideoLen->setObjectName(QString::fromUtf8("tlVideoLen"));
  QSizePolicy sizePolicy25((QSizePolicy::Policy)1, (QSizePolicy::Policy)5);
  sizePolicy25.setHorizontalStretch(0);
  sizePolicy25.setVerticalStretch(0);
  sizePolicy25.setHeightForWidth(tlVideoLen->sizePolicy().hasHeightForWidth());
  tlVideoLen->setSizePolicy(sizePolicy25);
  gridLayout7->addWidget(tlVideoLen, 1, 2, 1, 1);
  
    
  // push button "open video file'
  // --------------------------------------------------------------------------
  pbOpenVideoFile = new QPushButton(gbVideoFile);
  pbOpenVideoFile->setObjectName(QString::fromUtf8("pbOpenVideoFile"));
  pbOpenVideoFile->setPixmap( *(TTCut::imgFileOpen24) );
  pbOpenVideoFile->setMinimumSize(QSize(30, 30));
  pbOpenVideoFile->setMaximumSize(QSize(30, 30));
  gridLayout7->addWidget(pbOpenVideoFile, 0, 0, 2, 1);

  // add video file group box to main layout  
  gridLayout->addWidget(gbVideoFile, 0, 0, 1, 3);


  // --------------------------------------------------------------------------
  // audio files section
  // --------------------------------------------------------------------------
  // group box (frame)
  gbAudioFiles = new QGroupBox(centralWidget);
  gbAudioFiles->setObjectName(QString::fromUtf8("gbAudioFiles"));
  QSizePolicy sizePolicy14((QSizePolicy::Policy)5, (QSizePolicy::Policy)5);
  sizePolicy14.setHorizontalStretch(0);
  sizePolicy14.setVerticalStretch(1);
  sizePolicy14.setHeightForWidth(gbAudioFiles->sizePolicy().hasHeightForWidth());
  gbAudioFiles->setSizePolicy(sizePolicy14);
  gbAudioFiles->setMinimumSize(QSize(16, 120));
  gbAudioFiles->setMaximumSize(QSize(65500, 240));
  gridLayout6 = new QGridLayout(gbAudioFiles);
  gridLayout6->setSpacing(6);
  gridLayout6->setMargin(8);
  gridLayout6->setObjectName(QString::fromUtf8("gridLayout6"));

  lvAudioFiles = new TTAudioListView( gbAudioFiles, "lvAudioFiles" );
  //lvAudioFiles->setObjectName(QString::fromUtf8("lvAudioFiles"));
  QSizePolicy sizePolicy15((QSizePolicy::Policy)5, (QSizePolicy::Policy)5);
  sizePolicy15.setHorizontalStretch(0);
  sizePolicy15.setVerticalStretch(0);
  sizePolicy15.setHeightForWidth(lvAudioFiles->sizePolicy().hasHeightForWidth());
  lvAudioFiles->setSizePolicy(sizePolicy15);
  // --------------------------------------------------------------------------
  lvAudioFiles->addColumn( tr( "Filename" ) );
  lvAudioFiles->addColumn( trUtf8( "Length" ) );
  lvAudioFiles->addColumn( tr( "Version" ) );
  lvAudioFiles->addColumn( tr( "Bitrate" ) );
  lvAudioFiles->addColumn( tr( "Samplerate" ) );
  lvAudioFiles->addColumn( trUtf8( "Channel mode" ) );
  lvAudioFiles->addColumn( trUtf8( "Delay" ) );
  // --------------------------------------------------------------------------

  gridLayout6->addWidget(lvAudioFiles, 0, 1, 3, 1);

  pbOpenAudioFile = new QPushButton(gbAudioFiles);
  pbOpenAudioFile->setObjectName(QString::fromUtf8("pbOpenAudioFile"));
  pbOpenAudioFile->setPixmap( *(TTCut::imgFileOpen24) );
  pbOpenAudioFile->setMinimumSize(QSize(30, 30));
  pbOpenAudioFile->setMaximumSize(QSize(30, 30));
  
  gridLayout6->addWidget(pbOpenAudioFile, 0, 0, 1, 1);
  
  pbAudioFileUp = new QPushButton(gbAudioFiles);
  pbAudioFileUp->setObjectName(QString::fromUtf8("pbAudioFileUp"));
  pbAudioFileUp->setPixmap( *(TTCut::imgUpArrow) );
  pbAudioFileUp->setMinimumSize(QSize(30, 24));
  pbAudioFileUp->setMaximumSize(QSize(30, 24));
  
  gridLayout6->addWidget(pbAudioFileUp, 1, 0, 1, 1);
  
  pbAudioFileDown = new QPushButton(gbAudioFiles);
  pbAudioFileDown->setObjectName(QString::fromUtf8("pbAudioFileDown"));
  pbAudioFileDown->setPixmap( *(TTCut::imgDownArrow) );
  pbAudioFileDown->setMinimumSize(QSize(30, 24));
  pbAudioFileDown->setMaximumSize(QSize(30, 24));
  
  gridLayout6->addWidget(pbAudioFileDown, 2, 0, 1, 1);
  
  // add audio file section to main layout  
  gridLayout->addWidget(gbAudioFiles, 1, 0, 1, 3);
  
  // --------------------------------------------------------------------------
  // cut out frame preview
  // --------------------------------------------------------------------------
  gbCutOutFrame = new QGroupBox(centralWidget);
  gbCutOutFrame->setObjectName(QString::fromUtf8("gbCutOutFrame"));
  QSizePolicy sizePolicy1(QSizePolicy::Ignored, QSizePolicy::Ignored);
  sizePolicy1.setHorizontalStretch(6);
  sizePolicy1.setVerticalStretch(4);
  //sizePolicy1.setHeightForWidth(gbCutOutFrame->sizePolicy().hasHeightForWidth());
  gbCutOutFrame->setSizePolicy(sizePolicy1);
  gbCutOutFrame->setSizeIncrement(QSize(0, 4));
  gridLayout1 = new QGridLayout(gbCutOutFrame);
  gridLayout1->setSpacing(6);
  gridLayout1->setMargin(8);
  gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
  hboxLayout1 = new QHBoxLayout();
  hboxLayout1->setSpacing(6);
  hboxLayout1->setMargin(0);
  hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));

  // cut-out frame position
  // --------------------------------------------------------------------------
  laCutOutFramePosition = new QLabel(gbCutOutFrame);
  laCutOutFramePosition->setObjectName(QString::fromUtf8("laCutOutFramePosition"));

  hboxLayout1->addWidget(laCutOutFramePosition);

  // cut-out frame navigation previous
  pbPrevCutOutFrame = new QPushButton(gbCutOutFrame);
  pbPrevCutOutFrame->setObjectName(QString::fromUtf8("pbPrevCutOutFrame"));
  QSizePolicy sizePolicy2((QSizePolicy::Policy)1, (QSizePolicy::Policy)0);
  sizePolicy2.setHorizontalStretch(0);
  sizePolicy2.setVerticalStretch(1);
  //sizePolicy2.setHeightForWidth(pbPrevCutOutFrame->sizePolicy().hasHeightForWidth());
  pbPrevCutOutFrame->setSizePolicy(sizePolicy2);
  pbPrevCutOutFrame->setMinimumSize(QSize(24, 20));
  pbPrevCutOutFrame->setMaximumSize(QSize(30, 28));
  
  hboxLayout1->addWidget(pbPrevCutOutFrame);

  // cut-out frame navigation next
  pbNextCutOutFrame = new QPushButton(gbCutOutFrame);
  pbNextCutOutFrame->setObjectName(QString::fromUtf8("pbNextCutOutFrame"));
  QSizePolicy sizePolicy3((QSizePolicy::Policy)1, (QSizePolicy::Policy)0);
  sizePolicy3.setHorizontalStretch(0);
  sizePolicy3.setVerticalStretch(1);
  //sizePolicy3.setHeightForWidth(pbNextCutOutFrame->sizePolicy().hasHeightForWidth());
  pbNextCutOutFrame->setSizePolicy(sizePolicy3);
  pbNextCutOutFrame->setMinimumSize(QSize(24, 20));
  pbNextCutOutFrame->setMaximumSize(QSize(30, 28));
  
  hboxLayout1->addWidget(pbNextCutOutFrame);
  
  // cut-out frame search frame
  pbSearchFrame = new QPushButton(gbCutOutFrame);
  pbSearchFrame->setObjectName(QString::fromUtf8("pbSearchFrame"));
  pbSearchFrame->setIconSet( QIcon( *(TTCut::imgSearch) ) );
  QSizePolicy sizePolicy4((QSizePolicy::Policy)1, (QSizePolicy::Policy)0);
  sizePolicy4.setHorizontalStretch(0);
  sizePolicy4.setVerticalStretch(1);
  //sizePolicy4.setHeightForWidth(pbSearchFrame->sizePolicy().hasHeightForWidth());
  pbSearchFrame->setSizePolicy(sizePolicy4);
  pbSearchFrame->setMinimumSize(QSize(52, 20));
  pbSearchFrame->setMaximumSize(QSize(64, 28));
  
  hboxLayout1->addWidget(pbSearchFrame);
  
  gridLayout1->addLayout(hboxLayout1, 1, 0, 1, 1);

  // cut-out frame
  cutOutFrameWnd = new TTMPEG2Window( gbCutOutFrame, "cutOutFrameWnd" );
  //cutOutWnd->setObjectName(QString::fromUtf8("cutOutWnd"));
  cutOutFrameWnd->setMinimumSize(QSize(video_frame_width, video_frame_height));
  QSizePolicy sizePolicy44(QSizePolicy::Ignored, QSizePolicy::Ignored);
  sizePolicy44.setHorizontalStretch(0);
  sizePolicy44.setVerticalStretch(0);
  //sizePolicy44.setHeightForWidth(pbSearchFrame->sizePolicy().hasHeightForWidth());
  cutOutFrameWnd->setSizePolicy(sizePolicy44);

  gridLayout1->addWidget(cutOutFrameWnd, 0, 0, 1, 1);

  gridLayout->addWidget(gbCutOutFrame, 2, 0, 1, 1);


  // --------------------------------------------------------------------------
  // current frame preview (cut-in frame)
  // --------------------------------------------------------------------------
  gbCurrentFrame = new QGroupBox(centralWidget);
  gbCurrentFrame->setObjectName(QString::fromUtf8("gbCurrentFrame"));
  QSizePolicy sizePolicy5(QSizePolicy::Ignored, QSizePolicy::Ignored);
  sizePolicy5.setHorizontalStretch(6);
  sizePolicy5.setVerticalStretch(4);
  //sizePolicy5.setHeightForWidth(gbCurrentFrame->sizePolicy().hasHeightForWidth());
  gbCurrentFrame->setSizePolicy(sizePolicy5);
  gbCurrentFrame->setSizeIncrement(QSize(0, 0));
  gridLayout2 = new QGridLayout(gbCurrentFrame);
  gridLayout2->setSpacing(6);
  gridLayout2->setMargin(8);
  gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
  hboxLayout2 = new QHBoxLayout();
  hboxLayout2->setSpacing(6);
  hboxLayout2->setMargin(0);
  hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
  
  // play video button
  // --------------------------------------------------------------------------
  pbPlayVideo = new QPushButton(gbCurrentFrame);
  pbPlayVideo->setObjectName(QString::fromUtf8("pbPlayVideo"));
  pbPlayVideo->setPixmap( *(TTCut::imgPlay) );
  QSizePolicy sizePolicy6((QSizePolicy::Policy)1, (QSizePolicy::Policy)0);
  sizePolicy6.setHorizontalStretch(0);
  sizePolicy6.setVerticalStretch(1);
  //sizePolicy6.setHeightForWidth(pbPlayVideo->sizePolicy().hasHeightForWidth());
  pbPlayVideo->setSizePolicy(sizePolicy6);
  pbPlayVideo->setMinimumSize(QSize(24, 20));
  pbPlayVideo->setMaximumSize(QSize(30, 28));
  
  hboxLayout2->addWidget(pbPlayVideo);

  // current frame position
  // --------------------------------------------------------------------------
  tlCurrentPosition = new QLabel(gbCurrentFrame);
  tlCurrentPosition->setObjectName(QString::fromUtf8("tlCurrentPosition"));

  hboxLayout2->addWidget(tlCurrentPosition);

  gridLayout2->addLayout(hboxLayout2, 1, 0, 1, 1);

  currentFrameWnd = new TTMPEG2Window( gbCurrentFrame , "currentFrameWnd" );
  //currentFrameWnd->setObjectName(QString::fromUtf8("currentFrameWnd"));
  currentFrameWnd->setMinimumSize(QSize(video_frame_width, video_frame_height));
  QSizePolicy sizePolicy155(QSizePolicy::Ignored, QSizePolicy::Ignored);
  sizePolicy155.setHorizontalStretch(0);
  sizePolicy155.setVerticalStretch(0);
  //sizePolicy155.setHeightForWidth(pbSearchFrame->sizePolicy().hasHeightForWidth());
  currentFrameWnd->setSizePolicy(sizePolicy155);
  
  gridLayout2->addWidget(currentFrameWnd, 0, 0, 1, 1);

  gridLayout->addWidget(gbCurrentFrame, 2, 1, 1, 1);


  // --------------------------------------------------------------------------
  // navigation section
  // --------------------------------------------------------------------------
  gbNavigation = new QGroupBox(centralWidget);
  gbNavigation->setObjectName(QString::fromUtf8("gbNavigation"));
  QSizePolicy sizePolicy10(QSizePolicy::Minimum, QSizePolicy::Ignored);
  sizePolicy3.setHorizontalStretch(1);
  sizePolicy3.setVerticalStretch(10);
  //sizePolicy3.setHeightForWidth(gbNavigation->sizePolicy().hasHeightForWidth());
  gbNavigation->setSizePolicy(sizePolicy10);
  gbNavigation->setSizeIncrement(QSize(1, 0));
  gbNavigation->setMaximumSize(QSize(250,65500));
  gridLayout2 = new QGridLayout(gbNavigation);
  gridLayout2->setSpacing(6);
  gridLayout2->setMargin(8);
  gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
  gridLayout3 = new QGridLayout();
  gridLayout3->setSpacing(6);
  gridLayout3->setMargin(0);
  gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));

  pbSetCutOut = new QPushButton(gbNavigation);
  pbSetCutOut->setObjectName(QString::fromUtf8("pbSetCutOut"));
  pbSetCutOut->setMinimumSize(QSize(50, 20));
  pbSetCutOut->setMaximumSize(QSize(150, 28));
  
  gridLayout3->addWidget(pbSetCutOut, 1, 3, 1, 1);

  pbPrevIFrame = new QPushButton(gbNavigation);
  pbPrevIFrame->setObjectName(QString::fromUtf8("pbPrevIFrame"));
  QSizePolicy sizePolicy7((QSizePolicy::Policy)1, (QSizePolicy::Policy)5);
  sizePolicy7.setHorizontalStretch(0);
  sizePolicy7.setVerticalStretch(0);
  //sizePolicy7.setHeightForWidth(pbPrevIFrame->sizePolicy().hasHeightForWidth());
  pbPrevIFrame->setSizePolicy(sizePolicy7);
  pbPrevIFrame->setMinimumSize(QSize(24, 20));
  pbPrevIFrame->setMaximumSize(QSize(40, 28));
  gridLayout3->addWidget(pbPrevIFrame, 0, 0, 1, 1);

  pbNextIFrame = new QPushButton(gbNavigation);
  pbNextIFrame->setObjectName(QString::fromUtf8("pbNextIFrame"));
  QSizePolicy sizePolicy99((QSizePolicy::Policy)1, (QSizePolicy::Policy)5);
  sizePolicy4.setHorizontalStretch(0);
  sizePolicy4.setVerticalStretch(0);
  //sizePolicy4.setHeightForWidth(pbNextIFrame->sizePolicy().hasHeightForWidth());
  pbNextIFrame->setSizePolicy(sizePolicy99);
  pbNextIFrame->setMinimumSize(QSize(24, 20));
  pbNextIFrame->setMaximumSize(QSize(40, 28));  
  gridLayout3->addWidget(pbNextIFrame, 0, 1, 1, 1);

  pbSetCutIn = new QPushButton(gbNavigation);
  pbSetCutIn->setObjectName(QString::fromUtf8("pbSetCutIn"));
  pbSetCutIn->setMinimumSize(QSize(50, 20));
  pbSetCutIn->setMaximumSize(QSize(150, 28));
  gridLayout3->addWidget(pbSetCutIn, 0, 3, 1, 1);
  
  spacerItem4 = new QSpacerItem(191, 10, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
  gridLayout3->addItem(spacerItem4, 4, 0, 1, 4);

  spacerItem = new QSpacerItem(44, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
  gridLayout3->addItem(spacerItem, 1, 2, 1, 1);


  pbPrevPFrame = new QPushButton(gbNavigation);
  pbPrevPFrame->setObjectName(QString::fromUtf8("pbPrevPFrame"));
  QSizePolicy sizePolicy55((QSizePolicy::Policy)1, (QSizePolicy::Policy)5);
  sizePolicy5.setHorizontalStretch(0);
  sizePolicy5.setVerticalStretch(0);
  //sizePolicy5.setHeightForWidth(pbPrevPFrame->sizePolicy().hasHeightForWidth());
  pbPrevPFrame->setSizePolicy(sizePolicy55);
  pbPrevPFrame->setMinimumSize(QSize(24, 20));
  pbPrevPFrame->setMaximumSize(QSize(40, 28));
  gridLayout3->addWidget(pbPrevPFrame, 1, 0, 1, 1);

  pbNextPFrame = new QPushButton(gbNavigation);
  pbNextPFrame->setObjectName(QString::fromUtf8("pbNextPFrame"));
  QSizePolicy sizePolicy9((QSizePolicy::Policy)1, (QSizePolicy::Policy)5);
  sizePolicy9.setHorizontalStretch(0);
  sizePolicy9.setVerticalStretch(0);
  //sizePolicy9.setHeightForWidth(pbNextPFrame->sizePolicy().hasHeightForWidth());
  pbNextPFrame->setSizePolicy(sizePolicy9);
  pbNextPFrame->setMinimumSize(QSize(24, 20));
  pbNextPFrame->setMaximumSize(QSize(40, 28));
  gridLayout3->addWidget(pbNextPFrame, 1, 1, 1, 1);
  
  spacerItem2 = new QSpacerItem(41, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
  gridLayout3->addItem(spacerItem2, 2, 2, 1, 1);

  pbNextBFrame = new QPushButton(gbNavigation);
  pbNextBFrame->setObjectName(QString::fromUtf8("pbNextBFrame"));
  QSizePolicy sizePolicy66((QSizePolicy::Policy)1, (QSizePolicy::Policy)5);
  sizePolicy6.setHorizontalStretch(0);
  sizePolicy6.setVerticalStretch(0);
  //sizePolicy6.setHeightForWidth(pbNextBFrame->sizePolicy().hasHeightForWidth());
  pbNextBFrame->setSizePolicy(sizePolicy66);
  pbNextBFrame->setMinimumSize(QSize(24, 20));
  pbNextBFrame->setMaximumSize(QSize(40, 28));
  gridLayout3->addWidget(pbNextBFrame, 2, 1, 1, 1);
  
  spacerItem1 = new QSpacerItem(41, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
  gridLayout3->addItem(spacerItem1, 2, 3, 1, 1);
  
  pbPrevBFrame = new QPushButton(gbNavigation);
  pbPrevBFrame->setObjectName(QString::fromUtf8("pbPrevBFrame"));
  QSizePolicy sizePolicy8((QSizePolicy::Policy)1, (QSizePolicy::Policy)5);
  sizePolicy8.setHorizontalStretch(0);
  sizePolicy8.setVerticalStretch(0);
  //sizePolicy8.setHeightForWidth(pbPrevBFrame->sizePolicy().hasHeightForWidth());
  pbPrevBFrame->setSizePolicy(sizePolicy8);
  pbPrevBFrame->setMinimumSize(QSize(24, 20));
  pbPrevBFrame->setMaximumSize(QSize(40, 28));
  gridLayout3->addWidget(pbPrevBFrame, 2, 0, 1, 1);

  pbGotoCutIn = new QPushButton(gbNavigation);
  pbGotoCutIn->setObjectName(QString::fromUtf8("pbGotoCutIn"));
  pbGotoCutIn->setIconSet( QIconSet( *(TTCut::imgGoTo) ) );
  pbGotoCutIn->setMinimumSize(QSize(52, 20));
  pbGotoCutIn->setMaximumSize(QSize(100, 28));
  gridLayout3->addWidget(pbGotoCutIn, 5, 0, 1, 2);
  
  tlCutInPosition = new QLabel(gbNavigation);
  tlCutInPosition->setObjectName(QString::fromUtf8("tlCutInPosition"));
  gridLayout3->addWidget(tlCutInPosition, 5, 2, 1, 2);
  
  
  pbGotoCutOut = new QPushButton(gbNavigation);
  pbGotoCutOut->setObjectName(QString::fromUtf8("pbGotoCutOut"));
  pbGotoCutOut->setIconSet( QIconSet( *(TTCut::imgGoTo) ) );
  pbGotoCutOut->setMinimumSize(QSize(52, 20));
  pbGotoCutOut->setMaximumSize(QSize(100, 28));
  gridLayout3->addWidget(pbGotoCutOut, 6, 0, 1, 2);
  
  tlCutOutPosition = new QLabel(gbNavigation);
  tlCutOutPosition->setObjectName(QString::fromUtf8("tlCutOutPosition"));
  gridLayout3->addWidget(tlCutOutPosition, 6, 2, 1, 2);
  
  spacerItem3 = new QSpacerItem(44, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
  gridLayout3->addItem(spacerItem3, 0, 2, 1, 1);
  
  gridLayout2->addLayout(gridLayout3, 0, 0, 1, 1);
  
  spacerItem5 = new QSpacerItem(205, 4, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
  gridLayout2->addItem(spacerItem5, 1, 0, 1, 1);
  
  tlMarkerPosition = new QLabel(gbNavigation);
  tlMarkerPosition->setObjectName(QString::fromUtf8("tlMarkerPosition"));
  gridLayout2->addWidget(tlMarkerPosition, 3, 0, 1, 1);
  
  gridLayout4 = new QGridLayout();
  gridLayout4->setSpacing(4);
  gridLayout4->setMargin(0);
  gridLayout4->setObjectName(QString::fromUtf8("gridLayout4"));
  pbStreamPoints = new QPushButton(gbNavigation);
  pbStreamPoints->setObjectName(QString::fromUtf8("pbStreamPoints"));
  pbStreamPoints->setMinimumSize(QSize(52, 20));
  pbStreamPoints->setMaximumSize(QSize(300, 28));
  
  gridLayout4->addWidget(pbStreamPoints, 1, 1, 1, 1);
  
  pbSetMarker = new QPushButton(gbNavigation);
  pbSetMarker->setObjectName(QString::fromUtf8("pbSetMarker"));
  pbSetMarker->setIconSet( QIconSet( *(TTCut::imgMarker) ) );
  pbSetMarker->setMinimumSize(QSize(52, 20));
  pbSetMarker->setMaximumSize(QSize(300, 28));
  
  gridLayout4->addWidget(pbSetMarker, 2, 0, 1, 1);
  
  pbGotoMarker = new QPushButton(gbNavigation);
  pbGotoMarker->setObjectName(QString::fromUtf8("pbGotoMarker"));
  pbGotoMarker->setMinimumSize(QSize(52, 20));
  pbGotoMarker->setMaximumSize(QSize(300, 28));
  
  gridLayout4->addWidget(pbGotoMarker, 2, 1, 1, 1);
  
  pbAddCut = new QPushButton(gbNavigation);
  pbAddCut->setObjectName(QString::fromUtf8("pbAddCut"));
  pbAddCut->setIconSet( QIconSet( *(TTCut::imgAddToList) ) );
  pbAddCut->setMinimumSize(QSize(52, 20));
  pbAddCut->setMaximumSize(QSize(600, 600));
  
  gridLayout4->addWidget(pbAddCut, 0, 0, 1, 2);
  
  pbQuickJump = new QPushButton(gbNavigation);
  pbQuickJump->setObjectName(QString::fromUtf8("pbQuickJump"));
  pbQuickJump->setMinimumSize(QSize(52, 20));
  pbQuickJump->setMaximumSize(QSize(300, 28));
  
  gridLayout4->addWidget(pbQuickJump, 1, 0, 1, 1);
  
  
  gridLayout2->addLayout(gridLayout4, 2, 0, 1, 1);
  
  
  gridLayout->addWidget(gbNavigation, 2, 2, 1, 1);


  // --------------------------------------------------------------------------
  // slider
  // --------------------------------------------------------------------------
  sbVideoPosition = new QScrollBar(centralWidget);
  sbVideoPosition->setObjectName(QString::fromUtf8("sbVideoPosition"));
  QSizePolicy sizePolicy31((QSizePolicy::Policy)5, (QSizePolicy::Policy)0);
  sizePolicy31.setHorizontalStretch(4);
  sizePolicy31.setVerticalStretch(0);
  sizePolicy31.setHeightForWidth(sbVideoPosition->sizePolicy().hasHeightForWidth());
  sbVideoPosition->setSizePolicy(sizePolicy31);
  sbVideoPosition->setMinimumSize(QSize(600, 0));
  sbVideoPosition->setOrientation(Qt::Horizontal);

  sbVideoPosition->setTracking( true );
  
  gridLayout->addWidget(sbVideoPosition, 3, 0, 1, 3);

  // --------------------------------------------------------------------------
  // cut-list / chapter-list / result-list
  // --------------------------------------------------------------------------
  gridLayout8 = new QGridLayout();
  gridLayout8->setSpacing(6);
  gridLayout8->setMargin(0);
  gridLayout8->setObjectName(QString::fromUtf8("gridLayout8"));

  ccrTab = new TTCutCCRWnd (centralWidget, "ccrTab" );
  //ccrTab->setObjectName(QString::fromUtf8("ccrTab"));
  cutListView = ccrTab->getCutListView();

  QSizePolicy sizePolicy30(QSizePolicy::Expanding, QSizePolicy::Minimum);
  sizePolicy30.setHorizontalStretch(5);
  sizePolicy30.setVerticalStretch(1);
  //sizePolicy30.setHeightForWidth(ccrTab->sizePolicy().hasHeightForWidth());
  ccrTab->setSizePolicy(sizePolicy30);
  ccrTab->setMinimumSize(QSize(600,150));
  ccrTab->setMaximumSize(QSize(65500,150));

  ccrTab->setTabPosition(QTabWidget::North);
  ccrTab->setTabShape(QTabWidget::Rounded);

  gridLayout8->addWidget(ccrTab, 0, 0, 5, 1);

  spacerItem6 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

  gridLayout8->addItem(spacerItem6, 0, 1, 1, 1);

  pbChapter = new QPushButton(centralWidget);
  pbChapter->setObjectName(QString::fromUtf8("pbChapter"));
  pbChapter->setIconSet( QIcon( *(TTCut::imgChapter) ) );
  pbChapter->setMinimumSize(QSize(52, 24));
  pbChapter->setMaximumSize(QSize(300, 28));
  
    gridLayout8->addWidget(pbChapter, 1, 1, 1, 1);

  pbCutPreview = new QPushButton(centralWidget);
  pbCutPreview->setObjectName(QString::fromUtf8("pbCutPreview"));
  pbCutPreview->setIconSet( QIcon( *(TTCut::imgPreview) ) );
  pbCutPreview->setMinimumSize(QSize(52, 24));
  pbCutPreview->setMaximumSize(QSize(300, 28));
  
    gridLayout8->addWidget(pbCutPreview, 2, 1, 1, 1);

  pbVideoAudioCut = new QPushButton(centralWidget);
  pbVideoAudioCut->setObjectName(QString::fromUtf8("pbVideoAudioCut"));
  pbVideoAudioCut->setIconSet( QIcon( *(TTCut::imgCutAV) ) );
  pbVideoAudioCut->setMinimumSize(QSize(52, 24));
  pbVideoAudioCut->setMaximumSize(QSize(300, 28));

  gridLayout8->addWidget(pbVideoAudioCut, 3, 1, 1, 1);


  pbAudioCut = new QPushButton(centralWidget);
  pbAudioCut->setObjectName(QString::fromUtf8("pbAudioCut"));
  pbAudioCut->setIconSet( QIcon( *(TTCut::imgCutAudio) ) );
  pbAudioCut->setMinimumSize(QSize(52, 24));
  pbAudioCut->setMaximumSize(QSize(300, 28));
  
    gridLayout8->addWidget(pbAudioCut, 4, 1, 1, 1);

    gridLayout->addLayout(gridLayout8, 4, 0, 1, 3);


  disableNavigation();

  languageChange();

  setFocusPolicy( Qt::StrongFocus );

  // --------------------------------------------------------------------------
  // signals and slots connections
  // --------------------------------------------------------------------------
  // file-menu
  connect( fileNewAction,        SIGNAL( activated() ), this, SLOT( fileNew() ) );
  connect( fileOpenAction,       SIGNAL( activated() ), this, SLOT( fileOpen() ) );
  connect( fileSaveAction,       SIGNAL( activated() ), this, SLOT( fileSave() ) );
  connect( fileSaveAsAction,     SIGNAL( activated() ), this, SLOT( fileSaveAs() ) );
  connect( fileRecentAction,     SIGNAL( activated() ), this, SLOT( fileRecent() ) );
  connect( fileExitAction,       SIGNAL( activated() ), this, SLOT( fileExit() ) );
  // actions-menu
  connect( actionSaveAction,     SIGNAL( activated() ), this, SLOT( actionSave() ) );
  connect( actionSettingsAction, SIGNAL( activated() ), this, SLOT( actionSettings() ) );
  // help-menu
  connect( helpAboutAction,      SIGNAL( activated() ), this, SLOT( helpAbout() ) );
  // video stream section
  connect( pbOpenVideoFile,     SIGNAL( clicked() ), this, SLOT( openVideoFile() ) );
  // audio stream section
  connect( pbOpenAudioFile,  SIGNAL( clicked() ), this, SLOT( openAudioFile() ) );
  connect( pbAudioFileUp,    SIGNAL( clicked() ), this, SLOT( audioFileUp() ) );
  connect( pbAudioFileDown,  SIGNAL( clicked() ), this, SLOT( audioFileDown() ) );
  // cut out frame section
  connect( pbPrevCutOutFrame,  SIGNAL( clicked() ), this, SLOT( lastFramePrevious() ) );
  connect( pbNextCutOutFrame,  SIGNAL( clicked() ), this, SLOT( lastFrameNext() ) );
  connect( pbSearchFrame,    SIGNAL( clicked() ), this, SLOT( searchFrame() ) );
  // current frame section
  connect( pbPlayVideo, SIGNAL( clicked() ), this, SLOT( playVideo() ) );
  // video position slider
  connect( sbVideoPosition,  SIGNAL( valueChanged(int) ), this, SLOT( videoSliderChanged(int) ) );
  // navigation section
  connect( pbPrevIFrame,     SIGNAL( clicked() ), this, SLOT( gotoPrevIFrame() ) );
  connect( pbNextIFrame,     SIGNAL( clicked() ), this, SLOT( gotoNextIFrame() ) );
  connect( pbPrevPFrame,     SIGNAL( clicked() ), this, SLOT( gotoPrevPFrame() ) );
  connect( pbNextPFrame,     SIGNAL( clicked() ), this, SLOT( gotoNextPFrame() ) );
  connect( pbPrevBFrame,      SIGNAL( clicked() ), this, SLOT( gotoPrevFrame() ) );
  connect( pbNextBFrame,      SIGNAL( clicked() ), this, SLOT( gotoNextFrame() ) );
  connect( pbSetCutIn,       SIGNAL( clicked() ), this, SLOT( setCutInPoint() ) );
  connect( pbSetCutOut,      SIGNAL( clicked() ), this, SLOT( setCutOutPoint() ) );
  connect( pbGotoCutIn,      SIGNAL( clicked() ), this, SLOT( gotoCutInPosition() ) );
  connect( pbGotoCutOut,     SIGNAL( clicked() ), this, SLOT( gotoCutOutPosition() ) );
  connect( pbAddCut,   SIGNAL( clicked() ), this, SLOT( addRangeToCutList() ) );
  connect( pbQuickJump,     SIGNAL( clicked() ), this, SLOT( fastMotionOption() ) );
  connect( pbSetMarker,      SIGNAL( clicked() ), this, SLOT( setMarker() ) );
  connect( pbGotoMarker,     SIGNAL( clicked() ), this, SLOT( gotoMarker() ) );
  // cut section
  connect( pbCutPreview,     SIGNAL( clicked() ), this, SLOT( cutPreview() ) );
  connect( pbVideoAudioCut,  SIGNAL( clicked() ), this, SLOT( videoAudioCut() ) );
  connect( pbAudioCut,       SIGNAL( clicked() ), this, SLOT( audioCut() ) );
  // cut list
  connect( ccrTab->getCutTab()->pbCutUp,          SIGNAL( clicked() ), SLOT( moveCutEntryUp() ) );
  connect( ccrTab->getCutTab()->pbCutDelete,      SIGNAL( clicked() ), SLOT( deleteCutEntry() ) );
  connect( ccrTab->getCutTab()->pbCutDown,        SIGNAL( clicked() ), SLOT( moveCutEntryDown() ) );
  connect( cutListView, SIGNAL( editCutListItem(TTAVCutPosition&) ), SLOT( editCutListEntry(TTAVCutPosition&) ) );
  connect( cutListView, SIGNAL( selectCutOut(TTAVCutPosition&) ),    SLOT( selectCutOut(TTAVCutPosition&) ) );
  connect( cutListView, SIGNAL( showCutIn(TTAVCutPosition&) ),       SLOT( showSlaveCutIn(TTAVCutPosition&) ) );
  connect( cutListView, SIGNAL( showCutOut(TTAVCutPosition&) ),      SLOT( showSlaveCutOut(TTAVCutPosition&) ) );
  connect( cutListView, SIGNAL( previewSingleCut(uint) ),          SLOT( previewSingleCut(uint) ) );
  connect( this,        SIGNAL( cutOutIndexChanged( uint, QTime) ), cutListView,SLOT( cutOutIndexChanged( uint, QTime) ) );

  connect( cutListView, SIGNAL( newCutLen(uint, off64_t) ),  ccrTab->getResultTab(), SLOT( refreshCutVideoLength(uint, off64_t) ) );
}

// -----------------------------------------------------------------------------
// Destroys the object and frees any allocated resources
// -----------------------------------------------------------------------------
TTCutMainWnd::~TTCutMainWnd()
{
   if ( current_cut_pos )
      delete current_cut_pos;

   closeProject();
}


// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// Events
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////


// Sets the strings of the subwidgets using the current language.
// -----------------------------------------------------------------------------
void TTCutMainWnd::languageChange()
{
  // Caption for main window title
  // --------------------------------------------------------------------------
  setCaption( tr( "TTCut 0.10a" ) );

  // video stream section
  // --------------------------------------------------------------------------
  gbVideoFile->setTitle(QApplication::translate("TTCutMainWnd", "Video stream"));
  tlVideoFileName->setText(QApplication::translate("TTCutMainWnd", "---"));
  laVbvBuffer->setText(QApplication::translate("TTCutMainWnd", "vbvBuffer:"));
  laBitRate->setText(QApplication::translate("TTCutMainWnd", "Bitrate:"));
  laFrameRate->setText(QApplication::translate("TTCutMainWnd", "Framerate:"));
  tlVbvBuffer->setText(QApplication::translate("TTCutMainWnd", "---"));
  tlBitRate->setText(QApplication::translate("TTCutMainWnd", "---"));
  tlFrameRate->setText(QApplication::translate("TTCutMainWnd", "---"));
  tlVideoAspect->setText(QApplication::translate("TTCutMainWnd", "---"));
  tlResolution->setText(QApplication::translate("TTCutMainWnd", "---"));
  tlVideoLen->setText(QApplication::translate("TTCutMainWnd", "---"));
  laVideoAspect->setText(QApplication::translate("TTCutMainWnd", "Ratio:"));
  laResolution->setText(QApplication::translate("TTCutMainWnd", "Resolution:"));
  laVideoLen->setText(QApplication::translate("TTCutMainWnd", "Length:"));
  laVideoFileName->setText(QApplication::translate("TTCutMainWnd", "Filename:"));
  pbOpenVideoFile->setText( QString::null );


  // audio stream section
  // --------------------------------------------------------------------------
  gbAudioFiles->setTitle(QApplication::translate("TTCutMainWnd", "Audio streams"));
  pbOpenAudioFile->setText( QString::null );
  pbAudioFileUp->setText( QString::null );
  pbAudioFileDown->setText( QString::null );
  lvAudioFiles->header()->setLabel( 0, tr( "Filename" ) );
  lvAudioFiles->header()->setLabel( 1, tr( "Length" ) );
  lvAudioFiles->header()->setLabel( 2, tr( "Version" ) );
  lvAudioFiles->header()->setLabel( 3, tr( "Bitrate" ) );
  lvAudioFiles->header()->setLabel( 4, tr( "Samplerate" ) );
  lvAudioFiles->header()->setLabel( 5, tr( "Channel mode" ) );
  lvAudioFiles->header()->setLabel( 6, tr( "Delay" ) );

  // cut out frame section
  // --------------------------------------------------------------------------
  gbCutOutFrame->setTitle(QApplication::translate("TTCutMainWnd", "Cut out frame"));
  laCutOutFramePosition->setText(QApplication::translate("TTCutMainWnd", "00:00:00,000 (000000)"));
  pbPrevCutOutFrame->setText(QApplication::translate("TTCutMainWnd", "<"));
  pbNextCutOutFrame->setText(QApplication::translate("TTCutMainWnd", ">"));
  pbSearchFrame->setText(QApplication::translate("TTCutMainWnd", "Search"));

  // current frame section
  // --------------------------------------------------------------------------
  gbCurrentFrame->setTitle(QApplication::translate("TTCutMainWnd", "Current frame"));
  pbPlayVideo->setText( QString::null );
  tlCurrentPosition->setText(QApplication::translate("TTCutMainWnd", "00:00:00,000 (000000)"));

  // navigation section
  // --------------------------------------------------------------------------
  gbNavigation->setTitle(QApplication::translate("TTCutMainWnd", "Navigation"));
  tlCutInPosition->setText(QApplication::translate("TTCutMainWnd", "..."));
  pbGotoCutOut->setText(QApplication::translate("TTCutMainWnd", "Cut-out"));
  pbNextPFrame->setText(QApplication::translate("TTCutMainWnd", "P>"));
  pbGotoCutIn->setText(QApplication::translate("TTCutMainWnd", "Cut-in"));
  pbSetCutIn->setText(QApplication::translate("TTCutMainWnd", "Set cut-in"));
  pbSetCutOut->setText(QApplication::translate("TTCutMainWnd", "Set cut-out"));
  pbPrevIFrame->setText(QApplication::translate("TTCutMainWnd", "<I"));
  pbPrevBFrame->setText(QApplication::translate("TTCutMainWnd", "<B"));
  pbPrevPFrame->setText(QApplication::translate("TTCutMainWnd", "<P"));
  pbNextBFrame->setText(QApplication::translate("TTCutMainWnd", "B>"));
  tlCutOutPosition->setText(QApplication::translate("TTCutMainWnd", "..."));
  pbNextIFrame->setText(QApplication::translate("TTCutMainWnd", "I>"));
  pbStreamPoints->setText(QApplication::translate("TTCutMainWnd", "Stream points ..."));
  pbSetMarker->setText(QApplication::translate("TTCutMainWnd", "Set marker"));
  pbGotoMarker->setText(QApplication::translate("TTCutMainWnd", "Go to marker"));
  pbAddCut->setText(QApplication::translate("TTCutMainWnd", "Add range to cut list"));
  pbQuickJump->setText(QApplication::translate("TTCutMainWnd", "Quick jump ..."));
  tlMarkerPosition->setText(QApplication::translate("TTCutMainWnd", "--:--:--,---"));
  
  // cut button section
  // --------------------------------------------------------------------------
  pbChapter->setText(QApplication::translate("TTCutMainWnd", "Chapter"));
  pbCutPreview->setText(QApplication::translate("TTCutMainWnd", "Preview"));
  pbVideoAudioCut->setText(QApplication::translate("TTCutMainWnd", "Cut video/audio"));
  pbAudioCut->setText(QApplication::translate("TTCutMainWnd", "Cut audio only"));
  
  // application menu bar
  // --------------------------------------------------------------------------
  fileNewAction->setText( tr( "New" ) );
  fileNewAction->setMenuText( tr( "&New" ) );
  fileNewAction->setAccel( tr( "Ctrl+N" ) );
  fileOpenAction->setText( tr( "Open" ) );
  fileOpenAction->setMenuText( tr( "&Open..." ) );
  fileOpenAction->setAccel( tr( "Ctrl+O" ) );
  fileSaveAction->setText( tr( "Save" ) );
  fileSaveAction->setMenuText( tr( "&Save" ) );
  fileSaveAction->setAccel( tr( "Ctrl+S" ) );
  fileSaveAsAction->setText( tr( "Save as ..." ) );
  fileSaveAsAction->setMenuText( tr( "Save as ..." ) );
  //QT4: fileSaveAsAction->setAccel( QString::null );
  fileRecentAction->setText( tr( "Recent projects ..." ) );
  fileRecentAction->setMenuText( tr( "Recent projects ..." ) );
  //QT4: fileRecentAction->setAccel( QString::null );
  fileExitAction->setText( tr( "E%xit" ) );
  fileExitAction->setMenuText( tr( "E&xit" ) );
  fileExitAction->setAccel( tr("Ctrl+X") );

  actionSaveAction->setText( tr( "Save current frame" ) );
  actionSaveAction->setMenuText( tr( "Save current frame" ) );
  //QT4: actionSaveAction->setAccel( QString::null );
  actionSettingsAction->setText( tr( "Settings ..." ) );
  actionSettingsAction->setMenuText( tr( "Settings ..." ) );
  //QT4: actionSettingsAction->setAccel( QString::null );

  helpAboutAction->setText( tr( "About" ) );
  helpAboutAction->setMenuText( tr( "&About" ) );
  //QT4: helpAboutAction->setAccel( QString::null );
}


// Resize event
// -----------------------------------------------------------------------------
void TTCutMainWnd::resizeEvent (__attribute__ ((unused))QResizeEvent* re)
{
   // resize the mpeg2 windows
  //cutOutFrameWnd->setGeometry(0,0,lastVideoFrame->width(),lastVideoFrame->height());
  //currentFrameWnd->setGeometry(0,0,currentVideoFrame->width(),currentVideoFrame->height());
}

// Main window get the focus: grab the keybaord
// -----------------------------------------------------------------------------
void TTCutMainWnd::focusInEvent( __attribute__ ((unused))QFocusEvent* e )
{
  //qDebug( "%sfocus in event: grab keyboard",c_name );

  //grabKeyboard();
}

// Main window lost focus: release keyboard
// -----------------------------------------------------------------------------
void TTCutMainWnd::focusOutEvent( __attribute__ ((unused))QFocusEvent* e )
{
  //qDebug( "%sfocus out event: release keyboard",c_name );

  //releaseKeyboard();
}

// Key pres event; keyboard navigation
// -----------------------------------------------------------------------------
void TTCutMainWnd::keyPressEvent ( QKeyEvent * e )
{
  uint position;

  switch ( e->key() )
  {
  // ---------------------------------------------------------------------------
  // left arrow key
  // ---------------------------------------------------------------------------
  case Qt::Key_Left:
    // backward TTCut::stepPlusAlt
    if ( e->state() == Qt::AltButton )
      position = current_frame_pos-TTCut::stepPlusAlt;

    // backward TTCut::stepPlusCtrl
    else if( e->state() == Qt::ControlButton )
      position = current_frame_pos-TTCut::stepPlusCtrl;

    // backward TTCut::stepPlusShift
    else if( e->state() == Qt::ShiftButton )
      position = current_frame_pos-TTCut::stepPlusShift;

    // backward one frame
    else
      position = current_frame_pos-1;

    current_frame_pos = moveToMpeg2Index( position );

    currentFrameWnd->showFrameAt( current_frame_pos );
    break;
  // ---------------------------------------------------------------------------
  // right arrow key
  // ---------------------------------------------------------------------------
  case Qt::Key_Right:
    // forward TTCut::stepPlusAlt
    if ( e->state() == Qt::AltButton )
      position = current_frame_pos+TTCut::stepPlusAlt;

    // forward TTCut::stepPlusCtrl
    else if( e->state() == Qt::ControlButton )
      position = current_frame_pos+TTCut::stepPlusCtrl;

    // forward TTCut::stepPlusShift
    else if( e->state() == Qt::ShiftButton )
      position = current_frame_pos+TTCut::stepPlusShift;

    // forward one frame
    else
      position = current_frame_pos+1;

    current_frame_pos = moveToMpeg2Index( position );

    currentFrameWnd->showFrameAt( current_frame_pos );
    break;
  // ---------------------------------------------------------------------------
  // home key: show first frame
  // ---------------------------------------------------------------------------
  case Qt::Key_Home:
    position          = (uint)0;
    current_frame_pos = moveToMpeg2Index( position );

    currentFrameWnd->showFrameAt( current_frame_pos );
    break;
  // ---------------------------------------------------------------------------
  // end key: show last frame
  // ---------------------------------------------------------------------------
  case Qt::Key_End:
    position          = mpeg2_stream->frameCount()-1;
    current_frame_pos = moveToMpeg2Index( position );

    currentFrameWnd->showFrameAt( current_frame_pos );
    break;
  // ---------------------------------------------------------------------------
  // page down
  // ---------------------------------------------------------------------------
  case Qt::Key_PageUp:
    position          = current_frame_pos-TTCut::stepPgUpDown;
    current_frame_pos = moveToMpeg2Index( position );

    currentFrameWnd->showFrameAt( current_frame_pos );
    break;
  // ---------------------------------------------------------------------------
  // page up
  // ---------------------------------------------------------------------------
  case Qt::Key_PageDown:
    position          = current_frame_pos+TTCut::stepPgUpDown;
    current_frame_pos = moveToMpeg2Index( position );

    currentFrameWnd->showFrameAt( current_frame_pos );
    break;
  // ---------------------------------------------------------------------------
  // I-frame
  // ---------------------------------------------------------------------------
  case Qt::Key_I:
    // previous I-Frame
    if ( e->state() == Qt::ControlButton )
      gotoPrevIFrame();
    // next I-frame
    else
      gotoNextIFrame();
    break;
  // ---------------------------------------------------------------------------
  // P-frame
  // ---------------------------------------------------------------------------
  case Qt::Key_P:
    // previous P-Frame
    if ( e->state() == Qt::ControlButton )
      gotoPrevPFrame();
    // next P-frame
    else
      gotoNextPFrame();
    break;
  // ---------------------------------------------------------------------------
  // B-frame
  // ---------------------------------------------------------------------------
  case Qt::Key_B:
    // previous B-Frame
    if ( e->state() == Qt::ControlButton )
      gotoPrevFrame();
    // next B-frame
    else
      gotoNextFrame();
    break;
  // ---------------------------------------------------------------------------
  // space
  // ---------------------------------------------------------------------------
  case Qt::Key_Space:
    break;
  // ---------------------------------------------------------------------------
  // return
  // ---------------------------------------------------------------------------
  case Qt::Key_Return:
    break;
  // ---------------------------------------------------------------------------
  // move to next possible cut-out frame
  // ---------------------------------------------------------------------------
  case Qt::Key_Plus:
    lastFrameNext();
    break;
  // ---------------------------------------------------------------------------
  // move to previous possible cut-out frame
  // ---------------------------------------------------------------------------
  case Qt::Key_Minus:
    lastFramePrevious();
    break;
  // ---------------------------------------------------------------------------
  // Slider positioned only at I-frames
  // ---------------------------------------------------------------------------
  case Qt::Key_F12:
    // toggle fastSlider
    if ( TTCut::fastSlider )
      TTCut::fastSlider = false;
    else
      TTCut::fastSlider = true;
    break;

    //default:
    //e->ignore();
    //break;
  }
}


// mouse-wheel event
// -----------------------------------------------------------------------------
void TTCutMainWnd::wheelEvent( QWheelEvent* e )
{
  int iPosition;
  int iDeltaMod;

  // no video stream; return
  if ( !TTCut::isVideoOpen )
    return;

  iDeltaMod = TTCut::stepMouseWheel;

  //qDebug( "Wheel delta: %d - steps: %d",e->delta(),iDeltaMod );

  if ( e->state() == Qt::ControlButton )
    iDeltaMod += TTCut::stepPlusCtrl;

  // wheel was rotated forwards away from the user
  if ( e->delta() > 0 )
    iPosition = current_frame_pos - iDeltaMod;
  else
    iPosition = current_frame_pos + iDeltaMod;

  if ( iPosition < 0 )
    iPosition = 0;

  if( iPosition >= (int)mpeg2_stream->frameCount() )
    iPosition = mpeg2_stream->frameCount()-1;

  sliderUpdateFrame = true;
  sbVideoPosition->setValue( iPosition );
}

// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// Menu slots
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// New project
// -----------------------------------------------------------------------------
void TTCutMainWnd::fileNew()
{
  // TODO: ask for saving changes
  if ( TTCut::isVideoOpen )
  {
    closeProject();
  }
}

// Open existing project
// -----------------------------------------------------------------------------
void TTCutMainWnd::fileOpen()
{
  int cut_start_pos;
  int cut_end_pos;
  TTAudioType* new_audio_type;
  TTAudioStream* new_audio_stream;
  TTCutProject* project_file;

  // show file selection dialog and get project file name
  QString fn = QFileDialog::getOpenFileName( this,
					     tr("Open project-file"),
					     TTCut::lastDirPath,
					     "Projekt(*.prj)" );
  if ( !fn.isEmpty() )
  {
    try
    {
      project_file = new TTCutProject( fn, QIODevice::ReadOnly );
    }
    catch ( TTCutProjectOpenException )
    {
      qDebug( "%serror open project file (!)",c_name );
      return;
    }

    QString str_file_name;

    project_file->seekToVideoSection();
    if ( project_file->readVideoFileName( str_file_name ) )
    {
      // set current project file name
      project_file_name = fn;

      // video filename and fileinfo
      video_file_name = str_file_name;
      QFileInfo f_info( video_file_name );
      TTCut::lastDirPath = f_info.absolutePath();

      if ( TTCut::isVideoOpen )
      {
	closeProject();
      }

      // get the stream type and create according stream-object
      video_type   = new TTVideoType( video_file_name );

      // create the mpeg2 stream object
      if ( video_type->avStreamType() == TTAVTypes::mpeg2_demuxed_video )
      {
	mpeg2_stream = (TTMpeg2VideoStream*)video_type->createVideoStream();
    
	delete video_type;
      }
      // wrong video type
      else
      {
	qDebug( "%swrong video type",c_name );
	delete video_type;
	return;
      }
 
      // set progress bar
      progress_bar = new TTProgressBar( this, "PBAR" );
      mpeg2_stream->setProgressBar( progress_bar );
      progress_bar->show();
      qApp->processEvents();

      // create header- and index-list for mpeg2 stream
      num_header = mpeg2_stream->createHeaderList();

      // user abort during header list creation or error creating list
      if ( num_header > (uint)0 )
	num_index  = mpeg2_stream->createIndexList();
      else
	num_index = 0;
      
      // index successfully created
      if ( num_index > (uint)0 )
      {
	TTCut::isVideoOpen = true;

	// sort frame index list to display (decoder) order
	mpeg2_stream->indexList()->sortDisplayOrder();
	
	// show information about current video stream
	showVideoStreamInfo();
      }
      else
      {
	// error creating video index; abort
	qDebug( "%sno index list created; abort (!)",c_name );
	
	mpeg2_stream->setProgressBar( (TTProgressBar*)NULL );
	delete progress_bar;
	
	video_type   = (TTVideoType*)NULL;
	mpeg2_stream = (TTMpeg2VideoStream*)NULL;
	
	closeProject();
	
	return;
      }

      // delete the progress bar
      mpeg2_stream->setProgressBar( (TTProgressBar*)NULL );
      delete progress_bar;

      // show the first video frame in cut-in window
      if ( TTCut::isVideoOpen )
      {
	// open the video stream an set header and index list
	sliderUpdateFrame = false;
	currentFramePos   = 0;
	
	ccrTab->getResultTab()->setVideoStream( mpeg2_stream );
	
	// initialize the the mpeg2 window for the cut-in frame
	//qDebug( "%sopen video stream",c_name );
	currentFrameWnd->openVideoStream( mpeg2_stream );
	currentFrameWnd->moveToFirstFrame( );
	
	// initialize the mpeg2 window for the cut-out frame
	//qDebug( "%sinitialize cut-out stream and window",c_name );
	cut_out_stream = new TTMpeg2VideoStream( QFileInfo(video_file_name));
	cut_out_stream->makeSharedCopy( mpeg2_stream );
	cutOutFrameWnd->openVideoStream( cut_out_stream );
	cutOutFrameWnd->moveToFirstFrame( false );
	
	// set the slider range
	sbVideoPosition->setMinValue( 0 );
	sbVideoPosition->setMaxValue( mpeg2_stream->frameCount()-1 );
	
	// initialize current frame position
	mpeg2_stream->setCurrentIndex( (uint)0 );
	current_frame_pos = 0;
	sbVideoPosition->setValue( current_frame_pos );
	
	refreshVideoLength();
	refreshCurrentPosition();
	
	enableNavigation();
      }
      
    }
    else
    {
      qDebug( "%sno video file found in project file (!)",c_name );
      return;
    }

    project_file->seekToAudioSection();
    while ( project_file->readAudioFileName( str_file_name ) )
    { 
      QFileInfo f_info( str_file_name );
      TTCut::lastDirPath = f_info.absolutePath();

      // get the strem type and create according stream-object
      new_audio_type = new TTAudioType( str_file_name );

      // create the audio stream object for the first audio file
      if ( new_audio_type->avStreamType() == TTAVTypes::mpeg_audio  ||
	   new_audio_type->avStreamType() == TTAVTypes::ac3_audio   ||
	   new_audio_type->avStreamType() == TTAVTypes::dts14_audio ||
	   new_audio_type->avStreamType() == TTAVTypes::dts16_audio ||
	   new_audio_type->avStreamType() == TTAVTypes::pcm_audio      )
      {
	new_audio_stream = (TTAudioStream*)new_audio_type->createAudioStream();
	
	// set progress bar
	progress_bar = new TTProgressBar( this, "PBAR" );
	new_audio_stream->setProgressBar( progress_bar );
	progress_bar->show();
	qApp->processEvents();
	
	// create header list for audio stream
	num_header = new_audio_stream->createHeaderList();
	
	// error reading audio stream or user abort during operation
	if ( num_header == 0 && new_audio_type->avStreamType() != TTAVTypes::pcm_audio ||
	     num_header == 1 && new_audio_type->avStreamType() == TTAVTypes::pcm_audio    )
	{
	  qDebug( "%serror reading audio stream; no header list (!)",c_name );
	  
	  new_audio_stream->setProgressBar( (TTProgressBar*)NULL );
	  delete progress_bar;
	  delete new_audio_type;
	  
	  new_audio_stream = (TTAudioStream*)NULL;
	  new_audio_type   = (TTAudioType*)NULL;
	}
	else
	{
	  // remove progress bar
	  new_audio_stream->setProgressBar( (TTProgressBar*)NULL );
	  delete progress_bar;
	  
	  // audio stream succesfully parsed; add item to list
	  lvAudioFiles->addItem( str_file_name, new_audio_stream );
	  
	  // first audio track loaded
	  TTCut::numAudioTracks += 1;
	  
	  // delete the av type
	  delete new_audio_type;
	}
      }
      // wrong audio type
      else
      {
	qDebug( "%swrong audio type for file: %s",c_name,audio_file_name.toAscii().data() );
	
	delete new_audio_type;
	
	new_audio_stream = (TTAudioStream*)NULL;
	new_audio_type   = (TTAudioType*)NULL;
      }
    }
    
    project_file->seekToCutSection();
    while ( project_file->readCutEntry( cut_start_pos, cut_end_pos ) )
    {
      mpeg2_stream->setCurrentIndex( cut_start_pos );
      current_cut_pos->setCutIn( cut_start_pos );
      current_cut_pos->setCutInOffset( mpeg2_stream->currentFrameOffset() );

      mpeg2_stream->setCurrentIndex( cut_end_pos );
      current_cut_pos->setCutOut( cut_end_pos );
      current_cut_pos->setCutOutOffset( mpeg2_stream->currentFrameOffset() );

      cutListView->addItem( mpeg2_stream->fileName(),
			    current_cut_pos->cutIn(),
			    current_cut_pos->cutInOffset(),
			    mpeg2_stream->frameTime(current_cut_pos->cutIn()),
			    current_cut_pos->cutOut(),
			    current_cut_pos->cutOutOffset(),
			    mpeg2_stream->frameTime(current_cut_pos->cutOut()) );

      mpeg2_stream->setCurrentIndex( 0 );
    }

    delete project_file;
  }
}

// Save current project
// -----------------------------------------------------------------------------
void TTCutMainWnd::fileSave()
{
  TTCutProject* project_file;
  QString       prj_file_name;

  // no video file open
  if ( !TTCut::isVideoOpen )
    return;

  if ( project_file_name.isEmpty() )
  {
    prj_file_name = ttChangeFileExt( video_file_name, "prj" );

    QFileInfo prj_file_info( QDir(TTCut::lastDirPath), prj_file_name );

    project_file_name = QFileDialog::getSaveFileName( this,
						      tr("Save project-file"),
						      prj_file_info.absoluteFilePath(),
						      "Projekt(*.prj)" );

    if ( project_file_name.isEmpty() )
      return;
  }

  try
  {
    project_file = new TTCutProject( project_file_name, QIODevice::WriteOnly );
  }
  catch( TTCutProjectOpenException )
  {
    qDebug( "%serror open project file (!)",c_name );
    return;
  }

  project_file->clearFile();

  QFileInfo f_info( video_file_name );

  // write video file section
  project_file->writeVideoSection( true );
  project_file->writeVideoFileName( (QString)f_info.absoluteFilePath() );
  project_file->writeVideoSection( false );

  // write audio file section
  lvAudioFiles->writeListToProject( project_file );

  // write cut section
  cutListView->writeListToProject( project_file );
  
  // close the project file
  delete project_file;
}

// Save current project as
// -----------------------------------------------------------------------------
void TTCutMainWnd::fileSaveAs()
{
  QString prj_file_name;

  // no video file open
  if ( !TTCut::isVideoOpen )
    return;

  prj_file_name = ttChangeFileExt( video_file_name, "prj" );

  QFileInfo prj_file_info( QDir(TTCut::lastDirPath), prj_file_name );

  project_file_name = QFileDialog::getSaveFileName( this,
						    tr("Save project-file as"),
						    prj_file_info.absoluteFilePath(),
						    "Projekt(*.prj)" );

  if ( project_file_name.isEmpty() )
    return;
  else
    fileSave();
}

// Open list with recently opened projects
// -----------------------------------------------------------------------------
void TTCutMainWnd::fileRecent()
{
  // TODO: add recent project file list
}

// Exit application
// -----------------------------------------------------------------------------
void TTCutMainWnd::fileExit()
{
  // TODO: if project was changed, ask for save changes

  if ( ttAssigned(settings) )
    {
      settings->writeSettings();
      delete settings;
    }
    
  qApp->quit();
}

// Save current frame to file
// -----------------------------------------------------------------------------
void TTCutMainWnd::actionSave()
{
  QString      szTemp;
  QString      extension;
  QString      format;
  QString      fileName;
  QFileDialog* fileDlg;

  // no video file open
  if ( !TTCut::isVideoOpen )
    return;

  // get the image file name
  fileDlg = new QFileDialog( this,
			     "save current frame",
			     TTCut::lastDirPath,
			     "Portable Network Graphics (*.png);;JPEG (*.jpg);;Bitmap (*.bmp)" );
  
  // enable specifying a file that doesn't exist
  fileDlg->setMode( QFileDialog::AnyFile );
  fileDlg->setAcceptMode( QFileDialog::AcceptSave );

  // input filename specified
  if ( fileDlg->exec() == QDialog::Accepted )
  {
     szTemp   = fileDlg->selectedFilter();
     fileName = fileDlg->selectedFile();

     if ( szTemp == "Portable Network Graphics (*.png)" )
     {
       format    = "PNG";
       extension = "png";
     }
     else if ( szTemp == "JPEG (*.jpg)" )
     {
       format    = "JPG";
       extension = "jpg";
     }
     else if ( szTemp == "Bitmap (*.bmp)" )
     {
       format    = "BMP";
       extension = "bmp";
     }
     else
     {
       qDebug( "unsupported format" );
       return;
     }

     fileName = ttChangeFileExt( fileName, extension );
     
     //qDebug( "selected filter   : %s",szTemp.ascii() );
     //qDebug( "selected file name: %s",fileName.ascii() );

     currentFrameWnd->saveCurrentFrame( fileName, format );
  }
  delete fileDlg;
}

// Application settings
// -----------------------------------------------------------------------------
void TTCutMainWnd::actionSettings()
{
  settingsDlg = new TTCutSettingsDlg( this, "SettingsDlg" );

  settingsDlg->exec();
}

// About application
// -----------------------------------------------------------------------------
void TTCutMainWnd::helpAbout()
{
  TTCutAboutDlg* aboutDlg = new TTCutAboutDlg( this );

  aboutDlg->exec();
}

// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// Video section slots
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// Open video stream
// -----------------------------------------------------------------------------
void TTCutMainWnd::openVideoFile()
{
  QString szTemp;
  //double fBitRate;

  // get the video filename
  QString fn = QFileDialog::getOpenFileName( this,
					     "Open video file",
					     TTCut::lastDirPath,
					     "Video (*.m2v *.mpg)" );

  // input filename specified
  if ( !fn.isEmpty() )
  {

    // if already a video open, close current project and reset settings
    if ( TTCut::isVideoOpen )
    {
      closeProject();
    }

    // video filename and fileinfo
    video_file_name = fn;

    QFileInfo f_info( video_file_name );
    TTCut::lastDirPath = f_info.absolutePath();
    
    // check for corresponding first audiofile
    // TODO: check for further extension like *.ac3, *.mp2 etc.	
    audio_file_name = ttChangeFileExt( fn, "mpa" );

    // analyze and load video audio streams
    createAVStreams( );
  }
}

// open video and audio stream
// create according index and header lists
// -----------------------------------------------------------------------------
void TTCutMainWnd::createAVStreams( )
{
  TTCut::isVideoOpen    = false;
  TTCut::numAudioTracks = 0;
  TTAudioStream* current_audio_stream;

  //qDebug( "%screateAVStreams: %s",c_name,video_file_name.ascii());
  //qDebug( "%screateAVStreams: %s",c_name,audio_file_name.ascii());

  // ---------------------------------------------------------------------------
  // open the video file and gather information about current video stream
  // ---------------------------------------------------------------------------
  if ( !video_file_name.isEmpty() )
  {
    // get the stream type and create according stream-object
    video_type   = new TTVideoType( video_file_name );

    // create the mpeg2 stream object
    if ( video_type->avStreamType() == TTAVTypes::mpeg2_demuxed_video )
    {
      mpeg2_stream = (TTMpeg2VideoStream*)video_type->createVideoStream();
    
      delete video_type;
    }
    // wrong video type
    else
    {
      qDebug( "%swrong video type",c_name );
      delete video_type;
      return;
    }
 
    // set progress bar
    progress_bar = new TTProgressBar( this, "PBAR" );
    mpeg2_stream->setProgressBar( progress_bar );
    progress_bar->show();
    qApp->processEvents();

    // create header- and index-list for mpeg2 stream
    num_header = mpeg2_stream->createHeaderList();

    // user abort during header list creation or error creating list
    if ( num_header > (uint)0 )
      num_index  = mpeg2_stream->createIndexList();
    else
      num_index = 0;

    // index successfully created
    if ( num_index > (uint)0 )
    {
      TTCut::isVideoOpen = true;

      // sort frame index list to display (decoder) order
      mpeg2_stream->indexList()->sortDisplayOrder();

      // show information about current video stream
      showVideoStreamInfo();
    }
    else
    {
      // error creating video index; abort
      qDebug( "%sno index list created; abort (!)",c_name );

      mpeg2_stream->setProgressBar( (TTProgressBar*)NULL );
      delete progress_bar;

      video_type   = (TTVideoType*)NULL;
      mpeg2_stream = (TTMpeg2VideoStream*)NULL;

      closeProject();

      return;
    }

    // delete the progress bar
    mpeg2_stream->setProgressBar( (TTProgressBar*)NULL );
    delete progress_bar;
  }
  else
  {
    // no video file name given; abort
    qDebug( "%sno video file name (!)",c_name );

    return;
  }

  // ---------------------------------------------------------------------------
  // open the audio file and gather information about current audio stream
  // ---------------------------------------------------------------------------
  if ( !audio_file_name.isEmpty() )
  {
    // get the strem type and create according stream-object
    audio_type   = new TTAudioType( audio_file_name );

    // create the audio stream object for the first audio file
    if ( audio_type->avStreamType() == TTAVTypes::mpeg_audio  ||
	 audio_type->avStreamType() == TTAVTypes::ac3_audio   ||
	 audio_type->avStreamType() == TTAVTypes::dts14_audio ||
	 audio_type->avStreamType() == TTAVTypes::dts16_audio ||
	 audio_type->avStreamType() == TTAVTypes::pcm_audio      )
    {
      current_audio_stream = (TTAudioStream*)audio_type->createAudioStream();

      delete audio_type;

      // set progress bar
      progress_bar = new TTProgressBar( this, "PBAR" );
      current_audio_stream->setProgressBar( progress_bar );
      progress_bar->show();
      qApp->processEvents();

      // create header list for audio stream
      num_header = current_audio_stream->createHeaderList();

      // error reading audio stream or user abort during operation
      if ( num_header == 0 && audio_type->avStreamType() != TTAVTypes::pcm_audio ||
           num_header == 1 && audio_type->avStreamType() == TTAVTypes::pcm_audio    )
      {
	qDebug( "%serror reading audio stream; no header list (!)",c_name );

	current_audio_stream->setProgressBar( (TTProgressBar*)NULL );
	delete progress_bar;
	delete audio_type;

	current_audio_stream = (TTAudioStream*)NULL;
	audio_type           = (TTAudioType*)NULL;
      }
      else
      {
	// remove progress bar
	current_audio_stream->setProgressBar( (TTProgressBar*)NULL );
	delete progress_bar;

        // audio stream succesfully parsed; add item to list
        lvAudioFiles->addItem( audio_file_name, current_audio_stream );

        // first audio track loaded
        TTCut::numAudioTracks = 1;
      }
    }
    // wrong audio type
    else
    {
      qDebug( "%swrong audio type for file: %s",c_name,audio_file_name.ascii() );

      delete audio_type;

      current_audio_stream = (TTAudioStream*)NULL;
      audio_type           = (TTAudioType*)NULL;

      // force user to give an audio file
      openAudioFile();
    }
  }

  // show the first video frame in cut-in window
  if ( TTCut::isVideoOpen )
  {
    // open the video stream an set header and index list
    sliderUpdateFrame = false;
    currentFramePos   = 0;

    ccrTab->getResultTab()->setVideoStream( mpeg2_stream );
     
    // initialize the the mpeg2 window for the cut-in frame
    //qDebug( "%sopen video stream",c_name );
    currentFrameWnd->openVideoStream( mpeg2_stream );
    currentFrameWnd->moveToFirstFrame( );
     
    // initialize the mpeg2 window for the cut-out frame
    //qDebug( "%sinitialize cut-out stream and window",c_name );
    cut_out_stream = new TTMpeg2VideoStream( QFileInfo(video_file_name));
    cut_out_stream->makeSharedCopy( mpeg2_stream );
    cutOutFrameWnd->openVideoStream( cut_out_stream );
    cutOutFrameWnd->moveToFirstFrame( false );
     
    // set the slider range
    sbVideoPosition->setMinValue( 0 );
    sbVideoPosition->setMaxValue( mpeg2_stream->frameCount()-1 );

    // initialize current frame position
    mpeg2_stream->setCurrentIndex( (uint)0 );
    current_frame_pos = 0;
    sbVideoPosition->setValue( current_frame_pos );
     
    refreshVideoLength();
    refreshCurrentPosition();
     
    enableNavigation();
  }
}

void TTCutMainWnd::closeProject()
{
  // TODO: ask for saving changes if theire any

   if ( TTCut::isVideoOpen )
   {
     // clear the list views
     lvAudioFiles->clearList();
     cutListView->clearList();

     // disable navigation
     disableNavigation();

     // clear the mpeg2 preview windows
     cutOutFrameWnd->closeVideoStream();
     currentFrameWnd->closeVideoStream();

     // delete the mpeg2 stream
     delete mpeg2_stream;
   }
}

// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// Audio section slots
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// open a audio stream
// -----------------------------------------------------------------------------
void TTCutMainWnd::openAudioFile()
{
  TTAudioType* new_audio_type;
  TTAudioStream* new_audio_stream;

  // you must first open a video file before you can add an additional
  // audio file to the project
  if ( !TTCut::isVideoOpen )
    return;

  // show file selection dialog and get audio file name
  QString fn = QFileDialog::getOpenFileName( this,
					     "Open audio file",
					     TTCut::lastDirPath,
					     "Audio (*.mpa *.mp2 *.ac3)" );

  // input filename specified
  if ( !fn.isEmpty() )
  {
    //TODO: add a new audio stream to list:lvAudioFiles->insertAudioFile( fn, false );

    QFileInfo f_info( fn );
    TTCut::lastDirPath = f_info.absolutePath();

    // get the strem type and create according stream-object
    new_audio_type = new TTAudioType( fn );

    // create the audio stream object for the first audio file
    if ( new_audio_type->avStreamType() == TTAVTypes::mpeg_audio  ||
	 new_audio_type->avStreamType() == TTAVTypes::ac3_audio   ||
	 new_audio_type->avStreamType() == TTAVTypes::dts14_audio ||
	 new_audio_type->avStreamType() == TTAVTypes::dts16_audio ||
	 new_audio_type->avStreamType() == TTAVTypes::pcm_audio      )
    {
      new_audio_stream = (TTAudioStream*)new_audio_type->createAudioStream();

      // set progress bar
      progress_bar = new TTProgressBar( this, "PBAR" );
      new_audio_stream->setProgressBar( progress_bar );
      progress_bar->show();
      qApp->processEvents();

      // create header list for audio stream
      num_header = new_audio_stream->createHeaderList();

      // error reading audio stream or user abort during operation
      if ( num_header == 0 && new_audio_type->avStreamType() != TTAVTypes::pcm_audio ||
           num_header == 1 && new_audio_type->avStreamType() == TTAVTypes::pcm_audio    )
      {
	qDebug( "%serror reading audio stream; no header list (!)",c_name );

	new_audio_stream->setProgressBar( (TTProgressBar*)NULL );
	delete progress_bar;
	delete new_audio_type;

	new_audio_stream = (TTAudioStream*)NULL;
	new_audio_type   = (TTAudioType*)NULL;
      }
      else
      {
	// remove progress bar
	new_audio_stream->setProgressBar( (TTProgressBar*)NULL );
	delete progress_bar;

        // audio stream succesfully parsed; add item to list
        lvAudioFiles->addItem( fn, new_audio_stream );

        // first audio track loaded
        TTCut::numAudioTracks += 1;

	// delete the av type
	delete new_audio_type;
      }
    }
    // wrong audio type
    else
    {
      qDebug( "%swrong audio type for file: %s",c_name,audio_file_name.ascii() );

      delete new_audio_type;

      new_audio_stream = (TTAudioStream*)NULL;
      new_audio_type   = (TTAudioType*)NULL;
    }
  }
}

// TODO: delete audio file from list

// move selected entry in audio list up
// -----------------------------------------------------------------------------
void TTCutMainWnd::audioFileUp()
{
  lvAudioFiles->itemUp();
}

// move selected entry in audio list down
// -----------------------------------------------------------------------------
void TTCutMainWnd::audioFileDown()
{
  lvAudioFiles->itemDown();
}


// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// Cut out frame section slots
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// move cut out position one frame back
// -----------------------------------------------------------------------------
void TTCutMainWnd::lastFramePrevious()
{
  uint cut_out_index;

  // TODO: move only, if a cut list entry is selected
  if ( !cutListView->isItemSelected() )
    return;

  if ( !TTCut::encoderMode )
    cut_out_index = cut_out_stream->moveToPrevPIFrame();
  else
    cut_out_index = cut_out_stream->moveToPrevFrame();
    

  emit cutOutIndexChanged( cut_out_index, cut_out_stream->currentFrameTime() );
  refreshCutOutPosition();

  cutOutFrameWnd->showFrameAt( cut_out_index ); 
}

// move cut out position one frame next
// -----------------------------------------------------------------------------
void TTCutMainWnd::lastFrameNext()
{
  long cut_out_index;

  // TODO: move only, if a cut list entry is selected
  if ( !cutListView->isItemSelected() )
    return;

  if ( !TTCut::encoderMode )
    cut_out_index = cut_out_stream->moveToNextPIFrame();
  else
    cut_out_index = cut_out_stream->moveToNextFrame();


  emit cutOutIndexChanged( cut_out_index, cut_out_stream->currentFrameTime() );
  refreshCutOutPosition();

  cutOutFrameWnd->showFrameAt( cut_out_index ); 
}

// search frame equivalent to cut out frame in current video stream
// -----------------------------------------------------------------------------
void TTCutMainWnd::searchFrame()
{
  long searchPos;

  // TODO: search only, if theire is a cut out frame

  progress_bar = new TTProgressBar( this );

  progress_bar->show();
  qApp->processEvents();

  TTFrameSearch* searchFrame = new TTFrameSearch( progress_bar );

  searchFrame->initFrameSearch( mpeg2_stream );
  searchPos = searchFrame->searchFrame( cut_out_stream->currentIndex(), 
	 			        mpeg2_stream->currentIndex() );

  // show frame in cut-in window 
  if ( searchPos >= 0 )
  {
    current_frame_pos = mpeg2_stream->moveToIndexPos( searchPos );

    showCutInFrameAt( current_frame_pos );
  }

  delete progress_bar;
  delete searchFrame;
}


// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// Current frame section slots
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// play the current video stream
// -----------------------------------------------------------------------------
void TTCutMainWnd::playVideo()
{
  Q3Process* mplayerProc;
  QString mplayerCmd;
  QString audioFile;
  QTime   cTime;
  long seconds;

  mplayerProc = new Q3Process();

  // Setup interface with MPlayer
  mplayerProc->clearArguments();         

  // ----------------------------------------------------------------------
  // slave-mode
  // ----------------------------------------------------------------------
  // Switches on slave mode, in which MPlayer works as a backend for other
  // programs. Instead of intercept- ing keyboard events, MPlayer will read
  // commands from stdin.
  // NOTE: See -input cmdlist for a list of slave com- mands and
  // DOCS/tech/slave.txt for their description.
  // ----------------------------------------------------------------------
  
  // Every argument must have it's own addArgument
  mplayerProc->addArgument( "mplayer"   );   
  mplayerProc->addArgument( "-slave"    );
  mplayerProc->addArgument( "-identify" );
  mplayerProc->addArgument( "-quiet"    );
  mplayerProc->addArgument( "-wid"      );
  mplayerCmd.sprintf( "%d",(int)currentFrameWnd->winId() );
  mplayerProc->addArgument( mplayerCmd  );
  mplayerProc->addArgument( "-geometry" );
  mplayerCmd.sprintf( "%dx%d+0+0", currentFrameWnd->width(), currentFrameWnd->height() );
  mplayerProc->addArgument( mplayerCmd  );

  // add fileName to mplayer argument list and start playing
  mplayerProc->addArgument( video_file_name  );

  audioFile = ttChangeFileExt( audio_file_name, "mp2" );
  //qDebug( "audio file: %s",audioFile.ascii() );
  mplayerProc->addArgument( "-audiofile" );
  mplayerProc->addArgument( audioFile );

    // print out mplayer command line for debug purpose
  //cout<<"MPlayer command-line: "<<mplayerProc->arguments().join(" ")<<endl;

  // start the mplayer process
  if ( !mplayerProc->isRunning() && !mplayerProc->start() )
  {
    qDebug( "Error starting mplayer process" );
  }

  cTime = ttFramesToTime( current_frame_pos, mpeg2_stream->frameRate() );
  seconds = cTime.hour()*3600+cTime.minute()*60+cTime.second();

  //qDebug( "seek seconds: %ld",seconds );
  QString str_pause = "pause\n";
  mplayerProc->writeToStdin( str_pause  );
  //QT4: mplayerProc->writeToStdin( "seek seconds 2\n"  );
}


// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// Navigation section slots
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// go to the previous I-Frame
// -----------------------------------------------------------------------------
void TTCutMainWnd::gotoPrevIFrame()
{
  current_frame_pos = mpeg2_stream->moveToPrevIFrame();

  showCutInFrameAt( current_frame_pos );
}

// go to the next I-Frame
// -----------------------------------------------------------------------------
void TTCutMainWnd::gotoNextIFrame()
{
  current_frame_pos = mpeg2_stream->moveToNextIFrame();

  showCutInFrameAt( current_frame_pos );
}

// go to the previous P-Frame
// -----------------------------------------------------------------------------
void TTCutMainWnd::gotoPrevPFrame()
{
  current_frame_pos = mpeg2_stream->moveToPrevPFrame();

  showCutInFrameAt( current_frame_pos );
}

// go to the next P-Frame
// -----------------------------------------------------------------------------
void TTCutMainWnd::gotoNextPFrame()
{
  current_frame_pos = mpeg2_stream->moveToNextPFrame();

  showCutInFrameAt( current_frame_pos );
}

// go to the previous frame
// -----------------------------------------------------------------------------
void TTCutMainWnd::gotoPrevFrame()
{
  current_frame_pos = mpeg2_stream->moveToPrevFrame();

  showCutInFrameAt( current_frame_pos );
}

// go to the next frame
// -----------------------------------------------------------------------------
void TTCutMainWnd::gotoNextFrame()
{
  current_frame_pos = mpeg2_stream->moveToNextFrame();

  showCutInFrameAt( current_frame_pos );
}

// set the cut in position
// -----------------------------------------------------------------------------
void TTCutMainWnd::setCutInPoint()
{
  QString  szTemp;

  szTemp = mpeg2_stream->currentFrameTime().toString("hh:mm:ss.zzz");
  tlCutInPosition->setText( szTemp );

  current_cut_pos->setCutIn( mpeg2_stream->currentIndex() );
  current_cut_pos->setCutInOffset( mpeg2_stream->currentFrameOffset() );

  isCutInPos = true;
}

// set the cut out position
// -----------------------------------------------------------------------------
void TTCutMainWnd::setCutOutPoint()
{
  QString  szTemp;

  //qDebug( "set cut-out position" );

  szTemp = mpeg2_stream->currentFrameTime().toString("hh:mm:ss.zzz");
  tlCutOutPosition->setText( szTemp );

  //qDebug( "move to index: %d",mpeg2_stream->currentIndex() );

  current_cut_pos->setCutOut( mpeg2_stream->currentIndex() );
  current_cut_pos->setCutOutOffset( mpeg2_stream->currentFrameOffset() );

  cut_out_stream->moveToIndexPos( current_cut_pos->cutOut() );

  refreshCutOutPosition();

  cutOutFrameWnd->showFrameAt( current_cut_pos->cutOut() );

  isCutOutPos = true;
}

// go to the cut in position
// -----------------------------------------------------------------------------
void TTCutMainWnd::gotoCutInPosition()
{
  if ( isCutInPos )
  {
    current_frame_pos = mpeg2_stream->moveToIndexPos( current_cut_pos->cutIn() );
    showCutInFrameAt( current_frame_pos );
  }
}

// go to cut out position
// -----------------------------------------------------------------------------
void TTCutMainWnd::gotoCutOutPosition()
{
  if ( isCutOutPos )
  {
    current_frame_pos = mpeg2_stream->moveToIndexPos( current_cut_pos->cutOut() );
    showCutInFrameAt( current_frame_pos );
  }
}

// add current cut range to the cut list
// -----------------------------------------------------------------------------
void TTCutMainWnd::addRangeToCutList()
{
  if ( isCutInPos && isCutOutPos )
    {
      // add information about current cut position to cut list view
      cutListView->addItem( mpeg2_stream->fileName(),
			    current_cut_pos->cutIn(),
			    current_cut_pos->cutInOffset(),
			    mpeg2_stream->frameTime(current_cut_pos->cutIn()),
			    current_cut_pos->cutOut(),
			    current_cut_pos->cutOutOffset(),
			    mpeg2_stream->frameTime(current_cut_pos->cutOut()) );

      // TODO: calculate and show the new estimate video length after current cut

      // clear cut-in and cut-position
      tlCutInPosition->setText( "..." );
      tlCutOutPosition->setText( "..." );

      // no actually cut-in and cut position
      isCutInPos  = false;
      isCutOutPos = false;
    }
}

// show a dialog with thumbnail frame preview for quick jump
// -----------------------------------------------------------------------------
void TTCutMainWnd::fastMotionOption()
{

}

// stream points; cut proposal
// -----------------------------------------------------------------------------
void TTCutMainWnd::cutProposition()
{

}

// set a marker position
// -----------------------------------------------------------------------------
void TTCutMainWnd::setMarker()
{
  markerPosition = mpeg2_stream->currentIndex();
  pbGotoMarker->setEnabled( true );
  refreshMarkerPosition();
}

// go to previous set marker position
// -----------------------------------------------------------------------------
void TTCutMainWnd::gotoMarker()
{
  if ( markerPosition >= 0 )
  {
    current_frame_pos = mpeg2_stream->moveToIndexPos( markerPosition );
    showCutInFrameAt( current_frame_pos );
  }
}

// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// Video slider section
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// slider position has changed
// -----------------------------------------------------------------------------
void TTCutMainWnd::videoSliderChanged( int iSliderPos )
{
  //qDebug( "%svideo slider changed: %d - %d",c_name,sliderUpdateFrame,iSliderPos );

  if ( sliderUpdateFrame && TTCut::isVideoOpen )
  {
    if ( TTCut::fastSlider )
      current_frame_pos = mpeg2_stream->moveToIndexPos( iSliderPos, 1 );
    else
      current_frame_pos = mpeg2_stream->moveToIndexPos( iSliderPos, 0 );

    iSliderPrev = iSliderPos;

    checkCutPosition();

    refreshCurrentPosition();

    currentFrameWnd->showFrameAt( current_frame_pos );
  } 
  sliderUpdateFrame = true;
}

// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// Cut list section
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// if an entry in the cut-list is selected show the according cout-out frame
// -----------------------------------------------------------------------------
void TTCutMainWnd::selectCutOut( TTAVCutPosition& cut_pos )
{
  uint cut_out_index;

  cut_out_index = cut_out_stream->moveToIndexPos( cut_pos.cutOut() );
  refreshCutOutPosition();
  cutOutFrameWnd->showFrameAt( cut_out_index );
}

// show frame at cut-in position in cut-in window
// -----------------------------------------------------------------------------
void TTCutMainWnd::showSlaveCutIn( TTAVCutPosition& cut_pos )
{
  current_frame_pos = mpeg2_stream->moveToIndexPos( cut_pos.cutIn() );
  checkCutPosition();
  refreshCurrentPosition();
  showCutInFrameAt( current_frame_pos );
}

// show frame at cut-out position in cut-in window
// -----------------------------------------------------------------------------
void TTCutMainWnd::showSlaveCutOut( TTAVCutPosition& cut_pos )
{
  current_frame_pos = mpeg2_stream->moveToIndexPos( cut_pos.cutOut() );
  checkCutPosition();
  refreshCurrentPosition();
  showCutInFrameAt( current_frame_pos );
}

// move selected entry in cut list up
// -----------------------------------------------------------------------------
void TTCutMainWnd::moveCutEntryUp()
{
  cutListView->itemUp();
}


// delete selected entry in cut list
// -----------------------------------------------------------------------------
void TTCutMainWnd::deleteCutEntry()
{
  cutListView->deleteItem();
}

// push selected entry in cut list down
// -----------------------------------------------------------------------------
void TTCutMainWnd::moveCutEntryDown()
{
  cutListView->itemDown();
}

// edit selected entry in cut list
// -----------------------------------------------------------------------------
void TTCutMainWnd::editCutListEntry( TTAVCutPosition& cut_pos )
{
  // text for the cut-in and cut-out position
  tlCutInPosition->setText( mpeg2_stream->frameTime(cut_pos.cutIn()).toString("hh:mm:ss.zzz") );
  tlCutOutPosition->setText( mpeg2_stream->frameTime(cut_pos.cutOut()).toString("hh:mm:ss.zzz" ) );

  // set current mpeg2 stream frame position and show the cut's cut-in frame
  current_frame_pos = mpeg2_stream->moveToIndexPos( cut_pos.cutIn() );
  showCutInFrameAt( current_frame_pos );

  // set current cut position values
  current_cut_pos->setCutIn( cut_pos.cutIn() );
  current_cut_pos->setCutOut( cut_pos.cutOut() );

  // yes, we have valid cut-in and cut-out positions
  isCutInPos  = true;
  isCutOutPos = true;
}

void TTCutMainWnd::previewSingleCut( uint c_index )
{
  int num_audio;
  QList<TTAudioStream*> audio_list;
  TTAudioStream* current_audio_stream = NULL;

  TTAVCutList* avcut_list;

  if ( TTCut::isVideoOpen && cutListView->count() > 0 )
  {
    // Anzahl Audiodateien	
    num_audio = lvAudioFiles->count();
    
    if ( num_audio > 0 ) 
    {
      num_audio = lvAudioFiles->getAudioList( audio_list );
      current_audio_stream = audio_list.at( 0 );
    }    

    // create preview dialog frame
    TTCutPreview* cutPreview = new TTCutPreview( this );

    // set video/audio index- and header lists
    avcut_list = cutListView->cutList();
    cutPreview->initPreview( mpeg2_stream, current_audio_stream, avcut_list );
    cutPreview->createPreview( c_index );
    
    // execute modal dialog frame
    cutPreview->exec();
  }
}

// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// Button action in cut-chapter-result (ccr) section
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// preview the cut-clips in the cut list
// -----------------------------------------------------------------------------
void TTCutMainWnd::cutPreview()
{
  int num_audio;
  QList<TTAudioStream*> audio_list;
  TTAudioStream* current_audio_stream = NULL;

  TTAVCutList* avcut_list;

  if ( TTCut::isVideoOpen && cutListView->count() > 0 )
  {
   // Anzahl Audiodateien	
   num_audio = lvAudioFiles->count();

   if ( num_audio > 0 ) 
   {
     num_audio = lvAudioFiles->getAudioList( audio_list );
     current_audio_stream = audio_list.at( 0 );
   }

   // create preview dialog frame
   TTCutPreview* cutPreview = new TTCutPreview( this );
   
   // set video/audio index- and header lists
   avcut_list = cutListView->cutList();
   cutPreview->initPreview( mpeg2_stream, current_audio_stream, avcut_list );
   cutPreview->createPreview();
    
   // execute modal dialog frame
   cutPreview->exec();
  }
}

// button video/audio cut clicked
// -----------------------------------------------------------------------------
void TTCutMainWnd::videoAudioCut()
{
   QString        AudioDateiEndung;
   QString        HString;
   int            AudioAnzahl;
   //int            Nummerieren;
   int num_audio;
   int list_pos = 0;
   bool           nurAudioSchneiden;
   QString        videoCutName;
   QString        audio_cut_name;
   QString        audio_number;
   QFileInfo      video_cut_file_info;
   QFileInfo      audio_cut_file_info;
   uint len1, len2, len;
   //int i;
   //int   list_count;
  //int   current_entry;
  //uint   start_index;
  //uint   end_index;
  TTFileBuffer* video_cut_stream;
  TTFileBuffer* audio_cut_stream;
  TTAudioStream* current_audio_stream;
  QList<TTAudioStream*> audio_list;

   // no video stream open or no cut sequences defined; exit
   if ( !TTCut::isVideoOpen || cutListView->count() == 0 )
      return;

   // --------------------------------------------------------------------------
   // compose video cut name from video file name
   // --------------------------------------------------------------------------
   // get video file file-extension
   QString sExt = mpeg2_stream->fileInfo()->suffix();
   
   // remove the extension
   len1 = sExt.length();
   len2 = mpeg2_stream->fileInfo()->fileName().length();
   
   len  = len2 - len1 - 1;
   
   videoCutName = mpeg2_stream->fileInfo()->fileName();
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
   cutAVDlg = new TTCutAVCutDlg( this, "CutAVDlg" );


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

   avcut_list = cutListView->cutList();
   avcut_list->sort();

   // --------------------------------------------------------------------------
   // Cut video-file
   // --------------------------------------------------------------------------
     
   // cut only audio ???
   if ( !nurAudioSchneiden )
   {
     //qDebug("Meldung125: Die Videodaten werden in der Datei %s gespeichert.",videoCutName.ascii());
     
     progress_bar = new TTProgressBar( this, "ProgressBar", true);
     progress_bar->show();
     qApp->processEvents();
     
     video_cut_stream = new TTFileBuffer( videoCutName.ascii(), fm_open_write );
     
     mpeg2_stream->setProgressBar( progress_bar );
     
     mpeg2_stream->cut( video_cut_stream, avcut_list );
     
     //qDebug("Meldung128: Das Schneiden der Datei %s ist beendet.",HString.ascii());
     delete progress_bar;
     delete video_cut_stream;
   }
   // Ende Videoschnitt

   // --------------------------------------------------------------------------
   // Cut audio-file
   // --------------------------------------------------------------------------

   // Anzahl Audiodateien	
   num_audio = lvAudioFiles->count();

   if ( num_audio > 0 ) 
   {
     num_audio = lvAudioFiles->getAudioList( audio_list );
   }

   AudioAnzahl = num_audio;

   //qDebug( "%sEs wird(werden) %d Audiodatei(en) erzeugt.",c_name,AudioAnzahl );

   list_pos = 0;

   while ( AudioAnzahl > 0 )
   {
     current_audio_stream = audio_list.at( list_pos );

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
     
     progress_bar = new TTProgressBar( this, "ProgressBar", true);
     progress_bar->show();
     qApp->processEvents();

     current_audio_stream->setProgressBar( progress_bar );

     audio_cut_stream = new TTFileBuffer( audio_cut_name.ascii(), fm_open_write );

     current_audio_stream->cut( audio_cut_stream, avcut_list );

     delete progress_bar;
     delete audio_cut_stream;

     list_pos++;
     AudioAnzahl--;
   }
   // Ende Audioschnitt

}

// button "cut audio only" clicked
// -----------------------------------------------------------------------------
void TTCutMainWnd::audioCut()
{
  // TODO: cut only audio files
}


// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// Common methods for app's main window
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// disable all navigation items, since theire is no video file opened
void TTCutMainWnd::disableNavigation()
{
  pbPrevIFrame->setEnabled( false );
  pbPrevPFrame->setEnabled( false );
  pbPrevBFrame->setEnabled( false );
  pbNextIFrame->setEnabled( false );
  pbNextPFrame->setEnabled( false );
  pbNextBFrame->setEnabled( false );
  pbSetCutIn->setEnabled( false );
  pbSetCutOut->setEnabled( false );
  pbGotoCutIn->setEnabled( false );
  pbGotoCutOut->setEnabled( false );
  pbAddCut->setEnabled( false );
  pbStreamPoints->setEnabled( false );
  pbGotoMarker->setEnabled( false );
  pbQuickJump->setEnabled( false );
  pbSetMarker->setEnabled( false );
  sbVideoPosition->setEnabled( false );

  pbPlayVideo->setEnabled( false );
  pbPrevCutOutFrame->setEnabled( false );
  pbNextCutOutFrame->setEnabled( false );
  pbSearchFrame->setEnabled( false );

  pbChapter->setEnabled( false );
  pbCutPreview->setEnabled( false );
  pbVideoAudioCut->setEnabled( false );
  pbAudioCut->setEnabled( false );
}

// enable navigation items
void TTCutMainWnd::enableNavigation()
{
  pbPrevIFrame->setEnabled( true );
  pbPrevPFrame->setEnabled( true );
  pbPrevBFrame->setEnabled( true );
  pbNextIFrame->setEnabled( true );
  pbNextPFrame->setEnabled( true );
  pbNextBFrame->setEnabled( true );
  pbSetCutIn->setEnabled( true );
  pbSetCutOut->setEnabled( true );

  pbGotoCutIn->setEnabled( true );
  pbGotoCutOut->setEnabled( true );

  pbAddCut->setEnabled( true );

  pbStreamPoints->setEnabled( false );
  pbGotoMarker->setEnabled( false );
  pbQuickJump->setEnabled( false );
  pbSetMarker->setEnabled( true );

  sbVideoPosition->setEnabled( true );

  pbPlayVideo->setEnabled( false );

  pbPrevCutOutFrame->setEnabled( true );
  pbNextCutOutFrame->setEnabled( true );
  pbSearchFrame->setEnabled( true );

  pbChapter->setEnabled( false );
  pbCutPreview->setEnabled( true );
  pbVideoAudioCut->setEnabled( true );
  pbAudioCut->setEnabled( false );
}

// check cut position
void TTCutMainWnd::checkCutPosition()
{
  // cut-in possible
  if ( mpeg2_stream->isCutInPoint( current_frame_pos ) )
    pbSetCutIn->setEnabled( true );
  else
    pbSetCutIn->setEnabled( false );

  // cut-out possible
  if ( mpeg2_stream->isCutOutPoint( current_frame_pos ) )
    pbSetCutOut->setEnabled( true );
  else
    pbSetCutOut->setEnabled( false );
}


// refresh the text label for the current stream position
// and update the bitrate in the video stream properties display
void TTCutMainWnd::refreshCurrentPosition()
{
  QString      szTemp1, szTemp2;
  double       fBitRate;
  int          frame_type = mpeg2_stream->currentFrameType();

  szTemp1 = mpeg2_stream->currentFrameTime().toString("hh:mm:ss.zzz");

  szTemp2.sprintf(" (%d)",current_frame_pos);

  if ( frame_type == 1 ) szTemp2 += " [I]";
  if ( frame_type == 2 ) szTemp2 += " [P]";
  if ( frame_type == 3 ) szTemp2 += " [B]";

  szTemp1 += szTemp2;
  tlCurrentPosition->setText( szTemp1 );

  tlCurrentPosition->update();

  fBitRate = (double) mpeg2_stream->bitRate() * 8.0; // (double)1000.0;
  szTemp1.sprintf( "%4.1lf kBit/s", fBitRate );
  tlBitRate->setText( szTemp1 );

  tlBitRate->update();

  //qApp->processEvents();
}

// refresh the text label for current marker position
void TTCutMainWnd::refreshMarkerPosition()
{
  QString      szTemp1, szTemp2;
  //double       fBitRate;
  int          frame_type = mpeg2_stream->currentFrameType();

  szTemp1 = mpeg2_stream->currentFrameTime().toString("hh:mm:ss.zzz");

  szTemp2.sprintf(" (%ld)",markerPosition);

  if ( frame_type == 1 ) szTemp2 += " [I]";
  if ( frame_type == 2 ) szTemp2 += " [P]";
  if ( frame_type == 3 ) szTemp2 += " [B]";

  szTemp1 += szTemp2;
  tlMarkerPosition->setText( szTemp1 );
}

// refresh the text label for cut out frame position
void TTCutMainWnd::refreshCutOutPosition()
{
  QString      szTemp1, szTemp2;
  //double       fBitRate;
  int          frame_type = cut_out_stream->currentFrameType();

  szTemp1 = cut_out_stream->currentFrameTime().toString("hh:mm:ss.zzz");

  if ( frame_type == 1 ) szTemp2 = "[I]";
  if ( frame_type == 2 ) szTemp2 = "[P]";
  if ( frame_type == 3 ) szTemp2 = "[B]";

  szTemp1 += szTemp2;
  laCutOutFramePosition->setText( szTemp1 );
}

// refresh the text label for the video stream length
void TTCutMainWnd::refreshVideoLength()
{
  QTime   time;
  QString szTemp1, szTemp2;
  long    numFrames;

  numFrames = mpeg2_stream->frameCount();

  time = ttFramesToTime( numFrames, mpeg2_stream->frameRate() );

  szTemp1 = time.toString("hh:mm:ss.zzz");

  szTemp2.sprintf(" (%ld)",numFrames);

  szTemp1 += szTemp2;
  tlVideoLen->setText( szTemp1 );
}

// refresh the text label for the resulting video length
// TODO: unused
void TTCutMainWnd::refreshCutVideoLength()
{
  QTime   time;
  QString szTemp1, szTemp2;
  long    numFrames;

  numFrames = cutListView->getResultFrames();
  // CORRECT: time      = cutListView->getResultTime();

  szTemp1 = time.toString("hh:mm:ss.zzz");

  szTemp2.sprintf(" (%ld)",numFrames);

  szTemp1 += szTemp2;
}



void TTCutMainWnd::showVideoStreamInfo()
{
  QString szTemp;

  // video loaded, set filename label
  tlVideoFileName->setText( mpeg2_stream->fileInfo()->fileName() );
  
  // set video resolution
  szTemp.sprintf( "%dx%d",mpeg2_stream->currentSequenceHeader()->horizontalSize(),
		          mpeg2_stream->currentSequenceHeader()->verticalSize() );
  tlResolution->setText( szTemp );
  
  // set aspect
  tlVideoAspect->setText( mpeg2_stream->currentSequenceHeader()->aspectRatioText() );
  
  // set framerate
  tlFrameRate->setText( mpeg2_stream->currentSequenceHeader()->frameRateText() );
  
  // set bitrate
  szTemp.sprintf( "%4.1f kBit/s", mpeg2_stream->currentSequenceHeader()->bitRateKbit() );
  tlBitRate->setText( szTemp );
  
  // set VBV buffer size
  szTemp.sprintf( "%d kWords",mpeg2_stream->currentSequenceHeader()->vbvBufferSize() );
  tlVbvBuffer->setText( szTemp );
}


// show frame an index position in cut-in window
// -----------------------------------------------------------------------------
void TTCutMainWnd::showCutInFrameAt( uint i_pos )
{
  //qDebug( "%sshow cut-in frame: %d",c_name,i_pos );

  // check if cut is possible
  checkCutPosition();

  // disable slider update and set new slider value
  sliderUpdateFrame = false;
  sbVideoPosition->setValue( i_pos );

  // refresh information about current stream position and current
  // sequence
  refreshCurrentPosition();

  currentFrameWnd->showFrameAt( i_pos );
}

// move current position in mpeg2 stream to "i_pos" and frame type
// -----------------------------------------------------------------------------
uint TTCutMainWnd::moveToMpeg2Index( uint i_pos, int f_type )
{
  // set position
  current_frame_pos = mpeg2_stream->moveToIndexPos( i_pos, f_type );

  // check if cut is possible
  checkCutPosition();

  // disable slider update and set new slider value
  sliderUpdateFrame = false;
  sbVideoPosition->setValue( current_frame_pos );

  // refresh information about current stream position and current
  // sequence
  refreshCurrentPosition();

  return current_frame_pos;
}
