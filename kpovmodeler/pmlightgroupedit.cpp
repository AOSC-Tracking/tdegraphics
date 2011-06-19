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


#include "pmlightgroupedit.h"
#include "pmlightgroup.h"

#include <tqlayout.h>
#include <tqlabel.h>
#include <tqcheckbox.h>
#include <klocale.h>

PMLightGroupEdit::PMLightGroupEdit( TQWidget* tqparent, const char* name )
      : Base( tqparent, name )
{
   m_pDisplayedObject = 0;
}

void PMLightGroupEdit::createTopWidgets( )
{
   Base::createTopWidgets( );

   TQHBoxLayout* tqlayout;
   m_pGlobalLights = new TQCheckBox( i18n( "Global lights" ), this );

   tqlayout = new TQHBoxLayout( topLayout( ) );
   tqlayout->addWidget( m_pGlobalLights );
   tqlayout->addStretch( 1 );

   connect( m_pGlobalLights, TQT_SIGNAL( clicked( ) ), TQT_SIGNAL( dataChanged( ) ) );
}

void PMLightGroupEdit::displayObject( PMObject* o )
{
   if( o->isA( "LightGroup" ) )
   {
      bool readOnly = o->isReadOnly( );
      m_pDisplayedObject = ( PMLightGroup* ) o;

      m_pGlobalLights->setChecked( m_pDisplayedObject->globalLights( ) );
      m_pGlobalLights->setEnabled( !readOnly );
      
      Base::displayObject( o );
   }
   else
      kdError( PMArea ) << "PMLightGroupEdit: Can't display object\n";
}

void PMLightGroupEdit::saveContents( )
{
   if( m_pDisplayedObject )
   {
      Base::saveContents( );

      m_pDisplayedObject->setGlobalLights( m_pGlobalLights->isChecked( ) );
   }
}

bool PMLightGroupEdit::isDataValid( )
{
   return Base::isDataValid( );
}

#include "pmlightgroupedit.moc"
