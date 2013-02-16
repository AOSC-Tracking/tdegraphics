/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Luis Carvalho
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


#include "pmlibraryobject.h"
#include "pmlibraryentrypreview.h"
#include "pmlibraryobjectsearch.h"
#include "pmdialogeditbase.h"

#include <tqlabel.h>
#include <tqlineedit.h>
#include <tqpushbutton.h>
#include <tqlayout.h>
#include <tqpixmap.h>
#include <tqimage.h>
#include <tqframe.h>

#include <kurl.h>
#include <tdelocale.h>
#include <kdialog.h>
#include <tdelistview.h>

PMLibraryObjectSearch::PMLibraryObjectSearch( TQWidget* parent ) :
   TQWidget( parent, "" )
{
   setMinimumSize( 780, 300 );
   setMaximumSize( 800, 400 );
   TQVBoxLayout* vl = new TQVBoxLayout( this, KDialog::spacingHint( ) );

   // Search parameters
   TQFrame* frame = new TQFrame( this );
   TQHBoxLayout* fhl = new TQHBoxLayout( frame, KDialog::spacingHint( ) );
   TQGridLayout* grid = new TQGridLayout( fhl, 3, 2 );
   TQLabel *lbl = new TQLabel( i18n( "Search for:" ), frame );
   m_pSearch = new TQLineEdit( frame );
   grid->addWidget( lbl, 0, 0 );
   grid->addWidget( m_pSearch, 0, 1 );

   TQVBoxLayout* fvl = new TQVBoxLayout( fhl );
   m_pSearchButton = new TQPushButton( i18n( "&Search" ), frame );
   fvl->addWidget( m_pSearchButton );
   fvl->addStretch( 1 );

   vl->addWidget( frame );

   // Search results
   frame = new TQFrame( this );
   TQHBoxLayout* hl = new TQHBoxLayout( frame, KDialog::spacingHint( ) );
   m_pFileList = new TDEListView( frame );
   m_pFileList->addColumn( i18n( "File" ) );
   m_pFileList->addColumn( i18n( "Path" ) );
   m_pFileList->setFullWidth( true );
   m_pPreview = new PMLibraryEntryPreview( frame );
   hl->addWidget( m_pFileList, 1 );
   hl->addWidget( m_pPreview );
   vl->addWidget( frame );

   // Connect signals and slots
   connect( m_pSearchButton, TQT_SIGNAL( clicked( ) ), TQT_SLOT( slotSearchButtonPressed( ) ) );
}

void PMLibraryObjectSearch::slotSearchButtonPressed( )
{
//   TQStringList::Iterator it( s_libraryPath );
   // For each of the defined libraries
   //
   // Open recursively each library file
   // Check if any of the strings contains the search words
   // If it does add to the list
}

#include "pmlibraryobjectsearch.moc"
