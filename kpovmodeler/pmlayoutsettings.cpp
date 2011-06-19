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

#include "pmlayoutsettings.h"

#include "pmlineedits.h"
#include "pmviewfactory.h"
#include "pmdebug.h"

#include <tqlayout.h>
#include <tqcombobox.h>
#include <tqlistbox.h>
#include <tqlistview.h>
#include <tqpushbutton.h>
#include <tqlabel.h>
#include <tqgroupbox.h>
#include <klocale.h>
#include <kmessagebox.h>

PMLayoutSettings::PMLayoutSettings( TQWidget* tqparent, const char* name )
      : PMSettingsDialogPage( tqparent, name )
{
   TQHBoxLayout* htqlayout;
   TQVBoxLayout* vtqlayout;
   TQVBoxLayout* gvl;
   TQGridLayout* grid;
   TQGroupBox* gb;
   TQGroupBox* gbe;
   TQHBoxLayout* ghe;
   TQVBoxLayout* gvle;

   vtqlayout = new TQVBoxLayout( this, 0, KDialog::spacingHint( ) );

   htqlayout = new TQHBoxLayout( vtqlayout );
   htqlayout->addWidget( new TQLabel( i18n( "Default view tqlayout:" ), this ) );
   m_pDefaultLayout = new TQComboBox( this );
   htqlayout->addWidget( m_pDefaultLayout, 1 );
   htqlayout->addStretch( 1 );

   gb = new TQGroupBox( i18n( "Available View Layouts" ), this );
   vtqlayout->addWidget( gb );
   gvl = new TQVBoxLayout( gb, KDialog::marginHint( ), KDialog::spacingHint( ) );
   gvl->addSpacing( 10 );
   
   grid = new TQGridLayout( gvl, 3, 2 );
   m_pViewLayouts = new TQListBox( gb );
   connect( m_pViewLayouts, TQT_SIGNAL( highlighted( int ) ), 
                            TQT_SLOT( slotLayoutSelected( int ) ) );
   grid->addMultiCellWidget( m_pViewLayouts, 0, 2, 0, 0 );
   m_pAddLayout = new TQPushButton( i18n( "Add" ), gb );
   connect( m_pAddLayout, TQT_SIGNAL( clicked( ) ), TQT_SLOT( slotAddLayout( ) ) );
   grid->addWidget( m_pAddLayout, 0, 1 );
   m_pRemoveLayout = new TQPushButton( i18n( "Remove" ), gb );
   connect( m_pRemoveLayout, TQT_SIGNAL( clicked( ) ), TQT_SLOT( slotRemoveLayout( ) ) );
   grid->addWidget( m_pRemoveLayout, 1, 1 );
   grid->setRowStretch( 2, 1 );

   gbe = new TQGroupBox( i18n( "View Layout" ), gb );
   gvl->addWidget( gbe );
   gvle = new TQVBoxLayout( gbe, KDialog::marginHint( ), KDialog::spacingHint( ) );
   gvle->addSpacing( 10 );
   ghe = new TQHBoxLayout( gvle );
   ghe->addWidget( new TQLabel( i18n( "Name:" ), gbe ) );
   m_pViewLayoutName = new TQLineEdit( gbe );
   connect( m_pViewLayoutName, TQT_SIGNAL( textChanged( const TQString& ) ), 
                               TQT_SLOT( slotLayoutNameChanged( const TQString& ) ) );
   ghe->addWidget( m_pViewLayoutName );
   grid = new TQGridLayout( gvle, 4, 2 );
   m_pViewEntries = new TQListView( gbe );
   m_pViewEntries->setAllColumnsShowFocus( true );
   m_pViewEntries->addColumn( " " );  // This column is for the view entry number 
   m_pViewEntries->addColumn( i18n( "Type" ) );
   m_pViewEntries->addColumn( i18n( "Position" ) );
   m_pViewEntries->setSorting( -1 );
   m_pViewEntries->setMaximumHeight( 150 );
   connect( m_pViewEntries, TQT_SIGNAL( selectionChanged( TQListViewItem* ) ),
                            TQT_SLOT( slotViewEntrySelected( TQListViewItem* ) ) );
   grid->addMultiCellWidget( m_pViewEntries, 0, 3, 0, 0 );
   m_pAddEntry = new TQPushButton( i18n( "Add" ), gbe );
   connect( m_pAddEntry, TQT_SIGNAL( clicked( ) ), TQT_SLOT( slotAddViewEntryClicked( ) ) );
   grid->addWidget( m_pAddEntry, 0, 1 );
   m_pRemoveEntry = new TQPushButton( i18n( "Remove" ), gbe );
   connect( m_pRemoveEntry, TQT_SIGNAL( clicked( ) ), TQT_SLOT( slotRemoveViewEntryClicked( ) ) );
   grid->addWidget( m_pRemoveEntry, 1, 1 );
   /* //TODO
   m_pMoveUpEntry = new TQPushButton( i18n( "Move Up" ), gbe );
   connect( m_pMoveUpEntry, TQT_SIGNAL( clicked( ) ), TQT_SLOT( slotMoveUpViewEntryClicked( ) ) );
   grid->addWidget( m_pMoveUpEntry, 2, 1 );
   m_pMoveDownEntry = new TQPushButton( i18n( "Move Down" ), gbe );
   connect( m_pMoveDownEntry, TQT_SIGNAL( clicked( ) ), TQT_SLOT( slotMoveDownViewEntryClicked( ) ) );
   grid->addWidget( m_pMoveDownEntry, 3, 1 );
   */

   TQHBoxLayout* ghl = new TQHBoxLayout( gvle );
   grid = new TQGridLayout( ghl, 7, 4 );
   grid->addWidget( new TQLabel( i18n( "Type:" ), gbe ), 0, 0 );
   grid->addWidget( new TQLabel( i18n( "Dock position:" ), gbe ), 2, 0 );
   m_pColumnWidthLabel = new TQLabel( i18n( "Column width:" ), gbe );
   grid->addWidget( m_pColumnWidthLabel, 3, 0 );
   m_pViewHeightLabel = new TQLabel( i18n( "View height:" ), gbe );
   grid->addWidget( m_pViewHeightLabel, 4, 0 );
   m_pViewTypeEdit = new TQComboBox( gbe );

   // insert all view types
   const TQPtrList<PMViewTypeFactory>& types =
      PMViewFactory::theFactory( )->viewTypes( );
   TQPtrListIterator<PMViewTypeFactory> it( types );
   for( ; *it; ++it )
      m_pViewTypeEdit->insertItem( ( *it )->description( ) );
   
   connect( m_pViewTypeEdit, TQT_SIGNAL( activated( int ) ),
                             TQT_SLOT( slotViewTypeChanged( int ) ) );
   grid->addWidget( m_pViewTypeEdit, 0, 1 );
   m_pDockPositionEdit = new TQComboBox( gbe );
   m_pDockPositionEdit->insertItem( i18n( "New Column" ) );
   m_pDockPositionEdit->insertItem( i18n( "Below" ) );
   m_pDockPositionEdit->insertItem( i18n( "Tabbed" ) );
   m_pDockPositionEdit->insertItem( i18n( "Floating" ) );
   connect( m_pDockPositionEdit, TQT_SIGNAL( activated( int ) ),
            TQT_SLOT( slotDockPositionChanged( int ) ) );
   grid->addWidget( m_pDockPositionEdit, 2, 1 );
   
   m_pColumnWidthEdit = new PMIntEdit( gbe );
   connect( m_pColumnWidthEdit, TQT_SIGNAL( textChanged( const TQString& ) ),
            TQT_SLOT( slotColumnWidthChanged( const TQString& ) ) );
   grid->addWidget( m_pColumnWidthEdit, 3, 1 );
   
   m_pViewHeightEdit = new PMIntEdit( gbe );
   connect( m_pViewHeightEdit, TQT_SIGNAL( textChanged( const TQString& ) ),
            TQT_SLOT( slotViewHeightChanged( const TQString& ) ) );
   grid->addWidget( m_pViewHeightEdit, 4, 1 );

   m_pFloatingWidthLabel = new TQLabel( i18n( "Width:" ), gbe );
   m_pFloatingHeightLabel = new TQLabel( i18n( "Height:" ), gbe );
   m_pFloatingPosXLabel = new TQLabel( i18n( "Position x:" ), gbe );
   m_pFloatingPosYLabel = new TQLabel( TQString( "y:" ), gbe );
   grid->addWidget( m_pFloatingWidthLabel, 5, 0 );
   grid->addWidget( m_pFloatingHeightLabel, 5, 2 );
   grid->addWidget( m_pFloatingPosXLabel, 6, 0 );
   grid->addWidget( m_pFloatingPosYLabel, 6, 2 );

   m_pFloatingWidth = new PMIntEdit( gbe );
   connect( m_pFloatingWidth, TQT_SIGNAL( textChanged( const TQString& ) ),
            TQT_SLOT( slotFloatingWidthChanged( const TQString& ) ) );
   m_pFloatingHeight = new PMIntEdit( gbe );
   connect( m_pFloatingHeight, TQT_SIGNAL( textChanged( const TQString& ) ),
            TQT_SLOT( slotFloatingHeightChanged( const TQString& ) ) );
   m_pFloatingPosX = new PMIntEdit( gbe );
   connect( m_pFloatingPosX, TQT_SIGNAL( textChanged( const TQString& ) ),
            TQT_SLOT( slotFloatingPosXChanged( const TQString& ) ) );
   m_pFloatingPosY = new PMIntEdit( gbe );
   connect( m_pFloatingPosY, TQT_SIGNAL( textChanged( const TQString& ) ),
            TQT_SLOT( slotFloatingPosYChanged( const TQString& ) ) );
   grid->addWidget( m_pFloatingWidth, 5, 1 );
   grid->addWidget( m_pFloatingHeight, 5, 3 );
   grid->addWidget( m_pFloatingPosX, 6, 1 );
   grid->addWidget( m_pFloatingPosY, 6, 3 );

   // create a holder widget for custom options widgets
   m_pCustomOptionsWidget = 0;
   m_pCustomOptionsHolder = new TQWidget( gbe );

   TQVBoxLayout* covl = new TQVBoxLayout( ghl );
   covl->addWidget( m_pCustomOptionsHolder );
   covl->addStretch( 1 );
   
   ghl->addStretch( 1 );

   vtqlayout->addStretch( 1 );
}

