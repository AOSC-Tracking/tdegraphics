/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Luis Carvalho
    email                : lpassos@oninetspeed.pt
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/

#include "pmlibrarymanager.h"

#include <tdeconfig.h>
#include <kstandarddirs.h>
#include <kglobal.h>

#include <tqfile.h>
#include <tqdir.h>

#include "pmdebug.h"

PMLibraryManager* PMLibraryManager::s_pInstance = 0;
KStaticDeleter<PMLibraryManager> PMLibraryManager::s_staticDeleter;

PMLibraryHandle* PMLibraryManager::getLibraryHandle( const TQString& libraryName ) 
{
   TQPtrListIterator<PMLibraryHandle> it( m_libraries );

   for( ; it.current( ); ++it )
      if( it.current( )->name( ) == libraryName )
         return it.current( );

   return NULL;
}

PMLibraryManager::PMLibraryManager( )
{
   m_libraries.setAutoDelete( true );
   scanLibraries( );
}

PMLibraryManager::~PMLibraryManager( )
{
   m_libraries.clear( );
}

void PMLibraryManager::saveConfig( TDEConfig* /*cfg*/ )
{
}

void PMLibraryManager::restoreConfig( TDEConfig* /*cfg*/ )
{
}

TQValueList<TQString> PMLibraryManager::availableLibraries( )
{
   TQValueList<TQString> result;
   TQPtrListIterator<PMLibraryHandle> it( m_libraries );

   for( ; it.current( ); ++it )
      result.push_back( it.current( )->name( ) );

   return result;
}

PMLibraryManager* PMLibraryManager::theManager( )
{
   if( !s_pInstance )
      s_staticDeleter.setObject( s_pInstance, new PMLibraryManager( ) );
   return s_pInstance;
}

void PMLibraryManager::scanLibraries( )
{
   TQStringList libraryDirectories;
   
   // Search for sub directories in /usr/share/kpovmodeler/library
   libraryDirectories = TDEGlobal::dirs( )->findDirs( "data", "kpovmodeler/library" );

   for( TQStringList::Iterator i = libraryDirectories.begin( ); i != libraryDirectories.end( ); ++i )
   {
      TQDir curDir( *i );
      curDir.setFilter( TQDir::Dirs );
      TQFileInfoListIterator it( *( curDir.entryInfoList( ) ) );

      // For each sub directory
      TQFileInfo* fi;
      for( ; ( fi = it.current( ) ) != NULL; ++it ) 
      {
         // check for the existance of library_index.xml
         // If it's there it's a library
         if( TQFile::exists( fi->absFilePath( ) + "/library_index.xml" ) )
         {
            // Create the corresponding PMLibraryHandle
            PMLibraryHandle* h;

            h = new PMLibraryHandle( fi->absFilePath( ) );
            if( !getLibraryHandle( h->name( ) ) )
               m_libraries.append( h );
            else
               // a library with that name already exists
               delete h;
         }
      }
   }
}

void PMLibraryManager::refresh( )
{
   // TODO: Manage the list incrementaly so that previouly handed out 
   // PMLibraryHandle pointers are kept valid
   m_libraries.clear( );
   scanLibraries( );
}
