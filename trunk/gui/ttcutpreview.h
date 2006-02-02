/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttcutpreview.h                                                  */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 03/13/2005 */
/* MODIFIED: b. altendorf                                    DATE: 06/22/2005 */
/* MODIFIED: b. altendorf                                    DATE: 07/16/2005 */
/* MODIFIED: b. altendorf                                    DATE: 08/09/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTCUTPREVIEW
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

#include "ttcut.h"
#include "ttprogressbar.h"
#include "ttcutlistview.h"

#include "../common/ttmessagelogger.h"
#include "../avstream/ttavstream.h"
#include "../avstream/ttvideoindexlist.h"

#include "../avcut/ttavcutposition.h"

class QProcess;
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
class TTCutPreview : public QDialog
{
    Q_OBJECT

  public:
       // public constructor / destructor
    TTCutPreview( QWidget* parent = 0, int prevW = 640, int prevH = 480,
		  const char* name = 0, bool modal = TRUE, Qt::WFlags fl = 0 );
    ~TTCutPreview();

    // public member methods
    void initPreview( TTVideoStream* v_stream, TTAudioStream* a_stream, TTAVCutList* c_list );
    void createPreview( int cut_index=-1 );

 protected:
    // protected member methods
    bool playMPlayer( QString videoFile, QString audioFile );
    bool stopMPlayer();

    void createCutPreviewList( );

protected slots:
    // sets the strings of the subwidgets using the current language
    virtual void languageChange();

    // combobox and button slots
    virtual void selectCut( int iCut );
    virtual void playPreview();
    virtual void stopPreview();
    virtual void exitPreview();

    // mplayer process has information ready for us
    virtual void readFromStdout();
    // mplayer process exited
    virtual void exitMPlayer( int e_code );

protected:
    // gui elements
    TTProgressBar* progress_bar;
    QComboBox*     cbCutPreview;
    QPushButton*   pbPlay;
    QPushButton*   pbStop;
    QPushButton*   pbExit;
    QFrame*        videoFrame;
    QGridLayout*   TTCutPreviewLayout;
    QHBoxLayout*   layout1;
    QSpacerItem*   spacer1;

 private:
    // private member
    TTMessageLogger*   log;
    TTVideoStream*     video_stream;
    TTAudioStream*     audio_stream;
    TTVideoIndexList*  video_index;      
    TTVideoHeaderList* video_header;     
    TTAVCutList*       avcut_list;       
    TTAVCutList*       preview_cut_list; 
    int                cut_index;
    TTFileBuffer*      video_cut_stream;
    TTFileBuffer*      audio_cut_stream;
    QProcess*          mplayerProc;      
    QProcess*          mplexProc;        
    int                previewWidth;        
    int                previewHeight;       
    int                num_cut_list;        
    int                num_preview;         
    QString            current_video_file;    
    QString            current_audio_file;    
    bool               isPlaying;           
};

#endif // TTCUTPREVIEW_H
