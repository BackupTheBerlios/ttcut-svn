/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / ttcut.tritime.org                       */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttmpeg2mainwnd.h                                                */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 03/01/2007 */
/* MODIFIED: b. altendorf                                    DATE: 04/02/2007 */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTMPEG2MAINWND
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

#include "ui_ttmpeg2_mainwnd.h"

#include "../tools/ttheaderwriter.h"

#include "../../common/ttcut.h"
#include "../../common/ttmessagelogger.h"
#include "../../avstream/ttavtypes.h"
#include "../../avstream/ttmpeg2videostream.h"
#include "../../avstream/ttvideoheaderlist.h"
#include "../../avstream/ttvideoindexlist.h"
#include "../../avstream/ttmpegaudiostream.h"

class TTMpeg2MainWnd : public QMainWindow, Ui::TTMpeg2MainWnd
{
  Q_OBJECT

  public:
    TTMpeg2MainWnd();

  private slots:
    void onAnalyze();
    void onSettings();
    void onExit();
    void onWriteHeaderList();
    void onLoadVideoFile(QString fn);
    void onSliderValueChanged(int pos);
    void onGotoNextFrame(int type, int order);
    void onGotoPrevFrame(int type, int order);

  private:
    void enableControls(bool value);
    void openProject(QString fileName);
    void closeProject();
    bool openVideoFile(QString fn);
    void analyzeMpegStream(TTMpeg2VideoStream* stream);
    void switchStreamOrder(int order);
    void updateDisplay(int indexPos);
    void updateFrameInfo(int indexPos);
    void updateVideoFileInfo(TTMpeg2VideoStream* mpegStream);

   // Member variables
   bool                isProjectOpen;
   int                 currentStreamOrder;
   bool                sliderUpdateFrame;
   QTime               totalTime;
   TTVideoType*        videoType;
   TTAudioType*        audioType;
   TTMpeg2VideoStream* mpegStream;
   TTVideoHeaderList*  videoHeaderList;
   TTVideoIndexList*   videoIndexList;

};

#endif //TTMPEG2MAINWND_H
