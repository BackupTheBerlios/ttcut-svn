/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2007                                                      */
/* FILE     : ttstepby.cpp                                                    */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (b.altendorf@tritime.de)           DATE: 03/01/2007 */
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
 
#include "ttstepby.h"

TTStepBy::TTStepBy(QWidget* parent)
  :QWidget(parent)
{
  setupUi( this );

  currentFrameType = 1;
  currentOrder     = 0;

  connect(rbIFrame,       SIGNAL(clicked()), SLOT(onFrameTypeI()));
  connect(rbPFrame,       SIGNAL(clicked()), SLOT(onFrameTypeP()));
  connect(rbBFrame,       SIGNAL(clicked()), SLOT(onFrameTypeB()));
  connect(rbOrderDisplay, SIGNAL(clicked()), SLOT(onOrderDisplay()));
  connect(rbOrderStream,  SIGNAL(clicked()), SLOT(onOrderStream()));
  connect(btnPrev,        SIGNAL(clicked()), SLOT(onPrevFrame()));
  connect(btnNext,        SIGNAL(clicked()), SLOT(onNextFrame()));
}

void TTStepBy::setTitle( __attribute__((unused))const QString & title )
{

}

/*!
 * Enable / disable the control and his child controls
 */
void TTStepBy::enableControl(bool value)
{
  rbIFrame->setEnabled(value);
  rbPFrame->setEnabled(value);
  rbBFrame->setEnabled(value);
  rbOrderDisplay->setEnabled(value);
  rbOrderStream->setEnabled(value);
  btnPrev->setEnabled(value);
  btnNext->setEnabled(value);
}

/*
 * Returns the current selected "step by frame-type" value.
 */
int TTStepBy::getCurrentFrameType()
{
  return currentFrameType;
}

/*
 * Return the current selected "order by" value.
 */
int TTStepBy::getCurrentOrder()
{
  return currentOrder;
}

/*
 * Signal: Goto next frame
 */
void TTStepBy::onNextFrame()
{
  emit gotoNextFrame(currentFrameType, currentOrder);
}

/*
 * Signal: Goto previous frame
 */
void TTStepBy::onPrevFrame()
{
  emit gotoPrevFrame(currentFrameType, currentOrder);
}

/*
 * Signal: Step by I-frame
 */
void TTStepBy::onFrameTypeI()
{
  currentFrameType = 1;
  emit stepByIFrame();
}

/*
 * Signal: Step by P-frame
 */
void TTStepBy::onFrameTypeP()
{
  currentFrameType = 2;
  emit stepByPFrame();
}

/*
 * Signal: Step by B-frame
 */
void TTStepBy::onFrameTypeB()
{
  currentFrameType = 3;
  emit stepByBFrame();
}

/*
 * Signal: Order display
 */
void TTStepBy::onOrderDisplay()
{
  currentOrder = 0;
  emit orderDisplay();
}

/*
 * Signal: Order stream
 */
void TTStepBy::onOrderStream()
{
  currentOrder = 1;
  emit orderStream();
}
