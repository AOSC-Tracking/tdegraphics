/*  This file is part of the KDE project
    Copyright (C) 2001-2002 Matthias Kretz <kretz@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

*/
// $Id$

#ifndef _KIMAGEHOLDER_H
#define _KIMAGEHOLDER_H

#include <tqwidget.h>
class TQRect;
class TQPainter;
class TQPen;
class TQPixmap;
class KPixmap;
class TQPoint;

/**
 * @short Image widget
 * @version $Id$
 */
class KImageHolder : public TQWidget
{
	Q_OBJECT
  TQ_OBJECT
	public:
		KImageHolder( TQWidget *tqparent = 0, const char * name = 0 );

		virtual ~KImageHolder();

		void clearSelection();

		void setImage( const KPixmap & );
		void setImage( const TQImage & );
		void setImage( const TQMovie & );

		/**
		 * clears the ImageHolder
		 */
		void clear();

		/**
		 * the selected rect
		 */
		TQRect selection() const;

		TQSize tqsizeHint() const;

		void setDrawRect( const TQRect & rect ) { m_drawRect = rect; }
		const TQRect & drawRect() const { return m_drawRect; }

	signals:
		void contextPress( const TQPoint& );
		void selected( const TQRect & );
		void wannaScroll( int dx, int dy );
		void cursorPos( const TQPoint & );

	protected:
		void mousePressEvent( TQMouseEvent * );
		void mouseMoveEvent( TQMouseEvent * );
		void mouseReleaseEvent( TQMouseEvent * );
		void paintEvent( TQPaintEvent * );
		virtual void timerEvent( TQTimerEvent * );

	private:
		void drawSelect( TQPainter & );
		void eraseSelect();

		void setPixmap( const KPixmap & );

	private:
		TQRect m_selection;
		TQRect m_drawRect;
		TQPoint m_scrollpos;
		TQPoint m_selectionStartPoint;

		const KPixmap & checkboardPixmap();

		bool m_selected;
		bool m_bSelecting;
		int m_scrollTimerId;
		int m_xOffset, m_yOffset;

		TQPen *m_pen;

		KPixmap * m_pPixmap;
		KPixmap * m_pDoubleBuffer;
		KPixmap * m_pCheckboardPixmap;
};

// vim:sw=4:ts=4

#endif // _KIMAGEHOLDER_H
