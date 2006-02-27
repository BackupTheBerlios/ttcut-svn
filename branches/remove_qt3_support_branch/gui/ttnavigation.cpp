/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / ttcut.tritime.org                       */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttnavigation.cpp                                                */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 02/26/2006 */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTNAVIGATION
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

#include "ttnavigation.h"

TTNavigation::TTNavigation(QWidget* parent)
  :QWidget(parent)
{
  setupUi( this );

  isEditCut        = false;
  isCutInPosition  = false;
  isCutOutPosition = false;

  cutInPosition  = -1;
  cutOutPosition = -1;
  markerPosition = -1;
  
  connect(pbNextIFrame,   SIGNAL(clicked()), SLOT(onNextIFrame()));
  connect(pbPrevIFrame,   SIGNAL(clicked()), SLOT(onPrevIFrame()));
  connect(pbNextPFrame,   SIGNAL(clicked()), SLOT(onNextPFrame()));
  connect(pbPrevPFrame,   SIGNAL(clicked()), SLOT(onPrevPFrame()));
  connect(pbNextBFrame,   SIGNAL(clicked()), SLOT(onNextBFrame()));
  connect(pbPrevBFrame,   SIGNAL(clicked()), SLOT(onPrevBFrame()));
  connect(pbSetCutIn,     SIGNAL(clicked()), SLOT(onSetCutIn()));
  connect(pbSetCutOut,    SIGNAL(clicked()), SLOT(onSetCutOut()));
  connect(pbGotoCutIn,    SIGNAL(clicked()), SLOT(onGotoCutIn()));
  connect(pbGotoCutOut,   SIGNAL(clicked()), SLOT(onGotoCutOut()));
  connect(pbAddCut,       SIGNAL(clicked()), SLOT(onAddCutRange()));
  connect(pbQuickJump,    SIGNAL(clicked()), SLOT(onQuickJump()));
  connect(pbStreamPoints, SIGNAL(clicked()), SLOT(onStreamPoints()));
  connect(pbSetMarker,    SIGNAL(clicked()), SLOT(onSetMarker()));
  connect(pbGotoMarker,   SIGNAL(clicked()), SLOT(onGotoMarker()));
}

void TTNavigation::setTitle( const QString & title )
{
  gbNavigation->setTitle( title );
}

void TTNavigation::controlEnabled( bool enabled )
{
  pbNextIFrame->setEnabled( enabled );
  pbPrevIFrame->setEnabled( enabled );
  pbNextPFrame->setEnabled( enabled );
  pbPrevPFrame->setEnabled( enabled );
  pbNextBFrame->setEnabled( enabled );
  pbPrevBFrame->setEnabled( enabled );
  pbSetCutIn->setEnabled( enabled );
  pbSetCutOut->setEnabled( enabled );
  pbGotoCutIn->setEnabled( enabled );
  pbGotoCutOut->setEnabled( enabled );
  pbAddCut->setEnabled( enabled );
  pbSetMarker->setEnabled( enabled );
  pbGotoMarker->setEnabled( false );
}

void TTNavigation::checkCutPosition( TTVideoStream* vs )
{
  currentPosition  = vs->currentIndex();
  currentTime      = vs->currentFrameTime().toString("hh:mm:ss.zzz");
  currentFrameType = vs->currentFrameType();
  
  // cut in possible
  if ( vs->isCutInPoint( -1 ) )
    pbSetCutIn->setEnabled( true );
  else
    pbSetCutIn->setEnabled( false );

  // cut out possible
  if ( vs->isCutOutPoint( -1 ) )
    pbSetCutOut->setEnabled( true );
  else
    pbSetCutOut->setEnabled( false );
}

void TTNavigation::onPrevIFrame()
{
  emit prevIFrame();
}


void TTNavigation::onNextIFrame()
{
  emit nextIFrame();
}


void TTNavigation::onPrevPFrame()
{
  emit prevPFrame();
}


void TTNavigation::onNextPFrame()
{
  emit nextPFrame();
}


void TTNavigation::onPrevBFrame()
{
  emit prevBFrame();
}


void TTNavigation::onNextBFrame()
{
  emit nextBFrame();
}


