/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTMPEG2 2005                                                    */
/* FILE     : ttmpeg2mainwnd.cpp                                              */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 05/14/2005 */
/* MODIFIED: b. altendorf                                    DATE: 05/17/2005 */
/* MODIFIED: b. altendorf                                    DATE: 06/02/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTMPEG2MAINWND
// *** TTCURRENTFRAMEINFO
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

#include "ttmpeg2mainwnd.h"

#include <qapplication.h>
#include <qvariant.h>
#include <qpushbutton.h>
#include <q3groupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <q3buttongroup.h>
#include <qradiobutton.h>
#include <q3frame.h>
#include <q3header.h>
#include <q3listview.h>
#include <qscrollbar.h>
#include <qlayout.h>
#include <qtooltip.h>  
#include <q3whatsthis.h>
#include <qaction.h>
#include <qmenubar.h>
#include <q3popupmenu.h>
#include <q3toolbar.h>
#include <qimage.h>
#include <qpixmap.h>
#include <q3filedialog.h>
#include <qtoolbutton.h>
//Added by qt3to4:
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

// pixmaps
// -----------------------------------------------------------------------------
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

#include "../pixmaps/iframe_18.xpm"
#include "../pixmaps/pframe_18.xpm"
#include "../pixmaps/bframe_18.xpm"

#include "ttmpeg2mainwnd.h"

// debug print
const char cName[] = "TTMPEG2WND    : ";

TTMpeg2MainWnd::TTMpeg2MainWnd( QWidget* parent, const char* name, Qt::WFlags fl )
  : Q3MainWindow( parent, name, fl )
			      //      image0( (const char **) image0_data )
{
  if ( !name )
    setName( "TTMpeg2MainWnd" );

  // images
  // ---------------------------------------------------------------------------
  TTCut::imgDownArrow  = new QPixmap( (const char**)downarrow_18_xpm );
  TTCut::imgUpArrow    = new QPixmap( (const char**)uparrow_18_xpm );
  TTCut::imgDelete     = new QPixmap( (const char**)cancel_18_xpm );
  TTCut::imgFileOpen24 = new QPixmap( (const char**)fileopen_24_xpm );
  TTCut::imgFileNew    = new QPixmap( (const char**)filenew_16_xpm );
  TTCut::imgFileOpen   = new QPixmap( (const char**)fileopen_16_xpm );
  TTCut::imgFileSave   = new QPixmap( (const char**)filesave_16_xpm );;
  TTCut::imgFileSaveAs = new QPixmap( (const char**)filesaveas_16_xpm );
  TTCut::imgSaveImage  = new QPixmap( (const char**)saveimage_16_xpm );
  TTCut::imgSettings   = new QPixmap( (const char**)settings_16_xpm );
  TTCut::imgSettings18 = new QPixmap( (const char**)settings_18_xpm );
  TTCut::imgExit       = new QPixmap( (const char**)exit_16_xpm );
  TTCut::imgPlay       = new QPixmap( (const char**)play_18_xpm );
  TTCut::imgStop       = new QPixmap( (const char**)stop_18_xpm );
  TTCut::imgSearch     = new QPixmap( (const char**)search_18_xpm );
  TTCut::imgChapter    = new QPixmap( (const char**)chapter_18_xpm );
  TTCut::imgPreview    = new QPixmap( (const char**)preview_18_xpm );
  TTCut::imgCutAV      = new QPixmap( (const char**)cutav_18_xpm );
  TTCut::imgCutAudio   = new QPixmap( (const char**)cutaudio_18_xpm );
  TTCut::imgGoTo       = new QPixmap( (const char**)goto_18_xpm );
  TTCut::imgMarker     = new QPixmap( (const char**)note_18_xpm );
  TTCut::imgClock      = new QPixmap( (const char**)clock_16_xpm );
  TTCut::imgApply      = new QPixmap( (const char**)apply_18_xpm );
  TTCut::imgAddToList  = new QPixmap( (const char**)addtolist_18_xpm );
  TTCut::imgFileClose  = new QPixmap( (const char**)fileclose_18_xpm );

  slider_update = true;

  // min widgets size
  // --------------------------------------------------------------------------
  setMinimumSize( QSize( 860, 480 ) );
  setMaximumSize( QSize( 1600, 1280 ) );

  // central widget
  // --------------------------------------------------------------------------
  setCentralWidget( new QWidget( this, "qt_central_widget" ) );


  // --------------------------------------------------------------------------
  // menu-bar
  // --------------------------------------------------------------------------

  // menu actions
  // --------------------------------------------------------------------------

  // file-menu
  fileOpenAction       = new QAction( this, "fileOpenAction" );
  fileOpenAction->setIconSet( QIcon( *(TTCut::imgFileOpen) ) );
  fileSettingsAction   = new QAction( this, "fileSettingsAction" );
  fileSettingsAction->setIconSet( QIcon( *(TTCut::imgSettings) ) );
  fileAnalyzeAction    = new QAction( this, "fileAnalyzeAction" );
  fileAnalyzeAction->setIconSet( QIcon( *(TTCut::imgPreview) ) );
  fileExitAction       = new QAction( this, "fileExitAction" );
  fileExitAction->setIconSet( QIcon( *(TTCut::imgExit) ) );

  // menubar
  MenuBar = new QMenuBar( this, "MenuBar" );
  
  //file-menu
  fileMenu = new Q3PopupMenu( this );
  fileOpenAction->addTo( fileMenu );
  fileAnalyzeAction->addTo( fileMenu );
  fileMenu->insertSeparator();
  fileSettingsAction->addTo( fileMenu );
  fileMenu->insertSeparator();
  fileExitAction->addTo( fileMenu );
  MenuBar->insertItem( QString("File"), fileMenu,   1 );
  

  // ---------------------------------------------------------------------------
  // tool-bar
  // ---------------------------------------------------------------------------
  toolBar = new Q3ToolBar( this, "ToolBar" );

  tbStartAnalyzing = new QToolButton( toolBar, "StartAnalyze" );
  tbStartAnalyzing->setIconSet( QIcon( *(TTCut::imgPreview) ) );

  // ---------------------------------------------------------------------------
  // status-bar
  // ---------------------------------------------------------------------------
  (void)statusBar();


  // central layout manager (grid 4x2)
  // ---------------------------------------------------------------------------
  TTMpeg2MainWndLayout = new QGridLayout( centralWidget(), 
					  4, 1, 11, 6, 
					  "TTMpeg2MainWndLayout"); 

  // open stream section
  // ---------------------------------------------------------------------------
  gbStreamName = new Q3GroupBox( centralWidget(), "gbStreamName" );
  gbStreamName->setColumnLayout(0, Qt::Vertical );
  gbStreamName->layout()->setSpacing( 6 );
  gbStreamName->layout()->setMargin( 11 );
  gbStreamNameLayout = new QGridLayout( gbStreamName->layout() );
  gbStreamNameLayout->setAlignment( Qt::AlignTop );
  
  leStreamName = new QLineEdit( gbStreamName, "leStreamName" );
  
  gbStreamNameLayout->addWidget( leStreamName, 0, 1 );
  
  pbOpenStream = new QPushButton( gbStreamName, "pbOpenStream" );
  pbOpenStream->setMinimumSize( QSize( 24, 24 ) );
  pbOpenStream->setMaximumSize( QSize( 24, 24 ) );
  pbOpenStream->setPixmap( (QPixmap)*(TTCut::imgFileOpen) );
  
  gbStreamNameLayout->addWidget( pbOpenStream, 0, 0 );
  TTMpeg2MainWndLayout->addMultiCellWidget( gbStreamName, 0, 0, 0, 1 );


  // stream info section
  // ---------------------------------------------------------------------------
  layout10 = new QHBoxLayout( 0, 0, 6, "layout10"); 
  layout9 = new QVBoxLayout( 0, 0, 6, "layout9"); 

  // buffer statistics
  gbBufferStatistics = new Q3GroupBox( centralWidget(), "gbBufferStatistics" );
  gbBufferStatistics->setColumnLayout(0, Qt::Vertical );
  gbBufferStatistics->layout()->setSpacing( 6 );
  gbBufferStatistics->layout()->setMargin( 11 );
  gbBufferStatisticsLayout = new QGridLayout( gbBufferStatistics->layout() );
  gbBufferStatisticsLayout->setAlignment( Qt::AlignTop );
  
  layout1 = new QVBoxLayout( 0, 0, 6, "layout1"); 

  slFileLength = new QLabel( gbBufferStatistics, "slFileLength" );
  layout1->addWidget( slFileLength );
  
  slReadOps = new QLabel( gbBufferStatistics, "slReadOps" );
  layout1->addWidget( slReadOps );
  
  slFillOps = new QLabel( gbBufferStatistics, "slFillOps" );
  layout1->addWidget( slFillOps );
  
  slReadTime = new QLabel( gbBufferStatistics, "slReadTime" );
  layout1->addWidget( slReadTime );
  
  gbBufferStatisticsLayout->addLayout( layout1, 0, 0 );
  
  layout2 = new QVBoxLayout( 0, 0, 6, "layout2"); 
  
  laFileLength = new QLabel( gbBufferStatistics, "laFileLength" );
  layout2->addWidget( laFileLength );
  
  laReadOps = new QLabel( gbBufferStatistics, "laReadOps" );
  layout2->addWidget( laReadOps );
  
  laFillOps = new QLabel( gbBufferStatistics, "laFillOps" );
  layout2->addWidget( laFillOps );
  
  laReadTime = new QLabel( gbBufferStatistics, "laReadTime" );
  layout2->addWidget( laReadTime );
  
  gbBufferStatisticsLayout->addLayout( layout2, 0, 1 );
  layout9->addWidget( gbBufferStatistics );

  // stream statistics
  gbStreamStatistics = new Q3GroupBox( centralWidget(), "gbStreamStatistics" );
  gbStreamStatistics->setColumnLayout(0, Qt::Vertical );
  gbStreamStatistics->layout()->setSpacing( 6 );
  gbStreamStatistics->layout()->setMargin( 11 );
  gbStreamStatisticsLayout = new QGridLayout( gbStreamStatistics->layout() );
  gbStreamStatisticsLayout->setAlignment( Qt::AlignTop );

  line1 = new Q3Frame( gbStreamStatistics, "line1" );
  line1->setFrameShape( Q3Frame::HLine );
  line1->setFrameShadow( Q3Frame::Sunken );
  line1->setFrameShape( Q3Frame::HLine );
  
  gbStreamStatisticsLayout->addMultiCellWidget( line1, 1, 1, 0, 1 );
  
  layout5 = new QVBoxLayout( 0, 0, 6, "layout5"); 
  
  slTotalFrames = new QLabel( gbStreamStatistics, "slTotalFrames" );
  layout5->addWidget( slTotalFrames );
  
  slIFrames = new QLabel( gbStreamStatistics, "slIFrames" );
  layout5->addWidget( slIFrames );
  
  slPFrames = new QLabel( gbStreamStatistics, "slPFrames" );
  layout5->addWidget( slPFrames );
  
  slBFrames = new QLabel( gbStreamStatistics, "slBFrames" );
  layout5->addWidget( slBFrames );
  
  gbStreamStatisticsLayout->addLayout( layout5, 0, 0 );
  
  layout4 = new QVBoxLayout( 0, 0, 6, "layout4"); 
  
  slSeqHeader = new QLabel( gbStreamStatistics, "slSeqHeader" );
  layout4->addWidget( slSeqHeader );
  
  slGOPHeader = new QLabel( gbStreamStatistics, "slGOPHeader" );
  layout4->addWidget( slGOPHeader );
  
  slPicHeader = new QLabel( gbStreamStatistics, "slPicHeader" );
  layout4->addWidget( slPicHeader );
  
  slSeqEndHeader = new QLabel( gbStreamStatistics, "slSeqEndHeader" );
  layout4->addWidget( slSeqEndHeader );
  
  gbStreamStatisticsLayout->addLayout( layout4, 2, 0 );
  
  layout6 = new QVBoxLayout( 0, 0, 6, "layout6"); 
  
  laSequence = new QLabel( gbStreamStatistics, "laSequence" );
  layout6->addWidget( laSequence );
  
  laGOP = new QLabel( gbStreamStatistics, "laGOP" );
  layout6->addWidget( laGOP );
  
  laPicture = new QLabel( gbStreamStatistics, "laPicture" );
  layout6->addWidget( laPicture );
  
  laSequenceEnd = new QLabel( gbStreamStatistics, "laSequenceEnd" );
  layout6->addWidget( laSequenceEnd );
  
  gbStreamStatisticsLayout->addLayout( layout6, 2, 1 );

  layout7 = new QVBoxLayout( 0, 0, 6, "layout7"); 

  laFramesTotal = new QLabel( gbStreamStatistics, "laFramesTotal" );
  layout7->addWidget( laFramesTotal );
  
  laIFrames = new QLabel( gbStreamStatistics, "laIFrames" );
  layout7->addWidget( laIFrames );
  
  laPFrames = new QLabel( gbStreamStatistics, "laPFrames" );
  layout7->addWidget( laPFrames );
  
  laBFrames = new QLabel( gbStreamStatistics, "laBFrames" );
  layout7->addWidget( laBFrames );
  
  gbStreamStatisticsLayout->addLayout( layout7, 0, 1 );
  layout9->addWidget( gbStreamStatistics );
  layout10->addLayout( layout9 );

  // list view stream info
  gbStreamInfo = new Q3GroupBox( centralWidget(), "gbStreamInfo" );
  gbStreamInfo->setColumnLayout(0, Qt::Vertical );
  gbStreamInfo->layout()->setSpacing( 6 );
  gbStreamInfo->layout()->setMargin( 11 );
  gbStreamInfoLayout = new QGridLayout( gbStreamInfo->layout() );
  gbStreamInfoLayout->setAlignment( Qt::AlignTop );
  
  lvStreamInfo = new Q3ListView( gbStreamInfo, "lvStreamInfo" );
  lvStreamInfo->addColumn( tr( "Header" ) );
  lvStreamInfo->addColumn( tr( "Offset" ) );
  lvStreamInfo->addColumn( tr( "Type" ) );
  lvStreamInfo->addColumn( tr( "Temp Ref." ) );
  lvStreamInfo->addColumn( tr( "S-Order" ) );
  lvStreamInfo->addColumn( tr( "D-Order" ) );
  lvStreamInfo->addColumn( tr( "Time-code" ) );

  lvStreamInfo->setSorting( 5, TRUE );

  gbStreamInfoLayout->addWidget( lvStreamInfo, 0, 0 );
  layout10->addWidget( gbStreamInfo );
  
  TTMpeg2MainWndLayout->addLayout( layout10, 1, 0 );
  
  // sequence info
  gbSequenceInfo = new Q3GroupBox( centralWidget(), "gbSequenceInfo" );
  gbSequenceInfo->setColumnLayout(0, Qt::Vertical );
  gbSequenceInfo->layout()->setSpacing( 6 );
  gbSequenceInfo->layout()->setMargin( 11 );
  gbSequenceInfoLayout = new QGridLayout( gbSequenceInfo->layout() );
  gbSequenceInfoLayout->setAlignment( Qt::AlignTop );
  
  layout19 = new QVBoxLayout( 0, 0, 6, "layout19"); 
  
  slBitrate = new QLabel( gbSequenceInfo, "slBitrate" );
  layout19->addWidget( slBitrate );
  
  slFramerate = new QLabel( gbSequenceInfo, "slFramerate" );
  layout19->addWidget( slFramerate );
  
  slWidth = new QLabel( gbSequenceInfo, "slWidth" );
  layout19->addWidget( slWidth );
  
  slHeight = new QLabel( gbSequenceInfo, "slHeight" );
  layout19->addWidget( slHeight );
  
  gbSequenceInfoLayout->addLayout( layout19, 0, 0 );
  
  layout20 = new QVBoxLayout( 0, 0, 6, "layout20"); 
   
  laSeqBitrate = new QLineEdit( gbSequenceInfo, "laSeqBitrate" );
  laSeqBitrate->setEnabled( FALSE );
  laSeqBitrate->setReadOnly( TRUE );
  layout20->addWidget( laSeqBitrate );
  
  laSeqFramerate = new QLineEdit( gbSequenceInfo, "laSeqFramerate" );
  laSeqFramerate->setEnabled( FALSE );
  laSeqFramerate->setReadOnly( TRUE );
  layout20->addWidget( laSeqFramerate );
  
  laSeqPicWidth = new QLineEdit( gbSequenceInfo, "laSeqPicWidth" );
  laSeqPicWidth->setEnabled( FALSE );
  laSeqPicWidth->setReadOnly( TRUE );
  layout20->addWidget( laSeqPicWidth );
  
  laSeqPicHeight = new QLineEdit( gbSequenceInfo, "laSeqPicHeight" );
  laSeqPicHeight->setEnabled( FALSE );
  laSeqPicHeight->setReadOnly( TRUE );
  layout20->addWidget( laSeqPicHeight );
  
  gbSequenceInfoLayout->addLayout( layout20, 0, 1 );
  
  TTMpeg2MainWndLayout->addWidget( gbSequenceInfo, 1, 1 );
  
  // scroll-bar
  // ---------------------------------------------------------------------------
  sbFrames = new QScrollBar( centralWidget(), "sbFrames" );
  sbFrames->setOrientation( Qt::Horizontal );
  
  TTMpeg2MainWndLayout->addMultiCellWidget( sbFrames, 2, 2, 0, 1 );
  
  // current frame info and navigation
  // ---------------------------------------------------------------------------
  frameInfo = new TTCurrentFrameInfo( centralWidget() , "FrameInfo" );

  TTMpeg2MainWndLayout->addMultiCellWidget( frameInfo, 3, 3, 0 ,1 );

  // ready
  languageChange();
  resize( QSize(963, 725).expandedTo(minimumSizeHint()) );
  //Qt4.0: clearWState( WState_Polished );

  // initialize some attributes
  // ---------------------------------------------------------------------------
  step_frame     = 0;
  step_order     = 0;
  is_stream_open = false;

  // show the video window
  // ---------------------------------------------------------------------------
  mpeg2_window = new TTMPEG2Window( 0 );
  mpeg2_window->show();
  mpeg2_window->resize( 720, 576 );

  // show the chart window
  // ---------------------------------------------------------------------------
  //chart_window = new TTFrameChart( 0, "CHARTWND", 420, 120 );
  //chart_window->show();

  // mpeg2 info window
  // ---------------------------------------------------------------------------
  //info_window = new TTMpeg2InfoView( 0, "INFOWND" );
  //info_window->show();

  // signals and slots connections
  // ---------------------------------------------------------------------------
  // file-menu
  connect(fileOpenAction,     SIGNAL( activated() ),   this, SLOT(fileOpen()));
  connect(fileAnalyzeAction,  SIGNAL( activated() ),   this, SLOT(fileAnalyze()));
  connect(fileSettingsAction, SIGNAL( activated() ),   this, SLOT(fileSettings()));
  connect(fileExitAction,     SIGNAL( activated() ),   this, SLOT(fileExit()));

  // tool-bar
  connect(tbStartAnalyzing, SIGNAL(clicked()), this, SLOT(fileAnalyze()));

  // open file button
  connect(pbOpenStream,       SIGNAL( clicked() ),     this,   SLOT( fileOpen()));

  // video position slider
  connect( sbFrames,  SIGNAL( valueChanged(int) ), this, SLOT( videoSliderChanged(int) ) );
  
  // current frame info / navigation
  connect(frameInfo,          SIGNAL(nextFrame()),     this, SLOT(nextFrameAction()));
  connect(frameInfo,          SIGNAL(prevFrame()),     this, SLOT(prevFrameAction()));
  connect(frameInfo,          SIGNAL(orderAnyFrame()), this, SLOT(orderAnyFrame()));
  connect(frameInfo,          SIGNAL(orderIFrame()),   this, SLOT(orderIFrame()));
  connect(frameInfo,          SIGNAL(orderPFrame()),   this, SLOT(orderPFrame()));
  connect(frameInfo,          SIGNAL(orderBFrame()),   this, SLOT(orderBFrame()));
  connect(frameInfo,          SIGNAL(orderDisplay()),  this, SLOT(orderDisplay()));
  connect(frameInfo,          SIGNAL(orderStream()),   this, SLOT(orderStream()));

}


TTMpeg2MainWnd::~TTMpeg2MainWnd()
{
  // no need to delete child widgets, Qt does it all for us
}

// Sets the strings of the subwidgets using the current language
void TTMpeg2MainWnd::languageChange()
{
  setCaption( tr( "TTMpeg2 - 0.10" ) );

  gbStreamName->setTitle( tr( "Video stream" ) );
  pbOpenStream->setText( QString::null );
  gbBufferStatistics->setTitle( tr( "Buffer statistics" ) );
  slFileLength->setText( tr( "File length (bytes):" ) );
  slReadOps->setText( tr( "Read operations:" ) );
  slFillOps->setText( tr( "Fill operations:" ) );
  slReadTime->setText( tr( "Read time:" ) );

  gbStreamStatistics->setTitle( tr( "Stream statistics" ) );
  slTotalFrames->setText( tr( "Frames total:" ) );
  slIFrames->setText( tr( "I-Frames:" ) );
  slPFrames->setText( tr( "P-Frames:" ) );
  slBFrames->setText( tr( "B-Frames:" ) );
  slSeqHeader->setText( tr( "Sequence header:" ) );
  slGOPHeader->setText( tr( "GOP header:" ) );
  slPicHeader->setText( tr( "Picture header:" ) );
  slSeqEndHeader->setText( tr( "Sequence end header:" ) );


  gbStreamInfo->setTitle( tr( "Stream info" ) );
  lvStreamInfo->header()->setLabel( 0, tr( "Header" ) );
  lvStreamInfo->header()->setLabel( 1, tr( "Offset" ) );
  lvStreamInfo->header()->setLabel( 2, tr( "Type" ) );
  lvStreamInfo->header()->setLabel( 3, tr( "Temp Ref." ) );
  lvStreamInfo->header()->setLabel( 4, tr( "S-Order" ) );
  lvStreamInfo->header()->setLabel( 5, tr( "D-Order" ) );
  lvStreamInfo->header()->setLabel( 6, tr( "Time-code" ) );
  lvStreamInfo->clear();

  gbSequenceInfo->setTitle( tr( "Sequence Info" ) );
  slBitrate->setText( tr( "Bitrate:" ) );
  slFramerate->setText( tr( "Framerate:" ) );
  slWidth->setText( tr( "Picture width:" ) );
  slHeight->setText( tr( "Picture height:" ) );

  //if (MenuBar->findItem(1))
  //MenuBar->findItem(1)->setText( tr( "File" ) );

  fileOpenAction->setText( tr( "Open..." ) );
  fileSettingsAction->setText( tr( "Settings..." ) );
  fileAnalyzeAction->setText( tr( "Analyze" ) );
  fileExitAction->setText( tr( "Exit" ) );

  setInitialValues();
}

// fill the information label with data
// -----------------------------------------------------------------------------
// buffer statistics
void TTMpeg2MainWnd::setFileLength( uint64_t length )
{
  str_tmp.sprintf("%lld", length);
  laFileLength->setText( str_tmp );
}


void TTMpeg2MainWnd::setReadOps( long ops )
{
  str_tmp.sprintf("%ld", ops);
  laReadOps->setText( str_tmp );
}

void TTMpeg2MainWnd::setReadOps( long ops, float percent )
{
  str_tmp.sprintf("%d - %4.2f%%",ops,percent);
  laReadOps->setText( str_tmp );
}


void TTMpeg2MainWnd::setFillOps( long ops )
{
  str_tmp.sprintf("%ld", ops);
  laFillOps->setText( str_tmp );
}


void TTMpeg2MainWnd::setReadTime( QTime time )
{
  str_tmp = time.toString( );
  laReadTime->setText( str_tmp );
}

// stream statistics
void TTMpeg2MainWnd::setNumFramesTotal( uint frames )
{
  str_tmp.sprintf("%d", frames);
  laFramesTotal->setText( str_tmp );
}


void TTMpeg2MainWnd::setNumIFrames( uint frames )
{
  str_tmp.sprintf("%d", frames);
  laIFrames->setText( str_tmp );
}


void TTMpeg2MainWnd::setNumPFrames( uint frames )
{
  str_tmp.sprintf("%d", frames);
  laPFrames->setText( str_tmp );
}


void TTMpeg2MainWnd::setNumBFrames( uint frames )
{
  str_tmp.sprintf("%d", frames);
  laBFrames->setText( str_tmp );
}


void TTMpeg2MainWnd::setNumSequence( uint num )
{
  str_tmp.sprintf("%d", num);
  laSequence->setText( str_tmp );
}


void TTMpeg2MainWnd::setNumGOP( uint num )
{
  str_tmp.sprintf("%d", num);
  laGOP->setText( str_tmp );
}


void TTMpeg2MainWnd::setNumPicture( uint num )
{
  str_tmp.sprintf("%d", num);
  laPicture->setText( str_tmp );
}


void TTMpeg2MainWnd::setNumSequenceEnd( uint num )
{
  str_tmp.sprintf("%d", num);
  laSequenceEnd->setText( str_tmp );
}

// sequence info
void TTMpeg2MainWnd::setBitrate( long rate )
{
  str_tmp.sprintf("%ld", rate);
  laSeqBitrate->setText( str_tmp );
}


void TTMpeg2MainWnd::setFramerate( float rate )
{
  str_tmp.sprintf("%2.2f", rate);
  laSeqFramerate->setText( str_tmp );
}


void TTMpeg2MainWnd::setPictureWidth( int width )
{
  str_tmp.sprintf("%d", width);
  laSeqPicWidth->setText( str_tmp );
}


void TTMpeg2MainWnd::setPictureHeight( int height )
{
  str_tmp.sprintf("%d", height);
  laSeqPicHeight->setText( str_tmp );
}


// slots
// -----------------------------------------------------------------------------
void TTMpeg2MainWnd::fileOpen()
{
  if ( !file_name.isEmpty() )
    {
      qDebug("%sreset project",cName);
      resetProject();
    }

  // get the video filename
  // get the video filename
  QString fn = QFileDialog::getOpenFileName( this,
					     "Open video file",
					     TTCut::lastDirPath,
					     "Video (*.m2v *.mpg)" );

  // input filename specified
  if ( !fn.isEmpty() )
    {
      qDebug( "%sopen file: %s",cName,fn.ascii() );

      file_name = fn;

      leStreamName->setText( file_name );
    }
}


void TTMpeg2MainWnd::fileAnalyze()
{
  qDebug("%sAnalyze stream action",cName);

  analyzeMpegStream();
}

void TTMpeg2MainWnd::fileSettings()
{
  qDebug("%sFile settings action",cName);

}

void TTMpeg2MainWnd::fileExit()
{
  qApp->quit();
}


void TTMpeg2MainWnd::startAnalyze()
{
  qDebug("%sAnalyze stream...",cName);

  analyzeMpegStream();
}


void TTMpeg2MainWnd::newPosition( int n_pos )
{
  qDebug("%sNew position action: %d",cName, n_pos);

}



// navigation
void TTMpeg2MainWnd::nextFrameAction()
{
  uint next_index;

  //qDebug("next frame: %d / %d",step_frame,step_order);

  switch ( step_frame )
    {
      // I-Frame
    case 1:
      next_index = video_stream->moveToNextIFrame();
      break;

      // P-Frame
    case 2:
      next_index = video_stream->moveToNextPFrame();
      break;

      // B-Frame
    case 3:
      next_index = video_stream->moveToNextFrame( 3 );
      break;

    default:
      next_index = video_stream->moveToNextFrame();
      break;
    }
  showFrame( next_index );
  slider_update = false;
  sbFrames->setValue( next_index );
}


void TTMpeg2MainWnd::prevFrameAction()
{
  uint prev_index;

  //qDebug("prev frame: %d / %d",step_frame,step_order);

  switch ( step_frame )
    {
      // I-Frame
    case 1:
      prev_index = video_stream->moveToPrevIFrame();
      break;

      // P-Frame
    case 2:
      prev_index = video_stream->moveToPrevPFrame();
      break;

      // B-Frame
    case 3:
      prev_index = video_stream->moveToPrevFrame( 3 );
      break;

    default:
      prev_index = video_stream->moveToPrevFrame();
      break;
    }
  showFrame( prev_index );
  slider_update = false;
  sbFrames->setValue( prev_index );
}

void TTMpeg2MainWnd::videoSliderChanged( int iSliderPos )
{
  uint slide_index;

  if ( slider_update )
  {
    slide_index = video_stream->moveToIndexPos( iSliderPos );
    showFrame( slide_index );
  }
  else
    slider_update = true;
}

void TTMpeg2MainWnd::orderAnyFrame()
{
  step_frame = 0;
}


void TTMpeg2MainWnd::orderIFrame()
{
  step_frame = 1;
}


void TTMpeg2MainWnd::orderPFrame()
{
  step_frame = 2;
}


void TTMpeg2MainWnd::orderBFrame()
{
  step_frame = 3;
}


void TTMpeg2MainWnd::orderDisplay()
{
  step_frame = 0;
  step_order = 0;

  // sort stream in display (decoder) order
  index_list->setDisplayOrder();    
  video_stream->setCurrentIndex( frameInfo->numDisplayOrder() );
}


void TTMpeg2MainWnd::orderStream()
{
  step_frame = 0;
  step_order = 1;

  // sort stream in stream order
  index_list->setStreamOrder();
  video_stream->setCurrentIndex( frameInfo->numStreamOrder() );
}


void TTMpeg2MainWnd::setInitialValues()
{
  // buffer statistic
  setFileLength( 0 );
  setReadOps( 0 );
  setFillOps( 0 );
  laReadTime->setText( "--:--:--" );
  
  // stream statistics
  setNumFramesTotal( 0 );
  setNumIFrames( 0 );
  setNumPFrames( 0 );
  setNumBFrames( 0 );
  setNumSequence( 0 );
  setNumGOP( 0 );
  setNumPicture( 0 );
  setNumSequenceEnd( 0 );

  // sequence info
  setBitrate( 0 );
  setFramerate( 0.0 );
  setPictureWidth( 0 );
  setPictureHeight( 0 );
}

void TTMpeg2MainWnd::resetProject()
{
  lvStreamInfo->clear();

  if ( ttAssigned(video_type) )
  delete video_type;


  setInitialValues();
}


void TTMpeg2MainWnd::analyzeMpegStream( )
{
  uint     max_print_frames = 2500;
  uint     i;
  long     num_frames = 0;
  QString  strHeaderType;
  QString  strHeaderOffset;
  QString  strPictureType;
  QString  strTempRef;
  QString  strDOrder;
  QString  strSOrder;
  QString  strTime;
  QString  strOut;
  Q3ListViewItem*      item;
  TTPicturesHeader*  current_picture;
  uint num_index;

  double             read_percent;

  // TODO: use QTime for timing purpose (have better resolution)
  time_t             seconds_start;
  time_t             seconds_end;

  num_frames = 0;

  // start time
  seconds_start = time( NULL );

  // open the mpeg2 stream
  if ( !file_name.isEmpty() )
  {
    // get the stream type and create according stream-object
    video_type   = new TTVideoType( file_name );

    // test
    audio_type = new TTAudioType( "rbb_test.mpa" );
    if ( audio_type->avStreamType() == TTAVTypes::mpeg_audio )
    {
      qDebug( "%sfound mpeg audio stream",cName );

      TTMPEGAudioStream* audio_stream = (TTMPEGAudioStream*)audio_type->createAudioStream();
      uint h_count = audio_stream->createHeaderList();
    }

    if ( video_type->avStreamType() == TTAVTypes::mpeg2_demuxed_video )
      video_stream = (TTMpeg2VideoStream*)video_type->createVideoStream();
    else
    {
      qDebug("%swrong video type",cName);
      return;
    }
 
    progress_bar = new TTProgressBar( this, "PBAR" );
    video_stream->setProgressBar( progress_bar );
    progress_bar->show();
    qApp->processEvents();

    // create header- and index-list
    video_stream->createHeaderList();
    num_index = video_stream->createIndexList();

    qDebug("%snum index: %d",cName,num_index);
    // get pointer to the lists
    index_list  = video_stream->indexList();
    header_list = video_stream->headerList();

    // get pointer to stream
    mpeg2_stream = video_stream->streamBuffer();

    // sort frame index list to display (decoder) order
    index_list->sortDisplayOrder();
    
    // get end time
    seconds_end = time( NULL );

    setReadTime( diffTime( seconds_start, seconds_end ) );

    // print out statistic informations
    // get stream length and set label text
    setFileLength( mpeg2_stream->streamLength() );

    read_percent = (double)mpeg2_stream->readCount()/(double)mpeg2_stream->streamLength()*100.0;

    setReadOps( mpeg2_stream->readCount(), read_percent );
    setFillOps( mpeg2_stream->fillCount() );

    // print out the number of frames in stream
    num_frames = index_list->count();
    total_time = ttFramesToTime( num_frames, 25.0 );

    setNumFramesTotal( num_frames );
    setNumIFrames( index_list->numIFrames() );
    setNumPFrames( index_list->numPFrames() );
    setNumBFrames( index_list->numBFrames() );

    setNumSequence( header_list->numSequenceHeader() );
    setNumPicture( header_list->numPictureHeader() );
    setNumGOP( header_list->numGopHeader() );
    setNumSequenceEnd( header_list->numSequenceEndHeader() );

    if ( video_stream->indexList()->count() < max_print_frames )
      max_print_frames = video_stream->indexList()->count();

    // print out the frame list; print maximal 1000 rows (!)
    for( i = 0; i < max_print_frames; i++ )
    {
      item = new Q3ListViewItem( lvStreamInfo );

      strDOrder.sprintf("%08ld",index_list->displayOrder(i));
      strSOrder.sprintf("%08ld",index_list->streamOrder(i));
      strHeaderType.sprintf("0x%02x",header_list->at(index_list->headerListIndex(i))->headerType());
      strHeaderOffset.sprintf("%12ld",header_list->at(index_list->headerListIndex(i))->headerOffset());
      
      current_picture = (TTPicturesHeader*)header_list->at(index_list->headerListIndex(i));
      
      switch(index_list->videoIndexAt(i)->picture_coding_type)
      {
      case(1):
	strPictureType.sprintf("I-Frame");
	strTempRef.sprintf("%04d",current_picture->temporal_reference);
	break;
      case(2):
	strPictureType.sprintf("P-Frame");
	strTempRef.sprintf("%04d",current_picture->temporal_reference);
	break;
      case(3):
	strPictureType.sprintf("B-Frame");
	strTempRef.sprintf("%04d",current_picture->temporal_reference);
	break;
      }
      num_frames++;
      
      item->setText( 0, strHeaderType );
      item->setText( 1, strHeaderOffset );
      item->setText( 2, strPictureType );
      item->setText( 3, strTempRef );
      item->setText( 4, strSOrder );
      item->setText( 5, strDOrder );
      strTime.sprintf("%s,%d",ttFramesToTime(index_list->displayOrder(i),25.0).toString().ascii(),
		      ttFramesToTime(index_list->displayOrder(i),25.0).msec());
      item->setText( 6, strTime );
    }

    // delete the progress bar
    delete progress_bar;
    video_stream->setProgressBar( NULL );

    sbFrames->setMinValue( 0 );
    sbFrames->setMaxValue( index_list->count()-1 );
    slider_update = false;
    sbFrames->setValue( 0 );

    // show information about the first frame
    showFrame( 0 );
    mpeg2_window->openVideoFile( file_name, 
    				 index_list, 
    				 header_list );
    mpeg2_window->moveToFirstFrame( );
    
  }
}


void TTMpeg2MainWnd::showFrame( uint f_index )
{
  TTPicturesHeader* current_picture;
  TTSequenceHeader* current_sequence;

try
 {
  frameInfo->setFrameOffset( header_list->at(index_list->headerListIndex(f_index))->headerOffset());
  
  current_picture = (TTPicturesHeader*)header_list->at(index_list->headerListIndex(f_index));

  //frameInfo->setNumDisplayOrder( index_list->displayOrder(f_index));
  frameInfo->setNumDisplayOrder( index_list->displayOrder(f_index),
				 current_picture->temporal_reference);

  frameInfo->setNumStreamOrder( index_list->streamOrder(f_index));
  //frameInfo->setNumStreamOrder( index_list->streamOrder(f_index),
  //                              current_picture->temporal_reference);
      
  frameInfo->setFrameType( index_list->videoIndexAt(f_index)->picture_coding_type );

  frameInfo->setFrameTime( ttFramesToTime(index_list->displayOrder(f_index),25.0),
			   total_time);

  frameInfo->setFrameSize( (long)index_list->frameSize( f_index ) );

  //frameInfo->setGOPNumber( index_list->gopNumber( f_index ) );
  frameInfo->setGOPNumber( index_list->gopNumber( f_index ),
  			   current_picture->temporal_reference);

  //current_sequence = header_list->sequenceHeaderAt( index_list->sequenceIndex( f_index ) );
  current_sequence = video_stream->currentSequenceHeader();

  setBitrate( current_sequence->bit_rate_value );
  setFramerate( current_sequence->frame_rate_code );
  setPictureWidth( current_sequence->horizontal_size_value );
  setPictureHeight( current_sequence->vertical_size_value );

  mpeg2_window->moveToVideoFrame( f_index );
 }
 catch(TTStreamEOFException)
 {
   qDebug("stream EOF exception...");
 }
 catch(TTListIndexException)
 {
   qDebug("index list exception...");
 }
 catch(...)
 {
   qDebug("unknown exception...");
 }
}


QTime TTMpeg2MainWnd::diffTime( time_t start, time_t end )
{
  QString strOut;
  int     hour,min,sec;
  time_t  seconds_diff;

  // times are in seconds (!)
  seconds_diff = end - start;

  // calculate time in hh:mm:ss
  hour = (int)(seconds_diff / 3600.0);
  min  = (int)((seconds_diff - hour*3600.0)/60.0 );
  sec  = (int)(seconds_diff - hour*3600.0 - min*60.0 );

  QTime res_time( hour, min, sec );

  return res_time;
}
// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// TTCurrentFrameInfo
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

TTCurrentFrameInfo::TTCurrentFrameInfo( QWidget* parent,  const char* name, Qt::WFlags fl )
  : QWidget( parent, name, fl )
{
  if ( !name )
    setName( "TTCurrentFrameInfo" );
  
  TTCut::imgIFrame  = new QPixmap( (const char**)i_frame_18_xpm );
  TTCut::imgPFrame  = new QPixmap( (const char**)p_frame_18_xpm );
  TTCut::imgBFrame  = new QPixmap( (const char**)b_frame_18_xpm );

  // main grid-layout  
  // ---------------------------------------------------------------------------
  TTCurrentFrameInfoLayout = new QGridLayout( this ); 
  TTCurrentFrameInfoLayout->setSpacing( 4 );
  TTCurrentFrameInfoLayout->setMargin( 0 );
  TTCurrentFrameInfoLayout->setColStretch( 0, 4 );
  TTCurrentFrameInfoLayout->setColStretch( 1, 1 );
  TTCurrentFrameInfoLayout->setColStretch( 2, 1 );

  // current frame information
  // ---------------------------------------------------------------------------
  gbCurrentFrameInfo = new Q3GroupBox( this, "gbCurrentFrameInfo" );
  gbCurrentFrameInfo->setTitle( tr( "Current Frame Info" ) );
  gbCurrentFrameInfo->setColumnLayout(0, Qt::Vertical );
  gbCurrentFrameInfo->layout()->setSpacing( 0 );
  gbCurrentFrameInfo->layout()->setMargin( 0 );
  gbCurrentFrameInfoLayout = new QGridLayout( gbCurrentFrameInfo->layout() );
  gbCurrentFrameInfoLayout->setAlignment( Qt::AlignTop );
  gbCurrentFrameInfoLayout->setSpacing( 6 );
  gbCurrentFrameInfoLayout->setMargin( 11 );

  laNumDisplayOrder = new QLabel( gbCurrentFrameInfo, "laNumDisplayOrder" );
  laNumDisplayOrder->setText( tr( "Number in display order" ) );

  gbCurrentFrameInfoLayout->addWidget( laNumDisplayOrder, 0, 0 );

  laNumStreamOrder = new QLabel( gbCurrentFrameInfo, "laNumStreamOrder" );
  laNumStreamOrder->setText( tr( "Number in stream order" ) );

  gbCurrentFrameInfoLayout->addWidget( laNumStreamOrder, 1, 0 );

  leNumDisplayOrder = new QLineEdit( gbCurrentFrameInfo, "leNumDisplayOrder" );
  //leNumDisplayOrder->setEnabled( FALSE );
  leNumDisplayOrder->setReadOnly( TRUE );
  leNumDisplayOrder->setFocusPolicy( Qt::NoFocus );
  leNumDisplayOrder->setCursorPosition( 0 );

  gbCurrentFrameInfoLayout->addWidget( leNumDisplayOrder, 0, 1 );

  leNumStreamOrder = new QLineEdit( gbCurrentFrameInfo, "leNumStreamOrder" );
  //leNumStreamOrder->setEnabled( FALSE );
  leNumStreamOrder->setReadOnly( TRUE );

  gbCurrentFrameInfoLayout->addWidget( leNumStreamOrder, 1, 1 );

  laFrameTime = new QLabel( gbCurrentFrameInfo, "laFrameTime" );
  laFrameTime->setText( tr( "Time" ) );

  gbCurrentFrameInfoLayout->addWidget( laFrameTime, 0, 2 );

  leFrameTime = new QLineEdit( gbCurrentFrameInfo, "leFrameTime" );
  //leFrameTime->setEnabled( FALSE );
  leFrameTime->setReadOnly( TRUE );

  gbCurrentFrameInfoLayout->addMultiCellWidget( leFrameTime, 0, 0, 3, 5 );

  laFrameType = new QLabel( gbCurrentFrameInfo, "laFrameType" );
  laFrameType->setText( tr( "Type" ) );

  gbCurrentFrameInfoLayout->addWidget( laFrameType, 0, 6 );

  viewFrameType = new QLabel( gbCurrentFrameInfo, "viewFrameType" );
  viewFrameType->setMinimumSize( QSize( 22, 22 ) );
  viewFrameType->setMaximumSize( QSize( 22, 22 ) );
  viewFrameType->setFrameShape( QLabel::Panel );
  viewFrameType->setFrameShadow( QLabel::Sunken );
  //viewFrameType->setPixmap( image0 );
  viewFrameType->setScaledContents( TRUE );
  viewFrameType->setAlignment( int( Qt::AlignCenter ) );

  gbCurrentFrameInfoLayout->addWidget( viewFrameType, 0, 7 );

  laFrameSize = new QLabel( gbCurrentFrameInfo, "laFrameSize" );
  laFrameSize->setText( tr( "Size" ) );

  gbCurrentFrameInfoLayout->addWidget( laFrameSize, 1, 2 );

  leFrameSize = new QLineEdit( gbCurrentFrameInfo, "leFrameSize" );
  //leFrameSize->setEnabled( FALSE );
  leFrameSize->setReadOnly( TRUE );

  gbCurrentFrameInfoLayout->addWidget( leFrameSize, 1, 3 );

  laFrameOffset = new QLabel( gbCurrentFrameInfo, "laFrameOffset" );
  laFrameOffset->setText( tr( "Offset" ) );

  gbCurrentFrameInfoLayout->addWidget( laFrameOffset, 1, 4 );

  leFrameOffset = new QLineEdit( gbCurrentFrameInfo, "leFrameOffset" );
  //leFrameOffset->setEnabled( FALSE );
  leFrameOffset->setReadOnly( TRUE );

  gbCurrentFrameInfoLayout->addMultiCellWidget( leFrameOffset, 1, 1, 5, 7 );

  TTCurrentFrameInfoLayout->addWidget( gbCurrentFrameInfo, 0, 0 );

  // GOP number
  // ---------------------------------------------------------------------------
  gbGOPNumber = new Q3GroupBox( this, "gbGOPNumber" );
  gbGOPNumber->setTitle( tr( "GOP Number" ) );
  gbGOPNumber->setColumnLayout(0, Qt::Vertical );
  gbGOPNumber->layout()->setSpacing( 0 );
  gbGOPNumber->layout()->setMargin( 0 );
  gbGOPNumberLayout = new QGridLayout( gbGOPNumber->layout() );
  gbGOPNumberLayout->setAlignment( Qt::AlignTop );
  gbGOPNumberLayout->setSpacing( 6 );
  gbGOPNumberLayout->setMargin( 11 );

  leGOPNumber = new QLineEdit( gbGOPNumber, "leGOPNumber" );
  //leGOPNumber->setEnabled( FALSE );
  leGOPNumber->setReadOnly( TRUE );

  gbGOPNumberLayout->addWidget( leGOPNumber, 0, 0 );

  TTCurrentFrameInfoLayout->addWidget( gbGOPNumber, 0, 1 );

  // navigation
  // ---------------------------------------------------------------------------
  gbNavigation = new Q3GroupBox( this, "gbNavigation" );
  gbNavigation->setTitle( tr( "Step by" ) );
  gbNavigation->setColumnLayout(0, Qt::Vertical );
  gbNavigation->layout()->setSpacing( 0 );
  gbNavigation->layout()->setMargin( 0 );
  gbNavigationLayout = new QGridLayout( gbNavigation->layout() );
  gbNavigationLayout->setAlignment( Qt::AlignTop );
  gbNavigationLayout->setSpacing( 2 );
  gbNavigationLayout->setMargin( 2 );

  Layout1 = new QHBoxLayout; 
  Layout1->setSpacing( 6 );
  Layout1->setMargin( 0 );

  pbPrevFrame = new QPushButton( gbNavigation, "pbPrevFrame" );
  pbPrevFrame->setMinimumSize( QSize( 24, 24 ) );
  pbPrevFrame->setMaximumSize( QSize( 24, 24 ) );
  pbPrevFrame->setText( tr( "<" ) );
  Layout1->addWidget( pbPrevFrame );

  pbNextFrame = new QPushButton( gbNavigation, "pbNextFrame" );
  pbNextFrame->setMinimumSize( QSize( 24, 24 ) );
  pbNextFrame->setMaximumSize( QSize( 24, 24 ) );
  pbNextFrame->setText( tr( ">" ) );
  Layout1->addWidget( pbNextFrame );

  gbNavigationLayout->addLayout( Layout1, 0, 0 );

  bgFrameOrder = new Q3ButtonGroup( gbNavigation, "bgFrameOrder" );
  bgFrameOrder->setFrameShape( QFrame::NoFrame );
  bgFrameOrder->setFrameShadow( QFrame::Plain );
  bgFrameOrder->setTitle( tr( "" ) );
  bgFrameOrder->setColumnLayout(0, Qt::Vertical );
  bgFrameOrder->layout()->setSpacing( 0 );
  bgFrameOrder->layout()->setMargin( 0 );
  bgFrameOrderLayout = new QGridLayout( bgFrameOrder->layout() );
  bgFrameOrderLayout->setAlignment( Qt::AlignTop );
  bgFrameOrderLayout->setSpacing( 2 );
  bgFrameOrderLayout->setMargin( 2 );

  rbIFrame = new QRadioButton( bgFrameOrder, "rbIFrame" );
  rbIFrame->setText( tr( "I" ) );
  bgFrameOrderLayout->addWidget( rbIFrame, 0, 0 );

  rbPFrame = new QRadioButton( bgFrameOrder, "rbPFrame" );
  rbPFrame->setText( tr( "P" ) );
  bgFrameOrderLayout->addMultiCellWidget( rbPFrame, 1, 2, 0, 0 );

  rbBFrame = new QRadioButton( bgFrameOrder, "rbBFrame" );
  rbBFrame->setText( tr( "B" ) );
  bgFrameOrderLayout->addWidget( rbBFrame, 3, 0 );

  rbDisplayOrder = new QRadioButton( bgFrameOrder, "rbDisplayOrder" );
  rbDisplayOrder->setText( tr( "Display order" ) );
  bgFrameOrderLayout->addMultiCellWidget( rbDisplayOrder, 0, 1, 1, 1 );

  rbStreamOrder = new QRadioButton( bgFrameOrder, "rbStreamOrder" );
  rbStreamOrder->setText( tr( "Stream order" ) );
  bgFrameOrderLayout->addMultiCellWidget( rbStreamOrder, 2, 3, 1, 1 );


  gbNavigationLayout->addWidget( bgFrameOrder, 0, 1 );

  TTCurrentFrameInfoLayout->addWidget( gbNavigation, 0, 2 );

  // signals and slots connection
  // ---------------------------------------------------------------------------
  connect(pbPrevFrame,  SIGNAL(clicked()),    this, SLOT(prevFrameAction()));
  connect(pbNextFrame,  SIGNAL(clicked()),    this, SLOT(nextFrameAction()));
  connect(bgFrameOrder, SIGNAL(clicked(int)), this, SLOT(stepOrderAction(int)));
}

/*  
 *  Destroys the object and frees any allocated resources
 */
