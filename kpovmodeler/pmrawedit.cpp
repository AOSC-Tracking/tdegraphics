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


#include "pmrawedit.h"
#include "pmraw.h"

#include <tqlayout.h>
#include <tqlabel.h>
#include <tqmultilineedit.h>
#include <kglobalsettings.h>
#include <klocale.h>

PMRawEdit::PMRawEdit( TQWidget* parent, const char* name )
      : Base( parent, name )
{
   m_pDisplayedObject = 0;
}

void PMRawEdit::createTopWidgets( )
{
   Base::createTopWidgets( );

   topLayout( )->addWidget( new TQLabel( i18n( "Povray code:" ), this ) );
   m_pEdit = new TQMultiLineEdit( this );
   m_pEdit->setTextFormat( TQt::PlainText );
   m_pEdit->setWordWrap( TQTextEdit::NoWrap );
   m_pEdit->setFont( KGlobalSettings::fixedFont( ) );
   topLayout( )->addWidget( m_pEdit, 2 );
   
   connect( m_pEdit, TQT_SIGNAL( textChanged( ) ), TQT_SIGNAL( dataChanged( ) ) );
}

void PMRawEdit::displayObject( PMObject* o )
{
   if( o->isA( "Raw" ) )
   {
      m_pDisplayedObject = ( PMRaw* ) o;
      m_pEdit->setText( m_pDisplayedObject->code( ) );

      m_pEdit->setReadOnly( o->isReadOnly( ) );
      
      Base::displayObject( o );
   }
   else
      kdError( PMArea ) << "PMRawEdit: Can't display object\n";
}

void PMRawEdit::saveContents( )
{
   if( m_pDisplayedObject )
   {
      Base::saveContents( );
      m_pDisplayedObject->setCode( m_pEdit->text( ) );
   }
}

bool PMRawEdit::isDataValid( )
{
   return Base::isDataValid( );
}

#include "pmrawedit.moc"
