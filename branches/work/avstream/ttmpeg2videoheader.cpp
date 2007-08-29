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
// TTSEQUENCEHEADER
// TTSEQUENCEENDHEADER
// TTGOPHEADER
// TTPICTURESHEADER
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

const char cName[] = "MPEGVIDEOHEADER";

/* /////////////////////////////////////////////////////////////////////////////
 * TTMpeg2VideoHeader
 * Base class for all MPEG2 video header
 */
TTMpeg2VideoHeader::TTMpeg2VideoHeader()
{
  log = TTMessageLogger::getInstance();
}

/* /////////////////////////////////////////////////////////////////////////////
 * Read header from given stream
 */
bool TTMpeg2VideoHeader::readHeader( __attribute__ ((unused))TTFileBuffer* mpeg2_stream )
{
  return false;
}

/* /////////////////////////////////////////////////////////////////////////////
 * Read header at given offset
 */
bool TTMpeg2VideoHeader::readHeader( __attribute__ ((unused))TTFileBuffer* mpeg2_stream, __attribute__ ((unused))off64_t offset )
{
  return false;
}

/* /////////////////////////////////////////////////////////////////////////////
 * Parse basic header data
 */
void TTMpeg2VideoHeader::parseBasicData( __attribute__ ((unused))uint8_t* data, __attribute__ ((unused))int offset )
{

}

/* /////////////////////////////////////////////////////////////////////////////
 * Parse extended header data
 */
void TTMpeg2VideoHeader::parseExtendedData( __attribute__ ((unused))uint8_t* data, __attribute__ ((unused))int offset )
{

}

/* /////////////////////////////////////////////////////////////////////////////
 * Print header date for debug/logfile usage
 */
void TTMpeg2VideoHeader::printHeader( )
{

}



/* /////////////////////////////////////////////////////////////////////////////
 * TTSequenceHeader: Sequence header [0x000001B3]
 * Default constructor, extends TTMpeg2VideoHeader
 */
TTSequenceHeader::TTSequenceHeader() : TTMpeg2VideoHeader()
{

}

/* /////////////////////////////////////////////////////////////////////////////
 * Read sequence header from stream
 */
