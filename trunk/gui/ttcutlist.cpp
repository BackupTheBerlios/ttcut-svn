/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / ttcut.tritime.org                       */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttcutlist.cpp                                                   */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 02/26/2006 */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTCUTLIST
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

#include "ttcutlist.h"

#include <QMenu>
#include <QPoint>
#include <QHeaderView>
#include <QModelIndex>

//! Construct the cut list object
TTCutList::TTCutList(QWidget* parent)
  :QWidget(parent)
{
  setupUi( this );

  // not implemented
  pbCutAudio->setEnabled(false);
  
  // set list view header (column) width
  videoCutList->setRootIsDecorated( false );
  QHeaderView* header = videoCutList->header();
  header->resizeSection(0, 200);
  header->resizeSection(1, 140);
  header->resizeSection(2, 140);
  header->resizeSection(3, 150);
  header->resizeSection(4,  80);
  
  // data struct for cut list view
  cutListData   = NULL;
  editItemIndex = -1;
  
  // actions for context menu
  createActions();

  // signal and slot connections
  connect(pbEntryUp,       SIGNAL(clicked()), SLOT(onEntryUp()));
  connect(pbEntryDown,     SIGNAL(clicked()), SLOT(onEntryDown()));
  connect(pbEntryDelete,   SIGNAL(clicked()), SLOT(onEntryDelete()));
  connect(pbPreview,       SIGNAL(clicked()), SLOT(onPreview()));
  connect(pbCutAudioVideo, SIGNAL(clicked()), SLOT(onAVCut()));
  connect(pbCutAudio,      SIGNAL(clicked()), SLOT(onAudioCut()));
  connect(videoCutList,    SIGNAL(itemClicked( QTreeWidgetItem*, int)), SLOT(onEntrySelected(QTreeWidgetItem*, int)));
  connect(videoCutList,    SIGNAL( customContextMenuRequested( const QPoint& ) ), SLOT( onContextMenuRequest( const QPoint& ) ) );
}

//! Neede by Qt Designer
void TTCutList::setTitle( __attribute__ ((unused))const QString& title )
{
}

//! Set the cut list data object
void TTCutList::setListData( TTCutListData* ld )
{
  cutListData = ld;
}

void TTCutList::clearList()
{
  videoCutList->clear();

  if (ttAssigned(cutListData)) {
    cutListData->deleteAll();
    emit refreshDisplay();
  }
}

void TTCutList::focusOutEvent(QFocusEvent* event) 
{
  if (videoCutList->currentItem() != NULL) {
    qDebug("cut list lost focus");
    videoCutList->setCurrentItem(NULL);
  }
}

//! Add or update an entry in the cut list
void TTCutList::onAddEntry(int cutIn, int cutOut)
{
  int cutIndex;

  if ( ttAssigned(cutListData) ) {

    if (editItemIndex < 0) {
      cutIndex = cutListData->addItem( cutIn, cutOut );

      QTreeWidgetItem* treeItem = new QTreeWidgetItem(videoCutList);
      treeItem->setText(0, cutListData->streamFileName());
      treeItem->setText(1, cutListData->cutInPosString(cutIndex));
      treeItem->setText(2, cutListData->cutOutPosString(cutIndex));
      treeItem->setText(3, cutListData->cutLengthString(cutIndex));
    } else {
      cutListData->updateItem(editItemIndex, cutIn, cutOut);
      QTreeWidgetItem* editItem = videoCutList->topLevelItem(editItemIndex);
      editItem->setText(0, cutListData->streamFileName());
      editItem->setText(1, cutListData->cutInPosString(editItemIndex));
      editItem->setText(2, cutListData->cutOutPosString(editItemIndex));
      editItem->setText(3, cutListData->cutLengthString(editItemIndex));

      editItem->setBackgroundColor(0, Qt::white);
      editItem->setBackgroundColor(1, Qt::white);
      editItem->setBackgroundColor(2, Qt::white);
      editItem->setBackgroundColor(3, Qt::white);
      editItem->setBackgroundColor(4, Qt::white);

      editItemIndex = -1;
    }
    emit refreshDisplay();
    //cutListData->print();
  }
}