void TTNavigation::onSetCutIn()
{
  QString szTemp1, szTemp2;

  isCutInPosition = true;
  cutInPosition = currentPosition;

  szTemp1 = currentTime;
  szTemp2.sprintf(" (%d)",cutInPosition);

  if ( currentFrameType == 1 ) szTemp2 += " [I]";
  if ( currentFrameType == 2 ) szTemp2 += " [P]";
  if ( currentFrameType == 3 ) szTemp2 += " [B]";

  szTemp1 += szTemp2;
  laCutInPosition->setText( szTemp1 );

  emit setCutIn( cutInPosition );
}


void TTNavigation::onSetCutOut()
{
  QString szTemp1, szTemp2;
  
  isCutOutPosition = true;
  cutOutPosition = currentPosition;

  szTemp1 = currentTime;
  szTemp2.sprintf(" (%d)",cutOutPosition);

  if ( currentFrameType == 1 ) szTemp2 += " [I]";
  if ( currentFrameType == 2 ) szTemp2 += " [P]";
  if ( currentFrameType == 3 ) szTemp2 += " [B]";

  szTemp1 += szTemp2;
  laCutOutPosition->setText( szTemp1 );

  emit setCutOut( cutOutPosition );
}


void TTNavigation::onGotoCutIn()
{
  if ( isCutInPosition )
  emit gotoCutIn( cutInPosition );
}


void TTNavigation::onGotoCutOut()
{
  if ( isCutOutPosition )
  emit gotoCutOut(cutOutPosition);
}


void TTNavigation::onAddCutRange()
{
  if ( isCutInPosition && isCutOutPosition ) {
    isCutInPosition  = false;
    isCutOutPosition = false;
    laCutInPosition->setText("...");
    laCutOutPosition->setText("...");

    if (isEditCut) {
      pbAddCut->setText(tr("Add range to cut list"));
      isEditCut = false;
    }

    emit addCutRange(cutInPosition, cutOutPosition);
  }
}


void TTNavigation::onSetMarker()
{
  QString szTemp1, szTemp2;

  markerPosition = currentPosition;
  pbGotoMarker->setEnabled( true );

  szTemp1 = currentTime;
  szTemp2.sprintf(" (%d)",markerPosition);

  if ( currentFrameType == 1 ) szTemp2 += " [I]";
  if ( currentFrameType == 2 ) szTemp2 += " [P]";
  if ( currentFrameType == 3 ) szTemp2 += " [B]";

  szTemp1 += szTemp2;
  laMarkerPosition->setText( szTemp1 );
}


void TTNavigation::onGotoMarker()
{
  if ( markerPosition >= 0 )
    emit gotoMarker(markerPosition);
}


void TTNavigation::onStreamPoints()
{
}


void TTNavigation::onQuickJump()
{
}

void TTNavigation::onEditCut(const TTCutListDataItem& cutData)
{
  QString szTemp1, szTemp2;

  isCutInPosition  = true;
  isCutOutPosition = true;
  cutInPosition    = cutData.getCutInIndex();
  cutOutPosition   = cutData.getCutOutIndex();

  szTemp1 = cutData.getCutInTime().toString("hh:mm:ss.zzz");
  szTemp2.sprintf(" (%d)",cutInPosition);

  if ( cutData.getCutInFrameType() == 1 ) szTemp2 += " [I]";
  if ( cutData.getCutInFrameType() == 2 ) szTemp2 += " [P]";
  if ( cutData.getCutInFrameType() == 3 ) szTemp2 += " [B]";

  szTemp1 += szTemp2;
  laCutInPosition->setText( szTemp1 );

  szTemp1 = cutData.getCutOutTime().toString("hh:mm:ss.zzz");
  szTemp2.sprintf(" (%d)",cutOutPosition);

  if ( cutData.getCutOutFrameType() == 1 ) szTemp2 += " [I]";
  if ( cutData.getCutOutFrameType() == 2 ) szTemp2 += " [P]";
  if ( cutData.getCutOutFrameType() == 3 ) szTemp2 += " [B]";

  szTemp1 += szTemp2;
  laCutOutPosition->setText( szTemp1 );

  isEditCut = true;
  pbAddCut->setText(tr("Update range in cut list"));

  emit gotoCutIn(cutInPosition);
}

