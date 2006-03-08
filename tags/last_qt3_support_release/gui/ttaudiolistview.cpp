/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttaudiolistview.cpp                                             */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 02/28/2005 */
/* MODIFIED: b. altendorf                                    DATE: 04/05/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTAUDIOLISTVIEW
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


#include "ttaudiolistview.h"
//Added by qt3to4:
#include <QMouseEvent>
#include <Q3PopupMenu>
 
const char c_name[] = "TTAUDIOLISTV. : ";

// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// TTAudioListItem
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// construct item
// -----------------------------------------------------------------------------
TTAudioListItem::TTAudioListItem( TTAudioListView* parent )
  : Q3ListViewItem( (Q3ListView*)parent )
{

}

TTAudioListItem::TTAudioListItem( TTAudioListView* parent, TTAudioListItem* after)
  : Q3ListViewItem( (Q3ListView*) parent, (Q3ListViewItem*)after )
{

}

TTAudioListItem::~TTAudioListItem()
{
  //qDebug( "%sdestructor call list item",c_name );
  //qDebug( "%saudio stream object: %d",c_name,audio_stream );
}

// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// TTAudioListView
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// construct listview object
// -----------------------------------------------------------------------------
TTAudioListView::TTAudioListView( QWidget* parent, const char* name )
  :Q3ListView( parent, name )
{
  //no sorting
  setSorting( -1, false);

  setSelectionMode(Q3ListView::Single);
  setAllColumnsShowFocus(TRUE);

  setColumnWidth( 0, 250 );
  setColumnWidth( 1, 120 );
  setColumnWidth( 2, 120 );
  setColumnWidth( 3, 120 );
  setColumnWidth( 4, 120 );
  setColumnWidth( 5, 120 );
  setColumnWidth( 6, 120 );

  // Item context menu
  // ---------------------------------------------------------------------------
  itemContextMenu = new Q3PopupMenu(this, "itemContextMenu");

  itemContextMenu->insertItem("Move &up",      this, SLOT(itemUp()),    0,0);
  itemContextMenu->insertItem("Move d&own",    this, SLOT(itemDown()),  0,1);
  itemContextMenu->insertItem("&Delete",       this, SLOT(deleteItem()),0,2);

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

// destruct item
// -----------------------------------------------------------------------------
TTAudioListView::~TTAudioListView()
{

}

// mouse press event inside listview
// -----------------------------------------------------------------------------
void TTAudioListView::contentsMousePressEvent( QMouseEvent *e )
{
  QPoint mousePressPos;

  // map mouse coordinates to viewport
  mousePressPos = contentsToViewport( e->pos());

  // select listview item by mouse coordinates
  TTAudioListItem* i = (TTAudioListItem*)itemAt( mousePressPos );

  if( i && !isSelected(i) )
    Q3ListView::contentsMousePressEvent(e);

  // right mouse button pressed for list context menu
  if( !i && e->button() == Qt::RightButton )
  {
    qDebug("list context menu not implemented (!)");
    return;    
  }

  // right mouse button pressed for item context menu
  if( i && e->button() == Qt::RightButton )
  {
    itemContextMenu->exec(QCursor::pos());
  }
}

// add a item to the list
// -----------------------------------------------------------------------------
void TTAudioListView::addItem( QString fName, TTAudioStream* a_stream )
{
  QString          szTemp;
  TTAudioListItem* listItem;
  QFileInfo        fInfo ( fName );

  //qDebug( "%sadd item: audio stream object: %d",c_name,a_stream );

  listItem = new TTAudioListItem( this, (TTAudioListItem*)lastItem() );

  listItem->audioFileInfo = fInfo;
  listItem->audioFileName = fInfo.fileName();

  // audio length
  listItem->audioLength.sprintf("%lld", a_stream->streamLengthByte());

  // get the first audio header
  audio_header = a_stream->headerAt(0);

  // layer
  listItem->version = audio_header->descString();
    
  // bitrate
  listItem->bitRate = audio_header->bitRateString();

  // samplerate
  listItem->sampleRate = audio_header->sampleRateString();

  // mode
  listItem->channelMode = audio_header->modeString();

  // TODO: add real delay value
  int delay = 0;
  listItem->delay.sprintf("%d", delay); 

  // pointer to audio stream object
  listItem->audio_stream = a_stream;

  listItem->setText( 0, listItem->audioFileName );
  listItem->setText( 1, listItem->audioLength );
  listItem->setText( 2, listItem->version );
  listItem->setText( 3, listItem->bitRate );
  listItem->setText( 4, listItem->sampleRate );
  listItem->setText( 5, listItem->channelMode );
  listItem->setText( 6, listItem->delay );

  setColumnWidth( 0, 180 );
  setColumnWidth( 1, 100 );
  setColumnWidth( 2, 130 );
  setColumnWidth( 3, 110 );
  setColumnWidth( 4, 100 );
  setColumnWidth( 5, 110 );
  setColumnWidth( 6,  80 );
}

// clear the whole list; remove all entrys from the list
// -----------------------------------------------------------------------------
void TTAudioListView::clearList()
{
  TTAudioListItem* item;

  //list-view iterator
  Q3ListViewItemIterator cutIt( this );

  //qDebug( "%sclear audio list: num items: %d",c_name,count() );
  //qDebug( "%s--------------------------------------------", c_name );

  // iterate trough the list and delete each item
  while ( cutIt.current() )
  {
    item = (TTAudioListItem*)cutIt.current();
    //qDebug( "%sremove item from list: audio stream object: %d",c_name,item->audio_stream );
    delete item->audio_stream;
    delete cutIt.current();
  }
  //qDebug( "%s--------------------------------------------",c_name );
}

int TTAudioListView::count()
{
  int list_count;

  list_count = childCount();
  TTCut::numAudioTracks = list_count;

  return list_count;
}

// delete selected item
// -----------------------------------------------------------------------------
void TTAudioListView::deleteItem()
{
  TTAudioListItem* item;

  if ( currentItem() )
  {
    item = (TTAudioListItem*)currentItem();
    delete item->audio_stream;

    delete currentItem();
    setSelected( currentItem(), true );

    TTCut::numAudioTracks = childCount();
  }
}

// move selected item one place up in the list
// -----------------------------------------------------------------------------
void TTAudioListView::itemUp()
{
  if ( !currentItem() )
    return;

  TTAudioListItem* above   = (TTAudioListItem*)currentItem()->itemAbove();
  TTAudioListItem* current = (TTAudioListItem*)currentItem();

  if ( above )
  {
    takeItem( above );
    TTAudioListItem* listItem = new TTAudioListItem( this, current);

    listItem->audioFileInfo = above->audioFileInfo;
    listItem->audioFileName = above->audioFileName;
    listItem->audioLength   = above->audioLength;
    listItem->version       = above->version;
    listItem->bitRate       = above->bitRate;
    listItem->sampleRate    = above->sampleRate;
    listItem->channelMode   = above->channelMode;
    listItem->delay         = above->delay;
    listItem->audio_stream  = above->audio_stream;

    listItem->setText( 0, listItem->audioFileName );
    listItem->setText( 1, listItem->audioLength );
    listItem->setText( 2, listItem->version );
    listItem->setText( 3, listItem->bitRate );
    listItem->setText( 4, listItem->sampleRate );
    listItem->setText( 5, listItem->channelMode );
    listItem->setText( 6, listItem->delay );

    delete above;
  }
}

// move selected item one place down in the list
// -----------------------------------------------------------------------------
void TTAudioListView::itemDown()
{
  if ( !currentItem() )
    return;

  TTAudioListItem* below   = (TTAudioListItem*)currentItem()->itemBelow();
  TTAudioListItem* current = (TTAudioListItem*)currentItem();

  if ( below )
  {
    takeItem( current );
    TTAudioListItem* listItem = new TTAudioListItem( this, below);

    listItem->audioFileInfo = current->audioFileInfo;
    listItem->audioFileName = current->audioFileName;
    listItem->audioLength   = current->audioLength;
    listItem->version       = current->version;
    listItem->bitRate       = current->bitRate;
    listItem->sampleRate    = current->sampleRate;
    listItem->channelMode   = current->channelMode;
    listItem->delay         = current->delay;
    listItem->audio_stream  = current->audio_stream;

    listItem->setText( 0, listItem->audioFileName );
    listItem->setText( 1, listItem->audioLength );
    listItem->setText( 2, listItem->version );
    listItem->setText( 3, listItem->bitRate );
    listItem->setText( 4, listItem->sampleRate );
    listItem->setText( 5, listItem->channelMode );
    listItem->setText( 6, listItem->delay );

    setSelected( listItem, true );

    delete current;
  }
}

int TTAudioListView::getAudioList( QList<TTAudioStream*>& a_list )
{
  int num_count;

  num_count = fillAudioList();

  a_list = audio_list;

  return num_count;
}


int TTAudioListView::fillAudioList()
{
  TTAudioListItem* item;

  audio_list.clear();

  Q3ListViewItemIterator audioIt( this );

  while( audioIt.current() )
  {
    item = (TTAudioListItem*)audioIt.current();
    audio_list.insert( audio_list.size(), item->audio_stream );
    audioIt++;
  }
  return audio_list.size()-audio_list.count(0);
}

void TTAudioListView::writeListToProject( TTCutProject* prj )
{
  TTAudioListItem* item;
  Q3ListViewItemIterator audioIt( this );

  prj->writeAudioSection( true );

  while( audioIt.current() )
  {
    item = (TTAudioListItem*)audioIt.current();

    prj->writeAudioFileName( (QString)item->audioFileInfo.absoluteFilePath() );

    audioIt++;
  }
  prj->writeAudioSection( false );
}








