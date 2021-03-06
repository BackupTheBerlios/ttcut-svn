/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttmpeg2videoheader.cpp                                          */
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


#include "ttmpeg2videoheader.h"

// -----------------------------------------------------------------------------
// TTMpeg2VideoHeader
// -----------------------------------------------------------------------------
TTMpeg2VideoHeader::TTMpeg2VideoHeader()
{

}

bool TTMpeg2VideoHeader::readHeader( __attribute__ ((unused))TTFileBuffer* mpeg2_stream )
{
return false;
}

bool TTMpeg2VideoHeader::readHeader( __attribute__ ((unused))TTFileBuffer* mpeg2_stream, __attribute__ ((unused))off64_t offset )
{
return false;
}

void TTMpeg2VideoHeader::parseBasicData( __attribute__ ((unused))uint8_t* data, __attribute__ ((unused))int offset )
{

}


void TTMpeg2VideoHeader::parseExtendedData( __attribute__ ((unused))uint8_t* data, __attribute__ ((unused))int offset )
{

}


void TTMpeg2VideoHeader::printHeader( )
{

}



// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// TTSequenceHeader: Sequence header [0x000001B3]
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////
TTSequenceHeader::TTSequenceHeader()
  :TTMpeg2VideoHeader()
{

}

bool TTSequenceHeader::readHeader( TTFileBuffer* mpeg2_stream )
{
  uint8_t  header_data[8];
  uint8_t  start_code;

  try
  {
    // read 8 byte from stream, starting at current offset
    mpeg2_stream->readArray( header_data, 8 ) ;

    // fill sequence header
    header_start_code = sequence_start_code;
    header_offset     = mpeg2_stream->currentOffset() - 12;

    parseBasicData( header_data );

    if ( (header_data[7] & 0x02) == 2 )
      mpeg2_stream->seekRelative( 64 );  //Seek Exception

    if ( (header_data[7] & 0x01) == 1 )
      mpeg2_stream->seekRelative( 64 ); //Seek Exception

    do
    {
      // read next start code
      mpeg2_stream->nextStartCodeTS();

      mpeg2_stream->readByte( start_code );
	
      //sequence_extension_code [0xB5]
      if ( header_data[0] == sequence_extension_code )
      {
	is_sequence_extension = true;

	mpeg2_stream->readArray( header_data, 6 );

	// sequence_extension_id
	switch ((header_data[0] & 0xF0) >> 4)
	{
	case sequence_extension_id:
	  profile_and_level_indication = ((header_data[0] & 0x0F) << 4) + ((header_data[1] & 0xF0) >> 4);
	  progressive_sequence         = ((header_data[1] & 0x08) >> 3) == 1;
	  chroma_format                = (header_data[1] & 0x06) >> 1;
	  horizontal_size_value       += ((header_data[1] & 0x01) << 13) + ((header_data[2] & 0x80) << 5);
	  vertical_size_value         += ((header_data[2] & 0x60) << 7);
	  bit_rate_value              += ((header_data[2] & 0x1F) << 25) + ((header_data[3] & 0xFE) << 18);
	  vbv_buffer_size_value       += (header_data[4] << 10);
	  low_delay                   = ((header_data[5] & 0x80) >> 7) == 1;
	  break;

	case sequence_display_extension_id:
	  video_format  = ((header_data[0] & 0x0e)>>1);
	  mpeg2_stream->seekRelative( -4 );
	  break;
	}
      }
    } while ( start_code == sequence_extension_code );

    mpeg2_stream->seekRelative( -4 );
  }
  catch ( TTStreamSeekException )
  {
    return false;
  }
  catch ( TTStreamEOFException )
  {
    return false;
  }
  return true;
}


bool TTSequenceHeader::readHeader( TTFileBuffer* mpeg2_stream, off64_t offset )
{
  mpeg2_stream->seekAbsolute( offset+4 );

  return readHeader( mpeg2_stream );
}


void TTSequenceHeader::parseBasicData( uint8_t* data, int offset )
{
  horizontal_size_value        = (data[offset+0] << 4) + ((data[1] & 0xF0) >> 4);
  vertical_size_value          = ((data[1] & 0x0F) << 8) + data[2];
  aspect_ratio_information     = (data[3] & 0xF0) >> 4;
  frame_rate_code              = (data[3] & 0x0F);
  bit_rate_value               = (int)(((data[4] << 10) + (data[5] << 2)+((data[6] & 0xC0) >> 6))*400);
  vbv_buffer_size_value        = ((data[6] & 0x1F) << 5)+((data[7] & 0xF8) >> 3);
  profile_and_level_indication = 0;
  progressive_sequence         = false;
  chroma_format                = 0;
  low_delay                    = false;
}

