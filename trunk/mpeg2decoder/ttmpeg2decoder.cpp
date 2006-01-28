/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttmpeg2decoder.cpp                                              */
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


#include "ttmpeg2decoder.h"	

static TFrameInfo frameInfo;

const char c_name[] = "MPEG2DECODER  : ";

// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
TTMpeg2Decoder::TTMpeg2Decoder()
{
  mpeg2Stream        = NULL;
  mpeg2FileName      = " ";
  mpeg2Decoder       = NULL;
  decoderBuffer      = NULL;
  decoderBufferSize  = 0;
  streamBuffer       = NULL;
  streamBufferSize   = 0;
  mpeg2StreamOK      = false;
  isDecoder          = false;
  streamEndReached   = false;
  currentStreamFrame = 0;
  currentFrameIndex  = 0;
  currentStreamPos   = 0;
  iSkipFrames        = 1;
  sliceData          = NULL;
  isIndexSeek        = false;
  videoIndexList     = NULL;
  videoHeaderList    = NULL;
  t_frame_info       = NULL;

  // initialize mpeg-2 decoder objekt
  initDecoder();
}


// -----------------------------------------------------------------------------
// Constructor with file name
// -----------------------------------------------------------------------------
TTMpeg2Decoder::TTMpeg2Decoder( QString cFName, 
				TTVideoIndexList* viIndex, 
				TTVideoHeaderList* viHeader,
				long lOffset, long lSize )
{
  //int iCount = 0;

  mpeg2Stream        = NULL;
  mpeg2FileName      = " ";
  mpeg2Decoder       = NULL;
  decoderBuffer      = NULL;
  decoderBufferSize  = 0;
  streamBuffer       = NULL;
  streamBufferSize   = 0;
  mpeg2StreamOK      = false;
  isDecoder          = false;
  streamEndReached   = false;
  currentStreamFrame = 0;
  currentFrameIndex  = 0;
  currentStreamPos   = 0;
  iSkipFrames        = 1;
  sliceData          = NULL;
  isIndexSeek        = false;
  videoIndexList     = viIndex;
  videoHeaderList    = viHeader;
  t_frame_info       = NULL;

  // index seek available
  if ( ttAssigned(videoIndexList) && ttAssigned(videoHeaderList) )
    isIndexSeek = true;

  // open the mpeg-2 stream
  if ( openMPEG2File( cFName, lOffset, lSize ) )
  {
    streamBufferSize = initialStreamBufferSize;
    streamBuffer = new uint8_t[streamBufferSize];
  }

  // initialize mpeg-2 decoder objekt
  initDecoder();
}


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
TTMpeg2Decoder::~TTMpeg2Decoder()
{
  mpeg2_close( mpeg2Decoder );

  if ( ttAssigned(mpeg2Stream) )
  {
    mpeg2Stream->closeFile();
    delete mpeg2Stream;
  }

  if ( ttAssigned(streamBuffer) )
     delete streamBuffer;
}

// -----------------------------------------------------------------------------
// Initialize libmpeg2 and create a decoder object
// -----------------------------------------------------------------------------
bool TTMpeg2Decoder::initDecoder()
{

  // TODO: check for subsequent calling initDecoder

  mpeg2Decoder = mpeg2_init();

  if ( mpeg2Decoder == NULL )
  {
    fprintf (stderr, "%sCould not allocate a decoder object.\n",c_name);
    isDecoder = false;
    return isDecoder;
  }

  isDecoder = true;

  // initialize the decoder buffer
  decoderBuffer = new uint8_t [initialDecoderBufferSize];
  decoderBufferSize = initialDecoderBufferSize;

  mpeg2Info = mpeg2_info(mpeg2Decoder);

  if ( !ttAssigned(mpeg2Info) )
  {
    isDecoder = false;
  }

  // read the first sequence header
  if ( isDecoder )
  {
     decodeFirstMPEG2Frame( formatRGB24 );
  }

  return isDecoder;
}



// -----------------------------------------------------------------------------
// Return current decoder object
// -----------------------------------------------------------------------------
mpeg2dec_t* TTMpeg2Decoder::decoder()
{
  return mpeg2Decoder;
}


