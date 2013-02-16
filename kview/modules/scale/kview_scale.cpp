/*  This file is part of the KDE project
    Copyright (C) 2002 Matthias Kretz <kretz@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

*/

/* $Id$ */

#include "kview_scale.h"
#include "scaledlg.h"

#include <tqimage.h>
#include <tqvbox.h>

#include <tdeaction.h>
#include <tdelocale.h>
#include <kgenericfactory.h>
#include <kdebug.h>
#include <kimageviewer/viewer.h>
#include <kimageviewer/canvas.h>
#include <kdialogbase.h>

typedef KGenericFactory<KViewScale> KViewScaleFactory;
K_EXPORT_COMPONENT_FACTORY( kview_scale, KViewScaleFactory( "kview_scale" ) )

KViewScale::KViewScale( TQObject* parent, const char* name, const TQStringList & )
	: Plugin( parent, name )
	, m_pViewer( 0 )
	, m_pCanvas( 0 )
{
	m_pViewer = static_cast<KImageViewer::Viewer *>( parent );
	if( m_pViewer )
	{
		kdDebug( 4630 ) << "m_pViewer->canvas() = " << m_pViewer->canvas() << endl;
		m_pCanvas = m_pViewer->canvas();

		(void) new TDEAction( i18n( "&Scale Image..." ), 0, 0,
							this, TQT_SLOT( slotScaleDlg() ),
							actionCollection(), "plugin_scale" );
	}
	else
		kdWarning( 4630 ) << "no KImageViewer interface found - the scale plugin won't work" << endl;
}

KViewScale::~KViewScale()
{
}

