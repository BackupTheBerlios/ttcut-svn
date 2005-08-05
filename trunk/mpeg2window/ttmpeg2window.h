/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttmpeg2window.h                                                 */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 02/23/2005 */
/* MODIFIED: b. altendorf                                    DATE: 03/01/2005 */
/* MODIFIED: b. altendorf                                    DATE: 04/30/2005 */
/* MODIFIED: b. altendorf                                    DATE: 06/22/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTMPEG2WINDOW
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


#ifndef TTMPEG2WINDOW_H
#define TTMPEG2WINDOW_H

#include <QGLWidget>
#include <qdatetime.h>
#include <qimage.h>
#include <QFileDialog>

//#if defined( __TTCUT)
#include "../gui/ttcut.h"
//#else
//#include "ttmpeg2.h"
//#endif

#include "../mpeg2decoder/ttmpeg2decoder.h"

#include "../avstream/ttfilebuffer.h"
#include "../avstream/ttavheader.h"
#include "../avstream/ttvideoheaderlist.h"
#include "../avstream/ttvideoindexlist.h"
#include "../avstream/ttmpeg2videoheader.h"
#include "../avstream/ttmpeg2videostream.h"

class TTMPEG2Window : public QGLWidget
{
  Q_OBJECT
    
    public:
  TTMPEG2Window( QWidget* parent=0, const char* name=0 );

  void openVideoFile( QString fName, TTVideoIndexList* viIndex=NULL, TTVideoHeaderList* viHeader=NULL );
  void openVideoStream( TTMpeg2VideoStream* v_stream );


  // navigation
  void moveToFirstFrame( bool show = true );
  long moveToVideoFrame( long iFramePos, int iFrameType=0 );

  void showVideoFrame();
  void showFrameAt( uint index );
  bool showDecodedSlice();

  long currentFrameIndex();

  int getVideoWidth();
  int getVideoHeight();
  int getSceneWidth();
  int getSceneHeight();

  void saveCurrentFrame( QString fName, const char* format );

 protected:
  void initializeGL();
  void resizeGL( int, int );
  void paintGL();
  bool decodeAndShowSlice();

  protected slots:
  void updateFrame();

 private:
  int                iSceneWidth;
  int                iSceneHeight;
  int                iVideoWidth;
  int                iVideoHeight;
  int                iOldWidth;
  int                iOldHeight;
  bool               isResizeAction;
  float              fAspect;
  long               currentFrame;
  uint8_t*           picBuffer;
  TFrameInfo*        frameInfo;
  QString            mpeg2FileName;
  TTMpeg2Decoder*    mpeg2_decoder;
  TTVideoIndex*      frame_index;
  TTVideoIndexList*  video_index;
  TTVideoHeaderList* video_header;
};

#endif //TTMPEG2WINDOW_H
