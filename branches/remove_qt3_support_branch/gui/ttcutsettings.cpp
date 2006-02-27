/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttcutsettings.cpp                                               */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 03/01/2005 */
/* MODIFIED: b. altendorf                                    DATE: 03/23/2005 */
/* MODIFIED: b. altendorf                                    DATE: 04/05/2005 */
/* MODIFIED: b. altendorf                                    DATE: 08/09/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTCUTSETTINGS
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


#include "ttcutsettings.h"

#include <qcheckbox.h>
#include <q3groupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
//#include <q3whatsthis.h>
#include <qcombobox.h>
#include <qradiobutton.h>
#include <qtabwidget.h>
#include <q3buttongroup.h>

//Added by qt3to4:
#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// TTCut settings object
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////
TTCutSettings::TTCutSettings()
  : QSettings("TriTime", "TTCut")
{

}


TTCutSettings::~TTCutSettings()
{

}


void TTCutSettings::readSettings()
{
  // read application settings
  // ---------------------------------------------------------------------------
  // Navigation settings
  // ---------------------------------------------------------------------------
  beginGroup( "/Settings" );

  beginGroup( "/Navigation" );
  TTCut::fastSlider      = value( "FastSlider/", TTCut::fastSlider ).toBool();
  TTCut::stepSliderClick = value( "StepSliderClick/", TTCut::stepSliderClick ).toInt();
  TTCut::stepPgUpDown    = value( "StepPgUpDown/",TTCut::stepPgUpDown ).toInt();
  TTCut::stepArrowKeys   = value( "StepArrowKeys/",TTCut::stepArrowKeys ).toInt();
  TTCut::stepPlusAlt     = value( "StepPlusAlt/", TTCut::stepPlusAlt ).toInt();
  TTCut::stepPlusCtrl    = value( "StepPlusCtrl/", TTCut::stepPlusCtrl ).toInt();
  TTCut::stepQuickJump   = value( "StepQuickJump/", TTCut::stepQuickJump ).toInt();
  TTCut::stepMouseWheel  = value( "StepMouseWheel/", TTCut::stepMouseWheel ).toInt();
  endGroup();

  // Common options
  // ---------------------------------------------------------------------------
  beginGroup( "/Common" );
  TTCut::tempDirPath = value( "TempDirPath/", TTCut::tempDirPath ).toString();
  TTCut::lastDirPath = value( "LastDirPath/", TTCut::lastDirPath ).toString();
  endGroup();

  // Preview
  // ---------------------------------------------------------------------------
  beginGroup( "/Preview" );
  TTCut::cutPreviewSeconds = value( "PreviewSeconds/", TTCut::cutPreviewSeconds ).toInt();
  TTCut::playSkipFrames    = value( "SkipFrames/", TTCut::playSkipFrames ).toInt();
  endGroup();

  // Search
  // ---------------------------------------------------------------------------
  beginGroup( "/Search" );
  TTCut::searchLength   = value( "Length/", TTCut::searchLength ).toInt();
  TTCut::searchAccuracy = value( "Accuracy/", TTCut::searchAccuracy ).toInt();
  endGroup();

  // Index files
  // ---------------------------------------------------------------------------
  beginGroup( "/IndexFiles" );
  TTCut::createVideoIDD = value( "CreateVideoIDD/", TTCut::createVideoIDD ).toBool();
  TTCut::createAudioIDD = value( "CreateAudioIDD/", TTCut::createAudioIDD ).toBool();
  TTCut::createPrevIDD  = value( "CreatePrevIDD/", TTCut::createPrevIDD ).toBool();
  TTCut::createD2V      = value( "CreateD2V/", TTCut::createD2V ).toBool();
  TTCut::readVideoIDD   = value( "ReadVideoIDD/", TTCut::readVideoIDD ).toBool();
  TTCut::readAudioIDD   = value( "ReadAudioIDD", TTCut::readAudioIDD ).toBool();
  TTCut::readPrevIDD    = value( "ReadPrevIDD/", TTCut::readPrevIDD ).toBool();
  endGroup();

  // Encoder settings
  // ---------------------------------------------------------------------------
  beginGroup( "/Encoder" );
  TTCut::encoderMode = value( "EncoderMode/", TTCut::encoderMode ).toBool();
  endGroup();

  // Muxer settings
  // ---------------------------------------------------------------------------
  beginGroup( "/Muxer" );
  TTCut::muxMode       = value( "MuxMode/", TTCut::muxMode ).toInt();
  TTCut::mpeg2Target   = value( "Mpeg2Target/", TTCut::mpeg2Target ).toInt();
  TTCut::muxProg       = value( "MuxProg/", TTCut::muxProg ).toString();
  TTCut::muxProgPath   = value( "MuxProgPath/", TTCut::muxProgPath ).toString();
  TTCut::muxProgCmd    = value( "MuxProgCmd/", TTCut::muxProgCmd ).toString();
  TTCut::muxOutputPath = value( "MuxOutputDir/", TTCut::muxOutputPath ).toString();
  endGroup();

  // Chapter settings
  // ---------------------------------------------------------------------------
  beginGroup( "/Chapter" );
  TTCut::spumuxChapter = value( "SpumuxChapter/", TTCut::spumuxChapter ).toBool();
  endGroup();

  // Cut option
  // ---------------------------------------------------------------------------
  beginGroup( "/CutOptions" );
  TTCut::cutDirPath         = value( "DirPath/", TTCut::cutDirPath ).toString();
  TTCut::cutVideoName       = value( "VideoName/", TTCut::cutVideoName ).toString();
  TTCut::cutWriteMaxBitrate = value( "WriteMaxBitrate/", TTCut::cutWriteMaxBitrate ).toBool();
  TTCut::cutWriteSeqEnd     = value( "WriteSeqEnd/", TTCut::cutWriteSeqEnd ).toBool();
  TTCut::correctCutTimeCode = value( "CorrectTimeCode/", TTCut::correctCutTimeCode ).toBool();
  TTCut::correctCutBitRate  = value( "CorrectBitrate/", TTCut::correctCutBitRate ).toBool();
  TTCut::createCutIDD       = value( "CreateIDD/", TTCut::createCutIDD ).toBool();
  TTCut::readCutIDD         = value( "ReadIDD/", TTCut::readCutIDD ).toBool();
  endGroup();

  endGroup();

  // check temporary path; we must ensure we have a temporary directory
  // the temporary directory is used for the preview clips and for
  // the temporary avi-clips
  if ( !QDir( TTCut::tempDirPath ).exists() )
    TTCut::tempDirPath = QDir::tempPath();

  // check the cut directory path
  if ( !QDir( TTCut::cutDirPath ).exists() )
    TTCut::cutDirPath = QDir::currentPath();
}


void TTCutSettings::writeSettings()
{
  beginGroup( "/Settings" );

  // Navigation settings
  // ---------------------------------------------------------------------------
  beginGroup( "/Navigation" );
  setValue( "FastSlider/", TTCut::fastSlider );
  setValue( "StepSliderClick/", TTCut::stepSliderClick );
  setValue( "StepPgUpDown/", TTCut::stepPgUpDown );
  setValue( "StepArrowKeys/", TTCut::stepArrowKeys );
  setValue( "StepPlusAlt/", TTCut::stepPlusAlt );
  setValue( "StepPlusCtrl/",TTCut::stepPlusCtrl );
  setValue( "StepQuickJump/", TTCut::stepQuickJump );
  setValue( "StepMouseWheel/", TTCut::stepMouseWheel );
  endGroup();

  // Common options
  // ---------------------------------------------------------------------------
  beginGroup( "/Common" );
  setValue( "TempDirPath/" , TTCut::tempDirPath );
  setValue( "LastDirPath/" , TTCut::lastDirPath );
  endGroup();

  // Preview
  // ---------------------------------------------------------------------------
  beginGroup( "/Preview" );
  setValue( "PreviewSeconds/",  TTCut::cutPreviewSeconds );
  setValue( "SkipFrames/",      TTCut::playSkipFrames );
  endGroup();

  // Search
  // ---------------------------------------------------------------------------
  beginGroup( "/Search" );
  setValue( "Length/",          TTCut::searchLength );
  setValue( "Accuracy/",        TTCut::searchAccuracy );
  endGroup();

  // Index files
  // ---------------------------------------------------------------------------
  beginGroup( "/IndexFiles" );
  setValue( "CreateVideoIDD/",  TTCut::createVideoIDD );
  setValue( "CreateAudioIDD/",  TTCut::createAudioIDD );
  setValue( "CreatePrevIDD/",   TTCut::createPrevIDD );
  setValue( "CreateD2V/",       TTCut::createD2V );
  setValue( "ReadVideoIDD/",    TTCut::readVideoIDD );
  setValue( "ReadAudioIDD",     TTCut::readAudioIDD );
  setValue( "ReadPrevIDD/",     TTCut::readPrevIDD );
  endGroup();

  // Encoder settings
  // ---------------------------------------------------------------------------
  beginGroup( "/Encoder" );
  setValue( "EncoderMode/",     TTCut::encoderMode );
  endGroup();

  // Muxer settings
  // ---------------------------------------------------------------------------
  beginGroup( "/Muxer" );
  setValue( "MuxMode/",         TTCut::muxMode );
  setValue( "Mpeg2Target/",     TTCut::mpeg2Target );
  setValue( "MuxProg/",         TTCut::muxProg );
  setValue( "MuxProgPath/",     TTCut::muxProgPath );
  setValue( "MuxProgCmd/",      TTCut::muxProgCmd );
  setValue( "MuxOutputDir/",    TTCut::muxOutputPath );
  endGroup();

  // Chapter settings
  // ---------------------------------------------------------------------------
  beginGroup( "/Chapter" );
  setValue( "SpumuxChapter/",   TTCut::spumuxChapter );
  endGroup();

  // Cut option
  // ---------------------------------------------------------------------------
  beginGroup( "/CutOptions" );
  setValue( "DirPath/",         TTCut::cutDirPath );
  setValue( "VideoName/",       TTCut::cutVideoName );
  setValue( "WriteMaxBitrate/", TTCut::cutWriteMaxBitrate );
  setValue( "WriteSeqEnd/",     TTCut::cutWriteSeqEnd );
  setValue( "CorrectTimeCode/", TTCut::correctCutTimeCode );
  setValue( "CorrectBitrate/",  TTCut::correctCutBitRate );
  setValue( "CreateIDD/",       TTCut::createCutIDD );
  setValue( "ReadIDD/",         TTCut::readCutIDD );
  endGroup();

  endGroup();
}


// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// TTCut settings dialog; Container for the setting tabs
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////
TTCutSettingsDlg::TTCutSettingsDlg( QWidget* parent, Qt::WFlags fl )
    : QDialog( parent, fl )
{
  setModal( true );

  setName( "TTCutSettingsDlg" );
  resize( 596, 480 );

  setCaption( tr( "Settings" ) );

  // Dialog layout manager
  TTCutSettingsDlgLayout = new QGridLayout( this );
  TTCutSettingsDlgLayout->setSpacing( 6 );
  TTCutSettingsDlgLayout->setMargin( 11 );

  // the tab widget
  TTSettingsTab = new QTabWidget( this, "TTSettingsTab" );
  TTSettingsTab->setEnabled( TRUE );

  // common settings tab
  // ---------------------------------------------------------------
  commonTab = new TTCutCommonSettings( 0, "CommonTab" );
  TTSettingsTab->insertTab( commonTab, tr( "Common" ) );

  // files settings tab
  // ---------------------------------------------------------------
  filesTab = new TTCutFilesSettings( 0, "filesTab" );
  TTSettingsTab->insertTab( filesTab, tr( "Files" ) );

  // encoder settings tab
  // ---------------------------------------------------------------
  encoderTab = new TTCutEncoderSettings( 0, "EncoderTab" );
  TTSettingsTab->insertTab( encoderTab, tr( "Encoding" ) );

  // muxer settings tab
  // ---------------------------------------------------------------
  muxerTab = new TTCutMuxerSettings( 0, "MuxerTab" );
  TTSettingsTab->insertTab( muxerTab, tr( "Muxing" ) );

  // chapter settings tab
  // ---------------------------------------------------------------
  chapterTab = new TTCutChapterSettings( 0, "ChapterTab" );
  TTSettingsTab->insertTab( chapterTab, tr( "Chapters" ) );

  // add tab widget to central layout
  TTCutSettingsDlgLayout->addWidget( TTSettingsTab, 0, 0 );

  // button ok, cancel
  // ---------------------------------------------------------------
  Layout1 = new QHBoxLayout;
  Layout1->setSpacing( 6 );
  Layout1->setMargin( 0 );
  QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
  Layout1->addItem( spacer );

  btnOk = new QPushButton( this, "BtnOk" );
  btnOk->setText( tr( "&Ok" ) );
  Layout1->addWidget( btnOk );

  btnCancel = new QPushButton( this, "BtnCancel" );
  btnCancel->setText( tr( "&Cancel" ) );
  Layout1->addWidget( btnCancel );

  TTCutSettingsDlgLayout->addLayout( Layout1, 1, 0 );

  // signals and slot connection
  // ------------------------------------------------------------------
  connect( btnOk,     SIGNAL( clicked() ),  SLOT( onDlgOk() ) );
  connect( btnCancel, SIGNAL( clicked() ),  SLOT( onDlgCancel() ) );

  // set the tabs data
  // ------------------------------------------------------------------
  commonTab->setTabData();
  filesTab->setTabData();
  encoderTab->setTabData();
  muxerTab->setTabData();
  chapterTab->setTabData();
}

TTCutSettingsDlg::~TTCutSettingsDlg()
{

}


// save the tabs data
void TTCutSettingsDlg::setGlobalData()
{
    commonTab->getTabData();
    filesTab->getTabData();
    encoderTab->getTabData();
    muxerTab->getTabData();
    chapterTab->getTabData();
}


// exit, saving changes
void TTCutSettingsDlg::onDlgOk()
{
   setGlobalData();

   done( 0 );
}


// exit, discard changes
void TTCutSettingsDlg::onDlgCancel()
{
   done( 1 );
}


// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// TTCut common settings tab
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// constructor
TTCutCommonSettings::TTCutCommonSettings( QWidget* parent,  const char* name, Qt::WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "TTCutCommonSettings" );

    resize( 577, 328 );

    setCaption( tr( "Common" ) );
    TTCutCommonSettingsLayout = new QGridLayout( this );
    TTCutCommonSettingsLayout->setSpacing( 6 );
    TTCutCommonSettingsLayout->setMargin( 11 );

    gbNavigation = new Q3GroupBox( this, "gbNavigation" );
    gbNavigation->setTitle( tr( "Navigation" ) );
    gbNavigation->setColumnLayout(0, Qt::Vertical );
    gbNavigation->layout()->setSpacing( 0 );
    gbNavigation->layout()->setMargin( 0 );
    gbNavigationLayout = new QGridLayout( gbNavigation->layout() );
    gbNavigationLayout->setAlignment( Qt::AlignTop );
    gbNavigationLayout->setSpacing( 6 );
    gbNavigationLayout->setMargin( 11 );

    cbQuickSearch = new QCheckBox( gbNavigation, "cbQuickSearch" );
    cbQuickSearch->setText( tr( "Use quick seek (slider positioned only on I-Frames)" ) );

    gbNavigationLayout->addWidget( cbQuickSearch, 1, 0 );

    Layout4 = new QHBoxLayout;
    Layout4->setSpacing( 6 );
    Layout4->setMargin( 0 );

    Layout1 = new QVBoxLayout;
    Layout1->setSpacing( 6 );
    Layout1->setMargin( 0 );

    laSliderClick = new QLabel( gbNavigation, "laSliderClick" );
    laSliderClick->setText( tr( "Slider click placement" ) );
    Layout1->addWidget( laSliderClick );

    laPgUpDown = new QLabel( gbNavigation, "laPgUpDown" );
    laPgUpDown->setText( tr( "Page up/down placement" ) );
    Layout1->addWidget( laPgUpDown );

    laArrowPlacement = new QLabel( gbNavigation, "laArrowPlacement" );
    laArrowPlacement->setText( tr( "Arrow keys placement" ) );
    Layout1->addWidget( laArrowPlacement );

    laAltJump = new QLabel( gbNavigation, "laAltJump" );
    laAltJump->setText( tr( "'Alt' key jump distance" ) );
    Layout1->addWidget( laAltJump );

    laCtrlJump = new QLabel( gbNavigation, "laCtrlJump" );
    laCtrlJump->setText( tr( "'Ctrl' key jump distance" ) );
    Layout1->addWidget( laCtrlJump );

    laQuickJump = new QLabel( gbNavigation, "laQuickJump" );
    laQuickJump->setText( tr( "Quick jump distance" ) );
    Layout1->addWidget( laQuickJump );

    laMouseWheel = new QLabel( gbNavigation, "laMouseWheel" );
    laMouseWheel->setText( tr( "Mouse wheel distance" ) );
    Layout1->addWidget( laMouseWheel );
    Layout4->addLayout( Layout1 );

    Layout2 = new QVBoxLayout;
    Layout2->setSpacing( 6 );
    Layout2->setMargin( 0 );

    // TODO: add apropriate values for min/max values and step

    sbSliderClickPlacement = new QSpinBox( gbNavigation, "sbSliderClickPlacement" );
    sbSliderClickPlacement->setMaxValue( 200 );
    sbSliderClickPlacement->setMinValue( 5 );
    sbSliderClickPlacement->setLineStep( 5 );
    Layout2->addWidget( sbSliderClickPlacement );

    sbPgUpDown = new QSpinBox( gbNavigation, "sbPgUpDown" );
    sbPgUpDown->setMaxValue( 200 );
    sbPgUpDown->setMinValue( 5 );
    sbPgUpDown->setLineStep( 5 );
    Layout2->addWidget( sbPgUpDown );

    sbArrowKeyPlacement = new QSpinBox( gbNavigation, "sbArrowKeyPlacement" );
    sbArrowKeyPlacement->setMaxValue( 40 );
    sbArrowKeyPlacement->setMinValue( 1 );
    sbArrowKeyPlacement->setLineStep( 1 );
    Layout2->addWidget( sbArrowKeyPlacement );

    sbAltDistance = new QSpinBox( gbNavigation, "sbAltDistance" );
    sbAltDistance->setMaxValue( 200 );
    sbAltDistance->setMinValue( 5 );
    sbAltDistance->setLineStep( 5 );
    Layout2->addWidget( sbAltDistance );

    sbCtrlDistance = new QSpinBox( gbNavigation, "sbCtrlDistance" );
    sbCtrlDistance->setMaxValue( 200 );
    sbCtrlDistance->setMinValue( 5 );
    sbCtrlDistance->setLineStep( 5 );
    Layout2->addWidget( sbCtrlDistance );

    sbQuickJumpDistance = new QSpinBox( gbNavigation, "sbQuickJumpDistance" );
    sbQuickJumpDistance->setMaxValue( 200 );
    sbQuickJumpDistance->setMinValue( 5 );
    sbQuickJumpDistance->setLineStep( 5 );
    Layout2->addWidget( sbQuickJumpDistance );

    sbMouseWheel = new QSpinBox( gbNavigation, "sbMouseWheel" );
    sbMouseWheel->setMaxValue( 200 );
    sbMouseWheel->setMinValue( 5 );
    sbMouseWheel->setLineStep( 5 );
    Layout2->addWidget( sbMouseWheel );
    Layout4->addLayout( Layout2 );

    Layout3 = new QVBoxLayout;
    Layout3->setSpacing( 6 );
    Layout3->setMargin( 0 );

    laFrames1 = new QLabel( gbNavigation, "laFrames1" );
    laFrames1->setText( tr( "frames" ) );
    Layout3->addWidget( laFrames1 );

    laFrames2 = new QLabel( gbNavigation, "laFrames2" );
    laFrames2->setText( tr( "frames" ) );
    Layout3->addWidget( laFrames2 );

    laFrames3 = new QLabel( gbNavigation, "laFrames3" );
    laFrames3->setText( tr( "frames" ) );
    Layout3->addWidget( laFrames3 );

    laFrames4 = new QLabel( gbNavigation, "laFrames4" );
    laFrames4->setText( tr( "frames" ) );
    Layout3->addWidget( laFrames4 );

    laFrames5 = new QLabel( gbNavigation, "laFrames5" );
    laFrames5->setText( tr( "frames" ) );
    Layout3->addWidget( laFrames5 );

    laSeconds1 = new QLabel( gbNavigation, "laSeconds1" );
    laSeconds1->setText( tr( "seconds" ) );
    Layout3->addWidget( laSeconds1 );

    laFrames7 = new QLabel( gbNavigation, "laFrames6" );
    laFrames7->setText( tr( "frames" ) );
    Layout3->addWidget( laFrames7 );
    Layout4->addLayout( Layout3 );

    gbNavigationLayout->addLayout( Layout4, 0, 0 );

    TTCutCommonSettingsLayout->addMultiCellWidget( gbNavigation, 0, 1, 0, 0 );

    gbSearchPlay = new Q3GroupBox( this, "gbSearchPlay" );
    gbSearchPlay->setTitle( tr( "Search/Play" ) );
    gbSearchPlay->setColumnLayout(0, Qt::Vertical );
    gbSearchPlay->layout()->setSpacing( 0 );
    gbSearchPlay->layout()->setMargin( 0 );
    gbSearchPlayLayout = new QGridLayout( gbSearchPlay->layout() );
    gbSearchPlayLayout->setAlignment( Qt::AlignTop );
    gbSearchPlayLayout->setSpacing( 6 );
    gbSearchPlayLayout->setMargin( 11 );

    Layout5 = new QVBoxLayout;
    Layout5->setSpacing( 6 );
    Layout5->setMargin( 0 );

    laSearch = new QLabel( gbSearchPlay, "laSearch" );
    laSearch->setText( tr( "Search CutOut Frame within" ) );
    Layout5->addWidget( laSearch );

    laPreview = new QLabel( gbSearchPlay, "laPreview" );
    laPreview->setText( tr( "Preview each Cut" ) );
    Layout5->addWidget( laPreview );

    laSkip = new QLabel( gbSearchPlay, "laSkip" );
    laSkip->setText( tr( "Skip while playing" ) );
    Layout5->addWidget( laSkip );

    gbSearchPlayLayout->addLayout( Layout5, 0, 0 );

    Layout6 = new QVBoxLayout;
    Layout6->setSpacing( 6 );
    Layout6->setMargin( 0 );

    sbSearchIntervall = new QSpinBox( gbSearchPlay, "sbSearchIntervall" );
    sbSearchIntervall->setMaxValue( 200 );
    sbSearchIntervall->setMinValue( 5 );
    sbSearchIntervall->setLineStep( 5 );
    Layout6->addWidget( sbSearchIntervall );

    spPreviewLength = new QSpinBox( gbSearchPlay, "spPreviewLength" );
    spPreviewLength->setMaxValue( 200 );
    spPreviewLength->setMinValue( 5 );
    spPreviewLength->setLineStep( 5 );
    Layout6->addWidget( spPreviewLength );

    sbSkipFrames = new QSpinBox( gbSearchPlay, "sbSkipFrames" );
    sbSkipFrames->setMaxValue( 200 );
    sbSkipFrames->setMinValue( 5 );
    sbSkipFrames->setLineStep( 5 );
    Layout6->addWidget( sbSkipFrames );

    gbSearchPlayLayout->addLayout( Layout6, 0, 1 );

    Layout7 = new QVBoxLayout;
    Layout7->setSpacing( 6 );
    Layout7->setMargin( 0 );

    laSeconds2 = new QLabel( gbSearchPlay, "laSeconds2" );
    laSeconds2->setText( tr( "seconds" ) );
    Layout7->addWidget( laSeconds2 );

    laSeconds3 = new QLabel( gbSearchPlay, "laSeconds3" );
    laSeconds3->setText( tr( "seconds" ) );
    Layout7->addWidget( laSeconds3 );

    laFrame6 = new QLabel( gbSearchPlay, "laFrame6" );
    laFrame6->setText( tr( "frames" ) );
    Layout7->addWidget( laFrame6 );

    gbSearchPlayLayout->addLayout( Layout7, 0, 2 );

    TTCutCommonSettingsLayout->addWidget( gbSearchPlay, 0, 1 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    TTCutCommonSettingsLayout->addItem( spacer, 1, 1 );

    gbFilesDirs = new Q3GroupBox( this, "gbFilesDirs" );
    gbFilesDirs->setTitle( tr( "Files/Directories" ) );
    gbFilesDirs->setColumnLayout(0, Qt::Vertical );
    gbFilesDirs->layout()->setSpacing( 0 );
    gbFilesDirs->layout()->setMargin( 0 );
    gbFilesDirsLayout = new QGridLayout( gbFilesDirs->layout() );
    gbFilesDirsLayout->setAlignment( Qt::AlignTop );
    gbFilesDirsLayout->setSpacing( 6 );
    gbFilesDirsLayout->setMargin( 11 );

    laTempDir = new QLabel( gbFilesDirs, "laTempDir" );
    laTempDir->setText( tr( "Temp directory" ) );

    gbFilesDirsLayout->addWidget( laTempDir, 0, 0 );

    leTempDirectory = new QLineEdit( gbFilesDirs, "leTempDirectory" );

    gbFilesDirsLayout->addWidget( leTempDirectory, 0, 1 );

    btnDirOpen = new QPushButton( gbFilesDirs, "btnDirOpen" );
    btnDirOpen->setMinimumSize( QSize( 30, 30 ) );
    btnDirOpen->setMaximumSize( QSize( 30, 30 ) );
    //FIXME: btnDirOpen->setPixmap( *(TTCut::imgFileOpen24) );

    gbFilesDirsLayout->addWidget( btnDirOpen, 0, 2 );

    TTCutCommonSettingsLayout->addMultiCellWidget( gbFilesDirs, 3, 3, 0, 1 );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    TTCutCommonSettingsLayout->addItem( spacer_2, 2, 0 );

    connect( btnDirOpen, SIGNAL( clicked() ), this, SLOT( selectTempDirAction() ) );
}

// destructor
TTCutCommonSettings::~TTCutCommonSettings()
{
    // no need to delete child widgets, Qt does it all for us
}

// select a temp directory path
void TTCutCommonSettings::selectTempDirAction()
{
  QString str_dir = QFileDialog::getExistingDirectory( this,
						       "Select temporary directory",
						       TTCut::tempDirPath,
						       (QFileDialog::DontResolveSymlinks |
							QFileDialog::ShowDirsOnly) );

  if ( !str_dir.isEmpty() )
  {
    TTCut::tempDirPath = str_dir;

    leTempDirectory->setText( TTCut::tempDirPath );
  }
}

// set the tab data from the global parameter
void TTCutCommonSettings::setTabData()
{

// --------------------------------------------------------------
// common settings
// --------------------------------------------------------------

// Navigation
  sbSliderClickPlacement->setValue( TTCut::stepSliderClick );
  sbPgUpDown->setValue( TTCut::stepPgUpDown );
  sbArrowKeyPlacement->setValue( TTCut::stepArrowKeys );
  sbAltDistance->setValue( TTCut::stepPlusAlt );
  sbCtrlDistance->setValue( TTCut::stepPlusCtrl );
  sbQuickJumpDistance->setValue( TTCut::stepQuickJump );
  sbMouseWheel->setValue( TTCut::stepMouseWheel );

// Preview
  spPreviewLength->setValue( TTCut::cutPreviewSeconds );

// Frame search
  sbSearchIntervall->setValue( TTCut::searchLength );
  sbSkipFrames->setValue( TTCut::playSkipFrames );

// Options, directories
  if ( TTCut::fastSlider )
    cbQuickSearch->setChecked( true );
  else
    cbQuickSearch->setChecked( false );

  leTempDirectory->setText( TTCut::tempDirPath );
}

// get the tab data and fill the global parameter
void TTCutCommonSettings::getTabData()
{
// Navigation
  TTCut::stepSliderClick = sbSliderClickPlacement->value( );
  TTCut::stepPgUpDown    = sbPgUpDown->value( );
  TTCut::stepArrowKeys   = sbArrowKeyPlacement->value( );
  TTCut::stepPlusAlt     = sbAltDistance->value( );
  TTCut::stepPlusCtrl    = sbCtrlDistance->value( );
  TTCut::stepQuickJump   = sbQuickJumpDistance->value( );
  TTCut::stepMouseWheel  = sbMouseWheel->value( );

// Preview
  TTCut::cutPreviewSeconds = spPreviewLength->value( );

// Frame search
  TTCut::searchLength   = sbSearchIntervall->value( );
  TTCut::playSkipFrames = sbSkipFrames->value( );

// Options, directories
  if ( cbQuickSearch->isChecked() )
    TTCut::fastSlider = true;
  else
    TTCut::fastSlider = false;

  TTCut::tempDirPath = leTempDirectory->text( );

  if ( !QDir(TTCut::tempDirPath).exists() )
    TTCut::tempDirPath = QDir::tempPath();
}



// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// TTCut files settings tab
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// constructor

TTCutFilesSettings::TTCutFilesSettings( QWidget* parent,  const char* name, Qt::WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "TTCutFilesSettings" );

    resize( 536, 263 );

    // Main layout
    // -------------------------------------------------------------------------
    TTCutFilesSettingsLayout = new QGridLayout( this );
    TTCutFilesSettingsLayout->setSpacing( 6 );
    TTCutFilesSettingsLayout->setMargin( 11 );

    // MPEG2Schnitt index files (*.idd)
    // -------------------------------------------------------------------------
    cbCreateCutVideoIDD = new Q3GroupBox( this, "cbCreateCutVideoIDD" );
    cbCreateCutVideoIDD->setGeometry( QRect( 10, 10, 254, 137 ) );
    cbCreateCutVideoIDD->setTitle( tr( "MPEG2Schnitt Index Files (*.idd)" ) );
    cbCreateCutVideoIDD->setColumnLayout(0, Qt::Vertical );
    cbCreateCutVideoIDD->layout()->setSpacing( 0 );
    cbCreateCutVideoIDD->layout()->setMargin( 0 );
    cbCreateCutVideoIDDLayout = new QGridLayout( cbCreateCutVideoIDD->layout() );
    cbCreateCutVideoIDDLayout->setAlignment( Qt::AlignTop );
    cbCreateCutVideoIDDLayout->setSpacing( 6 );
    cbCreateCutVideoIDDLayout->setMargin( 11 );

    cbCreateVideoIDD = new QCheckBox( cbCreateCutVideoIDD, "cbCreateVideoIDD" );
    cbCreateVideoIDD->setText( tr( "Create video index file" ) );

    cbCreateCutVideoIDDLayout->addWidget( cbCreateVideoIDD, 0, 0 );

    cbReadVideoIDD = new QCheckBox( cbCreateCutVideoIDD, "cbReadVideoIDD" );
    cbReadVideoIDD->setText( tr( "Read video index file" ) );

    cbCreateCutVideoIDDLayout->addWidget( cbReadVideoIDD, 1, 0 );

    cbCreateAudioIDD = new QCheckBox( cbCreateCutVideoIDD, "cbCreateAudioIDD" );
    cbCreateAudioIDD->setText( tr( "Create audio index file" ) );

    cbCreateCutVideoIDDLayout->addWidget( cbCreateAudioIDD, 2, 0 );

    cbReadAudioIDD = new QCheckBox( cbCreateCutVideoIDD, "cbReadAudioIDD" );
    cbReadAudioIDD->setText( tr( "Read audio index file" ) );

    cbCreateCutVideoIDDLayout->addWidget( cbReadAudioIDD, 3, 0 );

    CheckBox9 = new QCheckBox( cbCreateCutVideoIDD, "CheckBox9" );
    CheckBox9->setText( tr( "Create cut video index file" ) );

    cbCreateCutVideoIDDLayout->addWidget( CheckBox9, 4, 0 );
    TTCutFilesSettingsLayout->addWidget( cbCreateCutVideoIDD, 0, 0 );

    // Logfile and logfile options
    // -------------------------------------------------------------------------
    gbLogfile = new Q3GroupBox( this, "gbLogfile" );
    gbLogfile->setGeometry( QRect( 270, 10, 237, 119 ) );
    gbLogfile->setTitle( tr( "Logfile" ) );
    gbLogfile->setColumnLayout(0, Qt::Vertical );
    gbLogfile->layout()->setSpacing( 0 );
    gbLogfile->layout()->setMargin( 0 );
    gbLogfileLayout = new QGridLayout( gbLogfile->layout() );
    gbLogfileLayout->setAlignment( Qt::AlignTop );
    gbLogfileLayout->setSpacing( 6 );
    gbLogfileLayout->setMargin( 11 );

    gbLogfileOptions = new Q3GroupBox( gbLogfile, "gbLogfileOptions" );
    gbLogfileOptions->setTitle( tr( "Logfile Options" ) );
    gbLogfileOptions->setColumnLayout(0, Qt::Vertical );
    gbLogfileOptions->layout()->setSpacing( 0 );
    gbLogfileOptions->layout()->setMargin( 0 );
    gbLogfileOptionsLayout = new QGridLayout( gbLogfileOptions->layout() );
    gbLogfileOptionsLayout->setAlignment( Qt::AlignTop );
    gbLogfileOptionsLayout->setSpacing( 6 );
    gbLogfileOptionsLayout->setMargin( 11 );

    cbLogPlusVideoIndex = new QCheckBox( gbLogfileOptions, "cbLogPlusVideoIndex" );
    cbLogPlusVideoIndex->setText( tr( "Video index information" ) );

    gbLogfileOptionsLayout->addWidget( cbLogPlusVideoIndex, 1, 0 );

    cbLogExtended = new QCheckBox( gbLogfileOptions, "cbLogExtended" );
    cbLogExtended->setText( tr( "Extended messages and information" ) );

    gbLogfileOptionsLayout->addWidget( cbLogExtended, 0, 0 );

    gbLogfileLayout->addWidget( gbLogfileOptions, 1, 0 );

    cbCreateLog = new QCheckBox( gbLogfile, "cbCreateLog" );
    cbCreateLog->setText( tr( "Create logfile" ) );

    gbLogfileLayout->addWidget( cbCreateLog, 0, 0 );

    TTCutFilesSettingsLayout->addWidget( gbLogfile, 0, 1 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    TTCutFilesSettingsLayout->addItem( spacer, 1, 0 );
}

// destruct object
TTCutFilesSettings::~TTCutFilesSettings()
{
    // no need to delete child widgets, Qt does it all for us
}

// set the tab data from the global parameter
void TTCutFilesSettings::setTabData()
{
  cbCreateVideoIDD->setChecked( TTCut::createVideoIDD );
  cbCreateAudioIDD->setChecked( TTCut::createAudioIDD );
  cbReadVideoIDD->setChecked( TTCut::readVideoIDD );
  cbReadAudioIDD->setChecked( TTCut::readAudioIDD );

  cbCreateLog->setChecked( TTCut::createLogFile );
  cbLogExtended->setChecked( TTCut::logModeExtended );
  cbLogPlusVideoIndex->setChecked( TTCut::logVideoIndexInfo );
}

// get the tab data and fill the global parameter
void TTCutFilesSettings::getTabData()
{
  TTCut::createVideoIDD    = cbCreateVideoIDD->isChecked( );
  TTCut::createAudioIDD    = cbCreateAudioIDD->isChecked( );
  TTCut::readVideoIDD      = cbReadVideoIDD->isChecked( );
  TTCut::readAudioIDD      = cbReadAudioIDD->isChecked( );

  TTCut::createLogFile     = cbCreateLog->isChecked( );
  TTCut::logModeExtended   = cbLogExtended->isChecked( );
  TTCut::logVideoIndexInfo = cbLogPlusVideoIndex->isChecked( );
}

// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// TTCut encoding settings tab
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// constructor
TTCutEncoderSettings::TTCutEncoderSettings( QWidget* parent,  const char* name, Qt::WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "TTCutEncoderSettings" );
    resize( 596, 370 );
    setCaption( tr( "Form1" ) );

    cbEncodingMode = new QCheckBox( this, "cbEncodingMode" );
    cbEncodingMode->setGeometry( QRect( 10, 20, 200, 20 ) );
    cbEncodingMode->setText( tr( "Use encoding mode" ) );
}

// destructor
TTCutEncoderSettings::~TTCutEncoderSettings()
{
    // no need to delete child widgets, Qt does it all for us
}


// set the tab data from the global parameter
void TTCutEncoderSettings::setTabData()
{
  cbEncodingMode->setChecked( TTCut::encoderMode );
}

// get the tab data and fill the global parameter
void TTCutEncoderSettings::getTabData()
{
  if ( cbEncodingMode->isChecked() )
    TTCut::encoderMode = true;
  else
    TTCut::encoderMode = false;
}



// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// TTCut muxer settings tab
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// constructor
TTCutMuxerSettings::TTCutMuxerSettings( QWidget* parent,  const char* name, Qt::WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "TTCutMuxerSettings" );
    resize( 596, 370 );
    setCaption( tr( "Form2" ) );
    TTCutMuxerSettingsLayout = new QGridLayout( this );
    TTCutMuxerSettingsLayout->setSpacing( 6 );
    TTCutMuxerSettingsLayout->setMargin( 11 );

    bgMuxOptions = new Q3ButtonGroup( this, "bgMuxOptions" );
    bgMuxOptions->setTitle( tr( "Mux options" ) );
    bgMuxOptions->setColumnLayout(0, Qt::Vertical );
    bgMuxOptions->layout()->setSpacing( 0 );
    bgMuxOptions->layout()->setMargin( 0 );
    bgMuxOptionsLayout = new QGridLayout( bgMuxOptions->layout() );
    bgMuxOptionsLayout->setAlignment( Qt::AlignTop );
    bgMuxOptionsLayout->setSpacing( 6 );
    bgMuxOptionsLayout->setMargin( 11 );


    rbMuxNone = new QRadioButton( bgMuxOptions, "rbMuxNone" );
    rbMuxNone->setText( tr( "No muxing" ) );

    bgMuxOptionsLayout->addWidget( rbMuxNone, 0, 2 );

    rbCreateMuxScript = new QRadioButton( bgMuxOptions, "rbCreateMuxScript" );
    rbCreateMuxScript->setText( tr( "Create mux script for batch muxing" ) );

    bgMuxOptionsLayout->addWidget( rbCreateMuxScript, 0, 1 );

    rbMuxStreams = new QRadioButton( bgMuxOptions, "rbMuxStreams" );
    rbMuxStreams->setText( tr( "Finally mux result streams" ) );

    bgMuxOptionsLayout->addWidget( rbMuxStreams, 0, 0 );



    TTCutMuxerSettingsLayout->addWidget( bgMuxOptions, 0, 0 );


    gbMuxerSettings = new Q3GroupBox( this, "gbMuxerSettings" );
    gbMuxerSettings->setTitle( tr( "Muxer settings" ) );
    gbMuxerSettings->setColumnLayout(0, Qt::Vertical );
    gbMuxerSettings->layout()->setSpacing( 0 );
    gbMuxerSettings->layout()->setMargin( 0 );
    gbMuxerSettingsLayout = new QGridLayout( gbMuxerSettings->layout() );
    gbMuxerSettingsLayout->setAlignment( Qt::AlignTop );
    gbMuxerSettingsLayout->setSpacing( 6 );
    gbMuxerSettingsLayout->setMargin( 11 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    gbMuxerSettingsLayout->addMultiCell( spacer, 1, 1, 4, 5 );

    laMuxTarget = new QLabel( gbMuxerSettings, "laMuxTarget" );
    laMuxTarget->setText( tr( "MPEG-2 target" ) );

    gbMuxerSettingsLayout->addWidget( laMuxTarget, 0, 0 );

    cbMuxTarget = new QComboBox( FALSE, gbMuxerSettings, "cbMuxTarget" );

    gbMuxerSettingsLayout->addWidget( cbMuxTarget, 0, 1 );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    gbMuxerSettingsLayout->addItem( spacer_2, 0, 2 );

    laOutputPath = new QLabel( gbMuxerSettings, "laOutputPath" );
    laOutputPath->setText( tr( "Output path" ) );

    gbMuxerSettingsLayout->addWidget( laOutputPath, 2, 0 );

    cbMuxerProg = new QComboBox( FALSE, gbMuxerSettings, "cbMuxerProg" );

    gbMuxerSettingsLayout->addMultiCellWidget( cbMuxerProg, 1, 1, 1, 2 );

    laMuxerProg = new QLabel( gbMuxerSettings, "laMuxerProg" );
    laMuxerProg->setText( tr( "Used muxer" ) );

    gbMuxerSettingsLayout->addWidget( laMuxerProg, 1, 0 );

    pbConfigureMuxer = new QPushButton( gbMuxerSettings, "pbConfigureMuxer" );
    pbConfigureMuxer->setMinimumSize( QSize( 0, 0 ) );
    pbConfigureMuxer->setMaximumSize( QSize( 32767, 32767 ) );
    pbConfigureMuxer->setText( tr( "configure..." ) );
    //FIXME: pbConfigureMuxer->setIconSet( QIcon( *(TTCut::imgSettings18) ) );

    gbMuxerSettingsLayout->addWidget( pbConfigureMuxer, 1, 3 );

    btnOutputPath = new QPushButton( gbMuxerSettings, "btnOutputPath" );
    btnOutputPath->setMinimumSize( QSize( 30, 30 ) );
    btnOutputPath->setMaximumSize( QSize( 30, 30 ) );
    //FIXME: btnOutputPath->setPixmap( *(TTCut::imgFileOpen24) );

    gbMuxerSettingsLayout->addWidget( btnOutputPath, 2, 5 );

    leOutputPath = new QLineEdit( gbMuxerSettings, "leOutputPath" );

    gbMuxerSettingsLayout->addMultiCellWidget( leOutputPath, 2, 2, 1, 4 );

    TTCutMuxerSettingsLayout->addWidget( gbMuxerSettings, 1, 0 );

    gbPostMuxing = new Q3GroupBox( this, "gbPostMuxing" );
    gbPostMuxing->setTitle( tr( "post muxing action" ) );
    gbPostMuxing->setColumnLayout(0, Qt::Vertical );
    gbPostMuxing->layout()->setSpacing( 0 );
    gbPostMuxing->layout()->setMargin( 0 );
    gbPostMuxingLayout = new QGridLayout( gbPostMuxing->layout() );
    gbPostMuxingLayout->setAlignment( Qt::AlignTop );
    gbPostMuxingLayout->setSpacing( 6 );
    gbPostMuxingLayout->setMargin( 11 );

    cbPause = new QCheckBox( gbPostMuxing, "cbPause" );
    cbPause->setText( tr( "Pause" ) );

    gbPostMuxingLayout->addWidget( cbPause, 0, 0 );

    cbDeleteES = new QCheckBox( gbPostMuxing, "cbDeleteES" );
    cbDeleteES->setText( tr( "Delete elemtary streams" ) );

    gbPostMuxingLayout->addWidget( cbDeleteES, 1, 0 );

    TTCutMuxerSettingsLayout->addWidget( gbPostMuxing, 2, 0 );
    QSpacerItem* spacer_3 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    TTCutMuxerSettingsLayout->addItem( spacer_3, 3, 0 );
}

// destructor
TTCutMuxerSettings::~TTCutMuxerSettings()
{
   // no need to delete child widgets, Qt does it all for us
}


// set the tab data from the global parameter
void TTCutMuxerSettings::setTabData()
{

}

// get the tab data and fill the global parameter
void TTCutMuxerSettings::getTabData()
{

}



// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// TTCut chapter settings tab
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////
TTCutChapterSettings::TTCutChapterSettings( QWidget* parent,  const char* name, Qt::WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "TTCutChapterSettings" );
    resize( 596, 480 );
    setCaption( tr( "Form3" ) );
    TTCutChapterSettingsLayout = new QGridLayout( this );
    TTCutChapterSettingsLayout->setSpacing( 6 );
    TTCutChapterSettingsLayout->setMargin( 11 );

    cbSpumuxChapter = new QCheckBox( this, "cbSpumuxChapter" );
    cbSpumuxChapter->setText( tr( "Write spumux (XML) chapter-list" ) );

    TTCutChapterSettingsLayout->addWidget( cbSpumuxChapter, 0, 0 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    TTCutChapterSettingsLayout->addItem( spacer, 1, 0 );
}


TTCutChapterSettings::~TTCutChapterSettings()
{
    // no need to delete child widgets, Qt does it all for us
}


// set the tab data from the global parameter
void TTCutChapterSettings::setTabData()
{

}

// get the tab data and fill the global parameter
void TTCutChapterSettings::getTabData()
{

}







