/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttcutlistview.h                                                 */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 02/24/2005 */
/* MODIFIED: b. altendorf                                    DATE: 03/15/2005 */
/* MODIFIED: b. altendorf                                    DATE: 06/22/2005 */
/* MODIFIED:                                                 DATE:            */
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

#ifndef TTCUTLISTVIEW_H
#define TTCUTLISTVIEW_H

#include <qdatetime.h>
#include <q3listview.h>
#include <q3popupmenu.h>
#include <qcursor.h>
#include <qfileinfo.h>
//Added by qt3to4:
#include <QMouseEvent>

#include "ttcut.h"

// avcut
// -----------------------------------------------------------------------------
#include "../avcut/ttavcutposition.h"

class TTCutListView;

// -----------------------------------------------------------------------------
// TTAVCutListItem
// -----------------------------------------------------------------------------
class TTCutListItem : public Q3ListViewItem
{
  friend class TTCutListView;

 public:
  TTCutListItem( TTCutListView* parent );
  TTCutListItem( TTCutListView* parent, TTCutListItem* after );

 private:
  uint cut_order;
};


// -----------------------------------------------------------------------------
// TTCutListView
// -----------------------------------------------------------------------------
class TTCutListView : public Q3ListView
{
  Q_OBJECT

 public:
  TTCutListView( QWidget* parent, const char* name );
  ~TTCutListView();

  // events
  void contentsMousePressEvent( QMouseEvent* e );

 signals:
  void editCutListItem( TTAVCutPosition& cut_pos );
  void selectCutOut( TTAVCutPosition& cut_pos );
  void showCutIn( TTAVCutPosition& cut_pos );
  void showCutOut( TTAVCutPosition& cut_pos );
  void newCutLen( uint cut_length, off64_t cut_size );
  void previewSingleCut( uint cut_order );

 public:
  void addItem( QString f_name, uint s_index, QTime s_time, uint e_index, QTime e_time );
  void addItem( QString f_name, uint s_index, off64_t s_off, QTime s_time, 
		uint e_index, off64_t e_off, QTime e_time );
  void addItem( QString f_name, TTAVCutPosition* cut_pos );
  void clearList();
  int  count();
  uint getResultFrames();
  bool isItemSelected();

  TTAVCutList* cutList();

  public slots:
  void deleteItem();
  void deselectAll();
  void editItem();
  void itemUp();
  void itemDown();
  void gotoCutIn();
  void gotoCutOut();
  void cutOutIndexChanged( uint c_out_index, QTime c_out_time );
  void singleCutPreview();

 private:
  void newOrder();
  void calcResultCutLen();
  void testListView();

 private:
  Q3PopupMenu*      itemContextMenu;
  Q3PopupMenu*      listContextMenue;
  TTAVCutList*     avcut_list;      // list view managed the cut list
  TTAVCutPosition* cut_position;
  uint             video_cut_length;
  uint             video_cut_size;
};

#endif //TTCUTLISTVIEW_H
