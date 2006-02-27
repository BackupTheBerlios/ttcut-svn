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

  createActions();

  connect( pbAudioFileOpen,    SIGNAL( clicked() ), SLOT( onFileOpen() ) );
  connect( pbAudioEntryUp,     SIGNAL( clicked() ), SLOT( onItemUp() ) );
  connect( pbAudioEntryDown,   SIGNAL( clicked() ), SLOT( onItemDown() ) );
  connect( pbAudioEntryDelete, SIGNAL( clicked() ), SLOT( onDeleteItem() ) );
  connect( audioListView,      SIGNAL( customContextMenuRequested( const QPoint& ) ), SLOT( onContextMenuRequest( const QPoint& ) ) );
}

/*!
 * Set the group box title string. This method is needed by designer.
 */
void TTAudioFileList::setTitle ( const QString& title )
{
  gbAudioFiles->setTitle( title );
}

/*!
 * Enable or disable the audiolist widget
 */
void TTAudioFileList::setControlEnabled( bool enabled )
{
    pbAudioFileOpen->setEnabled( enabled );
    pbAudioEntryUp->setEnabled( enabled );
    pbAudioEntryDelete->setEnabled( enabled );
    pbAudioEntryDown->setEnabled( enabled );
    audioListView->setEnabled( enabled );
}

void TTAudioFileList::openAudioFile()
{
  onFileOpen();
}

/*!
 * Event handler for file open button clicked.
 */
void TTAudioFileList::onFileOpen()
{
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

/*!
 * Event handler for item up button clicked.
 */
void TTAudioFileList::onItemUp()
{
  qDebug("item up");
}

/*!
 * Event handler for item down button clicked.
 */
void TTAudioFileList::onItemDown()
{
  qDebug("item down");
}

/*!
 * Event handler for delete item button clicked.
 */
void TTAudioFileList::onDeleteItem()
{
  qDebug("delete item");
}

/*!
 * User requested a context menu.
 */
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

/*!
 * Create the actions used by the context menu.
 */
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
