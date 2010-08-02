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


#include "pmnamedobjectedit.h"
#include "pmnamedobject.h"

#include <tqlayout.h>
#include <tqlineedit.h>
#include <tqlabel.h>
#include <klocale.h>

PMNamedObjectEdit::PMNamedObjectEdit( TQWidget* parent, const char* name )
      : Base( parent, name )
{
   m_pDisplayedObject = 0;
}

void PMNamedObjectEdit::createTopWidgets( )
{
   Base::createTopWidgets( );

   TQHBoxLayout* layout = new TQHBoxLayout( topLayout( ) );
   m_pNameEdit = new TQLineEdit( this );
   TQLabel* label = new TQLabel( i18n( "Name:" ), this );

   layout->addWidget( label );
   layout->addWidget( m_pNameEdit );

   connect( m_pNameEdit, TQT_SIGNAL( textChanged( const TQString& ) ),
            TQT_SLOT( slotNameChanged( const TQString& ) ) );
}

void PMNamedObjectEdit::displayObject( PMObject* o )
{
   if( o->isA( "NamedObject" ) )
   {
      m_pDisplayedObject = ( PMNamedObject* ) o;
      m_pNameEdit->setText( m_pDisplayedObject->name( ) );

      m_pNameEdit->setReadOnly( m_pDisplayedObject->isReadOnly( ) );
      
      Base::displayObject( o );
   }
   else
      kdError( PMArea ) << "PMNamedObjectEdit: Can't display object\n";
}

void PMNamedObjectEdit::saveContents( )
{
   if( m_pDisplayedObject )
   {
      Base::saveContents( );
      m_pDisplayedObject->setName( m_pNameEdit->text( ) );
   }
}

bool PMNamedObjectEdit::isDataValid( )
{
   return Base::isDataValid( );
}

void PMNamedObjectEdit::slotNameChanged( const TQString& )
{
   emit dataChanged( );
}
#include "pmnamedobjectedit.moc"
