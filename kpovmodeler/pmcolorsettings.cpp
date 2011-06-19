/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Andreas Zehender
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

#include "pmcolorsettings.h"

#include "pmrendermanager.h"
#include "pmdefaults.h"

#include <tqlayout.h>
#include <tqlabel.h>
#include <kcolorbutton.h>
#include <klocale.h>

PMColorSettings::PMColorSettings( TQWidget* tqparent, const char* name )
      : PMSettingsDialogPage( tqparent, name )
{
   TQHBoxLayout* htqlayout;
   TQVBoxLayout* vtqlayout;
   TQGridLayout* grid;

   vtqlayout = new TQVBoxLayout( this, 0, KDialog::spacingHint( ) );
   grid = new TQGridLayout( vtqlayout, 6, 3 );
   
   grid->addWidget( new TQLabel( i18n( "Background:" ), this ), 0, 0 );
   htqlayout = new TQHBoxLayout( );
   grid->addLayout( htqlayout, 0, 2 );
   m_pBackgroundColor = new KColorButton( this );
   htqlayout->addWidget( m_pBackgroundColor );
   htqlayout->addStretch( 1 );

   grid->addWidget( new TQLabel( i18n( "Wire frame:" ), this ), 1, 0 );
   htqlayout = new TQHBoxLayout( );
   grid->addLayout( htqlayout, 1, 2 );
   m_pGraphicalObjectsColor[0] = new KColorButton( this );
   htqlayout->addWidget( m_pGraphicalObjectsColor[0] );
   htqlayout->addWidget( new TQLabel( i18n( "Selected:" ), this ) );
   m_pGraphicalObjectsColor[1] = new KColorButton( this );
   htqlayout->addWidget( m_pGraphicalObjectsColor[1] );
   htqlayout->addStretch( 1 );
   
   grid->addWidget( new TQLabel( i18n( "Control points:" ), this ), 2, 0 );
   htqlayout = new TQHBoxLayout( );
   grid->addLayout( htqlayout, 2, 2 );
   m_pControlPointsColor[0] = new KColorButton( this );
   htqlayout->addWidget( m_pControlPointsColor[0] );
   htqlayout->addWidget( new TQLabel( i18n( "Selected:" ), this ) );
   m_pControlPointsColor[1] = new KColorButton( this );
   htqlayout->addWidget( m_pControlPointsColor[1] );
   htqlayout->addStretch( 1 );

   grid->addWidget( new TQLabel( i18n( "Axes:" ), this ), 3, 0 );
   htqlayout = new TQHBoxLayout( );
   grid->addLayout( htqlayout, 3, 2 );
   grid->addWidget( new TQLabel( "x", this ), 3, 1 );
   m_pAxesColor[0] = new KColorButton( this );
   htqlayout->addWidget( m_pAxesColor[0] );
   htqlayout->addWidget( new TQLabel( "y", this ) );
   m_pAxesColor[1] = new KColorButton( this );
   htqlayout->addWidget( m_pAxesColor[1] );
   htqlayout->addWidget( new TQLabel( "z", this ) );
   m_pAxesColor[2] = new KColorButton( this );
   htqlayout->addWidget( m_pAxesColor[2] );
   htqlayout->addStretch( 1 );

   grid->addWidget( new TQLabel( i18n( "Field of view:" ), this ), 4, 0 );
   htqlayout = new TQHBoxLayout( );
   grid->addLayout( htqlayout, 4, 2 );
   m_pFieldOfViewColor = new KColorButton( this );
   htqlayout->addWidget( m_pFieldOfViewColor );
   htqlayout->addStretch( 1 );
}

void PMColorSettings::displaySettings( )
{
   PMRenderManager* rm = PMRenderManager::theManager( );
   m_pBackgroundColor->setColor( rm->backgroundColor( ) );
   m_pGraphicalObjectsColor[0]->setColor( rm->graphicalObjectColor( 0 ) );
   m_pGraphicalObjectsColor[1]->setColor( rm->graphicalObjectColor( 1 ) );
   m_pControlPointsColor[0]->setColor( rm->controlPointColor( 0 ) );
   m_pControlPointsColor[1]->setColor( rm->controlPointColor( 1 ) );
   m_pAxesColor[0]->setColor( rm->axesColor( 0 ) );
   m_pAxesColor[1]->setColor( rm->axesColor( 1 ) );
   m_pAxesColor[2]->setColor( rm->axesColor( 2 ) );
   m_pFieldOfViewColor->setColor( rm->fieldOfViewColor( ) );
}

void PMColorSettings::displayDefaults( )
{
   m_pBackgroundColor->setColor( c_defaultBackgroundColor );
   m_pGraphicalObjectsColor[0]->setColor( c_defaultGraphicalObjectColor0 );
   m_pGraphicalObjectsColor[1]->setColor( c_defaultGraphicalObjectColor1 );
   m_pControlPointsColor[0]->setColor( c_defaultControlPointColor0 );
   m_pControlPointsColor[1]->setColor( c_defaultControlPointColor1 );
   m_pAxesColor[0]->setColor( c_defaultAxesColorX );
   m_pAxesColor[1]->setColor( c_defaultAxesColorY );
   m_pAxesColor[2]->setColor( c_defaultAxesColorZ );
   m_pFieldOfViewColor->setColor( c_defaultFieldOfViewColor );
}

bool PMColorSettings::validateData( )
{
   return true;
}

void PMColorSettings::applySettings( )
{
   bool tqrepaint = false;
   int i;
   
   PMRenderManager* rm = PMRenderManager::theManager( );
   if( rm->backgroundColor( ) != m_pBackgroundColor->color( ) )
   {
      rm->setBackgroundColor( m_pBackgroundColor->color( ) );
      tqrepaint = true;
   }
   for( i = 0; i < 2; i++ )
   {
      if( rm->graphicalObjectColor( i ) != m_pGraphicalObjectsColor[i]->color( ) )
      {
         rm->setGraphicalObjectColor( i, m_pGraphicalObjectsColor[i]->color( ) );
         tqrepaint = true;
      }
   }
   for( i = 0; i < 2; i++ )
   {
      if( rm->controlPointColor( i ) != m_pControlPointsColor[i]->color( ) )
      {
         rm->setControlPointColor( i, m_pControlPointsColor[i]->color( ) );
         tqrepaint = true;
      }
   }
   for( i = 0; i < 3; i++ )
   {
      if( rm->axesColor( i ) != m_pAxesColor[i]->color( ) )
      {
         rm->setAxesColor( i, m_pAxesColor[i]->color( ) );
         tqrepaint = true;
      }
   }
   if( rm->fieldOfViewColor( ) != m_pFieldOfViewColor->color( ) )
   {
      rm->setFieldOfViewColor( m_pFieldOfViewColor->color( ) );
      tqrepaint = true;
   }
   if( tqrepaint )
      emit tqrepaintViews( );
}

#include "pmcolorsettings.moc"
