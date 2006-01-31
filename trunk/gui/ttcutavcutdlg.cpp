/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttcutavcutdlg.cpp                                               */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 04/01/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTCUTAVCUTDLG
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


#include "ttcutavcutdlg.h"

#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qcheckbox.h>
#include <q3groupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include <qtooltip.h>
#include <q3whatsthis.h>
#include <qcombobox.h>
#include <qradiobutton.h>
#include <q3buttongroup.h>

//Added by qt3to4:
#include <QApplication>
#include <QProcess>
#include <QFileDialog>
#include <Q3Frame>
#include <QHBoxLayout>
#include <QGridLayout>

const char c_name[] = "TTAVCUTDIALOG : ";

// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// TTCut A/V cut dialog
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////
TTCutAVCutDlg::TTCutAVCutDlg( QWidget* parent,  const char* name, bool modal, Qt::WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "TTCutAVCutDlg" );

    resize( 619, 395 );
    setCaption( tr( "Cut video/audio" ) );

    // Dialog layout manager
    TTCutAVCutDlgLayout = new QGridLayout( this );
    TTCutAVCutDlgLayout->setSpacing( 6 );
    TTCutAVCutDlgLayout->setMargin( 11 );

    // the tab widget
    twAVCut = new QTabWidget( this, "twAVCut" );

    // common A/V cut settings
    // -------------------------------------------------------------------------
    commonTab = new TTCutAVCutCommonTab( 0, "tab" );
    twAVCut->insertTab( commonTab, tr( "Common" ) );

    // encoder settings tab
    // ---------------------------------------------------------------
    encoderTab = new TTCutEncoderSettings( 0, "EncoderTab" );
    twAVCut->insertTab( encoderTab, tr( "Encoding" ) );

    // muxer settings tab
    // ---------------------------------------------------------------
    muxerTab = new TTCutMuxerSettings( 0, "MuxerTab" );
    twAVCut->insertTab( muxerTab, tr( "Muxing" ) );

    // chapter settings tab
    // ---------------------------------------------------------------
    chapterTab = new TTCutChapterSettings( 0, "ChapterTab" );
    twAVCut->insertTab( chapterTab, tr( "Chapters" ) );

    // add tab widget to central layout
    TTCutAVCutDlgLayout->addWidget( twAVCut, 0, 0 );

    // spacer
    Layout1 = new QHBoxLayout;
    Layout1->setSpacing( 6 );
    Layout1->setMargin( 0 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( spacer );

    // button start, cancel
    // -------------------------------------------------------------------------
    pbStart = new QPushButton( this, "pbStart" );
    pbStart->setText( tr( "Start" ) );
    Layout1->addWidget( pbStart );

    pbCancel = new QPushButton( this, "pbCancel" );
    pbCancel->setText( tr( "Cancel" ) );
    Layout1->addWidget( pbCancel );

    TTCutAVCutDlgLayout->addLayout( Layout1, 1, 0 );

    // signals and slot connection
    // ------------------------------------------------------------------
    connect( pbStart,  SIGNAL( clicked() ),  SLOT( onDlgStart() ) );
    connect( pbCancel, SIGNAL( clicked() ),  SLOT( onDlgCancel() ) );

    // set the tabs data
    // ------------------------------------------------------------------
    commonTab->setTabData();
    encoderTab->setTabData();
    muxerTab->setTabData();
    chapterTab->setTabData();

}

// destruct object
// -----------------------------------------------------------------------------
TTCutAVCutDlg::~TTCutAVCutDlg()
{

}


// save the tabs data
void TTCutAVCutDlg::setGlobalData()
{
    commonTab->getTabData();
    encoderTab->getTabData();
    muxerTab->getTabData();
    chapterTab->getTabData();
}


// exit, saving changes; start A/V cut
// -----------------------------------------------------------------------------
void TTCutAVCutDlg::onDlgStart()
{
   setGlobalData();

   done( 0 );
}


// exit, discard changes
// -----------------------------------------------------------------------------
void TTCutAVCutDlg::onDlgCancel()
{
   done( 1 );
}



// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// TTCut A/V cut common cut settings tab
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// construct object
// -----------------------------------------------------------------------------
TTCutAVCutCommonTab::TTCutAVCutCommonTab( QWidget* parent,  const char* name, Qt::WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "TTCutAVCutCommonTab" );
    resize( 627, 380 );
    setCaption( tr( "Form2" ) );
    TTCutAVCutCommonTabLayout = new QGridLayout( this );
    TTCutAVCutCommonTabLayout->setSpacing( 6 );
    TTCutAVCutCommonTabLayout->setMargin( 11 );

    Layout2 = new QGridLayout;
    Layout2->setSpacing( 6 );
    Layout2->setMargin( 0 );

    leOutputFile = new QLineEdit( this, "leOutputFile" );

    Layout2->addMultiCellWidget( leOutputFile, 0, 0, 1, 2 );

    laOutPath = new QLabel( this, "laOutPath" );
    laOutPath->setText( tr( "Ouput path:" ) );

    Layout2->addWidget( laOutPath, 1, 0 );

    leOutputPath = new QLineEdit( this, "leOutputPath" );

    Layout2->addWidget( leOutputPath, 1, 1 );

    laOutFile = new QLabel( this, "laOutFile" );
    laOutFile->setText( tr( "Output file:" ) );

    Layout2->addWidget( laOutFile, 0, 0 );

    pbSelectPath = new QPushButton( this, "pbSelectPath" );
    pbSelectPath->setMinimumSize( QSize( 30, 30 ) );
    pbSelectPath->setMaximumSize( QSize( 30, 30 ) );
    pbSelectPath->setPixmap( *(TTCut::imgFileOpen24) );

    Layout2->addWidget( pbSelectPath, 1, 2 );

    TTCutAVCutCommonTabLayout->addLayout( Layout2, 0, 0 );

    Layout3 = new QGridLayout( 0, 1, 1, 11, 6, "layout3");

    laFreeSpace = new QLabel( this, "laFreeSpace" );
    laFreeSpace->setText( tr( "Free drive space:" ) );
    Layout3->addWidget( laFreeSpace, 0, 0 );

    textFreeDriveSpace1 = new QLabel( this, "textFreeDriveSpace" );
    textFreeDriveSpace1->setText( tr( " " ) );
    Layout3->addWidget( textFreeDriveSpace1, 0, 1);

    textFreeDriveSpace2 = new QLabel( this, "textFreeDriveSpace" );
    textFreeDriveSpace2->setText( tr( "No information available" ) );
    Layout3->addWidget( textFreeDriveSpace2, 1, 1 );

    TTCutAVCutCommonTabLayout->addLayout( Layout3, 1, 0 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    TTCutAVCutCommonTabLayout->addItem( spacer, 4, 0 );

    gbCutOptions = new Q3GroupBox( this, "gbCutOptions" );
    gbCutOptions->setFrameShape( Q3GroupBox::Box );
    gbCutOptions->setTitle( tr( "Cut options" ) );
    gbCutOptions->setColumnLayout(0, Qt::Vertical );
    gbCutOptions->layout()->setSpacing( 0 );
    gbCutOptions->layout()->setMargin( 0 );
    gbCutOptionsLayout = new QGridLayout( gbCutOptions->layout() );
    gbCutOptionsLayout->setAlignment( Qt::AlignTop );
    gbCutOptionsLayout->setSpacing( 6 );
    gbCutOptionsLayout->setMargin( 11 );

    cbMaxBitrate = new QCheckBox( gbCutOptions, "cbMaxBitrate" );
    cbMaxBitrate->setText( tr( "Write max bitrate to first sequence" ) );

    gbCutOptionsLayout->addWidget( cbMaxBitrate, 0, 0 );

    cbWriteSeqEnd = new QCheckBox( gbCutOptions, "cbWriteSeqEnd" );
    cbWriteSeqEnd->setText( tr( "Write Sequence End Code" ) );

    gbCutOptionsLayout->addWidget( cbWriteSeqEnd, 1, 0 );

    TTCutAVCutCommonTabLayout->addWidget( gbCutOptions, 3, 0 );

    Frame4 = new Q3Frame( this, "Frame4" );
    Frame4->setMinimumSize( QSize( 100, 15 ) );
    Frame4->setMaximumSize( QSize( 32767, 15 ) );
    Frame4->setFrameShape( Q3Frame::NoFrame );
    Frame4->setFrameShadow( Q3Frame::Raised );
    Frame4->setMargin( 2 );

    TTCutAVCutCommonTabLayout->addWidget( Frame4, 2, 0 );

    connect( pbSelectPath, SIGNAL( clicked() ), this, SLOT( selectCutDirAction() ) );
}

