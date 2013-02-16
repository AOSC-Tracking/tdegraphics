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

#include "pmgridsettings.h"

#include "pmlineedits.h"
#include "pmrendermanager.h"
#include "pmcontrolpoint.h"
#include "pmdefaults.h"

#include <tqlayout.h>
#include <tqgroupbox.h>
#include <tqlabel.h>
#include <kcolorbutton.h>
#include <tdelocale.h>

PMGridSettings::PMGridSettings( TQWidget* parent, const char* name )
      : PMSettingsDialogPage( parent, name )
{
   TQHBoxLayout* hlayout;
   TQVBoxLayout* vlayout;
   TQVBoxLayout* gvl;
   TQGridLayout* grid;
   TQGroupBox* gb;

   vlayout = new TQVBoxLayout( this, 0, KDialog::spacingHint( ) );
   gb = new TQGroupBox( i18n( "Displayed Grid" ), this );
   vlayout->addWidget( gb );
   gvl = new TQVBoxLayout( gb, KDialog::marginHint( ), KDialog::spacingHint( ) );
   gvl->addSpacing( 10 );

   hlayout = new TQHBoxLayout( gvl );
   hlayout->addWidget( new TQLabel( i18n( "Color:" ), gb ) );
   m_pGridColor = new KColorButton( gb );
   hlayout->addWidget( m_pGridColor );
   hlayout->addStretch( 1 );

   hlayout = new TQHBoxLayout( gvl );
   hlayout->addWidget( new TQLabel( i18n( "Distance:" ), gb ) );
   m_pGridDistance = new PMIntEdit( gb );
   m_pGridDistance->setValidation( true, 20, false, 0 );
   hlayout->addWidget( m_pGridDistance );
   hlayout->addStretch( 1 );

   gb = new TQGroupBox( i18n( "Control Point Grid" ), this );
   vlayout->addWidget( gb );
   gvl = new TQVBoxLayout( gb, KDialog::marginHint( ), KDialog::spacingHint( ) );
   gvl->addSpacing( 10 );

   hlayout = new TQHBoxLayout( gvl );
   grid = new TQGridLayout( hlayout, 3, 2 );
   grid->addWidget( new TQLabel( i18n( "2D/3D movement:" ), gb ), 0, 0 );
   m_pMoveGrid = new PMFloatEdit( gb );
   m_pMoveGrid->setValidation( true, 0.001, true, 100 );
   grid->addWidget( m_pMoveGrid, 0, 1 );
   
   grid->addWidget( new TQLabel( i18n( "Scale:" ), gb ), 1, 0 );
   m_pScaleGrid = new PMFloatEdit( gb );
   m_pScaleGrid->setValidation( true, 0.001, true, 100 );
   grid->addWidget( m_pScaleGrid, 1, 1 );
   
   grid->addWidget( new TQLabel( i18n( "Rotation:" ), gb ), 2, 0 );
   m_pRotateGrid = new PMFloatEdit( gb );
   m_pRotateGrid->setValidation( true, 0.001, true, 180 );
   grid->addWidget( m_pRotateGrid, 2, 1 );

   hlayout->addStretch( 1 );

   vlayout->addStretch( 1 );
}

void PMGridSettings::displaySettings( )
{
   PMRenderManager* rm = PMRenderManager::theManager( );
   m_pGridColor->setColor( rm->gridColor( ) );
   m_pGridDistance->setValue( rm->gridDistance( ) );
   m_pMoveGrid->setValue( PMControlPoint::moveGrid( ) );
   m_pScaleGrid->setValue( PMControlPoint::scaleGrid( ) );
   m_pRotateGrid->setValue( PMControlPoint::rotateGrid( ) );
}

void PMGridSettings::displayDefaults( )
{
   m_pGridColor->setColor( c_defaultGridColor );
   m_pGridDistance->setValue( c_defaultGridDistance );
   m_pMoveGrid->setValue( c_defaultMoveGrid );
   m_pScaleGrid->setValue( c_defaultScaleGrid );
   m_pRotateGrid->setValue( c_defaultRotateGrid );      
}

bool PMGridSettings::validateData( )
{
   if( !m_pGridDistance->isDataValid( ) )
   {
      emit showMe( );
      m_pGridDistance->setFocus( );
      return false;
   }
   if( !m_pMoveGrid->isDataValid( ) )
   {
      emit showMe( );
      m_pMoveGrid->setFocus( );
      return false;
   }
   if( !m_pScaleGrid->isDataValid( ) )
   {
      emit showMe( );
      m_pScaleGrid->setFocus( );
      return false;
   }
   if( !m_pRotateGrid->isDataValid( ) )
   {
      emit showMe( );
      m_pRotateGrid->setFocus( );
      return false;
   }
   return true;
}

void PMGridSettings::applySettings( )
{
   bool repaint = false;
   PMRenderManager* rm = PMRenderManager::theManager( );
   if( rm->gridColor( ) != m_pGridColor->color( ) )
   {
      rm->setGridColor( m_pGridColor->color( ) );
      repaint = true;
   }
   if( rm->gridDistance( ) != m_pGridDistance->value( ) )
   {
      rm->setGridDistance( m_pGridDistance->value( ) );
      repaint = true;
   }
   PMControlPoint::setMoveGrid( m_pMoveGrid->value( ) );
   PMControlPoint::setScaleGrid( m_pScaleGrid->value( ) );
   PMControlPoint::setRotateGrid( m_pRotateGrid->value( ) );
   if( repaint )
      emit repaintViews( );
}

#include "pmgridsettings.moc"
