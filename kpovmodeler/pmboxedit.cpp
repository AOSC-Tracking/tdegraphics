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


#include "pmboxedit.h"
#include "pmbox.h"
#include "pmvectoredit.h"

#include <tqlayout.h>
#include <tqlabel.h>
#include <tdelocale.h>

PMBoxEdit::PMBoxEdit( TQWidget* parent, const char* name )
      : Base( parent, name )
{
   m_pDisplayedObject = 0;
}

void PMBoxEdit::createTopWidgets( )
{
   Base::createTopWidgets( );
   
   m_pCorner1 = new PMVectorEdit( "x", "y", "z", this );
   m_pCorner2 = new PMVectorEdit( "x", "y", "z", this );

   TQGridLayout* gl = new TQGridLayout( topLayout( ), 2, 2 );
   gl->addWidget( new TQLabel( i18n( "Corner 1:" ), this ), 0, 0 );
   gl->addWidget( m_pCorner1, 0, 1 );
   gl->addWidget( new TQLabel( i18n( "Corner 2:" ), this ), 1, 0 );
   gl->addWidget( m_pCorner2, 1, 1 );

   connect( m_pCorner1, TQT_SIGNAL( dataChanged( ) ), TQT_SIGNAL( dataChanged( ) ) );
   connect( m_pCorner2, TQT_SIGNAL( dataChanged( ) ), TQT_SIGNAL( dataChanged( ) ) );
}

void PMBoxEdit::displayObject( PMObject* o )
{
   if( o->isA( "Box" ) )
   {
      bool readOnly = o->isReadOnly( );
      m_pDisplayedObject = ( PMBox* ) o;

      m_pCorner1->setVector( m_pDisplayedObject->corner1( ) );
      m_pCorner2->setVector( m_pDisplayedObject->corner2( ) );

      m_pCorner1->setReadOnly( readOnly );
      m_pCorner2->setReadOnly( readOnly );
      
      Base::displayObject( o );
   }
   else
      kdError( PMArea ) << "PMBoxEdit: Can't display object\n";
}

void PMBoxEdit::saveContents( )
{
   if( m_pDisplayedObject )
   {
      Base::saveContents( );
      m_pDisplayedObject->setCorner1( m_pCorner1->vector( ) );
      m_pDisplayedObject->setCorner2( m_pCorner2->vector( ) );
   }
}

bool PMBoxEdit::isDataValid( )
{
   if( m_pCorner1->isDataValid( ) )
      if( m_pCorner2->isDataValid( ) )
         return Base::isDataValid( );
   return false;
}
#include "pmboxedit.moc"
