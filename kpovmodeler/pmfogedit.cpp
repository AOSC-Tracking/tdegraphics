/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Luis Carvalho
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


#include "pmfogedit.h"
#include "pmfog.h"
#include "pmlineedits.h"
#include "pmvectoredit.h"
#include "pmcoloredit.h"

#include <tqlayout.h>
#include <tqlabel.h>
#include <tqcheckbox.h>
#include <tqcombobox.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kdialog.h>

PMFogEdit::PMFogEdit( TQWidget* parent, const char* name )
      : Base( parent, name )
{
   m_pDisplayedObject = 0;
}

void PMFogEdit::createTopWidgets( )
{
   TQHBoxLayout* hl;
   TQVBoxLayout* vl;
   TQGridLayout* gl;
   TQLabel* lbl;
   
   Base::createTopWidgets( );

   lbl = new TQLabel( i18n( "Fog type:" ), this );
   m_pFogTypeEdit = new TQComboBox( this );
   m_pFogTypeEdit->insertItem( i18n( "Constant" ) );
   m_pFogTypeEdit->insertItem( i18n( "Ground" ) );
   hl = new TQHBoxLayout( topLayout( ) );
   hl->addWidget( lbl );
   hl->addWidget( m_pFogTypeEdit );
   hl->addStretch( 1 );

   lbl = new TQLabel( i18n( "Distance:" ), this );
   m_pDistance = new PMFloatEdit( this );
   hl = new TQHBoxLayout( topLayout( ) );
   hl->addWidget( lbl );
   hl->addWidget( m_pDistance );
   hl->addStretch( 1 );
   
   lbl = new TQLabel( i18n( "Color:" ), this );
   m_pColor = new PMColorEdit( false, this );
   hl = new TQHBoxLayout( topLayout( ) );
   hl->addWidget( lbl );
   hl->addWidget( m_pColor );
   hl->addStretch( 1 );

   m_pTurbulenceCheck = new TQCheckBox( i18n( "Turbulence" ), this );
   topLayout( )->addWidget( m_pTurbulenceCheck );

   m_pTurbulenceWidget = new TQWidget( this );
   vl = new TQVBoxLayout( m_pTurbulenceWidget, 0, KDialog::spacingHint( ) );
   hl = new TQHBoxLayout( vl );
   lbl = new TQLabel( i18n( "Value: " ), m_pTurbulenceWidget );
   m_pTurbulenceVector = new PMVectorEdit( "x", "y", "z", m_pTurbulenceWidget);
   hl->addWidget( lbl );
   hl->addWidget( m_pTurbulenceVector );
   hl->addStretch( 1 );
   hl = new TQHBoxLayout( vl );
   gl = new TQGridLayout( hl, 4, 2 );
   lbl = new TQLabel( i18n( "Octaves:" ), m_pTurbulenceWidget );
   m_pOctaves = new PMIntEdit( m_pTurbulenceWidget );
   gl->addWidget( lbl, 0, 0 );
   gl->addWidget( m_pOctaves, 0, 1 );
   lbl = new TQLabel( i18n( "Omega:" ), m_pTurbulenceWidget );
   m_pOmega = new PMFloatEdit( m_pTurbulenceWidget );
   gl->addWidget( lbl, 1, 0 );
   gl->addWidget( m_pOmega, 1, 1 );
   lbl = new TQLabel( i18n( "Lambda:" ), m_pTurbulenceWidget );
   m_pLambda = new PMFloatEdit( m_pTurbulenceWidget );
   gl->addWidget( lbl, 2, 0 );
   gl->addWidget( m_pLambda, 2, 1 );
   lbl = new TQLabel( i18n( "Depth:" ), m_pTurbulenceWidget );
   m_pDepth = new PMFloatEdit( m_pTurbulenceWidget );
   gl->addWidget( lbl, 3, 0 );
   gl->addWidget( m_pDepth, 3, 1 );
   hl->addStretch( 1 );
   topLayout( )->addWidget( m_pTurbulenceWidget );

   hl = new TQHBoxLayout( topLayout( ) );
   gl = new TQGridLayout( hl, 2, 2 );
   m_pFogOffsetLabel = new TQLabel( i18n( "Offset: " ), this );
   m_pFogOffset = new PMFloatEdit( this );
   m_pFogAltLabel = new TQLabel( i18n( "Altitude: " ), this );
   m_pFogAlt = new PMFloatEdit( this );
   gl->addWidget( m_pFogOffsetLabel, 0, 0 );
   gl->addWidget( m_pFogOffset, 0, 1 );
   gl->addWidget( m_pFogAltLabel, 1, 0 );
   gl->addWidget( m_pFogAlt, 1, 1 );
   hl->addStretch( 1 );

   hl = new TQHBoxLayout( topLayout( ) );
   m_pUpLabel = new TQLabel( i18n( "Up: " ), this );
   m_pUp = new PMVectorEdit( "x", "y", "z", this );
   hl->addWidget( m_pUpLabel );
   hl->addWidget( m_pUp );

   connect( m_pFogTypeEdit, TQT_SIGNAL( activated( int ) ), TQT_SLOT( slotFogTypeChanged( int ) ) );
   connect( m_pDistance, TQT_SIGNAL( dataChanged( ) ), TQT_SIGNAL( dataChanged( ) ) );
   connect( m_pColor, TQT_SIGNAL( dataChanged( ) ), TQT_SIGNAL( dataChanged( ) ) );
   connect( m_pTurbulenceCheck, TQT_SIGNAL( clicked( ) ), TQT_SLOT( slotTurbulenceClicked( ) ) );
   connect( m_pTurbulenceVector, TQT_SIGNAL( dataChanged( ) ), TQT_SIGNAL( dataChanged( ) ) );
   connect( m_pOctaves, TQT_SIGNAL( dataChanged( ) ), TQT_SIGNAL( dataChanged( ) ) );
   connect( m_pOmega, TQT_SIGNAL( dataChanged( ) ), TQT_SIGNAL( dataChanged( ) ) );
   connect( m_pLambda, TQT_SIGNAL( dataChanged( ) ), TQT_SIGNAL( dataChanged( ) ) );
   connect( m_pDepth, TQT_SIGNAL( dataChanged( ) ), TQT_SIGNAL( dataChanged( ) ) );
   connect( m_pFogOffset, TQT_SIGNAL( dataChanged( ) ), TQT_SIGNAL( dataChanged( ) ) );
   connect( m_pFogAlt, TQT_SIGNAL( dataChanged( ) ), TQT_SIGNAL( dataChanged( ) ) );
   connect( m_pUp, TQT_SIGNAL( dataChanged( ) ), TQT_SIGNAL( dataChanged( ) ) );
}

