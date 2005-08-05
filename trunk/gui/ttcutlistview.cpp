/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttcutlistview.cpp                                               */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 02/24/2005 */
/* MODIFIED: b. altendorf                                    DATE: 03/15/2005 */
/* MODIFIED: b. altendorf                                    DATE: 06/20/2005 */
/* MODIFIED: b. altendorf                                    DATE: 06/22/2005 */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTCUTLISTVIEW
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


#include "ttcutlistview.h"
//Added by qt3to4:
#include <QMouseEvent>
#include <Q3PopupMenu>


const char c_name[] = "TTCUTLISTVIEW : ";

// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// TTCutListItem
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////
TTCutListItem::TTCutListItem( TTCutListView* parent )
  : Q3ListViewItem( (Q3ListView*)parent )
{
  cut_order = (uint)0;

  setText( 0, " ---- " );
  setText( 1, "00:00:00,000 (0)" );
  setText( 2, "00:00:00,000 (0)" );
}


TTCutListItem::TTCutListItem( TTCutListView* parent, TTCutListItem* after)
  : Q3ListViewItem( (Q3ListView*) parent, (Q3ListViewItem*)after )
{
  cut_order = (uint)0;

  setText( 0, " ---- " );
  setText( 1, "00:00:00,000 (0)" );
  setText( 2, "00:00:00,000 (0)" );
}


// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// TTCutListView
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// construct a list view object
// -----------------------------------------------------------------------------
TTCutListView::TTCutListView( QWidget* parent, const char* name )
  :Q3ListView( parent, name )
{
  // AV cut list object
  avcut_list = NULL;
  avcut_list = new TTAVCutList();

  //no sorting
  setSorting( -1, false);

  setSelectionMode(Q3ListView::Single);
  setAllColumnsShowFocus(TRUE);

  setColumnWidth( 0, 450 );
  setColumnWidth( 1, 250 );
  setColumnWidth( 2, 250 );

  // Item context menu
  // ---------------------------------------------------------------------------
  itemContextMenu = new Q3PopupMenu(this, "itemContextMenu");

  itemContextMenu->insertItem("Goto cut &in",  this, SLOT(gotoCutIn()),       0,0);
  itemContextMenu->insertItem("Goto cut &out", this, SLOT(gotoCutOut()),      0,1);
  itemContextMenu->insertSeparator(2);
  itemContextMenu->insertItem("Move &up",      this, SLOT(itemUp()),          0,3);
  itemContextMenu->insertItem("Move d&own",    this, SLOT(itemDown()),        0,4);
  itemContextMenu->insertItem("&Delete",       this, SLOT(deleteItem()),      0,5);
  itemContextMenu->insertSeparator(6);
  itemContextMenu->insertItem("&Preview",      this, SLOT(singleCutPreview()),0,7);
  itemContextMenu->insertSeparator(8);
  itemContextMenu->insertItem("&Edit",         this, SLOT(editItem()),        0,9);

  // Listview context menue
  // ---------------------------------------------------------------------------
  //listContextMenu = new QPopupMenu(this, "listContextMenu");

  //listContextMenu->insertItem("tiny",       this, SLOT(setThumbsSize(int )), 0, 0);
  //listContextMenu->insertItem("small",      this, SLOT(setThumbsSize(int )), 0, 1);
  //listContextMenu->insertItem("medium",     this, SLOT(setThumbsSize(int )), 0, 2);
  //listContextMenu->insertItem("large",      this, SLOT(setThumbsSize(int )), 0, 3);
  //listContextMenu->insertItem("huge",       this, SLOT(setThumbsSize(int )), 0, 4);
  //listContextMenu->insertItem("gigantic",   this, SLOT(setThumbsSize(int )), 0, 5);
}

// destructor
// -----------------------------------------------------------------------------
TTCutListView::~TTCutListView()
{
  // delete cut list object
  if( ttAssigned( avcut_list ) )
    delete avcut_list;
}

