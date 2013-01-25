//-*-C++-*-
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

#include "pmlibraryiconview.h"

#include <unistd.h>

#include <tqstring.h>
#include <tqdir.h>

#include <kurl.h>
#include <kio/job.h>
#include <kmessagebox.h>
#include <klocale.h>

#include <stdlib.h>

#include "pmlibraryhandle.h"
#include "pmlibraryobject.h"
#include "pmdebug.h"

const char* PMLibraryIconDrag::format( int i ) const
{
   switch( i )
   {
      case 0:
         return "application/x-qiconlist";
         break;
      case 1:
         return "text/sublib-list";
         break;
      default:
         return 0;
         break;
   }
}

TQByteArray PMLibraryIconDrag::encodedData( const char* mime ) const
{
   TQByteArray a;
   if ( TQString( mime ) == "application/x-qiconlist" )
      a = TQIconDrag::encodedData( mime );
   else if ( TQString( mime ) == "text/sublib-list" )
   {
      TQString s , l;
      for( unsigned i = 0; i < m_paths.count( ); ++i )
      {
         if( m_subLibs[i] )
            l = "true";
         else
            l = "false";
         s += m_paths[i] + "\r" + l + "\n";
      }
      a.resize( s.length( ) );
      memcpy( a.data( ), s.latin1( ), s.length( ) );
   }
   return a;
}

bool PMLibraryIconDrag::canDecode( TQMimeSource* e )
{
   return e->provides( "application/x-qiconlist" ) ||
          e->provides( "text/sublib-list" );
}

bool PMLibraryIconDrag::decode( TQMimeSource* e, TQStringList& strList, TQValueList<bool>& subLibList )
{
   TQByteArray a = e->encodedData( "text/sublib-list" );
   if( a.isEmpty( ) )
      return false;

   TQStringList list = TQStringList::split( "\n", TQString( a ) );
   for( unsigned i = 0; i < list.count( ); ++i )
   {
      strList.append( list[i].section( "\r", 0, 0 ) );
      if( list[i].section( "\r", 1, 1 ) == "true" )
         subLibList.append( true );
      else
         subLibList.append( false );
   }
   return true;
}

void PMLibraryIconDrag::append( const TQIconDragItem &item, const TQRect &pr,
                               const TQRect &tr, const TQString &path, bool isSubLibrary )
{
   TQIconDrag::append( item, pr, tr );
   m_paths << path;
   m_subLibs.append( isSubLibrary );
}

PMLibraryIconView::PMLibraryIconView( TQWidget* parent, const char* name )
      : KIconView( parent, name )
{
   m_pLibrary = NULL;
   m_pCurrentLibrary = NULL;
   setSelectionMode( Single );
   setMode( Execute );
}

void PMLibraryIconView::setLibrary( PMLibraryHandle* h )
{
   m_pLibrary = h;
   refresh( );
}

void PMLibraryIconView::refresh( )
{
   // Clear all the icons
   clear( );

   PMLibraryHandle::EntryIterator* it;

   // Scan all the library objects and load them into the view
   // First let's add the libraries
   it = m_pLibrary->createSubLibraryIterator( );
   for( ; it->current( ); ++(*it) )
   {
      TQString f_name = *( it->current( ) );
      PMLibraryHandle h( f_name );

      new PMLibraryIconViewItem( this, h.name( ), f_name, true );
   }
   delete it;

   // Then the objects
   it = m_pLibrary->createObjectIterator( );
   for( ; it->current( ); ++(*it) )
   {
      TQString f_name = *( it->current( ) );
      PMLibraryObject obj( f_name );

      if( obj.preview( ) )
         new PMLibraryIconViewItem( this, obj.name( ), obj.preview( )->copy( ), f_name, false );
      else
         new PMLibraryIconViewItem( this, obj.name( ), f_name, false );
   }
   delete it;
}

void PMLibraryIconView::slotDropped( TQDropEvent *e, const TQValueList<TQIconDragItem> & )
{
   e->ignore( );
}

TQDragObject* PMLibraryIconView::dragObject( )
{
   if ( !currentItem( ) )
      return 0;

   TQPoint orig = viewportToContents( viewport( )->mapFromGlobal( TQCursor::pos( ) ) );
   PMLibraryIconDrag *drag = new PMLibraryIconDrag( viewport( ) );
   drag->setPixmap( *currentItem( )->pixmap( ),
                     TQPoint( currentItem( )->pixmapRect( ).width( ) / 2,
                             currentItem( )->pixmapRect( ).height( ) / 2 ) );

   for ( PMLibraryIconViewItem *item = (PMLibraryIconViewItem*)firstItem( );
         item; item = ( PMLibraryIconViewItem* )item->nextItem( ) )
   {
      if ( item->isSelected( ) )
      {
         TQIconDragItem id;
         id.setData( TQCString( item->path( ).latin1( ) ) );
         drag->append( id,
                       TQRect( item->pixmapRect( FALSE ).x( ) - orig.x( ),
                              item->pixmapRect( FALSE ).y( ) - orig.y( ),
                              item->pixmapRect( ).width( ),
                              item->pixmapRect( ).height( ) ),
                       TQRect( item->textRect( FALSE ).x( ) - orig.x( ),
                              item->textRect( FALSE ).y( ) - orig.y( ),
                              item->textRect().width( ),
                              item->textRect( ).height( ) ),
                       item->path( ),
                       item->isSubLibrary( ) );
      }
   }
   return drag;
}

