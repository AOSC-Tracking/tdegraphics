/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Luis Carvalho
    email                : lpassos@oninetspeed.pt
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/

#include "pmlibraryhandleedit.h"

#include <tqpushbutton.h>
#include <tqcheckbox.h>
#include <tqlineedit.h>
#include <textedit.h>
#include <tqvbox.h>
#include <layout.h>
#include <tqlabel.h>

#include <klocale.h>
#include <kconfig.h>
#include <kmessagebox.h>
#include <kfiledialog.h>

#include "pmlineedits.h"
#include "pmdialogeditbase.h"
#include "pmlibraryhandle.h"

TQSize PMLibraryHandleEdit::s_size = TQSize( 600, 400 );

PMLibraryHandleEdit::PMLibraryHandleEdit( PMLibraryHandle* lib, TQWidget* parent, const char* name )
      : KDialogBase( parent, name, true, i18n( "Create Library" ),
                     Ok | Cancel, Ok )
{
   m_pLibrary = lib;

   resize( s_size );
   TQWidget* page = new TQWidget( this );
   setMainWidget( page );
   TQVBoxLayout* vl = new TQVBoxLayout( page, KDialog::spacingHint( ) );

   TQGridLayout* grid = new TQGridLayout( vl, 5, 2 );
   TQLabel* lbl = new TQLabel( i18n( "Name: " ), page );
   m_pNameEdit = new TQLineEdit( page );
   grid->addWidget( lbl, 0, 0 );
   grid->addWidget( m_pNameEdit, 0, 1 );

   lbl = new TQLabel( i18n( "Author: " ), page );
   m_pAuthorEdit = new TQLineEdit( page );
   grid->addWidget( lbl, 1, 0 );
   grid->addWidget( m_pAuthorEdit, 1, 1 );

   lbl = new TQLabel( i18n( "Description: " ), page );
   m_pDescriptionEdit = new TQTextEdit( page );
   m_pDescriptionEdit->setMaximumHeight( 120 );
   grid->addWidget( lbl, 2, 0 );
   grid->addMultiCellWidget( m_pDescriptionEdit, 2, 3, 1, 1 );
   grid->setRowStretch( 3, 1 );
   
   m_pReadOnlyEdit = new TQCheckBox( i18n( "Allow changes to the library?" ), page );
   grid->addMultiCellWidget( m_pReadOnlyEdit, 4, 4, 0, 1 );

   // Load the fields with values
   m_pNameEdit->setText( lib->name( ) );
   m_pDescriptionEdit->setText( lib->description( ) );
   m_pAuthorEdit->setText( lib->author( ) );
   m_pReadOnlyEdit->setChecked( !lib->isReadOnly( ) ); 
   
   // Setup the signals
   connect( m_pNameEdit, TQT_SIGNAL( textChanged( const TQString& ) ), TQT_SLOT( slotEditsChanged( const TQString& ) ) );
   connect( m_pAuthorEdit, TQT_SIGNAL( textChanged( const TQString& ) ), TQT_SLOT( slotEditsChanged( const TQString& ) ) );
   connect( m_pDescriptionEdit, TQT_SIGNAL( textChanged( ) ), TQT_SLOT( slotDescriptionChanged( ) ) );
   connect( m_pReadOnlyEdit, TQT_SIGNAL( clicked( ) ), TQT_SLOT( slotReadOnlyChanged( ) ) );

   // On startup you can only cancel
   enableButtonOK( false );
}

void PMLibraryHandleEdit::slotReadOnlyChanged( )
{
   enableButtonOK( true );
}

void PMLibraryHandleEdit::slotEditsChanged( const TQString& /*str*/ )
{
   enableButtonOK( true );
}

void PMLibraryHandleEdit::slotDescriptionChanged( )
{
   enableButtonOK( true );
}

void PMLibraryHandleEdit::saveConfig( KConfig* cfg )
{
   cfg->setGroup( "Appearance" );
   cfg->writeEntry( "LibraryHandleEditSize", s_size );
}

void PMLibraryHandleEdit::restoreConfig( KConfig* cfg )
{
   cfg->setGroup( "Appearance" );

   TQSize defaultSize( 300, 200 );
   s_size = cfg->readSizeEntry( "LibraryHandleEditSize", &defaultSize );
}

void PMLibraryHandleEdit::resizeEvent( TQResizeEvent* ev )
{
   s_size = ev->size( );
}

void PMLibraryHandleEdit::slotOk( )
{
   m_pLibrary->setName( m_pNameEdit->text( ) );
   m_pLibrary->setAuthor( m_pAuthorEdit->text( ) );
   m_pLibrary->setDescription( m_pDescriptionEdit->text( ) );
   m_pLibrary->setReadOnly( !m_pReadOnlyEdit->isChecked( ) );
   
   accept( );
}

#include "pmlibraryhandleedit.moc"
