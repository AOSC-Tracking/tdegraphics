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


#include "pmheightfieldedit.h"
#include "pmheightfield.h"
#include "pmlineedits.h"

#include <tqlayout.h>
#include <tqlabel.h>
#include <tqcheckbox.h>
#include <tqcombobox.h>
#include <tqlineedit.h>
#include <tqpushbutton.h>
#include <tqslider.h>
#include <tdelocale.h>
#include <kiconloader.h>
#include <tdefiledialog.h>

PMHeightFieldEdit::PMHeightFieldEdit( TQWidget* parent, const char* name )
      : Base( parent, name )
{
   m_pDisplayedObject = 0;
}

void PMHeightFieldEdit::createTopWidgets( )
{
   Base::createTopWidgets( );

   TQHBoxLayout* hl = new TQHBoxLayout( topLayout( ) );
   hl->addWidget( new TQLabel( i18n( "Type:" ), this ) );
   m_pHeightFieldType = new TQComboBox( false, this );
   hl->addWidget( m_pHeightFieldType );
   hl->addStretch( 0 );
   m_pHeightFieldType->insertItem( "gif" );
   m_pHeightFieldType->insertItem( "tga" );
   m_pHeightFieldType->insertItem( "pot" );
   m_pHeightFieldType->insertItem( "png" );
   m_pHeightFieldType->insertItem( "pgm" );
   m_pHeightFieldType->insertItem( "ppm" );
   m_pHeightFieldType->insertItem( "sys" );

   hl = new TQHBoxLayout( topLayout( ) );
   hl->addWidget( new TQLabel( i18n( "File name:" ), this ) );
   m_pFileName = new TQLineEdit( this );
   hl->addWidget( m_pFileName );
   m_pChooseFileName = new TQPushButton( this );
   m_pChooseFileName->setPixmap( SmallIcon( "document-open" ) );
   hl->addWidget( m_pChooseFileName );

   hl = new TQHBoxLayout( topLayout( ) );
   hl->addWidget( new TQLabel( i18n( "Water level:" ), this ) );
   m_pWaterLevel = new PMFloatEdit( this );
   m_pWaterLevel->setValidation( true, 0.0, true, 1.0 );
   hl->addWidget( m_pWaterLevel );
   hl->addStretch( 1 );

   m_pHierarchy = new TQCheckBox( i18n( "Hierarchy" ), this );
   topLayout( )->addWidget( m_pHierarchy );

   m_pSmooth = new TQCheckBox( i18n( "Smooth" ), this );
   topLayout( )->addWidget( m_pSmooth );

   connect( m_pHeightFieldType, TQ_SIGNAL( activated( int ) ),
            TQ_SLOT( slotTypeChanged( int ) ) );
   connect( m_pFileName, TQ_SIGNAL( textChanged( const TQString& ) ),
            TQ_SLOT( slotFileNameChanged( const TQString& ) ) );
   connect( m_pChooseFileName, TQ_SIGNAL( clicked( ) ),
            TQ_SLOT( slotFileNameClicked( ) ) );
   connect( m_pWaterLevel, TQ_SIGNAL( dataChanged( ) ), TQ_SIGNAL( dataChanged( ) ) );
   connect( m_pHierarchy, TQ_SIGNAL( clicked( ) ), TQ_SIGNAL( dataChanged( ) ) );
   connect( m_pSmooth, TQ_SIGNAL( clicked( ) ), TQ_SIGNAL( dataChanged( ) ) );
}

void PMHeightFieldEdit::displayObject( PMObject* o )
{
   if( o->isA( "HeightField" ) )
   {
      bool readOnly = o->isReadOnly( );
      m_pDisplayedObject = ( PMHeightField* ) o;

      switch( m_pDisplayedObject->heightFieldType( ) )
      {
         case PMHeightField::HFgif:
            m_pHeightFieldType->setCurrentItem( 0 );
            break;
         case PMHeightField::HFtga:
            m_pHeightFieldType->setCurrentItem( 1 );
            break;
         case PMHeightField::HFpot:
            m_pHeightFieldType->setCurrentItem( 2 );
            break;
         case PMHeightField::HFpng:
            m_pHeightFieldType->setCurrentItem( 3 );
            break;
         case PMHeightField::HFpgm:
            m_pHeightFieldType->setCurrentItem( 4 );
            break;
         case PMHeightField::HFppm:
            m_pHeightFieldType->setCurrentItem( 5 );
            break;
         case PMHeightField::HFsys:
            m_pHeightFieldType->setCurrentItem( 6 );
            break;
      }
      m_pFileName->setText( m_pDisplayedObject->fileName( ) );
      m_pWaterLevel->setValue( m_pDisplayedObject->waterLevel( ) );
      m_pHierarchy->setChecked( m_pDisplayedObject->hierarchy( ) );
      m_pSmooth->setChecked( m_pDisplayedObject->smooth( ) );

      m_pHeightFieldType->setEnabled( !readOnly );
      m_pFileName->setReadOnly( readOnly );
      m_pChooseFileName->setEnabled( !readOnly );
      m_pHierarchy->setEnabled( !readOnly );
      m_pSmooth->setEnabled( !readOnly );

      Base::displayObject( o );
   }
   else
      kdError( PMArea ) << "PMHeightFieldEdit: Can't display object\n";
}

void PMHeightFieldEdit::saveContents( )
{
   if( m_pDisplayedObject )
   {
      Base::saveContents( );
      switch( m_pHeightFieldType->currentItem( ) )
      {
         case 0:
            m_pDisplayedObject->setHeightFieldType( PMHeightField::HFgif );
            break;
         case 1:
            m_pDisplayedObject->setHeightFieldType( PMHeightField::HFtga );
            break;
         case 2:
            m_pDisplayedObject->setHeightFieldType( PMHeightField::HFpot );
            break;
         case 3:
            m_pDisplayedObject->setHeightFieldType( PMHeightField::HFpng );
            break;
         case 4:
            m_pDisplayedObject->setHeightFieldType( PMHeightField::HFpgm );
            break;
         case 5:
            m_pDisplayedObject->setHeightFieldType( PMHeightField::HFppm );
            break;
         case 6:
            m_pDisplayedObject->setHeightFieldType( PMHeightField::HFsys );
            break;
      }
      m_pDisplayedObject->setFileName( m_pFileName->text( ) );
      m_pDisplayedObject->setWaterLevel( m_pWaterLevel->value( ) );
      m_pDisplayedObject->setHierarchy( m_pHierarchy->isChecked( ) );
      m_pDisplayedObject->setSmooth( m_pSmooth->isChecked( ) );
   }
}

bool PMHeightFieldEdit::isDataValid( )
{
   if( m_pWaterLevel->isDataValid( ) )
      return Base::isDataValid( );
   return false;
}

void PMHeightFieldEdit::slotTypeChanged( int )
{
   emit dataChanged( );
}

void PMHeightFieldEdit::slotFileNameChanged( const TQString& )
{
   emit dataChanged( );
}

void PMHeightFieldEdit::slotFileNameClicked( )
{
   TQString str = KFileDialog::getOpenFileName( TQString(), TQString() );

   if( !str.isEmpty() )
   {
      m_pFileName->setText( str );
      emit dataChanged( );
   }
}

#include "pmheightfieldedit.moc"
