/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2007                                                      */
/* FILE     : ttmpeg2main.cpp                                                 */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 03/01/2007 */
/* MODIFIED: b. altendorf                                    DATE:            */
/*----------------------------------------------------------------------------*/

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

// Qt headers
#include <QApplication>
#include <QMessageBox>
#include <QGLFormat>
#include <QPlastiqueStyle>

// class declaration for the main window class
#include "ttmpeg2mainwnd.h"

int main( int argc, char **argv )
{
  QT_REQUIRE_VERSION(argc, argv, "4.1.0");

  QApplication::setColorSpec( QApplication::CustomColor );
  QApplication a( argc, argv );

  // OpenGL is required for the MPEG2 window
  if ( !QGLFormat::hasOpenGL() ) {
    qWarning( "This system has no OpenGL support. Exiting." );
    return -1;
  }

  // Application main widget
  TTMpeg2MainWnd* mainWnd = new TTMpeg2MainWnd();

  QPlastiqueStyle* style = new QPlastiqueStyle();
  a.setStyle(style);

  // Caption text in applications title bar
  mainWnd->setWindowTitle( "TTMpeg2" );
  mainWnd->show();

  // set initial size of applications main window
  mainWnd->resize( 800, 600 );

  //a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
  // Execute application and start event loop
  return a.exec();
}