TTCurrentFrameInfo::~TTCurrentFrameInfo()
{
  // no need to delete child widgets, Qt does it all for us
}

// slots
// -----------------------------------------------------------------------------
void TTCurrentFrameInfo::prevFrameAction()
{
  //qDebug("prev frame action");
  emit prevFrame();
}

void TTCurrentFrameInfo::nextFrameAction()
{
  //qDebug("next frame action");
  emit nextFrame();
}

void TTCurrentFrameInfo::stepOrderAction( int button_id )
{
  //qDebug("step order action: %d",button_id );

  switch ( button_id )
    {
    case 0:
      emit orderIFrame();
      break;
    case 1:
      emit orderPFrame();
      break;
    case 2:
      emit orderBFrame();
      break;
    case 3:
      emit orderDisplay();
      break;
    case 4:
      emit orderStream();
      break;
    }
}


void TTCurrentFrameInfo::setNumDisplayOrder( uint num )
{
  QString tmp_string;

  num_display_order = num;

  tmp_string.sprintf("%d",num);
  leNumDisplayOrder->setText( tmp_string );
}

void TTCurrentFrameInfo::setNumDisplayOrder( uint num, int temp_ref)
{
  QString tmp_string;

  num_display_order = num;

  tmp_string.sprintf("%d / %d",num, temp_ref);
  leNumDisplayOrder->setText( tmp_string );
}

