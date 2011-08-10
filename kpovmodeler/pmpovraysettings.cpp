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

#include "pmpovraysettings.h"

#include "pmdocumentationmap.h"
#include "pmpovrayrenderwidget.h"
#include "pmdefaults.h"
#include "pmresourcelocator.h"
#include "pmtext.h"

#include <tqlayout.h>
#include <tqlineedit.h>
#include <tqpushbutton.h>
#include <tqlistbox.h>
#include <tqcombobox.h>
#include <tqgroupbox.h>
#include <tqlabel.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kmessagebox.h>
#include <kfiledialog.h>

PMPovraySettings::PMPovraySettings( TQWidget* tqparent, const char* name )
      : PMSettingsDialogPage( tqparent, name )
{
   m_selectionIndex = 0;

   TQHBoxLayout* htqlayout;
   TQVBoxLayout* vtqlayout;
   TQVBoxLayout* gvl;
   TQGroupBox* gb;

   vtqlayout = new TQVBoxLayout( this, 0, KDialog::spacingHint( ) );

   gb = new TQGroupBox( i18n( "Povray Command" ), this );
   gvl = new TQVBoxLayout( gb, KDialog::marginHint( ), KDialog::spacingHint( ) );
   gvl->addSpacing( 10 );
   htqlayout = new TQHBoxLayout( gvl );
   htqlayout->addWidget( new TQLabel( i18n( "Command:" ), gb ) );
   m_pPovrayCommand = new TQLineEdit( gb );
   htqlayout->addWidget( m_pPovrayCommand );
   m_pBrowsePovrayCommand = new TQPushButton( gb );
   m_pBrowsePovrayCommand->setPixmap( SmallIcon( "fileopen" ) );
   connect( m_pBrowsePovrayCommand, TQT_SIGNAL( clicked( ) ),
            TQT_SLOT( slotBrowsePovrayCommand( ) ) );
   htqlayout->addWidget( m_pBrowsePovrayCommand );
   vtqlayout->addWidget( gb );

   gb = new TQGroupBox( i18n( "Povray User Documentation" ), this );
   gvl = new TQVBoxLayout( gb, KDialog::marginHint( ), KDialog::spacingHint( ) );
   gvl->addSpacing( 10 );
   htqlayout = new TQHBoxLayout( gvl );
   htqlayout->addWidget( new TQLabel( i18n( "Path:" ), gb ) );
   m_pDocumentationPath = new TQLineEdit( gb );
   htqlayout->addWidget( m_pDocumentationPath );
   m_pBrowseDocumentationPath = new TQPushButton( gb );
   m_pBrowseDocumentationPath->setPixmap( SmallIcon( "fileopen" ) );
   connect( m_pBrowseDocumentationPath, TQT_SIGNAL( clicked( ) ),
            TQT_SLOT( slotBrowsePovrayDocumentation( ) ) );
   htqlayout->addWidget( m_pBrowseDocumentationPath );
   vtqlayout->addWidget( gb );
   htqlayout = new TQHBoxLayout( gvl );
   htqlayout->addWidget( new TQLabel( i18n( "Version:" ), gb ) );
   m_pDocumentationVersion = new TQComboBox( false, gb );
   TQValueList<TQString> versions = PMDocumentationMap::theMap( )->availableVersions( );
   TQValueListIterator<TQString> it;
   for( it = versions.begin( ); it != versions.end( ); ++it )
      m_pDocumentationVersion->insertItem( *it );
   htqlayout->addWidget( m_pDocumentationVersion );
   htqlayout->addStretch( );

   gb = new TQGroupBox( i18n( "Library Paths" ), this );
   gvl = new TQVBoxLayout( gb, KDialog::marginHint( ), KDialog::spacingHint( ) );
   gvl->addSpacing( 10 );
   htqlayout = new TQHBoxLayout( gvl );
   m_pLibraryPaths = new TQListBox( gb );
   connect( m_pLibraryPaths, TQT_SIGNAL( highlighted( int ) ),
            TQT_SLOT( slotPathSelected( int ) ) );
   htqlayout->addWidget( m_pLibraryPaths );

   TQVBoxLayout* bl = new TQVBoxLayout( htqlayout );
   m_pAddLibraryPath = new TQPushButton( i18n( "Add..." ), gb );
   connect( m_pAddLibraryPath, TQT_SIGNAL( clicked( ) ), TQT_SLOT( slotAddPath( ) ) );
   bl->addWidget( m_pAddLibraryPath );
   m_pRemoveLibraryPath = new TQPushButton( i18n( "Remove" ), gb );
   connect( m_pRemoveLibraryPath, TQT_SIGNAL( clicked( ) ),
            TQT_SLOT( slotRemovePath( ) ) );
   bl->addWidget( m_pRemoveLibraryPath );
   m_pChangeLibraryPath = new TQPushButton( i18n( "Edit..." ), gb );
   connect( m_pChangeLibraryPath, TQT_SIGNAL( clicked( ) ),
            TQT_SLOT( slotEditPath( ) ) );
   bl->addWidget( m_pChangeLibraryPath );
   m_pLibraryPathUp = new TQPushButton( i18n( "Up" ), gb );
   connect( m_pLibraryPathUp, TQT_SIGNAL( clicked( ) ), TQT_SLOT( slotPathUp( ) ) );
   bl->addWidget( m_pLibraryPathUp );
   m_pLibraryPathDown = new TQPushButton( i18n( "Down" ), gb );
   connect( m_pLibraryPathDown, TQT_SIGNAL( clicked( ) ),
            TQT_SLOT( slotPathDown( ) ) );
   bl->addWidget( m_pLibraryPathDown );
   bl->addStretch( 1 );
   vtqlayout->addWidget( gb );

   vtqlayout->addStretch( 1 );
}