PMLibraryIconViewItem::PMLibraryIconViewItem( TQIconView *parent, const TQString &text, const TQString& path, bool isSubLibrary )
   : KIconViewItem( parent, text )
{
   m_path = path;
   m_isSubLibrary = isSubLibrary;
}

PMLibraryIconViewItem::PMLibraryIconViewItem( TQIconView *parent, const TQString &text, const TQImage& image, const TQString& path, bool isSubLibrary )
   : KIconViewItem( parent, text, image )
{
   m_path = path;
   m_isSubLibrary = isSubLibrary;
}

bool PMLibraryIconViewItem::acceptDrop( const TQMimeSource *e ) const
{
   if ( m_isSubLibrary && e->provides( "text/sublib-list" ) )
      return true;

   return false;
}

void PMLibraryIconViewItem::dropped( TQDropEvent *e, const TQValueList<TQIconDragItem> & )
{
   TQStringList pathList;
   TQValueList<bool> subLibList;
   if( m_isSubLibrary && PMLibraryIconDrag::decode( e, pathList, subLibList ) )
   {
      PMLibraryIconView* source = static_cast<PMLibraryIconView*>( e->source( )->parentWidget( ) );
      PMLibraryHandle* parentLib = source->library( );
      PMLibraryHandle newParentLib = PMLibraryHandle( m_path );
      if ( parentLib->isReadOnly() || newParentLib.isReadOnly() )
      {
         e->ignore();
         return;
      }

      for( unsigned i = 0; i < pathList.count( ); ++i )
      {
         bool success = true;
         TQString path = pathList[i];
         if( path != ( m_path +"/" + path.section( '/', -1 ) ) )
         {
            if( subLibList[i] )
            {
               TQString newpath = newPath( path, true );
               if( parentLib->deleteSubLibrary( path ) == PMLibraryHandle::Ok )
               {
                  PMLibraryHandle lib = PMLibraryHandle( path );
                  if( newParentLib.addSubLibrary( newpath, lib.name() ) == PMLibraryHandle::Ok )
                  {
                     lib.changeParentLibrary( m_path );
                     TDEIO::move( path, newpath );
                  }
                  else
                  {
                     success = false;
                     parentLib->addSubLibrary( path, lib.name( ) );
                  }
               }
               else
                  success = false;
            }
            else
            {
               TQString newpath = newPath( path, false );
               if( parentLib->deleteObject( path ) == PMLibraryHandle::Ok )
               {
                  PMLibraryObject obj = PMLibraryObject( path );
                  if( newParentLib.addObject( newpath, obj.name() ) == PMLibraryHandle::Ok )
                  {
                     TDEIO::move( path, newpath );
                  }
                  else
                  {
                     success = false;
                     parentLib->addObject( path, obj.name( ) );
                  }
               }
               else
                  success = false;
            }

            if( !success )
            {
               KMessageBox::error( 0, i18n( "Error moving \"%1\" to \"%2\"" ).arg( path ).arg( m_path ) );
               e->ignore( );
               return;
            }
         }
      }
      e->acceptAction( );
      source->refresh( );
   }
   else
   {
      e->ignore( );
   }
}

TQString PMLibraryIconViewItem::newPath( const TQString oldPath, bool /*subLib*/ )
{
   /// @todo Need to replace mkdtemp and mkstemps before enabling libs
   return oldPath;
   /*
   TQString path = m_path + "/" + oldPath.section( '/', -1 );
   if( subLib )
   {
      TQString test = path + "/library_index.xml";
      if( TQFile::exists( test ) )
      {
         TQCString s = m_path.latin1();
         s+= "/libXXXXXX";
         char* dirname = mkdtemp( s.data() );
         rmdir( dirname );
         path = dirname;
      }
   }
   else if( TQFile::exists( path ) )
   {
      // we need to rename it.
      TQCString s = m_path.latin1();
      s += "/objXXXXXX.kpml";
      int fh = mkstemps( s.data( ), 5 );
      close( fh );
      unlink( s.data() );
      path = s;
   }

   return path;
   */
}

#include "pmlibraryiconview.moc"