uint TTCurrentFrameInfo::numDisplayOrder()
{
  return num_display_order;
}

void TTCurrentFrameInfo::setNumStreamOrder( uint num )
{
  QString tmp_string;

  num_stream_order = num;

  tmp_string.sprintf("%d",num);
  leNumStreamOrder->setText( tmp_string );
}

void TTCurrentFrameInfo::setNumStreamOrder( uint num, int temp_ref )
{
  QString tmp_string;

  num_stream_order = num;

  tmp_string.sprintf("%d / %d",num, temp_ref);
  leNumStreamOrder->setText( tmp_string );
}

uint TTCurrentFrameInfo::numStreamOrder()
{
  return num_stream_order;
}

void TTCurrentFrameInfo::setNumOrder( uint num_display, uint num_stream )
{
  setNumDisplayOrder( num_display );
  setNumStreamOrder( num_stream );
}


void TTCurrentFrameInfo::setFrameTime( QTime time_1, QTime time_2 )
{
  QString str_time;

  str_time.sprintf("%s,%d / %s,%d",time_1.toString().ascii(), time_1.msec(),
		   time_2.toString().ascii(), time_2.msec() );
  leFrameTime->setText( str_time );

}


void TTCurrentFrameInfo::setFrameTime( QTime time_1, int temp_ref )
{
  QString str_time;

  str_time.sprintf("%s / %d",time_1.toString().ascii(), temp_ref);
  leFrameTime->setText( str_time );
}

