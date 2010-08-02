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

#include "pmresourcelocator.h"
#include "pmpovrayrenderwidget.h"
#include "pmdebug.h"

#include <tqfileinfo.h>
#include <tqdir.h>

PMResourceLocator* PMResourceLocator::s_pInstance = 0;
KStaticDeleter<PMResourceLocator> PMResourceLocator::s_staticDeleter;

PMResourceLocator::PMResourceLocator( )
      : m_cache( 100, 109, true )
{
   m_cache.setAutoDelete( true );
}

PMResourceLocator::~PMResourceLocator( )
{
   m_cache.clear( );
}

TQString PMResourceLocator::findFile( const TQString& file )
{
   if( !s_pInstance )
      s_staticDeleter.setObject( s_pInstance, new PMResourceLocator( ) );
   return s_pInstance->lookUp( file );
}

void PMResourceLocator::clearCache( )
{
   if( s_pInstance )
      s_pInstance->m_cache.clear( );
}

TQString PMResourceLocator::lookUp( const TQString& file )
{
   if( file.isEmpty( ) )
      return TQString::null;

   kdDebug( PMArea ) << "LookUp: " << file << endl;
   
   TQString* ps = m_cache.find( file );
   if( ps )
      return *ps;

   bool found = false;
   TQString fullPath = TQString::null;
   
   if( file[0] == '/' )
   {
      // absolute path, library paths are not used
      TQFileInfo info( file );
      if( info.exists( ) && info.isReadable( ) && info.isFile( ) )
      {
         found = true;
         fullPath = file;
      }
   }
   else
   {
      TQStringList plist = PMPovrayRenderWidget::povrayLibraryPaths( );
      TQStringList::ConstIterator it = plist.begin( );
      for( ; ( it != plist.end( ) ) && !found; ++it )
      {
         TQDir dir( *it );
         TQFileInfo info( dir, file );
         if( info.exists( ) && info.isReadable( ) && info.isFile( ) )
         {
            found = true;
            fullPath = info.absFilePath( );
         }
      }
   }

   if( found )
   {
      TQString* ni = new TQString( fullPath );
      if( !m_cache.insert( file, ni ) )
         delete ni;
      kdDebug( PMArea ) << "File \"" << file << "\" found in "
                        << fullPath << endl;
   }

   return fullPath;
}

