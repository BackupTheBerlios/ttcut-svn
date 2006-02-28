#ifndef TTCURRENTFRAME_H
#define TTCURRENTFRAME_H

#include "ui_currentframewidget.h"

#include "../common/ttcut.h"
#include "../avstream//ttmpeg2videostream.h"


class TTCurrentFrame : public QWidget, Ui::TTCurrentFrameWidget
{
  Q_OBJECT

  public:
    TTCurrentFrame( QWidget* parent=0 );

    void setTitle ( const QString & title );
    void controlEnabled( bool enabled );
    void initVideoStream( TTMpeg2VideoStream *vs );
    int currentFramePos();
    void saveCurrentFrame();
    void closeVideoStream();

    public slots:
      void onPrevIFrame();
    void onNextIFrame();
    void onPrevPFrame();
    void onNextPFrame();
    void onPrevBFrame();
    void onNextBFrame();
    void onGotoMarker( int markerPos );
    void onSetCutIn( int cutInPos );
    void onSetCutOut( int cutOutPos );
    void onGotoCutIn( int pos );
    void onGotoCutOut( int pos );
    void onGotoFrame( int pos );
    void onGotoFrame( int pos, int fast );

signals:
    void newFramePosition(int);

  private:
    void updateCurrentPosition();

  private:
    TTMpeg2VideoStream* mpeg2Stream;
};

#endif //TTCURRENTFRAME_H 