void PMLayoutSettings::displaySettings( )
{
   PMViewLayoutManager* m = PMViewLayoutManager::theManager( );
   m_viewLayouts = m->tqlayouts( );
   m_currentViewLayout = m_viewLayouts.begin( );
   m_defaultViewLayout = m_viewLayouts.begin( );
   for( ; ( m_defaultViewLayout != m_viewLayouts.end( ) ) &&
          ( *m_defaultViewLayout ).name( ) != m->defaultLayout( );
        ++m_defaultViewLayout );
   displayLayoutList( );
   m_pViewLayouts->setCurrentItem( 0 );
   if( m_pViewLayouts->numRows( ) == 1 )
      m_pRemoveLayout->setEnabled( false );
}

void PMLayoutSettings::displayDefaults( )
{
}

bool PMLayoutSettings::validateData( )
{
   TQValueListIterator<PMViewLayout> lit;
   for( lit = m_viewLayouts.begin( ); lit != m_viewLayouts.end( ); ++lit )
   {
      if( ( *lit ).name( ).isEmpty( ) )
      {
         emit showMe( );
         KMessageBox::error( this, i18n( "View tqlayouts may not have empty names." ),
                             i18n( "Error" ) );
         return false;
      }
      TQValueListIterator<PMViewLayoutEntry> eit = ( *lit ).begin( );
      if( eit != ( *lit ).end( ) )
      {
         if( ( *eit ).dockPosition( ) != PMDockWidget::DockRight )
         {
            emit showMe( );
            KMessageBox::error( this, i18n( "The docking position of the first view tqlayout entry has to be 'New Column'." ),
                                i18n( "Error" ) );
            return false;
         }
      }
   }
   return true;
}

