/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Andreas Zehender
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

#include "pmdocumentationmap.h"

#include <kconfig.h>
#include <kstandarddirs.h>

#include <tqfile.h>
#include <tqdom.h>

#include "pmdebug.h"

PMDocumentationMap* PMDocumentationMap::s_pInstance = 0;
KStaticDeleter<PMDocumentationMap> PMDocumentationMap::s_staticDeleter;

TQString PMDocumentationVersion::documentation( const TQString& className ) const
{
   if( m_map.contains( className ) )
      return m_map[className];
   return m_index;
}

void PMDocumentationVersion::loadData( TQDomElement& e )
{
   TQString className;
   TQString target;
   
   m_version = e.attribute( "number", "3.1" );
   m_index = e.attribute( "index", "index.htm" );

   TQDomNode m = e.firstChild( );
   while( !m.isNull( ) )
   {
      if( m.isElement( ) )
      {
         TQDomElement me = m.toElement( );
         className = me.attribute( "className", "" );
         target = me.attribute( "target", "" );
         if( !className.isEmpty( ) && !target.isEmpty( ) )
            m_map.insert( className, target );
      }
      m = m.nextSibling( );
   }
}

PMDocumentationMap::PMDocumentationMap( )
{
   m_pCurrentVersion = 0;
   m_mapLoaded = false;
}

PMDocumentationMap::~PMDocumentationMap( )
{
   m_maps.setAutoDelete( true );
   m_maps.clear( );
}

void PMDocumentationMap::saveConfig( KConfig* cfg )
{
   cfg->setGroup( "Povray" );
   cfg->writePathEntry( "DocumentationPath", m_documentationPath );
   cfg->writeEntry( "DocumentationVersion", m_currentVersion );
}

void PMDocumentationMap::restoreConfig( KConfig* cfg )
{
   cfg->setGroup( "Povray" );
   m_documentationPath = cfg->readPathEntry( "DocumentationPath" );
   m_currentVersion = cfg->readEntry( "DocumentationVersion", "3.1" );
}

void PMDocumentationMap::setDocumentationVersion( const TQString& v )
{
   m_currentVersion = v;
   if( m_mapLoaded )
      findVersion( );
}

TQValueList<TQString> PMDocumentationMap::availableVersions( )
{
   if( !m_mapLoaded )
      loadMap( );

   TQValueList<TQString> result;
   TQPtrListIterator<PMDocumentationVersion> it( m_maps );
   
   for( ; it.current( ); ++it )
      result.push_back( it.current( )->version( ) );
   
   return result;
}

TQString PMDocumentationMap::documentation( const TQString& objectName )
{
   if( !m_mapLoaded )
      loadMap( );
   
   TQString url;

   if( !m_documentationPath.isEmpty( ) )
      if( !m_documentationPath.endsWith( TQString( "/" ) ) )
         m_documentationPath += "/";
   
   if( !m_documentationPath.isEmpty( ) && m_pCurrentVersion )
      url = m_documentationPath
            + m_pCurrentVersion->documentation( objectName );

   return url;
}

void PMDocumentationMap::loadMap( )
{
   if( !m_mapLoaded )
   {
      m_mapLoaded = true;
      
      TQString fileName = locate( "data", "kpovmodeler/povraydocmap.xml" );
      if( fileName.isEmpty( ) )
      {
         kdError( PMArea ) << "Povray documentation map not found" << endl;
         return;
      }
      
      TQFile file( fileName );
      if( !file.open( IO_ReadOnly ) )
      {
         kdError( PMArea ) << "Could not open the povray documentation map file"
                           << endl;
         return;
      }

      TQDomDocument doc( "DOCMAP" );
      doc.setContent( &file );

      TQDomElement e = doc.documentElement( );
      TQDomNode c = e.firstChild( );

      TQString str;

      while( !c.isNull( ) )
      {
         if( c.isElement( ) )
         {
            TQDomElement ce = c.toElement( );
            PMDocumentationVersion* v = new PMDocumentationVersion( );
            m_maps.append( v );
            v->loadData( ce );
         }
         c = c.nextSibling( );
      }

      findVersion( );
   }
}

void PMDocumentationMap::findVersion( )
{
   TQPtrListIterator< PMDocumentationVersion > it( m_maps );
   bool found = false;

   m_pCurrentVersion = 0;

   for( ; it.current( ) && !found; ++it )
   {
      if( it.current( )->version( ) == m_currentVersion )
      {
         found = true;
         m_pCurrentVersion = it.current( );
      }
   }
}

PMDocumentationMap* PMDocumentationMap::theMap( )
{
   if( !s_pInstance )
      s_staticDeleter.setObject( s_pInstance, new PMDocumentationMap( ) );
   return s_pInstance;
}
