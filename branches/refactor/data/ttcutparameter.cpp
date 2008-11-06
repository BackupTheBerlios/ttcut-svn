/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005/2010 / www.tritime.org                    */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttavcutposition.cpp                                             */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 02/23/2005 */
/* MODIFIED: b. altendorf                                    DATE: 06/18/2008 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTCUTPARAMETER
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

#include "ttcutparameter.h"

#include "../avstream/ttmpeg2videoheader.h"

const char c_name[] = "TTCutParameter";

TTCutParameter::TTCutParameter(TTFileBuffer* fBuffer)
{
  targetStreamBuffer   = fBuffer;
  isWriteSequenceEnd   = false;
  isWriteMaxBitrate    = false;
  isDVDCompliantStream = false;
  numPicturesWritten   = 0;
  resultHeaderList     = new TTVideoHeaderList(1000);
}

TTCutParameter::~TTCutParameter()
{
  resultHeaderList->clear();
}

TTFileBuffer* TTCutParameter::getTargetStreamBuffer()
{
  return targetStreamBuffer;
}

bool TTCutParameter::getIsWriteSequenceEnd()
{
  return isWriteSequenceEnd;
}

void TTCutParameter::setIsWriteSequenceEnd(bool value)
{
  isWriteSequenceEnd = value;
}

bool TTCutParameter::getIsWriteMaxBitrate()
{
  return isWriteMaxBitrate;
}

void TTCutParameter::setIsWriteMaxBitrate(bool value)
{
  isWriteMaxBitrate = value;
}

bool TTCutParameter::getIsDVDCompliantStream()
{
  return isDVDCompliantStream;
}  

void TTCutParameter::setIsDVDCompliantStream(bool value)
{
  isDVDCompliantStream = value;
}

int TTCutParameter::getNumPicturesWritten()
{
  return numPicturesWritten;
}

void TTCutParameter::setNumPicturesWritten(int value)
{
  numPicturesWritten = value;
}

TTVideoHeaderList* TTCutParameter::getResultHeaderList()
{
  return resultHeaderList;
}

int TTCutParameter::getMaxBitrate()
{
  return maxBitrateValue;
}

void TTCutParameter::setMaxBitrate(int value)
{
  maxBitrateValue = value;
}

int TTCutParameter::getCutInIndex()
{
  return cutInIndex;
}

void TTCutParameter::setCutInIndex(int value)
{
  cutInIndex = value;
}

int TTCutParameter::getCutOutIndex()
{
  return cutOutIndex;
}

void TTCutParameter::setCutOutIndex(int value)
{
  cutOutIndex = value;
}
  
void TTCutParameter::firstCall()
{
  if (!ttAssigned(resultHeaderList))
    resultHeaderList = new TTVideoHeaderList(1000);
  else
    resultHeaderList->deleteAll();
}

void TTCutParameter::lastCall()
{
  if(isWriteSequenceEnd)
    writeSequenceEndHeader();
}

void TTCutParameter::writeSequenceEndHeader()
{
  quint8 seqEndCode[4];

  seqEndCode[0] = 0x00;
  seqEndCode[1] = 0x00;
  seqEndCode[2] = 0x01;
  seqEndCode[3] = 0xb7;

  TTSequenceEndHeader* sequenceEndHeader = new TTSequenceEndHeader();
  sequenceEndHeader->setHeaderOffset(targetStreamBuffer->position());

  resultHeaderList->add(sequenceEndHeader);
  targetStreamBuffer->directWrite(seqEndCode, 4);
}