void PMLayoutSettings::applySettings( )
{
   TQValueListIterator<PMViewLayout> lit;
   for( lit = m_viewLayouts.begin( ); lit != m_viewLayouts.end( ); ++lit )
      ( *lit ).normalize( );
   PMViewLayoutManager::theManager( )->setDefaultLayout( m_pDefaultLayout->currentText( ) );
   PMViewLayoutManager::theManager( )->setLayouts( m_viewLayouts );
   PMViewLayoutManager::theManager( )->saveData( );
}

void PMLayoutSettings::displayLayoutList( )
{
   TQValueListIterator<PMViewLayout> it;
   
   m_pViewLayouts->clear( );
   m_pDefaultLayout->clear( );
   for( it = m_viewLayouts.begin( ); it != m_viewLayouts.end( ); ++it )
   {
      m_pViewLayouts->insertItem( ( *it ).name( ) );
      m_pDefaultLayout->insertItem( ( *it ).name( ) );
      if( it == m_defaultViewLayout )
         m_pDefaultLayout->setCurrentText( ( *it ).name( ) );
   }
}

void PMLayoutSettings::slotAddLayout( )
{
   TQString new_name;
   int i = 1;
   TQString str;

   new_name = i18n( "Unnamed" );
   str.setNum( i );
   while( m_pViewLayouts->tqfindItem( new_name, TQt::ExactMatch ) )
   {
      new_name = i18n( "Unnamed" ) + str;
      i++;
      str.setNum( i );
   }

   PMViewLayout l;
   l.setName( new_name );

   m_currentViewLayout++;
   m_viewLayouts.insert( m_currentViewLayout, l );
   displayLayoutList( );
   m_pViewLayouts->setCurrentItem( m_pViewLayouts->tqfindItem( new_name, TQt::ExactMatch ) );
   m_pRemoveLayout->setEnabled( true );
}