void KViewScale::slotScaleDlg()
{
	kdDebug( 4630 ) << k_funcinfo << endl;
	KDialogBase dlg( m_pViewer->widget(), "KView scale dialog", true, i18n( "Scale Image" ), KDialogBase::Ok|KDialogBase::Cancel );
	ScaleDlg widget( m_pCanvas->imageSize(), dlg.makeVBoxMainWidget() );
#if 0
	TQVBox * layout = dlg.makeVBoxMainWidget();

	TQGroupBox * pixelgroup = new TQGroupBox( i18n( "Pixel Dimensions" ), layout );
	TQGridLayout * pixelgroupgrid = new TQGridLayout( pixelgroup, 1, 1, 0, KDialog::spacingHint() );
	pixelgroupgrid->setSpacing( KDialog::spacingHint() );
	pixelgroupgrid->setMargin( KDialog::marginHint() );
	TQLabel * label;
	TQSize imagesize = m_pCanvas->imageSize();

	// show original width
	label = new TQLabel( i18n( "Original width:" ), pixelgroup );
	label->setAlignment( TQLabel::AlignRight );
	pixelgroupgrid->addWidget( label, 0, 0 );
	pixelgroupgrid->addWidget( new TQLabel( TQString::number( imagesize.width() ), pixelgroup ), 0, 1 );
	label = new TQLabel( i18n( "Height:" ), pixelgroup );
	label->setAlignment( TQLabel::AlignRight );
	pixelgroupgrid->addWidget( label, 1, 0 );
	pixelgroupgrid->addWidget( new TQLabel( TQString::number( imagesize.height() ), pixelgroup ), 1, 1 );

	pixelgroupgrid->addRowSpacing( 2, KDialog::spacingHint() );

	label = new TQLabel( i18n( "New width:" ), pixelgroup );
	label->setAlignment( TQLabel::AlignRight );
	pixelgroupgrid->addWidget( label, 3, 0 );
	label = new TQLabel( i18n( "Height:" ), pixelgroup );
	label->setAlignment( TQLabel::AlignRight );
	pixelgroupgrid->addWidget( label, 4, 0 );
	TQSpinBox * newwidth = new TQSpinBox( 1, 100000, 1, pixelgroup );
	pixelgroupgrid->addWidget( newwidth, 3, 1 );
	TQSpinBox * newheight = new TQSpinBox( 1, 100000, 1, pixelgroup );
	pixelgroupgrid->addWidget( newheight, 4, 1 );
	KComboBox * newsizeunit = new KComboBox( pixelgroup );
	newsizeunit->insertItem( i18n( "px" ) );
	newsizeunit->insertItem( i18n( "%" ) );
	pixelgroupgrid->addMultiCellWidget( newsizeunit, 3, 4, 2, 2, TQt::AlignVCenter );

	pixelgroupgrid->addRowSpacing( 5, KDialog::spacingHint() );

	label = new TQLabel( i18n( "Ratio X:" ), pixelgroup );
	label->setAlignment( TQLabel::AlignRight );
	pixelgroupgrid->addWidget( label, 6, 0 );
	label = new TQLabel( i18n( "Y:" ), pixelgroup );
	label->setAlignment( TQLabel::AlignRight );
	pixelgroupgrid->addWidget( label, 7, 0 );
	TQSpinBox * ratiox = new TQSpinBox( pixelgroup );
	ratiox->setValidator( new TQDoubleValidator( 0.0001, 10000, 4, ratiox ) );
	pixelgroupgrid->addWidget( ratiox, 6, 1 );
	TQSpinBox * ratioy = new TQSpinBox( pixelgroup );
	ratioy->setValidator( new TQDoubleValidator( 0.0001, 10000, 4, ratioy ) );
	pixelgroupgrid->addWidget( ratioy, 7, 1 );
	pixelgroupgrid->addMultiCellWidget( new TQCheckBox( i18n( "Link" ), pixelgroup ), 6, 7, 2, 2, TQt::AlignVCenter );

	TQGroupBox * printgroup = new TQGroupBox( i18n( "Print Size && Display Units" ), layout );
	TQGridLayout * printgroupgrid = new TQGridLayout( printgroup, 1, 1, 0, KDialog::spacingHint() );
	printgroupgrid->setSpacing( KDialog::spacingHint() );
	printgroupgrid->setMargin( KDialog::marginHint() );

	label = new TQLabel( i18n( "New width:" ), printgroup );
	label->setAlignment( TQLabel::AlignRight );
	printgroupgrid->addWidget( label, 0, 0 );
	label = new TQLabel( i18n( "Height:" ), printgroup );
	label->setAlignment( TQLabel::AlignRight );
	printgroupgrid->addWidget( label, 1, 0 );
	TQSpinBox * newwidth2 = new TQSpinBox( printgroup );
	printgroupgrid->addWidget( newwidth2, 0, 1 );
	TQSpinBox * newheight2 = new TQSpinBox( printgroup );
	printgroupgrid->addWidget( newheight2, 1, 1 );
	KComboBox * newsizeunit2 = new KComboBox( printgroup );
	newsizeunit2->insertItem( i18n( "in" ) );
	newsizeunit2->insertItem( i18n( "mm" ) );
	printgroupgrid->addMultiCellWidget( newsizeunit2, 0, 1, 2, 2, TQt::AlignVCenter );

	printgroupgrid->addRowSpacing( 2, KDialog::spacingHint() );

	label = new TQLabel( i18n( "Resolution X:" ), printgroup );
	label->setAlignment( TQLabel::AlignRight );
	printgroupgrid->addWidget( label, 3, 0 );
	label = new TQLabel( i18n( "Y:" ), printgroup );
	label->setAlignment( TQLabel::AlignRight );
	printgroupgrid->addWidget( label, 4, 0 );
	TQSpinBox * resx = new TQSpinBox( printgroup );
	printgroupgrid->addWidget( resx, 3, 1 );
	TQSpinBox * resy = new TQSpinBox( printgroup );
	printgroupgrid->addWidget( resy, 4, 1 );
	printgroupgrid->addMultiCellWidget( new TQCheckBox( i18n( "Link" ), printgroup ), 3, 4, 2, 2, TQt::AlignVCenter );
	//KComboBox * newsizeunit2 = new KComboBox( printgroup );
	//newsizeunit2->insertItem( i18n( "in" ) );
	//newsizeunit2->insertItem( i18n( "mm" ) );
	//printgroupgrid->addMultiCellWidget( newsizeunit2, 0, 1, 2, 2, TQt::AlignVCenter );
#endif

	dlg.exec();
}

void KViewScale::slotScale()
{
	// retrieve current image
	kdDebug( 4630 ) << "m_pCanvas = " <<  m_pCanvas << endl;
	const TQImage * image = m_pCanvas->image();
	kdDebug( 4630 ) << "image pointer retrieved" << endl;
	if( image )
	{
		// scale
		TQImage newimage = image->smoothScale( 50, 50 );
		// put back (modified)
		m_pCanvas->setImage( newimage );
		m_pViewer->setModified( true );
	}
	else
		kdDebug( 4630 ) << "no image to scale" << endl;
}

// vim:sw=4:ts=4:cindent
#include "kview_scale.moc"
