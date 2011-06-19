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

// $Id$

#include "scaledlg.h"

#include <tqvbox.h>
#include <tqlayout.h>
#include <tqlabel.h>
#include <tqspinbox.h>
#include <tqcheckbox.h>
#include <tqgroupbox.h>
#include <tqsize.h>

#include <kdebug.h>
#include <kcombobox.h>
#include <klocale.h>
#include <kdialog.h>
#include "kfloatspinbox.h"
#include <kglobal.h>

#define ONEINCHINMM 2.54

ScaleDlg::ScaleDlg( const TQSize & origsize, TQVBox * tqparent, const char * name )
	: TQObject( tqparent, name )
	, m_origsize( origsize )
	, m_newsizeunit( 0 )
	, m_newsizeunit2( 0 )
	, m_resolutionunit( 0 )
	, m_newwidth( origsize.width() )
	, m_newheight( origsize.height() )
	, m_resx( 72 )
	, m_resy( 72 )
{
	TQGroupBox * pixelgroup = new TQGroupBox( i18n( "Pixel Dimensions" ), tqparent );
	TQGroupBox * printgroup = new TQGroupBox( i18n( "Print Size && Display Units" ), tqparent );

	TQGridLayout * pixelgroupgrid = new TQGridLayout( pixelgroup, 1, 1,
			KDialog::marginHint(), KDialog::spacingHint() );
	TQGridLayout * printgroupgrid = new TQGridLayout( printgroup, 1, 1,
			KDialog::marginHint(), KDialog::spacingHint() );

	TQLabel * label;

	pixelgroupgrid->addRowSpacing( 0, KDialog::spacingHint() );

	label = new TQLabel( i18n( "Original width:" ), pixelgroup );
    label->tqsetAlignment( int( TQLabel::AlignVCenter | TQLabel::AlignRight ) );
	pixelgroupgrid->addWidget( label, 1, 0 );
	label = new TQLabel( i18n( "Height:" ), pixelgroup );
    label->tqsetAlignment( int( TQLabel::AlignVCenter | TQLabel::AlignRight ) );
	pixelgroupgrid->addWidget( label, 2, 0 );

	pixelgroupgrid->addRowSpacing( 3, KDialog::spacingHint() );

	label = new TQLabel( i18n( "New width:" ), pixelgroup );
    label->tqsetAlignment( int( TQLabel::AlignVCenter | TQLabel::AlignRight ) );
	pixelgroupgrid->addWidget( label, 4, 0 );
	label = new TQLabel( i18n( "Height:" ), pixelgroup );
    label->tqsetAlignment( int( TQLabel::AlignVCenter | TQLabel::AlignRight ) );
	pixelgroupgrid->addWidget( label, 5, 0 );

	pixelgroupgrid->addRowSpacing( 6, KDialog::spacingHint() );

	label = new TQLabel( i18n( "Ratio X:" ), pixelgroup );
    label->tqsetAlignment( int( TQLabel::AlignVCenter | TQLabel::AlignRight ) );
	pixelgroupgrid->addWidget( label, 7, 0 );
	label = new TQLabel( i18n( "Y:" ), pixelgroup );
    label->tqsetAlignment( int( TQLabel::AlignVCenter | TQLabel::AlignRight ) );
	pixelgroupgrid->addWidget( label, 8, 0 );

	printgroupgrid->addRowSpacing( 0, KDialog::spacingHint() );

	label = new TQLabel( i18n( "New width:" ), printgroup );
    label->tqsetAlignment( int( TQLabel::AlignVCenter | TQLabel::AlignRight ) );
	printgroupgrid->addWidget( label, 1, 0 );
	label = new TQLabel( i18n( "Height:" ), printgroup );
    label->tqsetAlignment( int( TQLabel::AlignVCenter | TQLabel::AlignRight ) );
	printgroupgrid->addWidget( label, 2, 0 );

	printgroupgrid->addRowSpacing( 3, KDialog::spacingHint() );

	label = new TQLabel( i18n( "Resolution X:" ), printgroup );
    label->tqsetAlignment( int( TQLabel::AlignVCenter | TQLabel::AlignRight ) );
	printgroupgrid->addWidget( label, 4, 0 );
	label = new TQLabel( i18n( "Y:" ), printgroup );
    label->tqsetAlignment( int( TQLabel::AlignVCenter | TQLabel::AlignRight ) );
	printgroupgrid->addWidget( label, 5, 0 );

	m_pOldWidth = new TQLabel( TQString::number( origsize.width() ), pixelgroup );
    m_pOldWidth->tqsetAlignment( int( TQLabel::AlignVCenter | TQLabel::AlignRight ) );
	pixelgroupgrid->addWidget( m_pOldWidth, 1, 1 );
	m_pOldHeight = new TQLabel( TQString::number( origsize.height() ), pixelgroup );
    m_pOldHeight->tqsetAlignment( int( TQLabel::AlignVCenter | TQLabel::AlignRight ) );
	pixelgroupgrid->addWidget( m_pOldHeight, 2, 1 );

	m_pNewWidth = new KFloatSpinBox( 1.0, 100000.0, 10.0, 0, pixelgroup );
	pixelgroupgrid->addWidget( m_pNewWidth, 4, 1 );
	m_pNewHeight = new KFloatSpinBox( 1.0, 100000.0, 10.0, 0, pixelgroup );
	pixelgroupgrid->addWidget( m_pNewHeight, 5, 1 );

	m_pNewSizeUnit = new KComboBox( pixelgroup );
	m_pNewSizeUnit->insertItem( i18n( "px" ) );
	m_pNewSizeUnit->insertItem( i18n( "%" ) );
	pixelgroupgrid->addMultiCellWidget( m_pNewSizeUnit, 4, 5, 2, 2, TQt::AlignVCenter );

	m_pRatioX = new KFloatSpinBox( 0.0001, 10000.0, 0.1, 4, pixelgroup );
	pixelgroupgrid->addWidget( m_pRatioX, 7, 1 );
	m_pRatioY = new KFloatSpinBox( 0.0001, 10000.0, 0.1, 4, pixelgroup );
	pixelgroupgrid->addWidget( m_pRatioY, 8, 1 );

	m_pLinkRatio = new TQCheckBox( i18n( "Link" ), pixelgroup );
	pixelgroupgrid->addMultiCellWidget( m_pLinkRatio, 7, 8, 2, 2, TQt::AlignVCenter );

	m_pNewWidth2 = new KFloatSpinBox( 0.0001, 10000.0, 0.1, 4, printgroup );
	printgroupgrid->addWidget( m_pNewWidth2, 1, 1 );
	m_pNewHeight2 = new KFloatSpinBox( 0.0001, 10000.0, 0.1, 4, printgroup );
	printgroupgrid->addWidget( m_pNewHeight2, 2, 1 );

	m_pNewSizeUnit2 = new KComboBox( printgroup );
	m_pNewSizeUnit2->insertItem( i18n( "in" ) );
	m_pNewSizeUnit2->insertItem( i18n( "mm" ) );
	printgroupgrid->addMultiCellWidget( m_pNewSizeUnit2, 1, 2, 2, 2, TQt::AlignVCenter );

	m_pResolutionX = new KFloatSpinBox( 0.0001, 6000.0, 1.0, 4, printgroup );
	printgroupgrid->addWidget( m_pResolutionX, 4, 1 );
	m_pResolutionY = new KFloatSpinBox( 0.0001, 6000.0, 1.0, 4, printgroup );
	printgroupgrid->addWidget( m_pResolutionY, 5, 1 );

	m_pLinkResolution = new TQCheckBox( i18n( "Link" ), printgroup );
	printgroupgrid->addMultiCellWidget( m_pLinkResolution, 4, 5, 2, 2, TQt::AlignVCenter );
	m_pResolutionUnit = new KComboBox( printgroup );
	m_pResolutionUnit->insertItem( i18n( "pixels/in" ) );
	m_pResolutionUnit->insertItem( i18n( "pixels/mm" ) );
	printgroupgrid->addMultiCellWidget( m_pResolutionUnit, 6, 6, 1, 2, TQt::AlignLeft );

	m_pNewWidth->setValue( m_origsize.width() );
	m_pNewHeight->setValue( m_origsize.height() );

	m_newsizeunit = 0;
	m_newsizeunit2 = 0;
	m_resolutionunit = 0;

	connect( m_pNewWidth, TQT_SIGNAL( valueChanged( float ) ), TQT_SLOT( slotNewWidth( float ) ) );
	connect( m_pNewHeight, TQT_SIGNAL( valueChanged( float ) ), TQT_SLOT( slotNewHeight( float ) ) );
	connect( m_pNewWidth2, TQT_SIGNAL( valueChanged( float ) ), TQT_SLOT( slotNewWidth2( float ) ) );
	connect( m_pNewHeight2, TQT_SIGNAL( valueChanged( float ) ), TQT_SLOT( slotNewHeight2( float ) ) );
	connect( m_pResolutionX, TQT_SIGNAL( valueChanged( float ) ), TQT_SLOT( slotResolutionX( float ) ) );
	connect( m_pResolutionY, TQT_SIGNAL( valueChanged( float ) ), TQT_SLOT( slotResolutionY( float ) ) );

	connect( m_pNewSizeUnit, TQT_SIGNAL( activated( int ) ), TQT_SLOT( slotChangeNewSizeUnit( int ) ) );
	connect( m_pNewSizeUnit2, TQT_SIGNAL( activated( int ) ), TQT_SLOT( slotChangeNewSizeUnit2( int ) ) );
	connect( m_pResolutionUnit, TQT_SIGNAL( activated( int ) ), TQT_SLOT( slotChangeResolutionUnit( int ) ) );
}

