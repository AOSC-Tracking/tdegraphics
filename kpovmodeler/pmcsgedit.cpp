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


#include "pmcsgedit.h"
#include "pmcsg.h"

#include <tqlayout.h>
#include <tqlabel.h>
#include <tqcombobox.h>
#include <klocale.h>

PMCSGEdit::PMCSGEdit( TQWidget* parent, const char* name )
      : Base( parent, name )
{
   m_pDisplayedObject = 0;
}

void PMCSGEdit::createTopWidgets( )
{
   Base::createTopWidgets( );
   
   TQHBoxLayout* tqlayout;
   m_pTypeCombo = new TQComboBox( false, this );
   m_pTypeCombo->insertItem( i18n( "Union" ) );
   m_pTypeCombo->insertItem( i18n( "Intersection" ) );
   m_pTypeCombo->insertItem( i18n( "Difference" ) );
   m_pTypeCombo->insertItem( i18n( "Merge" ) );
   
   tqlayout = new TQHBoxLayout( topLayout( ) );
   tqlayout->addWidget( new TQLabel( i18n( "Type:" ), this ) );
   tqlayout->addWidget( m_pTypeCombo );
   tqlayout->addStretch( 1 );

   connect( m_pTypeCombo, TQT_SIGNAL( activated( int ) ), TQT_SLOT( slotTypeSelected( int ) ) );
}

void PMCSGEdit::displayObject( PMObject* o )
{
   if( o->isA( "CSG" ) )
   {
      bool readOnly = o->isReadOnly( );
      m_pDisplayedObject = ( PMCSG* ) o;

      switch( m_pDisplayedObject->csgType( ) )
      {
         case PMCSG::CSGUnion:
            m_pTypeCombo->setCurrentItem( 0 );
            break;
         case PMCSG::CSGIntersection:
            m_pTypeCombo->setCurrentItem( 1 );
            break;
         case PMCSG::CSGDifference:
            m_pTypeCombo->setCurrentItem( 2 );
            break;
         case PMCSG::CSGMerge:
            m_pTypeCombo->setCurrentItem( 3 );
            break;
      }
      
      m_pTypeCombo->setEnabled( !readOnly );
      
      Base::displayObject( o );
   }
   else
      kdError( PMArea ) << "PMCSGEdit: Can't display object\n";
}

void PMCSGEdit::saveContents( )
{
   if( m_pDisplayedObject )
   {
      Base::saveContents( );
      switch( m_pTypeCombo->currentItem( ) )
      {
         case 0:
            m_pDisplayedObject->setCSGType( PMCSG::CSGUnion );
            break;
         case 1:
            m_pDisplayedObject->setCSGType( PMCSG::CSGIntersection );
            break;
         case 2:
            m_pDisplayedObject->setCSGType( PMCSG::CSGDifference );
            break;
         case 3:
            m_pDisplayedObject->setCSGType( PMCSG::CSGMerge );
            break;
         default:
            m_pDisplayedObject->setCSGType( PMCSG::CSGUnion );
            break;
      }
   }
}

bool PMCSGEdit::isDataValid( )
{
   return Base::isDataValid( );
}

void PMCSGEdit::slotTypeSelected( int )
{
   emit dataChanged( );
}
#include "pmcsgedit.moc"
