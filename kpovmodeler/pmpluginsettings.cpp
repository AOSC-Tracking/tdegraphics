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

#include "pmpluginsettings.h"

#include "pmpluginmanager.h"

#include <tqlayout.h>
#include <tqgroupbox.h>
#include <tqlistview.h>
#include <tqpushbutton.h>
#include <tdelocale.h>

class PMPluginListViewItem : public TQListViewItem
{
public:
   PMPluginListViewItem( TQListView* parent, PMPluginInfo* info )
         : TQListViewItem( parent, info->name( ), info->description( ) )
   {
      m_info = info;
      m_enabled = info->enabled( );
      setStatus( );
   }
   void toggleStatus( )
   {
      m_enabled = !m_enabled;
      setStatus( );
   }
   void setStatus( )
   {
      if( m_enabled )
         setText( 2, i18n( "loaded" ) );
      else
         setText( 2, i18n( "deactivated" ) );
   }
   PMPluginInfo* m_info;
   bool m_enabled;
};

PMPluginSettings::PMPluginSettings( TQWidget* parent, const char* name )
      : PMSettingsDialogPage( parent, name )
{
   TQVBoxLayout* vlayout = new TQVBoxLayout( this, 0, KDialog::spacingHint( ) );

   TQGroupBox* gb = new TQGroupBox( i18n( "Installed Plugins" ), this );
   vlayout->addWidget( gb );

   TQVBoxLayout* gvl = new TQVBoxLayout( gb, KDialog::marginHint( ), KDialog::spacingHint( ) );
   gvl->addSpacing( 10 );

   m_pPluginsList = new TQListView( gb );
   connect( m_pPluginsList, TQ_SIGNAL( selectionChanged( ) ),
            TQ_SLOT( slotSelectionChanged( ) ) );
   m_pPluginsList->addColumn( i18n( "Name" ) );
   m_pPluginsList->addColumn( i18n( "Description" ) );
   m_pPluginsList->addColumn( i18n( "Status" ) );
   gvl->addWidget( m_pPluginsList, 1 );

   TQHBoxLayout* hl = new TQHBoxLayout( gvl );
   m_pToggle = new TQPushButton( i18n( "Load" ), gb );
   m_pToggle->setEnabled( false );
   connect( m_pToggle, TQ_SIGNAL( clicked( ) ), TQ_SLOT( slotToggle( ) ) );
   hl->addWidget( m_pToggle );
   hl->addStretch( 1 );

   vlayout->addStretch( 1 );
}

void PMPluginSettings::displaySettings( )
{
   TQPtrList<PMPluginInfo> plugins = PMPluginManager::theManager( )->plugins( );
   TQPtrListIterator<PMPluginInfo> it( plugins );

   m_pPluginsList->clear( );
   for( ; *it; ++it )
      new PMPluginListViewItem( m_pPluginsList, *it );
}

void PMPluginSettings::applySettings( )
{
   bool changes = false;
   PMPluginListViewItem* item =
      ( PMPluginListViewItem* ) m_pPluginsList->firstChild( );
   for( ; item; item = ( PMPluginListViewItem* ) item->nextSibling( ) )
   {
      if( item->m_enabled != item->m_info->enabled( ) )
      {
         item->m_info->enable( item->m_enabled );
         changes = true;
      }
   }
   if( changes )
      PMPluginManager::theManager( )->updatePlugins( );
}

bool PMPluginSettings::validateData( )
{
   return true;
}

void PMPluginSettings::displayDefaults( )
{
}

void PMPluginSettings::slotToggle( )
{
   PMPluginListViewItem* item =
      ( PMPluginListViewItem* ) m_pPluginsList->currentItem( );
   if( item )
   {
      item->toggleStatus( );
   
      if( item->m_enabled )
         m_pToggle->setText( i18n( "Deactivate" ) );
      else
         m_pToggle->setText( i18n( "Load" ) );
   }
}

void PMPluginSettings::slotSelectionChanged( )
{
   PMPluginListViewItem* item =
      ( PMPluginListViewItem* ) m_pPluginsList->currentItem( );
   if( item )
   {
      m_pToggle->setEnabled( true );
      if( item->m_enabled )
         m_pToggle->setText( i18n( "Deactivate" ) );
      else
         m_pToggle->setText( i18n( "Load" ) );
   }
   else
      m_pToggle->setEnabled( false );
}

#include "pmpluginsettings.moc"
