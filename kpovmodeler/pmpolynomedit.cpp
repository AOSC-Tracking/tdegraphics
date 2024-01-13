/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Andreas Zehender
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


#include "pmpolynomedit.h"
#include "pmpolynom.h"
#include "pmpolynomexponents.h"
#include "pmlineedits.h"
#include "pmformulalabel.h"

#include <tqlayout.h>
#include <tqcheckbox.h>
#include <tqspinbox.h>
#include <tqlabel.h>
#include <tdelocale.h>
#include <kdialog.h>

PMPolynomEdit::PMPolynomEdit( TQWidget* parent, const char* name )
      : Base( parent, name )
{
   m_pDisplayedObject = 0;
   m_currentOrder = 0;
   m_readOnly = false;
}

void PMPolynomEdit::createTopWidgets( )
{
   Base::createTopWidgets( );

   TQHBoxLayout* hl = new TQHBoxLayout( topLayout( ) );
   hl->addWidget( new TQLabel( i18n( "Order" ), this ) );
   m_pOrder = new TQSpinBox( 2, 7, 1, this );
   hl->addWidget( m_pOrder );
   hl->addStretch( 1 );
   connect( m_pOrder, TQ_SIGNAL( valueChanged( int ) ), TQ_SLOT( slotOrderChanged( int ) ) );
   
   topLayout( )->addWidget( new TQLabel( i18n( "Formula:" ), this ) );
   m_pPolyWidget = new TQWidget( this );
   topLayout( )->addWidget( m_pPolyWidget );
   m_pSturm = new TQCheckBox( i18n( "Sturm" ), this );
   topLayout( )->addWidget( m_pSturm );
   connect( m_pSturm, TQ_SIGNAL( clicked( ) ), TQ_SIGNAL( dataChanged( ) ) );
}

void PMPolynomEdit::displayObject( PMObject* o )
{
   if( o->isA( "Polynom" ) )
   {
      bool readOnly = o->isReadOnly( );
      m_readOnly = readOnly;
      m_pDisplayedObject = ( PMPolynom* ) o;

      displayCoefficients( m_pDisplayedObject->coefficients( ),
                           m_pDisplayedObject->polynomOrder( ),
                           m_pDisplayedObject->polynomOrder( ) );
      
      m_pSturm->setChecked( m_pDisplayedObject->sturm( ) );
      m_pSturm->setEnabled( !readOnly );
      if( m_pDisplayedObject->polynomOrder( ) == 2 )
         m_pSturm->hide( );
      else
         m_pSturm->show( );

      bool sb = m_pOrder->signalsBlocked( );
      m_pOrder->blockSignals( true );
      m_pOrder->setValue( m_pDisplayedObject->polynomOrder( ) );
      m_pOrder->blockSignals( sb );
      
      Base::displayObject( o );
   }
   else
      kdError( PMArea ) << "PMPolynomEdit: Can't display object\n";
}