// -----------------------------------------------------------------------------
// Open a mpeg2 file stream and init buffered file I/O
// -----------------------------------------------------------------------------
bool TTMpeg2Decoder::openMPEG2File(QString cFName, __attribute__ ((unused))long lOffset, __attribute__ ((unused))long lSize)
{
  //int iNameLen;

  // TODO: check if theire is already a file stream opened and connected to the
  //       decoder

  mpeg2FileName = cFName;

  if ( mpeg2Stream == NULL )
  {
     mpeg2Stream = new TTFileBuffer( mpeg2FileName, fm_open_read );

     if ( mpeg2Stream == NULL )
	mpeg2StreamOK = false;
     else
	mpeg2StreamOK = true;
  }
  else
     mpeg2StreamOK = mpeg2Stream->openFile( mpeg2FileName, fm_open_read );

  // initialize the stream buffer for file I/O
  if ( mpeg2StreamOK )
  {
    fileSize = mpeg2Stream->streamLength();

    if ( streamBuffer == NULL )
    {
      streamBufferSize = initialStreamBufferSize;
      streamBuffer = new uint8_t[streamBufferSize];
    }
  }

  return mpeg2StreamOK;
}


// -----------------------------------------------------------------------------
// Open a mpeg2 disk (DVD); not implemented
// -----------------------------------------------------------------------------
bool TTMpeg2Decoder::openMPEG2Disk(__attribute__ ((unused))uint8_t Disk,__attribute__ ((unused)) long lOffset,__attribute__ ((unused)) long lSize)
{
  return false;
}

// set the index list's, needed for index seeking in video stream
void TTMpeg2Decoder::setIndexFiles( TTVideoIndexList* viIndex, 
				    TTVideoHeaderList* viHeader )
{
  videoIndexList     = viIndex;
  videoHeaderList    = viHeader;

  if ( ttAssigned(videoIndexList) && ttAssigned(videoHeaderList) )
    isIndexSeek = true;
}

// -----------------------------------------------------------------------------
// Return current stream size in byte
// -----------------------------------------------------------------------------
off64_t TTMpeg2Decoder::getFileSize()
{
  return fileSize;
}


// -----------------------------------------------------------------------------
// Close mpeg2 stream and free file buffer
// -----------------------------------------------------------------------------
void TTMpeg2Decoder::closeMPEG2File()
{
  // close the mpeg2 file stream

  mpeg2Stream->closeFile();

  mpeg2StreamOK = false;
}


// -----------------------------------------------------------------------------
// Return the last decoded slice data
// -----------------------------------------------------------------------------
uint8_t* TTMpeg2Decoder::getMPEG2Frame()
{
  return sliceData;
}