// Processes the mouse press event e on behalf of the viewed widget
// -----------------------------------------------------------------------------
void TTCutListView::contentsMousePressEvent( QMouseEvent *e )
{
  QPoint mousePressPos;

  // map mouse coordinates to viewport
  mousePressPos = contentsToViewport( e->pos());

  // select listview item by mouse coordinates
  TTCutListItem* i = (TTCutListItem*)itemAt( mousePressPos );

  // base class event handling; select item
  if( i && !isSelected(i) )
    Q3ListView::contentsMousePressEvent(e);

  // item is selected and mouse button is left buttom; select current
  // cut-out frame
  if( i && e->button() == Qt::LeftButton )
  {
    cut_position = avcut_list->entryAtOrder( i->cut_order );

    //qDebug( "%sselect item at order: %d",c_name,i->cut_order );

    // emit signal and pass reference of cut position to receiver
    emit selectCutOut( *cut_position );
    return;
  }

  // ask for list view context menu
  if( !i && e->button() == Qt::RightButton )
  {
    qDebug("list context menu not implemented (!)");
    return;
  }

  // ask for item context menu
  if( i && e->button() == Qt::RightButton )
  {
    itemContextMenu->exec(QCursor::pos());
  }
}

// add a item at the end of the list view
// -----------------------------------------------------------------------------
void TTCutListView::addItem( QString f_name, 
			     uint c_in_index, QTime c_in_time,
			     uint c_out_index, QTime c_out_time )
{
  uint           current_order;
  QString        sz_temp;
  TTCutListItem* list_item;

  // current order is the lists entry count
  current_order = childCount();

  //qDebug( "%sadd item at order: %d",c_name,current_order );

  // new list item
  list_item = new TTCutListItem( this, (TTCutListItem*)lastItem() );

  // set item text
  list_item->cut_order = current_order;
  list_item->setText( 0, f_name );
  sz_temp.sprintf( "%s (%d)", c_in_time.toString("hh:mm:ss.zzz").ascii(), c_in_index );
  list_item->setText( 1, sz_temp );
  sz_temp.sprintf( "%s (%d)", c_out_time.toString("hh:mm:ss.zzz").ascii(), c_out_index );
  list_item->setText( 2, sz_temp);

  // set column width
  setColumnWidth( 0, 250 );
  setColumnWidth( 1, 150 );
  setColumnWidth( 2, 150 );

  deselectAll();

  // add the new cut position to the av cut list
  avcut_list->addCutPosition( c_in_index, c_out_index, current_order );
  
  // calculate the new resulting video length 
  calcResultCutLen();

  //testListView();
}

// add a item at the end of the list view
// -----------------------------------------------------------------------------
void TTCutListView::addItem( QString f_name, 
			     uint c_in_index, off64_t c_in_off, QTime c_in_time,
			     uint c_out_index, off64_t c_out_off, QTime c_out_time )
{
  uint           current_order;
  QString        sz_temp;
  TTCutListItem* list_item;

  // current order is the lists entry count
  current_order = childCount();

  //qDebug( "%sadd item at order: %d",c_name,current_order );

  // new list item
  list_item = new TTCutListItem( this, (TTCutListItem*)lastItem() );

  // set item text
  list_item->cut_order = current_order;
  list_item->setText( 0, f_name );
  sz_temp.sprintf( "%s (%d)", c_in_time.toString("hh:mm:ss.zzz").ascii(), c_in_index );
  list_item->setText( 1, sz_temp );
  sz_temp.sprintf( "%s (%d)", c_out_time.toString("hh:mm:ss.zzz").ascii(), c_out_index );
  list_item->setText( 2, sz_temp);

  // set column width
  setColumnWidth( 0, 250 );
  setColumnWidth( 1, 150 );
  setColumnWidth( 2, 150 );

  deselectAll();

  // add the new cut position to the av cut list
  avcut_list->addCutPosition( c_in_index, c_in_off,
			      c_out_index, c_out_off, current_order );
  
  // calculate the new resulting video length 
  calcResultCutLen();

  //testListView();
}

// clear the list view; remove all items from the list
// -----------------------------------------------------------------------------
void TTCutListView::clearList()
{
  //list-view iterator
  Q3ListViewItemIterator cutIt( this );

  // iterate trough the list
  while ( cutIt.current() )
  {
     delete cutIt.current();
  }

  //testListView();
}

// deselect all items
// -----------------------------------------------------------------------------
void TTCutListView::deselectAll()
{
  if ( currentItem() )
    setSelected( currentItem(), false );
}

// return true if currently one list item is selcted
// -----------------------------------------------------------------------------
bool TTCutListView::isItemSelected()
{
  if ( currentItem() )
    return isSelected( currentItem() );
  else
    return false;
}

