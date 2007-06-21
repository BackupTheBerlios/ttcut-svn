/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2007                                                      */
/* FILE     : ttframeinfo.h                                                   */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 03/01/2007 */
/* MODIFIED: b. altendorf                                    DATE:            */
/*----------------------------------------------------------------------------*/
 
// ----------------------------------------------------------------------------
// TTFRAMEINFO
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
  

#ifndef TTFRAMEINFO_H
#define TTFRAMEINFO_H

#include "ui_frameinfowidget.h"

class TTFrameInfo : public QWidget, Ui::FrameInfoWidget
{
  Q_OBJECT

  public:
    TTFrameInfo( QWidget* parent=0 );

    void setTitle ( const QString & title );
    void clearControl();
    void setNumDisplayOrder(int num);
    void setNumDisplayOrder(int num, int tempRef);
    void setNumStreamOrder(int num);
    void setNumStreamOrder(int num, int tempRef);
    void setNumOrder(int display, int stream);
    void setFrameTime(QTime current, QTime total);
    void setFrameTime(QTime current, int tempRef);
    void setFrameSize(long size);
    void setFrameType(int type);
    void setFrameOffset(long offset);
    void setGOPNumber(int gopNum);
    void setGOPNumber(int gopNum, int tempRef);
};
#endif
