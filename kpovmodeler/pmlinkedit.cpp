/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001-2002 by Andreas Zehender
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

#include "pmlinkedit.h"
#include <tqlineedit.h>
#include <tqlabel.h>
#include <layout.h>
#include <klocale.h>
#include <kdialog.h>
#include <kpushbutton.h>
#include <kstdguiitem.h>

#include "pmdeclare.h"
#include "pmobjectselect.h"

PMLinkEdit::PMLinkEdit( const TQString& declareType,
                        TQWidget* parent, const char* name )
      : TQWidget( parent, name )
{
   m_declareTypes.append( declareType );
   init( );
}

PMLinkEdit::PMLinkEdit( const TQStringList& declareTypes,
                        TQWidget* parent, const char* name )
      : TQWidget( parent, name )
{
   m_declareTypes = declareTypes;
   init( );
}

PMLinkEdit::PMLinkEdit( TQWidget* parent, const char* name )
      : TQWidget( parent, name )
{
   init( );
}

void PMLinkEdit::init( )
{
   m_pDeclare = 0;
   m_pDisplayedObject = 0;
   m_bReadOnly = false;

   TQGridLayout* grid = new TQGridLayout( this, 2, 2, 0, KDialog::spacingHint( ) );

   grid->addWidget( new TQLabel( i18n( "Prototype:" ), this ), 0, 0 );
   grid->setColStretch( 0, 0 );
   grid->setColStretch( 1, 1 );
   m_pIDEdit = new TQLineEdit( this );
   m_pIDEdit->setReadOnly( true );
   grid->addWidget( m_pIDEdit, 0, 1 );

   TQHBoxLayout* layout = new TQHBoxLayout( );
   grid->addLayout( layout, 1, 1 );
   m_pSelectButton = new TQPushButton( i18n( "Select..." ), this );
   layout->addWidget( m_pSelectButton );
   m_pClearButton = new KPushButton( KStdGuiItem::clear(), this );
   layout->addWidget( m_pClearButton );

   connect( m_pSelectButton, TQT_SIGNAL( clicked( ) ), TQT_SLOT( slotSelectClicked( ) ) );
   connect( m_pClearButton, TQT_SIGNAL( clicked( ) ), TQT_SLOT( slotClearClicked( ) ) );
}

void PMLinkEdit::setDisplayedObject( PMObject* obj )
{
   m_pDisplayedObject = obj;
   m_pDeclare = obj->linkedObject( );
   if( m_pDeclare )
   {
      m_pIDEdit->setText( m_pDeclare->id( ) );
      if( !m_bReadOnly )
         m_pClearButton->setEnabled( true );
   }
   else
   {
      m_pIDEdit->clear( );
      if( !m_bReadOnly )
         m_pClearButton->setEnabled( false );
   }
}

void PMLinkEdit::setLinkPossibility( const TQString& t )
{
   m_declareTypes.clear( );
   m_declareTypes.append( t );
}


void PMLinkEdit::setLinkPossibilities( const TQStringList& t )
{
   m_declareTypes = t;
}

void PMLinkEdit::setReadOnly( bool yes )
{
   m_bReadOnly = yes;
   m_pClearButton->setEnabled( !m_bReadOnly && m_pDeclare );
   m_pSelectButton->setEnabled( !m_bReadOnly );
}

void PMLinkEdit::slotSelectClicked( )
{
   if( m_pDisplayedObject )
   {
      PMObject* obj = 0;
      int result;

      if( m_declareTypes.count( ) == 1 )
         result = PMObjectSelect::selectDeclare(
            m_pDisplayedObject, m_declareTypes.first( ), obj, this );
      else
         result = PMObjectSelect::selectDeclare(
            m_pDisplayedObject, m_declareTypes, obj, this );

      if( ( result == TQDialog::Accepted ) && obj )
      {
         m_pDeclare = ( PMDeclare* ) obj;
         m_pIDEdit->setText( m_pDeclare->id( ) );
         m_pClearButton->setEnabled( true );
         emit dataChanged( );
      }
   }
}

void PMLinkEdit::slotClearClicked( )
{
   m_pDeclare = 0;
   m_pIDEdit->clear( );
   emit dataChanged( );
}

#include "pmlinkedit.moc"