void TTSequenceHeader::parseExtendedData( __attribute__ ((unused))uint8_t* data, __attribute__ ((unused))int offset )
{

}

void TTSequenceHeader::printHeader( )
{

}


int TTSequenceHeader::horizontalSize()
{
  return horizontal_size_value;
}


int TTSequenceHeader::verticalSize()
{
  return vertical_size_value;
}


QString TTSequenceHeader::aspectRatioText()
{
  QString szTemp;

  if ( aspect_ratio_information == 1 ) szTemp = "1:1";
  if ( aspect_ratio_information == 2 ) szTemp = "4:3";
  if ( aspect_ratio_information == 3 ) szTemp = "16:9";
  if ( aspect_ratio_information == 4 ) szTemp = "1:2";

  return szTemp;
}


QString TTSequenceHeader::frameRateText()
{
  QString szTemp;

  if ( frame_rate_code == 2 ) szTemp = "24 fps";
  if ( frame_rate_code == 3 ) szTemp = "25 fps";
  if ( frame_rate_code == 5 ) szTemp = "30 fps";

  return szTemp;
}


float TTSequenceHeader::frameRateValue()
{
  float value = 25.0;

  if ( frame_rate_code == 2 ) value = 24.0;
  if ( frame_rate_code == 3 ) value = 25.0;
  if ( frame_rate_code == 5 ) value = 30.0;

  if ( frame_rate_code < 2 || frame_rate_code > 5 )
  {
    //qDebug("Error frame_rate_code (!): %d",frame_rate_code);
    value = 25.0;
  }

  return value;
}

float TTSequenceHeader::bitRateKbit()
{
  return (float)bit_rate_value / 1000.0;
}


int TTSequenceHeader::vbvBufferSize()
{
  return vbv_buffer_size_value;
}


// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// TTSequenceEndHeader
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////
TTSequenceEndHeader::TTSequenceEndHeader()
  : TTMpeg2VideoHeader()
{

}

bool TTSequenceEndHeader::readHeader( TTFileBuffer* mpeg2_stream )
{
  header_start_code = sequence_end_code;
  header_offset     = mpeg2_stream->currentOffset() - 4;

  return true;
}

bool TTSequenceEndHeader::readHeader( TTFileBuffer* mpeg2_stream, off64_t offset )
{
  mpeg2_stream->seekAbsolute( offset+4 );

  return readHeader( mpeg2_stream );
}

void TTSequenceEndHeader::parseBasicData( __attribute__ ((unused))uint8_t* data, __attribute__ ((unused))int offset )
{

}

void TTSequenceEndHeader::parseExtendedData( __attribute__ ((unused))uint8_t* data, __attribute__ ((unused))int offset )
{

}

void TTSequenceEndHeader::printHeader( )
{

}


// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// TTGOPHeader: Group of pictures header [000001B8]
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////
TTGOPHeader::TTGOPHeader()
  :TTMpeg2VideoHeader()
{

}

bool TTGOPHeader::readHeader( TTFileBuffer* mpeg2_stream )
{
  uint8_t header_data[4];

  try
  {
    mpeg2_stream->readArray( header_data,4 );

    header_start_code = group_start_code;
    header_offset     = mpeg2_stream->currentOffset() - 8;

    parseBasicData( header_data );

    return true;
  }
  catch ( TTStreamEOFException )
  {
    return false;
  }
}

bool TTGOPHeader::readHeader( TTFileBuffer* mpeg2_stream, off64_t offset )
{
  mpeg2_stream->seekAbsolute( offset+4 );

  return readHeader( mpeg2_stream );
}

void TTGOPHeader::parseBasicData( uint8_t* data, int offset )
{
  time_code.drop_frame_flag = (data[offset+0] >> 7) == 1;
  time_code.hours           = (int)((data[offset+0] & 0x7C) >> 2);
  time_code.minutes         = (int)(((data[offset+0] & 0x03) << 4) + ((data[offset+1] & 0xF0) >> 4));
  time_code.marker_bit      = ((data[offset+1] & 0x08) >> 3) == 1;
  time_code.seconds         = (int)(((data[offset+1] & 0x07) << 3) + ((data[offset+2] & 0xE0) >> 5));
  time_code.pictures        = (int)(((data[offset+2] & 0x1F) << 1) + ((data[offset+3] & 0x80) >> 7));
  closed_gop                = ((data[offset+3] & 0x40) >> 6) == 1;
  broken_link               = ((data[offset+3] & 0x20) >> 5) == 1;
}

