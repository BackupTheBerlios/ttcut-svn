/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttavcutposition.cpp                                             */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 02/23/2005 */
/* MODIFIED: b. altendorf                                    DATE: 06/20/2005 */
/* MODIFIED: b. altendorf                                    DATE: 06/22/2005 */
/* MODIFIED: b. altendorf                                    DATE: 08/13/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTCUTPARAMETER
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

//#define TTAVCUTLIST_DEBUG
//#define TTCUTPARAMETER_DEBUG

const char c_name[] = "TTCutParameter";

TTCutParameter::TTCutParameter()
{
#if defined(TTCUTPARAMETER_DEBUG)
  qDebug( "%screate cut parameter object",c_name );
#endif
  pictures_written            = 0;
  first_call                  = true;
  last_call                   = false;
  write_max_bitrate           = false;
  write_sequence_end_code     = false;
  create_dvd_compliant_stream = false;

  result_header_list = (TTVideoHeaderList*)NULL;
}

TTCutParameter::~TTCutParameter()
{
#if defined(TTCUTPARAMETER_DEBUG)
  qDebug( "%sdelete cut parameter object",c_name );
#endif
  if ( ttAssigned( result_header_list ) )
  {
#if defined(TTCUTPARAMETER_DEBUG)
    qDebug( "%sdelete header list",c_name );
#endif
    delete result_header_list;
  }
}

bool TTCutParameter::writeSequenceEndCode()
{
  return write_sequence_end_code;
}

bool TTCutParameter::writeMaxBitrate()
{
  return write_max_bitrate;
}


bool TTCutParameter::createDVDCompilantStream()
{
  return create_dvd_compliant_stream;
}  

