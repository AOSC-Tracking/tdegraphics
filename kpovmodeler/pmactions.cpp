/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2002 by Andreas Zehender
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

#include "pmactions.h"

#include <tqcombobox.h>
#include <tqwhatsthis.h>
#include <tqspinbox.h>
#include <tqlabel.h>
#include <tqstyle.h>
#include <tqpainter.h>
#include <tdetoolbar.h>
#include <tdetoolbarbutton.h>

#include "pmdebug.h"

// Fixed widths are calculated wrong in a toolbar.
// Fixed sizeHint for the combo box to return
// at least the minimum size
class PMComboBox : public TQComboBox
{
public:
   PMComboBox( TQWidget* parent, const char* name = 0 )
         : TQComboBox( parent, name )
   {
   }

   virtual TQSize minimumSizeHint( ) const
   {
      TQSize s = TQComboBox::minimumSizeHint( );
      return s.expandedTo( minimumSize( ) );
   }
   virtual TQSize sizeHint( ) const
   {
      TQSize s = TQComboBox::sizeHint( );
      return s.expandedTo( minimumSize( ) );
   }
};

PMComboAction::PMComboAction( const TQString& text, int accel, const TQObject* receiver, const char* member,
                              TQObject* parent, const char* name )
      : TDEAction( text, accel, parent, name )
{
   m_receiver = receiver;
   m_member = member;
   m_minWidth = 0;
   m_maxWidth = 0;
}

PMComboAction::~PMComboAction( )
{
}

int PMComboAction::plug( TQWidget* w, int index )
{
   if( !w->inherits( "TDEToolBar" ) )
      return -1;

   TDEToolBar* toolBar = ( TDEToolBar* ) w;

   int id = TDEAction::getToolButtonID( );

   TQComboBox* comboBox = new PMComboBox( toolBar );
   if( m_minWidth > 0 )
      comboBox->setMinimumWidth( m_minWidth );
   if( m_maxWidth > 0 )
      comboBox->setMaximumWidth( m_maxWidth );

   toolBar->insertWidget( id, m_minWidth > 0 ? m_minWidth : 300,
                          comboBox, index );
   connect( comboBox, TQT_SIGNAL( activated( int ) ), m_receiver, m_member );

   addContainer( toolBar, id );

   connect( toolBar, TQT_SIGNAL( destroyed( ) ), this, TQT_SLOT( slotDestroyed( ) ) );

   //toolBar->setItemAutoSized( id, true );

   m_combo = comboBox;

   emit plugged( );

   TQWhatsThis::add( comboBox, whatsThis( ) );

   return containerCount( ) - 1;
}

void PMComboAction::unplug( TQWidget *w )
{
   if( !w->inherits( "TDEToolBar" ) )
      return;

   TDEToolBar *toolBar = ( TDEToolBar* ) w;

   int idx = findContainer( w );

   toolBar->removeItem( itemId( idx ) );

   removeContainer( idx );
   m_combo = 0L;
}


// Use a toolbutton instead of a label so it is styled correctly.
// copied from konq_actions.cc
class PMToolBarLabel : public TQToolButton
{
public:
   PMToolBarLabel( const TQString& text, TQWidget* parent = 0, const char* name = 0 )
         : TQToolButton( parent, name )
   {
      setText( text );
   }
protected:
   TQSize sizeHint( ) const
   {
      int w = fontMetrics( ).width( text( ) );
      int h = fontMetrics( ).height( );
      return TQSize( w, h );
   }
   void drawButton( TQPainter* p )
   {
      // Draw the background
      style( ).drawComplexControl( TQStyle::CC_ToolButton, p, this, rect( ), colorGroup( ),
                                   TQStyle::Style_Enabled, TQStyle::SC_ToolButton );
      // Draw the label
      style( ).drawControl( TQStyle::CE_ToolButtonLabel, p, this, rect( ), colorGroup( ),
                            TQStyle::Style_Enabled );
   }
};

PMLabelAction::PMLabelAction( const TQString &text, TQObject *parent, const char *name )
    : TDEAction( text, 0, parent, name )
{
   m_button = 0;
}

int PMLabelAction::plug( TQWidget *widget, int index )
{
   //do not call the previous implementation here

   if( widget->inherits( "TDEToolBar" ) )
   {
      TDEToolBar* tb = ( TDEToolBar* ) widget;

      int id = TDEAction::getToolButtonID( );

      m_button = new PMToolBarLabel( text( ), widget );
      tb->insertWidget( id, m_button->width( ), m_button, index );

      addContainer( tb, id );

      connect( tb, TQT_SIGNAL( destroyed( ) ), this, TQT_SLOT( slotDestroyed( ) ) );

      return containerCount( ) - 1;
  }

  return -1;
}

void PMLabelAction::unplug( TQWidget *widget )
{
   if( widget->inherits( "TDEToolBar" ) )
   {
      TDEToolBar* bar = ( TDEToolBar* ) widget;
      
      int idx = findContainer( bar );
      
      if( idx != -1 )
      {
         bar->removeItem( itemId( idx ) );
         removeContainer( idx );
      }
      
      m_button = 0;
      return;
   }
}


PMSpinBoxAction::PMSpinBoxAction( const TQString& text, int accel, const TQObject* receiver, const char* member,
                                  TQObject* parent, const char* name )
      : TDEAction( text, accel, parent, name )
{
   m_receiver = receiver;
   m_member = member;
}

PMSpinBoxAction::~PMSpinBoxAction( )
{
}

int PMSpinBoxAction::plug( TQWidget* w, int index )
{
   if( !w->inherits( "TDEToolBar" ) )
      return -1;

   TDEToolBar* toolBar = ( TDEToolBar* ) w;

   int id = TDEAction::getToolButtonID( );
   
   TQSpinBox* spinBox = new TQSpinBox( -1000, 1000, 1, w );
   toolBar->insertWidget( id, 70, spinBox, index );
   
   connect( spinBox, TQT_SIGNAL( valueChanged( int ) ), m_receiver, m_member );

   addContainer( toolBar, id );

   connect( toolBar, TQT_SIGNAL( destroyed( ) ), this, TQT_SLOT( slotDestroyed( ) ) );
   //toolBar->setItemAutoSized( id, false );

   m_spinBox = spinBox;

   emit plugged( );

   TQWhatsThis::add( spinBox, whatsThis( ) );

   return containerCount( ) - 1;
}

void PMSpinBoxAction::unplug( TQWidget *w )
{
   if( !w->inherits( "TDEToolBar" ) )
      return;

   TDEToolBar *toolBar = (TDEToolBar *)w;

   int idx = findContainer( w );

   toolBar->removeItem( itemId( idx ) );

   removeContainer( idx );
   m_spinBox = 0L;
}

#include "pmactions.moc"