// -----------------------------------------------------------------------------
// Decode a mpeg2 slice
// -----------------------------------------------------------------------------
TFrameInfo* TTMpeg2Decoder::decodeMPEG2Frame( TPixelFormat pixelFormat, int type )
{
  int iSkip  = iSkipFrames;

  convType = pixelFormat;

  t_frame_info = NULL;
  isDecodedFrame = false;

  do
  {
    state         = mpeg2_parse (mpeg2Decoder);
    mpeg2Sequence = mpeg2Info->sequence;

    switch (state)
    {
    case STATE_BUFFER:
      // stream end reached ???
      try
      {
      if (!mpeg2Stream->readArray( streamBuffer, streamBufferSize ))
      {
	// TODO: video stream contains no END code

	// LeseX returns false if the number of readed bytes lower
	// streamBufferSize
	// in this case we must ask if stream end was reached or not
	//if ( mpeg2Stream->isStreamEnd() && !streamEndReached )
	//{
	//qDebug( "error LesenX: No sequence end code found" );
	  // no SequenceEndCode; inject code into buffer to cause the decoder
	  // to decode the last frames
	  //streamBufferSize = 4;
	  //streamBuffer[0]=streamBuffer[1]=0x00;
	  //streamBuffer[2]=0x01;
	  //streamBuffer[3]=0xb7;
	//}  
      }
      } 
      catch ( TTStreamEOFException )
      {
	qDebug( "%sdecode: stream EOF (!)",c_name );
      }
      //fill mpeg2 buffer with data
      streamEndReached = false;
      mpeg2_buffer (mpeg2Decoder, streamBuffer, streamBuffer + streamBufferSize);
      break;
    case STATE_SEQUENCE:
      switch ( convType )
      {
      case formatRGB24:
	mpeg2_convert (mpeg2Decoder, mpeg2convert_rgb24, NULL);
	break;
      case formatRGB32:
	mpeg2_convert (mpeg2Decoder, mpeg2convert_rgb32, NULL);
	break;
      }
    case STATE_END:
      streamEndReached = true;
    case STATE_SLICE:
    case STATE_INVALID_END:
      if ( mpeg2Info->display_fbuf )
	iSkip--;

      isDecodedFrame = true;
      break;
    }
  } while ( iSkip > 0 );

  // reset skip frames parameter
  iSkipFrames = 1;

  if ( mpeg2Info->display_fbuf && isDecodedFrame )
  {
    t_frame_info            = &frameInfo;
    frameInfo.Y             = mpeg2Info->display_fbuf->buf[0];
    frameInfo.U             = mpeg2Info->display_fbuf->buf[1];
    frameInfo.V             = mpeg2Info->display_fbuf->buf[2];
    frameInfo.width         = mpeg2Info->sequence->width;
    frameInfo.height        = mpeg2Info->sequence->height;
    frameInfo.type          = mpeg2Info->display_picture->flags&0x03;
    frameInfo.chroma_width  = mpeg2Info->sequence->chroma_width;
    frameInfo.chroma_height = mpeg2Info->sequence->chroma_height;

    // wrong frame type decoded (!)
    if ( frameInfo.type != type && type != 0 )
      qDebug( "%sDecoded slice is %d; desired is %d",c_name,frameInfo.type,type );

    switch ( pixelFormat )
    {
    case formatRGB24:
      frameInfo.size=frameInfo.width*frameInfo.height*3;
      frameInfo.chroma_size=0;
      break;
    case formatRGB32:
      frameInfo.size=frameInfo.width*frameInfo.height*4;
      frameInfo.chroma_size=0;
      break;
    case formatYV12:
      frameInfo.size=frameInfo.width*frameInfo.height;
      frameInfo.chroma_size=frameInfo.chroma_width*frameInfo.chroma_height;

    }
  }
  else
  {
    qDebug( "%sno frame...",c_name );
    t_frame_info = NULL;
  }
  return t_frame_info;
}


// -----------------------------------------------------------------------------
// Seek to position 0 and decode the first slice
// -----------------------------------------------------------------------------
TFrameInfo* TTMpeg2Decoder::decodeFirstMPEG2Frame( TPixelFormat pixelFormat, int type )
{
  //int      iSkip  = 1;

  //qDebug( "%sdecode first frame",c_name );

  // seek to position 0
  seekMPEG2Stream ( 0 );

  currentFrameIndex = 0;

  // decoder full reset
  mpeg2_reset( mpeg2Decoder, 1 );

  decodeMPEG2Frame( pixelFormat, type );

  return t_frame_info;
}

void TTMpeg2Decoder::getCurrentFrameData( uint8_t* data )
{
  switch ( convType )
  {
  case formatRGB24:
  case formatRGB32:
    memcpy(data,t_frame_info->Y,t_frame_info->size);
    break;
  case formatYV12:
    memcpy(data,t_frame_info->Y,t_frame_info->size);
    data+=t_frame_info->size;
    memcpy(data,t_frame_info->V,t_frame_info->chroma_size);
    data+=t_frame_info->chroma_size;
    memcpy(data,t_frame_info->U,t_frame_info->chroma_size);
    break;
  }
}


// -----------------------------------------------------------------------------
// Skip a number of slices
// -----------------------------------------------------------------------------
void TTMpeg2Decoder::skipMPEG2Frames( int iFrameCount )
{
  iSkipFrames = iFrameCount+1;
}


TFrameInfo* TTMpeg2Decoder::getFrameInfo()
{
  return t_frame_info;
}

