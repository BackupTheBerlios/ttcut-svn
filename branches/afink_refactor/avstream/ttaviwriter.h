/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttaciwriter.h                                                   */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 08/06/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTAVIWRITER
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


#ifndef TTAVIWRITER_H
#define TTAVIWRITER_H


#include <QString>

#include "../gui/ttprogressbar.h"
#include "../avstream/ttavstream.h"
#include "../mpeg2decoder/ttmpeg2decoder.h"

extern "C"
{
#include "../avilib/avilib.h"
}

/* /////////////////////////////////////////////////////////////////////////////
 * AVI writer class
 */
class TTAVIWriter
{
 public:
   TTAVIWriter( TTProgressBar* b_bar = NULL );
   ~TTAVIWriter();

   bool initAVIWriter( TTVideoStream* v_stream );
   int  writeAVI( int start_frame_pos, int end_frame_pos );
   bool closeAVI();

 protected:
   bool initDecoder();
   long compareFrames();

 private:
   TTProgressBar*     progress_bar;
   TTMpeg2Decoder*    decoder;			
   QString            video_file_name;
   TTVideoHeaderList* header_list; 
   TTVideoIndexList*  index_list; 
   int                current_frame;
   qint64             file_size;
   quint64            file_offset;
   quint8*            ref_data;
   avi_t*             avi_file;
};

#endif //TTAVIWRITER_H


