/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2002 by Andreas Zehender
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

#include "pmdeclare.h"
#include "pmdeclareedit.h"
#include "pmxmlhelper.h"
#include "pmmemento.h"
#include "pmprototypemanager.h"
#include "pmpart.h"

#include <tqvaluelist.h>
#include <klocale.h>

PMDefinePropertyClass( PMDeclare, PMDeclareProperty );

PMMetaObject* PMDeclare::s_pMetaObject;
PMObject* createNewDeclare( PMPart* part )
{
   return new PMDeclare( part );
}

PMDeclare::PMDeclare( PMPart* part )
      : Base( part )
{
   m_pDeclareType = 0;
}

PMDeclare::PMDeclare( const PMDeclare& d )
      : Base( d )
{
   m_id = d.m_id; // CAUTION! Duplication of the id which has to be unique
   m_pDeclareType = 0; // will be set automatically in the
   // base constuctor when the tqchildren are copied
}

PMMetaObject* PMDeclare::tqmetaObject( ) const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "Declare", Base::tqmetaObject( ),
                                        createNewDeclare );
      s_pMetaObject->addProperty(
         new PMDeclareProperty( "id", &PMDeclare::setID, &PMDeclare::id ) );
   }
   return s_pMetaObject;
}

void PMDeclare::cleanUp( ) const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp( );
}

PMDeclare::~PMDeclare( )
{
}

TQString PMDeclare::description( ) const
{
   PMPart* pPart = part( );
   TQString d = i18n( "declaration" );

   if( m_pDeclareType && pPart )
   {
      const TQValueList<PMDeclareDescription>& descriptions
         = pPart->prototypeManager( )->declarationTypes( );
      TQValueList<PMDeclareDescription>::const_iterator it;
      bool found = false;
      for( it = descriptions.begin( ); it != descriptions.end( ) && !found; ++it )
      {
         if( ( *it ).type == m_pDeclareType )
         {
            d = ( *it ).description;
            found = true;
         }
      }
   }
   return d;
}

TQString PMDeclare::pixmap( ) const
{
   PMPart* pPart = part( );
   TQString d = "pmdeclare";

   if( m_pDeclareType && pPart )
   {
      const TQValueList<PMDeclareDescription>& descriptions
         = pPart->prototypeManager( )->declarationTypes( );
      TQValueList<PMDeclareDescription>::const_iterator it;
      bool found = false;
      for( it = descriptions.begin( ); it != descriptions.end( ) && !found; ++it )
      {
         if( ( *it ).type == m_pDeclareType )
         {
            d = ( *it ).pixmap;
            found = true;
         }
      }
   }
   return d;
}

void PMDeclare::serialize( TQDomElement& e, TQDomDocument& doc ) const
{
   e.setAttribute( "id", m_id );
   Base::serialize( e, doc );
}

void PMDeclare::readAttributes( const PMXMLHelper& h )
{
   m_id = h.stringAttribute( "id", "object" );
   Base::readAttributes( h );
}

PMDialogEditBase* PMDeclare::editWidget( TQWidget* parent ) const
{
   return new PMDeclareEdit( parent );
}

void PMDeclare::setID( const TQString& newID )
{
   if( newID != m_id )
   {
      if( m_pMemento )
      {
         m_pMemento->addIDChange( s_pMetaObject, PMIDID, m_id );
         m_pMemento->setDescriptionChanged( );
      }

      m_id = newID;
   }
}

TQString PMDeclare::declareType( ) const
{
   if( !m_pDeclareType )
      return TQString( "None" );
   return m_pDeclareType->className( );
}

void PMDeclare::setDeclareType( PMMetaObject* t )
{
   if( m_pDeclareType != t )
   {
      if( m_pMemento )
         m_pMemento->setDescriptionChanged( );

      m_pDeclareType = t;
   }
}

void PMDeclare::updateDeclareType( )
{
   PMPart* pPart = part( );
   if( !pPart )
      return;

   PMMetaObject* type = 0;
   PMObject* o = firstChild( );
   PMPrototypeManager* m = pPart->prototypeManager( );
   if( o )
   {
      if( o->isA( "GraphicalObject" ) )
         type = m->tqmetaObject( "GraphicalObject" );
      else
         type = o->tqmetaObject( );
   }
   setDeclareType( type );
}

void PMDeclare::restoreMemento( PMMemento* s )
{
   PMMementoDataIterator it( s );
   PMMementoData* data;

   for( ; it.current( ); ++it )
   {
      data = it.current( );
      if( data->objectType( ) == s_pMetaObject )
      {
         switch( data->valueID( ) )
         {
            case PMIDID:
               setID( data->stringData( ) );
               break;
            default:
               kdError( PMArea ) << "Wrong ID in PMDeclare::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}

void PMDeclare::addLinkedObject( PMObject* o )
{
   m_linkedObjects.append( o );
}

void PMDeclare::removeLinkedObject( PMObject* o )
{
   m_linkedObjects.removeRef( o );
}

void PMDeclare::childAdded( PMObject* o )
{
   if( !m_pDeclareType )
      updateDeclareType( );

   Base::childAdded( o );
}

void PMDeclare::childRemoved( PMObject* o )
{
   if( !firstChild( ) )
      setDeclareType( 0 );

   Base::childRemoved( o );
}
