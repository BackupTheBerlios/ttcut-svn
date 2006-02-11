/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttmpeg2decoder.h                                                */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 02/23/2005 */
/* MODIFIED: b. altendorf                                    DATE: 03/01/2005 */
/* MODIFIED: b. altendorf                                    DATE: 03/31/2005 */
/* MODIFIED: b. altendorf                                    DATE: 04/30/2005 */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTMPEG2DECODER
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

#ifndef TTMPEG2DECODER_H
#define TTMPEG2DECODER_H

//#include "../avstream/ttcommon.h"

// project header files
//#if defined(__TTCUT)
#include "../gui/ttcut.h"
//#else
//#include "ttmpeg2.h"
//#endif

// standard C header files
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef __WIN32
#include <inttypes.h>
#endif

// libmpeg2-dec header files
extern "C"
{
#include <mpeg2dec/mpeg2.h>
#include <mpeg2dec/mpeg2convert.h>
}
  
// Qt header files
#include <qstring.h>

#ifdef __WIN32
#include "../avstream/ttwfilebuffer.h"
#else
#include "../avstream/ttfilebuffer.h"
#endif

#include "../avstream/ttavheader.h"
#include "../avstream/ttvideoheaderlist.h"
#include "../avstream/ttvideoindexlist.h"
#include "../avstream/ttmpeg2videoheader.h"



// constants for mpeg2 pixel format
// -----------------------------------------------------------------------------
enum TPixelFormat 
{
  formatRGB24 = 1,  // RGB interleaved; default
  formatRGB32 = 2,  
  formatRGB8  = 3,
  formatYV12  = 4,
  formatYUV24 = 5   // YUV planes
};


// Frame info struct
// -----------------------------------------------------------------------------
typedef struct
{
  uint8_t* Y;
  uint8_t* U;
  uint8_t* V;
  int      width;
  int      height;
  int      size;
  int      type;
  int      chroma_width;
  int      chroma_height;
  int      chroma_size;
} TFrameInfo;


// Sequence properties struct
// -----------------------------------------------------------------------------
typedef struct
{
  int          width;
  int          height;
  unsigned int byte_rate;
  unsigned int vbv_buffer_size;
  unsigned int frame_period;
} TSequenceInfo;


// Stream properties struct
// -----------------------------------------------------------------------------
typedef struct
{
  off64_t size;
  off64_t position;
  int     frame;
  int     videoPTS;
} TStreamInfo;


// Initial buffer size
// -----------------------------------------------------------------------------
//const int initialStreamBufferSize  = 1048576;
//const int initialStreamBufferSize  = 524288;
const int initialStreamBufferSize  = 65536;
const int initialDecoderBufferSize = 5129;


// -----------------------------------------------------------------------------
// TTMpeg2Decoder class declaration
// -----------------------------------------------------------------------------
class TTMpeg2Decoder
{
 public:
  TTMpeg2Decoder( );
  TTMpeg2Decoder( QString cFName, TTVideoIndexList* viIndex=NULL, 
		  TTVideoHeaderList* viHeader=NULL,long lOffset=0, long lSize=-1);
  ~TTMpeg2Decoder();

  // public methods
  //bool      isDecoder();
  mpeg2dec_t* decoder();

  bool        openMPEG2File( QString cFName, long lOffset=0, long lSize=-1 );
  bool        openMPEG2Disk( uint8_t Disk, long lOffset=0, long lSize=-1 );
  void        setIndexFiles( TTVideoIndexList* viIndex, TTVideoHeaderList* viHeader );

  off64_t     getFileSize();
  void        closeMPEG2File();
  long        moveToFrameIndex( long iFramePos, int iFrameType=0 );
  uint8_t*    getMPEG2Frame();

  TFrameInfo* decodeMPEG2Frame( TPixelFormat pixelFormat=formatRGB24, int type=0 );
  TFrameInfo* decodeFirstMPEG2Frame( TPixelFormat pixelFormat=formatRGB24, int type=0 );

  void        getCurrentFrameData( uint8_t* data );

  void        skipMPEG2Frames( int iFrameCount );

  TFrameInfo* getFrameInfo();

  void        getMPEG2SequenceInfo( TSequenceInfo& sequenceInfo );
  void        getMPEG2StreamInfo( TStreamInfo& streamInfo );

  void        seekMPEG2Stream(off64_t lStreamPos);
  void        setMPEG2PixelFormat(TPixelFormat pFormat);
  void        setRGBScaleFlag(bool bDoScaling);
  long        writeDataToFile(QString cFName, long lSize);

 protected:
  bool initDecoder();
  //bool framePosition(long sOrderI, long dOrderI, long posSOrder, long posDOrder, uint64_t sIAdress);
  bool framePosition( long dOrderI, long posDOrder, uint64_t sIAdress, int tempRefer );

 private:
  mpeg2dec_t*             mpeg2Decoder;
  mpeg2_state_t           state;
  bool                    isDecoder;
  uint8_t*                decoderBuffer;
  int                     decoderBufferSize;
  TTFileBuffer*           mpeg2Stream;
  off64_t                 fileSize;
  bool                    mpeg2StreamOK;
  uint8_t*                streamBuffer;
  int                     streamBufferSize;
  bool                    streamEndReached;
  QString                 mpeg2FileName;
  const mpeg2_info_t*     mpeg2Info;
  const mpeg2_sequence_t* mpeg2Sequence;
  uint8_t*                sliceData;
  int                     iSkipFrames;
  uint64_t                iNextAdress;
  off64_t                 currentStreamPos;     // current position in mpeg2 stream
  off64_t                 currentStreamFrame;   // current frame index
  long                    currentFrameIndex;   
  bool                    isIndexSeek;          
  TTVideoHeaderList*      videoHeaderList; 
  TTVideoIndexList*       videoIndexList; 
  TPixelFormat            convType;
  TFrameInfo*             t_frame_info;

  bool                    isDecodedFrame;
};

#endif //TTMPEG2DECODER_H
 
