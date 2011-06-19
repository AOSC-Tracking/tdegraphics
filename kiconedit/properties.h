/*
    kiconedit - a small graphics drawing program the KDE.

    Copyright (C) 1998 Thomas Tanghus (tanghus@kde.org)

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef __PROPS_H__
#define __PROPS_H__

#include <tqwidget.h>

#include "kicongrid.h"

class KIconEditProperties : public TQObject
{
public:
  static KIconEditProperties* self();
  ~KIconEditProperties();

  TQString bgPixmap() { return m_bgPixmap; }
  TQColor bgColor() { return m_bgColor; }
  TQWidget::BackgroundMode bgMode() { return m_bgMode; }
  bool showGrid() { return m_showGrid; }
  bool pasteTransparent() { return m_pasteTransparent; }
  bool showRulers() { return m_showRulers; }
  int gridScale() { return m_gridScale; }
  
  KIconEditGrid::TransparencyDisplayType transparencyDisplayType() const { return m_transparencyDisplayType; }
  TQColor checkerboardColor1() const { return m_checkerboardColor1; }
  TQColor checkerboardColor2() const { return m_checkerboardColor2; }
  KIconEditGrid::CheckerboardSize checkerboardSize() const { return m_checkerboardSize; }
  TQColor transparencySolidColor() const { return m_transparencySolidColor; }

  void setBgPixmap( const TQString &p ) { m_bgPixmap = p; }
  void setBgColor( const TQColor &c ) { m_bgColor = c; }
  void setBgMode( TQWidget::BackgroundMode m ) { m_bgMode = m; }
  void setShowGrid( bool b ) { m_showGrid = b; }
  void setPasteTransparent( bool b ) { m_pasteTransparent = b; }
  void setShowRulers( bool b ) { m_showRulers = b; }
  void setGridScale( int s ) { m_gridScale = s; }
  void setTransparencyDisplayType(KIconEditGrid::TransparencyDisplayType t) { m_transparencyDisplayType = t; }
  void setCheckerboardColor1(const TQColor& c) { m_checkerboardColor1 = c; }
  void setCheckerboardColor2(const TQColor& c) { m_checkerboardColor2 = c; }
  void setCheckerboardSize(KIconEditGrid::CheckerboardSize size) { m_checkerboardSize = size; }
  void setTransparencySolidColor(const TQColor& c) { m_transparencySolidColor = c; }

  void save();

protected:
  KIconEditProperties();
  TQString m_bgPixmap;
  TQColor m_bgColor;
  TQWidget::BackgroundMode m_bgMode;
  bool m_showGrid;
  bool m_pasteTransparent;
  bool m_showRulers;
  int m_gridScale;
  KIconEditGrid::TransparencyDisplayType m_transparencyDisplayType;
  TQColor m_checkerboardColor1;
  TQColor m_checkerboardColor2;
  KIconEditGrid::CheckerboardSize m_checkerboardSize;
  TQColor m_transparencySolidColor;

  static KIconEditProperties* m_self;
};

#endif //__PROPS_H__