void PMPolynomEdit::displayCoefficients( const PMVector& co, int cOrder,
                                         int dOrder )
{
   TQValueList<PMPolynomExponents>& polynom
      = PMPolynomExponents::polynom( dOrder );
      
   if( dOrder != m_currentOrder )
   {
      if( m_currentOrder > 0 )
      {
         if( m_pPolyWidget->layout( ) )
            delete m_pPolyWidget->layout( );
         
         m_labels.setAutoDelete( true );
         m_labels.clear( );
         m_labels.setAutoDelete( false );
         m_edits.setAutoDelete( true );
         m_edits.clear( );
         m_edits.setAutoDelete( false );
      }

      int nedit = polynom.count( );
      int nr = ( nedit + 2 ) / 3;
      int i;
      
      TQGridLayout* gl = new TQGridLayout( m_pPolyWidget, nr * 2 - 1, 9, 0 );
      TQLabel* l = 0;
      PMFloatEdit* edit;
      PMFormulaLabel* fl;
      
      TQValueList<PMPolynomExponents>::ConstIterator it = polynom.begin( );
      TQString text;
      int row, col;
      TQString plus( "+" );
      
      for( i = 0; it != polynom.end( ); i++, ++it )
      {
         row = ( i / 3 ) * 2;
         col = ( i % 3 ) * 3;

         if( i != 0 )
         {
            l = new TQLabel( plus, m_pPolyWidget );
            m_labels.append( l );
            gl->addWidget( l, row, col );
            l->show( );
         }
         
         edit = new PMFloatEdit( m_pPolyWidget );
         connect( edit, TQ_SIGNAL( dataChanged( ) ), TQ_SIGNAL( dataChanged( ) ) );
         m_edits.append( edit );
         gl->addWidget( edit, row, col + 1 );
         edit->show( );
         edit->setReadOnly( m_readOnly );

         fl = new PMFormulaLabel( *it, m_pPolyWidget );
         
         m_labels.append( fl );
         gl->addWidget( fl, row, col + 2 );
         fl->show( );
      }
      for( i = 0; i < ( nr - 1 ); i++ )
         gl->addRowSpacing( i * 2 + 1, KDialog::spacingHint( ) );
      
      emit sizeChanged( );
   }
   m_currentOrder = dOrder;

   
   if( dOrder == cOrder )
   {
      TQPtrListIterator<PMFloatEdit> eit( m_edits );
      int i;
      for( i = 0; *eit; ++eit, ++i )
         ( *eit )->setValue( co[i] );
   }
   else if( dOrder > cOrder )
   {
      TQValueList<PMPolynomExponents>::ConstIterator dit = polynom.begin( );
      TQValueList<PMPolynomExponents>& cpoly
         = PMPolynomExponents::polynom( cOrder );
      TQValueList<PMPolynomExponents>::ConstIterator cit = cpoly.begin( );
      TQPtrListIterator<PMFloatEdit> eit( m_edits );
      int i = 0;
      
      for( ; ( dit != polynom.end( ) ) && ( cit != cpoly.end( ) ); ++dit, ++eit )
      {
         if( *dit == *cit )
         {
            ( *eit )->setValue( co[i] );
            i++;
            cit++;
         }
         else
            ( *eit )->setValue( 0.0 );
      }
      if( ( dit != polynom.end( ) ) || ( cit != cpoly.end( ) ) )
         kdError( PMArea ) << "displayExponents is buggy!\n";
   } 
   else // dOrder < cOrder
   {
      TQValueList<PMPolynomExponents>::ConstIterator dit = polynom.begin( );
      TQValueList<PMPolynomExponents>& cpoly
         = PMPolynomExponents::polynom( cOrder );
      TQValueList<PMPolynomExponents>::ConstIterator cit = cpoly.begin( );
      TQPtrListIterator<PMFloatEdit> eit( m_edits );
      int i = 0;
      
      for( ; ( dit != polynom.end( ) ) && ( cit != cpoly.end( ) ); ++cit, ++i )
      {
         if( *dit == *cit )
         {
            ( *eit )->setValue( co[i] );
            ++eit;
            ++dit;
         }
      }
      if( ( dit != polynom.end( ) ) || ( cit != cpoly.end( ) ) )
         kdError( PMArea ) << "displayExponents is buggy!\n";
   }
}

PMVector PMPolynomEdit::coefficients( ) const
{
   TQPtrListIterator<PMFloatEdit> eit( m_edits );
   int num = m_edits.count( );
   PMVector v( num );
   int i;
   
   for( i = 0 ; *eit; ++eit, ++i )
      v[i] = ( *eit )->value( );
   return v;
}

void PMPolynomEdit::saveContents( )
{
   if( m_pDisplayedObject )
   {
      m_pDisplayedObject->setPolynomOrder( m_pOrder->value( ) );
      m_pDisplayedObject->setCoefficients( coefficients( ) );
      
      Base::saveContents( );

      m_pDisplayedObject->setSturm( m_pSturm->isChecked( ) );
   }
}

bool PMPolynomEdit::isDataValid( )
{
   TQPtrListIterator<PMFloatEdit> eit( m_edits );
   for( ; *eit; ++eit )
      if( !( *eit )->isDataValid( ) )
         return false;
   return Base::isDataValid( );
}

void PMPolynomEdit::slotOrderChanged( int order )
{
   if( order == 2 )
      m_pSturm->hide( );
   else
      m_pSturm->show( );
   
   displayCoefficients( coefficients( ), m_currentOrder, order );
}

#include "pmpolynomedit.moc"
