/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttcutmainwnd.h                                                  */
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


#ifndef TTCUTMAINWND_H
#define TTCUTMAINWND_H

#include <qvariant.h>
#include <qpixmap.h>
//#include <q3mainwindow.h>
#include <qscrollbar.h>
#include <qthread.h>
//Added by qt3to4:
#include <QMainWindow>
#include <Q3ActionGroup>

#include <QWheelEvent>
#include <QFocusEvent>
#include <QGridLayout>
#include <QKeyEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QResizeEvent>


#include <QMenuBar>
#include <QFileDialog>

// gui
// -----------------------------------------------------------------------------
#include "ttcut.h"              /*..global parameter..........................*/
#include "ttcutsettings.h"      /*..settings dialog; load/save settings.......*/
#include "ttcutavcutdlg.h"      /*..cut A/V settings/start dialog.............*/
#include "ttaudiolistview.h"    /*..the audio files listview..................*/
#include "ttcutccrwnd.h"        /*..cut-list/chapter/result (ccr) widget......*/
#include "ttcutlistview.h"      /*..cut listview..............................*/
#include "ttprogressbar.h"      /*..the progress dialog.......................*/
#include "ttcutpreview.h"       /*..the cut preview window....................*/
#include "ttsearchframe.h"      /*..frame search method.......................*/
#include "ttcutaboutdlg.h"      /*..the everlasting about.....................*/
#include "ttcutproject.h"       /*..project file..............................*/

// mpeg2window
// -----------------------------------------------------------------------------
#include "../mpeg2window/ttmpeg2window.h" /*..video output widget.............*/

// avstream
// -----------------------------------------------------------------------------
#include "../avstream/ttavtypes.h"          /*..AV types (audio, video).......*/ 
#include "../avstream/ttmpeg2videostream.h" /*..AV stream objects.............*/

// avcut
// -----------------------------------------------------------------------------
#include "../avcut/ttavcutposition.h"  /*..cut class...........................*/
//#include "../avcut/tvideoschnitt.h"    /*..video cut processing stuff..........*/
//#include "../avcut/ttaudiocut.h"       /*..audio cut processing stuff..........*/


class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QAction;
class Q3ActionGroup;
class QScrollBar;
class QPushButton;
class Q3ListView;
class Q3ListViewItem;
class QLabel;
class QGroupBox;

class TTCutMainWnd : public QMainWindow
{
    Q_OBJECT

public:
  TTCutMainWnd( QWidget* parent=0, const char* name=0, Qt::WFlags fl=Qt::WType_TopLevel );
    ~TTCutMainWnd();

    void refreshCurrentPosition();

public slots:
    void resizeEvent( QResizeEvent* re );
    void focusInEvent( QFocusEvent* e );
    void focusOutEvent( QFocusEvent* e );
    void wheelEvent( QWheelEvent* e );
    void keyPressEvent ( QKeyEvent * e );

    // file-menu
    virtual void fileNew();
    virtual void fileOpen();
    virtual void fileSave();
    virtual void fileSaveAs();
    virtual void fileRecent();
    virtual void fileExit();

    // action-menu
    virtual void actionSave();
    virtual void actionSettings();

    // help menue
    virtual void helpAbout();

    // video stream section
    virtual void openVideoFile();
    virtual void refreshCutVideoLength();

    // audio stream section
    virtual void openAudioFile();
    virtual void audioFileUp();
    virtual void audioFileDown();

    // cut out frame section
    virtual void lastFramePrevious();
    virtual void lastFrameNext();
    virtual void searchFrame();
    virtual void selectCutOut( TTAVCutPosition& cutPos );
    virtual void showSlaveCutIn( TTAVCutPosition& cutPos );
    virtual void showSlaveCutOut( TTAVCutPosition& cutPos );

    // current frame section
    virtual void playVideo();

    // navigation section
    virtual void gotoPrevIFrame();
    virtual void gotoNextIFrame();
    virtual void gotoPrevPFrame();
    virtual void gotoNextPFrame();
    virtual void gotoPrevFrame();
    virtual void gotoNextFrame();
    virtual void setCutInPoint();
    virtual void setCutOutPoint();
    virtual void gotoCutInPosition();
    virtual void gotoCutOutPosition();
    virtual void addRangeToCutList();
    virtual void fastMotionOption();
    virtual void cutProposition();
    virtual void setMarker();
    virtual void gotoMarker();

    // video position slider
    virtual void videoSliderChanged( int );

    // cut section
    virtual void cutPreview();
    virtual void previewSingleCut( uint i_pos );
    virtual void videoAudioCut();
    virtual void audioCut();
    virtual void moveCutEntryUp();
    virtual void deleteCutEntry();
    virtual void moveCutEntryDown();
    virtual void editCutListEntry( TTAVCutPosition& cutPos );


    void showVideoStreamInfo();

    uint moveToMpeg2Index( uint i_pos, int f_type=0 );
    void createAVStreams( );

    void showCutInFrameAt( uint i_pos );


protected slots:
    virtual void languageChange();

 signals:
    void cutOutIndexChanged( uint cut_out_index, QTime cut_out_time );

 protected:
    void enableNavigation();
    void disableNavigation();
    void checkCutPosition();
    void refreshMarkerPosition();
    void refreshCutOutPosition();
    void refreshVideoLength();