void PMLayoutSettings::slotRemoveLayout( )
{
   if( m_currentViewLayout == m_defaultViewLayout )
   {
      m_defaultViewLayout--;
      if( m_defaultViewLayout == m_viewLayouts.end( ) )
      {
         m_defaultViewLayout++;
         m_defaultViewLayout++;
      }
   }
   m_viewLayouts.remove( m_currentViewLayout );
   displayLayoutList( );
   m_pViewLayouts->setCurrentItem( m_pViewLayouts->firstItem( ) );

   if( m_pViewLayouts->numRows( ) == 1 )
      m_pRemoveLayout->setEnabled( false );
}

void PMLayoutSettings::slotLayoutSelected( int index )
{
  int i;
  TQString str;
  bool sb;

  m_currentViewLayout = m_viewLayouts.at( index );
  m_currentViewEntry = ( *m_currentViewLayout ).begin( );

  sb = m_pViewLayoutName->signalsBlocked( );
  m_pViewLayoutName->blockSignals( true );
  m_pViewLayoutName->setText( ( *m_currentViewLayout ).name( ) );
  m_pViewLayoutName->blockSignals( sb );
  
  PMViewLayout::iterator it;
  TQListViewItem* previous = NULL;
  m_pViewEntries->clear( );
  i = 0;
  for( it = ( *m_currentViewLayout ).begin( );
       it != ( *m_currentViewLayout ).end( ); ++it )
  {
     i++; str.setNum( i );
     previous = new TQListViewItem( m_pViewEntries, previous, str,
                                   ( *it ).viewTypeAsString( ),
                                   ( *it ).dockPositionAsString( ) );
     if( i == 1 )
        m_pViewEntries->setSelected( previous, true );
  }
  if( i == 0 )
     slotViewEntrySelected( 0 );
}

