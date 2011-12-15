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


#include "pmtextedit.h"
#include "pmtext.h"
#include "pmvectoredit.h"
#include "pmlineedits.h"

#include <layout.h>
#include <tqlabel.h>
#include <tqlineedit.h>
#include <tqpushbutton.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kfiledialog.h>

PMTextEdit::PMTextEdit( TQWidget* parent, const char* name )
      : Base( parent, name )
{
   m_pDisplayedObject = 0;
}

void PMTextEdit::createTopWidgets( )
{
   Base::createTopWidgets( );
   
   TQHBoxLayout* hl = new TQHBoxLayout( topLayout( ) );
   hl->addWidget( new TQLabel( i18n( "Font:" ), this ) );
   m_pFont = new TQLineEdit( this );
   hl->addWidget( m_pFont );
   m_pChooseFont = new TQPushButton( this );
   m_pChooseFont->setPixmap( SmallIcon( "fileopen" ) );
   hl->addWidget( m_pChooseFont );

   hl = new TQHBoxLayout( topLayout( ) );
   hl->addWidget( new TQLabel( i18n( "Text:" ), this ) );
   m_pText = new TQLineEdit( this );
   hl->addWidget( m_pText );

   hl = new TQHBoxLayout( topLayout( ) );
   hl->addWidget( new TQLabel( i18n( "Thickness:" ), this ) );
   m_pThickness = new PMFloatEdit( this );
   hl->addWidget( m_pThickness );
   hl->addStretch( 1 );

   hl = new TQHBoxLayout( topLayout( ) );
   hl->addWidget( new TQLabel( i18n( "Offset:" ), this ) );
   m_pOffset = new PMVectorEdit( "x", "y", this );
   hl->addWidget( m_pOffset );
   
   connect( m_pFont, TQT_SIGNAL( textChanged( const TQString& ) ),
            TQT_SLOT( slotTextChanged( const TQString& ) ) );
   connect( m_pChooseFont, TQT_SIGNAL( clicked( ) ),
            TQT_SLOT( slotChooseFont( ) ) );
   connect( m_pText, TQT_SIGNAL( textChanged( const TQString& ) ),
            TQT_SLOT( slotTextChanged( const TQString& ) ) );
   connect( m_pThickness, TQT_SIGNAL( dataChanged( ) ),
            TQT_SIGNAL( dataChanged( ) ) );
   connect( m_pOffset, TQT_SIGNAL( dataChanged( ) ),
            TQT_SIGNAL( dataChanged( ) ) );
}

void PMTextEdit::displayObject( PMObject* o )
{
   if( o->isA( "Text" ) )
   {
      bool readOnly = o->isReadOnly( );
      m_pDisplayedObject = ( PMText* ) o;

      m_pFont->setText( m_pDisplayedObject->font( ) );
      m_pText->setText( m_pDisplayedObject->text( ) );
      m_pThickness->setValue( m_pDisplayedObject->thickness( ) );
      m_pOffset->setVector( m_pDisplayedObject->offset( ) );

      m_pFont->setReadOnly( readOnly );
      m_pChooseFont->setEnabled( !readOnly );
      m_pText->setReadOnly( readOnly );
      m_pThickness->setReadOnly( readOnly );
      m_pOffset->setReadOnly( readOnly );
      
      Base::displayObject( o );
   }
   else
      kdError( PMArea ) << "PMTextEdit: Can't display object\n";
}

void PMTextEdit::saveContents( )
{
   if( m_pDisplayedObject )
   {
      Base::saveContents( );
      m_pDisplayedObject->setFont( m_pFont->text( ) );
      m_pDisplayedObject->setText( m_pText->text( ) );
      m_pDisplayedObject->setThickness( m_pThickness->value( ) );
      m_pDisplayedObject->setOffset( m_pOffset->vector( ) );
   }
}

bool PMTextEdit::isDataValid( )
{
   if( m_pThickness->isDataValid( ) )
      if( m_pOffset->isDataValid( ) )
         return Base::isDataValid( );
   return false;
}

void PMTextEdit::slotTextChanged( const TQString& )
{
   emit dataChanged( );
}

void PMTextEdit::slotChooseFont( )
{
   TQString str = KFileDialog::getOpenFileName( TQString(), TQString() );

   if( !str.isEmpty() )
   {
      m_pFont->setText( str );
      emit dataChanged( );
   }   
}

#include "pmtextedit.moc"
