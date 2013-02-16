/*  This file is part of the KDE project
    Copyright (C) 2003 Matthias Kretz <kretz@kde.org>

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

#include "kviewpluginsconfig.h"

#include <kpluginselector.h>
#include <kgenericfactory.h>
#include <tdelocale.h>
#include <tdeglobal.h>
#include <ksimpleconfig.h>
#include <kplugininfo.h>

typedef KGenericFactory<KViewPluginsConfig, TQWidget> KViewPluginsConfigFactory;
K_EXPORT_COMPONENT_FACTORY( kcm_kviewpluginsconfig, KViewPluginsConfigFactory( "kcm_kviewpluginsconfig" ) )

KViewPluginsConfig::KViewPluginsConfig( TQWidget * parent, const char *, const TQStringList & args )
	: KSettings::PluginPage( KViewPluginsConfigFactory::instance(), parent, args )
{
	m_config = new KSimpleConfig( "kviewrc" );
	pluginSelector()->addPlugins( TQString::fromAscii( "kview" ), i18n( "Application" ), TQString(), m_config );
	pluginSelector()->setShowEmptyConfigPage( false );
}

KViewPluginsConfig::~KViewPluginsConfig()
{
	delete m_config;
}

// vim: sw=4 ts=4 noet

#include "kviewpluginsconfig.moc"
