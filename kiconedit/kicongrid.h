/*
    KDE Icon Editor - a small graphics drawing program for the KDE.
    Copyright (C) 1998  Thomas Tanghus (tanghus@kde.org)

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

#ifndef __KICONEDITGRID_H__
#define __KICONEDITGRID_H__

#include <tqpixmap.h>
#include <tqimage.h>
#include <tqcursor.h>
#include <tqpointarray.h>
#include <tqframe.h>
#include <kcommand.h>
#include <klocale.h>

#include "kcolorgrid.h"

class KCommandHistory;
class KRuler;
class KIconEditGrid;
class TQScrollView;

enum Direction {
    DirIn = 0, DirOut = 1,
    DirUp = DirIn, DirDown = DirOut,
    DirLeft, DirRight
};

class DrawCommand : public KCommand {
    public:
        DrawCommand( int xx, int yy, uint newcol, TQImage* img, KIconEditGrid* g ) {
            x = xx;
	    y = yy;
	    newcolor = newcol;
	    image = img;
	    grid = g;
        }
        
        void execute();
        void unexecute();
        TQString name() const {
            return i18n("Drawed Something");
        }

    protected:
        int x;
	int y;
        uint newcolor;
        uint oldcolor;
	TQImage* image;
	KIconEditGrid* grid;
};

class RepaintCommand : public KCommand {
    public:
	RepaintCommand( TQRect a, KIconEditGrid* g ) {
		area = a;
		grid = g;
	}
	
	void execute();
	void unexecute() {
		execute();
	}
	
	TQString name() const {
		return "repainted";
	}
    protected:
	KIconEditGrid* grid;
	TQRect area;
};

class KGridView : public TQFrame
{
    Q_OBJECT
  TQ_OBJECT
public:
  KGridView( TQImage *image, KCommandHistory* history, TQWidget * tqparent = 0, const char *name = 0);

  KRuler *hruler() { return _hruler;}
  KRuler *vruler() { return _vruler;}
  TQFrame *corner() { return _corner;}
  KIconEditGrid *grid() { return _grid; }
  void setShowRulers(bool mode);
  void setAcceptDrop(bool a);
  const TQRect viewRect();
  TQScrollView *viewPortWidget() { return viewport;}

public slots:
  void sizeChange(int, int);
  void moving(int, int);
  void scalingChange(int);
  void paintGrid();
  void checkClipboard();

protected:
  virtual void paintEvent(TQPaintEvent*);
  virtual void resizeEvent(TQResizeEvent*);
  void paintDropSite();
  void setSizes();

  TQFrame *_corner;
  KIconEditGrid *_grid;
  KRuler *_hruler, *_vruler;
  TQScrollView *viewport;
  bool acceptdrop;
};

/**
* KIconEditGrid
* @short KIconEditGrid
* @author Thomas Tanghus <tanghus@kde.org>
* @version 0.3
*/
class KIconEditGrid : public KColorGrid
{
    Q_OBJECT
  TQ_OBJECT
public:
  KIconEditGrid( TQImage *image, KCommandHistory* h, TQWidget * tqparent = 0, const char *name = 0);
  virtual ~KIconEditGrid();

  enum DrawTool { Line, Freehand, FloodFill, Spray, Rect, FilledRect, Circle,
        FilledCircle, Ellipse, FilledEllipse, Eraser, SelectRect, SelectCircle, Find };
  enum DrawAction { Mark, UnMark, Draw };

  void setGrid(bool g) { KColorGrid::setGrid(g); emit needPainting(); }
  bool isModified() { return modified; };
  void setModified(bool m);
  const TQPixmap &pixmap();
  const TQImage &image() { return *img; }
  TQImage clipboardImage(bool &ok);
  TQImage getSelection(bool);
  int rows() { return numRows(); };
  int cols() { return numCols(); };
  uint getColors( uint *_colors) { return colors(_colors); }
  bool isMarked(TQPoint p);
  bool isMarked(int x, int y);
  int scaling() { return cellSize(); }
  void loadBlank( int w = 0, int h = 0);
  void setUndoColor( int colNum, uint v, bool update = true ) {
  	setColor( colNum, v, update );
  };

