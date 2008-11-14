/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2006                                                      */
/* FILE     : ttaudiofilelist.cpp                                             */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 02/28/2005 */
/* MODIFIED: b. altendorf                                    DATE: 02/19/2006 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTAUDIOFILELIST
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


#include "ttaudiofilelist.h"
#include "../data/ttaudiolistdata.h"

#include <QHeaderView>
#include <QFileDialog>
#include <QFileInfo>
#include <QMenu>
#include <QAction>

/*!
 * Construct a new TTAudioFileList widget.
 */
TTAudioFileList::TTAudioFileList(QWidget* parent)
  :QWidget(parent)
{
  setupUi( this );

  // set list view header (column) width
  audioListView->setRootIsDecorated(false);
  QHeaderView* header = audioListView->header();
  header->resizeSection(0, 220);
  header->resizeSection(1, 220);
  header->resizeSection(2, 140);
  header->resizeSection(3, 100);
  header->resizeSection(4, 100);
  header->resizeSection(5, 100);
  header->resizeSection(6,  60);

  // data struct for audio list view
  audioListData = NULL;

  // actions for context menu
  createActions();

  // signal and slot connections
  connect( pbAudioFileOpen,    SIGNAL( clicked() ), SLOT( onFileOpen() ) );
  connect( pbAudioEntryUp,     SIGNAL( clicked() ), SLOT( onItemUp() ) );
  connect( pbAudioEntryDown,   SIGNAL( clicked() ), SLOT( onItemDown() ) );
  connect( pbAudioEntryDelete, SIGNAL( clicked() ), SLOT( onDeleteItem() ) );
  connect( audioListView,      SIGNAL( customContextMenuRequested( const QPoint& ) ), SLOT( onContextMenuRequest( const QPoint& ) ) );
}

//! Set the group box title string. This method is needed by designer.
void TTAudioFileList::setTitle ( __attribute__((unused))const QString& title )
{
  //gbAudioFiles->setTitle( title );
}

//! Set the audio list data object
void TTAudioFileList::setListData(TTAudioListData* ad)
{
  audioListData = ad;

  // add all files from the audio list data to the list
  audioListView->clear();
  for ( int i=0; i<audioListData->count(); ++i )
    addItem( audioListData->itemAt(i) );
}

//! Enable or disable the audiolist widget
void TTAudioFileList::setControlEnabled( bool enabled )
{
  pbAudioFileOpen->setEnabled( enabled );
  pbAudioEntryUp->setEnabled( enabled );
  pbAudioEntryDelete->setEnabled( enabled );
  pbAudioEntryDown->setEnabled( enabled );
  audioListView->setEnabled( enabled );
}

void TTAudioFileList::clearList()
{
  audioListView->clear();

  if (ttAssigned(audioListData)) {
    audioListData->deleteAll();
  }
}

//! Open audio file
void TTAudioFileList::openAudioFile()
{
  onFileOpen();
}

//! Add audio item
void TTAudioFileList::addItem(TTAudioListDataItem& item)
{
  if (ttAssigned(audioListData)){
    QTreeWidgetItem* treeItem = new QTreeWidgetItem(audioListView);
    treeItem->setText(0, item.getFileName());
    treeItem->setText(1, item.getLength());
    treeItem->setText(2, item.getVersion());
    treeItem->setText(3, item.getBitrate());
    treeItem->setText(4, item.getSamplerate());
    treeItem->setText(5, item.getMode());
    treeItem->setText(6, item.getDelay());
  }
}

//! Event handler for file open button clicked
void TTAudioFileList::onFileOpen()
{
  // no video loaded
  if (!TTCut::isVideoOpen)
    return;

  QString fn = QFileDialog::getOpenFileName( this,
      tr("Open audio file"),
      TTCut::lastDirPath,
      "Audio (*.mpa *.mp2 *.ac3)" );

  if( !fn.isEmpty() ) {
    QFileInfo fInfo( fn );
    TTCut::lastDirPath = fInfo.absolutePath();
    emit fileOpened( fn );
  }
}

