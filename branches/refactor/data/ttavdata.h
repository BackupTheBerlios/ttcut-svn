/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2008                                                      */
/* FILE     : ttavdata.h                                                      */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : a.fink (E-Mail: andreas.fink85@gmail.com)       DATE: 03/11/2008 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTAVDATA
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

/******************************************************************************/
/**                         TTAVData                                          */
/** class which manages the composition of a video stream with its            */
/** audio files. Before deleting you can call deleteAll() to clear            */
/** the data.                                                                 */
/******************************************************************************/

#ifndef TTAVDATA_H
#define TTAVDATA_H

class TTVideoStream;
class TTAudioListData;
class TTAudioStream;
class TTAudioListDataItem;

class TTAVData
{
  public:
    TTAVData( TTVideoStream* pVideoStream, TTAudioListData* pAudioList );

    void                 deleteAll();
    TTVideoStream*       videoStream();
    TTAudioStream*       audioStream( int index );
    TTAudioListData*     audioList();
    int                  audioCount();
    TTAudioListDataItem& audioItem( int index );
    int                  indexOfAudioStream( TTAudioStream* pCompareStream );
    bool                 CanCutWith( TTAVData* pOther );

  private:
    TTVideoStream*   m_pVideoStream;
    TTAudioListData* m_pAudioList;
};
#endif