//! Move the currently selected entry up
void TTCutList::onEntryUp()
{
  if (videoCutList->currentItem() != NULL &&
      editItemIndex < 0) {
    // current index
    int index = videoCutList->indexOfTopLevelItem(videoCutList->currentItem());

    if (index > 0) {
      // take current item form list but don't delete it
      QTreeWidgetItem* curItem = videoCutList->takeTopLevelItem(index);

      // insert new item
      videoCutList->insertTopLevelItem(index-1, curItem);       
      videoCutList->setCurrentItem(curItem);

      cutListData->swap(index, index-1);
      //cutListData->print();
    }
  }
}

//! Move the currently selected entry down
void TTCutList::onEntryDown()
{
  if (videoCutList->currentItem() != NULL &&
      editItemIndex < 0) {
    // current index
    int index = videoCutList->indexOfTopLevelItem(videoCutList->currentItem());

    if (index < videoCutList->topLevelItemCount()-1) {
      // take current item from list but don't delete it
      QTreeWidgetItem* curItem = videoCutList->takeTopLevelItem(index);

      // insert new item
      videoCutList->insertTopLevelItem(index+1, curItem);       
      videoCutList->setCurrentItem(curItem);

      cutListData->swap(index, index+1);
      //cutListData->print();
    }
  }
}

//! Remove the currently selected entry from the list
void TTCutList::onEntryDelete()
{
  if (videoCutList->currentItem() != NULL &&
      editItemIndex < 0) {
    // current index
    int index = videoCutList->indexOfTopLevelItem(videoCutList->currentItem());

    // remove current item from list
    delete videoCutList->takeTopLevelItem(index);

    cutListData->removeAt(index);
    emit refreshDisplay();
    //cutListData->print();
  }
}

//! Entry selected; Selection changed
void TTCutList::onEntrySelected(__attribute((unused))QTreeWidgetItem* item, __attribute__((unused))int column)
{
  if (ttAssigned(cutListData)) {
    int index = videoCutList->indexOfTopLevelItem(videoCutList->currentItem());
    emit entrySelected(cutListData->cutOutPos(index));
  }
}

//! Edit selected entry
void TTCutList::onEntryEdit()
{
  if (ttAssigned(cutListData)) {
    QTreeWidgetItem* curItem = videoCutList->currentItem();
    int index = videoCutList->indexOfTopLevelItem(curItem);
    curItem->setBackgroundColor(0, Qt::lightGray);
    curItem->setBackgroundColor(1, Qt::lightGray);
    curItem->setBackgroundColor(2, Qt::lightGray);
    curItem->setBackgroundColor(3, Qt::lightGray);
    curItem->setBackgroundColor(4,Qt::lightGray);
    videoCutList->setCurrentItem(NULL);
    editItemIndex = index;
    emit entryEdit(cutListData->at(index));
  }
}

//! Goto cut-in action
void TTCutList::onGotoCutIn()
{
  if (ttAssigned(cutListData)) {
    int index = videoCutList->indexOfTopLevelItem(videoCutList->currentItem());
    emit gotoCutIn(cutListData->cutInPos(index));
  }
}

//! Goto cut-out action
void TTCutList::onGotoCutOut()
{
  if (ttAssigned(cutListData)) {
    int index = videoCutList->indexOfTopLevelItem(videoCutList->currentItem());
    emit gotoCutOut(cutListData->cutOutPos(index));
  }
}

//! Preview the selected cut
void TTCutList::onEntryPreview()
{
  int index = videoCutList->indexOfTopLevelItem(videoCutList->currentItem());
  emit previewCut(index);
}

//! Cut the selected cut
void TTCutList::onEntryCut()
{
  int index = videoCutList->indexOfTopLevelItem(videoCutList->currentItem());
  emit audioVideoCut(index);
}

//! Preview button action
void TTCutList::onPreview()
{
  emit previewCut(-1);
}

//! Cut audio+video button action
void TTCutList::onAVCut()
{
  emit audioVideoCut(-1);
}

//! Cut audio only button action
void TTCutList::onAudioCut()
{
  videoCutList->indexOfTopLevelItem(videoCutList->currentItem());
  emit audioCut(-1);
}

