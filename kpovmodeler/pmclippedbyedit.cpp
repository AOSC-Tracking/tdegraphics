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


#include "pmclippedbyedit.h"
#include "pmclippedby.h"

#include <tqlayout.h>
#include <tqlabel.h>
#include <klocale.h>

PMClippedByEdit::PMClippedByEdit( TQWidget* tqparent, const char* name )
      : Base( tqparent, name )
{
   m_pDisplayedObject = 0;
}

void PMClippedByEdit::createTopWidgets( )
{
   Base::createTopWidgets( );

   m_pChildLabel = new TQLabel( i18n( "No child objects" ), this );
   topLayout( )->addWidget( m_pChildLabel );
   m_pBoundedByLabel = new TQLabel( i18n( "(= bounded by)" ), this );
   topLayout( )->addWidget( m_pBoundedByLabel );   
}

void PMClippedByEdit::displayObject( PMObject* o )
{
   if( o->isA( "ClippedBy" ) )
   {
      m_pDisplayedObject = ( PMClippedBy* ) o;

      if( m_pDisplayedObject->boundedBy( ) )
      {
         m_pChildLabel->show( );
         m_pBoundedByLabel->show( );
      }
      else
      {
         m_pChildLabel->hide( );
         m_pBoundedByLabel->hide( );
      }
      
      Base::displayObject( o );
   }
   else
      kdError( PMArea ) << "PMClippedByEdit: Can't display object\n";
}

#include "pmclippedbyedit.moc"
