/* This file is part of the KDE Project
   Copyright (C) 2000 Klaas Freitag <freitag@suse.de>  

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include <tqpainter.h>
#include <tqpixmap.h>

#include "dispgamma.h"

DispGamma::DispGamma( TQWidget *parent ) : TQWidget( parent )
{
    vals = 0;
    margin = 10;
}

DispGamma::~DispGamma()
{

}

void DispGamma::resizeEvent (TQResizeEvent* )
{
    tqrepaint();
}

void DispGamma::paintEvent( TQPaintEvent *ev )
{
    TQPainter p(this);
    int w = vals->size() +1;

    // Viewport auf margin setzen.
    p.setViewport( margin, margin, width() - margin, height() - margin );
    p.setWindow( 0, 255, w, -256 );

    p.setClipRect( ev->rect());

    p.setPen( tqcolorGroup().highlight() );
    p.setBrush( tqcolorGroup().base() );
    // Backgrond
    p.drawRect( 0,0, w, 256 );
    p.setPen( TQPen(tqcolorGroup().midlight(), 1, DotLine));
    // horizontal Grid
    for( int l = 1; l < 5; l++ )
            p.drawLine( 1, l*51, 255, l*51 );

    // vertical Grid
    for( int l = 1; l < 5; l++ )
            p.drawLine( l*51, 2, l*51, 255 );

    // draw gamma-Line
    p.setPen( tqcolorGroup().highlight() );
    p.moveTo( 1, vals->at(1) );
    for( int i = 2; i < w-1; i++ )
    {
        p.lineTo( i, vals->at(i) );
    }
    p.flush();
}


TQSize DispGamma::tqsizeHint( void )
{
    return TQSize( 256 + 2*margin,256 + 2 * margin );
}

TQSizePolicy DispGamma::sizePolicy( void )
{
    return TQSizePolicy( TQSizePolicy::MinimumExpanding, TQSizePolicy::MinimumExpanding );
}
#include "dispgamma.moc"
