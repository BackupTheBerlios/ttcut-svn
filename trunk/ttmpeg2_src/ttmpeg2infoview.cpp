/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTMPEG2 2005                                                    */
/* FILE     : ttmpeg2infoview.cpp                                             */
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

#include "ttmpeg2infoview.h"

#include <q3frame.h>
#include <q3header.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <q3listview.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <q3whatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>


/* 
 *  Constructs a TTMpeg2InfoView which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
TTMpeg2InfoView::TTMpeg2InfoView( QWidget* parent,  const char* name )
    : QWidget( parent, name )
{
    if ( !name )
	setName( "TTMpeg2InfoView" );
    setEnabled( TRUE );
    resize( 592, 480 ); 
    setCaption( tr( "TTMpeg2InfoView" ) );
    setBackgroundOrigin( QWidget::WidgetOrigin );
    Form1Layout = new QGridLayout( this ); 
    Form1Layout->setSpacing( 6 );
    Form1Layout->setMargin( 11 );

    TabWidget2 = new QTabWidget( this, "TabWidget2" );

    File = new QWidget( TabWidget2, "File" );
    FileLayout = new QGridLayout( File ); 
    FileLayout->setSpacing( 6 );
    FileLayout->setMargin( 11 );

    Frame3 = new Q3Frame( File, "Frame3" );
    Frame3->setFrameShape( Q3Frame::StyledPanel );
    Frame3->setFrameShadow( Q3Frame::Sunken );

    TextLabel1 = new QLabel( Frame3, "TextLabel1" );
    TextLabel1->setGeometry( QRect( 20, 20, 130, 20 ) ); 
    TextLabel1->setText( tr( "Stream type:" ) );

    FileLayout->addWidget( Frame3, 0, 0 );
    TabWidget2->insertTab( File, tr( "File" ) );

    tab = new QWidget( TabWidget2, "tab" );
    tabLayout = new QGridLayout( tab ); 
    tabLayout->setSpacing( 6 );
    tabLayout->setMargin( 11 );

    Frame4 = new Q3Frame( tab, "Frame4" );
    Frame4->setFrameShape( Q3Frame::StyledPanel );
    Frame4->setFrameShadow( Q3Frame::Sunken );
    Frame4Layout = new QGridLayout( Frame4 ); 
    Frame4Layout->setSpacing( 6 );
    Frame4Layout->setMargin( 11 );

    Layout3 = new QHBoxLayout; 
    Layout3->setSpacing( 6 );
    Layout3->setMargin( 0 );

    Layout1 = new QVBoxLayout; 
    Layout1->setSpacing( 6 );
    Layout1->setMargin( 0 );

    TextLabel2 = new QLabel( Frame4, "TextLabel2" );
    TextLabel2->setText( tr( "Picture type:" ) );
    Layout1->addWidget( TextLabel2 );

    TextLabel3 = new QLabel( Frame4, "TextLabel3" );
    TextLabel3->setText( tr( "Position:" ) );
    Layout1->addWidget( TextLabel3 );

    TextLabel4 = new QLabel( Frame4, "TextLabel4" );
    TextLabel4->setText( tr( "mb_type:" ) );
    Layout1->addWidget( TextLabel4 );

    TextLabel5 = new QLabel( Frame4, "TextLabel5" );
    TextLabel5->setText( tr( "size (in bits)" ) );
    Layout1->addWidget( TextLabel5 );

    TextLabel6 = new QLabel( Frame4, "TextLabel6" );
    TextLabel6->setText( tr( "Motion type:" ) );
    Layout1->addWidget( TextLabel6 );
    Layout3->addLayout( Layout1 );

    Layout2 = new QVBoxLayout; 
    Layout2->setSpacing( 6 );
    Layout2->setMargin( 0 );

    LineEdit1 = new QLineEdit( Frame4, "LineEdit1" );
    Layout2->addWidget( LineEdit1 );

    LineEdit2 = new QLineEdit( Frame4, "LineEdit2" );
    Layout2->addWidget( LineEdit2 );

    LineEdit3 = new QLineEdit( Frame4, "LineEdit3" );
    Layout2->addWidget( LineEdit3 );

    LineEdit4 = new QLineEdit( Frame4, "LineEdit4" );
    Layout2->addWidget( LineEdit4 );

    LineEdit5 = new QLineEdit( Frame4, "LineEdit5" );
    Layout2->addWidget( LineEdit5 );
    Layout3->addLayout( Layout2 );

    Frame4Layout->addLayout( Layout3, 0, 0 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Frame4Layout->addItem( spacer, 1, 0 );

    tabLayout->addWidget( Frame4, 0, 0 );
    TabWidget2->insertTab( tab, tr( "MB" ) );

    tab_2 = new QWidget( TabWidget2, "tab_2" );
    tabLayout_2 = new QGridLayout( tab_2 ); 
    tabLayout_2->setSpacing( 6 );
    tabLayout_2->setMargin( 11 );

    ListView1 = new Q3ListView( tab_2, "ListView1" );
    ListView1->addColumn( tr( "element" ) );
    ListView1->addColumn( tr( "value" ) );
    Q3ListViewItem * item_2 = new Q3ListViewItem( ListView1, 0 );
    Q3ListViewItem * item = new Q3ListViewItem( item_2, 0 );
    item->setText( 0, tr( "sequence_header_code" ) );
    item = new Q3ListViewItem( item_2, item );
    item->setText( 0, tr( "horizontal_size_value" ) );
    item = new Q3ListViewItem( item_2, item );
    item->setText( 0, tr( "vertical_size_value" ) );
    item = new Q3ListViewItem( item_2, item );
    item->setText( 0, tr( "aspect_ratio_information" ) );
    item = new Q3ListViewItem( item_2, item );
    item->setText( 0, tr( "frame_rate_code" ) );
    item = new Q3ListViewItem( item_2, item );
    item->setText( 0, tr( "bit_rate_value" ) );
    item = new Q3ListViewItem( item_2, item );
    item->setText( 0, tr( "marker_bit" ) );
    item = new Q3ListViewItem( item_2, item );
    item->setText( 0, tr( "vbv_buffer_size_value" ) );
    item = new Q3ListViewItem( item_2, item );
    item->setText( 0, tr( "constrained_parameters_flag" ) );
    item = new Q3ListViewItem( item_2, item );
    item->setText( 0, tr( "load_intra_quantiser_matrix" ) );
    item = new Q3ListViewItem( item_2, item );
    item->setText( 0, tr( "if(load_intra_quantiser_matrix)" ) );
    item = new Q3ListViewItem( item_2, item );
    item->setText( 0, tr( "load_non_intra_quantiser_matrix" ) );
    item = new Q3ListViewItem( item_2, item );
    item->setText( 0, tr( "if(load_non_intra_quantiser_matrix)" ) );
    item_2->setText( 0, tr( "sequence_header()" ) );

    Q3ListViewItem * item_3 = new Q3ListViewItem( ListView1, item_2 );
    item = new Q3ListViewItem( item_3, item_2 );
    item->setText( 0, tr( "extension_start_code" ) );
    item = new Q3ListViewItem( item_3, item );
    item->setText( 0, tr( "extension_start_code_indentifier" ) );
    item = new Q3ListViewItem( item_3, item );
    item->setText( 0, tr( "profile_and_level_indication" ) );
    item = new Q3ListViewItem( item_3, item );
    item->setText( 0, tr( "progressive_sequence" ) );
    item = new Q3ListViewItem( item_3, item );
    item->setText( 0, tr( "chroma_format" ) );
    item = new Q3ListViewItem( item_3, item );
    item->setText( 0, tr( "horizontal_size_extension" ) );
    item = new Q3ListViewItem( item_3, item );
    item->setText( 0, tr( "vertical_size_extension" ) );
    item = new Q3ListViewItem( item_3, item );
    item->setText( 0, tr( "bit_rate_extension" ) );
    item = new Q3ListViewItem( item_3, item );
    item->setText( 0, tr( "marker_bit" ) );
    item = new Q3ListViewItem( item_3, item );
    item->setText( 0, tr( "vbv_buffer_size_extension" ) );
    item = new Q3ListViewItem( item_3, item );
    item->setText( 0, tr( "low_delay" ) );
    item = new Q3ListViewItem( item_3, item );
    item->setText( 0, tr( "frame_rate_extension_n" ) );
    item = new Q3ListViewItem( item_3, item );
    item->setText( 0, tr( "frame_rate_extension_d" ) );
    item_3->setText( 0, tr( "sequence_extension()" ) );

    Q3ListViewItem * item_4 = new Q3ListViewItem( ListView1, item_3 );
    item = new Q3ListViewItem( item_4, item_3 );
    item->setText( 0, tr( "group_start_code" ) );
    item = new Q3ListViewItem( item_4, item );
    item->setText( 0, tr( "time_code" ) );
    item = new Q3ListViewItem( item_4, item );
    item->setText( 0, tr( "closed_gop" ) );
    item = new Q3ListViewItem( item_4, item );
    item->setText( 0, tr( "broken_link" ) );
    item_4->setText( 0, tr( "group_of_pictures_header()" ) );

    Q3ListViewItem * item_5 = new Q3ListViewItem( ListView1, item_4 );
    item = new Q3ListViewItem( item_5, item_4 );
    item->setText( 0, tr( "picture_start_code" ) );
    item = new Q3ListViewItem( item_5, item );
    item->setText( 0, tr( "temporal_reference" ) );
    item = new Q3ListViewItem( item_5, item );
    item->setText( 0, tr( "picture_coding_type" ) );
    item = new Q3ListViewItem( item_5, item );
    item->setText( 0, tr( "vbv_delay" ) );
    Q3ListViewItem * item_6 = new Q3ListViewItem( item_5, item );
    item = new Q3ListViewItem( item_6, item );
    item->setText( 0, tr( "full_pel_forward_vector" ) );
    item = new Q3ListViewItem( item_6, item );
    item->setText( 0, tr( "forward_f_code" ) );
    item_6->setText( 0, tr( "if(picture_coding_type == 2 || picture_coding_type == 3 )" ) );
    Q3ListViewItem * item_7 = new Q3ListViewItem( item_5, item_6 );
    item = new Q3ListViewItem( item_7, item_6 );
    item->setText( 0, tr( "full_pel_backward_vector" ) );
    item = new Q3ListViewItem( item_7, item );
    item->setText( 0, tr( "backward_f_code" ) );
    item_7->setText( 0, tr( "if(picture_coding_type == 3)" ) );
    item_5->setText( 0, tr( "picture_header()" ) );

    Q3ListViewItem * item_8 = new Q3ListViewItem( ListView1, item_5 );
    item = new Q3ListViewItem( item_8, item_5 );
    item->setText( 0, tr( "extension_start_code" ) );
    item = new Q3ListViewItem( item_8, item );
    item->setText( 0, tr( "extension_start_code_identifier" ) );
    item = new Q3ListViewItem( item_8, item );
    item->setText( 0, tr( "f_code[0][0]" ) );
    item = new Q3ListViewItem( item_8, item );
    item->setText( 0, tr( "f_code[0][1]" ) );
    item = new Q3ListViewItem( item_8, item );
    item->setText( 0, tr( "f_code[1][0]" ) );
    item = new Q3ListViewItem( item_8, item );
    item->setText( 0, tr( "f_code[1][1]" ) );
    item = new Q3ListViewItem( item_8, item );
    item->setText( 0, tr( "intra_dc_precision" ) );
    item = new Q3ListViewItem( item_8, item );
    item->setText( 0, tr( "picture_structure" ) );
    item = new Q3ListViewItem( item_8, item );
    item->setText( 0, tr( "top_field_first" ) );
    item = new Q3ListViewItem( item_8, item );
    item->setText( 0, tr( "frame_pred_frame_dct" ) );
    item = new Q3ListViewItem( item_8, item );
    item->setText( 0, tr( "concealment_motion_vectors" ) );
    item = new Q3ListViewItem( item_8, item );
    item->setText( 0, tr( "q_scale_type" ) );
    item = new Q3ListViewItem( item_8, item );
    item->setText( 0, tr( "intra_vlc_format" ) );
    item = new Q3ListViewItem( item_8, item );
    item->setText( 0, tr( "alternate_scan" ) );
    item = new Q3ListViewItem( item_8, item );
    item->setText( 0, tr( "repeat_first_field" ) );
    item = new Q3ListViewItem( item_8, item );
    item->setText( 0, tr( "chroma_420_type" ) );
    item = new Q3ListViewItem( item_8, item );
    item->setText( 0, tr( "progressive_frame" ) );
    item = new Q3ListViewItem( item_8, item );
    item->setText( 0, tr( "composite_display_flag" ) );
    Q3ListViewItem * item_9 = new Q3ListViewItem( item_8, item );
    item = new Q3ListViewItem( item_9, item );
    item->setText( 0, tr( "v_axis" ) );
    item = new Q3ListViewItem( item_9, item );
    item->setText( 0, tr( "field_sequence" ) );
    item = new Q3ListViewItem( item_9, item );
    item->setText( 0, tr( "sub_carrier" ) );
    item = new Q3ListViewItem( item_9, item );
    item->setText( 0, tr( "burst_amplitude" ) );
    item = new Q3ListViewItem( item_9, item );
    item->setText( 0, tr( "sub_carrier_phase" ) );
    item_9->setText( 0, tr( "if(composite_display_flag)" ) );
    item_8->setText( 0, tr( "picture_coding_extension()" ) );

    ListView1->setRootIsDecorated( TRUE );

    tabLayout_2->addWidget( ListView1, 0, 0 );
    TabWidget2->insertTab( tab_2, tr( "Headers" ) );

    Form1Layout->addWidget( TabWidget2, 0, 0 );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
TTMpeg2InfoView::~TTMpeg2InfoView()
{
    // no need to delete child widgets, Qt does it all for us
}


void TTMpeg2InfoView::setSequenceInfo( TTSequenceHeader* header )
{

}

void TTMpeg2InfoView::setSequenceEndInfo( TTSequenceEndHeader* header )
{

}

void TTMpeg2InfoView::setGOPInfo( TTGOPHeader* header )
{

}

void TTMpeg2InfoView::setPicturesInfo( TTPicturesHeader* header )
{

}

