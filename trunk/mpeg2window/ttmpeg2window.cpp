/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttmpeg2window.cpp                                               */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 02/23/2005 */
/* MODIFIED: b. altendorf                                    DATE: 03/01/2005 */
/* MODIFIED: b. altendorf                                    DATE: 04/30/2005 */
/* MODIFIED: b. altendorf                                    DATE: 05/31/2005 */
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

// The TTMPEG2Window class is responsible for all OpenGL drawings

// qApplication for the qApp pointer
#include <qapplication.h>
#include <qpixmap.h>

#include <QTimer>

// -----------------------------------------------------------------------------
// class declaration
// -----------------------------------------------------------------------------
#include "ttmpeg2window.h"


const char c_name[] = "TTMPEG2WINDOW : ";

// -----------------------------------------------------------------------------
// Constructor for the TTMPEG2Window
// -----------------------------------------------------------------------------
TTMPEG2Window::TTMPEG2Window( QWidget *parent, const char *name )
  : QGLWidget( parent, name )
{
  // initialize member variables
  mpeg2_decoder    = NULL;
  video_header     = NULL;
  video_index      = NULL;
  picBuffer        = NULL;
  currentFrame     = 0;
  iOldWidth        = 0;
  iOldHeight       = 0;
  isResizeAction   = false;
}


// -----------------------------------------------------------------------------
// Initialize the Qt OpenGL context
// -----------------------------------------------------------------------------
void TTMPEG2Window::initializeGL()
{
   // background color: black
   glClearColor(0.0, 0.0, 0.0, 0.0);

   // shade model
   glShadeModel(GL_FLAT);

   // set the function for depth test an enable depth testing
   glDepthFunc(GL_LEQUAL);
   glEnable( GL_DEPTH_TEST );
}


// -----------------------------------------------------------------------------
// Handle widgets resize events
// -----------------------------------------------------------------------------
void TTMPEG2Window::resizeGL( int width, int height )
{
  GLdouble zoomFactor;
  GLdouble rasterPosX, rasterPosY;

  makeCurrent();

  //qDebug( "%sresizeGL: %d - %d",c_name,width,height );
 
  // set the new GL viewport according to the new window size
  iSceneWidth  = width;
  iSceneHeight = height;

  // select PROJECTION matrix and load identity
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();

  // openGL viewport
  glViewport( 0, 0, width, height );

  // viewing volume
  gluOrtho2D(0.0, (GLdouble)width, 0.0, (GLdouble)height);

  // window height is authoritative
  if ( (long)(iSceneHeight*1000.0*fAspect) <= (long)(iSceneWidth*1000.0) )
  {
    zoomFactor = (GLdouble)iSceneHeight / (GLdouble)iVideoHeight;
  }
  else
    zoomFactor = (GLdouble)iSceneWidth  / (GLdouble)iVideoWidth;
  
  glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
  glClear( GL_COLOR_BUFFER_BIT );
  glClear( GL_DEPTH_BUFFER_BIT );
  
  rasterPosX = (GLdouble)0.50*((GLdouble)iSceneWidth - (GLdouble)iVideoWidth*zoomFactor);
  rasterPosY = (GLdouble)iVideoHeight*(GLdouble)zoomFactor +
    (GLdouble)0.50*((GLdouble)iSceneHeight-(GLdouble)iVideoHeight*zoomFactor);
  
  //qDebug( "%srX: %8.4lf rY: %8.4lf zoom: %8.4lf",c_name,rasterPosX,rasterPosY,zoomFactor );
  
  glRasterPos2d( rasterPosX, rasterPosY );
  
  glPixelZoom( zoomFactor,-zoomFactor );
  
  // for slow maschines delay scene refresh 
  //if ( !isResizeAction ) 
  //{
  //isResizeAction = true;
  //QTimer::singleShot(200, this, SLOT(updateFrame()));
  //}

  // show current frame
  showVideoFrame();
  swapBuffers();
}

// -----------------------------------------------------------------------------
// Handle widgets paint event
// -----------------------------------------------------------------------------
void TTMPEG2Window::paintGL()
{
  //qDebug( "%spaint event",c_name );
}

void TTMPEG2Window::updateFrame()
{
  if ( iSceneWidth != iOldWidth || iSceneHeight != iOldHeight )
  {
    //qDebug( "%supdate frame",c_name );

    iOldWidth        = iSceneWidth;
    iOldHeight       = iSceneHeight;
    isResizeAction   = false;

    showVideoFrame();

    swapBuffers();
  }
}

