/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttcutchaptertab.cpp                                             */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 03/10/2005 */
/* MODIFIED: b. altendorf                                    DATE: 06/22/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTCUTCHAPTERTAB
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



#include "ttcutccrwnd.h"
//Added by qt3to4:
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>


// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// Tab widget for cut-, chapter- and result-tab
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// Constructor
TTCutCCRWnd::TTCutCCRWnd( QWidget* parent, const char* name, Qt::WFlags fl )
:QTabWidget( parent, name, fl )
{

   // create the widget for the single tabs
   cutTab     = new TTCutCutTab( 0, "CUT_TAB" );
   chapterTab = new TTCutChapterTab( 0, "CHAPTER_TAB" );
   resultTab  = new TTCutResultTab( 0, "RESULT_TAB" );

   // insert the tabs
   insertTab( cutTab,     tr( "Cut list" )     );
   insertTab( chapterTab, tr( "Chapter list" ) );
   insertTab( resultTab,  tr( "Result list" )  );

   setTabIconSet( cutTab,     QIcon( *(TTCut::imgCutAV) )   );
   setTabIconSet( chapterTab, QIcon( *(TTCut::imgChapter) ) );
   setTabIconSet( resultTab,  QIcon( *(TTCut::imgClock) )   );

}


// Destructor
TTCutCCRWnd::~TTCutCCRWnd()
{

}


// public methods
// -----------------------------------------------------------------------------
TTCutCutTab* TTCutCCRWnd::getCutTab()
{
   return cutTab;
}

TTCutListView* TTCutCCRWnd::getCutListView()
{
  return cutTab->cutListView;
}

TTCutChapterTab* TTCutCCRWnd::getChapterTab()
{
   return chapterTab;
}

TTCutResultTab* TTCutCCRWnd::getResultTab()
{
   return resultTab;
}



// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// Cut tab
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// Constructor
TTCutCutTab::TTCutCutTab( QWidget* parent, const char* name, Qt::WFlags fl )
:QWidget( parent, name, fl )
{
   parentWnd = (TTCutCCRWnd*)parent;

  // images
  // --------------------------------------------------------------------------

   // Grid-layout for the entire widget
   tabLayout = new QGridLayout( this );
   tabLayout->setSpacing( 6 );
   tabLayout->setMargin( 11 );

   // HBox-layout for buttons and listview
   layoutH1 = new QHBoxLayout( 0, 0, 6, "layout32");
   // VBox-layout for buttons
   layoutV1 = new QVBoxLayout( 0, 0, 6, "layout20");

   // button cut "up"
   pbCutUp = new QPushButton( this, "pbCutUp" );
   pbCutUp->setMinimumSize( QSize( 30, 20 ) );
   pbCutUp->setMaximumSize( QSize( 30, 20 ) );
   pbCutUp->setPixmap( *(TTCut::imgUpArrow) );
   layoutV1->addWidget( pbCutUp );

   // button cut "delete"
   pbCutDelete = new QPushButton( this, "pbCutDelete" );
   pbCutDelete->setMinimumSize( QSize( 30, 20 ) );
   pbCutDelete->setMaximumSize( QSize( 30, 20 ) );
   pbCutDelete->setPixmap( *(TTCut::imgDelete) );
   layoutV1->addWidget( pbCutDelete );

   // button cut "down"
   pbCutDown = new QPushButton( this, "pbCutDown" );
   pbCutDown->setMinimumSize( QSize( 30, 20 ) );
   pbCutDown->setMaximumSize( QSize( 30, 20 ) );
   pbCutDown->setPixmap( *(TTCut::imgDownArrow) );
   layoutV1->addWidget( pbCutDown );
   layoutH1->addLayout( layoutV1 );

   // cut list-view
   cutListView = new TTCutListView( this, "listView2" );
   cutListView->addColumn( tr( "Videofile" ) );
   cutListView->addColumn( tr( "Start" ) );
   cutListView->addColumn( tr( "End" ) );
   layoutH1->addWidget( cutListView );

   tabLayout->addLayout( layoutH1, 0, 0 );
}


// Destructor
TTCutCutTab::~TTCutCutTab()
{

}


// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// Chapter tab
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// Constructor
TTCutChapterTab::TTCutChapterTab( QWidget* parent, const char* name, Qt::WFlags fl )
:QWidget( parent, name, fl )
{
   parentWnd = (TTCutCCRWnd*)parent;

   video_stream = (TTVideoStream*)NULL;

   // Grid-layout for the entire widget
   tabLayout = new QGridLayout( this );
   tabLayout->setSpacing( 4 );
   tabLayout->setMargin( 8 );

   // listview for temporary chapters
   lvTempChapter = new Q3ListView( this, "lvTempChapter" );
   lvTempChapter->addColumn( tr( "Temporal chapter" ) );

   tabLayout->addMultiCellWidget( lvTempChapter, 0, 1, 1, 1 );

   // listview for destiantion time-stamps
   lvDestTimeStamp = new Q3ListView( this, "lvDestTimeStamp" );
   lvDestTimeStamp->addColumn( tr( "Destination time stamp" ) );
   lvDestTimeStamp->addColumn( tr( "Source time stamp" ) );
   lvDestTimeStamp->addColumn( tr( "Source videostream" ) );

   tabLayout->addMultiCellWidget( lvDestTimeStamp, 0, 1, 3, 3 );

   Layout5 = new QVBoxLayout;
   Layout5->setSpacing( 4 );
   Layout5->setMargin( 0 );

   // auto-create chapter
   laAutoCreate = new QLabel( this, "laAutoCreate" );
   laAutoCreate->setText( tr( "Auto create \nchapters each" ) );
   Layout5->addWidget( laAutoCreate );

   Layout2 = new QHBoxLayout;
   Layout2->setSpacing( 6 );
   Layout2->setMargin( 0 );

   sbMinutes = new QSpinBox( this, "sbMinutes" );
   Layout2->addWidget( sbMinutes );

   laMinutes = new QLabel( this, "laMinutes" );
   laMinutes->setText( tr( "minutes" ) );
   Layout2->addWidget( laMinutes );
   Layout5->addLayout( Layout2 );

   btnApplyAutoCreate = new QPushButton( this, "btnApplyAutoCreate" );
   btnApplyAutoCreate->setText( tr( "apply" ) );
   btnApplyAutoCreate->setIconSet( QIcon( *(TTCut::imgApply) ) );
   Layout5->addWidget( btnApplyAutoCreate );

   tabLayout->addLayout( Layout5, 1, 4 );

   Layout7 = new QVBoxLayout;
   Layout7->setSpacing( 6 );
   Layout7->setMargin( 0 );
   QSpacerItem* spacer_2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum );
   Layout7->addItem( spacer_2 );

   pbDelTimeStamp = new QPushButton( this, "pbDelTimeStamp" );
   pbDelTimeStamp->setMaximumSize( QSize( 24, 24 ) );
   pbDelTimeStamp->setText( tr( "PushButton8" ) );
   pbDelTimeStamp->setIconSet( QIcon( *(TTCut::imgDelete) ) );
   Layout7->addWidget( pbDelTimeStamp );

   tabLayout->addMultiCellLayout( Layout7, 0, 1, 2, 2 );

   Layout6 = new QVBoxLayout;
   Layout6->setSpacing( 6 );
   Layout6->setMargin( 0 );
   QSpacerItem* spacer_3 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
   Layout6->addItem( spacer_3 );

   pbDelTempChapter = new QPushButton( this, "pbDelTempChapter" );
   pbDelTempChapter->setMaximumSize( QSize( 24, 24 ) );
   pbDelTempChapter->setText( tr( "PushButton8" ) );
   pbDelTempChapter->setIconSet( QIcon( *(TTCut::imgDelete) ) );
   Layout6->addWidget( pbDelTempChapter );

   tabLayout->addMultiCellLayout( Layout6, 0, 1, 0, 0 );
   QSpacerItem* spacer_4 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
   tabLayout->addItem( spacer_4, 0, 4 );
}


// Destructor
TTCutChapterTab::~TTCutChapterTab()
{

}

void TTCutChapterTab::setVideoStream( TTVideoStream* v_stream )
{
  video_stream = v_stream;
}


// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// Result tab
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// Constructor
TTCutResultTab::TTCutResultTab( QWidget* parent, const char* name, Qt::WFlags fl )
:QWidget( parent, name, fl )
{
   parentWnd = (TTCutCCRWnd*)parent;

   video_stream = (TTVideoStream*)NULL;

   // main tab layout
   tabLayout = new QGridLayout( this );
   tabLayout->setSpacing( 6 );
   tabLayout->setMargin( 11 );

   // result cut length listview
   lvResultLength = new Q3ListView( this, "lvResultLength" );
   lvResultLength->addColumn( tr( "Stream type" ) );
   lvResultLength->addColumn( tr( "Length" ) );
   lvResultLength->addColumn( tr( "Size" ) );

   tabLayout->addWidget( lvResultLength, 0, 0 );
   QSpacerItem* spacer_5 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
   tabLayout->addItem( spacer_5, 0, 1 );
}


// Destructor
TTCutResultTab::~TTCutResultTab()
{

}

void TTCutResultTab::setVideoStream( TTVideoStream* v_stream )
{
  video_stream = v_stream;
}

void TTCutResultTab::refreshCutVideoLength( uint v_length, off64_t v_size )
{
  QString stream_type;
  QString length_time;
  QString length_size;
  QString sz_temp;
  double  res_size;

  clearList();

  if ( ttAssigned( video_stream ) )
  {
    stream_type = "video";

    length_time = ttFramesToTime( v_length, video_stream->frameRate() ).toString("hh:mm:ss");

    sz_temp.sprintf( "(%d)",v_length );
    length_time += sz_temp;

    res_size    = 0.0;
    length_size = "n.n";

    // size in bytes
    if ( v_size < (off64_t)1024 )
      {
	res_size = (double)v_size;
	length_size.sprintf( "%6.2lf bytes", res_size );
      }
    // size in kb
    else if ( v_size >= (off64_t)1024 && v_size < (off64_t)(1024*1024) )
      {
	res_size = (double)v_size / (double)1024.0;
	length_size.sprintf( "%6.2lf kb", res_size );
      }
    // size in mb
    else
      {
	res_size = (double)v_size / (double)(1024.0*1024.0);
	length_size.sprintf( "%6.2lf mb", res_size );
      }

    Q3ListViewItem* listItem = new Q3ListViewItem( lvResultLength, lvResultLength->lastItem(),
						 stream_type,
						 length_time,
						 length_size );
  }
}

void TTCutResultTab::clearList()
{
  //list-view iterator
  Q3ListViewItemIterator cutIt( lvResultLength );

  // iterate trough the list
  while ( cutIt.current() )
  {
     delete cutIt.current();
  }
}
