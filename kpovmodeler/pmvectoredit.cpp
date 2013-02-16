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


#include "pmvectoredit.h"
#include "pmdebug.h"
#include <tqstring.h>
#include <tqlayout.h>
#include <tqlabel.h>
#include <kdialog.h>
#include <tdemessagebox.h>
#include <tdelocale.h>


PMVectorEdit::PMVectorEdit( const TQString& descriptionX,
                            const TQString& descriptionY,
                            TQWidget* parent, const char* name )
      : TQWidget( parent, name )
{
   unsigned int i;
   TQHBoxLayout* layout;
   TQLabel* label;

   m_edits.resize( 2 );
   for( i = 0; i < 2; i++ )
   {
      m_edits.insert( i, new TQLineEdit( this ) );
      connect( m_edits[i], TQT_SIGNAL( textChanged( const TQString& ) ),
               TQT_SLOT( slotTextChanged( const TQString& ) ) );
   }

   layout = new TQHBoxLayout( this );

   if( !descriptionX.isEmpty( ) )
   {
      label = new TQLabel( descriptionX, this );
      layout->addWidget( label );
      layout->addSpacing( KDialog::spacingHint( ) );
   }
   layout->addWidget( m_edits[0] );
   layout->addSpacing( KDialog::spacingHint( ) );

   if( !descriptionY.isEmpty( ) )
   {
      label = new TQLabel( descriptionY, this );
      layout->addWidget( label );
      layout->addSpacing( KDialog::spacingHint( ) );
   }
   layout->addWidget( m_edits[1] );
}

PMVectorEdit::PMVectorEdit( const TQString& descriptionX,
                            const TQString& descriptionY,
                            const TQString& descriptionZ,
                            TQWidget* parent, const char* name )
      : TQWidget( parent, name )
{
   unsigned int i;
   TQHBoxLayout* layout;
   TQLabel* label;

   m_edits.resize( 3 );
   for( i = 0; i < 3; i++ )
   {
      m_edits.insert( i, new TQLineEdit( this ) );
      connect( m_edits[i], TQT_SIGNAL( textChanged( const TQString& ) ),
               TQT_SLOT( slotTextChanged( const TQString& ) ) );
   }

   layout = new TQHBoxLayout( this );

   if( !descriptionX.isEmpty( ) )
   {
      label = new TQLabel( descriptionX, this );
      layout->addWidget( label );
      layout->addSpacing( KDialog::spacingHint( ) );
   }
   layout->addWidget( m_edits[0] );
   layout->addSpacing( KDialog::spacingHint( ) );

   if( !descriptionY.isEmpty( ) )
   {
      label = new TQLabel( descriptionY, this );
      layout->addWidget( label );
      layout->addSpacing( KDialog::spacingHint( ) );
   }
   layout->addWidget( m_edits[1] );
   layout->addSpacing( KDialog::spacingHint( ) );

   if( !descriptionZ.isEmpty( ) )
   {
      label = new TQLabel( descriptionZ, this );
      layout->addWidget( label );
      layout->addSpacing( KDialog::spacingHint( ) );
   }
   layout->addWidget( m_edits[2] );
}

PMVectorEdit::PMVectorEdit( const TQString& descriptionA,
                            const TQString& descriptionB,
                            const TQString& descriptionC,
                            const TQString& descriptionD,
                            TQWidget* parent, const char* name )
      : TQWidget( parent, name )
{
   unsigned int i;
   TQHBoxLayout* layout;
   TQLabel* label;

   m_edits.resize( 4 );
   for( i = 0; i < 4; i++ )
   {
      m_edits.insert( i, new TQLineEdit( this ) );
      connect( m_edits[i], TQT_SIGNAL( textChanged( const TQString& ) ),
               TQT_SLOT( slotTextChanged( const TQString& ) ) );
   }

   layout = new TQHBoxLayout( this );

   if( !descriptionA.isEmpty( ) )
   {
      label = new TQLabel( descriptionA, this );
      layout->addWidget( label );
      layout->addSpacing( KDialog::spacingHint( ) );
   }
   layout->addWidget( m_edits[0] );
   layout->addSpacing( KDialog::spacingHint( ) );

   if( !descriptionB.isEmpty( ) )
   {
      label = new TQLabel( descriptionB, this );
      layout->addWidget( label );
      layout->addSpacing( KDialog::spacingHint( ) );
   }
   layout->addWidget( m_edits[1] );
   layout->addSpacing( KDialog::spacingHint( ) );

   if( !descriptionC.isEmpty( ) )
   {
      label = new TQLabel( descriptionC, this );
      layout->addWidget( label );
      layout->addSpacing( KDialog::spacingHint( ) );
   }
   layout->addWidget( m_edits[2] );
   layout->addSpacing( KDialog::spacingHint( ) );

   if( !descriptionD.isEmpty( ) )
   {
      label = new TQLabel( descriptionD, this );
      layout->addWidget( label );
      layout->addSpacing( KDialog::spacingHint( ) );
   }
   layout->addWidget( m_edits[3] );
}

void PMVectorEdit::setVector( const PMVector& v, int precision )
{
   unsigned int i;
   TQString str;

   if( v.size( ) != m_edits.size( ) )
      kdError( PMArea ) << "Vector has wrong size in PMVectorEdit::setVector\n";

   for( i = 0; ( i < m_edits.size( ) ) && ( i < v.size( ) ); i++ )
   {
      str.setNum( v[i], 'g', precision );
      m_edits[i]->setText( str );
   }
}

PMVector PMVectorEdit::vector( ) const
{
   PMVector result( m_edits.size( ) );
   unsigned int i;

   for( i = 0; i < m_edits.size( ); i++ )
      result[i] = m_edits[i]->text( ).toDouble( );

   return result;
}

bool PMVectorEdit::isDataValid( )
{
   bool ok = true;
   unsigned int i;

   for( i = 0; ( i < m_edits.size( ) ) && ok; i++ )
   {
      m_edits[i]->text( ).toDouble( &ok );
      if( !ok )
      {
         KMessageBox::error( this, i18n( "Please enter a valid float value!" ),
                             i18n( "Error" ) );
         m_edits[i]->setFocus( );
         m_edits[i]->selectAll( );
      }
   }

   return ok;
}

void PMVectorEdit::slotTextChanged( const TQString& )
{
   emit dataChanged( );
}

void PMVectorEdit::setReadOnly( bool yes )
{
   unsigned int i;
   for( i = 0; ( i < m_edits.size( ) ); i++ )
      m_edits[i]->setReadOnly( yes );
}

void PMVectorEdit::setEnabled( bool yes )
{
   unsigned int i;
   for( i = 0; ( i < m_edits.size( ) ); i++ )
      m_edits[i]->setEnabled( yes );
}

#include "pmvectoredit.moc"
