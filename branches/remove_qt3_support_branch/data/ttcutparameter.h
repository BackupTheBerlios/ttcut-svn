/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttavcutposition.h                                               */
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
#ifndef TTCUTPARAMETER_H
#define TTCUTPARAMETER_H

#include <qdatetime.h>

#include <QVector>

#include "../avstream/ttcommon.h"
#include "../avstream/ttvideoheaderlist.h"

// -----------------------------------------------------------------------------
// TTCutParameter
// -----------------------------------------------------------------------------
class TTCutParameter
{
 public:
  TTCutParameter();
  ~TTCutParameter();

  bool  writeSequenceEndCode();
  bool  writeMaxBitrate();
  bool  createDVDCompilantStream();

  int   max_bitrate;
  int   pictures_written;
  bool  first_call;
  bool  last_call;
  TTVideoHeaderList* result_header_list;
  bool  write_max_bitrate;
  bool  write_sequence_end_code;
  bool  create_dvd_compliant_stream;
  float compliance_max_bitrate;
  int   compliance_max_mrames;
};


#endif //TTCUTPARAMETER_H
