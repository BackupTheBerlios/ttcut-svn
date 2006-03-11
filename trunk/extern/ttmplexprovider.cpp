/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttmplexprovider.cpp                                             */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 03/11/2006 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTMPLEXPROVIDER
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

#include "ttmplexprovider.h"

#include "../common/ttcut.h"
#include "../data/ttmuxlistdata.h"

#include <QString>
#include <QStringList>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

TTMplexProvider::TTMplexProvider()
{
}

TTMplexProvider::~TTMplexProvider()
{
}

void TTMplexProvider::writeMuxScript(TTMuxListData* muxData)
{
  QStringList mplexCmd;
  QString     mpegFileName;
  QFileInfo   muxInfo(QDir(TTCut::cutDirPath), "muxscript.sh" );
  QFile       muxFile(muxInfo.absoluteFilePath());

  if (muxInfo.exists())
    muxFile.remove();

  muxFile.open(QIODevice::WriteOnly | QIODevice::Text);

  QTextStream muxOutStream(&muxFile);

  muxOutStream << "# TTCut - Mplex script ver. 0.10.1" << "\n";
  muxOutStream << "#!/bin/sh" << "\n";
  muxOutStream << "#\n";

  for (int i=0; i < muxData->count(); i++) {

    mplexCmd.clear();

    // video output file name
    mpegFileName = ttChangeFileExt(muxData->videoFileAt(i), "mpg");

    mplexCmd << "mplex"
      << "-f8"
      << "-o"
      << mpegFileName
      << muxData->videoFileAt(i);

    for (int j=0; j < muxData->numAudioFilesAt(i); j++) {
      mplexCmd << muxData->audioFileAt(i, j);
    }
    muxOutStream << mplexCmd.join(" ") << "\n";
    muxOutStream << "#\n";
  }
  muxFile.flush();
  muxFile.close();
}