  enum TransparencyDisplayType
  {
    TRD_SOLIDCOLOR,
    TRD_CHECKERBOARD
  };
  enum CheckerboardSize
  {
    CHK_SMALL = 0,
    CHK_MEDIUM = 1,
    CHK_LARGE = 2
  };

  TransparencyDisplayType transparencyDisplayType() const { return m_transparencyDisplayType; }
  TQColor checkerboardColor1() const { return m_checkerboardColor1; }
  TQColor checkerboardColor2() const { return m_checkerboardColor2; }
  CheckerboardSize checkerboardSize() const { return m_checkerboardSize; }
  TQColor transparencySolidColor() const { return m_transparencySolidColor; }

  void setTransparencyDisplayType(TransparencyDisplayType t) { m_transparencyDisplayType = t; }
  void setCheckerboardColor1(const TQColor& c) { m_checkerboardColor1 = c; }
  void setCheckerboardColor2(const TQColor& c) { m_checkerboardColor2 = c; }
  void setCheckerboardSize(CheckerboardSize size) { m_checkerboardSize = size; }
  void setTransparencySolidColor(const TQColor& c) { m_transparencySolidColor = c; }

public slots:
  void load( TQImage *);
  void editCopy(bool cut = false);
  void editPaste(bool paste = false);
  void editPasteAsNew();
  void editSelectAll();
  void editClear();
  void getImage(TQImage *image);
//#if [[[TQT_VERSION IS DEPRECATED]]] <= 140
  void editResize();
//#endif
  void setSize(const TQSize s);
  void grayScale();
  void mapToKDEPalette();
  void setTool(DrawTool tool);
  bool zoom(Direction direct);
  bool zoomTo(int);
  
  void checkClipboard();

signals:
  void scalingchanged(int);
  void changed( const TQPixmap & );
  void sizechanged( int, int );
  void poschanged( int, int );
  void xposchanged( int );
  void yposchanged( int );
  void newmessage(const TQString &);
  void clipboarddata(bool);
  void selecteddata(bool);
  void needPainting();
  void modifiedchanged(bool);
  void colorSelected(uint);

protected slots:
  void setColorSelection( uint );
  void updatePreviewPixmap();

protected:
  virtual void paintEvent(TQPaintEvent*);
  virtual void paintCell( TQPainter*, int, int ) {}
  virtual void paintForeground(TQPainter* p, TQPaintEvent* e);
  virtual void mousePressEvent(TQMouseEvent*);
  virtual void mouseReleaseEvent(TQMouseEvent*);
  virtual void mouseMoveEvent(TQMouseEvent*);
  void createCursors();
  void drawPointArray(TQPointArray, DrawAction);
  void drawEllipse(bool);
  void drawLine(bool drawIt, bool drawStraight);
  void drawRect(bool);
  void drawSpray(TQPoint);
  void drawFlood(int x, int y, uint oldcolor);
  static void clearImage(TQImage *image);

  uint currentcolor;
  TQPoint start, end;
  TQRect insrect;
  TQSize cbsize;
  TQImage *img;
  TQPixmap p;
  int selected, tool; //, numrows, numcols;
  bool modified, btndown, ispasting, isselecting;
  TQPointArray pntarray;
  KColorArray iconcolors;
  KCommandHistory* history;
  KMacroCommand* m_command;
  TQCursor cursor_normal, cursor_aim, cursor_flood, cursor_spray, cursor_erase, cursor_paint, cursor_colorpicker;
  TransparencyDisplayType m_transparencyDisplayType;
  TQColor m_checkerboardColor1;
  TQColor m_checkerboardColor2;
  CheckerboardSize m_checkerboardSize;
  TQColor m_transparencySolidColor;
};



#endif //__KICONEDITGRID_H__