// returns the number of parentless child (top-level) items in the list
// -----------------------------------------------------------------------------
int TTCutListView::count()
{
  return childCount();
}

// delete the currently selected item
// -----------------------------------------------------------------------------
void TTCutListView::deleteItem()
{
  if ( currentItem() )
  {
    // remove the corresponding entry in avcut list
    TTCutListItem* list_item = (TTCutListItem*)currentItem();

    //qDebug( "%sremove item at order: %d",c_name,list_item->cut_order );

    avcut_list->removeEntryAtOrder( list_item->cut_order );

    // delete item from the list view
    delete currentItem();

    // new order
    newOrder();

    // select the next current item
    setSelected( currentItem(), true );

    // new resulting video length
    calcResultCutLen();

    //testListView();
  }
}

// edit selected list view item data
// -----------------------------------------------------------------------------
void TTCutListView::editItem()
{
  // no item selected
  if ( !currentItem() )
    return;

  // get pointer to current selected item
  TTCutListItem* current = (TTCutListItem*)currentItem();

  // get according cut position from av cut list data
  //qDebug( "%sedit item at order: %d",c_name,current->cut_order );

  cut_position = avcut_list->entryAtOrder( current->cut_order );

  // create copy
  TTAVCutPosition temp_pos( cut_position->cutIn(), cut_position->cutOut(), cut_position->cutOrder() );

  // remove the entry from the cut list
  avcut_list->removeEntryAtOrder( current->cut_order );

  // remove the current item from list view
  delete current;

  // new order
  newOrder();

  // new resulting length
  calcResultCutLen();

  //testListView();

  // emit signal an pass reference of cut position to receiver
  emit editCutListItem( temp_pos );
}

// refresh the list's order
void TTCutListView::newOrder()
{
  uint old_order;
  uint new_order = (uint)0;
  TTCutListItem* list_item;

  //list-view iterator
  Q3ListViewItemIterator cutIt( this );

  // iterate trough the list
  while ( cutIt.current() )
  {
    list_item = (TTCutListItem*)cutIt.current();
    old_order = list_item->cut_order;
    list_item->cut_order = new_order;

    new_order++;
    cutIt++;
  }

  //testListView();
}

// move the selected item one step up in the list
// -----------------------------------------------------------------------------
void TTCutListView::itemUp()
{
  uint order_current;
  uint order_above;

  // no item selected
  if ( !currentItem() )
    return;

  // pointer to current and above item (precursor)
  TTCutListItem* above   = (TTCutListItem*)currentItem()->itemAbove();
  TTCutListItem* current = (TTCutListItem*)currentItem();

  // item have a precursor
  if ( above )
  {
    // get the cut order
    order_current = current->cut_order;
    order_above   = above->cut_order;

    // take item from list but don't delete it
    takeItem( above );

    // create a new item after current position
    TTCutListItem* list_item = new TTCutListItem( this, current);

    // copy data from above item to new item
    list_item->setText( 0 , above->text( 0 ) );
    list_item->setText( 1 , above->text( 1 ) );
    list_item->setText( 2 , above->text( 2 ) );

    // swap the cut order
    list_item->cut_order = order_current;
    current->cut_order   = order_above;

    avcut_list->entryAtOrder( order_current )->setCutOrder( order_above );
    avcut_list->entryAtOrder( order_above )->setCutOrder( order_current );

    avcut_list->sort();

    // remove above item from list
    delete above;

    //testListView();
  }
}

// move the selected item one step down in the list
// -----------------------------------------------------------------------------
void TTCutListView::itemDown()
{
  uint order_current;
  uint order_below;

  // no item selected
  if ( !currentItem() )
    return;

  // pointer to current and above item (precursor)
  TTCutListItem* below   = (TTCutListItem*)currentItem()->itemBelow();
  TTCutListItem* current = (TTCutListItem*)currentItem();

  // item have a successor
  if ( below )
  {
    // get the cut order
    order_current = current->cut_order;
    order_below   = below->cut_order;

    // take current item from list but don't delete it
    takeItem( current );

    // create a new item after below position
    TTCutListItem* list_item = new TTCutListItem( this, below );

    // copy data from above item to new item
    list_item->setText( 0 , current->text( 0 ) );
    list_item->setText( 1 , current->text( 1 ) );
    list_item->setText( 2 , current->text( 2 ) );

    setSelected( list_item, true );

    // swap the cut order
    list_item->cut_order = order_below;
    below->cut_order     = order_current;

    avcut_list->entryAtOrder( order_current )->setCutOrder( order_below );
    avcut_list->entryAtOrder( order_below )->setCutOrder( order_current );

    avcut_list->sort();

    // remove current item from list
    delete current;

    //testListView();
  }
}

