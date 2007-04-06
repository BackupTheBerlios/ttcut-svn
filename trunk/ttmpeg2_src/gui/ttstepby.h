/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2007                                                      */
/* FILE     : ttstepby.h                                                      */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 03/01/2007 */
/* MODIFIED: b. altendorf                                    DATE:            */
/*----------------------------------------------------------------------------*/
 
// ----------------------------------------------------------------------------
// TTSTEPBY
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
  

#ifndef TTSTEPBY_H
#define TTSTEPBY_H

#include "ui_stepbywidget.h"

class TTStepBy : public QWidget, Ui::StepByWidget
{
  Q_OBJECT

  public:
    TTStepBy( QWidget* parent=0 );

    void setTitle ( const QString & title );
    void enableControl(bool value);
    int  getCurrentFrameType();
    int  getCurrentOrder();

  public slots:
    void onFrameTypeI();
    void onFrameTypeP();
    void onFrameTypeB();
    void onOrderStream();
    void onOrderDisplay();
    void onNextFrame();
    void onPrevFrame();

  signals:
    void gotoNextFrame(int frameType, int order);
    void gotoPrevFrame(int frameType, int order);
    void stepByIFrame();
    void stepByPFrame();
    void stepByBFrame();
    void orderDisplay();
    void orderStream();
  
  private:
    int  currentFrameType;
    int  currentOrder;
};

#endif
