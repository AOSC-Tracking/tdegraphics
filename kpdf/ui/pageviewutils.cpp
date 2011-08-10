/***************************************************************************
 *   Copyright (C) 2004 by Enrico Ros <eros.kde@email.it>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

// qt/kde includes
#include <tqbitmap.h>
#include <tqpainter.h>
#include <tqimage.h>
#include <tqtimer.h>
#include <kapplication.h>
#include <kimageeffect.h>
#include <kiconloader.h>

// local includes
#include "pageviewutils.h"
#include "core/page.h"
#include "conf/settings.h"

PageViewMessage::PageViewMessage( TQWidget * parent )
    : TQWidget( parent, "pageViewMessage" ), m_timer( 0 )
{
    setFocusPolicy( TQ_NoFocus );
    setBackgroundMode( NoBackground );
    setPaletteBackgroundColor(kapp->palette().color(TQPalette::Active, TQColorGroup::Background));
    // if the tqlayout is LtR, we can safely place it in the right position
    if ( !TQApplication::reverseLayout() )
        move( 10, 10 );
    resize( 0, 0 );
    hide();
}

void PageViewMessage::display( const TQString & message, Icon icon, int durationMs )
// give to Caesar what Caesar owns: code taken from Amarok's osd.h/.cpp
// "redde (reddite, pl.) cesari quae sunt cesaris", just btw. ;)
{
    if ( !KpdfSettings::showOSD() )
    {
        hide();
        return;
    }

    // determine text rectangle
    TQRect textRect = fontMetrics().boundingRect( message );
    textRect.moveBy( -textRect.left(), -textRect.top() );
    textRect.addCoords( 0, 0, 2, 2 );
    int width = textRect.width(),
        height = textRect.height(),
        textXOffset = 0,
        iconXOffset = 0,
        shadowOffset = 1;

    // load icon (if set) and update tqgeometry
    TQPixmap symbol;
    if ( icon != None )
    {
        switch ( icon )
        {
            case Find:
                symbol = SmallIcon( "viewmag" );
                break;
            case Error:
                symbol = SmallIcon( "messagebox_critical" );
                break;
            case Warning:
                symbol = SmallIcon( "messagebox_warning" );
                break;
            default:
                symbol = SmallIcon( "messagebox_info" );
                break;
        }
        if ( TQApplication::reverseLayout() )
        {
            iconXOffset = 2 + textRect.width();
        }
        else
        {
            textXOffset = 2 + symbol.width();
        }
        width += 2 + symbol.width();
        height = TQMAX( height, symbol.height() );
    }
    TQRect tqgeometry( 0, 0, width + 10, height + 8 );

    // resize pixmap, mask and widget
    static TQBitmap mask;
    mask.resize( tqgeometry.size() );
    m_pixmap.resize( tqgeometry.size() );
    resize( tqgeometry.size() );

    // create and set transparency mask
    TQPainter maskPainter( &mask);
    mask.fill( TQt::black );
    maskPainter.setBrush( TQt::white );
    maskPainter.drawRoundRect( tqgeometry, 1600 / tqgeometry.width(), 1600 / tqgeometry.height() );
    setMask( mask );

    // draw background
    TQPainter bufferPainter( &m_pixmap );
    bufferPainter.setPen( TQt::black );
    bufferPainter.setBrush( paletteBackgroundColor() );
    bufferPainter.drawRoundRect( tqgeometry, 1600 / tqgeometry.width(), 1600 / tqgeometry.height() );

    // draw icon if present
    if ( !symbol.isNull() )
        bufferPainter.drawPixmap( 5 + iconXOffset, 4, symbol, 0, 0, symbol.width(), symbol.height() );

    // draw shadow and text
    int yText = tqgeometry.height() - height / 2;
    bufferPainter.setPen( paletteBackgroundColor().dark( 115 ) );
    bufferPainter.drawText( 5 + textXOffset + shadowOffset, yText + 1, message );
    bufferPainter.setPen( foregroundColor() );
    bufferPainter.drawText( 5 + textXOffset, yText, message );

    // if the tqlayout is RtL, we can move it to the right place only after we
    // know how much size it will take
    if ( TQApplication::reverseLayout() )
        move( parentWidget()->width() - tqgeometry.width() - 10, 10 );

    // show widget and schedule a tqrepaint
    show();
    update();

    // close the message window after given mS
    if ( durationMs > 0 )
    {
        if ( !m_timer )
        {
            m_timer = new TQTimer( this );
            connect( m_timer, TQT_SIGNAL( timeout() ), TQT_SLOT( hide() ) );
        }
        m_timer->start( durationMs, true );
    } else if ( m_timer )
        m_timer->stop();
}

void PageViewMessage::paintEvent( TQPaintEvent * e )
{
    TQPainter p( this );
    p.drawPixmap( e->rect().topLeft(), m_pixmap, e->rect() );
}

void PageViewMessage::mousePressEvent( TQMouseEvent * /*e*/ )
{
    if ( m_timer )
        m_timer->stop();
    hide();
}



PageViewItem::PageViewItem( const KPDFPage * page )
    : m_page( page ), m_zoomFactor( 1.0 )
{
}

const KPDFPage * PageViewItem::page() const
{
    return m_page;
}

int PageViewItem::pageNumber() const
{
    return m_page->number();
}

const TQRect& PageViewItem::tqgeometry() const
{
    return m_tqgeometry;
}

int PageViewItem::width() const
{
    return m_tqgeometry.width();
}

int PageViewItem::height() const
{
    return m_tqgeometry.height();
}

double PageViewItem::zoomFactor() const
{
    return m_zoomFactor;
}

void PageViewItem::setGeometry( int x, int y, int width, int height )
{
    m_tqgeometry.setRect( x, y, width, height );
}

void PageViewItem::setWHZ( int w, int h, double z )
{
    m_tqgeometry.setWidth( w );
    m_tqgeometry.setHeight( h );
    m_zoomFactor = z;
}

void PageViewItem::moveTo( int x, int y )
{
    m_tqgeometry.moveLeft( x );
    m_tqgeometry.moveTop( y );
}
