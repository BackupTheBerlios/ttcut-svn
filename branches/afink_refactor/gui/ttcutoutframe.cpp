/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2006                                                      */
/* FILE     : ttcutoutframe.cpp                                               */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 02/19/2006 */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTCUTOUTFRAME
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
  if ( currentMpeg2Stream == vs || vs == 0 )
    return;

  currentMpeg2Stream = vs;
  QFileInfo fInfo(vs->filePath());

  if ( !videoToSharedVideoMap.contains(vs) ) {
    mpeg2Stream = new TTMpeg2VideoStream(fInfo);
    mpeg2Stream->makeSharedCopy( vs );
    videoToSharedVideoMap[vs] = mpeg2Stream;
  }
  else
    mpeg2Stream = videoToSharedVideoMap[vs];

  mpegWindow->openVideoStream( mpeg2Stream );
  mpegWindow->moveToFirstFrame( false );
}

//! Returns the current frame position in stream
int TTCutOutFrame::currentFramePos()
{
  if ( mpeg2Stream != 0 )
    return mpeg2Stream->currentIndex();
  else
    return -1;
}

void TTCutOutFrame::closeVideoStream(TTMpeg2VideoStream* vs)
{
  if ( vs == currentMpeg2Stream )
    mpegWindow->closeVideoStream();

  delete videoToSharedVideoMap[vs];
  videoToSharedVideoMap.remove(vs);

  if ( vs == currentMpeg2Stream )
    mpeg2Stream = 0;
}

//! Goto specified cut out position
void TTCutOutFrame::onGotoCutOut( int pos, TTMpeg2VideoStream* pMpeg2Stream )
{
  if ( pMpeg2Stream != 0 )
    initVideoStream( pMpeg2Stream );

  if ( mpeg2Stream == 0 )
    return;

  currentPosition = mpeg2Stream->moveToIndexPos(pos);
  mpegWindow->showFrameAt( currentPosition );

  updateCurrentPosition();
}

//! Goto previous possible cut-out position
void TTCutOutFrame::onPrevCutOutPos()
{
  int cutOutIndex;

  if (!TTCut::encoderMode)
    cutOutIndex = mpeg2Stream->moveToPrevPIFrame();
  else
    cutOutIndex = mpeg2Stream->moveToPrevFrame();

  emit newCutOutFramePos(cutOutIndex);

  mpegWindow->showFrameAt(cutOutIndex);
  updateCurrentPosition();
}

//! Goto next possible cut-out position
void TTCutOutFrame::onNextCutOutPos()
{
   int cutOutIndex;

  if (!TTCut::encoderMode)
    cutOutIndex = mpeg2Stream->moveToNextPIFrame();
  else
    cutOutIndex = mpeg2Stream->moveToNextFrame();

  emit newCutOutFramePos(cutOutIndex);

  mpegWindow->showFrameAt(cutOutIndex);
  updateCurrentPosition();
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
  if ( mpeg2Stream == 0 )
    return;

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

  //remove: emit newCutOutFramePos( mpeg2Stream->currentIndex() );
}
