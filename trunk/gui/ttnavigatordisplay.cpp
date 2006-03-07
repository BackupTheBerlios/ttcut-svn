/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / ttcut.tritime.org                       */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttnavigatordisplay.cpp                                          */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 03/03/2006 */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTNAVIGATORDISPLAY
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

#include "ttnavigatordisplay.h"

#include "../common/ttcut.h"
#include "../data/ttcutlistdata.h"

#include <QPainter>

TTNavigatorDisplay::TTNavigatorDisplay(QWidget* parent)
  :QFrame(parent)
{
  setupUi( this );

  isControlEnabled = false;
  cutListData      = NULL;
}

void TTNavigatorDisplay::controlEnabled(bool enabled)
{
  isControlEnabled = enabled;
}

void TTNavigatorDisplay::setCutListData(TTCutListData* data)
{
  cutListData = data;
}

void TTNavigatorDisplay::setMinValue(int val)
{
  minValue    = val;
}

void TTNavigatorDisplay::setMaxValue(int val)
{
  maxValue    = val;
}

void TTNavigatorDisplay::onRefreshDisplay()
{
  
}

void TTNavigatorDisplay::resizeEvent(__attribute__((unused))QResizeEvent* event)
{
  scaleFactor = navigatorDisplay->geometry().width() / (double)(maxValue-minValue);
}

void TTNavigatorDisplay::paintEvent(__attribute__((unused))QPaintEvent *event)
{
  if (ttAssigned(cutListData) && isControlEnabled)
    drawCutList();
}

void TTNavigatorDisplay::drawCutList()
{
  int cutIn;
  int cutOut;
  int cutWidth;
  QRect clientRect = navigatorDisplay->geometry();
  int startX = clientRect.x();
  int startY = clientRect.y();
  int height = clientRect.height();
  int width  = 0;

  scaleFactor = navigatorDisplay->geometry().width() / (double)(maxValue-minValue);

  QPainter painter(this);

  painter.fillRect(clientRect, QBrush(Qt::gray));

  for (int i=0; i < cutListData->count(); i++) {

    cutIn    = cutListData->cutInPosAt(i);
    cutOut   = cutListData->cutOutPosAt(i);
    cutWidth = cutOut - cutIn;

    startX = clientRect.x() + (int)(cutIn*scaleFactor);
    width  = (int)((cutOut-cutIn)*scaleFactor);

    painter.fillRect(startX, startY, width, height, QBrush(Qt::green));
  }
}