// destruct object
// -----------------------------------------------------------------------------
TTCutAVCutCommonTab::~TTCutAVCutCommonTab()
{
    // no need to delete child widgets, Qt does it all for us
}

// select a directory for the cut result
void TTCutAVCutCommonTab::selectCutDirAction()
{
  QStringList df_cmd_list;

  QString str_dir = QFileDialog::getExistingDirectory( this,
						       "Select cut-result directory",
						       TTCut::cutDirPath,
						       (QFileDialog::DontResolveSymlinks |
							QFileDialog::ShowDirsOnly) );

  if ( !str_dir.isEmpty() )
  {
    TTCut::cutDirPath = str_dir;

    leOutputPath->setText( TTCut::cutDirPath );

    qApp->processEvents();
  }

  // TODO: Straingth interaction with QSettings ???? Something's wrong here
  // get free disk space
  dfOutput2 = "No information available";
  //dfProc = new QProcess( );

  //df_cmd_list.clear();

  //df_cmd_list << "-h"
  //	      << TTCut::cutDirPath;

  //dfProc->start( "df", df_cmd_list );

  //connect(dfProc, SIGNAL( readyRead() ),SLOT( readFromStdout() ) );
  //connect(dfProc, SIGNAL( finished(int) ),  SLOT( exitProcess(int) ) );

  //dfProc->waitForFinished();

  //delete dfProc;
}


// set the tab data from global parameter
// -----------------------------------------------------------------------------
void TTCutAVCutCommonTab::setTabData()
{
  QStringList df_cmd_list;

  if ( !QDir(TTCut::cutDirPath).exists() )
    TTCut::cutDirPath = QDir::currentPath();

  // cut output filename and output path
  leOutputFile->setText( TTCut::cutVideoName );
  leOutputPath->setText( TTCut::cutDirPath );

  // cut options
  // write max bittrate tp first sequence
  if ( TTCut::cutWriteMaxBitrate )
    cbMaxBitrate->setChecked( true );
  else
    cbMaxBitrate->setChecked( false );

  // write sequence end code
  if ( TTCut::cutWriteSeqEnd )
    cbWriteSeqEnd->setChecked( true );
  else
    cbWriteSeqEnd->setChecked( false );

  // TODO: core dumb !!!
  // get free disk space
  dfOutput2 = "No information available";
  //dfProc = new QProcess( );

  //df_cmd_list.clear();

  //df_cmd_list << "-h"
  //	      << TTCut::cutDirPath;

  //dfProc->start( "df", df_cmd_list );

  //connect(dfProc, SIGNAL( readyRead() ),SLOT( readFromStdout() ) );
  //connect(dfProc, SIGNAL( finished(int) ),  SLOT( exitProcess(int) ) );

  //dfProc->waitForFinished();

  //delete dfProc;
}

// get tab data and set global parameter
// -----------------------------------------------------------------------------
void TTCutAVCutCommonTab::getTabData()
{
  // cut output filename and output path
  TTCut::cutVideoName = leOutputFile->text();
  TTCut::cutDirPath   = leOutputPath->text();

  if ( !QDir(TTCut::cutDirPath).exists() )
    TTCut::cutDirPath = QDir::currentPath();

  // cut options
  // write max bittrate tp first sequence
  if ( cbMaxBitrate->isChecked() )
    TTCut::cutWriteMaxBitrate = true;
  else
    TTCut::cutWriteMaxBitrate = false;

  // write sequence end code
  if ( cbWriteSeqEnd->isChecked() )
    TTCut::cutWriteSeqEnd = true;
  else
    TTCut::cutWriteSeqEnd = false;
}


void TTCutAVCutCommonTab::readFromStdout()
{
  char       temp_str[101];
  int        i, i_pos, i_line;
  QString    line;
  QByteArray ba;

  ba = dfProc->readAll();

  i_pos  = 0;
  i_line = 0;

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
      line = QString::fromLocal8Bit( temp_str );;

      if ( i_line == 0 )
	textFreeDriveSpace1->setText( line );
      else
	textFreeDriveSpace2->setText( line );

      i_pos = 0;
      i_line++;
    }
  }
  qApp->processEvents();
}

void TTCutAVCutCommonTab::exitProcess( __attribute__ ((unused))int e_code )
{
  qApp->processEvents();
}