ScaleDlg::~ScaleDlg()
{
}

void ScaleDlg::slotNewWidth( float newval )
{
	float newwidth;
	switch( m_newsizeunit )
	{
		case 0: // Pixel
			newwidth = newval;
			break;
		case 1: // Percent
			newwidth = newval / 100.0 * m_origsize.width();
			break;
		default:
			kdError( 4630 ) << "unknown unit\n";
			break;
	}

	m_newwidth = newwidth;
	m_pNewWidth2->setValueBlocking( m_newwidth / m_resx / ( m_newsizeunit2 ? ONEINCHINMM : 1 ) );
	m_pRatioX->setValueBlocking( m_newwidth / m_origsize.width() );
	if( m_pLinkRatio->isChecked() )
	{
		m_newheight = m_newwidth / m_origsize.width() * m_origsize.height();
		m_pNewHeight->setValueBlocking( m_newheight * ( m_newsizeunit ? 100.0 / m_origsize.height() : 1 ) );
		m_pRatioY->setValueBlocking( m_newheight / m_origsize.height() );
		m_pNewHeight2->setValueBlocking( m_newheight / m_resy / ( m_newsizeunit2 ? ONEINCHINMM : 1 ) );
	}
}

void ScaleDlg::slotNewHeight( float newval )
{
}

