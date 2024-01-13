/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Leon Pennington
    email                : leon@leonscape.co.uk
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#include "pmphotonsedit.h"
#include "pmphotons.h"
#include "pmlineedits.h"

#include <tqlayout.h>
#include <tqlabel.h>
#include <tqcheckbox.h>
#include <tdelocale.h>
#include <kdialog.h>
#include <tdemessagebox.h>


PMPhotonsEdit::PMPhotonsEdit( TQWidget* parent, const char* name )
      : Base( parent, name )
{
   m_pDisplayedObject = 0;
}

void PMPhotonsEdit::createTopWidgets( )
{
   TQGridLayout* gl;
	TQHBoxLayout* hl;

   Base::createTopWidgets( );

   m_pLayoutWidget = new TQWidget( this );
   m_pTarget = new TQCheckBox( i18n( "Target" ), m_pLayoutWidget );
   m_pSpacingMultiLabel = new TQLabel( i18n( "Spacing multiplier:" ), m_pLayoutWidget );
   m_pSpacingMulti = new PMFloatEdit( m_pLayoutWidget );
   m_pSpacingMulti->setValidation( true, 0, false, 0 );

   m_pRefraction = new TQCheckBox( i18n( "Refraction" ), this );
   m_pReflection = new TQCheckBox( i18n( "Reflection" ), this );
   m_pCollect = new TQCheckBox( i18n( "Collect" ), this );
   m_pPassThrough = new TQCheckBox( i18n( "Pass through" ), this );
   m_pAreaLight = new TQCheckBox( i18n( "Area light" ), this );

	hl = new TQHBoxLayout( m_pLayoutWidget, 0, KDialog::spacingHint( ) );
   gl = new TQGridLayout( hl, 2, 2 );
   gl->addMultiCellWidget( m_pTarget, 0, 0, 0, 1 );
   gl->addWidget( m_pSpacingMultiLabel, 1, 0 );
   gl->addWidget( m_pSpacingMulti, 1, 1 );
	hl->addStretch( 1 );
   topLayout( )->addWidget( m_pLayoutWidget );

   gl = new TQGridLayout( topLayout( ), 2, 2 );
   gl->addWidget( m_pRefraction, 0, 0 );
   gl->addWidget( m_pReflection, 0, 1 );
   gl->addWidget( m_pCollect, 1, 0 );
   gl->addWidget( m_pPassThrough, 1, 1 );
   gl->addWidget( m_pAreaLight, 1, 0 );

   connect( m_pTarget, TQ_SIGNAL( clicked( ) ), TQ_SLOT( slotTargetClicked( ) ) );
   connect( m_pSpacingMulti, TQ_SIGNAL( dataChanged( ) ), TQ_SIGNAL( dataChanged( ) ) );
   connect( m_pRefraction, TQ_SIGNAL( clicked( ) ), TQ_SIGNAL( dataChanged( ) ) );
   connect( m_pReflection, TQ_SIGNAL( clicked( ) ), TQ_SIGNAL( dataChanged( ) ) );
   connect( m_pCollect, TQ_SIGNAL( clicked( ) ), TQ_SIGNAL( dataChanged( ) ) );
   connect( m_pPassThrough, TQ_SIGNAL( clicked( ) ), TQ_SIGNAL( dataChanged( ) ) );
   connect( m_pAreaLight, TQ_SIGNAL( clicked( ) ), TQ_SIGNAL( dataChanged( ) ) );
}

void PMPhotonsEdit::displayObject( PMObject* o )
{
   if( o->isA( "Photons" ) )
   {
      bool readOnly =  o->isReadOnly( );
      m_pDisplayedObject = ( PMPhotons* ) o;

      if ( o->parent( ) && ( o->parent( )->isA( "Light" ) ) )
      {
         m_pLayoutWidget->hide( );
         m_pCollect->hide( );
         m_pPassThrough->hide( );
         m_pAreaLight->show( );
      }
      else
      {
         m_pLayoutWidget->show( );
         m_pCollect->show( );
         m_pPassThrough->show( );
         m_pAreaLight->hide( );
      }

      m_pTarget->setChecked( m_pDisplayedObject->target( ) );
      m_pTarget->setEnabled( !readOnly );
      m_pSpacingMulti->setValue( m_pDisplayedObject->spacingMulti( ) );
      m_pSpacingMulti->setReadOnly( readOnly );
      m_pRefraction->setChecked( m_pDisplayedObject->refraction( ) );
      m_pRefraction->setEnabled( !readOnly );
      m_pReflection->setChecked( m_pDisplayedObject->reflection( ) );
      m_pReflection->setEnabled( !readOnly );
      m_pCollect->setChecked( m_pDisplayedObject->collect( ) );
      m_pCollect->setEnabled( !readOnly );
      m_pPassThrough->setChecked( m_pDisplayedObject->passThrough( ) );
      m_pPassThrough->setEnabled( !readOnly );
      m_pAreaLight->setChecked( m_pDisplayedObject->areaLight( ) );
      m_pAreaLight->setEnabled( !readOnly );

      slotTargetClicked( );

      Base::displayObject( o );
   }
   else
      kdError( PMArea ) << "PMPhotonsEdit: Can't display object\n";
}

void PMPhotonsEdit::saveContents( )
{
   if( m_pDisplayedObject )
   {
      Base::saveContents( );
      m_pDisplayedObject->setTarget( m_pTarget->isChecked( ) );
      m_pDisplayedObject->setSpacingMulti( m_pSpacingMulti->value( ) );
      m_pDisplayedObject->setRefraction( m_pRefraction->isChecked( ) );
      m_pDisplayedObject->setReflection( m_pReflection->isChecked( ) );
      m_pDisplayedObject->setCollect( m_pCollect->isChecked( ) );
      m_pDisplayedObject->setPassThrough( m_pPassThrough->isChecked( ) );
      m_pDisplayedObject->setAreaLight( m_pAreaLight->isChecked( ) );
   }
}

bool PMPhotonsEdit::isDataValid( )
{
   if( !m_pSpacingMulti->isDataValid( ) ) return false;

   return Base::isDataValid( );
}

void PMPhotonsEdit::slotTargetClicked( )
{
   if ( m_pTarget->isChecked( ) && m_pTarget->isEnabled( ) )
   {
      m_pSpacingMulti->setEnabled( true );
   }
   else
   {
      m_pSpacingMulti->setEnabled( false );
   }
	emit dataChanged( );
}

#include "pmphotonsedit.moc"