void PMLayoutSettings::slotLayoutNameChanged( const TQString& text )
{
   int n_item = m_pViewLayouts->currentItem( );
   bool sb = m_pViewLayouts->signalsBlocked( );
   m_pViewLayouts->blockSignals( true );
   m_pViewLayouts->removeItem( n_item );
   m_pViewLayouts->insertItem( text, n_item );
   m_pViewLayouts->setCurrentItem( n_item );
   m_pViewLayouts->blockSignals( sb );

   ( *m_currentViewLayout ).setName( text );
   
   TQValueListIterator<PMViewLayout> it;   
   m_pDefaultLayout->clear( );
   for( it = m_viewLayouts.begin( ); it != m_viewLayouts.end( ); ++it )
   {
      m_pDefaultLayout->insertItem( ( *it ).name( ) );
      if( it == m_defaultViewLayout )
         m_pDefaultLayout->setCurrentText( ( *it ).name( ) );
   }
}

void PMLayoutSettings::slotViewEntrySelected( TQListViewItem *item )
{
   if( item )
   {
      m_pViewTypeEdit->setEnabled( true );
      m_pDockPositionEdit->setEnabled( true );
      
      int n_item = item->text( 0 ).toInt( ) - 1;

      m_currentViewEntry = ( *m_currentViewLayout ).at( n_item );
      TQString vt = ( *m_currentViewEntry ).viewType( );

      // find the view type
      int index = 0;
      bool found = false;
      const TQPtrList<PMViewTypeFactory>& types =
         PMViewFactory::theFactory( )->viewTypes( );
      TQPtrListIterator<PMViewTypeFactory> it( types );

      for( ; *it && !found; ++it )
      {
         if( ( *it )->viewType( ) == vt )
            found = true;
         else
            index++;
      }
            
      if( !found )
      {
         kdError( PMArea ) << "Unknown view type in PMLayoutSettings::slotViewEntrySelected" << endl;
         m_pViewTypeEdit->setCurrentItem( 0 );
      }
      else
         m_pViewTypeEdit->setCurrentItem( index );
      
      /*
      switch( ( *m_currentViewEntry ).glViewType( ) )
      {
         case PMGLView::PMViewNegY:
            m_pGLViewTypeEdit->setCurrentItem( 0 );
            break;
         case PMGLView::PMViewPosY:
            m_pGLViewTypeEdit->setCurrentItem( 1 );
            break;
         case PMGLView::PMViewPosX:
            m_pGLViewTypeEdit->setCurrentItem( 2 );
            break;
         case PMGLView::PMViewNegX:
            m_pGLViewTypeEdit->setCurrentItem( 3 );
            break;
         case PMGLView::PMViewPosZ:
            m_pGLViewTypeEdit->setCurrentItem( 4 );
            break;
         case PMGLView::PMViewNegZ:
            m_pGLViewTypeEdit->setCurrentItem( 5 );
            break;
         case PMGLView::PMViewCamera:
            m_pGLViewTypeEdit->setCurrentItem( 6 );
            break;
      }
      */
      switch( ( *m_currentViewEntry ).dockPosition( ) )
      {
         case PMDockWidget::DockRight:
            m_pDockPositionEdit->setCurrentItem( 0 );
            m_pColumnWidthLabel->show( );
            m_pColumnWidthEdit->show( );
            m_pViewHeightEdit->show( );
            m_pViewHeightLabel->show( );
            m_pFloatingWidth->hide( );
            m_pFloatingHeight->hide( );
            m_pFloatingPosX->hide( );
            m_pFloatingPosY->hide( );
            m_pFloatingWidthLabel->hide( );
            m_pFloatingHeightLabel->hide( );
            m_pFloatingPosXLabel->hide( );
            m_pFloatingPosYLabel->hide( );
            break;
         case PMDockWidget::DockBottom:
            m_pDockPositionEdit->setCurrentItem( 1 );
            m_pColumnWidthLabel->hide( );
            m_pColumnWidthEdit->hide( );
            m_pViewHeightEdit->show( );
            m_pViewHeightLabel->show( );
            m_pFloatingWidth->hide( );
            m_pFloatingHeight->hide( );
            m_pFloatingPosX->hide( );
            m_pFloatingPosY->hide( );
            m_pFloatingWidthLabel->hide( );
            m_pFloatingHeightLabel->hide( );
            m_pFloatingPosXLabel->hide( );
            m_pFloatingPosYLabel->hide( );
            break;
         case PMDockWidget::DockCenter:
            m_pDockPositionEdit->setCurrentItem( 2 );
            m_pColumnWidthLabel->hide( );
            m_pColumnWidthEdit->hide( );
            m_pViewHeightEdit->hide( );
            m_pViewHeightLabel->hide( );
            m_pFloatingWidth->hide( );
            m_pFloatingHeight->hide( );
            m_pFloatingPosX->hide( );
            m_pFloatingPosY->hide( );
            m_pFloatingWidthLabel->hide( );
            m_pFloatingHeightLabel->hide( );
            m_pFloatingPosXLabel->hide( );
            m_pFloatingPosYLabel->hide( );
            break;
         default:
            m_pDockPositionEdit->setCurrentItem( 3 );
            m_pColumnWidthLabel->hide( );
            m_pColumnWidthEdit->hide( );
            m_pViewHeightEdit->hide( );
            m_pViewHeightLabel->hide( );
            m_pFloatingWidth->show( );
            m_pFloatingHeight->show( );
            m_pFloatingPosX->show( );
            m_pFloatingPosY->show( );
            m_pFloatingWidthLabel->show( );
            m_pFloatingHeightLabel->show( );
            m_pFloatingPosXLabel->show( );
            m_pFloatingPosYLabel->show( );
            break;
      }
      m_pColumnWidthEdit->setValue( ( *m_currentViewEntry ).columnWidth( ) );
      m_pViewHeightEdit->setValue( ( *m_currentViewEntry ).height( ) );
      m_pFloatingWidth->setValue( ( *m_currentViewEntry ).floatingWidth( ) );
      m_pFloatingHeight->setValue( ( *m_currentViewEntry ).floatingHeight( ) );
      m_pFloatingPosX->setValue( ( *m_currentViewEntry ).floatingPositionX( ) );
      m_pFloatingPosY->setValue( ( *m_currentViewEntry ).floatingPositionY( ) );
      m_pViewEntries->triggerUpdate( );
      displayCustomOptions( );
   }
   else
   {
      m_pViewTypeEdit->setEnabled( false );
      m_pDockPositionEdit->setEnabled( false );
      m_pColumnWidthLabel->hide( );
      m_pColumnWidthEdit->hide( );
      m_pViewHeightEdit->hide( );
      m_pViewHeightLabel->hide( );
      m_pFloatingWidth->hide( );
      m_pFloatingHeight->hide( );
      m_pFloatingPosX->hide( );
      m_pFloatingPosY->hide( );
      m_pFloatingWidthLabel->hide( );
      m_pFloatingHeightLabel->hide( );
      m_pFloatingPosXLabel->hide( );
      m_pFloatingPosYLabel->hide( );
   }
}

