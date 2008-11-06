/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttcutpreview.h                                                  */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 03/13/2005 */
/* MODIFIED: b. altendorf                                    DATE: 03/12/2006 */
/* MODIFIED: b. altendorf                                    DATE: 04/19/2007 */
/* MODIFIED: b. altendorf                                    DATE: 05/06/2008 */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTCUTPREVIEW
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

#ifndef TTCUTPREVIEW_H
#define TTCUTPREVIEW_H

#include "ui_previewwidget.h"

#include <QProcess>
#include <QResizeEvent>
#include <QCloseEvent>

#include "../common/ttcut.h"
#include "../common/ttmessagelogger.h"
#include "../data/ttaudiolistdata.h"
#include "../data/ttcutlistdata.h"
#include "../avstream/ttavstream.h"
#include "../avstream/ttvideoindexlist.h"
#include "ttprogressbar.h"
#include "ttmoviewidget.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QComboBox;
class QPushButton;
class QFrame;

// -----------------------------------------------------------------------------
// TTCUTPREVIEW
// -----------------------------------------------------------------------------
class TTCutPreview : public QDialog, Ui::TTPreviewWidget
{
    Q_OBJECT

  public:
       // public constructor / destructor
    TTCutPreview( QWidget* parent = 0, int prevW = 640, int prevH = 480);
    ~TTCutPreview();

    void resizeEvent(QResizeEvent* event);
    void initPreview( TTVideoStream* v_stream, TTAudioStream* a_stream, TTCutListData* c_list );
    void createPreview( int cut_index=-1 );

 protected:
    void closeEvent(QCloseEvent* event);
    void cleanUp();
    void createCutPreviewList( );

protected slots:
    void updateSizes();
    void isPlayingSlot(bool value);
    void selectCut( int iCut );
    void playPreview();
    void exitPreview();

 private:
    TTMessageLogger*   log;
    TTProgressBar*     progress_bar;
    TTVideoStream*     video_stream;
    TTAudioStream*     audio_stream;
    TTVideoIndexList*  video_index;      
    TTVideoHeaderList* video_header;     
    TTCutListData*     avcut_list;
    TTCutListData*     preview_cut_list;
    int                cut_index;
    TTFileBuffer*      video_cut_stream;
    TTFileBuffer*      audio_cut_stream;
    QProcess*          mplexProc;        
    int                previewWidth;        
    int                previewHeight;       
    int                num_cut_list;        
    int                num_preview;         
    QString            current_video_file;    
    QString            current_audio_file;    
    bool               isPlaying;           
    TTMovieWidget*     movieWidget;
};

#endif // TTCUTPREVIEW_H
