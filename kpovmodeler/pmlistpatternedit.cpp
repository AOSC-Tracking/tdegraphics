/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001 by Luis Carvalho
    email                : lpassos@mail.telepac.pt
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#include "pmlistpatternedit.h"
#include "pmlistpattern.h"
#include "pmvectoredit.h"
#include "pmvector.h"

#include <tqwidget.h>
#include <layout.h>
#include <tqcombobox.h>
#include <tqlabel.h>
#include "pmlineedits.h"
#include <ktabctl.h>
#include <klocale.h>
#include <kmessagebox.h>


PMListPatternEdit::PMListPatternEdit( TQWidget* parent, const char* name )
      : Base( parent, name )
{
   m_pDisplayedObject = 0;
}

void PMListPatternEdit::createTopWidgets( )
{
   Base::createTopWidgets( );

   TQVBoxLayout* vlayout = new TQVBoxLayout( topLayout( ) );

   /* Field for Pattern List type */
   TQHBoxLayout* layout = new TQHBoxLayout( vlayout );
   TQLabel* label = new TQLabel( i18n( "Type:" ), this );
   m_pTypeCombo = new TQComboBox( false, this );
   m_pTypeCombo->insertItem( i18n( "Checkers" ) );
   m_pTypeCombo->insertItem( i18n( "Brick" ) );
   m_pTypeCombo->insertItem( i18n( "Hexagon" ) );
   layout->addWidget( label, 0, AlignTop );
   layout->addWidget( m_pTypeCombo );
   layout->addStretch( 1 );

   /* The depth field */
   layout = new TQHBoxLayout( vlayout );
   m_pDepthLabel = new TQLabel( i18n( "Depth:" ), this );
   m_pDepth = new PMFloatEdit( this );
   layout->addWidget( m_pDepthLabel );
   layout->addWidget( m_pDepth );
   layout->addStretch( 1 );

   /* The brick information */
   TQHBoxLayout* bricklayout = new TQHBoxLayout( vlayout );
   m_pBrickSizeLabel = new TQLabel( i18n( "Brick size:" ), this );
   m_pBrickSize = new PMVectorEdit( "x", "y", "z", this );
   bricklayout->addWidget( m_pBrickSizeLabel );
   bricklayout->addWidget( m_pBrickSize );
   layout = new TQHBoxLayout( vlayout );
   m_pMortarLabel = new TQLabel( i18n( "Mortar:" ), this );
   m_pMortar = new PMFloatEdit( this );
   layout->addWidget( m_pMortarLabel );
   layout->addWidget( m_pMortar );
   layout->addStretch( 1 );

   /* connect all signals to slots/signals */
   connect( m_pBrickSize, TQT_SIGNAL( dataChanged( ) ), TQT_SIGNAL( dataChanged( ) ) );
   connect( m_pTypeCombo, TQT_SIGNAL( activated( int ) ), TQT_SLOT( slotComboChanged( int ) ) );
   connect( m_pMortar, TQT_SIGNAL( dataChanged( ) ), TQT_SIGNAL( dataChanged( ) ) );
   connect( m_pDepth, TQT_SIGNAL( dataChanged( ) ), TQT_SIGNAL( dataChanged( ) ) );
}

void PMListPatternEdit::displayObject( PMObject* o )
{
   TQString str;

   if( o->isA( "ListPattern" ) )
   {
      m_pDisplayedObject = ( PMListPattern* ) o;

      switch( m_pDisplayedObject->listType( ) )
      {
         case PMListPattern::ListPatternChecker:
            m_pTypeCombo->setCurrentItem( 0 );
            m_pBrickSizeLabel->hide( );
            m_pBrickSize->hide( );
            m_pMortarLabel->hide( );
            m_pMortar->hide( );
            break;
         case PMListPattern::ListPatternBrick:
            m_pTypeCombo->setCurrentItem( 1 );
            m_pBrickSizeLabel->show( );
            m_pBrickSize->show( );
            m_pMortarLabel->show( );
            m_pMortar->show( );
            break;
         case PMListPattern::ListPatternHexagon:
            m_pTypeCombo->setCurrentItem( 2 );
            m_pBrickSizeLabel->hide( );
            m_pBrickSize->hide( );
            m_pMortarLabel->hide( );
            m_pMortar->hide( );
            break;
      }
      m_pMortar->setValue( m_pDisplayedObject->mortar( ) );
      m_pBrickSize->setVector( m_pDisplayedObject->brickSize( ) );
      if( o->type( ) == "NormalList" )
      {
         m_pDepth->setValue( ( ( PMNormalList* )o )->depth( ) );
         m_pDepth->show( );
         m_pDepthLabel->show( );
         emit sizeChanged( );
      }
      else
      {
         m_pDepth->hide( );
         m_pDepthLabel->hide( );
         emit sizeChanged( );
      }
      Base::displayObject( o );
   }
   else
      kdError( PMArea ) << "PMListPatternEdit: Can't display object\n";
}

void PMListPatternEdit::saveContents( )
{
   if( m_pDisplayedObject )
   {
      Base::saveContents( );
      switch( m_pTypeCombo->currentItem( ) )
      {
         case 0:
            m_pDisplayedObject->setListType( PMListPattern::ListPatternChecker );
            break;
         case 1:
            m_pDisplayedObject->setListType( PMListPattern::ListPatternBrick );
            m_pDisplayedObject->setMortar( m_pMortar->value( ) );
            m_pDisplayedObject->setBrickSize( m_pBrickSize->vector( ) );
            break;
         case 2:
            m_pDisplayedObject->setListType( PMListPattern::ListPatternHexagon );
            break;
      }
      if( m_pDisplayedObject->type( ) == "NormalList" )
         ( ( PMNormalList* )m_pDisplayedObject )->setDepth( m_pDepth->value( ) );
   }
}

bool PMListPatternEdit::isDataValid( )
{
   int children = 0;
   PMObject* o;

   if( !m_pBrickSize->isDataValid( ) )
      return false;
   if( !m_pMortar->isDataValid( ) )
      return false;

   // count child objects
   for( o = m_pDisplayedObject->firstChild( ); o; o = o->nextSibling( ) )
      if( o->type( ) == m_pDisplayedObject->listObjectType( ) )
         children++;

   switch( m_pTypeCombo->currentItem( ) )
   {
      case 0:
      case 1:
         if( children > 2 )
         {
            KMessageBox::error( this, i18n( "You can have at most two child"
                                            " items for that list type!" ),
                                i18n( "Error" ) );
            return false;
         }
         break;
      default:
         break;
   }

   return Base::isDataValid( );
}

void PMListPatternEdit::slotComboChanged( int c )
{
   switch( c )
   {
      case 0:
         m_pBrickSizeLabel->hide( );
         m_pBrickSize->hide( );
         m_pMortarLabel->hide( );
         m_pMortar->hide( );
         break;
      case 1:
         m_pBrickSizeLabel->show( );
         m_pBrickSize->show( );
         m_pMortarLabel->show( );
         m_pMortar->show( );
         break;
      case 2:
         m_pBrickSizeLabel->hide( );
         m_pBrickSize->hide( );
         m_pMortarLabel->hide( );
         m_pMortar->hide( );
         break;
      default:
         break;
   }
   emit sizeChanged( );
   emit dataChanged( );
}

#include "pmlistpatternedit.moc"
