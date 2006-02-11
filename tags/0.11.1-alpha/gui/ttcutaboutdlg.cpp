/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttcutaboutdlg.cpp                                               */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 03/23/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTCUTABOUTDLG
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

#include "ttcutaboutdlg.h"

#include <qlabel.h>
#include <qpushbutton.h>
#include <q3textview.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <q3whatsthis.h>
//Added by qt3to4:
#include <QGridLayout>

/* 
 *  Constructs a TTCutAboutDlg which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
TTCutAboutDlg::TTCutAboutDlg( QWidget* parent,  const char* name, bool modal, Qt::WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "TTCutAboutDlg" );

    resize( 520, 657 ); 
    setCaption( tr( "TTCut" ) );
    TTCutAboutDlgLayout = new QGridLayout( this ); 
    TTCutAboutDlgLayout->setSpacing( 6 );
    TTCutAboutDlgLayout->setMargin( 11 );

    laVersionInformation = new QLabel( this, "laVersionInformation" );
    laVersionInformation->setText( TTCut::versionString );
    laVersionInformation->setAlignment( int( Qt::AlignCenter ) );

    TTCutAboutDlgLayout->addWidget( laVersionInformation, 0, 0 );

    btnClose = new QPushButton( this, "btnClose" );
    btnClose->setText( tr( "&Close" ) );

    TTCutAboutDlgLayout->addWidget( btnClose, 3, 0 );

    tvDisclaimer = new Q3TextView( this, "tvDisclaimer" );
    tvDisclaimer->setEnabled( TRUE );
    QPalette pal;
    QColorGroup cg;
    cg.setColor( QColorGroup::Foreground, Qt::black );
    cg.setColor( QColorGroup::Button, QColor( 175, 172, 165) );
    cg.setColor( QColorGroup::Light, QColor( 255, 253, 247) );
    cg.setColor( QColorGroup::Midlight, QColor( 215, 212, 206) );
    cg.setColor( QColorGroup::Dark, QColor( 87, 85, 82) );
    cg.setColor( QColorGroup::Mid, QColor( 116, 114, 109) );
    cg.setColor( QColorGroup::Text, Qt::black );
    cg.setColor( QColorGroup::BrightText, Qt::white );
    cg.setColor( QColorGroup::ButtonText, Qt::black );
    cg.setColor( QColorGroup::Base, QColor( 255, 170, 127) );
    cg.setColor( QColorGroup::Background, QColor( 175, 172, 165) );
    cg.setColor( QColorGroup::Shadow, Qt::black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, Qt::white );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, Qt::black );
    cg.setColor( QColorGroup::Button, QColor( 175, 172, 165) );
    cg.setColor( QColorGroup::Light, QColor( 255, 253, 247) );
    cg.setColor( QColorGroup::Midlight, QColor( 201, 197, 189) );
    cg.setColor( QColorGroup::Dark, QColor( 87, 85, 82) );
    cg.setColor( QColorGroup::Mid, QColor( 116, 114, 109) );
    cg.setColor( QColorGroup::Text, Qt::black );
    cg.setColor( QColorGroup::BrightText, Qt::white );
    cg.setColor( QColorGroup::ButtonText, Qt::black );
    cg.setColor( QColorGroup::Base, QColor( 255, 170, 127) );
    cg.setColor( QColorGroup::Background, QColor( 175, 172, 165) );
    cg.setColor( QColorGroup::Shadow, Qt::black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, Qt::white );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 175, 172, 165) );
    cg.setColor( QColorGroup::Light, QColor( 255, 253, 247) );
    cg.setColor( QColorGroup::Midlight, QColor( 201, 197, 189) );
    cg.setColor( QColorGroup::Dark, QColor( 87, 85, 82) );
    cg.setColor( QColorGroup::Mid, QColor( 116, 114, 109) );
    cg.setColor( QColorGroup::Text, Qt::black );
    cg.setColor( QColorGroup::BrightText, Qt::white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, QColor( 255, 170, 127) );
    cg.setColor( QColorGroup::Background, QColor( 175, 172, 165) );
    cg.setColor( QColorGroup::Shadow, Qt::black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, Qt::white );
    pal.setDisabled( cg );
    tvDisclaimer->setPalette( pal );
    tvDisclaimer->setText( tr( "TTCut - The LINUX Mpeg Cutter\nCopyrigth (C) 2005 Ben Altendorf (ttcut@tritime.de)\nSee http://ttcut.tritime.org for updates.\n\nThis program is free software; you can redistribute it and/or modify it\nunder the terms of the GNU General Public License as published by the \nFree Software Foundation;  either version 2 of the License, or (at your option) \nany later version. \nThis program is distributed in the hope that it will be useful, but WITHOUT\nANY WARRANTY; without even the implied warranty of MERCHANTABILITY \nor  FITNESS FOR A PARTICULAR PURPOSE.                                          \nSee the GNU General Public License for more details.   \nYou should have received a copy of the GNU General Public License along\nwith this program; if not, write to the Free Software Foundation,\nInc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA." ) );

    TTCutAboutDlgLayout->addWidget( tvDisclaimer, 1, 0 );

    TextView2 = new Q3TextView( this, "TextView2" );
    TextView2->setEnabled( FALSE );
    cg.setColor( QColorGroup::Foreground, Qt::black );
    cg.setColor( QColorGroup::Button, QColor( 212, 208, 200) );
    cg.setColor( QColorGroup::Light, Qt::white );
    cg.setColor( QColorGroup::Midlight, QColor( 233, 231, 227) );
    cg.setColor( QColorGroup::Dark, QColor( 106, 104, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 141, 138, 133) );
    cg.setColor( QColorGroup::Text, Qt::black );
    cg.setColor( QColorGroup::BrightText, Qt::white );
    cg.setColor( QColorGroup::ButtonText, Qt::black );
    cg.setColor( QColorGroup::Base, QColor( 212, 208, 200) );
    cg.setColor( QColorGroup::Background, QColor( 212, 208, 200) );
    cg.setColor( QColorGroup::Shadow, Qt::black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 36, 106) );
    cg.setColor( QColorGroup::HighlightedText, Qt::white );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, Qt::black );
    cg.setColor( QColorGroup::Button, QColor( 212, 208, 200) );
    cg.setColor( QColorGroup::Light, Qt::white );
    cg.setColor( QColorGroup::Midlight, QColor( 243, 239, 230) );
    cg.setColor( QColorGroup::Dark, QColor( 106, 104, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 141, 138, 133) );
    cg.setColor( QColorGroup::Text, Qt::black );
    cg.setColor( QColorGroup::BrightText, Qt::white );
    cg.setColor( QColorGroup::ButtonText, Qt::black );
    cg.setColor( QColorGroup::Base, QColor( 212, 208, 200) );
    cg.setColor( QColorGroup::Background, QColor( 212, 208, 200) );
    cg.setColor( QColorGroup::Shadow, Qt::black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 36, 106) );
    cg.setColor( QColorGroup::HighlightedText, Qt::white );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 212, 208, 200) );
    cg.setColor( QColorGroup::Light, Qt::white );
    cg.setColor( QColorGroup::Midlight, QColor( 243, 239, 230) );
    cg.setColor( QColorGroup::Dark, QColor( 106, 104, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 141, 138, 133) );
    cg.setColor( QColorGroup::Text, Qt::black );
    cg.setColor( QColorGroup::BrightText, Qt::white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, QColor( 212, 208, 200) );
    cg.setColor( QColorGroup::Background, QColor( 212, 208, 200) );
    cg.setColor( QColorGroup::Shadow, Qt::black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 36, 106) );
    cg.setColor( QColorGroup::HighlightedText, Qt::white );
    pal.setDisabled( cg );
    TextView2->setPalette( pal );
    TextView2->setText( tr( "Used libraries:\nlibmpeg2 - a free MPEG-2 video stream decoder (http://libmpeg2.sourceforge.net/)\n\nThanks to:\nMartin Dienert's Mpeg2Schnitt (http://www.mdienert.de/mpeg2schnitt) for inspiration\nand for his fundamental work on analyzing mpeg-video and audio streams.\n\n\nTobias Arnold's Cuttermaran. TTCut's UI (look and feel) was taken from Cuttermaran." ) );

    TTCutAboutDlgLayout->addWidget( TextView2, 2, 0 );
    
    // signals and slot connection
    // ------------------------------------------------------------------
    connect( btnClose, SIGNAL( clicked() ),  SLOT( onDlgClose() ) );

    resize( QSize(840, 480).expandedTo(minimumSizeHint()) );
    setMaximumSize( QSize(840, 480).expandedTo(minimumSizeHint()) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
TTCutAboutDlg::~TTCutAboutDlg()
{

}

void TTCutAboutDlg::onDlgClose()
{
  done( 0 );
}