void ScaleDlg::slotNewWidth2( float newval )
{
}

void ScaleDlg::slotNewHeight2( float newval )
{
}

void ScaleDlg::slotResolutionX( float newval )
{
}

void ScaleDlg::slotResolutionY( float newval )
{
}

void ScaleDlg::slotChangeNewSizeUnit( int index )
{
	if( m_newsizeunit == index )
		return;

	m_newsizeunit = index;

	switch( m_newsizeunit )
	{
		case 0:
			// Pixel
			m_pNewWidth->setRange( 1.0, 100000.0, 10.0, 0 );
			m_pNewHeight->setRange( 1.0, 100000.0, 10.0, 0 );
			m_pNewWidth->setValueBlocking( m_newwidth );
			m_pNewHeight->setValueBlocking( m_newheight );
			break;
		case 1:
			// Percent
			m_pNewWidth->setRange( 0.0001, 100000.0, 0.1, 4 );
			m_pNewHeight->setRange( 0.0001, 100000.0, 0.1, 4 );
			m_pNewWidth->setValueBlocking( m_newwidth * 100.0 / m_origsize.width() );
			m_pNewHeight->setValueBlocking( m_newheight * 100.0 / m_origsize.height() );
			break;
		default:
			kdError( 4630 ) << "change to unknown unit\n";
			break;
	}
}

void ScaleDlg::slotChangeNewSizeUnit2( int index )
{
	if( m_newsizeunit2 == index )
		return;

	m_newsizeunit2 = index;

	switch( m_newsizeunit2 )
	{
		case 0:
			// Inch
			m_pNewWidth2->setRange( 0.0001, 10000.0, 0.1, 4 );
			m_pNewHeight2->setRange( 0.0001, 10000.0, 0.1, 4 );
			m_pNewWidth2->setValueBlocking( m_newwidth / m_resx );
			m_pNewHeight2->setValueBlocking( m_newheight / m_resy );
			break;
		case 1:
			// Millimeter
			m_pNewWidth2->setRange( 0.0002, 25400.0, 0.1, 4 );
			m_pNewHeight2->setRange( 0.0002, 25400.0, 0.1, 4 );
			m_pNewWidth2->setValueBlocking( m_newwidth / m_resx / ONEINCHINMM );
			m_pNewHeight2->setValueBlocking( m_newheight / m_resy / ONEINCHINMM );
			break;
		default:
			kdError( 4630 ) << "change to unknown unit\n";
			break;
	}
}

void ScaleDlg::slotChangeResolutionUnit( int index )
{
	if( m_resolutionunit == index )
		return;

	m_resolutionunit = index;

	switch( m_resolutionunit )
	{
		case 0:
			// Pixels per Inch
			m_pResolutionX->setRange( 0.0002, 25400.0, 0.1, 4 );
			m_pResolutionY->setRange( 0.0002, 25400.0, 0.1, 4 );
			m_pResolutionX->setValueBlocking( m_resx );
			m_pResolutionY->setValueBlocking( m_resy );
			break;
		case 1:
			// Pixels per Millimeter
			m_pResolutionX->setRange( 0.0001, 10000.0, 0.1, 4 );
			m_pResolutionY->setRange( 0.0001, 10000.0, 0.1, 4 );
			m_pResolutionX->setValueBlocking( m_resx / ONEINCHINMM );
			m_pResolutionY->setValueBlocking( m_resy / ONEINCHINMM );
			break;
		default:
			kdError( 4630 ) << "change to unknown unit\n";
			break;
	}
}

#include "scaledlg.moc"
