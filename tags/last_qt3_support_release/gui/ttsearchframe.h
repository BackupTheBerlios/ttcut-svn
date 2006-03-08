/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttframesearch.h                                                 */
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


#ifndef TTFRAMESEARCH_H
#define TTFRAMESEARCH_H


#include <qstring.h>

#include "ttcut.h"
#include "ttprogressbar.h"


#include "../avstream/ttavstream.h"
#include "../mpeg2decoder/ttmpeg2decoder.h"


class TTFrameSearch
{
 public:
   TTFrameSearch( TTProgressBar* b_bar = NULL );
   ~TTFrameSearch();

   bool initFrameSearch( TTVideoStream* v_stream );
   uint searchFrame( uint ref_frame_pos, uint start_frame_pos );

 protected:
   bool initDecoder();
   long compareFrames();

 private:
   TTProgressBar*     progress_bar;
   TTMpeg2Decoder*    decoder;			
   QString            video_file_name;
   TTVideoHeaderList* header_list; 
   TTVideoIndexList*  index_list; 
   uint               current_frame;
   off64_t            file_size;
   off64_t            file_offset;
   uint8_t*           ref_data;
   uint8_t*           com_data;
};

#endif //TTFRAMESEARCH_H