void PMLayoutSettings::slotViewTypeChanged( int index )
{
   const TQPtrList<PMViewTypeFactory>& types =
      PMViewFactory::theFactory( )->viewTypes( );
   TQPtrListIterator<PMViewTypeFactory> it( types );
   it += index;
   const PMViewTypeFactory* factory = *it;
   PMViewLayoutEntry& ve = ( *m_currentViewEntry );
   
   if( factory && factory->viewType( ) != ve.viewType( ) )
   {
      ve.setViewType( factory->viewType( ) );
      ve.setCustomOptions( factory->newOptionsInstance( ) );

      TQListViewItem* item = m_pViewEntries->currentItem( );
      if( item )
      {
         if( ve.customOptions( ) )
            item->setText( 1, factory->description( ve.customOptions( ) ) );
         else
            item->setText( 1, factory->description( ) );
         displayCustomOptions( );
      }
   }
}

void PMLayoutSettings::slotDockPositionChanged( int index )
{
   switch( index )
   {
      case 0:
         ( *m_currentViewEntry ).setDockPosition( PMDockWidget::DockRight );
         m_pColumnWidthLabel->show( );
         m_pColumnWidthEdit->show( );
         m_pViewHeightEdit->show( );
         m_pViewHeightLabel->show( );
         m_pFloatingWidth->hide( );
         m_pFloatingHeight->hide( );
         m_pFloatingPosX->hide( );
         m_pFloatingPosY->hide( );
         m_pFloatingWidthLabel->hide( );
         m_pFloatingHeightLabel->hide( );
         m_pFloatingPosXLabel->hide( );
         m_pFloatingPosYLabel->hide( );
         break;
      case 1:
         ( *m_currentViewEntry ).setDockPosition( PMDockWidget::DockBottom );
         m_pColumnWidthLabel->hide( );
         m_pColumnWidthEdit->hide( );
         m_pViewHeightEdit->show( );
         m_pViewHeightLabel->show( );
         m_pFloatingWidth->hide( );
         m_pFloatingHeight->hide( );
         m_pFloatingPosX->hide( );
         m_pFloatingPosY->hide( );
         m_pFloatingWidthLabel->hide( );
         m_pFloatingHeightLabel->hide( );
         m_pFloatingPosXLabel->hide( );
         m_pFloatingPosYLabel->hide( );
         break;
      case 2:
         ( *m_currentViewEntry ).setDockPosition( PMDockWidget::DockCenter );
         m_pColumnWidthLabel->hide( );
         m_pColumnWidthEdit->hide( );
         m_pViewHeightEdit->hide( );
         m_pViewHeightLabel->hide( );
         m_pFloatingWidth->hide( );
         m_pFloatingHeight->hide( );
         m_pFloatingPosX->hide( );
         m_pFloatingPosY->hide( );
         m_pFloatingWidthLabel->hide( );
         m_pFloatingHeightLabel->hide( );
         m_pFloatingPosXLabel->hide( );
         m_pFloatingPosYLabel->hide( );
         break;
      case 3:
         ( *m_currentViewEntry ).setDockPosition( PMDockWidget::DockNone );
         m_pColumnWidthLabel->hide( );
         m_pColumnWidthEdit->hide( );
         m_pViewHeightEdit->hide( );
         m_pViewHeightLabel->hide( );
         m_pFloatingWidth->show( );
         m_pFloatingHeight->show( );
         m_pFloatingPosX->show( );
         m_pFloatingPosY->show( );
         m_pFloatingWidthLabel->show( );
         m_pFloatingHeightLabel->show( );
         m_pFloatingPosXLabel->show( );
         m_pFloatingPosYLabel->show( );
         break;
   }
   TQListViewItem* item = m_pViewEntries->currentItem( );
   if( item )
      item->setText( 2, ( *m_currentViewEntry ).dockPositionAsString( ) );
}

