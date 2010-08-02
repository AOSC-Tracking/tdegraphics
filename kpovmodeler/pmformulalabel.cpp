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

#include "pmformulalabel.h"
#include "pmpolynomexponents.h"

#include <tqapplication.h>
#include <tqsimplerichtext.h>
#include <tqpainter.h>

const int c_indent = 3;
const int c_dotSize = 3;

TQString PMFormulaLabel::s_xyz[3] =
{
   TQString( "x" ), TQString( "y" ), TQString( "z" )
};

TQString PMFormulaLabel::s_digit[10] =
{
   TQString( "0" ),
   TQString( "1" ),
   TQString( "2" ),
   TQString( "3" ),
   TQString( "4" ),
   TQString( "5" ),
   TQString( "6" ),
   TQString( "7" ),
   TQString( "8" ),
   TQString( "9" )
};

TQString PMFormulaLabel::s_nullString = TQString( "= 0" );

PMFormulaLabel::PMFormulaLabel( const PMPolynomExponents& exp, TQWidget* parent, const char* name )
      : TQWidget( parent, name )
{
   m_exponents[0] = exp.exponent( 0 );
   m_exponents[1] = exp.exponent( 1 );
   m_exponents[2] = exp.exponent( 2 );

   calculateSizeHint( );
}

PMFormulaLabel::PMFormulaLabel( int x, int y, int z, TQWidget* parent, const char* name )
      : TQWidget( parent, name )
{
   m_exponents[0] = x;
   m_exponents[1] = y;
   m_exponents[2] = z;

   calculateSizeHint( );
}

PMFormulaLabel::~PMFormulaLabel( )
{
}

void PMFormulaLabel::drawContents( TQPainter* p )
{
   TQRect cr = rect( );
   int i;
   cr.setLeft( cr.left( ) + c_indent );
   
   int sum = m_exponents[0] + m_exponents[1] + m_exponents[2];
   if( sum == 0 )
      p->drawText( cr, Qt::AlignVCenter | Qt::AlignLeft, s_nullString );
   else
   {
      // draw dot
      int center = ( cr.top( ) + cr.bottom( ) ) / 2;
      int rad = c_dotSize / 2;
      p->setBrush( TQBrush( colorGroup( ).text( ) ) );
      p->drawEllipse( cr.left( ), center - rad, c_dotSize, c_dotSize );
      cr.setLeft( cr.left( ) + c_dotSize + c_indent );

      TQFontMetrics m1( font( ) );
      TQFont f2 = exponentFont( );
      TQFontMetrics m2( f2 );
      int up = m1.height( ) / 2;
      
      for( i = 0; i < 3; i++ )
      {
         
         if( m_exponents[i] > 0 )
         {
            p->drawText( cr, Qt::AlignVCenter | Qt::AlignLeft, s_xyz[i] );
            cr.setLeft( cr.left( ) + m1.width( s_xyz[i] ) );
            if( m_exponents[i] > 1 )
            {
               cr.setBottom( cr.bottom( ) - up );
               p->setFont( f2 );
               p->drawText( cr, Qt::AlignVCenter | Qt::AlignLeft,
                            s_digit[m_exponents[i]] );
               cr.setLeft( cr.left( ) + m2.width( s_digit[m_exponents[i]] ) + 1 );
               cr.setBottom( cr.bottom( ) + up );
               p->setFont( font( ) );
            }
         }
      }
   }
}

void PMFormulaLabel::paintEvent( TQPaintEvent* ev )
{
   TQPainter paint( this );
   if( ev->rect( ).intersects( rect( ) ) )
   {
      paint.setClipRegion( ev->region( ).intersect( rect( ) ) );
      drawContents( &paint );
   }
}

void PMFormulaLabel::calculateSizeHint( )
{
   int sum = m_exponents[0] + m_exponents[1] + m_exponents[2];

   TQFontMetrics m1( font( ) );
   if( sum == 0 )
      m_sizeHint.setWidth( m1.width( s_nullString ) );
   else
   {
      TQFontMetrics m2( exponentFont( ) );
      int width = c_indent * 3 + c_dotSize;
      int i;
      for( i = 0; i < 3; i++ )
      {
         if( m_exponents[i] > 0 )
         {
            width += m1.width( s_xyz[i] );
            if( m_exponents[i] > 1 )
               width += m2.width( s_digit[m_exponents[i]] ) + 1;
         }
      }
      m_sizeHint.setWidth( width );
   }
   m_sizeHint.setHeight( m1.height( ) + 7 );
}

TQSize PMFormulaLabel::sizeHint( ) const
{
   return minimumSizeHint( );
}

TQSize PMFormulaLabel::minimumSizeHint( ) const
{
   return m_sizeHint;
}

void PMFormulaLabel::fontChange( const TQFont& )
{
   calculateSizeHint( );
}


TQFont PMFormulaLabel::exponentFont( ) const
{
   TQFont small = font( );
   int fs = small.pointSize( );
   if( fs > 0 )
   {
      fs = fs * 2 / 3;
      if( fs == 0 )
         fs = 1;
      small.setPointSize( fs );
   }
   else
   {
      fs = small.pixelSize( );
      fs = fs * 2 / 3;
      if( fs == 0 )
         fs = 1;
      small.setPixelSize( fs );
   }
   return small;
}
