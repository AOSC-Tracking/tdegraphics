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

#include "kviewconfmodules.h"

#include <tqlayout.h>
#include <tqvbuttongroup.h>
#include <tqradiobutton.h>

#include <klocale.h>
#include <kdialog.h>
#include <kglobal.h>
#include <kconfig.h>
#include <kgenericfactory.h>
#include <tqwhatsthis.h>

typedef KGenericFactory<KViewGeneralConfig, TQWidget> KViewGeneralConfigFactory;
K_EXPORT_COMPONENT_FACTORY( kcm_kviewgeneralconfig, KViewGeneralConfigFactory( "kcm_kviewgeneralconfig" ) )

KViewGeneralConfig::KViewGeneralConfig( TQWidget * parent, const char * /*name*/, const TQStringList & args )
	: KCModule( KViewGeneralConfigFactory::instance(), parent, args )
{
	TQBoxLayout * tqlayout = new TQVBoxLayout( this, KDialog::marginHint(), KDialog::spacingHint() );

	m_pResizeGroup = new TQVButtonGroup( i18n( "Resizing" ), this );
	m_pResizeGroup->tqsetSizePolicy( TQSizePolicy( TQSizePolicy::Minimum, TQSizePolicy::Fixed ) );
	connect( m_pResizeGroup, TQT_SIGNAL( clicked( int ) ), this, TQT_SLOT( resizeChanged( int ) ) );
	tqlayout->addWidget( m_pResizeGroup );

	( void )new TQRadioButton( i18n( "Only resize window" ), m_pResizeGroup );
	( void )new TQRadioButton( i18n( "Resize image to fit window" ), m_pResizeGroup );
	( void )new TQRadioButton( i18n( "Don't resize anything" ), m_pResizeGroup );
	TQWhatsThis::add( new TQRadioButton( i18n( "Best fit" ), m_pResizeGroup ),
			i18n( "<p>KView will resize the window to fit the image. The image will never be scaled up but if it is too large for the screen the image will be scaled down.</p>" ) );

	load();
}

KViewGeneralConfig::~KViewGeneralConfig()
{
}

void KViewGeneralConfig::save()
{
	KConfigGroup cfgGroup( KGlobal::config(), "KView General" );
	m_savedPosition = m_pResizeGroup->id( m_pResizeGroup->selected() );
	cfgGroup.writeEntry( "Resize Mode", m_savedPosition );
	emit changed( false );
	KGlobal::config()->sync();
}

void KViewGeneralConfig::load()
{
	KConfigGroup cfgGroup( KGlobal::config(), "KView General" );
	int m_savedPosition = cfgGroup.readNumEntry( "Resize Mode", 2 );
	if( m_savedPosition < 0 || m_savedPosition > 3 )
		m_savedPosition = 2;
	m_pResizeGroup->setButton( m_savedPosition );
	emit changed( false );
}

void KViewGeneralConfig::defaults()
{
	m_pResizeGroup->setButton( 2 );
	emit changed( ( 2 != m_savedPosition ) );
}

void KViewGeneralConfig::resizeChanged( int pos )
{
	emit changed( ( pos != m_savedPosition ) );
}

// vim:sw=4:ts=4

#include "kviewconfmodules.moc"
