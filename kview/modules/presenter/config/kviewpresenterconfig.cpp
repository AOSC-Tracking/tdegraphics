/*  This file is part of the KDE project
    Copyright (C) 2002-2003 Matthias Kretz <kretz@kde.org>

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

#include "kviewpresenterconfig.h"

#include <tqlayout.h>
#include <tqcheckbox.h>
#include <tqframe.h>

#include <tdelocale.h>
#include <kdialog.h>
#include <tdeglobal.h>
#include <tdeconfig.h>
#include <kgenericfactory.h>

typedef KGenericFactory<KViewPresenterConfig, TQWidget> KViewPresenterConfigFactory;
K_EXPORT_COMPONENT_FACTORY( kcm_kviewpresenterconfig, KViewPresenterConfigFactory( "kcm_kviewpresenterconfig" ) )

KViewPresenterConfig::KViewPresenterConfig( TQWidget * parent, const char *, const TQStringList & args )
	: TDECModule( KViewPresenterConfigFactory::instance(), parent, args )
{
	TQBoxLayout * layout = new TQVBoxLayout( this, KDialog::marginHint(), KDialog::spacingHint() );
	layout->setAutoAdd( true );

	m_pCheckBox = new TQCheckBox( "This is only for testing...", this );
	connect( m_pCheckBox, TQT_SIGNAL( clicked() ), this, TQT_SLOT( checkChanged() ) );
}

KViewPresenterConfig::~KViewPresenterConfig()
{
}

void KViewPresenterConfig::checkChanged()
{
	if( m_pCheckBox->isChecked() )
		emit changed( true );
	else
		emit changed( false );
}

void KViewPresenterConfig::load()
{
	emit changed( false );
}

void KViewPresenterConfig::save()
{
	emit changed( false );
}

void KViewPresenterConfig::defaults()
{
}

// vim:sw=4:ts=4

#include "kviewpresenterconfig.moc"
