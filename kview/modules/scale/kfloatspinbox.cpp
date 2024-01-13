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

#include "kfloatspinbox.h"

#if defined(TQT_ACCESSIBILITY_SUPPORT)
#include <tqaccessible.h>
#endif

#include <knumvalidator.h>
#include <tdelocale.h>
#include <tdeglobal.h>
#include <kdebug.h>

int pow( int a, int b )
{
	int ret = 1;
	for( int i = 0; i < b; ++i )
		ret *= a;
	return ret;
}

KFloatSpinBox::KFloatSpinBox( float minValue, float maxValue, float step, unsigned int precision, TQWidget * parent, const char * name )
	: TQSpinBox( parent, name )
	, m_doselection( true )
{
	setRange( minValue, maxValue, step, precision );
	connect( this, TQ_SIGNAL( valueChanged( int ) ), this, TQ_SLOT( slotValueChanged( int ) ) );
}

KFloatSpinBox::~KFloatSpinBox()
{
}

void KFloatSpinBox::setRange( float minValue, float maxValue, float step, unsigned int precision )
{
	m_factor = pow( 10, precision );
	m_min = (int)( minValue * m_factor );
	m_max = (int)( maxValue * m_factor );
	m_step = (int)( step * m_factor );
	TQSpinBox::setRange( m_min, m_max );
	setSteps( m_step, m_step * 10 );
	if( precision == 0 )
		setValidator( new KIntValidator( m_min, m_max, this, 10, "KFloatValidator::KIntValidator" ) );
	else
		setValidator( new KFloatValidator( minValue, maxValue, true, this, "KFloatSpinBox::KFloatValidator" ) );
}

float KFloatSpinBox::value() const
{
	float ret = (float)TQSpinBox::value() / m_factor;
	kdDebug( 4630 ) << ret << endl;
	return ret;
}

void KFloatSpinBox::setValue( float value )
{
	TQSpinBox::setValue( (int)( value * m_factor ) );
}

void KFloatSpinBox::setValueBlocking( float value )
{
	m_doselection = false;
	blockSignals( true );
	KFloatSpinBox::setValue( value );
	blockSignals( false );
	m_doselection = true;
}

TQString KFloatSpinBox::mapValueToText( int value )
{
	return TDEGlobal::locale()->formatNumber( (float)value / (float)m_factor, 4 );
}

int KFloatSpinBox::mapTextToValue( bool * ok )
{
	return (int)( m_factor * TDEGlobal::locale()->readNumber( text(), ok ) );
}

void KFloatSpinBox::valueChange()
{
	if( m_doselection )
		TQSpinBox::valueChange();
	else
	{
		updateDisplay();
		emit valueChanged( value() );
#if defined(TQT_ACCESSIBILITY_SUPPORT)
		TQAccessible::updateAccessibility( this, 0, TQAccessible::ValueChanged );
#endif
	}
}

void KFloatSpinBox::slotValueChanged( int )
{
	emit valueChanged( value() );
}

#include "kfloatspinbox.moc"
