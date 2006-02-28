#include "ttcutoutframe.h"

//! Default constructor
TTCutOutFrame::TTCutOutFrame(QWidget* parent)
  :QWidget(parent)
{
  setupUi( this );

  currentPosition = -1;
  
  connect(pbPrevCutOutFrame, SIGNAL(clicked()), SLOT(onPrevCutOutPos()));
  connect(pbNextCutOutFrame, SIGNAL(clicked()), SLOT(onNextCutOutPos()));
  connect(pbSearchFrame,     SIGNAL(clicked()), SLOT(onSearchFrame()));
}

//! Needed by Qt Designer
void TTCutOutFrame::setTitle ( const QString & title )
{
  gbCutOutFrame->setTitle( title );
}

void TTCutOutFrame::controlEnabled( bool enabled )
{
  pbPrevCutOutFrame->setEnabled( enabled );
  pbNextCutOutFrame->setEnabled( enabled );
  pbSearchFrame->setEnabled( enabled );
}

//! Set the mpeg stream
void TTCutOutFrame::initVideoStream(TTMpeg2VideoStream *vs)
{
  currentMpeg2Stream = vs;
  mpeg2Stream = new TTMpeg2VideoStream( QFileInfo(vs->fileName()) );
  mpeg2Stream->makeSharedCopy( vs );
  mpegWindow->openVideoStream( mpeg2Stream );
  mpegWindow->moveToFirstFrame( false );
}
 
//! Returns the current frame position in stream
int TTCutOutFrame::currentFramePos()
{
  return mpeg2Stream->currentIndex();
}

void TTCutOutFrame::closeVideoStream()
{
  mpegWindow->closeVideoStream();
}

//! Goto specified cut out position
void TTCutOutFrame::onGotoCutOut(int pos)
{
  currentPosition = mpeg2Stream->moveToIndexPos( pos, 0 );  
  mpegWindow->showFrameAt( currentPosition );

  updateCurrentPosition();
}

//! Goto previous possible cut-out position
void TTCutOutFrame::onPrevCutOutPos()
{
}

//! Goto next possible cut-out position
void TTCutOutFrame::onNextCutOutPos()
{
}

//! On search equal frame action
void TTCutOutFrame::onSearchFrame()
{
  int searchPos;

  if ( currentPosition >= 0 ) {

    TTProgressBar* pBar = new TTProgressBar( this );

    pBar->show();
    qApp->processEvents();

    TTFrameSearch* searchFrame = new TTFrameSearch( pBar );

    searchFrame->initFrameSearch( currentMpeg2Stream );
    searchPos = searchFrame->searchFrame( 
        mpeg2Stream->currentIndex(),
        currentMpeg2Stream->currentIndex() );

    // show frame in cut-in window 
    if ( searchPos >= 0 )
    {
      emit equalFrameFound( searchPos );
    } 

    delete pBar;
    delete searchFrame;
  }
}

void TTCutOutFrame::updateCurrentPosition()
{
  QString szTemp;
  QString szTemp1, szTemp2;
  int     frame_type = mpeg2Stream->currentFrameType();

  szTemp1 = mpeg2Stream->currentFrameTime().toString("hh:mm:ss.zzz");

  szTemp2.sprintf(" (%d)",mpeg2Stream->currentIndex());

  if ( frame_type == 1 ) szTemp2 += " [I]";
  if ( frame_type == 2 ) szTemp2 += " [P]";
  if ( frame_type == 3 ) szTemp2 += " [B]";

  szTemp1 += szTemp2;
  laCutOutFramePosition->setText( szTemp1 );

  laCutOutFramePosition->update();

  emit newCutOutFramePos( mpeg2Stream->currentIndex() );
}
