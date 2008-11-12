/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2008                                                      */
/* FILE     : ttavdata.cpp                                                    */
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
#include "ttavdata.h"
#include "../avstream/ttavstream.h"
#include "../avstream/ttmpeg2videostream.h"
#include "ttaudiolistdata.h"


TTAVData::TTAVData( TTVideoStream* pVideoStream, TTAudioListData* pAudioList )
  : m_pVideoStream( pVideoStream ), m_pAudioList( pAudioList )
{
}


void TTAVData::deleteAll()
{
  //FIXME: m_pVideoStream and m_pAudioList are pointer to objects outside this
  //       class!
  //       Sure you can do so, but imho this is evel. I share the position to
  //       delete objects in the same context were they are created.
  //delete m_pVideoStream;
  m_pVideoStream = NULL;
  m_pAudioList->deleteAll();

  //delete m_pAudioList;
  m_pAudioList = NULL;
}


TTVideoStream* TTAVData::videoStream()
{
  return m_pVideoStream;
}


TTAudioStream* TTAVData::audioStream( int index )
{
  if ( m_pAudioList == 0 )
    return 0;
  return m_pAudioList->audioStreamAt( index );
}


TTAudioListData* TTAVData::audioList()
{
  return m_pAudioList;
}


int TTAVData::audioCount()
{
  return m_pAudioList->count();
}


TTAudioListDataItem& TTAVData::audioItem( int index )
{
  return m_pAudioList->itemAt( index );
}


int TTAVData::indexOfAudioStream( TTAudioStream* pCompareStream )
{
  for ( int i=0; i<m_pAudioList->count(); ++i )
    if ( m_pAudioList->audioStreamAt(i) == pCompareStream )
      return i;
  return -1;   // if nothing found return -1
}


bool TTAVData::CanCutWith( TTAVData* pOther )
{
  // frame rate, horizontal and vertical size, aspect ratio must fit for both videos
  TTMpeg2VideoStream* vid1 = (TTMpeg2VideoStream*) videoStream();
  TTMpeg2VideoStream* vid2 = (TTMpeg2VideoStream*) pOther->videoStream();
  if ( vid1->frameRate() != vid2->frameRate() )
    return false;
  if ( vid1->currentSequenceHeader()->horizontalSize() != vid2->currentSequenceHeader()->horizontalSize() )
    return false;
  if ( vid1->currentSequenceHeader()->verticalSize() != vid2->currentSequenceHeader()->verticalSize() )
    return false;
  if ( vid1->currentSequenceHeader()->aspectRatioText() != vid2->currentSequenceHeader()->aspectRatioText() )
    return false;

  // audio lists must be the same, ie. same number of tracks, and tracks have to fit each other
  if ( pOther->audioCount() != audioCount() )
    return false;
  for ( int i=0; i<audioCount(); ++i ) {
    TTAudioListDataItem& aud1 = audioItem(i);
    TTAudioListDataItem& aud2 = pOther->audioItem(i);
//     if ( aud1.getVersion() != aud2.getVersion() )
//       return false;
    if ( aud1.getBitrate() != aud2.getBitrate() )
      return false;
    if ( aud1.getSamplerate() != aud2.getSamplerate() )
      return false;
    if ( aud1.getMode() != aud2.getMode() )
      return false;
  }
  return true;
}
