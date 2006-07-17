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
// TTMPLEXPROVIDER
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

/**
 *Usage: mplex [params] -o <output filename pattern> <input file>... 
 *%d in the output file name is by segment count
 *where possible params are:
 *--verbose|-v num
 *  Level of verbosity. 0 = quiet, 1 = normal 2 = verbose/debug
 *--format|-f fmt
 *  Set defaults for particular MPEG profiles
 *  [0 = Generic MPEG1, 1 = VCD, 2 = user-rate VCD, 3 = Generic MPEG2,
 *   4 = SVCD, 5 = user-rate SVCD
 *   6 = VCD Stills, 7 = SVCD Stills, 8 = DVD with NAV sectors, 9 = DVD]
 *--mux-bitrate|-r num
 *  Specify data rate of output stream in kbit/sec
 *  (default 0=Compute from source streams)
 *--video-buffer|-b num [, num...] 
 *  Specifies decoder buffers size in kB.  [ 20...2000]
 *--lpcm-params | -L samppersec:chan:bits [, samppersec:chan:bits]
 *--mux-limit|-l num
 *  Multiplex only num seconds of material (default 0=multiplex all)
 *--sync-offset|-O num ms|s|mpt
 *  Specify offset of timestamps (video-audio) in mSec
 *--sector-size|-s num
 *  Specify sector size in bytes for generic formats [256..16384]
 *--vbr|-V
 *  Multiplex variable bit-rate video
 *--packets-per-pack|-p num
 *  Number of packets per pack generic formats [1..100]
 *--system-headers|-h
 *  Create System header in every pack in generic formats
 *--max-segment-size|-S size
 *  Maximum size of output file(s) in Mbyte (default: 0) (no limit)
 *--split-segment|-M
 *  Simply split a sequence across files rather than building run-out/run-in
 *--workaround|-W workaround [, workaround ]
 *--help|-?
 *  Print this lot out!
 */

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

//! Level of verbosity (--verbose|-v num)
int TTMplexProvider::createVerboseHash()
{
  verbose["quiet"]         = 0;
  verbose["normal"]        = 1;
  verbose["verbose/debug"] = 2;

  return verbose.size();
}

//! Set defaults for particular MPEG profiles (--format|-f fmt)
int TTMplexProvider::createFormatHash()
{
  format["Generic MPEG1"]        = 0;
  format["VCD"]                  = 1;
  format["user-rate VCD"]        = 2;
  format["Generic MPEG2"]        = 3;
  format["SVCD"]                 = 4;
  format["user-rate VCD"]        = 5;
  format["VCD Stills"]           = 6;
  format["SVCD Stills"]          = 7;
  format["DVD with NAV sectors"] = 8;
  format["DVD"]                  = 9;

  return format.size();
}