void PMLayoutSettings::slotViewHeightChanged( const TQString& text )
{
   ( *m_currentViewEntry ).setHeight( text.toInt( ) );
}

void PMLayoutSettings::slotColumnWidthChanged( const TQString& text )
{
   ( *m_currentViewEntry ).setColumnWidth( text.toInt( ) );
}

void PMLayoutSettings::slotFloatingWidthChanged( const TQString& text )
{
   ( *m_currentViewEntry ).setFloatingWidth( text.toInt( ) );
}

void PMLayoutSettings::slotFloatingHeightChanged( const TQString& text )
{
   ( *m_currentViewEntry ).setFloatingHeight( text.toInt( ) );
}

void PMLayoutSettings::slotFloatingPosXChanged( const TQString& text )
{
   ( *m_currentViewEntry ).setFloatingPositionX( text.toInt( ) );
}

void PMLayoutSettings::slotFloatingPosYChanged( const TQString& text )
{
   ( *m_currentViewEntry ).setFloatingPositionY( text.toInt( ) );
}

void PMLayoutSettings::slotAddViewEntryClicked( )
{
   PMViewLayoutEntry p;
   TQString str;
   TQListViewItem* temp;

   temp = m_pViewEntries->currentItem( );
   if( temp )
   {
      int n_item = temp->text( 0 ).toInt( );
      ( *m_currentViewLayout ).addEntry( p, n_item );
      n_item++;
      str.setNum( n_item );
      TQListViewItem* a = new TQListViewItem( m_pViewEntries, temp,
                                            str, p.viewTypeAsString( ),
                                            p.dockPositionAsString( ) );
      m_pViewEntries->setSelected( a, true );
      temp = a->nextSibling( );
      while( temp )
      {
         n_item++;
         str.setNum( n_item );
         temp->setText( 0, str );
         temp = temp->nextSibling( );
      }
   } 
   else
   {
      // If there is no selected the list must be empty
      ( *m_currentViewLayout ).addEntry( p );
      str.setNum( 1 );
      TQListViewItem* a = new TQListViewItem( m_pViewEntries, NULL,
                                            str, p.viewTypeAsString( ),
                                            p.dockPositionAsString( ) );
      m_pViewEntries->setSelected( a, true );
   }
}

