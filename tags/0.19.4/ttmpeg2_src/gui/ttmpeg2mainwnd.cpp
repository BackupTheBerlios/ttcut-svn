/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / ttcut.tritime.org                       */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttmpeg2mainwnd.cpp                                              */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 03/01/2007 */
/* MODIFIED: b. altendorf                                    DATE: 04/02/2007 */
/* MODIFIED: b. altendrof                                    DATE: 04/10/2007 */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTMPEG2MAINWND
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

#include <QtGui>
#include <QPixmap>

#include "ttmpeg2mainwnd.h"
#include "../../gui/ttprogressbar.h"

/* ////////////////////////////////////////////////////////////////////////////
 * Default constructor
 */
TTMpeg2MainWnd::TTMpeg2MainWnd()
  :QMainWindow()
{
  setupUi(this);

  // some default values
  currentStreamOrder = -1;
  isProjectOpen      = false;
  sliderUpdateFrame  = false;

  enableControls(false);

  // Conect signals from main menu
  connect(actionFileOpen,  SIGNAL(triggered()), videoFileInfo, SLOT(onFileOpen()));
  connect(actionAnalyze,   SIGNAL(triggered()), SLOT(onAnalyze()));
  connect(actionSettings,  SIGNAL(triggered()), SLOT(onSettings()));
  connect(actionExit,      SIGNAL(triggered()), SLOT(onExit()));

  connect(actionWriteHeaderList, SIGNAL(triggered()), SLOT(onWriteHeaderList()));

  // Connect signals from video file info widget
  connect(videoFileInfo,  SIGNAL(fileOpened(QString)), SLOT(onLoadVideoFile(QString)));

  // Connect signals from the step control widget
  connect(stepControl,    SIGNAL(gotoNextFrame(int, int)), SLOT(onGotoNextFrame(int, int)));
  connect(stepControl,    SIGNAL(gotoPrevFrame(int, int)), SLOT(onGotoPrevFrame(int, int)));

  // Scroller
  connect(scroller,       SIGNAL(valueChanged(int)), SLOT(onSliderValueChanged(int)));
}

/* ////////////////////////////////////////////////////////////////////////////
 * Enable / disable the child controls
 */
void TTMpeg2MainWnd::enableControls(bool value)
{
  actionAnalyze->setEnabled(false);

  actionWriteHeaderList->setEnabled(value);
  scroller->setEnabled(value);
  stepControl->enableControl(value);
}

/* ////////////////////////////////////////////////////////////////////////////
 * Analyze the video stream
 */
void TTMpeg2MainWnd::onAnalyze()
{
  if (!isProjectOpen)
    return;

  analyzeMpegStream(mpegStream);
}

/* ////////////////////////////////////////////////////////////////////////////
 * Show the settings dialog
 */
void TTMpeg2MainWnd::onSettings()
{
}

/* ////////////////////////////////////////////////////////////////////////////
 * Exit Application
 */
void TTMpeg2MainWnd::onExit()
{
  if (isProjectOpen)
    closeProject();

  qApp->quit();
}

/* ////////////////////////////////////////////////////////////////////////////
 * Write the video header list
 */
void TTMpeg2MainWnd::onWriteHeaderList()
{
  QString fn = QFileDialog::getSaveFileName( this,
      tr("Export header index list"),
      TTCut::lastDirPath,
      "Headerlist (*.txt)" );

  if (fn.isEmpty())
    return;

  QFileInfo fInfo( fn );
  TTCut::lastDirPath     = fInfo.absolutePath();
  TTHeaderWriter* writer = new TTHeaderWriter(videoIndexList, videoHeaderList);

  writer->writeHeaderListToFile(fn);

  delete writer;
}

/* ////////////////////////////////////////////////////////////////////////////
 * Load an existing video file
 */
