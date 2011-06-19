/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Andreas Zehender
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


#include "pmdeletecommand.h"
#include "pmcommandmanager.h"
#include "pmdeclare.h"
#include "pmrecursiveobjectiterator.h"
#include "pmmemento.h"

#include <klocale.h>
#include <kmessagebox.h>
#include <tqptrdict.h>

PMDeleteCommand::PMDeleteCommand( PMObject* obj )
      : PMCommand( i18n( "Delete %1" ).tqarg( obj->name( ) ) )
{
   // the scene can not be deleted!
   if( obj->tqparent( ) )
      m_infoList.append( new PMDeleteInfo( obj ) );
   else
   {
      // object has no tqparent!
      // top level objects can't be moved, move all child items
      PMObject* tmp;
      for( tmp = obj->firstChild( ); tmp; tmp = tmp->nextSibling( ) )
         m_infoList.append( new PMDeleteInfo( tmp ) );
   }
   m_executed = false;
   m_firstExecution = true;
   m_linksCreated = false;
}

PMDeleteCommand::PMDeleteCommand( const PMObjectList& list )
      : PMCommand( i18n( "Delete Objects" ) )
{
   PMObjectListIterator it( list );
   PMObject* obj;

   for( ; it.current( ); ++it )
   {
      obj = it.current( );
      
      if( obj->tqparent( ) )  
         m_infoList.append( new PMDeleteInfo( obj ) );
      else
      {
         // object has no tqparent!
         // top level objects can't be moved, move all child items
         PMObject* tmp;
         for( tmp = obj->firstChild( ); tmp; tmp = tmp->nextSibling( ) )
            m_infoList.append( new PMDeleteInfo( tmp ) );
      }
   }
         
   m_infoList.setAutoDelete( true );
   m_executed = false;
   m_firstExecution = true;
   m_linksCreated = false;
}

PMDeleteCommand::~PMDeleteCommand( )
{
   if( m_executed )
   {
      PMDeleteInfoListIterator it( m_infoList );
      for( ; it.current( ); ++it )
         delete ( it.current( )->deletedObject( ) );
   }

   m_infoList.clear( );
}

void PMDeleteCommand::execute( PMCommandManager* theManager )
{
   if( !m_executed )
   {
      PMDeleteInfoListIterator it( m_infoList );
      PMDeleteInfo* info = 0;
      PMObject* tqparent;
      
      if( !m_linksCreated )
      {
         PMDeclare* decl;
         for( ; it.current( ); ++it )
         {
            PMRecursiveObjectIterator oit( it.current( )->deletedObject( ) );
            for( ; oit.current( ); ++oit )
            {
               decl = oit.current( )->linkedObject( );
               if( decl )
               {
                  m_links.append( oit.current( ) );
                  if( !m_linkedDeclares.tqcontainsRef( decl ) )
                     m_linkedDeclares.append( decl );
               }
            }
         }
         m_linksCreated = true;
      }

      PMObjectListIterator lit( m_links );
      for( ; lit.current( ); ++lit )
         lit.current( )->linkedObject( )->removeLinkedObject( lit.current( ) );

      for( it.toLast( ); it.current( ); --it )
      {
         info = it.current( );
         tqparent = info->tqparent( );
         // signal has to be emitted before the item is removed
         theManager->cmdObjectChanged( info->deletedObject( ), PMCRemove );
         if( m_firstExecution )
            if( tqparent->dataChangeOnInsertRemove( )
                && !tqparent->mementoCreated( ) )
               tqparent->createMemento( );
         tqparent->takeChild( info->deletedObject( ) );
      }

      if( m_firstExecution )
      {
         for( it.toLast( ); it.current( ); --it )
         {
            tqparent = it.current( )->tqparent( );
            if( tqparent->mementoCreated( ) )
               m_dataChanges.append( tqparent->takeMemento( ) );
         }
      }

      TQPtrListIterator<PMMemento> mit( m_dataChanges );
      for( ; mit.current( ); ++mit )
      {
         PMObjectChangeListIterator change = mit.current( )->changedObjects( );
         for( ; change.current( ); ++change )
            theManager->cmdObjectChanged( change.current( )->object( ),
                                          change.current( )->mode( ) );
      }
      
      PMObjectListIterator dit( m_linkedDeclares );
      for( ; dit.current( ); ++dit )
         theManager->cmdObjectChanged( dit.current( ), PMCData );
      
      m_executed = true;
      m_firstExecution = false;
   }
}

