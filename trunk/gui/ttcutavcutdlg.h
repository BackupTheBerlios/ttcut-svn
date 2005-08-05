/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttcutavcutdlg.h                                                 */
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

#ifndef TTCUTAVCUTDLG_H
#define TTCUTAVCUTDLG_H

#include <qvariant.h>
#include <qdialog.h>
#include <q3process.h>
//Added by qt3to4:
#include <QLabel>
#include <QVBoxLayout>
#include <Q3Frame>
#include <QHBoxLayout>
#include <QGridLayout>

#include "ttcut.h"
#include "ttcutsettings.h"

class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QPushButton;
class QTabWidget;
class QWidget;
class Q3Frame;
class QLineEdit;
class QLabel;
class Q3GroupBox;
class QCheckBox;


class TTCutAVCutCommonTab;

// -----------------------------------------------------------------------------
// TTCut A/V cut dialog; 
// -----------------------------------------------------------------------------
class TTCutAVCutDlg : public QDialog
{ 
    Q_OBJECT

public:
    TTCutAVCutDlg( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
    ~TTCutAVCutDlg();

    void setGlobalData();

protected slots:
    void onDlgStart();
    void onDlgCancel();

protected:
    QGridLayout* TTCutAVCutDlgLayout;
    QHBoxLayout* Layout1;
    QTabWidget*  twAVCut;
    QPushButton* pbStart;
    QPushButton* pbCancel;

 private:
    TTCutAVCutCommonTab*  commonTab;
    TTCutEncoderSettings* encoderTab;
    TTCutMuxerSettings*   muxerTab;
    TTCutChapterSettings* chapterTab;
};


// -----------------------------------------------------------------------------
// TTCut common settings for A/V cut
// -----------------------------------------------------------------------------
class TTCutAVCutCommonTab : public QWidget
{ 
    Q_OBJECT

public:
    TTCutAVCutCommonTab( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
    ~TTCutAVCutCommonTab();

    // set the tab data from the global parameter
    void setTabData();

    // get the tab data and fill the global parameter
    void getTabData();

    protected slots:
    virtual void readFromStdout();
    virtual void exitProcess();

 protected:
    QGridLayout* TTCutAVCutCommonTabLayout;
    QGridLayout* Layout2;
    QGridLayout* Layout3;
    QGridLayout* gbCutOptionsLayout;
    QLineEdit*   leOutputFile;
    QLabel*      laOutPath;
    QLineEdit*   leOutputPath;
    QLabel*      laOutFile;
    QPushButton* pbSelectPath;
    QLabel*      laFreeSpace;
    QLabel*      textFreeDriveSpace1;
    QLabel*      textFreeDriveSpace2;
    Q3GroupBox*   gbCutOptions;
    QCheckBox*   cbMaxBitrate;
    QCheckBox*   cbWriteSeqEnd;
    Q3Frame*      Frame4;

 private:
    Q3Process*    dfProc;
    QString      dfOutput1;
    QString      dfOutput2;
};

#endif // TTCUTAVCUTDLG_H
