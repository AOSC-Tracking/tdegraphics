/*
    Copyright (C) 2001-2003 KSVG Team
    This file is part of the KDE project

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

#ifndef KSVGWidget_H
#define KSVGWidget_H

#include <tqwidget.h>

#include <kpopupmenu.h>

#include <SVGEventImpl.h>

class KSVGPlugin;
class KSVGWidget : public TQWidget
{
Q_OBJECT
  TQ_OBJECT
public:
	KSVGWidget(KSVGPlugin *part, TQWidget *tqparent, const char *name);
	virtual ~KSVGWidget();

	KSVGPlugin *part() const;

	void reset();

protected:
	virtual void paintEvent(TQPaintEvent *);
	
	virtual void mouseMoveEvent(TQMouseEvent *);
	virtual void mousePressEvent(TQMouseEvent *);
	virtual void mouseReleaseEvent(TQMouseEvent *);

	virtual void keyPressEvent(TQKeyEvent *);
	virtual void keyReleaseEvent(TQKeyEvent *);

	virtual void resizeEvent(TQResizeEvent *);

signals:
	void browseURL(const TQString &);
	void redraw(const TQRect &);

private:
	KSVG::SVGMouseEventImpl *newMouseEvent(KSVG::SVGEvent::EventId id, TQMouseEvent *event);

	TQPoint m_panningPos;
	TQPoint m_oldPanningPos;
	KSVGPlugin *m_part;
};

#endif

// vim:ts=4:noet