void TTCurrentFrameInfo::setFrameSize( long size )
{
  QString str_size;

  str_size.sprintf( "%ld", size );
  leFrameSize->setText( str_size );
}


void TTCurrentFrameInfo::setFrameType( int type )
{
  switch ( type )
    {
    case 1:
      viewFrameType->setPixmap( *(TTCut::imgIFrame) );
      //viewFrameType->setText( "I" );
      break;
    case 2:
      viewFrameType->setPixmap( *(TTCut::imgPFrame) );
      //viewFrameType->setText( "P" );
      break;
    case 3:
      viewFrameType->setPixmap( *(TTCut::imgBFrame) );
      //viewFrameType->setText( "B" );
      break;
    }
}


//void TTCurrentFrameInfo::setFrameOffset( off64_t offset );
void TTCurrentFrameInfo::setFrameOffset( long offset )
{
  QString str_offset;

  str_offset.sprintf("%ld",offset);
  leFrameOffset->setText( str_offset );
}


void TTCurrentFrameInfo::setGOPNumber( uint gop_num )
{
  QString str_gop;

  str_gop.sprintf("%d",gop_num);
  leGOPNumber->setText( str_gop );
}

void TTCurrentFrameInfo::setGOPNumber( uint gop_num, int temp_ref )
{
  QString str_gop;

  str_gop.sprintf("%d / %d",gop_num, temp_ref);
  leGOPNumber->setText( str_gop );
}


