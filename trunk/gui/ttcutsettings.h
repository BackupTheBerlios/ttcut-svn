/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttcutsettings.h                                                 */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 03/01/2005 */
/* MODIFIED: b. altendorf                                    DATE: 03/23/2005 */
/* MODIFIED: b. altendorf                                    DATE: 04/05/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTCUTSETTINGS
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


#ifndef TTCUTSETTINGS_H
#define TTCUTSETTINGS_H


#include <qdir.h>
#include <qfile.h>
#include <qvariant.h>
#include <qdialog.h>
#include <qsettings.h>
//Added by qt3to4:
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QPushButton;
class QTabWidget;
class QWidget;
class QCheckBox;
class Q3GroupBox;
class QLabel;
class QLineEdit;
class QSpinBox;
class QComboBox;
class QRadioButton;
class Q3ButtonGroup;

#include "ttcut.h"

class TTCutCommonSettings;
class TTCutFilesSettings;
class TTCutEncoderSettings;
class TTCutMuxerSettings;
class TTCutChapterSettings;


// -----------------------------------------------------------------------------
// TTCut settings object; Save and restore settings from configuration file
// -----------------------------------------------------------------------------
class TTCutSettings : public QSettings
{
 public:
   TTCutSettings();
   ~TTCutSettings();

   void readSettings();
   void writeSettings();


   private:
   QString searchPath;
   QDir    userHomeDir;
};



// -----------------------------------------------------------------------------
// TTCut settings dialog; Container for the setting tabs
// -----------------------------------------------------------------------------
class TTCutSettingsDlg : public QDialog
{
    Q_OBJECT

public:
    TTCutSettingsDlg( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
    ~TTCutSettingsDlg();

    void setGlobalData();

protected slots:
    void onDlgOk();
    void onDlgCancel();

 protected:
    QGridLayout* TTCutSettingsDlgLayout;
    QHBoxLayout* Layout1;
    QPushButton* btnOk;
    QPushButton* btnCancel;
    QTabWidget*  TTSettingsTab;

 private:
    TTCutCommonSettings*  commonTab;
    TTCutFilesSettings*   filesTab;
    TTCutEncoderSettings* encoderTab;
    TTCutMuxerSettings*   muxerTab;
    TTCutChapterSettings* chapterTab;
};


// -----------------------------------------------------------------------------
// TTCut common settings tab
// -----------------------------------------------------------------------------
class TTCutCommonSettings : public QWidget
{
    Q_OBJECT

public:
    TTCutCommonSettings( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
    ~TTCutCommonSettings();

    // set the tab data from the global parameter
    void setTabData();

    // get the tab data and fill the global parameter
    void getTabData();

protected:
    Q3GroupBox*   gbNavigation;
    QCheckBox*   cbQuickSearch;
    QLabel*      laSliderClick;
    QLabel*      laPgUpDown;
    QLabel*      laArrowPlacement;
    QLabel*      laAltJump;
    QLabel*      laCtrlJump;
    QLabel*      laQuickJump;
    QLabel*      laMouseWheel;
    QSpinBox*    sbSliderClickPlacement;
    QSpinBox*    sbPgUpDown;
    QSpinBox*    sbArrowKeyPlacement;
    QSpinBox*    sbAltDistance;
    QSpinBox*    sbCtrlDistance;
    QSpinBox*    sbQuickJumpDistance;
    QSpinBox*    sbMouseWheel;
    QLabel*      laFrames1;
    QLabel*      laFrames2;
    QLabel*      laFrames3;
    QLabel*      laFrames4;
    QLabel*      laFrames5;
    QLabel*      laFrames7;
    QLabel*      laSeconds1;
    Q3GroupBox*   gbSearchPlay;
    QLabel*      laSearch;
    QLabel*      laPreview;
    QLabel*      laSkip;
    QSpinBox*    sbSearchIntervall;
    QSpinBox*    spPreviewLength;
    QSpinBox*    sbSkipFrames;
    QLabel*      laSeconds2;
    QLabel*      laSeconds3;
    QLabel*      laFrame6;
    Q3GroupBox*   gbFilesDirs;
    QLabel*      laTempDir;
    QLineEdit*   leTempDirectory;
    QPushButton* btnDirOpen;
    QGridLayout* TTCutCommonSettingsLayout;
    QGridLayout* gbNavigationLayout;
    QHBoxLayout* Layout4;
    QVBoxLayout* Layout1;
    QVBoxLayout* Layout2;
    QVBoxLayout* Layout3;
    QGridLayout* gbSearchPlayLayout;
    QVBoxLayout* Layout5;
    QVBoxLayout* Layout6;
    QVBoxLayout* Layout7;
    QGridLayout* gbFilesDirsLayout;
};

// -----------------------------------------------------------------------------
// TTCut files settings tab
// -----------------------------------------------------------------------------
class TTCutFilesSettings : public QWidget
{ 
    Q_OBJECT

public:
    TTCutFilesSettings( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
    ~TTCutFilesSettings();

    void setTabData();
    void getTabData();

 protected:
    QGridLayout* TTCutFilesSettingsLayout;
    Q3GroupBox*   cbCreateCutVideoIDD;
    QCheckBox*   cbCreateVideoIDD;
    QCheckBox*   cbReadVideoIDD;
    QCheckBox*   cbCreateAudioIDD;
    QCheckBox*   cbReadAudioIDD;
    QCheckBox*   CheckBox9;
    Q3GroupBox*   gbLogfile;
    Q3GroupBox*   gbLogfileOptions;
    QCheckBox*   cbLogPlusVideoIndex;
    QCheckBox*   cbLogExtended;
    QCheckBox*   cbCreateLog;
    QGridLayout* cbCreateCutVideoIDDLayout;
    QGridLayout* gbLogfileLayout;
    QGridLayout* gbLogfileOptionsLayout;
};


// -----------------------------------------------------------------------------
// TTCut encoding settings tab
// -----------------------------------------------------------------------------
class TTCutEncoderSettings : public QWidget
{
    Q_OBJECT

public:
    TTCutEncoderSettings( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
    ~TTCutEncoderSettings();

    // set the tab data from the global parameter
    void setTabData();

    // get the tab data and fill the global parameter
    void getTabData();

    QCheckBox* cbEncodingMode;

};



// -----------------------------------------------------------------------------
// TTCut muxer settings tab
// -----------------------------------------------------------------------------
class TTCutMuxerSettings : public QWidget
{
    Q_OBJECT

public:
    TTCutMuxerSettings( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
    ~TTCutMuxerSettings();

    // set the tab data from the global parameter
    void setTabData();

    // get the tab data and fill the global parameter
    void getTabData();

 protected:
    Q3ButtonGroup* bgMuxOptions;
    QRadioButton* rbCreateMuxScript;
    QRadioButton* rbMuxStreams;
    QRadioButton* rbMuxNone;
    Q3GroupBox*    gbMuxerSettings;
    QLabel*       laMuxTarget;
    QComboBox*    cbMuxTarget;
    QLabel*       laOutputPath;
    QComboBox*    cbMuxerProg;
    QLabel*       laMuxerProg;
    QPushButton*  pbConfigureMuxer;
    QPushButton*  btnOutputPath;
    QLineEdit*    leOutputPath;
    Q3GroupBox*    gbPostMuxing;
    QCheckBox*    cbPause;
    QCheckBox*    cbDeleteES;
    QGridLayout*  TTCutMuxerSettingsLayout;
    QGridLayout*  bgMuxOptionsLayout;
    QGridLayout*  gbMuxerSettingsLayout;
    QGridLayout*  gbPostMuxingLayout;
};


// -----------------------------------------------------------------------------
// TTCut chapter settings tab
// -----------------------------------------------------------------------------
class TTCutChapterSettings : public QWidget
{
    Q_OBJECT

public:
    TTCutChapterSettings( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
    ~TTCutChapterSettings();

    // set the tab data from the global parameter
    void setTabData();

    // get the tab data and fill the global parameter
    void getTabData();


protected:
    QCheckBox*   cbSpumuxChapter;
    QGridLayout* TTCutChapterSettingsLayout;
};



#endif //TTCUTSETTINGS_H

