/***************************************************************************
 *   Copyright (C) 2004 by Enrico Ros <eros.kde@email.it>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef _PAGEVIEW_UTILS_H
#define _PAGEVIEW_UTILS_H

#include <tqwidget.h>
#include <tqpixmap.h>
#include <tqpainter.h>
#include <tqrect.h>

class TQTimer;

class PageView;
class KPDFPage;

/**
 * @short PageViewItem represents graphically a kpdfpage into the PageView.
 *
 * It has methods for settings Item's geometry and other visual properties such
 * as the individual zoom factor.
 */
class PageViewItem
{
    public:
        PageViewItem( const KPDFPage * page );

        const KPDFPage * page() const;
        int pageNumber() const;
        const TQRect& geometry() const;
        int width() const;
        int height() const;
        double zoomFactor() const;

        void setGeometry( int x, int y, int width, int height );
        void setWHZ( int w, int h, double zoom );
        void moveTo( int x, int y );

    private:
        const KPDFPage * m_page;
        double m_zoomFactor;
        TQRect m_geometry;
};


/**
 * @short A widget that displays messages in the top-left corner.
 */
class PageViewMessage : public TQWidget
{
    public:
        PageViewMessage( TQWidget * parent );

        enum Icon { None, Info, Warning, Error, Find };
        void display( const TQString & message, Icon icon = Info, int durationMs = 4000 );

    protected:
        void paintEvent( TQPaintEvent * e );
        void mousePressEvent( TQMouseEvent * e );

    private:
        TQPixmap m_pixmap;
        TQTimer * m_timer;
};

#endif
