/*
  Copyright (C) 2003 Nadeem Hasan <nhasan@kde.org>

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

#ifndef REGIONGRABBER_H
#define REGIONGRABBER_H

#include <tqlabel.h>
#include <tqpixmap.h>

class TQTimer;

class SizeTip : public QLabel
{
  public:
    SizeTip( TQWidget *parent, const char *name=0 );
    ~SizeTip() {}

  void setTip( const TQRect &rect );
  void positionTip( const TQRect &rect );
};

class RegionGrabber : public QWidget
{
  Q_OBJECT

  public:
    RegionGrabber();
    ~RegionGrabber();

  protected slots:
    void initGrabber();
    void updateSizeTip();

  signals:
    void regionGrabbed( const TQPixmap & );

  protected:
    void mousePressEvent( TQMouseEvent *e );
    void mouseReleaseEvent( TQMouseEvent *e );
    void mouseMoveEvent( TQMouseEvent *e );
    void keyPressEvent( TQKeyEvent *e );

    void drawRubber();

    bool mouseDown;
    TQRect grabRect;
    TQPixmap pixmap;

    SizeTip *sizeTip;
    TQTimer *tipTimer;
};

#endif // REGIONGRABBER_H

