/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttavcutposition.h                                               */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 02/23/2005 */
/* MODIFIED: b. altendorf                                    DATE: 06/20/2005 */
/* MODIFIED: b. altendorf                                    DATE: 06/22/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTAVCUTPOSITION
// *** TTAVCUTLIST
// ----------------------------------------------------------------------------

/*----------------------------------------------------------------------------*/
/* Die Klasse TSchnittPunkt entstammt dem Projekt "MPEG2Schnitt" von Martin   */
/* Dienert und wurde von mir nach C++ portiert und an mein Projekt angepasst. */
/* Copyright (C) 2003  Martin Dienert                                         */
/* Homepage: http:www.mdienert.de/mpeg2schnitt/                               */
/* E-Mail:   m.dienert@gmx.de                                                 */
/* Martin Dienert ist nicht verantwortlich fuer diese Quellen und leistet     */
/* diesbezueglich auch keinen Support.                                        */
/*----------------------------------------------------------------------------*/

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
#ifndef TTAVCUTPOSITION_H
#define TTAVCUTPOSITION_H

#include <qdatetime.h>
#include <q3ptrvector.h>

#include "../avstream/ttcommon.h"
#include "../avstream/ttvideoheaderlist.h"

// -----------------------------------------------------------------------------
// TTAVCutPosition
// -----------------------------------------------------------------------------
class TTAVCutPosition
{
 public:
  TTAVCutPosition();
  TTAVCutPosition( uint c_in, uint c_out, uint c_order );
  TTAVCutPosition( uint c_in,  off64_t c_in_off,
		   uint c_out, off64_t c_out_off, uint c_order );
  ~TTAVCutPosition();

  void    setCutIn( uint c_in );
  void    setCutOut( uint c_out );
  void    setCutInOffset( off64_t c_in );
  void    setCutOutOffset( off64_t c_out );
  void    setCutOrder( uint c_order );
  uint    cutIn();
  uint    cutOut();
  off64_t cutInOffset();
  off64_t cutOutOffset();
  uint    cutOrder();

 private:
  uint    cut_in_position;
  uint    cut_out_position;
  off64_t cut_in_offset;
  off64_t cut_out_offset;
  uint    cut_order;
};


// -----------------------------------------------------------------------------
// TTAVCutList
// -----------------------------------------------------------------------------
class TTAVCutList : public Q3PtrVector<TTAVCutPosition>
{
 public:
  TTAVCutList( uint size=10 );
  ~TTAVCutList();

  uint addCutPosition( uint c_in, uint c_out, uint c_order );
  uint addCutPosition( uint c_in, off64_t c_in_off,
		       uint c_out, off64_t c_out_off, uint c_order );
  uint    addCutPosition( TTAVCutPosition* c_pos );
  uint    cutInAt( uint i_pos );
  uint    cutOutAt( uint i_pos );
  off64_t cutInOffsetAt( uint i_pos );
  off64_t cutOutOffsetAt( uint i_pos );
  uint    cutOrderAt( uint i_pos );
  uint    setCutOrderAt( uint i_pos, uint c_order );
  TTAVCutPosition* entryAt( uint i_pos );
  TTAVCutPosition* entryAtOrder( uint c_order );
  void    deleteAll();
  uint    removeEntryAt( uint i_pos );
  uint    removeEntryAtOrder( uint c_order );
  void    sortCutOrder();

 protected:
  int compareItems( Q3PtrCollection::Item Item1, Q3PtrCollection::Item Item2 );

 private:
  TTAVCutPosition* cut_position;
  uint             actual_size;
  uint             ins_position;
};


class TTCutParameter
{
 public:
  TTCutParameter();

  bool  writeSequenceEndCode();
  bool  writeMaxBitrate();
  bool  createDVDCompilantStream();

  int   max_bitrate;
  uint  pictures_written;
  bool  first_call;
  bool  last_call;
  TTVideoHeaderList* result_header_list;
  bool  write_max_bitrate;
  bool  write_sequence_end_code;
  bool  create_dvd_compliant_stream;
  float compliance_max_bitrate;
  int   compliance_max_mrames;
};


#endif //TTAVCUTPOSITION_H
