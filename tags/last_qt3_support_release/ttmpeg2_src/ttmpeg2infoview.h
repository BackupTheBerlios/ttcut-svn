/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTMPEG2 2005                                                    */
/* FILE     : ttmpeg2infoview.h                                               */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 06/05/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTMPEG2INFOVIEW
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

#ifndef TTMPEG2INFOVIEW_H
#define TTMPEG2INFOVIEW

#include <qvariant.h>
#include <qwidget.h>
//Added by qt3to4:
#include <QLabel>
#include <QVBoxLayout>
#include <Q3Frame>
#include <QHBoxLayout>
#include <QGridLayout>

#include "../avstream/ttmpeg2videoheader.h"

class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class Q3Frame;
class QLabel;
class QLineEdit;
class Q3ListView;
class Q3ListViewItem;
class QTabWidget;

class TTMpeg2InfoView : public QWidget
{ 
    Q_OBJECT

public:
    TTMpeg2InfoView( QWidget* parent = 0, const char* name = 0);
    ~TTMpeg2InfoView();

    void setSequenceInfo( TTSequenceHeader* header );
    void setSequenceEndInfo( TTSequenceEndHeader* header );
    void setGOPInfo( TTGOPHeader* header );
    void setPicturesInfo( TTPicturesHeader* header );

    QTabWidget* TabWidget2;
    QWidget* File;
    Q3Frame* Frame3;
    QLabel* TextLabel1;
    QWidget* tab;
    Q3Frame* Frame4;
    QLabel* TextLabel2;
    QLabel* TextLabel3;
    QLabel* TextLabel4;
    QLabel* TextLabel5;
    QLabel* TextLabel6;
    QLineEdit* LineEdit1;
    QLineEdit* LineEdit2;
    QLineEdit* LineEdit3;
    QLineEdit* LineEdit4;
    QLineEdit* LineEdit5;
    QWidget* tab_2;
    Q3ListView* ListView1;

protected:
    QGridLayout* Form1Layout;
    QGridLayout* FileLayout;
    QGridLayout* tabLayout;
    QGridLayout* Frame4Layout;
    QHBoxLayout* Layout3;
    QVBoxLayout* Layout1;
    QVBoxLayout* Layout2;
    QGridLayout* tabLayout_2;
};

#endif // TTMPEG2INFOVIEW_H
