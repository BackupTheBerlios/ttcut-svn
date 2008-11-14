/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2006                                                      */
/* FILE     : ttcutoutframe.h                                                 */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 02/19/2006 */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTCUTOUTFRAME
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

#ifndef TTCUTOUTFRAME_H

#include "../ui_h/ui_cutoutframewidget.h"
#include "../avstream/ttmpeg2videostream.h"
#include "ttprogressbar.h"
#include "ttsearchframe.h"

#include <QMap>

class TTCutOutFrame : public QWidget, Ui::TTCutOutFrameWidget
{
  Q_OBJECT

  public:
    TTCutOutFrame( QWidget* parent=0 );

    void setTitle ( const QString & title );
    void controlEnabled( bool enabled );
    void initVideoStream( TTMpeg2VideoStream* vs );
    int currentFramePos();
    void closeVideoStream( TTMpeg2VideoStream* vs );

  public slots:
    void onGotoCutOut( int pos, TTMpeg2VideoStream* pMpeg2Stream=0 );
    void onPrevCutOutPos();
    void onNextCutOutPos();
    void onSearchFrame();

  signals:
    void newCutOutFramePos( int );
    void equalFrameFound( int );

  private:
    void updateCurrentPosition();

  private:
    TTMpeg2VideoStream* currentMpeg2Stream;
    TTMpeg2VideoStream* mpeg2Stream;
    QMap<TTMpeg2VideoStream*, TTMpeg2VideoStream*> videoToSharedVideoMap;
    int currentPosition;
};

#endif //TTCUTOUTFRAME_H
