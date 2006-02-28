/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2006                                                      */
/* FILE     : ttaudiofilelist.h                                               */
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

#ifndef TTAUDIOFILELIST_H
#define TTAUDIOFILELIST_H

#include "ui_audiofilelistwidget.h"

#include "../common/ttcut.h"

class TTAudioListDataItem;
class TTAudioListData;
class QMenu;
class QAction;

class TTAudioFileList : public QWidget, Ui::TTAudioFileListWidget
{
  Q_OBJECT

  public:
    TTAudioFileList(QWidget* parent=0);

    void setTitle ( const QString & title );
    void setListData(TTAudioListData* ad);
    void setControlEnabled( bool enabled );
    void openAudioFile();
    void addItem(TTAudioListDataItem& item);
    void clearList();

  signals:
    void fileOpened( const QString& fName );
    void itemDeleted( QTreeWidgetItem* item );
    void itemPositionChanged( QTreeWidgetItem* item );

  protected slots:
    void onFileOpen();
    void onItemUp();
    void onItemDown();
    void onDeleteItem();
    void onContextMenuRequest( const QPoint& point );

  private:
    void createActions();

  private:
    TTAudioListData* audioListData;
    QMenu*           contextMenu;
    QAction*         itemUpAction;
    QAction*         itemDownAction;
    QAction*         itemDeleteAction;
    QAction*         itemNewAction;
};

#endif
