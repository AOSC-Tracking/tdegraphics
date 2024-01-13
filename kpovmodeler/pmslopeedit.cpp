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


#include "pmslopeedit.h"
#include "pmslope.h"
#include "pmvectoredit.h"
#include "pmlineedits.h"

#include <tqlayout.h>
#include <tqlabel.h>
#include <tqcombobox.h>
#include <tqcheckbox.h>
#include <tdelocale.h>
#include <kdialog.h>


PMSlopeEdit::PMSlopeEdit( TQWidget* parent, const char* name )
      : Base( parent, name )
{
   m_pDisplayedObject = 0;
}

void PMSlopeEdit::createTopWidgets( )
{
   TQHBoxLayout* hl;
   TQGridLayout* gl;

   Base::createTopWidgets( );

   TQLabel* label = new TQLabel( i18n( "Height:" ), this );
   m_pHeightEdit = new PMFloatEdit( this );
   hl = new TQHBoxLayout( topLayout( ) );
   gl = new TQGridLayout( hl, 2, 2 );
   gl->addWidget( label, 0, 0 );
   gl->addWidget( m_pHeightEdit, 0, 1 );
   label = new TQLabel( i18n( "Slope:" ), this );
   m_pSlopeEdit = new PMFloatEdit( this );
   gl->addWidget( label, 1, 0 );
   gl->addWidget( m_pSlopeEdit, 1, 1 );
   hl->addStretch( 1 );
   
   connect( m_pHeightEdit, TQ_SIGNAL( dataChanged( ) ), TQ_SIGNAL( dataChanged( ) ) );
   connect( m_pSlopeEdit, TQ_SIGNAL( dataChanged( ) ), TQ_SIGNAL( dataChanged( ) ) );
}

void PMSlopeEdit::displayObject( PMObject* o )
{
   if( o->isA( "Slope" ) )
   {
      m_pDisplayedObject = ( PMSlope* ) o;
      m_pHeightEdit->setValue( m_pDisplayedObject->height( ) );
      m_pHeightEdit->setReadOnly( m_pDisplayedObject->isReadOnly( ) );
      m_pSlopeEdit->setValue( m_pDisplayedObject->slope( ) );
      m_pSlopeEdit->setReadOnly( m_pDisplayedObject->isReadOnly( ) );
      Base::displayObject( o );
   }
   else
      kdError( PMArea ) << "PMSlopeEdit: Can't display object\n";
}

void PMSlopeEdit::saveContents( )
{
   if( m_pDisplayedObject )
   {
      Base::saveContents( );
      m_pDisplayedObject->setHeight( m_pHeightEdit->value( ) );
      m_pDisplayedObject->setSlope( m_pSlopeEdit->value( ) );
   }
}

bool PMSlopeEdit::isDataValid( )
{
   if( !m_pHeightEdit->isDataValid( ) ||
       !m_pSlopeEdit->isDataValid( ) )
      return false;

   return Base::isDataValid( );
}

#include "pmslopeedit.moc"
