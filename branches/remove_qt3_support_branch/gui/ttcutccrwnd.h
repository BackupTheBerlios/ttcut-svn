/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttcutchaptertab.h                                               */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 03/10/2005 */
/* MODIFIED: b. altendorf                                    DATE: 06/22/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTCUTCHAPTERTAB
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

#ifndef TTCUTCHAPTERTAB_H
#define TTCUTCHAPTERTAB_H

// Qt header and class declarations
// -----------------------------------------------------------------------------
#include <qvariant.h>
#include <qwidget.h>
#include <q3header.h>
#include <qlabel.h>
#include <q3listview.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qtabwidget.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <q3whatsthis.h>
#include <qpixmap.h>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QLabel;
class Q3ListView;
class Q3ListViewItem;
class QPushButton;
class QSpinBox;
class QTabWidget;

// TTCut header files
// -----------------------------------------------------------------------------
#include "ttcutlistview.h"

#include "../avstream/ttavstream.h"

class TTCutCutTab;
class TTCutChapterTab;
class TTCutResultTab;

// -----------------------------------------------------------------------------
// Tab-container for the tab widgets: cut-list, chapter-list and result-list
// -----------------------------------------------------------------------------
class TTCutCCRWnd : public QTabWidget
{
  Q_OBJECT
    
 public:
  TTCutCCRWnd( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
  ~TTCutCCRWnd();
  
  TTCutCutTab*     getCutTab();
  TTCutListView*   getCutListView();
  TTCutChapterTab* getChapterTab();
  TTCutResultTab*  getResultTab();
  
 signals:
  // signals from the cut-list tab
  
  // signals from the chapter-list tab
  
  // signals from the result-list tab
  
  
 private:
   TTCutCutTab*     cutTab;
   TTCutChapterTab* chapterTab;
   TTCutResultTab*  resultTab;
};


// -----------------------------------------------------------------------------
// Cut-list tab
// -----------------------------------------------------------------------------
class TTCutCutTab : public QWidget
{
  Q_OBJECT
    
    friend class TTCutCCRWnd;
  friend class TTCutMainWnd;
  
 public:
  TTCutCutTab( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
  ~TTCutCutTab();
  
 private:
  TTCutCCRWnd*   parentWnd;
  QPushButton*   pbCutUp;
  QPushButton*   pbCutDelete;
  QPushButton*   pbCutDown;
  TTCutListView* cutListView;
  
  QPixmap        imgDownArrow;
  QPixmap        imgUpArrow;
  QPixmap        imgDelete;
  QGridLayout*   tabLayout;
  QVBoxLayout*   layoutV1;
  QHBoxLayout*   layoutH1;
};


// -----------------------------------------------------------------------------
// Chapter-list tab
// -----------------------------------------------------------------------------
class TTCutChapterTab : public QWidget
{
   Q_OBJECT

     friend class TTCutCCRWnd;

 public:
   TTCutChapterTab( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
   ~TTCutChapterTab();

   void setVideoStream( TTVideoStream* v_stream );

private:
   TTCutCCRWnd* parentWnd;
   Q3ListView*   lvTempChapter;
   Q3ListView*   lvDestTimeStamp;
   QLabel*      laAutoCreate;
   QSpinBox*    sbMinutes;
   QLabel*      laMinutes;
   QPushButton* btnApplyAutoCreate;
   QPushButton* pbDelTimeStamp;
   QPushButton* pbDelTempChapter;
   
   QGridLayout* tabLayout;
   QVBoxLayout* Layout5;
   QHBoxLayout* Layout2;
   QVBoxLayout* Layout7;
   QVBoxLayout* Layout6;

   TTVideoStream* video_stream;
};


// -----------------------------------------------------------------------------
// Result-list tab
//-----------------------------------------------------------------------------
class TTCutResultTab : public QWidget
{
   Q_OBJECT

     friend class TTCutCCRWnd;

 public:
   TTCutResultTab( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
   ~TTCutResultTab();

   void setVideoStream( TTVideoStream* v_stream );
   void clearList();

   public slots:
   void refreshCutVideoLength( uint v_length, off64_t v_size );

 private:
   TTCutCCRWnd* parentWnd;
   QGridLayout* tabLayout;
   Q3ListView*   lvResultLength;

   TTVideoStream* video_stream;
};

#endif // TTCUTCHAPTERCUT_H
