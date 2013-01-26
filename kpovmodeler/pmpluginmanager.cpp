/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Andreas Zehender
    email                : zehender@kde.org
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/

#include "pmpluginmanager.h"
#include "pmpart.h"
#include "pmshell.h"
#include "pmfactory.h"
#include "pmdebug.h"

#include <tqmap.h>
#include <tqapplication.h>

#include <tdeparts/plugin.h>
#include <kxmlguifactory.h>
#include <kconfig.h>

PMPluginManager* PMPluginManager::s_pInstance = 0;
KStaticDeleter<PMPluginManager> PMPluginManager::s_staticDeleter;

using namespace KParts;

// workaround for protected Plugin::pluginInfos
class PMPluginWorkaround : public Plugin
{
public:
   PMPluginWorkaround( ) : Plugin( 0, 0 ) { };
   static TQValueList<Plugin::PluginInfo> installedPlugins( const TDEInstance* instance )
   {
      return pluginInfos( instance );
   }
};

PMPluginManager::PMPluginManager( )
{
   // find installed plugins
   TDEConfigGroup cfgGroup( PMFactory::instance( )->config( ),
                          "KParts Plugins" );
   TQValueList<Plugin::PluginInfo> plugins
      = PMPluginWorkaround::installedPlugins( PMFactory::instance( ) );
   TQValueList<Plugin::PluginInfo>::ConstIterator pIt = plugins.begin( );
   TQValueList<Plugin::PluginInfo>::ConstIterator pEnd = plugins.end( );
   
   for( ; pIt != pEnd; ++pIt )
   {
      TQDomElement docElem = ( *pIt ).m_document.documentElement( );
      TQString name = docElem.attribute( "name" );
      TQString description = docElem.attribute( "description" );
      if( !description.isEmpty( ) )
         description = i18n( description.latin1( ) );
      bool enabled = cfgGroup.readBoolEntry( name + "Enabled", false );
      m_plugins.append( new PMPluginInfo( name, description, enabled ) );
   }
}

PMPluginManager::~PMPluginManager( )
{
   m_plugins.setAutoDelete( true );
   m_plugins.clear( );
}

PMPluginManager* PMPluginManager::theManager( )
{
   if( !s_pInstance )
      s_staticDeleter.setObject( s_pInstance, new PMPluginManager( ) );
   return s_pInstance;
}

void PMPluginManager::registerPart( PMPart* p )
{
   if( !m_parts.containsRef( p ) )
   {
      m_parts.append( p );
      Plugin::loadPlugins( p, p, PMFactory::instance( ), false );
   }
}

void PMPluginManager::removePart( PMPart* p )
{
   m_parts.removeRef( p );
}

void PMPluginManager::updatePlugins( )
{
   TDEConfigGroup cfgGroup( PMFactory::instance( )->config( ),
                          "KParts Plugins" );
   TQPtrListIterator<PMPluginInfo> pit( m_plugins );
   for( ; *pit; ++pit )
      cfgGroup.writeEntry( ( *pit )->name( ) + "Enabled",
                           ( *pit )->enabled( ) );
   cfgGroup.sync( );

   TQPtrListIterator<PMPart> it( m_parts );
   for( ; *it; ++it )
   {
      Plugin::loadPlugins( *it, *it, PMFactory::instance( ), false );
      PMShell* shell = ( *it )->shell( );
      if( shell )
         shell->updateGUI( );
      // TODO find a solution to update the gui without using the shell
   }
}