bool TTSequenceHeader::readHeader( TTFileBuffer* mpeg2_stream )
{
  uint8_t  header_data[8];

  try
  {
    // read 8 byte from stream, starting at current offset
    mpeg2_stream->readArray( header_data, 8 ) ;

    // fill sequence header
    header_start_code = sequence_start_code;
    header_offset     = mpeg2_stream->currentOffset() - 12;

    parseBasicData( header_data );

    // step over intra quantiser matrix
    if ( (header_data[7] & 0x02) == 2 )
      mpeg2_stream->seekRelative( 64 );  //Seek Exception

    // step over non intra quantiser matrix
    if ( (header_data[7] & 0x01) == 1 )
      mpeg2_stream->seekRelative( 64 ); //Seek Exception

    //parseExtendedData(mpeg2_stream);

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

/* /////////////////////////////////////////////////////////////////////////////
 * Read sequence header at given offset
 */
bool TTSequenceHeader::readHeader( TTFileBuffer* mpeg2_stream, off64_t offset )
{
  mpeg2_stream->seekAbsolute( offset+4 );

  return readHeader( mpeg2_stream );
}

/* /////////////////////////////////////////////////////////////////////////////
 * Parse basic header data
 */
void TTSequenceHeader::parseBasicData( uint8_t* data, int offset )
{
  horizontal_size_value        = (data[offset+0] << 4) + ((data[1] & 0xF0) >> 4);
  vertical_size_value          = ((data[1] & 0x0F) << 8) + data[2];
  aspect_ratio_information     = (data[3] & 0xF0) >> 4;
  frame_rate_code              = (data[3] & 0x0F);
  bit_rate_value               = (int)(((data[4] << 10) + (data[5] << 2)+((data[6] & 0xC0) >> 6))*400);
  vbv_buffer_size_value        = ((data[6] & 0x1F) << 5)+((data[7] & 0xF8) >> 3);

  // werden diese Variablen wirklich benötigt???
  profile_and_level_indication = 0;
  progressive_sequence         = false;
  chroma_format                = 0;
  low_delay                    = false;
}

/* /////////////////////////////////////////////////////////////////////////////
 * Parse extended header data
 */
void TTSequenceHeader::parseExtendedData(uint8_t*, int )
{
}

void TTSequenceHeader::parseExtendedData(TTFileBuffer* mpeg2_stream)
{
}


/* /////////////////////////////////////////////////////////////////////////////
 * Print header data for debug/logfile usage
 */
void TTSequenceHeader::printHeader( )
{
  log->debugMsg(cName, "Sequence header data");
  log->debugMsg(cName, "----------------------------------------------");
  log->debugMsg(cName, "horizontal size:  %d", horizontal_size_value);
  log->debugMsg(cName, "vertical size  :  %d", vertical_size_value);
  log->debugMsg(cName, "aspect ratio   :  %d", aspect_ratio_information);
  log->debugMsg(cName, "frame rate     :  %d", frame_rate_code);
  log->debugMsg(cName, "bit rate code  :  %d", bit_rate_value);
  log->debugMsg(cName, "vbv buffer     :  %d", vbv_buffer_size_value);
}

/* /////////////////////////////////////////////////////////////////////////////
 * Returns the horizontal size value
 */
int TTSequenceHeader::horizontalSize()
{
  return horizontal_size_value;
}

/* /////////////////////////////////////////////////////////////////////////////
 * Returns the vertical size value
 */
int TTSequenceHeader::verticalSize()
{
  return vertical_size_value;
}

/* /////////////////////////////////////////////////////////////////////////////
 * Returns the aspect ration as string value
 */
QString TTSequenceHeader::aspectRatioText()
{
  QString szTemp;

  if ( aspect_ratio_information == 1 ) szTemp = "1:1";
  if ( aspect_ratio_information == 2 ) szTemp = "4:3";
  if ( aspect_ratio_information == 3 ) szTemp = "16:9";
  if ( aspect_ratio_information == 4 ) szTemp = "2.21:1";

  return szTemp;
}

/* /////////////////////////////////////////////////////////////////////////////
 * Returns the frame rate as string value
 */
QString TTSequenceHeader::frameRateText()
{
  QString szTemp;

  if ( frame_rate_code == 2 ) szTemp = "24 fps";
  if ( frame_rate_code == 3 ) szTemp = "25 fps";
  if ( frame_rate_code == 5 ) szTemp = "30 fps";

  return szTemp;
}

/* /////////////////////////////////////////////////////////////////////////////
 * Returns the frame rate value
 */
float TTSequenceHeader::frameRateValue()
{
  float value = 25.0;

  if ( frame_rate_code == 2 ) value = 24.0;
  if ( frame_rate_code == 3 ) value = 25.0;
  if ( frame_rate_code == 5 ) value = 30.0;

  if ( frame_rate_code < 2 || frame_rate_code > 5 )
  {
    log->errorMsg(cName, "Couldn't determine the correct frame rate: aussume 25 fps!");
    value = 25.0;
  }

  return value;
}

/* /////////////////////////////////////////////////////////////////////////////
 * Returns the bit rate value in Kbit
 */
float TTSequenceHeader::bitRateKbit()
{
  return (float)bit_rate_value / 1000.0;
}

/* /////////////////////////////////////////////////////////////////////////////
 * Returns the vbv buffer size value
 */
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

/* /////////////////////////////////////////////////////////////////////////////
 * Read the GOP header from given stream
 */
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

/* /////////////////////////////////////////////////////////////////////////////
 * Read the GOP header at given offset
 */
bool TTGOPHeader::readHeader( TTFileBuffer* mpeg2_stream, off64_t offset )
{
  mpeg2_stream->seekAbsolute( offset+4 );

  return readHeader( mpeg2_stream );
}

/* /////////////////////////////////////////////////////////////////////////////
 * Parse the basic GOP header data
 */
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

/* /////////////////////////////////////////////////////////////////////////////
 * Parse extended GOP header data, not basicly necessary for processing
 */
void TTGOPHeader::parseExtendedData( __attribute__ ((unused))uint8_t* data, __attribute__ ((unused))int offset )
{
}

/* /////////////////////////////////////////////////////////////////////////////
 * Print GOP header data for debug and logfile usage
 */
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

/* /////////////////////////////////////////////////////////////////////////////
 * Read picture header from stream
 */
bool TTPicturesHeader::readHeader( TTFileBuffer* mpeg2_stream )
{
  uint8_t header_data[5];
  //unused: uint8_t byte1;

  try
  {
    mpeg2_stream->readArray( header_data, 4 );

    header_start_code = picture_start_code;
    header_offset     = mpeg2_stream->currentOffset() - 8;

    parseBasicData( header_data );

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

/* /////////////////////////////////////////////////////////////////////////////
 * Read picture header at given offset.
 */
bool TTPicturesHeader::readHeader( TTFileBuffer* mpeg2_stream, off64_t offset )
{
  mpeg2_stream->seekAbsolute( offset+4 );
  return readHeader( mpeg2_stream );
}

/* /////////////////////////////////////////////////////////////////////////////
 * Parse basic picture header data.
 */
void TTPicturesHeader::parseBasicData( uint8_t* data, int offset )
{
  picture_coding_type = (int)((data[offset+1] & 0x38) >> 3);
  temporal_reference  = (int)((data[offset+0] << 2) + ((data[offset+1] & 0xC0) >> 6));
  vbv_delay           = ((data[offset+1] & 0x07) << 13) + (data[offset+2] << 5) + ((data[offset+3] & 0xF8) >> 3);
}

/* /////////////////////////////////////////////////////////////////////////////
 * Parse extended picture header data
 */
void TTPicturesHeader::parseExtendedData( __attribute__ ((unused))uint8_t* data, __attribute__ ((unused))int offset )
{
}

/* /////////////////////////////////////////////////////////////////////////////
 * Form an string representing the picture coding type.
 */
QString TTPicturesHeader::codingTypeString()
{
  switch (picture_coding_type)
  {
    case 1:
      return "I";

    case 2:
      return "P";

    case 3:
      return "B";

    default:
      return "-";
  }
}

/* /////////////////////////////////////////////////////////////////////////////
 * Print the header informations for debug purpose
 */
void TTPicturesHeader::printHeader( )
{

}