void PMLayoutSettings::slotRemoveViewEntryClicked( )
{
   TQListViewItem* temp;
   TQString str;

   TQListViewItem* current = m_pViewEntries->currentItem( );
   if( current )
   {
      int n_item = current->text( 0 ).toInt( ) - 1;
      ( *m_currentViewLayout ).removeEntry( n_item );

      // Save the next selected item in temp, since the current item will
      // be removed.
      temp = current->nextSibling( );
      if( !temp )
         temp = current->itemAbove( );
      else
         n_item++;

      delete current;

      if( temp )
      {
         str.setNum( n_item );
         temp->setText( 0, str );
         m_pViewEntries->setSelected( temp, true );
         n_item++;
         temp = temp->nextSibling( );
      }
      else
         slotViewEntrySelected( 0 );
      while( temp )
      {
         str.setNum( n_item );
         temp->setText( 0, str );
         n_item++;
         temp = temp->nextSibling( );
      }
   }
}

void PMLayoutSettings::slotMoveUpViewEntryClicked( )
{
}

void PMLayoutSettings::slotMoveDownViewEntryClicked( )
{
}

void PMLayoutSettings::displayCustomOptions( )
{
   // delete an old widget
   if( m_pCustomOptionsHolder->tqlayout( ) )
      delete m_pCustomOptionsHolder->tqlayout( );
   if( m_pCustomOptionsWidget )
   {
      delete m_pCustomOptionsWidget;
      m_pCustomOptionsWidget = 0;
   }
   
   if( m_currentViewLayout != m_viewLayouts.end( ) &&
       m_currentViewEntry != ( *m_currentViewLayout ).end( ) &&
       ( *m_currentViewEntry ).customOptions( ) )
   {
      PMViewTypeFactory* vf = PMViewFactory::theFactory( )->viewFactory(
         ( *m_currentViewEntry ).viewType( ) );
      if( vf )
      {
         m_pCustomOptionsWidget =
            vf->newOptionsWidget( m_pCustomOptionsHolder,
                                  ( *m_currentViewEntry ).customOptions( ) );
         if( m_pCustomOptionsWidget )
         {
            connect( m_pCustomOptionsWidget, TQT_SIGNAL( viewTypeDescriptionChanged( ) ),
                     TQT_SLOT( slotViewTypeDescriptionChanged( ) ) );
            TQHBoxLayout* hl = new TQHBoxLayout( m_pCustomOptionsHolder,
                                               0, KDialog::spacingHint( ) );
            hl->addWidget( m_pCustomOptionsWidget );
            m_pCustomOptionsWidget->show( );
         }
      }
   }
}

void PMLayoutSettings::slotViewTypeDescriptionChanged( )
{
   PMViewLayoutEntry& ve = *m_currentViewEntry;
   const PMViewTypeFactory* factory =
      PMViewFactory::theFactory( )->viewFactory( ve.viewType( ) );
   
   if( factory )
   {
      TQListViewItem* item = m_pViewEntries->currentItem( );
      if( item )
      {
         if( ve.customOptions( ) )
            item->setText( 1, factory->description( ve.customOptions( ) ) );
         else
            item->setText( 1, factory->description( ) );
      }
   }
}

#include "pmlayoutsettings.moc"
