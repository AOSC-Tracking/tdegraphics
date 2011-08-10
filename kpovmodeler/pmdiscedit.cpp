/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Leonardo Skorianez
    email                : lsk@if.ufrj.br
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#include "pmdiscedit.h"
#include "pmdisc.h"
#include "pmvectoredit.h"
#include "pmlineedits.h"

#include <tqlayout.h>
#include <tqlabel.h>
#include <tqpushbutton.h>
#include <klocale.h>
#include <kmessagebox.h>

PMDiscEdit::PMDiscEdit( TQWidget* parent, const char* name )
      : Base( parent, name )
{
   m_pDisplayedObject = 0;
}

void PMDiscEdit::createTopWidgets( )
{
   Base::createTopWidgets( );

   TQHBoxLayout* tqlayout;
   TQGridLayout* gl;

   m_pCenter = new PMVectorEdit( "x", "y", "z", this );
   m_pNormal = new PMVectorEdit( "x", "y", "z", this );
   m_pHRadius = new PMFloatEdit( this );
   m_pHRadius->setValidation( true, 0.0, false, 0.0 );
   m_pRadius = new PMFloatEdit( this );
   m_pRadius->setValidation( true, 0.0, false, 0.0 );

   gl = new TQGridLayout( topLayout( ), 2, 2 );
   gl->addWidget( new TQLabel( i18n( "Center:" ), this ), 0, 0 );
   gl->addWidget( m_pCenter, 0, 1 );
   gl->addWidget( new TQLabel( i18n( "Normal:" ), this ), 1, 0 );
   gl->addWidget( m_pNormal, 1, 1 );
   
   tqlayout = new TQHBoxLayout( topLayout( ) );
   gl = new TQGridLayout( tqlayout, 2, 2 );
   gl->addWidget( new TQLabel( i18n( "Radius:" ), this ), 0, 0 );
   gl->addWidget( m_pRadius, 0, 1 );
   gl->addWidget( new TQLabel( i18n( "Hole radius:" ), this ), 1, 0 );
   gl->addWidget( m_pHRadius, 1, 1 );
   tqlayout->addStretch( 1 );

   TQPushButton* nb = new TQPushButton( i18n( "Normalize" ), this );
   tqlayout = new TQHBoxLayout( topLayout( ) );
   tqlayout->addWidget( nb );
   tqlayout->addStretch( 1 );

   connect( m_pCenter, TQT_SIGNAL( dataChanged( ) ), TQT_SIGNAL( dataChanged( ) ) );
   connect( m_pNormal, TQT_SIGNAL( dataChanged( ) ), TQT_SIGNAL( dataChanged( ) ) );
   connect( m_pRadius, TQT_SIGNAL( dataChanged( ) ), TQT_SIGNAL( dataChanged( ) ) );
   connect( m_pHRadius, TQT_SIGNAL( dataChanged( ) ), TQT_SIGNAL( dataChanged( ) ) );
   connect( nb, TQT_SIGNAL( clicked( ) ), TQT_SLOT( slotNormalize( ) ) );
}

void PMDiscEdit::slotNormalize( )
{
   PMVector normal = m_pNormal->vector( );
   double l = normal.abs( );
   if( !approxZero( l ) )
      m_pNormal->setVector( normal / l );
}


void PMDiscEdit::displayObject( PMObject* o )
{
   if( o->isA( "Disc" ) )
   {
      bool readOnly = o->isReadOnly( );
      m_pDisplayedObject = ( PMDisc* ) o;

      m_pCenter->setVector( m_pDisplayedObject->center( ) );
      m_pNormal->setVector( m_pDisplayedObject->normal( ) );
      m_pRadius->setValue( m_pDisplayedObject->radius( ) );
      m_pHRadius->setValue( m_pDisplayedObject->holeRadius( ) );

      m_pCenter->setReadOnly( readOnly );
      m_pNormal->setReadOnly( readOnly );
      m_pRadius->setReadOnly( readOnly );
      m_pHRadius->setReadOnly( readOnly );

      Base::displayObject( o );
   }
   else
      kdError( PMArea ) << "PMDiscEdit: Can't display object\n";
}

void PMDiscEdit::saveContents( )
{
   if( m_pDisplayedObject )
   {
      Base::saveContents( );
      m_pDisplayedObject->setCenter( m_pCenter->vector( ) );
      m_pDisplayedObject->setNormal( m_pNormal->vector( ) );
      m_pDisplayedObject->setRadius( m_pRadius->value( ) );
      m_pDisplayedObject->setHoleRadius( m_pHRadius->value( ) );
   }
}

bool PMDiscEdit::isDataValid( )
{
   if( m_pNormal->isDataValid( ) )
   {
      if( approxZero( m_pNormal->vector( ).abs( ) ) )
      {
         KMessageBox::error( this, i18n( "The normal vector may not be a "
                                         "null vector." ),
                             i18n( "Error" ) );
         return false;
      }
      if( m_pCenter->isDataValid( ) )
         if( m_pRadius->isDataValid( ) )
            if( m_pHRadius->isDataValid( ) )
               if( m_pRadius->value( ) >= m_pHRadius->value( ) )
                  return Base::isDataValid( );
               else
               {
                  KMessageBox::error( this, i18n( "The radius may not be smaller "
                                                  "than the hole radius." ),
                                      i18n( "Error" ) );
                  m_pRadius->setFocus( );
               }
   }
   return false;
}

#include  "pmdiscedit.moc"
