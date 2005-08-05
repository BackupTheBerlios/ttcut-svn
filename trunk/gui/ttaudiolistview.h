/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttaudiolistview.h                                               */
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

#ifndef TTAUDIOLISTVIEW_H
#define TTAUDIOLISTVIEW_H

#include <qapplication.h>
#include <qdatetime.h>
#include <q3listview.h>
#include <q3popupmenu.h>
#include <qcursor.h>
#include <qfileinfo.h>
//Added by qt3to4:
#include <QMouseEvent>
#include <QList>

#include "ttcut.h"              /*..global parameter..........................*/
#include "ttprogressbar.h"      /*..the progress dialog.......................*/

#include "../avstream/ttavheader.h"
#include "../avstream/ttavstream.h"

class TTAudioListView;

// -----------------------------------------------------------------------------
// class: TTAudioListViewItem
// -----------------------------------------------------------------------------
class TTAudioListItem : public Q3ListViewItem
{
  friend class TTAudioListView;

 public:
  TTAudioListItem( TTAudioListView* parent );
  TTAudioListItem( TTAudioListView* parent, TTAudioListItem* after );
  ~TTAudioListItem();

 private:
  QFileInfo      audioFileInfo;
  QString        audioFileName;
  QString        audioLength;
  QString        version;
  QString        bitRate;
  QString        sampleRate;
  QString        channelMode;
  QString        delay;
  TTAudioStream* audio_stream;
};


// -----------------------------------------------------------------------------
// class: TTAudioListView
// -----------------------------------------------------------------------------
class TTAudioListView : public Q3ListView
{
  Q_OBJECT
    
 public:
  TTAudioListView( QWidget* parent, const char* name );
  ~TTAudioListView();

  // events
  void contentsMousePressEvent( QMouseEvent* e );

 public:
  void    addItem( QString fName, TTAudioStream* a_strem );
  int     getAudioList( QList<TTAudioStream*>& a_list );
  void    clearList();
  int     count();

  public slots:
  void    deleteItem();
  void    itemUp();
  void    itemDown();

 protected:
  int fillAudioList();

 private:
  TTProgressBar* progressBar;
  Q3PopupMenu*   itemContextMenu;
  Q3PopupMenu*   listContextMenue;
  TTAudioHeader* audio_header;
  TTAudioStream* audio_stream;
  QList<TTAudioStream*> audio_list;
};

#endif  //TTAUDIOLISTVIEW_H