void TTMpeg2MainWnd::onLoadVideoFile(QString fn)
{
  closeProject();
  openProject(fn);

  if (!openVideoFile(fn)) 
  {
    //TODO: throw exception
    return;
  }

  videoFileInfo->setFileName(fn);

  isProjectOpen = true;

  onAnalyze();
}

/* ////////////////////////////////////////////////////////////////////////////
 * Slider value changed
 */
void TTMpeg2MainWnd::onSliderValueChanged(int indexPos)
{
  if (!isProjectOpen)
    return;

    if (sliderUpdateFrame)
    {
      int newFramePos = mpegStream->moveToIndexPos(indexPos, 0);
      currentFrame->showFrameAt(newFramePos);
      updateFrameInfo(indexPos);
      updateVideoFileInfo(mpegStream);
    }
    sliderUpdateFrame = true;
}

/* ////////////////////////////////////////////////////////////////////////////
 * Open the video
 */
bool TTMpeg2MainWnd::openVideoFile(QString fn)
{
  QFileInfo      fInfo(fn);

  if (!fInfo.exists()) 
  {
    // TODO: throw an exception!
    return false;
  }

  videoType = new TTVideoType(fn);

  if (videoType->avStreamType() != TTAVTypes::mpeg2_demuxed_video) {
    return false;
  }

  mpegStream = (TTMpeg2VideoStream*)videoType->createVideoStream();

  return true;
}

/* ////////////////////////////////////////////////////////////////////////////
 * Move to next frame in stream
 */
void TTMpeg2MainWnd::onGotoNextFrame(int type, int order)
{
  uint nextIndex = 0;

  switchStreamOrder(order);

  switch (type)
  {
    // I-Frame
    case 1:
      nextIndex = mpegStream->moveToNextIFrame();
      break;

    // P-Frame
    case 2:
      nextIndex = mpegStream->moveToNextPFrame();
      break;

    case 3:
    default:
      nextIndex = mpegStream->moveToNextFrame();
      break;
  }

  // Update display
  updateDisplay(nextIndex);
}

/* ////////////////////////////////////////////////////////////////////////////
 * Move to previous frame in stream
 */
void TTMpeg2MainWnd::onGotoPrevFrame(int type, int order)
{
  uint prevIndex = 0;

  switchStreamOrder(order);

  switch (type)
  {
    // I-Frame
    case 1:
      prevIndex = mpegStream->moveToPrevIFrame();
      break;

    // P-Frame
    case 2:
      prevIndex = mpegStream->moveToPrevPFrame();
      break;

    // B-Frame
    case 3:
    default:
      prevIndex = mpegStream->moveToPrevFrame();
      break;
  }

  // Update display
  updateDisplay(prevIndex);
}

/* ////////////////////////////////////////////////////////////////////////////
 * Switch the stream order
 */
void TTMpeg2MainWnd::switchStreamOrder(int order)
{
  if (currentStreamOrder == order)
    return;

  currentStreamOrder = order;

  switch (order)
  {
    case 0:
    default:
      videoIndexList->setDisplayOrder();
      break;

    case 1:
      videoIndexList->setStreamOrder();
      break;
  }
}

/* ////////////////////////////////////////////////////////////////////////////
 * Update the video display
 */
void TTMpeg2MainWnd::updateDisplay(int indexPos)
{
  sliderUpdateFrame = false;
  currentFrame->showFrameAt(indexPos);

  if (indexPos == 0)
    currentFrame->moveToFirstFrame(true);

  scroller->setValue(indexPos);
  updateFrameInfo(indexPos);
  updateVideoFileInfo(mpegStream);
}

/* ////////////////////////////////////////////////////////////////////////////
 * Update the current frame information
 */
