/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / ttcut.tritime.org                       */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttstreamnavigator.cpp                                           */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 03/03/2006 */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTSTREAMNAVIGATOR
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

#include "ttstreamnavigator.h"

TTStreamNavigator::TTStreamNavigator(QWidget* parent)
  :QWidget(parent)
{
  setupUi( this );

  connect(videoSlider, SIGNAL(valueChanged(int)), SLOT(onNewSliderValue(int)));
  connect(videoSlider, SIGNAL(sliderMoved(int)),  SLOT(onSliderMoved(int)));
}

void TTStreamNavigator::setTitle(const QString& title)
{
}

void TTStreamNavigator::controlEnabled(bool enabled)
{
  videoSlider->setEnabled(enabled);
  navigatorDisplay->controlEnabled(enabled);
  navigatorDisplay->repaint();
}

void TTStreamNavigator::setCutListData(TTCutListData* data)
{
  navigatorDisplay->setCutListData(data);
}

void TTStreamNavigator::setMinValue(int val)
{
  videoSlider->setMinimum(val);
  navigatorDisplay->setMinValue(val);
}

void TTStreamNavigator::setMaxValue(int val)
{
  videoSlider->setMaximum(val);
  navigatorDisplay->setMaxValue(val);
}


QSlider* TTStreamNavigator::slider()
{
  return videoSlider;
}

void TTStreamNavigator::onNewSliderValue(int val)
{
  emit sliderValueChanged(val);
}

void TTStreamNavigator::onSliderMoved(int val)
{
  qApp->processEvents();
}

void TTStreamNavigator::onRefreshDisplay()
{
  navigatorDisplay->repaint();
}
