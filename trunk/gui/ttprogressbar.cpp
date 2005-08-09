/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttprogressbar.cpp                                               */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 03/11/2005 */
/* MODIFIED: b. altendorf                                    DATE: 06/05/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTPROGRESSBAR
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

#include "ttprogressbar.h"

#include <qapplication.h>
#include <qlabel.h>
#include <q3progressbar.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <q3whatsthis.h>
#include <qdatetime.h>
//Added by qt3to4:
#include <QHBoxLayout>
#include <QGridLayout>

const char c_name[] = "TTPROGRESSBAR : ";

TTProgressBar::TTProgressBar( QWidget* parent,  const char* name, bool modal, Qt::WFlags fl )
    : QDialog( parent, name, modal, fl )
{
  // initialize variables
  userCancel = false;

    if ( !name )
	setName( "TTProgressBar" );
    resize( 640, 100 );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, sizePolicy().hasHeightForWidth() ) );
    setMinimumSize( QSize( 640, 100 ) );
    setMaximumSize( QSize( 640, 100 ) );
    setBaseSize( QSize( 640, 100 ) );
    setCaption( tr( "Progress Info" ) );
    TTProgressBarLayout = new QGridLayout( this );
    TTProgressBarLayout->setSpacing( 6 );
    TTProgressBarLayout->setMargin( 11 );

    Layout1 = new QHBoxLayout;
    Layout1->setSpacing( 6 );
    Layout1->setMargin( 0 );

    laAction = new QLabel( this, "laAction" );
    laAction->setText( tr( "Action:" ) );
    Layout1->addWidget( laAction );

    actionString = new QLabel( this, "actionString" );
    actionString->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)1, actionString->sizePolicy().hasHeightForWidth() ) );
    actionString->setMinimumSize( QSize( 250, 0 ) );
    actionString->setText( tr( "TextLabel2" ) );
    Layout1->addWidget( actionString );

    laElapsedTime = new QLabel( this, "laElapsedTime" );
    laElapsedTime->setText( tr( "Elapsed time:" ) );
    Layout1->addWidget( laElapsedTime );

    elapsedTimeString = new QLabel( this, "elapsedTimeString" );
    elapsedTimeString->setText( tr( "00:00:00" ) );
    Layout1->addWidget( elapsedTimeString );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( spacer );

    laPercentage = new QLabel( this, "laPercentage" );
    laPercentage->setText( tr( "Percentage complete:" ) );
    Layout1->addWidget( laPercentage );

    percentageString = new QLabel( this, "percentageString" );
    percentageString->setText( tr( "0%" ) );
    Layout1->addWidget( percentageString );

    TTProgressBarLayout->addLayout( Layout1, 0, 0 );

    Layout2 = new QHBoxLayout;
    Layout2->setSpacing( 6 );
    Layout2->setMargin( 0 );

    progressBar = new Q3ProgressBar( this, "progressBar" );
    progressBar->setFrameShape( Q3ProgressBar::Panel );
    progressBar->setFrameShadow( Q3ProgressBar::Sunken );
    progressBar->setCenterIndicator( true );
    progressBar->setPercentageVisible( false );
    Layout2->addWidget( progressBar );

    pbCancel = new QPushButton( this, "pbCancel" );
    pbCancel->setText( tr( "Cancel" ) );
    Layout2->addWidget( pbCancel );

    TTProgressBarLayout->addLayout( Layout2, 1, 0 );

    // signals and slot connection
    connect( pbCancel, SIGNAL( clicked() ), SLOT( slotCancel() ) );

    // set start time
    elapsedMsec = 0;
    elapsedTime.start();
}

/*
 *  Destroys the object and frees any allocated resources
 */
TTProgressBar::~TTProgressBar()
{
  elapsedTime.elapsed();
}


void TTProgressBar::setActionText( QString action )
{
  actionString->setText( action );
  qApp->processEvents();
}


void TTProgressBar::setElapsedTime( QTime time )
{

}


void TTProgressBar::setPercentages( float percent )
{

}

void TTProgressBar::setTotalSteps( uint64_t t_steps, int r_int )
{
  totalSteps        = t_steps;
  normTotalSteps    = 100000;

  // manual
  if ( r_int > 0 )
    refresh_intervall = r_int;

  // automatic
  else
  {
    if ( (int64_t)t_steps*0.0000005 > 0 )
      refresh_intervall = (int64_t)t_steps*0.0000005;
    else if ( (int64_t)(t_steps*0.000001) > 0 )
      refresh_intervall = (int64_t)t_steps*0.000001;
    else if ( (int64_t)(t_steps*0.00001) > 0 )
      refresh_intervall = (int64_t)t_steps*0.00001;
    else if ( (int64_t)(t_steps*0.0001) > 0 )
      refresh_intervall = (int64_t)t_steps*0.0001;
    else if ( (int64_t)(t_steps*0.001) > 0 )
      refresh_intervall = (int64_t)t_steps*0.001;
    else if ( (int64_t)(t_steps*0.01) > 0 )
      refresh_intervall = (int64_t)t_steps*0.01;
    else if ( (int64_t)(t_steps*0.1) > 0 )
      refresh_intervall = (int64_t)t_steps*0.1;
    else
      refresh_intervall = 100;
  }

  refresh = refresh_intervall;

  //qDebug( "%stotal steps/refresh intervall: %lld/%d",c_name,t_steps,refresh_intervall );

  progressBar->setTotalSteps( normTotalSteps );
  progressBar->setProgress( -1 );
  qApp->processEvents();
}


bool TTProgressBar::setProgress( uint64_t progress )
{
  refresh--;
  if ( refresh == 0 )
  {
    // calculate progress in percent
    progressPercent = (double)progress / (double)totalSteps * 100.0;

    // TODO: Warning: Stoerendes letztes % in Format
    strPercentage.sprintf( "%02.0lf%",progressPercent );
    percentageString->setText( strPercentage );
    //percentageString->update();

    progressPercent = (double)progress / (double)totalSteps * 100.0;

    // normalized progress value for the progress bar
    normProgress = (int)(progressPercent*1000.0);

    //printf("norm_progress: %d\n",normProgress);

    progressBar->setProgress( normProgress );

    elapsedMsec += elapsedTime.restart();
    elapsedTimeString->setText( ttMsecToTime( elapsedMsec ).toString( "hh:mm:ss" ));
    //elapsedTimeString->update();
    // process events
    qApp->processEvents();
    refresh = refresh_intervall;
  }

  return userCancel;
}

void TTProgressBar::setComplete()
{
  refresh = 1;
  setProgress( totalSteps );
}

void TTProgressBar::resetProgress()
{
  progressBar->reset();
}


void TTProgressBar::slotCancel()
{
  qDebug( "%scancel button pressed: %d",c_name,userCancel );
  userCancel = true;
  qApp->processEvents();
}


void TTProgressBar::hideBar()
{
  if ( isVisible() )
  {
    hide();
    setModal( true );
  }
  else
  {
    show();
    setModal( false );
  }
  qApp->processEvents();
}

void TTProgressBar::showBar()
{
  if ( !isVisible() )
  {
    setModal( false );
    show();
  }
  else
  {
    hide();
    setModal( true );
  }
  qApp->processEvents();
}
