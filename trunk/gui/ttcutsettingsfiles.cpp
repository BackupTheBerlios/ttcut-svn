/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / ttcut.tritime.org                       */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttcutsettingsfiles.cpp                                          */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 02/26/2006 */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTCUTSETTINGSFILES
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

#include "ttcutsettingsfiles.h"

#include "../common/ttcut.h"

  
TTCutSettingsFiles::TTCutSettingsFiles(QWidget* parent)
:QWidget(parent)
{
  setupUi(this);
}

void TTCutSettingsFiles::setTitle(__attribute__((unused))const QString& title)
{
}

// set the tab data from the global parameter
void TTCutSettingsFiles::setTabData()
{
  cbCreateVideoIDD->setChecked( TTCut::createVideoIDD );
  cbCreateAudioIDD->setChecked( TTCut::createAudioIDD );
  cbReadVideoIDD->setChecked( TTCut::readVideoIDD );
  cbReadAudioIDD->setChecked( TTCut::readAudioIDD );

  cbCreateLog->setChecked( TTCut::createLogFile );
  cbLogExtended->setChecked( TTCut::logModeExtended );
  cbLogPlusVideoIndex->setChecked( TTCut::logVideoIndexInfo );
}

// get the tab data and fill the global parameter
void TTCutSettingsFiles::getTabData()
{
  // IDD files
  TTCut::createVideoIDD    = cbCreateVideoIDD->isChecked( );
  TTCut::createAudioIDD    = cbCreateAudioIDD->isChecked( );
  TTCut::readVideoIDD      = cbReadVideoIDD->isChecked( );
  TTCut::readAudioIDD      = cbReadAudioIDD->isChecked( );

  // logfile
  TTCut::createLogFile     = cbCreateLog->isChecked( );
  TTCut::logModeExtended   = cbLogExtended->isChecked( );
  TTCut::logVideoIndexInfo = cbLogPlusVideoIndex->isChecked( );
}


