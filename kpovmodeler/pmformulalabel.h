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


#ifndef PMFORMULALABEL_H
#define PMFORMULALABEL_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tqwidget.h>

class PMPolynomExponents;

/**
 * TQLabel with a rich text to display a polynom for the quadric,
 * cubic, quartic and polynom objects.
 */
class PMFormulaLabel : public TQWidget
{
public:
   /**
    * Displays the exponents of the @ref PMPolynomExponents
    */
   PMFormulaLabel( const PMPolynomExponents& exp, TQWidget* parent, const char* name = 0 );
   /**
    * Displays the given exponents
    */
   PMFormulaLabel( int x, int y, int z, TQWidget* parent, const char* name = 0 );
   /**
    * Destructor
    */
   ~PMFormulaLabel( );
   virtual TQSize sizeHint( ) const;
   virtual TQSize minimumSizeHint( ) const;
   
protected:
   virtual void drawContents( TQPainter* p );
   virtual void paintEvent( TQPaintEvent* e );
   virtual void fontChange( const TQFont& oldFont );
   
private:
   TQFont exponentFont( ) const;
   void calculateSizeHint( );
   
   TQSize m_sizeHint;
   int m_exponents[3];
   
   static TQString s_xyz[3];
   static TQString s_digit[10];
   static TQString s_nullString;
};

#endif
