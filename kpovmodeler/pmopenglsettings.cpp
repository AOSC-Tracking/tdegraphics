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

#include "pmopenglsettings.h"

#include "pmglview.h"
#include "pmdefaults.h"

#include <tqlayout.h>
#include <tqcheckbox.h>
#include <tqlabel.h>
#include <klocale.h>

PMOpenGLSettings::PMOpenGLSettings( TQWidget* tqparent, const char* name )
      : PMSettingsDialogPage( tqparent, name )
{
   TQVBoxLayout* vtqlayout = new TQVBoxLayout( this, 0, KDialog::spacingHint( ) );

   m_pDirect = new TQCheckBox( i18n( "Direct rendering" ), this );
   vtqlayout->addWidget( new TQLabel( i18n( "Changes take only effect after a restart!" ), this ) );
   vtqlayout->addWidget( m_pDirect );
   vtqlayout->addStretch( 1 );
}

void PMOpenGLSettings::displaySettings( )
{
   m_pDirect->setChecked( PMGLView::isDirectRenderingEnabled( ) );
}

void PMOpenGLSettings::displayDefaults( )
{
   m_pDirect->setChecked( true );
}

bool PMOpenGLSettings::validateData( )
{
   return true;
}

void PMOpenGLSettings::applySettings( )
{
   PMGLView::enableDirectRendering( m_pDirect->isChecked( ) );
}

#include "pmopenglsettings.moc"
