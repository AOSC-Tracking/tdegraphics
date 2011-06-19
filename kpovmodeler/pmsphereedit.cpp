/*
**************************************************************************

                          pmsphereedit.cpp  -  description
                             -------------------
    begin                : Wed Jun 6 2001
    copyright            : (C) 2001 by Philippe Van Hecke
    email                : lephiloux@tiscalinet.be
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/

#include "pmsphereedit.h"
#include "pmsphere.h"
#include "pmvectoredit.h"
#include "pmlineedits.h"

#include <tqlayout.h>
#include <tqlabel.h>
#include <klocale.h>

PMSphereEdit::PMSphereEdit( TQWidget* tqparent, const char* name )
      : Base( tqparent, name )
{
   m_pDisplayedObject = 0;
}

void PMSphereEdit::createTopWidgets( )
{
   Base::createTopWidgets( );

   TQHBoxLayout* tqlayout;

   m_pCentre = new PMVectorEdit( "x", "y", "z", this );
   m_pRadius = new PMFloatEdit( this );

   tqlayout = new TQHBoxLayout( topLayout( ) );
   tqlayout->addWidget( new TQLabel( i18n( "Center:" ), this ) );
   tqlayout->addWidget( m_pCentre );

   tqlayout = new TQHBoxLayout( topLayout( ) );
   tqlayout->addWidget( new TQLabel( i18n( "Radius:" ), this ) );
   tqlayout->addWidget( m_pRadius );
   tqlayout->addStretch( 1 );

   connect( m_pCentre, TQT_SIGNAL( dataChanged( ) ), TQT_SIGNAL( dataChanged( ) ) );
   connect( m_pRadius, TQT_SIGNAL( dataChanged( ) ), TQT_SIGNAL( dataChanged( ) ) );
}

void PMSphereEdit::displayObject( PMObject* o )
{
   if( o->isA( "Sphere" ) )
   {
      bool readOnly = o->isReadOnly( );
      m_pDisplayedObject = ( PMSphere* ) o;

      m_pCentre->setVector( m_pDisplayedObject->centre( ) );
      m_pRadius->setValue( m_pDisplayedObject->radius( ) );

      m_pCentre->setReadOnly( readOnly );
      m_pRadius->setReadOnly( readOnly );

      Base::displayObject( o );
   }
   else
      kdError( PMArea ) << "PMSphereEdit: Can't display object\n";
}

void PMSphereEdit::saveContents( )
{
   if( m_pDisplayedObject )
   {
      Base::saveContents( );
      m_pDisplayedObject->setCentre( m_pCentre->vector( ) );
      m_pDisplayedObject->setRadius( m_pRadius->value( ) );
   }
}

bool PMSphereEdit::isDataValid( )
{
   if( m_pCentre->isDataValid( ) )
      if( m_pRadius->isDataValid( ) )
         return Base::isDataValid( );
   return false;
}


#include "pmsphereedit.moc"