void PMFogEdit::displayObject( PMObject* o )
{
   if( o->isA( "Fog" ) )
   {
      bool readOnly =  o->isReadOnly( );
      m_pDisplayedObject = ( PMFog* ) o;

      m_pFogTypeEdit->setCurrentItem( m_pDisplayedObject->fogType( ) - 1 );
      m_pFogTypeEdit->setEnabled( !readOnly );
      m_pDistance->setValue( m_pDisplayedObject->distance( ) );
      m_pDistance->setReadOnly( readOnly );
      m_pColor->setColor( m_pDisplayedObject->color( ) );
      m_pColor->setReadOnly( readOnly );
      m_pTurbulenceCheck->setChecked( m_pDisplayedObject->isTurbulenceEnabled( ) );
      m_pTurbulenceCheck->setEnabled( !readOnly );
      m_pTurbulenceVector->setVector( m_pDisplayedObject->valueVector( ) );
      m_pTurbulenceVector->setReadOnly( readOnly );
      m_pOctaves->setValue( m_pDisplayedObject->octaves( ) );
      m_pOctaves->setReadOnly( readOnly );
      m_pOmega->setValue( m_pDisplayedObject->omega( ) );
      m_pOmega->setReadOnly( readOnly );
      m_pLambda->setValue( m_pDisplayedObject->lambda( ) );
      m_pLambda->setReadOnly( readOnly );
      m_pDepth->setValue( m_pDisplayedObject->depth( ) );
      m_pDepth->setReadOnly( readOnly );
      m_pFogOffset->setValue( m_pDisplayedObject->fogOffset( ) );
      m_pFogOffset->setReadOnly( readOnly );
      m_pFogAlt->setValue( m_pDisplayedObject->fogAlt( ) );
      m_pFogAlt->setReadOnly( readOnly );
      m_pUp->setVector( m_pDisplayedObject->up( ) );
      m_pUp->setReadOnly( readOnly );

      slotTurbulenceClicked( );
      slotFogTypeChanged( m_pFogTypeEdit->currentItem( ) );

      Base::displayObject( o );
   }
   else
      kdError( PMArea ) << "PMFogEdit: Can't display object\n";
}

void PMFogEdit::saveContents( )
{
   if( m_pDisplayedObject )
   {
      m_pDisplayedObject->setFogType( m_pFogTypeEdit->currentItem( ) + 1 );
      m_pDisplayedObject->setDistance( m_pDistance->value( ) );
      m_pDisplayedObject->setColor( m_pColor->color( ) );
      m_pDisplayedObject->enableTurbulence( m_pTurbulenceCheck->isChecked( ) );
      m_pDisplayedObject->setValueVector( m_pTurbulenceVector->vector( ) );
      m_pDisplayedObject->setOctaves( m_pOctaves->value( ) );
      m_pDisplayedObject->setOmega( m_pOmega->value( ) );
      m_pDisplayedObject->setLambda( m_pLambda->value( ) );
      m_pDisplayedObject->setDepth( m_pDepth->value( ) );
      m_pDisplayedObject->setFogOffset( m_pFogOffset->value( ) );
      m_pDisplayedObject->setFogAlt( m_pFogAlt->value( ) );
      m_pDisplayedObject->setUp( m_pUp->vector( ) );
      Base::saveContents( );
   }
}

bool PMFogEdit::isDataValid( )
{
   return Base::isDataValid( );
}

void PMFogEdit::slotTurbulenceClicked( )
{
   if( m_pTurbulenceCheck->isChecked( ) )
      m_pTurbulenceWidget->show( );
   else
      m_pTurbulenceWidget->hide( );

   emit dataChanged( );
   emit sizeChanged( );
}

void PMFogEdit::slotFogTypeChanged( int val )
{
   switch( val )
   {
      case 0:  // Constant Fog
         m_pFogOffsetLabel->hide( );
         m_pFogOffset->hide( );
         m_pFogAltLabel->hide( );
         m_pFogAlt->hide( );
         m_pUpLabel->hide( );
         m_pUp->hide( );
         break;
      case 1:   // Ground Fog
         m_pFogOffsetLabel->show( );
         m_pFogOffset->show( );
         m_pFogAltLabel->show( );
         m_pFogAlt->show( );
         m_pUpLabel->show( );
         m_pUp->show( );
         break;
      default:
         break;
   }

   emit dataChanged( );
   emit sizeChanged( );
}

#include "pmfogedit.moc"
