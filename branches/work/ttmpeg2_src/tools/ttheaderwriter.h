/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTMPEG2 2007                                                    */
/* FILE     : ttheaderwriter.h                                                */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 03/28/2007 */
/* MODIFIED:                                                 DATE:            */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTHEADERWRITER
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

#ifndef TTHEADERWRITER_H
#define TTHEADERWRITER_H

#include "../../common/ttcut.h"
#include "../../avstream/ttavtypes.h"
#include "../../avstream/ttmpeg2videostream.h"
#include "../../avstream/ttvideoheaderlist.h"
#include "../../avstream/ttvideoindexlist.h"

class TTHeaderWriter
{
  public:
    TTHeaderWriter(TTVideoIndexList* indexList, TTVideoHeaderList* headerList);

    void writeHeaderListToFile(QString fileName);

  protected:
    QString writeHeaderInfo(TTMpeg2VideoHeader* header);
    QString sequenceInfo(TTSequenceHeader* sequence);
    QString pictureInfo(TTPicturesHeader*picture);
    QString gopInfo(TTGOPHeader* gop);

  protected:
    TTVideoIndexList*  indexList;
    TTVideoHeaderList* headerList;
    int                frameCount;
    int                headerCount;
};


#endif //TTHEADERWRITER_H