// -----------------------------------------------------------------------------
// show the current video frame (picBuffer)
// -----------------------------------------------------------------------------
void TTMPEG2Window::showVideoFrame()
{
  makeCurrent();

  GLdouble zoomFactor;
  GLdouble rasterPosX, rasterPosY;

  // set the new GL viewport according to the new window size
  iSceneWidth  = width();
  iSceneHeight = height();
  
  // select PROJECTION matrix and load identity
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  
  // openGL viewport
  glViewport( 0, 0, width(), height() );
  
  // viewing volume
  gluOrtho2D(0.0, (GLdouble)width(), 0.0, (GLdouble)height());
  
  // window height is authoritative
  if ( (long)(iSceneHeight*1000.0*fAspect) <= (long)(iSceneWidth*1000.0) )
  {
    zoomFactor = (GLdouble)iSceneHeight / (GLdouble)iVideoHeight;
  }
  else
    zoomFactor = (GLdouble)iSceneWidth  / (GLdouble)iVideoWidth;
  
  glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
  glClear( GL_COLOR_BUFFER_BIT );
  glClear( GL_DEPTH_BUFFER_BIT );
  
  rasterPosX = (GLdouble)0.50*((GLdouble)iSceneWidth - (GLdouble)iVideoWidth*zoomFactor);
  rasterPosY = (GLdouble)iVideoHeight*(GLdouble)zoomFactor +
    (GLdouble)0.50*((GLdouble)iSceneHeight-(GLdouble)iVideoHeight*zoomFactor);
  
  //qDebug( "%srX: %8.4lf rY: %8.4lf zoom: %8.4lf",c_name,rasterPosX,rasterPosY,zoomFactor );
  
  glRasterPos2d( rasterPosX, rasterPosY );
  
  glPixelZoom( zoomFactor,-zoomFactor );

  if ( ttAssigned(picBuffer) )
  {
    glDrawPixels( iVideoWidth,iVideoHeight, GL_RGB, GL_UNSIGNED_BYTE, picBuffer );
  }
  else
  {
    glClear( GL_COLOR_BUFFER_BIT );
    glClear( GL_DEPTH_BUFFER_BIT );
  }
}

void TTMPEG2Window::showFrameAt( uint index )
{
  // TODO: showFrameAt method
  //qDebug( "%sshow frame at index: %d",c_name,index );

  moveToVideoFrame( index );
}


// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// Navigation in mpeg2 stream
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// decode and show the first video frame
// -----------------------------------------------------------------------------
void TTMPEG2Window::moveToFirstFrame( bool show )
{
  currentFrame = 0;

  frameInfo = mpeg2_decoder->decodeFirstMPEG2Frame( formatRGB24 );

  if ( show && ttAssigned(frameInfo->Y) )
  {
    iVideoWidth  = frameInfo->width;
    iVideoHeight = frameInfo->height;
    fAspect      = (float)iVideoWidth/(float)iVideoHeight;
    
    picBuffer    = frameInfo->Y;
    
    showVideoFrame();
    
    swapBuffers();
  }
}

// -----------------------------------------------------------------------------
// Get the video width
// -----------------------------------------------------------------------------
int TTMPEG2Window::getVideoWidth()
{
  return iVideoWidth;
}

// -----------------------------------------------------------------------------
// Get the video height
// -----------------------------------------------------------------------------
int TTMPEG2Window::getVideoHeight()
{
  return iVideoHeight;
}

// -----------------------------------------------------------------------------
// Get the scene width
// -----------------------------------------------------------------------------
int TTMPEG2Window::getSceneWidth()
{
  return iSceneWidth;
}

// -----------------------------------------------------------------------------
// Get the scene height
// -----------------------------------------------------------------------------
int TTMPEG2Window::getSceneHeight()
{
  return iSceneHeight;
}

// -----------------------------------------------------------------------------
// Open a video file and assign the mpeg2 decoder object
// -----------------------------------------------------------------------------
void TTMPEG2Window::openVideoFile( QString fName, TTVideoIndexList* viIndex, TTVideoHeaderList* viHeader )
{
  if ( !fName.isEmpty() )
  {
    mpeg2FileName = fName;
    video_index   = viIndex;
    video_header  = viHeader;

    if ( ttAssigned(mpeg2_decoder) )
       delete mpeg2_decoder;
 
    // create the decoder object
    mpeg2_decoder  = new TTMpeg2Decoder( fName.ascii(), video_index, video_header );
    frameInfo     = mpeg2_decoder->getFrameInfo();

    iVideoWidth  = frameInfo->width;
    iVideoHeight = frameInfo->height;
    fAspect      = (double)iVideoWidth/(double)iVideoHeight;
  }
}

