/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2006                                                      */
/* FILE     : ttvideofileinfo.h                                               */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 02/23/2005 */
/* MODIFIED: b. altendorf                                    DATE: 02/19/2006 */
/* MODIFIED: b. altendorf                                    DATE: 03/21/2007 */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTVIDEOFILEINFO
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


#ifndef TTVIDEOFILEINFO_H
#define TTVIDEOFILEINFO_H

#include "ui_videofileinfowidget.h"
#include "../common/ttcut.h"
#include "../avstream/ttavtypes.h"
#include "../avstream/ttavstream.h"

class TTAVData;


class TTVideoFileInfo : public QWidget, Ui::TTVideoFileInfoWidget
{
  Q_OBJECT

  public:
    TTVideoFileInfo( QWidget* parent=0 );

    void setTitle (const QString & title);
    void enableControl(bool value);
    void clearList();
    void addVideo( TTVideoStream* pVideoStream );
    void removeVideo( int index );
    void setVideoInfo(TTVideoStream* pVideoStream);

  public slots:
    void onFileOpen();
    void onDeleteVideo();
    void onContextMenuRequest( const QPoint& point );
    void onCurItemChange( QTreeWidgetItem* pNew, QTreeWidgetItem* pOld );
    void onVideoChange( TTAVData* pAVData );

  signals:
    void fileOpened( QString fileName );
    void changeVideo( TTVideoStream* pNewVideoStream );
    void deleteVideo( int idx );

  private:
    QList<TTVideoStream*> videoListData;
};

#endif
