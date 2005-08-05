/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttframesearch.cpp                                               */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 03/16/2005 */
/* MODIFIED: b. altendorf                                    DATE: 06/22/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTFRAMESEARCH
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


#include "ttsearchframe.h"


// -----------------------------------------------------------------------------
// Construct a TTFrameSearch object
// -----------------------------------------------------------------------------
TTFrameSearch::TTFrameSearch( TTProgressBar* pBar )
{
   decoder      = NULL;
   header_list  = NULL;
   index_list   = NULL;
   ref_data     = NULL;
   com_data     = NULL;

   progress_bar = pBar;

   file_size    = 0;
   file_offset  = 0;
}


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
TTFrameSearch::~TTFrameSearch()
{
   if ( decoder )
      delete decoder;
}


// -----------------------------------------------------------------------------
// Initialize the frame search
// -----------------------------------------------------------------------------
bool TTFrameSearch::initFrameSearch( TTVideoStream* v_stream )
{
   bool b_result;
 
   b_result = false;

   video_file_name = v_stream->filePath();
   index_list      = v_stream->indexList();
   header_list     = v_stream->headerList();
   file_size       = v_stream->streamLengthByte();

   //create the decoder object
   decoder = new TTMpeg2Decoder( video_file_name.ascii(), index_list, header_list );

   // must set the format for conversion here
   decoder->decodeFirstMPEG2Frame( formatYV12 );

   b_result = true;

   return b_result;
}


// -----------------------------------------------------------------------------
// Do the frame search
// -----------------------------------------------------------------------------
uint TTFrameSearch::searchFrame( uint ref_frame_pos, uint start_frame_pos )
{
   long        foundPosition;
   long        i,j,iLength;
   QTime       searchTime;
   bool        frameFound;
   TFrameInfo* frameInfo;
   ulong       delta;
   ulong       min_delta;
   long        resultPosition;


   foundPosition = -1;

   //qDebug( "search frame: ref: %ld | start: %ld", ref_frame_pos, start_frame_pos );

   // Progressbar action text
   if ( ttAssigned(progress_bar) )
      progress_bar->setActionText( "Search equal frame..." );

   // move decode position to "ref_frame_pos"
   current_frame = decoder->moveToFrameIndex( ref_frame_pos );

   // decode the current slice
   frameInfo = decoder->decodeMPEG2Frame( formatYV12 );

   ref_data = new uint8_t [frameInfo->size+2*frameInfo->chroma_size];
   com_data = new uint8_t [frameInfo->size+2*frameInfo->chroma_size];

   // get the reference slice data
   decoder->getCurrentFrameData( ref_data );


   if ( ttAssigned(frameInfo) )
   {
      // search intervall in seconds; see settings
      searchTime.setHMS(0,0,0);
      searchTime = searchTime.addSecs( TTCut::searchLength );

      // TODO: give framerate as variable
      iLength = ttTimeToFrames( searchTime, TTCut::frameRate );

      //qDebug( "search intervall: %ld",iLength );

      // setup progress intervall
      if ( ttAssigned(progress_bar) )
	progress_bar->setTotalSteps( iLength, 10 );

      // compare
      frameFound = false;
      i          = 0;

      // move decode position to "start_frame_pos"
      current_frame = decoder->moveToFrameIndex( start_frame_pos );
      min_delta     = (ulong)4000000000UL;

      do
      {
	 // decode the next frame afer reference position
	 frameInfo = decoder->decodeMPEG2Frame( formatYV12 );
	 decoder->getCurrentFrameData( com_data );

	 // slice decoded ??
	 if ( ttAssigned(frameInfo) )
	 {
	   delta     = 0;

	   for ( j = 0; j < frameInfo->size+2*frameInfo->chroma_size; j++ )
	     delta += (ulong)((com_data[j]-ref_data[j])*(com_data[j]-ref_data[j]));

	   if ( delta < min_delta )
	   {
	     min_delta      = delta;
	     resultPosition = i;
	   }

	   if (delta == 0) // Bild identisch, dann abbrechen
	   {
	     resultPosition = i;
	     frameFound     = true;
	     break;
	   }
	 }
	 i++;

	 if ( ttAssigned(progress_bar) )
	    progress_bar->setProgress( i );

      } while( !frameFound && i < iLength );

      foundPosition = start_frame_pos+resultPosition;

      if ( ttAssigned(progress_bar) )
	progress_bar->setComplete( );

   }
   // return the position, where the equal frame was found

   delete ref_data;
   delete com_data;

   //qDebug( "found slice at: %ld",foundPosition );

   return foundPosition;
}




