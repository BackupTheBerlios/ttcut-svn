/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttmpeg2videoheader.h                                            */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 05/12/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTSEQUENCEHEADER
// *** TTSEQUENCEENDHEADER
// *** TTGOPHEADER
// *** TTPICTURESHEADER
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


#ifndef TTMPEG2VIDEOHEADER_H
#define TTMPEG2VIDEOHEADER_H

#include "ttcommon.h"

#ifdef __WIN32
#include "ttwfilebuffer.h"
#else
#include "ttfilebuffer.h"
#endif

#include "ttavheader.h"




// -----------------------------------------------------------------------------
// Abstract class TTMpeg2VideoHeader
// -----------------------------------------------------------------------------
class TTMpeg2VideoHeader : public TTVideoHeader
{
public:
  TTMpeg2VideoHeader();

  virtual bool readHeader( TTFileBuffer* mpeg2_stream );
  virtual bool readHeader( TTFileBuffer* mpeg2_stream, off64_t offset );
  virtual void parseBasicData( uint8_t* data, int offset=0 );
  virtual void parseExtendedData( uint8_t* data, int offset=0 );
  virtual void printHeader( );

enum mpeg2StartCodes
  {
    picture_start_code            = 0x00,
    userdata_start_code           = 0xb2,
    sequence_start_code           = 0xb3,
    sequence_error_code           = 0xb4,
    sequence_extension_code       = 0xb5,
    sequence_end_code             = 0xb7,
    group_start_code              = 0xb8,
    sequence_extension_id         = 0x01,
    sequence_display_extension_id = 0x02
  };

 protected:
  uint8_t extension_start_code;
  uint8_t extension_start_code_identifier;
};

// -----------------------------------------------------------------------------
// Sequence header [0x000001B3]
// -----------------------------------------------------------------------------
class TTSequenceHeader : public TTMpeg2VideoHeader
{
 public:
  TTSequenceHeader();

  bool readHeader( TTFileBuffer* mpeg2_stream );
  bool readHeader( TTFileBuffer* mpeg2_stream, off64_t offset );
  bool writeDisplayExtension(TTFileBuffer* mpeg2_stream, off64_t offset);
  void parseBasicData( uint8_t* data, int offset=0);
  void parseExtendedData( uint8_t* data, int offset=0 );
  void parseExtendedData(TTFileBuffer* mpeg_stream);
  void printHeader( );

  int     horizontalSize();
  int     verticalSize();
  QString aspectRatioText();
  QString frameRateText();
  float   frameRateValue();
  float   bitRateKbit();
  int     vbvBufferSize();

  // from sequence [B3]
  int      horizontal_size_value;
  int      vertical_size_value;
  int      aspect_ratio_information;
  int      frame_rate_code;
  int      bit_rate_value;
  uint8_t  marker_bit;
  int      vbv_buffer_size_value;
  uint8_t  constrained_parameters_flag;
  bool     load_intra_quantiser_matrix;
  uint8_t* intra_quantiser_matrix;
  bool     load_non_intra_quantiser_matrix;
  uint8_t* non_intra_quantiser_matrix;

  //from sequence_extension [B5]
  bool     sequence_extension_exist;
  int      profile_and_level_indication;
  bool     progressive_sequence;
  int      chroma_format;
  int      horizontal_size_extension;
  int      vertical_size_enxtension;
  int      bit_rate_extension;
  int      vbv_buffer_size_extension;
  bool     low_delay;
  int      frame_rate_extension_n;
  int      frame_rate_extension_d;

  // from sequence_display_extension
  int      video_format;
  bool     colour_description;
  int      colour_primaries;
  int      transfer_characteristics;
  int      matrix_coefficients;
  int      display_horizontal_size;
  uint8_t  display_extension_marker_bit;
  int      display_vertical_size;

  // internal:
  int      pictures_in_sequence;          // number of pictures in sequence
  bool     is_sequence_extension;         // sequence has extension
  bool     is_sequence_diplay_extension;  // sequence has diplay extension
};

/*! \brief SequenceEndHeader
 * 
 */
class TTSequenceEndHeader : public TTMpeg2VideoHeader
{
 public:
  TTSequenceEndHeader();

  bool readHeader( TTFileBuffer* mpeg2_stream );
  bool readHeader( TTFileBuffer* mpeg2_stream, off64_t offset );
  void parseBasicData( uint8_t* data, int offset=0);
  void parseExtendedData( uint8_t* data, int offset=0 );
  void printHeader( );

};

// -----------------------------------------------------------------------------
// Group of pictures header [000001B8]
// -----------------------------------------------------------------------------
class TTGOPHeader : public TTMpeg2VideoHeader
{
public:
   TTGOPHeader();

  bool readHeader( TTFileBuffer* mpeg2_stream );
  bool readHeader( TTFileBuffer* mpeg2_stream, off64_t offset );
  void parseBasicData( uint8_t* data, int offset=0 );
  void parseExtendedData( uint8_t* data, int offset=0 );
  void printHeader( );

   // from group_of_pictures_header [B8]
   TTimeCode time_code;
   bool      closed_gop;
   bool      broken_link;

   // internal: number of pictures in GOP
   int       pictures_in_gop;
};

// -----------------------------------------------------------------------------
// Pictures header [00000100]
// -----------------------------------------------------------------------------
class TTPicturesHeader : public TTMpeg2VideoHeader
{
 public:
  TTPicturesHeader();

  bool    readHeader( TTFileBuffer* mpeg2_stream );
  bool    readHeader( TTFileBuffer* mpeg2_stream, off64_t offset );
  void    parseBasicData( uint8_t* data, int offset=0 );
  void    parseExtendedData( uint8_t* data, int offset=0 );
  QString codingTypeString();
  void    printHeader( );

  // from picture_header [00]
  int     temporal_reference;
  int     picture_coding_type;
  int     vbv_delay;
  //picture_coding_type == 2 || picture_coding_type == 1
  int     full_pel_forward_vector;
  uint8_t forward_f_code;
  //picture_coding_type == 3
  int     full_pel_backward_vector;
  uint8_t backward_f_code;

  // from picture_coding_extension [B5]
  int     f_code[2][2];
  int     intra_dc_precision;
  int     picture_structure;
  bool    top_field_first;
  bool    frame_pred_frame_dct;
  int     concealment_motion_vectors;
  int     q_scale_type;
  int     intra_vlc_format;
  bool    alternate_scan;
  bool    repeat_first_field;
  bool    chroma_420_type;
  bool    progressive_frame;
  bool    composite_display_flag;
  //if(composite_display_flag)
  int     v_axis;
  int     field_sequence;
  int     sub_carrier;
  int     burst_amplitude;
  int     sub_carrier_phase;

  // internal
  int abs_picture_number;
};

#endif //TTMPEG2VIDEOHEADER_H