void TTMPEG2Window::openVideoStream( TTMpeg2VideoStream* v_stream )
{
  mpeg2FileName = v_stream->filePath();
  video_index   = v_stream->indexList();
  video_header  = v_stream->headerList();

  if ( ttAssigned(mpeg2_decoder) )
    delete mpeg2_decoder;
 
  // create the decoder object
  mpeg2_decoder  = new TTMpeg2Decoder( mpeg2FileName.ascii(), video_index, video_header );

  frameInfo     = mpeg2_decoder->getFrameInfo();

  iVideoWidth  = frameInfo->width;
  iVideoHeight = frameInfo->height;
  fAspect      = (double)iVideoWidth/(double)iVideoHeight;
}

void TTMPEG2Window::closeVideoStream()
{
  if ( ttAssigned(mpeg2_decoder) )
  {
    delete mpeg2_decoder;
    mpeg2_decoder = NULL;
  }

  picBuffer        = NULL;
  currentFrame     = 0;
  iOldWidth        = 0;
  iOldHeight       = 0;
  isResizeAction   = false;

  showVideoFrame();
}
// -----------------------------------------------------------------------------
// Move to specified frame position and frame typr
// -----------------------------------------------------------------------------
long TTMPEG2Window::moveToVideoFrame( long iFramePos, int iFrameType )
{
  if ( iFramePos == currentFrame )
    return currentFrame;

  currentFrame = mpeg2_decoder->moveToFrameIndex( iFramePos, iFrameType );

  decodeAndShowSlice();

  return currentFrame;
}


// -----------------------------------------------------------------------------
// Get the current frame index
// -----------------------------------------------------------------------------
long TTMPEG2Window::currentFrameIndex()
{
  return currentFrame;
}

// -----------------------------------------------------------------------------
// Save current frame to file (jpeg, png, bmp)
// -----------------------------------------------------------------------------
void TTMPEG2Window::saveCurrentFrame( QString fName, const char* format )
{
  QPixmap screenShot;

  if ( ttAssigned(picBuffer) )
  {
    FILE * ppmfile;

    ppmfile = fopen ("temp.ppm", "wb");

    if (!ppmfile)
      return;

    fprintf (ppmfile, "P6\n%d %d\n255\n", iVideoWidth, iVideoHeight );
    fwrite (picBuffer, 3 * iVideoWidth, iVideoHeight, ppmfile);
    fclose (ppmfile);

    screenShot.load( "temp.ppm" );
    screenShot.save( fName, format );

    ttDeleteFile( "temp.ppm" );
   }
}

bool TTMPEG2Window::showDecodedSlice()
{
  frameInfo = mpeg2_decoder->getFrameInfo();

  // no slice data decoded
  if ( !ttAssigned(frameInfo->Y) )
  {
    qDebug("NO SLICE POINTER (!)");
    return false;
  }

  iVideoWidth  = frameInfo->width;
  iVideoHeight = frameInfo->height;
  fAspect      = (float)iVideoWidth/(float)iVideoHeight;

  // save pointer to slice
  picBuffer = frameInfo->Y;

  // Display the current slice
  showVideoFrame();

  swapBuffers();

  return true;
}


// -----------------------------------------------------------------------------
// Decode current video frame and show the resulting slice
// -----------------------------------------------------------------------------
bool TTMPEG2Window::decodeAndShowSlice()
{
  bool             result;

  result    = true;
  picBuffer = NULL;

  frame_index = video_index->videoIndexAt( currentFrame );

  // decode the current slice
  frameInfo = mpeg2_decoder->decodeMPEG2Frame( formatRGB24, frame_index->picture_coding_type );

  // no slice data decoded
  if ( !ttAssigned(frameInfo->Y) )
  {
    qDebug("NO SLICE POINTER (!)");
    return false;
  }

  iVideoWidth  = frameInfo->width;
  iVideoHeight = frameInfo->height;
  fAspect      = (float)iVideoWidth/(float)iVideoHeight;

  // save pointer to slice
  picBuffer = frameInfo->Y;

  showVideoFrame();

  swapBuffers();

  return result;
}



