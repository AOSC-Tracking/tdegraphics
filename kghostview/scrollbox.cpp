/** 
 * Copyright (C) 1997-2002 the KGhostView authors. See file AUTHORS.
 * 	
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <tqdrawutil.h>
#include <tqpainter.h>
#include <tqpixmap.h>

#include "scrollbox.h"

ScrollBox::ScrollBox( TQWidget* tqparent, const char* name )
    : TQFrame( tqparent, name )
{
    setFrameStyle( Panel | Sunken );
}

void ScrollBox::mousePressEvent( TQMouseEvent* e )
{
    mouse = e->pos();
    if( e->button() == Qt::RightButton )
	emit button3Pressed();
    if( e->button() == Qt::MidButton )
	emit button2Pressed();
}

void ScrollBox::mouseMoveEvent( TQMouseEvent* e )
{
    if( e->state() != Qt::LeftButton )
	return;

    int dx = ( e->pos().x() - mouse.x() ) * pagesize.width()  / width();
    int dy = ( e->pos().y() - mouse.y() ) * pagesize.height() / height();

    // Notify the word what the view position has changed
    // The word in turn will notify as that view position has changed
    // Even if coordinates are out of range TQScrollView handles
    // this properly
    emit valueChanged( TQPoint( viewpos.x() + dx, viewpos.y() + dy ) );
    emit valueChangedRelative( dx, dy );

    mouse = e->pos();
}

void ScrollBox::resizeEvent( TQResizeEvent * )
{
    if ( paletteBackgroundPixmap() ) {
	TQPixmap pm;
	pm.convertFromImage(paletteBackgroundPixmap()->convertToImage().smoothScale( size() ));
	setPaletteBackgroundPixmap( pm );
    }
}

void ScrollBox::drawContents( TQPainter* paint )
{
    if ( pagesize.isEmpty() )
	return;


    /* FIXME:
     *
     * The logic below is flawed because the page info given to us
     * contains the borders used for page decoration, while we assume
     * that it means only the actual displayed document.
     *
     */

    TQRect c( contentsRect() );

    paint -> setPen( TQt::red );

    int len = pagesize.width();
    int x = c.x() + c.width() * viewpos.x() / len;
    int w = c.width() * viewsize.width() / len ;
    if ( w > c.width() ) w = c.width();

    len = pagesize.height();
    int y = c.y() + c.height() * viewpos.y() / len;
    int h = c.height() * viewsize.height() / len;
    if ( h > c.height() ) h = c.height();

    paint->drawRect( x, y, w, h );
}

void ScrollBox::setPageSize( const TQSize& s )
{
    pagesize = s;
    setFixedHeight( s.height() * width() / s.width() );
    tqrepaint();
}

void ScrollBox::setViewSize( const TQSize& s )
{
    viewsize = s;
    tqrepaint();
}

void ScrollBox::setViewPos( const TQPoint& pos )
{
    viewpos = pos;
    tqrepaint();
}

void ScrollBox::setThumbnail( TQPixmap img )
{
    // The line below is needed to work around certain "features" of styles such as liquid
    // see bug:61711 for more info (LPC, 20 Aug '03)
    setBackgroundOrigin( TQWidget::WidgetOrigin );
    TQPixmap pm;
    pm.convertFromImage(img.convertToImage().smoothScale( size() ));
    setPaletteBackgroundPixmap( pm );
}

void ScrollBox::clear()
{
    unsetPalette();
}

#include "scrollbox.moc"

// vim:sw=4:sts=4:ts=8:noet
