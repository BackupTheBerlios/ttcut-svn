/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTMPEG2 2005                                                    */
/* FILE     : ttmpeg2mainwnd.h                                                */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 05/14/2005 */
/* MODIFIED: b. altendorf                                    DATE: 05/17/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTMPEG2MAINWND
// *** TTCURRENTFRAMEINFO
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

#ifndef TTMPEG2MAINWND_H
#define TTMPEG2MAINWND_H

#include "../common/ttcut.h"

#ifdef __WIN32
#include "../avstream/ttwfilebuffer.h"
#include "../mpeg2window/ttmpeg2window.h"
#else
#include "../avstream/ttfilebuffer.h"
#include "../mpeg2window/ttmpeg2window.h"
#endif

#include "../avstream/ttavtypes.h"
#include "../avstream/ttmpeg2videostream.h"
#include "../avstream/ttvideoheaderlist.h"
#include "../avstream/ttvideoindexlist.h"
#include "../avstream/ttmpegaudiostream.h"

#include "../gui/ttprogressbar.h"
#include "ttmpeg2infoview.h"
#include "ttframechart.h"

#include <qstring.h>
#include <qvariant.h>
#include <qpixmap.h>
#include <q3mainwindow.h>
//Added by qt3to4:
#include <Q3ActionGroup>
#include <QGridLayout>
#include <Q3Frame>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <Q3PopupMenu>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QAction;
class Q3ActionGroup;
class Q3ToolBar;
class QToolButton;
class Q3PopupMenu;
class Q3GroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class Q3ButtonGroup;
class QRadioButton;
class Q3Frame;
class Q3ListView;
class Q3ListViewItem;
class QScrollBar;

class TTCurrentFrameInfo;

// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// TTMpeg2MainWnd
// -----------------------------------------------------------------------------
class TTMpeg2MainWnd : public Q3MainWindow
{
    Q_OBJECT

public:
    TTMpeg2MainWnd( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = Qt::WType_TopLevel );
    ~TTMpeg2MainWnd();

    // buffer statistics
    void setFileLength( uint64_t length );
    void setReadOps( long ops );
    void setReadOps( long ops, float percent );
    void setFillOps( long ops );
    void setReadTime( QTime time );

    // stream statistics
    void setNumFramesTotal( uint frames );
    void setNumIFrames( uint frames );
    void setNumPFrames( uint frames );
    void setNumBFrames( uint frames );
    void setNumSequence( uint num );
    void setNumGOP( uint num );
    void setNumPicture( uint num );
    void setNumSequenceEnd( uint num );

    // sequence info
    void setBitrate( long rate );
    void setFramerate( float rate );
    void setPictureWidth( int width );
    void setPictureHeight( int height );

 protected:
    void  setInitialValues();
    void  resetProject();
    void  analyzeMpegStream( );
    QTime diffTime( time_t t_start, time_t t_end );
    void  showFrame( uint f_index );

public slots:
    virtual void fileOpen();
    virtual void fileSettings();
    virtual void fileAnalyze();
    virtual void fileExit();

    virtual void startAnalyze();
    virtual void newPosition( int );

    protected slots:
    virtual void languageChange();    void nextFrameAction();
    void prevFrameAction();
    void videoSliderChanged( int iSliderPos );
    void orderAnyFrame();
    void orderIFrame();
    void orderPFrame();
    void orderBFrame();
    void orderDisplay();
    void orderStream();

protected:
    QGridLayout* TTMpeg2MainWndLayout;
    QGridLayout* gbStreamNameLayout;
    QHBoxLayout* layout10;
    QVBoxLayout* layout9;
    QGridLayout* gbBufferStatisticsLayout;
    QVBoxLayout* layout1;
    QVBoxLayout* layout2;
    QGridLayout* gbStreamStatisticsLayout;
    QVBoxLayout* layout5;
    QVBoxLayout* layout4;
    QVBoxLayout* layout6;
    QVBoxLayout* layout7;
    QGridLayout* gbStreamInfoLayout;
    QGridLayout* gbSequenceInfoLayout;
    QVBoxLayout* layout19;
    QVBoxLayout* layout20;
    Q3GroupBox*   gbStreamName;
    QLineEdit*   leStreamName;
    QPushButton* pbOpenStream;
    Q3GroupBox*   gbBufferStatistics;
    QLabel*      slFileLength;
    QLabel*      slReadOps;
    QLabel*      slFillOps;
    QLabel*      slReadTime;
    QLabel*      laFileLength;
    QLabel*      laReadOps;
    QLabel*      laFillOps;
    QLabel*      laReadTime;
    Q3GroupBox*   gbStreamStatistics;
    Q3Frame*      line1;
    QLabel*      slTotalFrames;
    QLabel*      slIFrames;
    QLabel*      slPFrames;
    QLabel*      slBFrames;
    QLabel*      slSeqHeader;
    QLabel*      slGOPHeader;
    QLabel*      slPicHeader;
    QLabel*      slSeqEndHeader;
    QLabel*      laSequence;
    QLabel*      laGOP;
    QLabel*      laPicture;
    QLabel*      laSequenceEnd;
    QLabel*      laFramesTotal;
    QLabel*      laIFrames;
    QLabel*      laPFrames;
    QLabel*      laBFrames;
    Q3GroupBox*   gbStreamInfo;
    Q3ListView*   lvStreamInfo;
    Q3GroupBox*   gbSequenceInfo;
    QLabel*      slBitrate;
    QLabel*      slFramerate;
    QLabel*      slWidth;
    QLabel*      slHeight;
    QLineEdit*   laSeqBitrate;
    QLineEdit*   laSeqFramerate;
    QLineEdit*   laSeqPicWidth;
    QLineEdit*   laSeqPicHeight;
    QScrollBar*  sbFrames;
    Q3ToolBar*    toolBar;
    QToolButton* tbStartAnalyzing;
    QToolButton* tbSettings;
    QMenuBar*    MenuBar;
    Q3PopupMenu*  fileMenu;

    QAction*     fileOpenAction;
    QAction*     fileSettingsAction;
    QAction*     fileAnalyzeAction;
    QAction*     fileExitAction;

    TTCurrentFrameInfo* frameInfo;

    TTProgressBar* progress_bar;

 private:
    QString             str_tmp;
    QString             file_name;
    bool                is_stream_open;
    int                 step_order;
    int                 step_frame;
    QTime               total_time;
    bool                slider_update;

    TTVideoType*        video_type;
    TTAudioType*        audio_type;
    TTMpeg2VideoStream* video_stream;
    TTVideoHeaderList*  header_list;
    TTVideoIndexList*   index_list;
    TTFileBuffer*       mpeg2_stream;
    
    TTMPEG2Window*      mpeg2_window;
    TTFrameChart*       chart_window;
    TTMpeg2InfoView*    info_window;
};

// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// TTCurrentFrameInfo
// -----------------------------------------------------------------------------
class TTCurrentFrameInfo : public QWidget
{ 
  Q_OBJECT

    public:
    TTCurrentFrameInfo( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
    ~TTCurrentFrameInfo();

    void setNumDisplayOrder( uint num );
    void setNumDisplayOrder( uint num, int temp_ref );
    uint numDisplayOrder();
    void setNumStreamOrder( uint num );
    void setNumStreamOrder( uint num, int temp_ref );
    uint numStreamOrder();
    void setNumOrder( uint num_display, uint um_stream );
    void setFrameTime( QTime time_1, QTime time_2 );
    void setFrameTime( QTime time_1, int temp_ref );
    void setFrameSize( long size );
    void setFrameType( int type );
    //void setFrameOffset( off64_t offset );
    void setFrameOffset( long offset );
    void setGOPNumber( uint gop_num );
    void setGOPNumber( uint gop_num, int temp_ref );


    Q3GroupBox*    gbCurrentFrameInfo;
    QLabel*       laNumDisplayOrder;
    QLabel*       laNumStreamOrder;
    QLineEdit*    leNumDisplayOrder;
    QLineEdit*    leNumStreamOrder;
    QLabel*       laFrameTime;
    QLineEdit*    leFrameTime;
    QLabel*       laFrameType;
    QLabel*       viewFrameType;
    QLabel*       laFrameSize;
    QLineEdit*    leFrameSize;
    QLabel*       laFrameOffset;
    QLineEdit*    leFrameOffset;
    Q3GroupBox*    gbGOPNumber;
    QLineEdit*    leGOPNumber;
    Q3GroupBox*    gbNavigation;
    QPushButton*  pbPrevFrame;
    QPushButton*  pbNextFrame;
    Q3ButtonGroup* bgFrameOrder;
    QRadioButton* rbPFrame;
    QRadioButton* rbBFrame;
    QRadioButton* rbDisplayOrder;
    QRadioButton* rbStreamOrder;
    QRadioButton* rbIFrame;

protected:
    QGridLayout* TTCurrentFrameInfoLayout;
    QGridLayout* gbCurrentFrameInfoLayout;
    QGridLayout* gbGOPNumberLayout;
    QGridLayout* gbNavigationLayout;
    QHBoxLayout* Layout1;
    QGridLayout* bgFrameOrderLayout;


    signals:
    void nextFrame();
    void prevFrame();
    void orderAnyFrame();
    void orderIFrame();
    void orderPFrame();
    void orderBFrame();
    void orderDisplay();
    void orderStream();

    protected slots:
    void prevFrameAction();
    void nextFrameAction();
    void stepOrderAction( int button_id );

 protected:
    uint num_display_order;
    uint num_stream_order;
};

#endif // TTMPEG2MAINWND_H
