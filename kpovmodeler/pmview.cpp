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


#include <tqprinter.h>
#include <tqpainter.h>
#include <tqsplitter.h>
#include <tqlayout.h>
#include <tqlabel.h>
#include <tdeconfig.h>
#include <kdialog.h>
#include <klocale.h>

#include <stdio.h>

// application specific includes
#include "pmview.h"
#include "pmpart.h"
#include "pmtreeview.h"
#include "pmdialogview.h"
#include "pmglview.h"

PMView::PMView( PMPart* part, TQWidget* parent, const char* name )
      : TQWidget( parent, name )
{
   setBackgroundMode( PaletteBase );

   TQVBoxLayout* layout = new TQVBoxLayout( this );
   m_pMainSplitter = new TQSplitter( Qt::Horizontal, this, "MainSplitter" );
   m_pTreeEditSplitter = new TQSplitter( Qt::Vertical, m_pMainSplitter,
                                        "TreeEditSplitter" );

   
   m_pTreeView = new PMTreeView( part, m_pTreeEditSplitter, "TreeView" );
   m_pTreeView->show( );
   
   m_pDialogView = new PMDialogView( part, m_pTreeEditSplitter, "EditView" );
   m_pDialogView->show( );

   
   TQWidget* glArea = new TQWidget( m_pMainSplitter, "GLArea" );
   glArea->show( );
   PMGLView* gl;
   
   TQGridLayout* topLayout = new TQGridLayout( glArea, 2, 2, 1, 1 );
   gl = new PMGLView( part, PMGLView::PMViewPosZ, glArea );
   topLayout->addWidget( gl, 0, 0 );
   gl = new PMGLView( part, PMGLView::PMViewPosX, glArea );
   topLayout->addWidget( gl, 0, 1 );
   gl = new PMGLView( part, PMGLView::PMViewNegY, glArea );
   topLayout->addWidget( gl, 1, 0 );
   gl = new PMGLView( part, PMGLView::PMViewCamera, glArea );
   topLayout->addWidget( gl, 1, 1 );
   
   m_pMainSplitter->show( );

   layout->addWidget( m_pMainSplitter );
   layout->activate( );

   m_pPart = part;
}

PMView::~PMView( )
{
}

void PMView::print( TQPrinter* pPrinter )
{
   TQPainter printpainter;
   printpainter.begin( pPrinter );
	
   // TODO: add your printing code here

   printpainter.end( );
}

void PMView::saveConfig( TDEConfig* cfg )
{
   cfg->setGroup( "Appearance" );

   cfg->writeEntry( "MainSplitter", m_pMainSplitter->sizes( ) );
   cfg->writeEntry( "TreeEditSplitter", m_pTreeEditSplitter->sizes( ) );
}

void PMView::restoreConfig( TDEConfig* cfg )
{
   cfg->setGroup( "Appearance" );

   m_pMainSplitter->setSizes( cfg->readIntListEntry( "MainSplitter" ) );
   m_pTreeEditSplitter->setSizes( cfg->readIntListEntry( "TreeEditSplitter" ) );
}


#include "pmview.moc"