// -----------------------------------------------------------------------------
// Return information about the last sequence
// -----------------------------------------------------------------------------
void TTMpeg2Decoder::getMPEG2SequenceInfo( TSequenceInfo& sequenceInfo)
{
  if ( mpeg2Sequence )
  {
    sequenceInfo.width           = mpeg2Sequence->width;
    sequenceInfo.height          = mpeg2Sequence->height;
    sequenceInfo.frame_period    = mpeg2Sequence->frame_period;
    sequenceInfo.byte_rate       = mpeg2Sequence->byte_rate;
    sequenceInfo.vbv_buffer_size = mpeg2Sequence->vbv_buffer_size;
  }
}


// -----------------------------------------------------------------------------
// Return information about the current mpeg2 file stream and buffer
// -----------------------------------------------------------------------------
void TTMpeg2Decoder::getMPEG2StreamInfo( TStreamInfo& streamInfo )
{
  if ( mpeg2StreamOK )
  {
    currentStreamPos    = mpeg2Stream->currentOffset();
    streamInfo.size     = fileSize;
    streamInfo.position = currentStreamPos;
    streamInfo.frame    = currentStreamFrame;
    streamInfo.videoPTS = 0;
  }
}



// -----------------------------------------------------------------------------
// Move to specified frame position and frame type
// -----------------------------------------------------------------------------
long TTMpeg2Decoder::moveToFrameIndex( long iFramePos, int iFrameType )
{
  TTVideoIndex*     frame_index;
  TTPicturesHeader* current_picture;
  int           I;
  int           j;
  int           iCount;
  int           backIndex;

  //printf("GeheZuPosition: current: %ld | new: %d\n",currentFrameIndex,Position);

  // we must have index seek available
  if ( !isIndexSeek || !isDecoder )
    return currentFrameIndex;

  // the goto position is equal the current position and is not the
  // first frame (0)
  if ( iFramePos == currentFrameIndex && iFramePos > 0 )
    return currentFrameIndex;

  // goto index position must be in valid range
  if ( iFramePos >= (long)videoIndexList->count() ||
       iFramePos <  0                          )
  {
    qDebug( "%swrong position: %ld | %d",c_name,iFramePos,videoIndexList->count() );
    return currentFrameIndex;
  }

  // search a specific frame type in video index list
  // ---------------------------------------------------------------------------
  if ( iFrameType > 0 )
  {
    // search forward
    if ( iFramePos > currentFrameIndex )
    {
      j = 1;
      do
      {
	if ( iFramePos+j >= (long)videoIndexList->count() ) break;

	frame_index = videoIndexList->videoIndexAt( iFramePos+j );
	j++;
      } while ( frame_index->picture_coding_type != iFrameType );

      iFramePos += j-1;
      if ( iFramePos > (long)videoIndexList->count()-1 )
	iFramePos = videoIndexList->count()-1;
    }
    // search backward
    else
    {
      j = -1;
      do
      {
	if ( iFramePos+j < 0 ) break;

	frame_index = videoIndexList->videoIndexAt( iFramePos+j );
	j--;
      } while ( frame_index->picture_coding_type != iFrameType );
      iFramePos += j+1;
      if ( iFramePos < 0 )
	iFramePos = 0;
    }
  }

  // goto position
  // ---------------------------------------------------------------------------
  I  = iFramePos;   // goto position for I-Frame search

  frame_index = videoIndexList->videoIndexAt( I );

  // goto frame type is a I-Frame; adjust goto position for I-Frame search
  if ( frame_index->picture_coding_type == 1 && iFramePos > 1 )
  {
    I = iFramePos-1;

    frame_index = videoIndexList->videoIndexAt( I );
  }

  // search previous I-Frame in index list; go at least one GOP back
  iCount = 0;

  while( iCount < 1)
  {
    if ( I >= 0 )
    {
      frame_index = videoIndexList->videoIndexAt( I );

      if ( frame_index->picture_coding_type == 1 && iCount < 2) iCount++;
      backIndex = I;
    }
    else
      iCount = 2;

    I--;
  }

  // found I-Frame position at backIndex
  frame_index = videoIndexList->videoIndexAt( backIndex );
  current_picture = videoHeaderList->pictureHeaderAt( frame_index->header_list_index );

  // move to required stream position
  // ---------------------------------------------------------------------------
  framePosition( frame_index->display_order,iFramePos,
		 current_picture->headerOffset(), current_picture->temporal_reference );

  return currentFrameIndex;
}

