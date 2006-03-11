/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / ttcut.tritime.org                       */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttcutlist.h                                                     */
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

#ifndef TTCUTLIST_H 
#define TTCUTLIST_H

#include "ui_cutlistwidget.h"

#include "../common/ttcut.h"
#include "../data/ttcutlistdata.h"

class QMenu;
class QAction;

class TTCutList : public QWidget, Ui::TTCutListWidget
{
  Q_OBJECT

  public:
    TTCutList(QWidget* parent=0);

    void setTitle( const QString& title );

    void setListData( TTCutListData* ld );
    void clearList();
   
  public slots:
    void onAddEntry(int cutIn, int cutOut);
    void onEntryUp();
    void onEntryDown();
    void onEntryDelete();
    void onEntrySelected(QTreeWidgetItem* item, int column);
    void onEntryEdit();
    void onGotoCutIn();
    void onGotoCutOut();
    void onEntryPreview();
    void onEntryCut();
    void onPreview();
    void onAVCut();
    void onAudioCut();
    void onEditCutOut(int cutOut);
    void onContextMenuRequest(const QPoint& point);

  signals:
    void entrySelected(int cutOutIndex );
    void entryEdit(const TTCutListDataItem& cutData);
    void gotoCutIn(int cutInIndex);
    void gotoCutOut(int cutOutIndex);
    void previewCut(int index);
    void audioVideoCut(int index);
    void audioCut(int index);
    void refreshDisplay();
 
  private:
    void createActions();

  private:
    TTCutListData* cutListData;
    int            editItemIndex;
    QMenu*         contextMenu;
    QAction*       itemUpAction;
    QAction*       itemDownAction;
    QAction*       itemDeleteAction;
    QAction*       itemEditAction;
    QAction*       gotoCutInAction;
    QAction*       gotoCutOutAction;
    QAction*       itemPreviewAction;
};

#endif
