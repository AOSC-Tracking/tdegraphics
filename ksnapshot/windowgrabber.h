/*
  Copyright (C) 2004 Bernd Brandstetter <bbrand@freenet.de>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or ( at your option ) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this library; see the file COPYING.  If not, write to
  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301, USA.
*/

#ifndef WINDOWGRABBER_H
#define WINDOWGRABBER_H

#include <tqdialog.h>
#include <tqpixmap.h>
#include <vector>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

class WindowGrabber : public TQDialog
{
    TQ_OBJECT
  

public:
    WindowGrabber();
    ~WindowGrabber();

    static TQPixmap grabCurrent( bool includeDecorations = true );

signals:
    void windowGrabbed( const TQPixmap & );

protected:
    void mousePressEvent( TQMouseEvent * );
    void mouseReleaseEvent( TQMouseEvent * );
    void mouseMoveEvent( TQMouseEvent * );
    void wheelEvent( TQWheelEvent * );

private:
    void drawBorder();
    void increaseScope( const TQPoint & );
    void decreaseScope( const TQPoint & );
    int windowIndex( const TQPoint & ) const;
    std::vector<TQRect> windows;
    int current;
    int yPos;
};


#endif // WINDOWGRABBER_H