// item context menu: goto cut-in position
// -----------------------------------------------------------------------------
void TTCutListView::gotoCutIn()
{
  // no item selected
  if ( !currentItem() )
    return;

  // get pointer to current item
  TTCutListItem* current = (TTCutListItem*)currentItem();

  cut_position = avcut_list->entryAtOrder( current->cut_order );

  // emit signal and pass reference to selected cut position
  emit showCutIn( *cut_position );
}

// item context menu: goto cut-out position
// -----------------------------------------------------------------------------
void TTCutListView::gotoCutOut()
{
  // no item selected
  if ( !currentItem() )
    return;

  // get pointer to current item
  TTCutListItem* current = (TTCutListItem*)currentItem();

  cut_position = avcut_list->entryAtOrder( current->cut_order );

  // emit signal and pass reference to selected cut position
  emit showCutOut( *cut_position );
}

// cut-out position changed
// -----------------------------------------------------------------------------
void TTCutListView::cutOutIndexChanged( uint c_out_index, QTime c_out_time )
{
  QString sz_temp;

  // no item selected
  if ( !currentItem() )
    return;

  // get pointer to current item
  TTCutListItem* current = (TTCutListItem*)currentItem();

  avcut_list->entryAtOrder( current->cut_order )->setCutOut( c_out_index );

  // set new item text
  sz_temp.sprintf( "%s (%d)", c_out_time.toString("hh:mm:ss.zzz").ascii(), c_out_index );
  current->setText( 2, sz_temp );

  // new resulting video length
  calcResultCutLen();
}

// create cut position for single preview
// -----------------------------------------------------------------------------
void TTCutListView::singleCutPreview()
{
  // no item selected
  if ( !currentItem() )
    return;

  // get pointer to current item
  TTCutListItem* current = (TTCutListItem*)currentItem();

  emit previewSingleCut( current->cut_order );
}

TTAVCutList* TTCutListView::cutList()
{
  return avcut_list;
}

// calculate the resulting video length (in frames)
// -----------------------------------------------------------------------------
void TTCutListView::calcResultCutLen()
{
  uint    i;
  uint    start_pos;
  uint    end_pos;
  off64_t start_off;
  off64_t end_off;
  
  // reset video cut length
  video_cut_length = (uint)0;
  video_cut_size   = (off64_t)0;

  avcut_list->sortCutOrder();

  // walk through the av cut list
  for ( i = (uint)0; i < (uint)avcut_list->count(); (uint)i++ )
  {
    start_pos = avcut_list->cutInAt( (uint)i );
    end_pos   = avcut_list->cutOutAt( (uint)i );

    start_off = avcut_list->cutInOffsetAt( (uint)i );
    end_off   = avcut_list->cutOutOffsetAt( (uint)i );

    video_cut_length += end_pos - start_pos;
    video_cut_size   += end_off - start_off;
  }
      
  //qDebug( "%snew cut length: %d %ld",c_name,video_cut_length,video_cut_size );

  emit newCutLen( video_cut_length, video_cut_size );
}

// return the resulting video cut length (in frames)
// -----------------------------------------------------------------------------
uint TTCutListView::getResultFrames()
{
  return video_cut_length;
}


void TTCutListView::testListView()
{
  TTCutListItem* list_item;
  uint           list_order = 0;

  qDebug( "%savcut_list count: %d",c_name,avcut_list->count() );
  qDebug( "%slist view  count: %d",c_name,count() );
  qDebug( "%s----------------------------------------",c_name );


  //list-view iterator
  Q3ListViewItemIterator cutIt( this );

  // iterate trough the list
  while ( cutIt.current() )
  {
    list_item = (TTCutListItem*)cutIt.current();
    
    qDebug( "%slist / view order: %d - %d / %d",c_name,list_order,
	    list_item->cut_order,
	    avcut_list->entryAtOrder( list_item->cut_order )->cutOrder() );

    list_order++;
    cutIt++;
  }
}