void PMPovraySettings::displaySettings( )
{
   m_pPovrayCommand->setText( PMPovrayRenderWidget::povrayCommand( ) );
   m_pDocumentationPath->setText( PMDocumentationMap::theMap( )->povrayDocumentationPath( ) );
   int c = m_pDocumentationVersion->count( );
   TQString s = PMDocumentationMap::theMap( )->documentationVersion( );
   int i;
   for( i = 0; i < c; i++ )
      if( m_pDocumentationVersion->text( i ) == s )
         m_pDocumentationVersion->setCurrentItem( i );

   bool sb = m_pLibraryPaths->signalsBlocked( );
   m_pLibraryPaths->blockSignals( true );
   m_pLibraryPaths->clear( );
   TQStringList plist = PMPovrayRenderWidget::povrayLibraryPaths( );
   TQStringList::ConstIterator it = plist.begin( );
   m_selectionIndex = -1;
   for( ; it != plist.end( ); ++it )
   {
      m_pLibraryPaths->insertItem( *it );
      m_selectionIndex++;
   }

   m_pRemoveLibraryPath->setEnabled( false );
   m_pChangeLibraryPath->setEnabled( false );
   m_pLibraryPathUp->setEnabled( false );
   m_pLibraryPathDown->setEnabled( false );
   m_pLibraryPaths->blockSignals( sb );
}

void PMPovraySettings::displayDefaults( )
{
   m_pPovrayCommand->setText( c_defaultPovrayCommand );
   m_pDocumentationVersion->setCurrentItem( 0 );
   m_pDocumentationPath->setText( TQString() );
   m_pLibraryPaths->clear();

}

bool PMPovraySettings::validateData( )
{
   return true;
}

void PMPovraySettings::applySettings( )
{
   PMPovrayRenderWidget::setPovrayCommand( m_pPovrayCommand->text( ) );
   PMDocumentationMap::theMap( )->setPovrayDocumentationPath(
      m_pDocumentationPath->text( ) );
   PMDocumentationMap::theMap( )->setDocumentationVersion(
      m_pDocumentationVersion->currentText( ) );
   TQStringList plist;
   int num = ( signed ) m_pLibraryPaths->count( );
   int i;
   for( i = 0; i < num; i++ )
      plist.append( m_pLibraryPaths->text( i ) );

   if( PMPovrayRenderWidget::povrayLibraryPaths( ) != plist )
   {
      PMPovrayRenderWidget::setPovrayLibraryPaths( plist );
      PMResourceLocator::clearCache( );
      PMText::povrayLibraryPathsChanged( );
      emit repaintViews( );
   }
}

