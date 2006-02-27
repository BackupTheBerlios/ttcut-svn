#ifndef TTCUTOUTFRAME_H
#define TTCUTOUTFRAME_H

#include "ui_cutoutframewidget.h"

#include "../common/ttcut.h"
#include "../avstream/ttmpeg2videostream.h"
#include "ttprogressbar.h"
#include "ttsearchframe.h"

class TTCutOutFrame : public QWidget, Ui::TTCutOutFrameWidget
{
  Q_OBJECT

  public:
    TTCutOutFrame( QWidget* parent=0 );

    void setTitle ( const QString & title );
    void controlEnabled( bool enabled );    
    void initVideoStream( TTMpeg2VideoStream* vs );
    int currentFramePos();

  public slots:
    void onGotoCutOut( int pos );
    void onPrevCutOutPos();
    void onNextCutOutPos();
    void onSearchFrame();

signals:
    void newCutOutFramePos( int );
    void equalFrameFound( int );

  private:
    void updateCurrentPosition();

  private:
    TTMpeg2VideoStream* currentMpeg2Stream;
    TTMpeg2VideoStream* mpeg2Stream;
    int currentPosition;
};

#endif //TTCUTOUTFRAME_H  