void TTCutList::onEditCutOut(int cutOut)
{
  if (videoCutList->currentItem() != NULL && ttAssigned(cutListData)) {

    QTreeWidgetItem* curItem = videoCutList->currentItem();
    int index = videoCutList->indexOfTopLevelItem(curItem);

    cutListData->setCutOutPosAt(index, cutOut);
    curItem->setText(2, cutListData->cutOutPosString(index));
    curItem->setText(3, cutListData->cutLengthString(index));
  }
}

//! Contex menu requested
void TTCutList::onContextMenuRequest( const QPoint& point)
{
  if(videoCutList->currentItem() != NULL) {

    QMenu contextMenu(this);

    contextMenu.addAction(gotoCutInAction);
    contextMenu.addAction(gotoCutOutAction);
    contextMenu.addSeparator();
    contextMenu.addAction(itemUpAction);
    contextMenu.addAction(itemDeleteAction);
    contextMenu.addAction(itemDownAction);
    contextMenu.addSeparator();
    contextMenu.addAction(itemPreviewAction);
    contextMenu.addSeparator();
    contextMenu.addAction(itemEditAction);

    contextMenu.exec(videoCutList->mapToGlobal(point));
  }
}

//! Create the actions used by the context menu.
void TTCutList::createActions()
{
  itemUpAction = new QAction(tr("Move &up"), this);
  itemUpAction->setIcon(QIcon(QString::fromUtf8(":/pixmaps/pixmaps/uparrow_18.xpm")));
  itemUpAction->setStatusTip(tr("Move selected cut one position upward"));
  connect(itemUpAction, SIGNAL(triggered()), this, SLOT(onEntryUp()));

  itemDeleteAction = new QAction(tr("&Delete"), this);
  itemDeleteAction->setIcon(QIcon(QString::fromUtf8(":/pixmaps/pixmaps/cancel_18.xpm")));
  itemDeleteAction->setStatusTip(tr("Remove selected cut from list"));
  connect(itemDeleteAction, SIGNAL(triggered()), this, SLOT(onEntryDelete()));

  itemDownAction = new QAction(tr("Move d&own"), this);
  itemDownAction->setIcon(QIcon(QString::fromUtf8(":/pixmaps/pixmaps/downarrow_18.xpm")));
  itemDownAction->setStatusTip(tr("Move selected cut one position downward"));
  connect(itemDownAction, SIGNAL(triggered()), this, SLOT(onEntryDown()));

  itemEditAction = new QAction(tr("Edit &cut"), this);
  //itemEditAction->setIcon(QIcon(QString::fromUtf8(":/pixmaps/pixmaps/downarrow_18.xpm")));
  itemEditAction->setStatusTip(tr("Edit selected cut"));
  connect(itemEditAction, SIGNAL(triggered()), this, SLOT(onEntryEdit()));

  itemPreviewAction = new QAction(tr("Preview cut"), this);
  itemPreviewAction->setIcon(QIcon(QString::fromUtf8(":/pixmaps/pixmaps/preview_18.xpm")));
  itemPreviewAction->setStatusTip(tr("Preview selected cut"));
  connect(itemPreviewAction, SIGNAL(triggered()), this, SLOT(onEntryPreview()));

  gotoCutInAction = new QAction(tr("Goto Cut-In"), this);
  //gotoCutInAction->setIcon(QIcon(QString::fromUtf8(":/pixmaps/pixmaps/downarrow_18.xpm")));
  gotoCutInAction->setStatusTip(tr("Goto selected cut-in position"));
  connect(gotoCutInAction, SIGNAL(triggered()), this, SLOT(onGotoCutIn()));

  gotoCutOutAction = new QAction(tr("Goto Cut-Out"), this);
  //gotoCutOutAction->setIcon(QIcon(QString::fromUtf8(":/pixmaps/pixmaps/downarrow_18.xpm")));
  gotoCutOutAction->setStatusTip(tr("Goto selected cut-out position"));
  connect(gotoCutOutAction, SIGNAL(triggered()), this, SLOT(onGotoCutOut())); 
}
