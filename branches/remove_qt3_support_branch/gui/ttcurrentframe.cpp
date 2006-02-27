#include "ttcurrentframe.h"

//! Default constructor
TTCurrentFrame::TTCurrentFrame(QWidget* parent)
  :QWidget(parent)
{
  setupUi( this );
}

//! Needeb by Qt Designer
void TTCurrentFrame::setTitle ( const QString & title )
{
  gbCurrentFrame->setTitle( title );
}

void TTCurrentFrame::controlEnabled( bool enabled )
{
  pbPlayVideo->setEnabled( false );//enabled );
}


//! Set the mpeg stream
void TTCurrentFrame::initVideoStream(TTMpeg2VideoStream *vs)
{
  mpeg2Stream = vs;
  
  mpegWindow->openVideoStream( mpeg2Stream );
  mpegWindow->moveToFirstFrame();
}
 
//! Returns the current frame position in stream
int TTCurrentFrame::currentFramePos()
{
  return mpeg2Stream->currentIndex();
}

// Signals from the navigation widget
// ----------------------------------------------------------------------------

//! Navigate to previous I-Frame
void TTCurrentFrame::onPrevIFrame()
{
  int newFramePos;

  newFramePos = mpeg2Stream->moveToPrevIFrame( );  
  mpegWindow->showFrameAt( newFramePos );

  updateCurrentPosition();
}

//! Navigate to next I-Frame
void TTCurrentFrame::onNextIFrame()
{
  int newFramePos;

  newFramePos = mpeg2Stream->moveToNextIFrame( );  
  mpegWindow->showFrameAt( newFramePos );

  updateCurrentPosition();
}

//! Navigate to previous P-Frame
void TTCurrentFrame::onPrevPFrame()
{
  int newFramePos;

  newFramePos = mpeg2Stream->moveToPrevPFrame( );  
  mpegWindow->showFrameAt( newFramePos );

  updateCurrentPosition();
}

//! Navigate to next P-Frame
void TTCurrentFrame::onNextPFrame()
{
  int newFramePos;

  newFramePos = mpeg2Stream->moveToNextPFrame( );  
  mpegWindow->showFrameAt( newFramePos );

  updateCurrentPosition();
}

//! Navigate to previous B-Frame
void TTCurrentFrame::onPrevBFrame()
{
  int newFramePos;

  newFramePos = mpeg2Stream->moveToPrevFrame( );  
  mpegWindow->showFrameAt( newFramePos );

  updateCurrentPosition();
}

//! Navigate to next B-Frame
void TTCurrentFrame::onNextBFrame()
{
  int newFramePos;

  newFramePos = mpeg2Stream->moveToNextFrame( );  
  mpegWindow->showFrameAt( newFramePos );

  updateCurrentPosition();
}

//! Navigate to marker position
void TTCurrentFrame::onGotoMarker(int markerPos)
{
  int newFramePos;

  newFramePos = mpeg2Stream->moveToIndexPos( markerPos, 0 );
  mpegWindow->showFrameAt( newFramePos );

  updateCurrentPosition();
}

//! Cut in position was set
void TTCurrentFrame::onSetCutIn(int cutInPos)
{
}

//! Cut out position was set
void TTCurrentFrame::onSetCutOut(int cutOutPos)
{
}

//! Goto cut in position
void TTCurrentFrame::onGotoCutIn(int pos)
{
  int newFramePos;

  newFramePos = mpeg2Stream->moveToIndexPos( pos, 0 );
  mpegWindow->showFrameAt( newFramePos );

  updateCurrentPosition();
}

//! Goto cut out position
void TTCurrentFrame::onGotoCutOut(int pos)
{
  int newFramePos;

  newFramePos = mpeg2Stream->moveToIndexPos( pos, 0 );
  mpegWindow->showFrameAt( newFramePos );

  updateCurrentPosition();
}

void TTCurrentFrame::onGotoFrame(int pos)
{
  onGotoFrame(pos, 0);
}

//! Goto arbitrary frame at given position
void TTCurrentFrame::onGotoFrame(int pos, int fast)
{
  int newFramePos;

  newFramePos = mpeg2Stream->moveToIndexPos( pos, fast );  
  mpegWindow->showFrameAt( newFramePos );

  updateCurrentPosition();
}

void TTCurrentFrame::updateCurrentPosition()
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
  laCurrentPosition->setText( szTemp1 );

  laCurrentPosition->update();

  emit newFramePosition( mpeg2Stream->currentIndex() );
}

void TTCurrentFrame::saveCurrentFrame()
{
  QString      szTemp;
  QString      extension;
  QString      format;
  QStringList  fileList;
  QString      fileName;
  QFileDialog* fileDlg;

  // no video file open
  if ( !TTCut::isVideoOpen )
    return;

  // get the image file name
  fileDlg = new QFileDialog( this,
			     "save current frame",
			     TTCut::lastDirPath,
			     "Portable Network Graphics (*.png);;JPEG (*.jpg);;Bitmap (*.bmp)" );
  
  // enable specifying a file that doesn't exist
  fileDlg->setFileMode( QFileDialog::AnyFile );
  fileDlg->setAcceptMode( QFileDialog::AcceptSave );

  // input filename specified
  if ( fileDlg->exec() == QDialog::Accepted )
  {
     szTemp   = fileDlg->selectedFilter();
     fileList = fileDlg->selectedFiles();
     fileName = fileList.at(0);

     if ( szTemp == "Portable Network Graphics (*.png)" )
     {
       format    = "PNG";
       extension = "png";
     }
     else if ( szTemp == "JPEG (*.jpg)" )
     {
       format    = "JPG";
       extension = "jpg";
     }
     else if ( szTemp == "Bitmap (*.bmp)" )
     {
       format    = "BMP";
       extension = "bmp";
     }
     else
     {
       qDebug( "unsupported format" );
       return;
     }

     fileName = ttChangeFileExt( fileName, qPrintable(extension) );
     
     mpegWindow->saveCurrentFrame( fileName, qPrintable(format) );
  }
  delete fileDlg;
}
