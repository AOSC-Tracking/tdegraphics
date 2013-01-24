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


#include "pmcommentedit.h"
#include "pmcomment.h"

#include <tqlayout.h>
#include <tqmultilineedit.h>
#include <kglobalsettings.h>

PMCommentEdit::PMCommentEdit( TQWidget* parent, const char* name )
      : Base( parent, name )
{
   m_pDisplayedObject = 0;
}

void PMCommentEdit::createTopWidgets( )
{
   Base::createTopWidgets( );
   
   m_pEdit = new TQMultiLineEdit( this );
   m_pEdit->setTextFormat( TQt::PlainText );
   m_pEdit->setWordWrap( TQTextEdit::NoWrap );
   m_pEdit->setFont( TDEGlobalSettings::fixedFont( ) );
   topLayout( )->addWidget( m_pEdit, 2 );
   
   connect( m_pEdit, TQT_SIGNAL( textChanged( ) ), TQT_SIGNAL( dataChanged( ) ) );
}

void PMCommentEdit::displayObject( PMObject* o )
{
   if( o->isA( "Comment" ) )
   {
      m_pDisplayedObject = ( PMComment* ) o;
      m_pEdit->setText( m_pDisplayedObject->text( ) );

      m_pEdit->setReadOnly( o->isReadOnly( ) );
      
      Base::displayObject( o );
   }
   else
      kdError( PMArea ) << "PMCommentEdit: Can't display object\n";
}

void PMCommentEdit::saveContents( )
{
   if( m_pDisplayedObject )
   {
      Base::saveContents( );
      m_pDisplayedObject->setText( m_pEdit->text( ) );
   }
}

bool PMCommentEdit::isDataValid( )
{
   return Base::isDataValid( );
}
#include "pmcommentedit.moc"