//! Event handler for item up button clicked
void TTAudioFileList::onItemUp()
{
  if (audioListView->currentItem() != NULL) {

    // current index
    int index = audioListView->indexOfTopLevelItem(audioListView->currentItem());

    if (index > 0) {
      // take current item from list but don't delete it
      QTreeWidgetItem* cutItem = audioListView->takeTopLevelItem(index);

      // insert new item
      audioListView->insertTopLevelItem(index-1, cutItem);
      audioListView->setCurrentItem(cutItem);

      audioListData->swap(index, index-1);
      //audioListData->print();
    }
  }
}

//! Event handler for item down button clicked
void TTAudioFileList::onItemDown()
{
  if (audioListView->currentItem() != NULL) {

    // current index
    int index = audioListView->indexOfTopLevelItem(audioListView->currentItem());

    if (index < audioListView->topLevelItemCount()-1) {

      // take current item from list but don't delete it
      QTreeWidgetItem* curItem = audioListView->takeTopLevelItem(index);

      audioListView->insertTopLevelItem(index+1, curItem);
      audioListView->setCurrentItem(curItem);

      audioListData->swap(index, index+1);
      //audioListData->print();
    }
  }
}

//! Event handler for delete item button clicked
void TTAudioFileList::onDeleteItem()
{

  if (audioListView->currentItem() != NULL) {

    // current index
    int index = audioListView->indexOfTopLevelItem(audioListView->currentItem());
    emit deleteVideo( index );
  }
}

void TTAudioFileList::removeItem( int index )
{
  if ( index >= 0 && index < audioListData->count() ) {
    // remove item from list
    delete audioListView->takeTopLevelItem(index);

    audioListData->removeAt(index);
    //audioListData->print();
  }
}

//! User requested a context menu
void TTAudioFileList::onContextMenuRequest( const QPoint& point )
{
  if(audioListView->currentItem() != NULL){
    QMenu contextMenu(this);
    contextMenu.addAction(itemNewAction);
    contextMenu.addSeparator();
    contextMenu.addAction(itemUpAction);
    contextMenu.addAction(itemDeleteAction);
    contextMenu.addAction(itemDownAction);

    contextMenu.exec(audioListView->mapToGlobal(point));
  }
}


//!Create the actions used by the context menu.
void TTAudioFileList::createActions()
{
  itemNewAction = new QAction(tr("&Insert audiofile"), this);
  //itemNewAction->setShortcut(tr("Ctrl+I"));
  itemNewAction->setIcon(QIcon(QString::fromUtf8(":/pixmaps/pixmaps/fileopen_24.xpm")));
  itemNewAction->setStatusTip(tr("Open a new audiofile and insert to list"));
  connect(itemNewAction, SIGNAL(triggered()), SLOT(onFileOpen()));

  itemUpAction = new QAction(tr("Move &up"), this);
  //itemUpAction->setShortcut(tr("Ctrl+U"));
  itemUpAction->setIcon(QIcon(QString::fromUtf8(":/pixmaps/pixmaps/uparrow_18.xpm")));
  itemUpAction->setStatusTip(tr("Move selected audiofile one position upward"));
  connect(itemUpAction, SIGNAL(triggered()), SLOT(onItemUp()));

  itemDeleteAction = new QAction(tr("&Delete"), this);
  //itemDeleteAction->setShortcut(tr("Ctrl+D"));
  itemDeleteAction->setIcon(QIcon(QString::fromUtf8(":/pixmaps/pixmaps/cancel_18.xpm")));
  itemDeleteAction->setStatusTip(tr("Remove selected audiofile from list"));
  connect(itemDeleteAction, SIGNAL(triggered()), SLOT(onDeleteItem()));

  itemDownAction = new QAction(tr("Move d&own"), this);
  //itemDownAction->setShortcut(tr("Ctrl+O"));
  itemDownAction->setIcon(QIcon(QString::fromUtf8(":/pixmaps/pixmaps/downarrow_18.xpm")));
  itemDownAction->setStatusTip(tr("Move selected audiofile one position downward"));
  connect(itemDownAction, SIGNAL(triggered()), SLOT(onItemDown()));
}
