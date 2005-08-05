/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : main.cpp                                                        */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 02/01/2005 */
/* MODIFIED: b. altendorf                                    DATE: 02/23/2005 */
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
#include <qapplication.h>
#include <qgl.h>

// class declaration for the main window class
#include "ttcutmainwnd.h"

int main( int argc, char **argv )
{
   QApplication::setColorSpec( QApplication::CustomColor );
   QApplication a( argc, argv );
   
   // OpenGL is required for the MPEG2 window
   if ( !QGLFormat::hasOpenGL() ) {
      qWarning( "This system has no OpenGL support. Exiting." );
      return -1;
   }
   
   // Application main widget
   TTCutMainWnd* mainWnd = new TTCutMainWnd();
   
   a.setMainWidget( mainWnd );

   // Caption text in applications title bar
   mainWnd->setCaption( TTCut::versionString );
   mainWnd->show();
   
   // set initial size of applications main window
   mainWnd->resize( 800, 600 );
   
   a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
   // Execute application and start event loop
   return a.exec();	    
}
