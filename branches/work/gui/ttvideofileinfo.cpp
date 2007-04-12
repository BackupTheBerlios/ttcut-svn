/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2006                                                      */
/* FILE     : ttvideofileinfo.cpp                                             */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (b.altendorf@tritime.de)           DATE: 02/23/2005 */
/* MODIFIED: b. altendorf                                    DATE: 02/19/2006 */
/* MODIFIED: b. altendorf                                    DATE: 03/21/2007 */
/*----------------------------------------------------------------------------*/
 
// ----------------------------------------------------------------------------
// TTVIDEOFILEINFO
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
 
#include "ttvideofileinfo.h"

#include <QFileInfo>
#include <QFileDialog>

/*
 * Constructor
 */
TTVideoFileInfo::TTVideoFileInfo(QWidget* parent)
  :QWidget(parent)
{
  setupUi( this );

  connect(pbVideoOpen, SIGNAL(clicked()), SLOT(onFileOpen()));
}

/*
 * Set widget title
 */
void TTVideoFileInfo::setTitle( __attribute__((unused))const QString & title )
{
}

/*!
 * Enable / disable the control and his child controls
 */
void TTVideoFileInfo::enableControl(bool value)
{
  pbVideoOpen->setEnabled(value);
}

/*
 * legacy ;-)
 */
void TTVideoFileInfo::resetVideoInfo()
{
  clearControl();
}

/*
 * Reset the video file info text labels to default values
 */
void TTVideoFileInfo::clearControl()
{
  infoVideoFileName->setText("---");
  infoVideoLength->setText("---");
  infoVideoResolution->setText("---");
  infoVideoAspectRatio->setText("---");
  infoVideoFramerate->setText("---");
  infoVideoBitrate->setText("---");
  infoVideoBuffer->setText("---");
}

void TTVideoFileInfo::setVideoInfo( TTMpeg2VideoStream* mpeg2Stream )
{
  QString szTemp;

  // video file name
  infoVideoFileName->setText( mpeg2Stream->fileInfo()->fileName());

  // video length
  int numFrames = mpeg2Stream->frameCount();
  QTime time = ttFramesToTime( numFrames, mpeg2Stream->frameRate() );
  szTemp.sprintf("%s (%d)", qPrintable(time.toString("hh:mm:ss.zzz")), numFrames);
  infoVideoLength->setText( szTemp );

  // set video resolution
  szTemp.sprintf("%dx%d",mpeg2Stream->currentSequenceHeader()->horizontalSize(),
      mpeg2Stream->currentSequenceHeader()->verticalSize());
  infoVideoResolution->setText( szTemp );

  // set aspect
  infoVideoAspectRatio->setText(mpeg2Stream->currentSequenceHeader()->aspectRatioText());

  // set framerate
  infoVideoFramerate->setText(mpeg2Stream->currentSequenceHeader()->frameRateText());

  // set bitrate
  szTemp.sprintf("%4.1f kBit/s",
      mpeg2Stream->currentSequenceHeader()->bitRateKbit());
  infoVideoBitrate->setText( szTemp );

  // set VBV buffer size
  szTemp.sprintf("%d kWords",
      mpeg2Stream->currentSequenceHeader()->vbvBufferSize());
  infoVideoBuffer->setText( szTemp );
}



/*
 * Set the video stream file name
 */
void TTVideoFileInfo::setFileName(QString fName)
{
  infoVideoFileName->setText(fName); 
}

/*
 * Set the video stream length as QString
 */
void TTVideoFileInfo::setLength(QString length)
{
  infoVideoLength->setText(length);
}

/*
 * Set the video stream file length as QTime an the number of frames
 * as int
 */
void TTVideoFileInfo::setLength(QTime total, int numFrames)
{
  QString tmpString;

  tmpString.sprintf("%s (%d)", 
      qPrintable(total.toString("hh:mm:ss.zzz")), 
      numFrames);
  infoVideoLength->setText(tmpString);
}

/*
 * Set the video stream resolution as QString
 */
void TTVideoFileInfo::setResolution(QString resolution)
{
  infoVideoResolution->setText(resolution);
}

/*
 * Set the video stream resolution as width x heigth
 */
void TTVideoFileInfo::setResolution(int width, int height)
{
  QString tmpString;

  tmpString.sprintf("%dx%d", width, height);
  infoVideoResolution->setText(tmpString);
}

/*
 * Set the video stream aspect ratio as QString
 */
void TTVideoFileInfo::setAspect(QString aspect)
{
  infoVideoAspectRatio->setText( aspect );  
}

/*
 * Set the video stream framerate as QString
 */
void TTVideoFileInfo::setFrameRate(QString frameRate)
{
  infoVideoFramerate->setText(frameRate);
}

/*
 * Set the video stream bitrate as QString
 */
void TTVideoFileInfo::setBitRate(QString bitRate)
{
  infoVideoBitrate->setText(bitRate);
}

/*
 * Set the video stream bitrate as float value
 */
void TTVideoFileInfo::setBitRate(float rate)
{
  QString tmpString;

  tmpString.sprintf("%4.1f kBit/s", rate);
  infoVideoBitrate->setText(tmpString);
}

/*
 * Set the video stream VBV buffer size as QString
 */
void TTVideoFileInfo::setVBVBuffer(QString vbvBuffer)
{
  infoVideoBuffer->setText(vbvBuffer);
}

/*
 * Set the video stream VBV buffer as int value
 */
void TTVideoFileInfo::setVBVBuffer(int buffSize)
{
  QString tmpString;

  tmpString.sprintf("%d kWords", buffSize);
  infoVideoBuffer->setText(tmpString);
}

/*
 * Show the file open dialog 
 */
void TTVideoFileInfo::onFileOpen()
{
  QString fn = QFileDialog::getOpenFileName( this,
      tr("Open video file"),
      TTCut::lastDirPath,
      "Video (*.m2v)" );

  if( !fn.isEmpty() ){
    QFileInfo fInfo( fn );
    TTCut::lastDirPath = fInfo.absolutePath();
    emit fileOpened( fn );
  }
}
