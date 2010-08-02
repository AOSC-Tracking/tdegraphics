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


#include "pmraw.h"
#include "pmxmlhelper.h"

#include "pmrawedit.h"
#include "pmmemento.h"

#include <klocale.h>
#include <tqtextstream.h>

PMDefinePropertyClass( PMRaw, PMRawProperty );

PMMetaObject* PMRaw::s_pMetaObject = 0;
PMObject* createNewRaw( PMPart* part )
{
   return new PMRaw( part );
}

PMRaw::PMRaw( PMPart* part )
      : Base( part )
{
}

PMRaw::PMRaw( const PMRaw& r )
      : Base( r )
{
   m_code = r.m_code;
}

PMRaw::PMRaw( PMPart* part, const TQString& t )
   : Base( part )
{
   m_code = t;
}

PMRaw::~PMRaw( )
{
}

TQString PMRaw::description( ) const
{
   return i18n( "raw povray" );
}

void PMRaw::setCode( const TQString& code )
{
   if( code != m_code )
   {
      if( m_pMemento )
         m_pMemento->addData( s_pMetaObject, PMCodeID, m_code );
      m_code = code;
   }
}

PMMetaObject* PMRaw::metaObject( ) const
{
   if( !s_pMetaObject )
   {
      s_pMetaObject = new PMMetaObject( "Raw", Base::metaObject( ),
                                        createNewRaw );
      s_pMetaObject->addProperty(
         new PMRawProperty( "code", &PMRaw::setCode, &PMRaw::code ) );
   }
   return s_pMetaObject;
}

void PMRaw::cleanUp( ) const
{
   if( s_pMetaObject )
   {
      delete s_pMetaObject;
      s_pMetaObject = 0;
   }
   Base::cleanUp( );
}

void PMRaw::serialize( TQDomElement& e, TQDomDocument& doc ) const
{
   TQDomText t = doc.createTextNode( m_code );
   e.appendChild( t );
}

void PMRaw::readAttributes( const PMXMLHelper& h )
{
   TQDomNode e = h.element( ).firstChild( );
   if( e.isText( ) )
      m_code = e.toText( ).data( );
}

PMDialogEditBase* PMRaw::editWidget( TQWidget* parent ) const
{
   return new PMRawEdit( parent );
}

void PMRaw::restoreMemento( PMMemento* s )
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
            case PMCodeID:
               setCode( data->stringData( ) );
               break;
            default:
               kdError( PMArea ) << "Wrong ID in PMRaw::restoreMemento\n";
               break;
         }
      }
   }
   Base::restoreMemento( s );
}