void TTMpeg2MainWnd::updateFrameInfo(int indexPos)
{
  totalTime = ttFramesToTime(videoIndexList->count(), 25.0);

  TTPicturesHeader* currentPicture = (TTPicturesHeader*)videoHeaderList->at(
      videoIndexList->headerListIndex(indexPos));

  frameInfo->setFrameOffset(videoHeaderList->at(
      videoIndexList->headerListIndex(indexPos))->headerOffset());
  frameInfo->setNumDisplayOrder(videoIndexList->displayOrder(indexPos),
      currentPicture->temporal_reference);
  frameInfo->setNumStreamOrder(videoIndexList->streamOrder(indexPos));
  frameInfo->setFrameType(videoIndexList->videoIndexAt(indexPos)->picture_coding_type);
  frameInfo->setFrameTime(ttFramesToTime(videoIndexList->displayOrder(indexPos),
      25.0), totalTime);
  frameInfo->setFrameSize((long)videoIndexList->frameSize(indexPos));
  frameInfo->setGOPNumber(videoIndexList->gopNumber(indexPos),
      currentPicture->temporal_reference);
}

/* ////////////////////////////////////////////////////////////////////////////
 * Update the video file information
 */
void TTMpeg2MainWnd::updateVideoFileInfo(TTMpeg2VideoStream* mpegStream)
{
  videoFileInfo->setFileName(mpegStream->fileInfo()->fileName());
  videoFileInfo->setLength(
      ttFramesToTime(mpegStream->frameCount(), mpegStream->frameRate()),
      mpegStream->frameCount());
  videoFileInfo->setResolution(
      mpegStream->currentSequenceHeader()->horizontalSize(),
      mpegStream->currentSequenceHeader()->verticalSize());

  videoFileInfo->setAspect(
      mpegStream->currentSequenceHeader()->aspectRatioText());

  videoFileInfo->setFrameRate(
      mpegStream->currentSequenceHeader()->frameRateText());
  videoFileInfo->setBitRate(
      mpegStream->currentSequenceHeader()->bitRateKbit());
  videoFileInfo->setVBVBuffer(
      mpegStream->currentSequenceHeader()->vbvBufferSize());
}

/* ////////////////////////////////////////////////////////////////////////////
 * Open ttmpeg2 project: open video file
 *                       anaylize the stream
 *                       enable navigation
 */
void TTMpeg2MainWnd::openProject(QString fileName)
{
  if (!openVideoFile(fileName))
  {
    // TODO: throw exception
    return;
  }

  videoFileInfo->setFileName(fileName);

  onAnalyze();

  isProjectOpen = true;
  enableControls(true);
}

/* ////////////////////////////////////////////////////////////////////////////
 * Close the current project and free used resources.
 * Disable navigation.
 */
void TTMpeg2MainWnd::closeProject()
{
  if (!isProjectOpen)
    return;

  isProjectOpen = false;

  enableControls(false);
  scroller->setValue(0);
  videoFileInfo->clearControl();
  frameInfo->clearControl();
  currentFrame->closeVideoStream();

  // clean up used resources
  if (mpegStream != NULL)
  {
    delete mpegStream;
    mpegStream = NULL;
  }

  if (videoType != NULL)
  {
    delete videoType;
    videoType = NULL;
  }

  videoIndexList  = NULL;
  videoHeaderList = NULL;
}

/* ////////////////////////////////////////////////////////////////////////////
 * Analyze the mpeg stream
 */
void TTMpeg2MainWnd::analyzeMpegStream(TTMpeg2VideoStream* stream)
{
  if (stream == NULL)
  {
    // TODO: throw exception
    return;
  }

  TTProgressBar* pBar = new TTProgressBar(this);

  stream->setProgressBar(pBar);
  pBar->show();
  qApp->processEvents();

  stream->createHeaderList();
  stream->createIndexList();

  videoIndexList  = mpegStream->indexList();
  videoHeaderList = mpegStream->headerList();
 
  videoIndexList->sortDisplayOrder();
  switchStreamOrder(0);

  scroller->setMinimum(0);
  scroller->setMaximum(mpegStream->frameCount()-1);

  currentFrame->openVideoStream(mpegStream);
  updateDisplay(0);

  delete pBar;
  stream->setProgressBar(NULL);
}


