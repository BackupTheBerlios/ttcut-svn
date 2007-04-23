/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2006 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttcutavcutdlg.cpp                                               */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 04/01/2005 */
/* MODIFIED: b. altendorf                                    DATE: 03/05/2006 */
/* MODIFIED: b. altendorf                                    DATE: 04/24/2007 */
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

#include <QFileDialog>
#include <QProcess>
#include <QDir>

const char c_name[] = "TTCutAVCutDlg";

/* /////////////////////////////////////////////////////////////////////////////
 * Constructor
 */
TTCutAVCutDlg::TTCutAVCutDlg(QWidget* parent)
    : QDialog(parent)
{
  setupUi(this);

  // not implemented yet
  tabWidget->removeTab(3);

  // message logger instance
  log = TTMessageLogger::getInstance();

  // signals and slot connection
  // ------------------------------------------------------------------
  connect(btnDirOpen,   SIGNAL(clicked()),           SLOT(onDirectoryOpen()));
  connect(okButton,     SIGNAL(clicked()),           SLOT( onDlgStart()));
  connect(cancelButton, SIGNAL(clicked()),           SLOT( onDlgCancel()));
  connect(tabWidget,    SIGNAL(currentChanged(int)), SLOT(onTabChanged(int)));

  // set the tabs data
  // ------------------------------------------------------------------
  setCommonData();
  encodingPage->setTabData();
  muxingPage->setTabData();
  //chaptersPage->setTabData();
}

/* /////////////////////////////////////////////////////////////////////////////
 * Destructor
 */
TTCutAVCutDlg::~TTCutAVCutDlg()
{
  // nothing to do
}


/* /////////////////////////////////////////////////////////////////////////////
 * Save the tabs data
 */
void TTCutAVCutDlg::setGlobalData()
{
  getCommonData();
  encodingPage->getTabData();
  muxingPage->getTabData();
  //chaptersPage->getTabData();
}


/* /////////////////////////////////////////////////////////////////////////////
 * Exit, saving changes; start A/V cut
 */
void TTCutAVCutDlg::onDlgStart()
{
  setGlobalData();

  done( 0 );
}


/* /////////////////////////////////////////////////////////////////////////////
 * Exit, discard changes
 */
void TTCutAVCutDlg::onDlgCancel()
{
  done( 1 );
}

/* /////////////////////////////////////////////////////////////////////////////
 * Select a directory for the cut result
 */
void TTCutAVCutDlg::onDirectoryOpen()
{
  QString str_dir = QFileDialog::getExistingDirectory( this,
      "Select cut-result directory",
      TTCut::cutDirPath,
      (QFileDialog::DontResolveSymlinks |
       QFileDialog::ShowDirsOnly) );

  if ( !str_dir.isEmpty() ) 
  {
    TTCut::cutDirPath    = str_dir;
    TTCut::muxOutputPath = str_dir;
    leOutputPath->setText( TTCut::cutDirPath );
    qApp->processEvents();
  }

  //getFreeDiskSpace();
}

/* /////////////////////////////////////////////////////////////////////////////
 * Selected tab changed
 */
void TTCutAVCutDlg::onTabChanged(__attribute__ ((unused))int index)
{
  //qDebug("current index: %d", tabWidget->indexOf(tabWidget->currentWidget()));
  //qDebug("index:         %d", index);

  if (tabWidget->indexOf(tabWidget->currentWidget()) == 2)
  {
    TTCut::muxOutputPath = leOutputPath->text();
    muxingPage->setTabData();
  }
}

/* /////////////////////////////////////////////////////////////////////////////
 * Set the tab data from global parameter
 */
void TTCutAVCutDlg::setCommonData()
{
  if ( !QDir(TTCut::cutDirPath).exists() )
    TTCut::cutDirPath = QDir::currentPath();

  // cut output filename and output path
  leOutputFile->setText( TTCut::cutVideoName );
  leOutputPath->setText( TTCut::cutDirPath );

  // cut options
  // write max bittrate tp first sequence
  cbMaxBitrate->setChecked(TTCut::cutWriteMaxBitrate);

  // write sequence end code
  cbEndCode->setChecked(TTCut::cutWriteSeqEnd);

  //getFreeDiskSpace();
 }

/* /////////////////////////////////////////////////////////////////////////////
 * Get tab data and set global parameter
 */
void TTCutAVCutDlg::getCommonData()
{
  // cut output filename and output path
  TTCut::cutVideoName  = leOutputFile->text();
  TTCut::cutDirPath    = leOutputPath->text();
  TTCut::muxOutputPath = leOutputPath->text();

  if ( !QDir(TTCut::cutDirPath).exists() )
  {
    TTCut::cutDirPath    = QDir::currentPath();
    TTCut::muxOutputPath = QDir::currentPath();
  }

  // Check for video file extension
  QFileInfo cutFile(TTCut::cutVideoName);
  QString ext = cutFile.suffix();

  if (ext.isEmpty() || ext != "m2v")
    TTCut::cutVideoName += ".m2v";

  // cut options
  // write max bittrate tp first sequence
  TTCut::cutWriteMaxBitrate = cbMaxBitrate->isChecked();

  // write sequence end code
  TTCut::cutWriteSeqEnd = cbEndCode->isChecked();
}

/* /////////////////////////////////////////////////////////////////////////////
 * Calculate the available diskspace
 */
void TTCutAVCutDlg::getFreeDiskSpace()
{
  QStringList df_cmd_list;
  
  // get free disk space
  dfOutput2 = "No information available";
  dfProc = new QProcess( );

  df_cmd_list.clear();

  df_cmd_list << "-h"  << TTCut::cutDirPath;

  dfProc->start( "df", df_cmd_list );

  connect(dfProc, SIGNAL(readyRead() ),SLOT( readFromStdout()));
  connect(dfProc, SIGNAL(finished(int, QProcess::ExitStatus)), SLOT( exitProcess(int, QProcess::ExitStatus)));

  dfProc->waitForFinished();

  delete dfProc;
}

void TTCutAVCutDlg::readFromStdout()
{
  char       temp_str[101];
  int        i, i_pos, i_line;
  QString    line;
  QByteArray ba;

  ba = dfProc->readAll();

  i_pos  = 0;
  i_line = 0;

  for (i = 0; i < ba.size(); ++i) {
    //log->infoMsg(c_name, "line: %s", qPrintable(ba[i]));
    if ( ba.at(i) != '\n' && i_pos < 100) {
      temp_str[i_pos] = ba.at(i);
      i_pos++;
    } else {
      temp_str[i_pos] = '\0';
      //line = temp_str;
      line = QString::fromAscii(temp_str);

      if ( i_line == 0 )
        laFreeDiskSpace1->setText(line);
      else
        laFreeDiskSpace2->setText(line);

      i_pos = 0;
      i_line++;
    }
  }
  qApp->processEvents();
}

void TTCutAVCutDlg::exitProcess( __attribute__ ((unused))int e_code, __attribute__((unused))QProcess::ExitStatus e_stat)
{
  qApp->processEvents();
}