void TTGOPHeader::parseExtendedData( __attribute__ ((unused))uint8_t* data, __attribute__ ((unused))int offset )
{

}

void TTGOPHeader::printHeader( )
{

}


// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// TTPicturesHeader: Pictures header [00000100]
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////
TTPicturesHeader::TTPicturesHeader()
  :TTMpeg2VideoHeader()
{

}

bool TTPicturesHeader::readHeader( TTFileBuffer* mpeg2_stream )
{
  uint8_t header_data[5];
  uint8_t byte1;

  try
  {
    mpeg2_stream->readArray( header_data, 4 );

    header_start_code = picture_start_code;
    header_offset     = mpeg2_stream->currentOffset() - 8;

    parseBasicData( header_data );

    mpeg2_stream->nextStartCodeTS();

    mpeg2_stream->readArray( header_data, 1 );

    if ( header_data[0] == 0xb5 )
    {
      mpeg2_stream->readArray( header_data,5 );

      //picture_coding_extension
      if ((( header_data[0]&0xf0)>>4) == 8 )
      {
	//full information
	//----------------------------------------------------------------
	f_code[0][0]               =  (header_data[0] & 0x0F);
	f_code[0][1]               =  (header_data[1] & 0xF0);
	f_code[1][0]               =  (header_data[1] & 0x0F);
	f_code[1][1]               =  (header_data[2] & 0xF0);
	intra_dc_precision         =  (header_data[2] & 0x0C) >> 2;
	picture_structure          =   header_data[2] & 0x03;
	top_field_first            = ((header_data[3] & 0x80) >> 7) == 1;
	frame_pred_frame_dct       = ((header_data[3] & 0x40) >> 6) == 1;
	concealment_motion_vectors = ((header_data[3] & 0x20) >> 5) == 1;
	q_scale_type               = ((header_data[3] & 0x10) >> 4) == 1;
	intra_vlc_format           = ((header_data[3] & 0x08) >> 3) == 1;
	alternate_scan             = ((header_data[3] & 0x04) >> 2) == 1;
	repeat_first_field         = ((header_data[3] & 0x02) >> 1) == 1;
	chroma_420_type            = ( header_data[3] & 0x01) == 1;
	progressive_frame          = ((header_data[4] & 0x80) >> 7) == 1;
	composite_display_flag     = ((header_data[4] & 0x40) >> 6) == 1;
	
	//minimal
	//----------------------------------------------------------------
	//intra_dc_precision       = (int)((header_data[2] & 0x0c)>>2);
	//picture_structure        = (int)(header_data[2] & 0x03);
	//top_field_first          = ((header_data[3] & 0x80) >> 7) == 1;
	//composite_display_flag = ((header_data[4] & 0x40) >> 6) == 1;

	// composite_display_flag
	if (((header_data[4] & 0x40) >> 6) == 1)
	{
	  mpeg2_stream->readByte( byte1 );

	  field_sequence = ((header_data[4] & 0x1C) >> 2);
	  sub_carrier    = ((header_data[4] & 0x02) >> 1) == 1;
	  //burst_amplitude   = 0;     // wird nicht gebraucht
	  //sub_carrier_phase = 0;     // wird nicht gebraucht
		
	  return true;
	}
      }
      else
      {
	mpeg2_stream->seekRelative( -4 );
      }
    }
    return true;
  }
  catch ( TTStreamSeekException )
  {
    return false;
  }
  catch ( TTStreamEOFException )
  {
    return false;
  }
}

bool TTPicturesHeader::readHeader( TTFileBuffer* mpeg2_stream, off64_t offset )
{
  mpeg2_stream->seekAbsolute( offset+4 );
  return readHeader( mpeg2_stream );
}

void TTPicturesHeader::parseBasicData( uint8_t* data, int offset )
{
  picture_coding_type = (int)((data[offset+1] & 0x38) >> 3);
  temporal_reference  = (int)((data[offset+0] << 2) + ((data[offset+1] & 0xC0) >> 6));
  vbv_delay           = ((data[offset+1] & 0x07) << 13) + (data[offset+2] << 5) + ((data[offset+3] & 0xF8) >> 3);

  //qDebug("coding type: %d",picture_coding_type);
}

void TTPicturesHeader::parseExtendedData( __attribute__ ((unused))uint8_t* data, __attribute__ ((unused))int offset )
{
}

void TTPicturesHeader::printHeader( )
{

}
