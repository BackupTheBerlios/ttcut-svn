#ifndef TTCUTMAINWINDOW_H
#define TTCUTMAINWINDOW_H

#include "ui_mainwindow.h"

#include "../common/ttcut.h"
#include "../common/ttmessagelogger.h"
#include "../data/ttaudiolistdata.h"
#include "../data/ttcutlistdata.h"
#include "../avstream/ttavtypes.h"
#include "../avstream/ttmpeg2videostream.h"
#include "ttcutpreview.h"
#include "ttcutsettings.h"

class TTCutListData;

class TTCutMainWindow : public QMainWindow, Ui::TTCutMainWindow
{
  Q_OBJECT

  public:
    TTCutMainWindow();

  public slots:
    void onFileNew();
    void onFileOpen();
    void onFileSave();
    void onFileSaveAs();
    void onFileRecent();
    void onFileExit();

    void onActionSave();
    void onActionSettings();

    void onHelpAbout();

    void onReadVideoStream( QString fName );
    void onReadAudioStream( QString fName );

    void onVideoSliderChanged( int value );

    void onAddCutRange(int cutIn, int cutOut);

    void onPrevCutOutPos();
    void onNextCutOutPos();
    void onSearchFrame();

    void onNewFramePos(int);

    void onPreviewCut(int index);
    void onAudioVideoCut(int index);
    void onAudioCut(int index);

  private:
    void closeProject();
    void createAVStreams( QString videoFile, QString audioFile );
    QString audioFromVideoName(QString videoFile); 
    void navigationEnabled( bool enabled );
   
  private:
   TTCutSettings*      settings;
   TTAudioListData*    audioList;
   TTCutListData*      cutListData;
   TTMessageLogger*    log;
   TTMpeg2VideoStream* mpegStream;
   bool sliderUpdateFrame;
};

#endif //TTCUTMAINWINDOW_H