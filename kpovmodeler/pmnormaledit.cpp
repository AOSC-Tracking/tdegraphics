/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Luis Carvalho
    email                : lpassos@mail.telepac.pt
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#include "pmnormaledit.h"
#include "pmnormal.h"
#include "pmlinkedit.h"
#include "pmlineedits.h"

#include <tqlayout.h>
#include <tqlabel.h>
#include <tqcheckbox.h>
#include <tdelocale.h>


PMNormalEdit::PMNormalEdit( TQWidget* parent, const char* name )
      : Base( parent, name )
{
   m_pDisplayedObject = 0;
}

void PMNormalEdit::createTopWidgets( )
{
   TQHBoxLayout* hl;

   Base::createTopWidgets( );

   hl = new TQHBoxLayout( topLayout( ) );
   m_pBumpSizeCheck = new TQCheckBox( i18n( "Bump size" ), this );
   m_pBumpSizeEdit = new PMFloatEdit( this );
   hl->addWidget( m_pBumpSizeCheck );
   hl->addWidget( m_pBumpSizeEdit );
   hl->addStretch( 1 );

   hl = new TQHBoxLayout( topLayout( ) );
   TQLabel* lbl = new TQLabel( i18n( "Accuracy" ), this );
   m_pAccuracy = new PMFloatEdit( this );
   hl->addWidget( lbl );
   hl->addWidget( m_pAccuracy );
   hl->addStretch( 1 );

   m_pUVMapping = new TQCheckBox( i18n( "UV mapping" ), this );
   topLayout( )->addWidget( m_pUVMapping );

   connect( m_pBumpSizeCheck, TQ_SIGNAL( clicked( ) ), TQ_SLOT( slotBumpSizeClicked( )) );
   connect( m_pBumpSizeEdit, TQ_SIGNAL( dataChanged( ) ), TQ_SIGNAL( dataChanged( )) );
   connect( m_pAccuracy, TQ_SIGNAL( dataChanged( ) ), TQ_SIGNAL( dataChanged( ) ) );
   connect( m_pUVMapping, TQ_SIGNAL( clicked( ) ), TQ_SIGNAL( dataChanged( ) ) );
}

void PMNormalEdit::displayObject( PMObject* o )
{
   if( o->isA( "Normal" ) )
   {
      bool readOnly = o->isReadOnly( );
      m_pDisplayedObject = ( PMNormal* )o;

      m_pBumpSizeCheck->setChecked( m_pDisplayedObject->isBumpSizeEnabled( ) );
      m_pBumpSizeCheck->setEnabled( !readOnly );
      m_pBumpSizeEdit->setValue( m_pDisplayedObject->bumpSize( ) );
      m_pBumpSizeEdit->setReadOnly( readOnly );
      slotBumpSizeClicked( );
      m_pAccuracy->setValue( m_pDisplayedObject->accuracy( ) );
      m_pAccuracy->setReadOnly( readOnly );
      m_pUVMapping->setChecked( m_pDisplayedObject->uvMapping() );
      m_pUVMapping->setEnabled( !readOnly );

      Base::displayObject( o );
   }
   else
      kdError( PMArea ) << "PMNormalEdit: Can't display object\n";
}

void PMNormalEdit::saveContents( )
{
   if( m_pDisplayedObject )
   {
      Base::saveContents( );
      m_pDisplayedObject->enableBumpSize( m_pBumpSizeCheck->isChecked( ) );
      m_pDisplayedObject->setBumpSize( m_pBumpSizeEdit->value( ) );
      m_pDisplayedObject->setAccuracy( m_pAccuracy->value( ) );
      m_pDisplayedObject->setUVMapping( m_pUVMapping->isChecked() );
   }
}

bool PMNormalEdit::isDataValid( )
{
   if( !m_pBumpSizeEdit->isDataValid( ) ) return false;
   if( !m_pAccuracy->isDataValid( ) ) return false;

   return Base::isDataValid( );
}

void PMNormalEdit::slotBumpSizeClicked( )
{
   if( m_pBumpSizeCheck->isChecked( ) )
      m_pBumpSizeEdit->show( );
   else
      m_pBumpSizeEdit->hide( );
   emit sizeChanged( );
   emit dataChanged( );
}

#include "pmnormaledit.moc"