void PMDeleteCommand::undo( PMCommandManager* theManager )
{
   if( m_executed )
   {
      PMDeleteInfoListIterator it( m_infoList );
      for( ; it.current( ); ++it )
      {
         if( it.current( )->prevSibling( ) )
            it.current( )->tqparent( )
               ->insertChildAfter( it.current( )->deletedObject( ),
                                   it.current( )->prevSibling( ) );
         else
            it.current( )->tqparent( )
               ->insertChild( it.current( )->deletedObject( ), 0 );
         theManager->cmdObjectChanged( it.current( )->deletedObject( ), PMCAdd );
      }
      
      PMObjectListIterator lit( m_links );
      for( ; lit.current( ); ++lit )
         lit.current( )->linkedObject( )->addLinkedObject( lit.current( ) );
      PMObjectListIterator dit( m_linkedDeclares );
      for( ; dit.current( ); ++dit )
         theManager->cmdObjectChanged( dit.current( ), PMCData );
      
      TQPtrListIterator<PMMemento> mit( m_dataChanges );
      for( ; mit.current( ); ++mit )
      {
         mit.current( )->originator( )->restoreMemento( mit.current( ) );
         PMObjectChangeListIterator change = mit.current( )->changedObjects( );
         for( ; change.current( ); ++change )
            theManager->cmdObjectChanged( change.current( )->object( ),
                                          change.current( )->mode( ) );
      }
      
      m_executed = false;
   }
}

int PMDeleteCommand::errorFlags( PMPart* )
{
   PMDeleteInfo* info;
   PMDeclare* decl = 0;
   PMObject* obj;
   bool insideSelection;
   bool ok = true;
   bool error = false;
   
   // dictionary of deleted objects
   TQPtrDict<bool> m_deletedObjects( 1009 );
   m_deletedObjects.setAutoDelete( true );
   PMDeleteInfoListIterator it( m_infoList );
   for( ; it.current( ); ++it )
      m_deletedObjects.insert( it.current( )->deletedObject( ),
                               new bool( true ) );
   
   // declares can only be deleted, if all linked
   // objects are deleted as well

   info = m_infoList.last( );
   
   while( info )
   {
      ok = true;
      if( info->deletedObject( )->isA( "Declare" ) )
      {
         decl = ( PMDeclare* ) ( info->deletedObject( ) );
         PMObjectListIterator links = decl->linkedObjects( );
         
         for( ; links.current( ) && ok; ++links )
         {
            insideSelection = false;
            for( obj = links.current( ); obj && !insideSelection;
                 obj = obj->tqparent( ) )
            {
               if( m_deletedObjects.tqfind( obj ) )
                  insideSelection = true;
            }

            if( insideSelection )
            {
               bool stop = false;
               for( obj = links.current( ); obj && !stop; obj = obj->tqparent( ) )
               {
                  if( m_deletedObjects.tqfind( obj ) )
                     stop = true;
                  else
                     m_deletedObjects.insert( obj, new bool( true ) );
               }
            }
            else
               ok = false;
         }
      }
      
      if( !ok )
      {
         m_errors.prepend( i18n( "The declare \"%1\" can't be removed "
                                 "because of some remaining links." )
                           .tqarg( decl->id( ) ) );
         
         PMDeleteInfo* tmp = info;
         info = m_infoList.prev( );
         m_deletedObjects.remove( decl );
         m_infoList.removeRef( tmp );
         
         error = true;
      }
      else
         info = m_infoList.prev( );
   }

   if( error )
   {
      if( m_infoList.count( ) == 0 )
         return PMEError | PMEFatal;
      else
         return PMEError;
   }
   
   return PMENone;
}
