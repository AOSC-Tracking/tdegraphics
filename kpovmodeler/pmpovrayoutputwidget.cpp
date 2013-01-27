/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001 by Andreas Zehender
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

#include "pmpovrayoutputwidget.h"

#include <tqtextedit.h>
#include <tqlayout.h>

#include <klocale.h>
#include <kglobalsettings.h>
#include <tdeconfig.h>
#include <kpushbutton.h>
#include <kstdguiitem.h>

#include "pmdebug.h"

TQSize PMPovrayOutputWidget::s_size = TQSize( 400, 400 );

PMPovrayOutputWidget::PMPovrayOutputWidget( TQWidget* parent, const char* name )
      : KDialog( parent, name )
{
   TQVBoxLayout* topLayout = new TQVBoxLayout( this, KDialog::marginHint( ), KDialog::spacingHint( ) );

   m_pTextView = new TQTextEdit( this );
   topLayout->addWidget( m_pTextView, 1 );
   m_pTextView->setFont( TDEGlobalSettings::fixedFont( ) );
   m_pTextView->setTextFormat( TQt::PlainText );
   m_pTextView->setReadOnly( true );

   TQHBoxLayout* buttonLayout = new TQHBoxLayout( topLayout );
   buttonLayout->addStretch( 1 );
   TQPushButton* closeButton = new KPushButton( KStdGuiItem::close(), this );
   buttonLayout->addWidget( closeButton );
   closeButton->setDefault( true );
   connect( closeButton, TQT_SIGNAL( clicked( ) ), TQT_SLOT( hide( ) ) );

   setCaption( i18n( "Povray Output" ) );
   resize( s_size );

   m_startOfLastLine = 0;
}

PMPovrayOutputWidget::~PMPovrayOutputWidget( )
{
}

void PMPovrayOutputWidget::slotClear( )
{
   m_output = TQString();
   m_startOfLastLine = 0;
   m_pTextView->clear( );
}

void PMPovrayOutputWidget::slotText( const TQString& output )
{
   unsigned int i;

   for( i = 0; i < output.length( ); i++ )
   {
      TQChar c = output[i];
      if( c == '\r' )
         m_output.truncate( m_startOfLastLine );
      else if( c == '\n' )
      {
         m_output += c;
         m_startOfLastLine = m_output.length( );
         //kdDebug( PMArea ) << m_startOfLastLine << endl;
      }
      else if( c.isPrint( ) )
         m_output += c;
   }

   m_pTextView->setText( m_output );
}

void PMPovrayOutputWidget::slotClose( )
{
   hide( );
}


void PMPovrayOutputWidget::saveConfig( TDEConfig* cfg )
{
   cfg->setGroup( "Appearance" );
   cfg->writeEntry( "PovrayOutputWidgetSize", s_size );
}

void PMPovrayOutputWidget::restoreConfig( TDEConfig* cfg )
{
   cfg->setGroup( "Appearance" );

   TQSize defaultSize( 500, 400 );
   s_size = cfg->readSizeEntry( "PovrayOutputWidgetSize", &defaultSize );
}

void PMPovrayOutputWidget::resizeEvent( TQResizeEvent* ev )
{
   s_size = ev->size( );
}

#include "pmpovrayoutputwidget.moc"