    // project section
    void DateiAktualisieren( QFileInfo videoFInfo, QFileInfo audioFInfo );
    void closeProject();

private:
    // GUI
    QWidget     *centralWidget;
    QWidget     *widget;
    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QGroupBox   *gbCutOutFrame;
    QGridLayout *gridLayout1;
    QHBoxLayout *hboxLayout1;
    QLabel      *laCutOutFramePosition;
    QPushButton *pbPrevCutOutFrame;
    QPushButton *pbNextCutOutFrame;
    QPushButton *pbSearchFrame;
    QFrame      *cutOutWnd;
    QGroupBox   *gbCurrentFrame;
    QGridLayout *gridLayout2;
    QHBoxLayout *hboxLayout2;
    QPushButton *pbPlayVideo;
    QLabel      *tlCurrentPosition;
    QFrame      *curentFrameEnd;
    QGroupBox   *gbNavigation;
    QWidget     *widget1;
    QWidget     *widget2;
    QGridLayout *gridLayout3;
    QGridLayout *gridLayout4;
    QSpacerItem *spacerItem;
    QLabel      *tlCutInPosition;
    QPushButton *pbGotoCutOut;
    QSpacerItem *spacerItem1;
    QPushButton *pbNextPFrame;
    QPushButton *pbGotoCutIn;
    QSpacerItem *spacerItem2;
    QPushButton *pbSetCutIn;
    QPushButton *pbSetCutOut;
    QPushButton *pbPrevIFrame;
    QPushButton *pbPrevBFrame;
    QSpacerItem *spacerItem3;
    QPushButton *pbPrevPFrame;
    QSpacerItem *spacerItem4;
    QPushButton *pbNextBFrame;
    QLabel      *tlCutOutPosition;
    QPushButton *pbNextIFrame;
    QSpacerItem *spacerItem5;
    QGridLayout *gridLayout5;
    QPushButton *pbStreamPoints;
    QPushButton *pbSetMarker;
    QPushButton *pbGotoMarker;
    QPushButton *pbAddCut;
    QPushButton *pbQuickJump;
    QLabel      *tlMarkerPosition;
    QGroupBox   *gbAudioFiles;
    QGridLayout *gridLayout6;
    QVBoxLayout *vboxLayout;
    QPushButton *pbOpenAudioFile;
    QPushButton *pbAudioFileUp;
    QPushButton *pbAudioFileDown;
    QGroupBox   *gbVideoFile;
    QGridLayout *gridLayout7;
    QGridLayout *gridLayout8;
    QLabel      *tlVideoFileName;
    QLabel      *laVbvBuffer;
    QLabel      *laBitRate;
    QLabel      *laFrameRate;
    QLabel      *tlVbvBuffer;
    QLabel      *tlBitRate;
    QLabel      *tlFrameRate;
    QLabel      *tlVideoAspect;
    QLabel      *tlResolution;
    QLabel      *tlVideoLen;
    QLabel      *laVideoAspect;
    QLabel      *laResolution;
    QLabel      *laVideoLen;
    QLabel      *laVideoFileName;
    QPushButton *pbOpenVideoFile;
    QHBoxLayout *hboxLayout3;
    QVBoxLayout *vboxLayout1;
    QSpacerItem *spacerItem6;
    QPushButton *pbChapter;
    QPushButton *pbCutPreview;
    QPushButton *pbVideoAudioCut;
    QPushButton *pbAudioCut;
    QScrollBar  *sbVideoPosition;

    TTAudioListView* lvAudioFiles;
    TTCutListView*   cutListView;
    TTCutCCRWnd*     ccrTab;
		     
    QMenu*           fileMenu;
    QMenu*           actionMenu;
    QMenu*           helpMenu;
		     
    QAction*         fileNewAction;
    QAction*         fileOpenAction;
    QAction*         fileSaveAction;
    QAction*         fileSaveAsAction;
    QAction*         fileRecentAction;
    QAction*         fileExitAction;
    QAction*         actionSaveAction;
    QAction*         actionSettingsAction;
    QAction*         helpAboutAction;

    TTMessageLogger* log;
    // Progress bar
    TTProgressBar*  progress_bar;

    // Settings; Settings dialog
    TTCutSettings*    settings;
    TTCutSettingsDlg* settingsDlg;

    // Cut A/V dialog
    TTCutAVCutDlg*    cutAVDlg;

    // -------------------------------------------------------------------------
    // Application
    // -------------------------------------------------------------------------

    QString             video_file_name; /*..video file name..................*/
    QString             audio_file_name; /*..first audio file name............*/
    QString             project_file_name;

    // AV type
    // -------------------------------------------------------------------------
    TTVideoType*        video_type;      /*..video type.......................*/
    TTAudioType*        audio_type;      /*..audio type.......................*/

    // stream objects
    // -------------------------------------------------------------------------
    TTMpeg2VideoStream* mpeg2_stream;    /*..the mpeg2 video stream...........*/
    TTMpeg2VideoStream* cut_out_stream;  /*..stream copy for the cut-out frame*/
    //TTAudioStream*      audio_stream;    /*..the first audio stream...........*/

    TTMPEG2Window*      cutOutFrameWnd;     /*..mpeg2 window for cut-out frame...*/
    TTMPEG2Window*      currentFrameWnd;      /*..mpeg2 window for cut-in frame....*/


    bool sliderUpdateFrame;
    long currentFramePos;

    int            iSliderPrev;

    bool           isCutInPos;
    bool           isCutOutPos;
    bool           firstCutOut;
    long           markerPosition;
    
    uint             num_index;
    uint             num_header;
    uint             current_frame_pos;
    TTAVCutPosition* current_cut_pos;
    TTAVCutList*     avcut_list;
};

#endif // TTCUTMAINWND_H
