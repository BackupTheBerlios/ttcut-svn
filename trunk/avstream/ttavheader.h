/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttavheader.h                                                    */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 05/12/2005 */
/* MODIFIED: b. altendorf                                    DATE: 08/13/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTAVHEADER (abstract)
// TTAUDIOHEADER
// TTVIDEOHEADER
// TTVIDEOINDEX
// ----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Overview
// -----------------------------------------------------------------------------
//
//                               +- TTAC3AudioHeader
//                               |
//                               +- TTMpegAudioHeader
//             +- TTAudioHeader -|                    +- TTDTS14AudioHeader
//             |                 +- TTDTSAudioHeader -|
//             |                 |                    +- TTDTS16AudioHeader
// TTAVHeader -|                 +- TTPCMAudioHeader
//             |
//             |                                     +- TTSequenceHeader
//             |                                     |
//             |                                     +- TTSequenceEndHeader
//             +- TTVideoHeader -TTMpeg2VideoHeader -|
//             |                                     +- TTPicturesHeader
//             |                                     |
//             |                                     +- TTGOPHeader
//             |
//             +- TTVideoIndex
//             |
//             +- TTBreakObject
//
// -----------------------------------------------------------------------------

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

#ifndef TTAVHEADER_H
#define TTAVHEADER_H

#include "ttcommon.h"

#ifdef __WIN32
#include "ttwfilebuffer.h"
#else
#include "ttfilebuffer.h"
#endif

class QString;

// -----------------------------------------------------------------------------
// *** TTAVHeader: Base class for all header objects
// -----------------------------------------------------------------------------
class TTAVHeader
{
public:
  TTAVHeader();
  virtual ~TTAVHeader();

  virtual QString& descString();
  virtual QString& modeString();
  virtual QString& bitRateString();
  virtual QString& sampleRateString();

  virtual uint8_t  headerType();
  virtual void     setHeaderType( uint8_t h_type ) {header_start_code = h_type;}
  virtual off64_t  headerOffset();
  virtual void     setHeaderOffset( off64_t h_offset ){header_offset = h_offset;}

protected:
  off64_t  header_offset;
  uint8_t  header_start_code;
  QString* str_description;
  QString* str_mode;
  QString* str_bit_rate;
  QString* str_sample_rate;
};


// -----------------------------------------------------------------------------
// *** TTAudioHeader: Base class for all audio header objects
// -----------------------------------------------------------------------------
class TTAudioHeader : public TTAVHeader
{
public:
  TTAudioHeader();

  virtual int     bitRate();
  virtual int     sampleRate();
  virtual double  absFrameEndTime();
  virtual int     compareTo();
  virtual int     frameLength();

  //protected:
  long   position;  // header offset ???
  float  frame_time;
  float  abs_frame_time;
  int    frame_length;
  int    bit_rate;
  int    sample_rate;
};

// -----------------------------------------------------------------------------
// *** TTVideoHeader: Base class for all video header objects
// -----------------------------------------------------------------------------
class TTVideoHeader : public TTAVHeader
{
public:
  TTVideoHeader();

  virtual bool readHeader( TTFileBuffer* mpeg2_stream ){ return false; };
  virtual bool readHeader( TTFileBuffer* mpeg2_stream, off64_t offset ){ return false; };
  virtual void parseBasicData( uint8_t* data, int offset=0){};
  virtual void parseExtendedData( uint8_t* data, int offset=0 ){};
  virtual void printHeader( ){};

 protected:
  typedef struct
  {
    bool    drop_frame_flag;
    int     hours;
    int     minutes;
    bool    marker_bit;
    int     seconds;
    int     pictures;
  } TTimeCode;

};

// -----------------------------------------------------------------------------
// TTVideoIndex: Object (data) for the video index list
// -----------------------------------------------------------------------------
class TTVideoIndex : public TTAVHeader
{
 public:
  TTVideoIndex():TTAVHeader(){};

  int     stream_order;
  int     display_order;
  int     header_list_index;
  int     picture_coding_type;
  int     sequence_index;
  long    gop_number;
  off64_t picture_size;
};


// -----------------------------------------------------------------------------
// TTBreakObject
// -----------------------------------------------------------------------------
class TTBreakObject
{
 public:
  TTBreakObject();

  TTVideoHeader* stop_object;
  TTVideoHeader* restart_object;
  long copy_start;
  long copy_stopp;
};

#endif //TTAVHEADER_H


