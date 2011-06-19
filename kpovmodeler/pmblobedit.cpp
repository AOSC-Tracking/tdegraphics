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


#include "pmblobedit.h"
#include "pmblob.h"
#include "pmlineedits.h"

#include <tqlayout.h>
#include <tqlabel.h>
#include <tqcheckbox.h>
#include <klocale.h>

PMBlobEdit::PMBlobEdit( TQWidget* tqparent, const char* name )
      : Base( tqparent, name )
{
   m_pDisplayedObject = 0;
}

void PMBlobEdit::createTopWidgets( )
{
   Base::createTopWidgets( );

   TQHBoxLayout* hl = new TQHBoxLayout( topLayout( ) );
   hl->addWidget( new TQLabel( i18n( "Threshold:" ), this ) );
   m_pThreshold = new PMFloatEdit( this );
   hl->addWidget( m_pThreshold );
   m_pThreshold->setValidation( true, 0.0, false, 0 );
   m_pThreshold->setValidationOperator( PMFloatEdit::OpGreater,
                                        PMFloatEdit::OpLess );
   hl->addStretch( 1 );

   m_pSturm = new TQCheckBox( i18n( "Sturm" ), this );
   topLayout( )->addWidget( m_pSturm );
   m_pHierarchy = new TQCheckBox( i18n( "Hierarchy" ), this );
   topLayout( )->addWidget( m_pHierarchy );
   
   connect( m_pThreshold, TQT_SIGNAL( dataChanged( ) ), TQT_SIGNAL( dataChanged( ) ) );
   connect( m_pHierarchy, TQT_SIGNAL( clicked( ) ), TQT_SIGNAL( dataChanged( ) ) );
   connect( m_pSturm, TQT_SIGNAL( clicked( ) ), TQT_SIGNAL( dataChanged( ) ) );
}

void PMBlobEdit::displayObject( PMObject* o )
{
   if( o->isA( "Blob" ) )
   {
      bool readOnly = o->isReadOnly( );
      m_pDisplayedObject = ( PMBlob* ) o;

      m_pThreshold->setValue( m_pDisplayedObject->threshold( ) );
      m_pSturm->setChecked( m_pDisplayedObject->sturm( ) );
      m_pHierarchy->setChecked( m_pDisplayedObject->hierarchy( ) );

      m_pThreshold->setReadOnly( readOnly );
      m_pSturm->setEnabled( !readOnly );
      m_pHierarchy->setEnabled( !readOnly );
      
      Base::displayObject( o );
   }
   else
      kdError( PMArea ) << "PMBlobEdit: Can't display object\n";
}

void PMBlobEdit::saveContents( )
{
   if( m_pDisplayedObject )
   {
      Base::saveContents( );
      m_pDisplayedObject->setThreshold( m_pThreshold->value( ) );
      m_pDisplayedObject->setSturm( m_pSturm->isChecked( ) );
      m_pDisplayedObject->setHierarchy( m_pHierarchy->isChecked( ) );
   }
}

bool PMBlobEdit::isDataValid( )
{
   if( m_pThreshold->isDataValid( ) )
      return Base::isDataValid( );
   return false;
}

#include "pmblobedit.moc"