// -----------------------------------------------------------------------------
// move to specified frame position
// -----------------------------------------------------------------------------
bool TTMpeg2Decoder::framePosition( long dOrderI, long posDOrder,
				    uint64_t sIAdress, int tempRefer )
{
  bool          bResult;
  //int           i;
  int           numBFrames;

  //dOrderI  : previous I-Frame position in display order
  //posDOrder: go to position in display-order
  //sIAdress : stream offset previous I-Frame
  //tempRefer: temporal reference previous I-Frame

  // must have a valid decoder objekt an enabled index seek
  if ( isDecoder && isIndexSeek )
  {
    // current frame index is equal desired frame index; do nothing
    if ( posDOrder == currentFrameIndex  )
    {
      bResult = true;
      return bResult;
    }

    // decode the next following slice; set new position an exit
    if ( posDOrder - currentFrameIndex == 1 )
    {
      currentFrameIndex = posDOrder;
      bResult           = true;

      return bResult;
    }

    // the desired frame index isn't the next following one, so we
    // must change position in decoder stream and skip some frames

    // seek to I-frame offset
    seekMPEG2Stream( sIAdress );

    // check if we have a open or a closed GOP at I-frame position
    // open GOP
    numBFrames = tempRefer;

    // skip frames;index must be greater 0
    if ( (posDOrder-dOrderI+numBFrames) > 0 )
      skipMPEG2Frames( posDOrder-dOrderI+numBFrames );

    // set current frame index
    currentFrameIndex = posDOrder;

    bResult = true;
  }
  else
    bResult = false;

  return bResult;
}

// -----------------------------------------------------------------------------
// Seek to a position given as offset in bytes from the beginning of the stream
// -----------------------------------------------------------------------------
void TTMpeg2Decoder::seekMPEG2Stream(off64_t lStreamPos)
{
  off64_t steps;
  //int     iSkip = 0;

  currentStreamPos = mpeg2Stream->currentOffset();

  try
  {
  // move forward in mpeg2 stream
  if ( lStreamPos > currentStreamPos && lStreamPos <= fileSize )
  {
    steps = lStreamPos - currentStreamPos;
    if ( steps > 0 && steps <= fileSize )
    {
      mpeg2Stream->seekForward( steps );
      currentStreamPos = lStreamPos;
      mpeg2_reset( mpeg2Decoder, 0 );
    }
  }
  // move backward in mpeg2 stream
  else if(  lStreamPos < currentStreamPos && lStreamPos >= 0 )
  {
    steps = currentStreamPos - lStreamPos;
    if ( steps > 0 && steps <= fileSize )
    {
      mpeg2Stream->seekBackward( steps );
      currentStreamPos = lStreamPos;
      mpeg2_reset( mpeg2Decoder, 0 );
    }
  }
  }
  catch ( TTStreamEOFException )
  {
    qDebug("%sdecoder seek EOF(!)",c_name);
  }
}


// -----------------------------------------------------------------------------
// Set decoders pixel format
// -----------------------------------------------------------------------------
void TTMpeg2Decoder::setMPEG2PixelFormat(__attribute__ ((unused))TPixelFormat pFormat)
{

}


// -----------------------------------------------------------------------------
// Set the RGB scale flag
// -----------------------------------------------------------------------------
void TTMpeg2Decoder::setRGBScaleFlag(__attribute__ ((unused))bool bDoScaling)
{

}


// -----------------------------------------------------------------------------
// Write current slice data to file ???
// -----------------------------------------------------------------------------
long TTMpeg2Decoder::writeDataToFile(__attribute__ ((unused))QString cFName,__attribute__ ((unused)) long lSize)
{
  return (long)0;
}


