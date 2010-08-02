/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Andreas Zehender
    email                : zehender@kde.org
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

#include "pmobjectlibrarysettings.h"

#include "pmlibrarymanager.h"
#include "pmlibraryhandle.h"
#include "pmlibraryhandleedit.h"
#include "pmdebug.h"

#include <tqlayout.h>
#include <tqgroupbox.h>
#include <tqlabel.h>
#include <tqpushbutton.h>
#include <klocale.h>
#include <kfiledialog.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>

PMObjectLibrarySettings::PMObjectLibrarySettings( TQWidget* parent, const char* name )
      : PMSettingsDialogPage( parent, name )
{
   TQVBoxLayout* vlayout = new TQVBoxLayout( this, 0, KDialog::spacingHint( ) );

   TQGroupBox* gb = new TQGroupBox( i18n( "Libraries" ), this );
   vlayout->addWidget( gb );

   TQHBoxLayout* hlayout = new TQHBoxLayout( gb, KDialog::marginHint( ) + 5, KDialog::spacingHint( ) );
   m_pObjectLibraries = new TQListBox( gb );
   connect( m_pObjectLibraries, TQT_SIGNAL( selectionChanged( ) ), TQT_SLOT( slotObjectLibraryChanged( ) ) );
   hlayout->addWidget( m_pObjectLibraries );
   TQVBoxLayout* gvl = new TQVBoxLayout( hlayout );
   m_pCreateObjectLibrary = new TQPushButton( i18n( "Create..." ), gb );
   connect( m_pCreateObjectLibrary, TQT_SIGNAL( clicked( ) ), TQT_SLOT( slotCreateObjectLibrary( ) ) );
   gvl->addWidget( m_pCreateObjectLibrary );
   m_pRemoveObjectLibrary = new TQPushButton( i18n( "Remove" ), gb );
   connect( m_pRemoveObjectLibrary, TQT_SIGNAL( clicked( ) ), TQT_SLOT( slotRemoveObjectLibrary( ) ) );
   gvl->addWidget( m_pRemoveObjectLibrary );
   m_pImportObjectLibrary = new TQPushButton( i18n( "Import" ), gb );
   connect( m_pImportObjectLibrary, TQT_SIGNAL( clicked( ) ), TQT_SLOT( slotImportObjectLibrary( ) ) );
   gvl->addWidget( m_pImportObjectLibrary );
   m_pModifyObjectLibrary = new TQPushButton( i18n( "Properties" ), gb );
   connect( m_pModifyObjectLibrary, TQT_SIGNAL( clicked( ) ), TQT_SLOT( slotModifyObjectLibrary( ) ) );
   gvl->addWidget( m_pModifyObjectLibrary );
   gvl->addStretch( 1 );

   gb = new TQGroupBox( i18n( "Library Details" ), this );
   TQGridLayout* grid = new TQGridLayout( gb, 4, 2, KDialog::marginHint( ) + 5, KDialog::spacingHint( ) );
   TQLabel* lbl = new TQLabel( i18n( "Path" ), gb );
   grid->addWidget( lbl, 0, 0 );
   lbl = new TQLabel( i18n( "Author" ), gb );
   grid->addWidget( lbl, 1, 0 );
   lbl = new TQLabel( i18n( "Description" ), gb );
   grid->addWidget( lbl, 2, 0 );
   m_pLibraryPath = new TQLabel( "", gb );
   m_pLibraryAuthor = new TQLabel( "", gb );
   m_pLibraryDescription = new TQLabel( "", gb );
   m_pReadOnlyText = new TQLabel( "", gb );
   grid->addWidget( m_pLibraryPath, 0, 1 );
   grid->addWidget( m_pLibraryAuthor, 1, 1 );
   grid->addWidget( m_pLibraryDescription, 2, 1 );
   grid->addMultiCellWidget( m_pReadOnlyText, 3, 3, 0, 1 );
   grid->setColStretch( 1, 1 );
   vlayout->addWidget( gb );

   vlayout->addStretch( 1 );
}

void PMObjectLibrarySettings::displaySettings( )
{
   m_pObjectLibraries->clear( );
   m_pObjectLibraries->insertStringList( PMLibraryManager::theManager( )->availableLibraries( ) );
}

void PMObjectLibrarySettings::applySettings( )
{
}

bool PMObjectLibrarySettings::validateData( )
{
   return true;
}

void PMObjectLibrarySettings::displayDefaults( )
{
}

void PMObjectLibrarySettings::slotImportObjectLibrary( )
{
   // TODO
}

void PMObjectLibrarySettings::slotModifyObjectLibrary( )
{
   PMLibraryHandle* h = PMLibraryManager::theManager( )->getLibraryHandle( m_pObjectLibraries->currentText( ) );
   if( h )
   {
      PMLibraryHandleEdit h_dlg( h, this );

      if( h_dlg.exec( ) == TQDialog::Accepted )
      {
         h->saveLibraryInfo( );
         displaySettings( );
         for( unsigned i = 0; i < m_pObjectLibraries->count( ); ++i )
         {
            if( m_pObjectLibraries->text( i ) == h->name( ) )
            {
               m_pObjectLibraries->setSelected( i, true );
               break;
            }
         }
         slotObjectLibraryChanged( );
      }
   }
}

void PMObjectLibrarySettings::slotRemoveObjectLibrary( )
{
   m_pObjectLibraries->removeItem( m_pObjectLibraries->currentItem( ) );
}

void PMObjectLibrarySettings::slotCreateObjectLibrary( )
{
   PMLibraryHandle h;
   PMLibraryHandleEdit h_dlg( &h, this );
   TQString libfilename;

   if( h_dlg.exec( ) == TQDialog::Accepted )
   {
      libfilename = h.name( );
      h.setPath( locateLocal( "appdata", "library/" ) + libfilename.stripWhiteSpace( ) + "/" );
      // Create the new library
      switch( h.createLibrary( ) )
      {
         case PMLibraryHandle::Ok:
            PMLibraryManager::theManager( )->refresh( );
            displaySettings( );
            break;
         case PMLibraryHandle::ExistingDir:
            KMessageBox::error( this, i18n( "The folder already exists." ) );
            break;
         case PMLibraryHandle::CouldNotCreateDir:
            KMessageBox::error( this, i18n( "Could not create the folder." ) );
            break;
         default:
            kdError( PMArea ) << "Unexpected error in slotCreateObjectLibrary." << endl;
      }
   }
}

void PMObjectLibrarySettings::slotObjectLibraryChanged( )
{
   PMLibraryHandle* h = PMLibraryManager::theManager( )->getLibraryHandle( m_pObjectLibraries->currentText( ) );
   if( h )
   {
      m_pLibraryPath->setText( h->path( ) );
      m_pLibraryAuthor->setText( h->author( ) );
      m_pLibraryDescription->setText( h->description( ) );
      if( h->isReadOnly( ) )
         m_pReadOnlyText->setText( i18n( "This library is not modifiable." ) );
      else
         m_pReadOnlyText->setText( i18n( "This library is modifiable." ) );
   }
   else
   {
      m_pLibraryPath->setText( "" );
      m_pLibraryAuthor->setText( "" );
      m_pLibraryDescription->setText( "" );
      m_pReadOnlyText->setText( "" );
   }
}

#include "pmobjectlibrarysettings.moc"
