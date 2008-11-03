/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / ttcut.tritime.org                       */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttcutmainwindow.h                                               */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 02/26/2006 */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTCUTMAINWINDOW
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

#ifndef TTCUTMAINWINDOW_H
#define TTCUTMAINWINDOW_H

#include "ui_mainwindow.h"

#include "../common/ttcut.h"
#include "../common/ttmessagelogger.h"
#include "../data/ttaudiolistdata.h"
#include "../data/ttcutlistdata.h"
#include "../data/ttmuxlistdata.h"
#include "../avstream/ttavtypes.h"
#include "../avstream/ttmpeg2videostream.h"
#include "../extern/ttmplexprovider.h"
#include "ttcutpreview.h"
#include "ttcutsettings.h"
#include "ttcutsettingsdlg.h"
#include "ttcutproject.h"

class TTCutListData;

class TTCutMainWindow : public QMainWindow, Ui::TTCutMainWindow
{
  Q_OBJECT

  public:
    TTCutMainWindow();
    ~TTCutMainWindow();

void keyPressEvent(QKeyEvent* e);

  public slots:
    void onFileNew();
    void onFileOpen();
    void onFileSave();
    void onFileSaveAs();
    void onFileRecent();
    void onFileExit();
    void closeEvent(QCloseEvent* event);
    void onActionSave();
    void onActionSettings();

    void onHelpAbout();

    void onReadVideoStream( QString fName );
    void onReadAudioStream( QString fName );

    void onVideoSliderChanged( int value );

    void onNewFramePos(int);

    void onPreviewCut(int index);
    void onAudioVideoCut(int index, bool cutAudioOnly=false, TTCutListData* cutData=NULL);
    void onAudioCut(int index);
    void onSelectedAudioVideoCut(QVector<int> selectedItems);

  private:
    void closeProject();
    void createAVStreams( QString videoFile, QString audioFile );
    QFileInfoList audioFromVideoName(QString videoFile);
    void navigationEnabled( bool enabled );
    bool openProjectFile(QString fName);
    int openVideoStream(QString fName);
    int openAudioStream(QString fName);
    void initStreamNavigator();
    void updateRecentFileActions();
    void insertRecentFile(const QString& fName);

  private:
   TTCutSettings*      settings;
   TTAudioListData*    audioList;
   TTCutListData*      cutListData;
   TTMuxListData*      muxListData;
   TTMessageLogger*    log;
   TTMplexProvider*    mplexProvider;
   TTMpeg2VideoStream* mpegStream;
   bool                sliderUpdateFrame;
//    QMap                mpegStreamMap;

   // recent files menu
   enum {MaxRecentFiles = 5};
   QAction* recentFileAction[MaxRecentFiles];
};

#endif //TTCUTMAINWINDOW_H
