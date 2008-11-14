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
#include "data/ttavdata.h"

#include <QFileInfo>
#include <QFileDialog>
#include <QHeaderView>
#include <QMenu>

/*
 * Constructor
 */
TTVideoFileInfo::TTVideoFileInfo(QWidget* parent)
  :QWidget(parent)
{
  setupUi( this );

  videoListView->setRootIsDecorated(false);
  QHeaderView* header = videoListView->header();
  header->resizeSection(0, 220);
  header->resizeSection(1, 220);
  header->resizeSection(2, 140);
  header->resizeSection(3, 80);
  header->resizeSection(4, 80);
  header->resizeSection(5, 100);
  header->resizeSection(6,  60);

  connect( pbVideoOpen,   SIGNAL(clicked()),   SLOT(onFileOpen()));
  connect( pbVideoDelete, SIGNAL(clicked()),   SLOT(onDeleteVideo()) );
  connect( videoListView, SIGNAL(customContextMenuRequested(const QPoint&)),
                                              SLOT(onContextMenuRequest(const QPoint& )) );
  connect( videoListView, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)),
                                              SLOT(onCurItemChange(QTreeWidgetItem*, QTreeWidgetItem*)) );
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


void TTVideoFileInfo::setVideoInfo( TTVideoStream* pVideoStream )
{
  QString szTemp;

  TTSequenceHeader* currentSequence = pVideoStream->currentSequenceHeader();

  if (currentSequence == NULL)
    return;

  int idx = videoListData.indexOf(pVideoStream);
  if ( -1 == idx )
    return;

  QTreeWidgetItem* treeItem = videoListView->topLevelItem( idx );
  if ( 0 == treeItem )
    return;

  // video file name
  treeItem->setText( 0, pVideoStream->fileName() );

  // video length
  int numFrames = pVideoStream->frameCount();
  QTime time = ttFramesToTime( numFrames, pVideoStream->frameRate() );
  szTemp.sprintf("%s (%d)", qPrintable(time.toString("hh:mm:ss.zzz")), numFrames);
  treeItem->setText( 1, szTemp );

  // set video resolution
  szTemp.sprintf("%dx%d",pVideoStream->currentSequenceHeader()->horizontalSize(),
                 pVideoStream->currentSequenceHeader()->verticalSize());
  treeItem->setText( 2, szTemp );

  // set aspect
  treeItem->setText( 3, pVideoStream->currentSequenceHeader()->aspectRatioText() );

  // set framerate
  treeItem->setText( 4, pVideoStream->currentSequenceHeader()->frameRateText() );

  // set bitrate
  szTemp.sprintf("%4.1f kBit/s",
                 pVideoStream->currentSequenceHeader()->bitRateKbit());
  treeItem->setText( 5, szTemp );

  // set VBV buffer size
  szTemp.sprintf("%d kWords",
                 pVideoStream->currentSequenceHeader()->vbvBufferSize());
  treeItem->setText( 6, szTemp );
}


void TTVideoFileInfo::addVideo( TTVideoStream* pVideoStream )
{
  if ( -1 == videoListData.indexOf(pVideoStream) ) {
    QTreeWidgetItem* pNewItem = new QTreeWidgetItem( videoListView );
    videoListData.append( pVideoStream );
    setVideoInfo( pVideoStream );
    videoListView->setCurrentItem( pNewItem );
  }
}


void TTVideoFileInfo::removeVideo( int idx )
{
  if ( idx >= 0 && idx < videoListData.count() ) {
    delete videoListView->takeTopLevelItem( idx );
    videoListData.removeAt(idx);
  }
}

void TTVideoFileInfo::onDeleteVideo()
{
  if ( videoListView->currentItem() != NULL ) {
    int index = videoListView->indexOfTopLevelItem( videoListView->currentItem() );
    emit deleteVideo( index );
  }
}


void TTVideoFileInfo::clearList()
{
  videoListView->clear();
}


/*
 * Show the file open dialog
 */
void TTVideoFileInfo::onFileOpen()
{
  QString fn = QFileDialog::getOpenFileName( this,
      tr("Open video file"),
      TTCut::lastDirPath,
      "Video (*.m2v *.mpv)" );

  if( !fn.isEmpty() )
  {
    QFileInfo fInfo( fn );
    TTCut::lastDirPath = fInfo.absolutePath();
    emit fileOpened( fn );
  }
}


void TTVideoFileInfo::onContextMenuRequest( const QPoint& point )
{
  if ( videoListView->currentItem() != NULL ) {
    QAction* itemNewAction = new QAction(tr("&Insert video"), this);
    itemNewAction->setIcon(QIcon(QString::fromUtf8(":/pixmaps/pixmaps/fileopen_24.xpm")));
    itemNewAction->setStatusTip(tr("Open a new video and insert to list"));
    connect(itemNewAction, SIGNAL(triggered()), SLOT(onFileOpen()));

    QAction* itemDeleteAction = new QAction(tr("&Delete video"), this);
    itemDeleteAction->setIcon(QIcon(QString::fromUtf8(":/pixmaps/pixmaps/cancel_18.xpm")));
    itemDeleteAction->setStatusTip(tr("Remove selected video from list"));
    connect(itemDeleteAction, SIGNAL(triggered()), SLOT(onDeleteVideo()));

    QMenu contextMenu(this);
    contextMenu.addAction(itemNewAction);
    contextMenu.addSeparator();
    contextMenu.addAction(itemDeleteAction);

    contextMenu.exec( videoListView->mapToGlobal(point) );

    delete itemNewAction;
    delete itemDeleteAction;
  }
}


void TTVideoFileInfo::onCurItemChange( QTreeWidgetItem* pNew, QTreeWidgetItem* pOld )
{
  if ( pNew == 0 || pNew == pOld )
    return;

  int idx = videoListView->indexOfTopLevelItem( pNew );
  if ( -1 != idx )
    emit changeVideo( videoListData[idx] );
}


void TTVideoFileInfo::onVideoChange( TTAVData* pAVData )
{
  int idx = videoListData.indexOf( pAVData->videoStream() );
  if ( -1 != idx )
    videoListView->setCurrentItem( videoListView->topLevelItem(idx) );
}