void PMPovraySettings::slotAddPath( )
{
   if( m_pLibraryPaths->count( ) >= 20 )
      KMessageBox::error( this, i18n( "Povray only supports up to 20 library paths." ) );
   else
   {
      TQString path = KFileDialog::getExistingDirectory( TQString(), this );
      if( !path.isEmpty( ) )
      {
         TQListBoxItem* item = m_pLibraryPaths->findItem( path, ExactMatch );
         if( !item )
         {
            m_pLibraryPaths->insertItem( path, m_selectionIndex + 1 );
            m_pLibraryPaths->setCurrentItem( m_selectionIndex + 1 );
         }
         else
            KMessageBox::error( this, i18n( "The list of library paths already contains this path." ) );
      }
   }
}

void PMPovraySettings::slotRemovePath( )
{
   m_pLibraryPaths->removeItem( m_selectionIndex );
   if( ( unsigned ) m_selectionIndex >= m_pLibraryPaths->count( ) )
      m_selectionIndex--;
   m_pLibraryPaths->setCurrentItem( m_selectionIndex );
}

void PMPovraySettings::slotPathUp( )
{
   TQListBoxItem* lbi = m_pLibraryPaths->item( m_selectionIndex );
   if( lbi )
   {
      TQString text = lbi->text( );
      m_pLibraryPaths->removeItem( m_selectionIndex );
      if( m_selectionIndex > 0 )
         m_selectionIndex--;
      m_pLibraryPaths->insertItem( text, m_selectionIndex );
      m_pLibraryPaths->setCurrentItem( m_selectionIndex );
   }
}

void PMPovraySettings::slotPathDown( )
{
   TQListBoxItem* lbi = m_pLibraryPaths->item( m_selectionIndex );
   if( lbi )
   {
      TQString text = lbi->text( );
      m_pLibraryPaths->removeItem( m_selectionIndex );
      if( ( unsigned ) m_selectionIndex < m_pLibraryPaths->count( ) )
         m_selectionIndex++;
      m_pLibraryPaths->insertItem( text, m_selectionIndex );
      m_pLibraryPaths->setCurrentItem( m_selectionIndex );
   }
}

void PMPovraySettings::slotEditPath( )
{
   TQListBoxItem* lbi = m_pLibraryPaths->item( m_selectionIndex );
   if( lbi )
   {
      TQString text = lbi->text( );
      TQString path = KFileDialog::getExistingDirectory( text, this );
      if( !path.isEmpty( ) )
      {
         TQListBoxItem* item = m_pLibraryPaths->findItem( path, ExactMatch );
         if( !item )
            m_pLibraryPaths->changeItem( path, m_selectionIndex );
         else if( item != lbi )
            KMessageBox::error( this, i18n( "The list of library paths already contains this path." ) );
      }
   }
}

void PMPovraySettings::slotPathSelected( int index )
{
   m_selectionIndex = index;
   TQListBoxItem* lbi = m_pLibraryPaths->item( m_selectionIndex );
   if( lbi )
   {
      m_pRemoveLibraryPath->setEnabled( true );
      m_pChangeLibraryPath->setEnabled( true );
      m_pLibraryPathUp->setEnabled( index > 0 );
      m_pLibraryPathDown->setEnabled( index < ( ( signed ) m_pLibraryPaths->count( ) - 1 ) );
   }
   else
   {
      m_pRemoveLibraryPath->setEnabled( false );
      m_pChangeLibraryPath->setEnabled( false );
      m_pLibraryPathUp->setEnabled( false );
      m_pLibraryPathDown->setEnabled( false );
   }
}

void PMPovraySettings::slotBrowsePovrayCommand( )
{
   TQString str = KFileDialog::getOpenFileName( TQString(), TQString() );

   if( !str.isEmpty() )
   {
      m_pPovrayCommand->setText( str );
   }
}

void PMPovraySettings::slotBrowsePovrayDocumentation( )
{
   TQString str = KFileDialog::getExistingDirectory( );

   if( !str.isEmpty( ) )
      m_pDocumentationPath->setText( str );
}

#include "pmpovraysettings.moc"
