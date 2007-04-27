/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2007                                                      */
/* FILE     : ttframeinfo.cpp                                                 */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (b.altendorf@tritime.de)           DATE: 03/01/2007 */
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
 
#include "ttframeinfo.h"

#include <QTime>

#include "../../common/ttcut.h"
#include "../ui/pixmaps/iframe_18.xpm"
#include "../ui/pixmaps/pframe_18.xpm"
#include "../ui/pixmaps/bframe_18.xpm"


TTFrameInfo::TTFrameInfo(QWidget* parent)
  :QWidget(parent)
{
  setupUi( this );

  TTCut::imgIFrame  = new QPixmap(i_frame_18_xpm);
  TTCut::imgPFrame  = new QPixmap(p_frame_18_xpm);
  TTCut::imgBFrame  = new QPixmap(b_frame_18_xpm);
}

void TTFrameInfo::setTitle( __attribute__((unused))const QString & title )
{

}

void TTFrameInfo::clearControl()
{
  leDisplayOrder->setText("");
  leStreamOrder->setText("");
  leStreamTime->setText("");
  leFrameSize->setText("");
  laType->setPixmap(*(TTCut::imgIFrame));
  leOffset->setText("");
  leGopNumber->setText("");
}

void TTFrameInfo::setNumDisplayOrder(int num) 

{
  QString tmpString;

  tmpString.sprintf("%d", num);
  leDisplayOrder->setText(tmpString);
}

void TTFrameInfo::setNumDisplayOrder(int num, int tempRef)
{
  QString tmpString;

  tmpString.sprintf("%d / %d", num, tempRef);
  leDisplayOrder->setText(tmpString);
}

void TTFrameInfo::setNumStreamOrder(int num)
{
  QString tmpString;

  tmpString.sprintf("%d", num);
  leStreamOrder->setText(tmpString);
}

void TTFrameInfo::setNumStreamOrder(int num, int tempRef)
{
  QString tmpString;

  tmpString.sprintf("%d / %d", num, tempRef);
  leDisplayOrder->setText(tmpString);
}

void TTFrameInfo::setNumOrder(int display, int stream)
{
  setNumDisplayOrder(display);
  setNumStreamOrder(stream);
}

void TTFrameInfo::setFrameTime(QTime current, QTime total)
{
  QString tmpString;

  tmpString.sprintf("%s,%d / %s,%d", 
      current.toString().toAscii().data(), current.msec(),
      total.toString().toAscii().data(),   total.msec());
  leStreamTime->setText(tmpString);
}

void TTFrameInfo::setFrameTime(QTime current, int tempRef)
{
  QString tmpString;

  tmpString.sprintf("%s / %d",
      current.toString().toAscii().data(), tempRef);
  leStreamTime->setText(tmpString);
}

void TTFrameInfo::setFrameSize(long size)
{
  QString tmpString;

  tmpString.sprintf("%ld", size);
  leFrameSize->setText(tmpString);
}

void TTFrameInfo::setFrameType(int type)
{
  switch (type)
  {
    case 1:
      laType->setPixmap(*(TTCut::imgIFrame));
      break;

    case 2:
      laType->setPixmap(*(TTCut::imgPFrame));
      break;

    case 3:
    default:
      laType->setPixmap(*(TTCut::imgBFrame));
      break;
  }
}

void TTFrameInfo::setFrameOffset(long offset)
{
  QString tmpString;

  tmpString.sprintf("%ld", offset);
  leOffset->setText(tmpString);
}

void TTFrameInfo::setGOPNumber(int gopNum)
{
  QString tmpString;

  tmpString.sprintf("%d", gopNum);
  leGopNumber->setText(tmpString);
}

void TTFrameInfo::setGOPNumber(int gopNum, int tempRef)
{
  QString tmpString;

  tmpString.sprintf("%d / %d", gopNum, tempRef);
  leGopNumber->setText(tmpString);
}
