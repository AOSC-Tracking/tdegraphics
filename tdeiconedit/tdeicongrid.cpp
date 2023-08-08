/*
    TDE Icon Editor - a small graphics drawing program for the TDE.
    Copyright (C) 1998  Thomas Tanghus (tanghus@kde.org)

    Includes portions of code from TQt,
    Copyright (C) 1992-2000 Trolltech AS.

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

#include <stdlib.h>

#include <tqpainter.h>
#include <tqwhatsthis.h>
#include <tqscrollview.h>
#include <tqbitmap.h>
#include <tqclipboard.h>
#include <tqdatetime.h>

#include <kiconloader.h>
#include <kruler.h>
#include <tdelocale.h>
#include <tdemessagebox.h>
#include <kdebug.h>

#include "kresize.h"
#include "properties.h"
#include "tdeicongrid.h"
#include "tdeiconedit.h"
#ifndef PICS_INCLUDED
#include "pics/logo.xpm"
#define PICS_INCLUDED
#endif

#include <X11/Xos.h>

void DrawCommand::execute()
{
	oldcolor = *((uint*)image->scanLine(y) + x);
	*((uint*)image->scanLine(y) + x) = newcolor; 
        int cell = y * grid->numCols() + x;
        grid->setUndoColor( cell, newcolor, false );
}

void DrawCommand::unexecute()
{
	*((uint*)image->scanLine(y) + x) = oldcolor; 
        int cell = y * grid->numCols() + x;
        grid->setUndoColor( cell, oldcolor, false );
}

void RepaintCommand::execute()
{
        grid->update( area);
}

KGridView::KGridView(TQImage *image, KCommandHistory* history, TQWidget *parent, const char *name)
: TQFrame(parent, name)
{
  _corner = 0L;
  _hruler = _vruler = 0L;
  _grid = 0L;

  acceptdrop = false;

  TDEIconEditProperties *props = TDEIconEditProperties::self();

  viewport = new TQScrollView(this);
  TQ_CHECK_PTR(viewport);

  _grid = new TDEIconEditGrid(image, history, viewport->viewport());
  TQ_CHECK_PTR(_grid);
  viewport->addChild(_grid);
  _grid->setGrid(props->showGrid());
  _grid->setCellSize(props->gridScale());

  TQString str = i18n( "Icon draw grid\n\nThe icon grid is the area where"
      " you draw the icons.\nYou can zoom in and out using the magnifying"
      " glasses on the toolbar.\n(Tip: Hold the magnify button down for a"
      " few seconds to zoom to a predefined scale)" );
  TQWhatsThis::add( _grid, str );

  if(props->bgMode() == FixedPixmap)
  {
    TQPixmap pix(props->bgPixmap());
    if(pix.isNull())
    {
      TQPixmap pmlogo((const char **)logo);
      pix = pmlogo;
    }
    viewport->viewport()->setBackgroundPixmap(pix);
    _grid->setBackgroundPixmap(pix);
  }
  else
  {
    viewport->viewport()->setBackgroundColor(props->bgColor());
  }

  _corner = new TQFrame(this);
  _corner->setFrameStyle(TQFrame::WinPanel | TQFrame::Raised);

  _hruler = new KRuler(Qt::Horizontal, this);
  _hruler->setEndLabel(i18n("width"));
  _hruler->setOffset( -2 );
  _hruler->setRange(0, 1000);

  _vruler = new KRuler(Qt::Vertical, this);
  _vruler->setEndLabel(i18n("height"));
  _vruler->setOffset( -2 );
  _vruler->setRange(0, 1000);

  str = i18n( "Rulers\n\nThis is a visual representation of the current"
      " cursor position" );
  TQWhatsThis::add( _hruler, str );
  TQWhatsThis::add( _vruler, str );

  connect(_grid, TQT_SIGNAL(scalingchanged(int)), TQT_SLOT(scalingChange(int)));
  connect(_grid, TQT_SIGNAL(sizechanged(int, int)), TQT_SLOT(sizeChange(int, int)));
  connect(_grid, TQT_SIGNAL(needPainting()), TQT_SLOT(paintGrid()));
  connect( _grid, TQT_SIGNAL(xposchanged(int)), _hruler, TQT_SLOT(slotNewValue(int)) );
  connect( _grid, TQT_SIGNAL(yposchanged(int)), _vruler, TQT_SLOT(slotNewValue(int)) );
  connect(viewport, TQT_SIGNAL(contentsMoving(int, int)), TQT_SLOT(moving(int, int)));
  
  setSizes();
  TQResizeEvent e(size(), size());
  resizeEvent(&e);
}

void KGridView::paintGrid()
{
  _grid->update(viewRect());
}

void KGridView::setSizes()
{
  if(TDEIconEditProperties::self()->showRulers())
  {
    _hruler->setLittleMarkDistance(_grid->scaling());
    _vruler->setLittleMarkDistance(_grid->scaling());

    _hruler->setMediumMarkDistance(5);
    _vruler->setMediumMarkDistance(5);

    _hruler->setBigMarkDistance(10);
    _vruler->setBigMarkDistance(10);

    _hruler->setShowTinyMarks(true);
    _hruler->setShowLittleMarks(false);
    _hruler->setShowMediumMarks(true);
    _hruler->setShowBigMarks(true);
    _hruler->setShowEndMarks(true);

    _vruler->setShowTinyMarks(true);
    _vruler->setShowLittleMarks(false);
    _vruler->setShowMediumMarks(true);
    _vruler->setShowBigMarks(true);
    _vruler->setShowEndMarks(true);

    _hruler->setPixelPerMark(_grid->scaling());
    _vruler->setPixelPerMark(_grid->scaling());

    _hruler->setMaxValue(_grid->width()+20);
    _vruler->setMaxValue(_grid->height()+20);

    _hruler->show();
    _vruler->show();

    _corner->show();
    //resize(_grid->width()+_vruler->width(), _grid->height()+_hruler->height());
  }
  else
  {
    _hruler->hide();
    _vruler->hide();
    _corner->hide();
    //resize(_grid->size());
  }
}

void KGridView::sizeChange(int, int)
{
    setSizes();
}

void KGridView::moving(int x, int y)
{
    _hruler->setOffset(abs(x));
    _vruler->setOffset(abs(y));
}

void KGridView::scalingChange(int)
{
    setSizes();
}

void KGridView::setShowRulers(bool mode)
{
    TDEIconEditProperties::self()->setShowRulers( mode );
    setSizes();
    TQResizeEvent e(size(), size());
    resizeEvent(&e);
}

void KGridView::setAcceptDrop(bool a)
{
    if(a == acceptdrop) return;
    acceptdrop = a;
    paintDropSite();
}

void KGridView::checkClipboard()
{
     _grid->checkClipboard();
}

const TQRect KGridView::viewRect()
{
    int x, y, cx, cy;
    if(viewport->horizontalScrollBar()->isVisible())
    {
        x = viewport->contentsX();
        cx = viewport->viewport()->width();
    }
    else
    {
        x = 0;
        cx = viewport->contentsWidth();
    }

    if(viewport->verticalScrollBar()->isVisible())
    {
        y = viewport->contentsY();
        cy = viewport->viewport()->height();
    }
    else
    {
        y = 0;
        cy = viewport->contentsHeight();
    }

    return TQRect(x, y, cx, cy);
}

void KGridView::paintDropSite()
{
    TQPainter p;
    p.begin( _grid );
    p.setRasterOp (NotROP);
    p.drawRect(viewRect());
    p.end();
}

void KGridView::paintEvent(TQPaintEvent *)
{
  if(acceptdrop)
    paintDropSite();
}


void KGridView::resizeEvent(TQResizeEvent*)
{
  kdDebug(4640) << "KGridView::resizeEvent" << endl;

  setSizes();

  if(TDEIconEditProperties::self()->showRulers())
  {
    _hruler->setGeometry(_vruler->width(), 0, width(), _hruler->height());
    _vruler->setGeometry(0, _hruler->height(), _vruler->width(), height());

    _corner->setGeometry(0, 0, _vruler->width(), _hruler->height());
    viewport->setGeometry(_corner->width(), _corner->height(),
                   width()-_corner->width(), height()-_corner->height());
  }
  else
    viewport->setGeometry(0, 0, width(), height());
}


TDEIconEditGrid::TDEIconEditGrid(TQImage *image, KCommandHistory* h, TQWidget *parent, const char *name)
 : KColorGrid(parent, name, 1)
{
    img = image;
    history = h;
    selected = 0;
    m_command = 0;

    // the 42 normal kde colors - there can be an additional
    // 18 custom colors in the custom colors palette
    for(uint i = 0; i < 42; i++)
        iconcolors.append(iconpalette[i]);

    setupImageHandlers();
    btndown = isselecting = ispasting = modified = false;

    img->create(32, 32, 32);
    img->setAlphaBuffer(true);
    clearImage(img);

    currentcolor = tqRgb(0,0,0)|OPAQUE_MASK;
    emit colorSelected(currentcolor);

    setMouseTracking(true);

    setNumRows(32);
    setNumCols(32);
    fill(TRANSPARENT);

    connect( kapp->clipboard(), TQT_SIGNAL(dataChanged()), TQT_SLOT(checkClipboard()));
    connect( h, TQT_SIGNAL(commandExecuted()), this, TQT_SLOT(updatePreviewPixmap() ));    
    createCursors();

    TDEIconEditProperties *props = TDEIconEditProperties::self();

    setTransparencyDisplayType(props->transparencyDisplayType());
    setTransparencySolidColor(props->transparencySolidColor());
    setCheckerboardColor1(props->checkerboardColor1());
    setCheckerboardColor2(props->checkerboardColor2());
    setCheckerboardSize(props->checkerboardSize());
}

TDEIconEditGrid::~TDEIconEditGrid()
{
    kdDebug(4640) << "TDEIconEditGrid - destructor: done" << endl;
}

void TDEIconEditGrid::paintEvent(TQPaintEvent *e)
{
  const TQRect cellsRect(0, 0, numCols() * cellSize(), numRows() * cellSize());
  const TQRect paintCellsRect = cellsRect.intersect(e->rect());

  if(!paintCellsRect.isEmpty())
  {
    //TQTime time;

    //time.start();

    TQRgb *imageBuffer = new TQRgb[paintCellsRect.width() * paintCellsRect.height()];
    const int cellsize = cellSize();
    const int firstCellPixelsRemaining = cellsize - paintCellsRect.left() % cellsize;

    if(transparencyDisplayType() == TRD_SOLIDCOLOR)
    {
      const TQRgb backgroundColor = transparencySolidColor().rgb();
      const int backgroundRed = transparencySolidColor().red();
      const int backgroundGreen = transparencySolidColor().green();
      const int backgroundBlue = transparencySolidColor().blue();
      const int firstCellX = paintCellsRect.left() / cellsize;

      for(int y = paintCellsRect.top(); y <= paintCellsRect.bottom(); y++)
      {
        TQRgb *dest = imageBuffer + (y - paintCellsRect.top()) * paintCellsRect.width();
        
        if(y % cellsize == 0 || dest == imageBuffer)
        {
          // Paint the first scanline in each block of cellSize() identical lines.
          // The remaineder can just be copied from this one.
          const int cellY = y / cellsize;
          TQRgb *src = gridcolors.data() + cellY * numCols() + firstCellX;

          TQRgb sourcePixel = *src++;
          int sourceAlpha = tqAlpha(sourcePixel);
          
          TQRgb c;

          if(sourceAlpha == 255)
          {
            c = sourcePixel;
          }
          else
          if(sourceAlpha == 0)
          {
            c = backgroundColor;
          }
          else
          {
            const int sourceRed = tqRed(sourcePixel);
            const int sourceGreen = tqGreen(sourcePixel);
            const int sourceBlue = tqBlue(sourcePixel);

            int r = (sourceAlpha * (sourceRed - backgroundRed)) + 0x80;
            r = backgroundRed + ((r + (r >> 8)) >> 8);

            int g = (sourceAlpha * (sourceGreen - backgroundGreen)) + 0x80;
            g = backgroundGreen + ((g + (g >> 8)) >> 8);

            int b = (sourceAlpha * (sourceBlue - backgroundBlue)) + 0x80;
            b = backgroundBlue + ((b + (b >> 8)) >> 8);

            c = tqRgb(r, g, b);
          }

          int cellPixelsRemaining = firstCellPixelsRemaining;

          for(int x = paintCellsRect.left(); x <= paintCellsRect.right(); x++)
          {
            if(cellPixelsRemaining == 0)
            {
              cellPixelsRemaining = cellsize;

              // Fetch the next source pixel
              sourcePixel = *src++;
              sourceAlpha = tqAlpha(sourcePixel);

              if(sourceAlpha == 255)
              {
                c = sourcePixel;
              }
              else
              if(sourceAlpha == 0)
              {
                c = backgroundColor;
              }
              else
              {
                const int sourceRed = tqRed(sourcePixel);
                const int sourceGreen = tqGreen(sourcePixel);
                const int sourceBlue = tqBlue(sourcePixel);

                //int r = backgroundRed + (sourceAlpha * (sourceRed - backgroundRed)) / 255;
                //int g = backgroundGreen + (sourceAlpha * (sourceGreen - backgroundGreen)) / 255;
                //int b = backgroundBlue + (sourceAlpha * (sourceBlue - backgroundBlue)) / 255;

                int r = (sourceAlpha * (sourceRed - backgroundRed)) + 0x80;
                r = backgroundRed + ((r + (r >> 8)) >> 8);

                int g = (sourceAlpha * (sourceGreen - backgroundGreen)) + 0x80;
                g = backgroundGreen + ((g + (g >> 8)) >> 8);

                int b = (sourceAlpha * (sourceBlue - backgroundBlue)) + 0x80;
                b = backgroundBlue + ((b + (b >> 8)) >> 8);

                c = tqRgb(r, g, b);
              }
            }

            cellPixelsRemaining--;

            *dest++ = c;
          }
        }
        else
        {
          // Copy the scanline above.
          memcpy(dest, dest - paintCellsRect.width(), paintCellsRect.width() * sizeof(TQRgb));
        }
      }
    }
    else
    {
      int squareSize;
      const int fixedPointMultiplier = 4;

      if(checkerboardSize() == CHK_SMALL)
      {
        squareSize = (cellSize() * fixedPointMultiplier) / 4;
      }
      else
      if(checkerboardSize() == CHK_MEDIUM)
      {
        squareSize = (cellSize() * fixedPointMultiplier) / 2;
      }
      else
      {
        squareSize = (2 * cellSize() * fixedPointMultiplier) / 2;
      }

      TQRgb *color1ScanLine = new TQRgb[paintCellsRect.width()];
      TQRgb *color2ScanLine = new TQRgb[paintCellsRect.width()];
      TQRgb *color1Buffer = color1ScanLine;
      TQRgb *color2Buffer = color2ScanLine;

      for(int x = paintCellsRect.left(); x <= paintCellsRect.right(); x++)
      {
        if((((x * fixedPointMultiplier) / squareSize) & 1) == 0)
        {
          *color1Buffer++ = checkerboardColor1().rgb();
          *color2Buffer++ = checkerboardColor2().rgb();
        }
        else
        {
          *color1Buffer++ = checkerboardColor2().rgb();
          *color2Buffer++ = checkerboardColor1().rgb();
        }
      }

      const int firstCellX = paintCellsRect.left() / cellsize;
      const int firstCellPixelsRemaining = cellsize - paintCellsRect.left() % cellsize;
      int lastCellY = -1;
      int lastLineFirstSquareColour = 0;

      for(int y = paintCellsRect.top(); y <= paintCellsRect.bottom(); y++)
      {
        TQRgb *dest = imageBuffer + (y - paintCellsRect.top()) * paintCellsRect.width();
        const int cellY = y / cellsize;

        int firstSquareColour;
        const TQRgb *checkerboardSrc;

        if((((y * fixedPointMultiplier) / squareSize) & 1) == 0)
        {
          firstSquareColour = 1;
          checkerboardSrc = color1ScanLine;
        }
        else
        {
          firstSquareColour = 2;
          checkerboardSrc = color2ScanLine;
        }

        if(cellY == lastCellY && firstSquareColour == lastLineFirstSquareColour)
        {
          // Copy the scanline above.
          memcpy(dest, dest - paintCellsRect.width(), paintCellsRect.width() * sizeof(TQRgb));
        }
        else
        {
          TQRgb *src = gridcolors.data() + cellY * numCols() + firstCellX;

          TQRgb sourcePixel = *src++;
          int sourceRed = tqRed(sourcePixel);
          int sourceGreen = tqGreen(sourcePixel);
          int sourceBlue = tqBlue(sourcePixel);
          int sourceAlpha = tqAlpha(sourcePixel);

          int cellPixelsRemaining = firstCellPixelsRemaining;

          for(int x = paintCellsRect.left(); x <= paintCellsRect.right(); x++)
          {
            if(cellPixelsRemaining == 0)
            {
              cellPixelsRemaining = cellsize;

              // Fetch the next source pixel
              sourcePixel = *src++;
              sourceRed = tqRed(sourcePixel);
              sourceGreen = tqGreen(sourcePixel);
              sourceBlue = tqBlue(sourcePixel);
              sourceAlpha = tqAlpha(sourcePixel);
            }

            cellPixelsRemaining--;

            TQRgb c;

            if(sourceAlpha == 255)
            {
              c = sourcePixel;
            }
            else
            if(sourceAlpha == 0)
            {
              c = *checkerboardSrc;
            }
            else
            {
              const int backgroundColor = *checkerboardSrc;
              const int backgroundRed = tqRed(backgroundColor);
              const int backgroundGreen = tqGreen(backgroundColor);
              const int backgroundBlue = tqBlue(backgroundColor);

              //int r = backgroundRed + (sourceAlpha * (sourceRed - backgroundRed)) / 255;
              //int g = backgroundGreen + (sourceAlpha * (sourceGreen - backgroundGreen)) / 255;
              //int b = backgroundBlue + (sourceAlpha * (sourceBlue - backgroundBlue)) / 255;

              int r = (sourceAlpha * (sourceRed - backgroundRed)) + 0x80;
              r = backgroundRed + ((r + (r >> 8)) >> 8);

              int g = (sourceAlpha * (sourceGreen - backgroundGreen)) + 0x80;
              g = backgroundGreen + ((g + (g >> 8)) >> 8);

              int b = (sourceAlpha * (sourceBlue - backgroundBlue)) + 0x80;
              b = backgroundBlue + ((b + (b >> 8)) >> 8);

              c = tqRgb(r, g, b);
            }

            *dest++ = c;
            checkerboardSrc++;
          }
        }

        lastCellY = cellY;
        lastLineFirstSquareColour = firstSquareColour;
      }

      delete [] color1ScanLine;
      delete [] color2ScanLine;
    }

    TQImage image((uchar *)(imageBuffer), paintCellsRect.width(), paintCellsRect.height(), 32, 0, 0,
#if X_BYTE_ORDER == X_LITTLE_ENDIAN
      TQImage::LittleEndian);
#else
      TQImage::BigEndian);
#endif
    Q_ASSERT(!image.isNull());

    TQPixmap _pixmap;
    _pixmap.convertFromImage(image);

    TQPainter p;
    p.begin(&_pixmap);
    paintForeground(&p, e);
    p.end();

    bitBlt(this, paintCellsRect.left(), paintCellsRect.top(), &_pixmap);

    //kdDebug(4640) << "Image render elapsed: " << time.elapsed() << endl;

    delete [] imageBuffer;
  }
}

void TDEIconEditGrid::paintForeground(TQPainter* p, TQPaintEvent* e)
{
    TQWMatrix matrix;

    matrix.translate(-e->rect().x(), -e->rect().y());
    p->setWorldMatrix( matrix );

    TQRect cellsRect(0, 0, numCols() * cellSize(), numRows() * cellSize());
    TQRect paintCellsRect = cellsRect.intersect(e->rect());

    if(!paintCellsRect.isEmpty())
    {
      int firstColumn = paintCellsRect.left() / cellSize();
      int lastColumn = paintCellsRect.right() / cellSize();

      int firstRow = paintCellsRect.top() / cellSize();
      int lastRow = paintCellsRect.bottom() / cellSize();

      p->setPen(TQColor(0, 0, 0));
      p->setBrush(TQColor(0, 0, 0));

      for(int column = firstColumn; column <= lastColumn; column++)
      {
        for(int row = firstRow; row <= lastRow; row++)
        {
          int x = column * cellSize();
          int y = row * cellSize();

          if((ispasting || isselecting) && isMarked(column, row))
          {
            p->drawWinFocusRect(x + 1, y + 1, cellSize() - 2, cellSize() - 2);
          }
          else
          {
            switch( tool )
            {
              case FilledRect:
              case Rect:
              case Ellipse:
              case Circle:
              case FilledEllipse:
              case FilledCircle:
              case Line:
                if(btndown && isMarked(column, row))
                {
                  if(cellSize() > 1)
                  {
                    p->drawWinFocusRect( x + 1, y + 1, cellSize() - 2, cellSize() - 2);
                  }
                  else
                  {
                    p->drawPoint(x, y);
                  }
                }
                break;

              default:
                break;
            }
          }
        }
      }
    }

    if(hasGrid()&& !(cellSize()==1))
    {
        p->setPen(TQColor(0, 0, 0));
        int x = e->rect().x() - ((e->rect().x() % cellSize()) + cellSize());
        if(x < 0) x = 0;
        int y = e->rect().y() - ((e->rect().y() % cellSize()) + cellSize());
        if(y < 0) y = 0;
        int cx = e->rect().right() + cellSize();
        int cy = e->rect().bottom() + cellSize();

        // draw grid lines
        for(int i = x; i < cx; i += cellSize())
            p->drawLine(i, y, i, cy);

        for(int i = y; i < cy; i += cellSize())
            p->drawLine(x, i, cx, i);
    }
}

void TDEIconEditGrid::mousePressEvent( TQMouseEvent *e )
{
    if(!e || (e->button() != Qt::LeftButton))
        return;

    int row = findRow( e->pos().y() );
    int col = findCol( e->pos().x() );
    //int cell = row * numCols() + col;

    if(!img->valid(col, row))
        return;

    btndown = true;
    start.setX(col);
    start.setY(row);

    if(ispasting)
    {
        ispasting = false;
        editPaste(true);
    }

    if(isselecting)
    {
        TQPointArray a(pntarray.copy());
        pntarray.resize(0);
        drawPointArray(a, Mark);
        emit selecteddata(false);
    }

    switch( tool )
    {
        case SelectRect:
        case SelectCircle:
            isselecting = true;
            break;
        default:
            break;
    }
}

void TDEIconEditGrid::mouseMoveEvent( TQMouseEvent *e )
{
    if(!e) return;

    int row = findRow( e->pos().y() );
    int col = findCol( e->pos().x() );
    int cell = row * numCols() + col;

    if(img->valid(col, row))
    {
        //kdDebug(4640) << col << " X " << row << endl;
        emit poschanged(col, row);
        // for the rulers
        emit xposchanged((col*scaling())+scaling()/2);
        emit yposchanged((row*scaling())+scaling()/2);
    }

    TQPoint tmpp(col, row);
    if(tmpp == end) return;

    // need to use intersection of rectangles to allow pasting
    // only that part of clip image which intersects -jwc-
    if(ispasting && !btndown && img->valid(col, row))
    {
        if( (col + cbsize.width()) > (numCols()-1) )
            insrect.setX(numCols()-insrect.width());
        else
            insrect.setX(col);
        if( (row + cbsize.height()) > (numRows()-1) )
            insrect.setY(numRows()-insrect.height());
        else
            insrect.setY(row);

        insrect.setSize(cbsize);
        start = insrect.topLeft();
        end = insrect.bottomRight();
        drawRect(false);
        return;
    }

    if(!img->valid(col, row) || !btndown)
        return;

    end.setX(col);
    end.setY(row);

    if(isselecting)
    {
        if(tool == SelectRect)
            drawRect(false);
        else
            drawEllipse(false);
        return;
    }

    bool erase=false;
    switch( tool )
    {
        case Eraser:
            erase=true;

        case Freehand:
        {
            if( !m_command )
                m_command = new KMacroCommand( i18n("Free Hand") );
            
            if(erase)
                setColor( cell, TRANSPARENT );
            else
                setColor( cell, currentcolor );

            if ( selected != cell )
            {
                setModified( true );
                int prevSel = selected;
                selected = cell;
                TQRect area = TQRect( col*cellsize,row*cellsize, cellsize, cellsize ).unite( 
                    TQRect ( (prevSel%numCols())*cellsize,(prevSel/numCols())*cellsize, cellsize, cellsize ) );
                    
                m_command->addCommand( new RepaintCommand( area, this ) );
                DrawCommand* dc = new DrawCommand( col, row, colorAt(cell), img, this );
                RepaintCommand* rp = new RepaintCommand( area, this );
                dc->execute();
                rp->execute();
                m_command->addCommand( dc );
                m_command->addCommand( rp );
            }
            break;
        }
        case Find:
        {
            iconcolors.closestMatch(colorAt(cell));
            if ( selected != cell )
            {
                int prevSel = selected;
                selected = cell;
                update((prevSel%numCols())*cellsize,(prevSel/numCols())*cellsize, cellsize, cellsize);
                update(col*cellsize,row*cellsize, cellsize, cellsize);
                emit colorSelected(colorAt(selected));
            }
            break;
        }
        case Ellipse:
        case Circle:
        case FilledEllipse:
        case FilledCircle:
        {
            drawEllipse(false);
            break;
        }
        case FilledRect:
        case Rect:
        {
            drawRect(false);
            break;
        }
        case Line:
        {
            drawLine(false, false);
            break;
        }
        case Spray:
        {
            drawSpray(TQPoint(col, row));
            setModified(true);
            break;
        }
        default:
            break;
    }

    p = *img;
    emit changed(TQPixmap(p));
}

void TDEIconEditGrid::mouseReleaseEvent( TQMouseEvent *e )
{
  if(!e || (e->button() != Qt::LeftButton))
    return;

  int row = findRow( e->pos().y() );
  int col = findCol( e->pos().x() );
  btndown = false;
  end.setX(col);
  end.setY(row);
  int cell = row * numCols() + col;
  bool erase=false;
  switch( tool )
  {
    case Eraser:
        erase=true;
      //currentcolor = TRANSPARENT;
    case Freehand:
    {
      if(!img->valid(col, row))
        return;
      if(erase)
        setColor( cell, TRANSPARENT );
      else
        setColor( cell, currentcolor );
      //if ( selected != cell )
      //{
        setModified( true );
        int prevSel = selected;
        selected = cell;
        update((prevSel%numCols())*cellsize,(prevSel/numCols())*cellsize, cellsize, cellsize);
        update(col*cellsize,row*cellsize, cellsize, cellsize);
        //updateCell( prevSel/numCols(), prevSel%numCols(), FALSE );
        //updateCell( row, col, FALSE );
        *((uint*)img->scanLine(row) + col) = colorAt(cell);
        p = *img;
      //}
      
        if( m_command ) {
            history->addCommand( m_command, false );
            m_command = 0;
        }
        
      break;
    }
    case Ellipse:
    case Circle:
    case FilledEllipse:
    case FilledCircle:
    {
      drawEllipse(true);
      break;
    }
    case FilledRect:
    case Rect:
    {
      drawRect(true);
      break;
    }
    case Line:
    {
      drawLine(true, false);
      break;
    }
    case Spray:
    {
      drawSpray(TQPoint(col, row));
      break;
    }
    case FloodFill:
    {
      TQApplication::setOverrideCursor(waitCursor);
      drawFlood(col, row, colorAt(cell));
      TQApplication::restoreOverrideCursor();
      updateColors();
      emit needPainting();
      p = *img;
      break;
    }
    case Find:
    {
      currentcolor = colorAt(cell);
      if ( selected != cell )
      {
        int prevSel = selected;
        selected = cell;
        update((prevSel%numCols())*cellsize,(prevSel/numCols())*cellsize, cellsize, cellsize);
        update(col*cellsize,row*cellsize, cellsize, cellsize);
        emit colorSelected(currentcolor);
        //updateCell( prevSel/numCols(), prevSel%numCols(), FALSE );
        //updateCell( row, col, FALSE );
      }

      break;
    }
    default:
      break;
  }
  
  emit changed(TQPixmap(p));
  //emit colorschanged(numColors(), data());
}

//void TDEIconEditGrid::setColorSelection( const TQColor &color )
void TDEIconEditGrid::setColorSelection( uint c )
{
  currentcolor = c;
  emit colorSelected(currentcolor);
}

void TDEIconEditGrid::loadBlank( int w, int h )
{
  img->create(w, h, 32);
  img->setAlphaBuffer(true);
  clearImage(img);
  setNumRows(h);
  setNumCols(w);
  fill(TRANSPARENT);
  emit sizechanged(numCols(), numRows());
  emit colorschanged(numColors(), data());
  history->clear();
}



void TDEIconEditGrid::load( TQImage *image)
{
    kdDebug(4640) << "TDEIconEditGrid::load" << endl;

    setUpdatesEnabled(false);

    if(image == 0L)
    {
        TQString msg = i18n("There was an error loading a blank image.\n");
        KMessageBox::error(this, msg);
        return;
    }

    *img = image->convertDepth(32);
    img->setAlphaBuffer(true);
    setNumRows(img->height());
    setNumCols(img->width());

    for(int y = 0; y < numRows(); y++)
    {
        uint *l = (uint*)img->scanLine(y);
        for(int x = 0; x < numCols(); x++, l++)
        {
            setColor((y*numCols())+x, *l, false);
        }
        //kdDebug(4640) << "Row: " << y << endl;
        kapp->processEvents(200);
    }

    updateColors();
    emit sizechanged(numCols(), numRows());
    emit colorschanged(numColors(), data());
    emit changed(pixmap());
    setUpdatesEnabled(true);
    emit needPainting();
    //repaint(viewRect(), false);
    history->clear();
}

const TQPixmap &TDEIconEditGrid::pixmap()
{
    if(!img->isNull())
        p = *img;
    //p.convertFromImage(*img, 0);
    return(p);
}

void TDEIconEditGrid::getImage(TQImage *image)
{
    kdDebug(4640) << "TDEIconEditGrid::getImage" << endl;
    *image = *img;
}

bool TDEIconEditGrid::zoomTo(int scale)
{
    TQApplication::setOverrideCursor(waitCursor);
    setUpdatesEnabled(false);
    setCellSize( scale );
    setUpdatesEnabled(true);
    emit needPainting();
    TQApplication::restoreOverrideCursor();
    emit scalingchanged(cellSize());

    if(scale == 1)
        return false;
    return true;
}

bool TDEIconEditGrid::zoom(Direction d)
{
    int f = (d == DirIn) ? (cellSize()+1) : (cellSize()-1);
    TQApplication::setOverrideCursor(waitCursor);
    setUpdatesEnabled(false);
    setCellSize( f );
    setUpdatesEnabled(true);
    //emit needPainting();
    TQApplication::restoreOverrideCursor();

    emit scalingchanged(cellSize());
    if(d == DirOut && cellSize() <= 1)
        return false;
    return true;
}

void TDEIconEditGrid::checkClipboard()
{
  bool ok = false;
  TQImage tmp = clipboardImage(ok);
  if(ok)
    emit clipboarddata(true);
  else
  {
    emit clipboarddata(false);
  }
}

TQImage TDEIconEditGrid::clipboardImage(bool &ok)
{
  //###### Remove me later.
  //Workaround TQt bug -- check whether format provided first.
  //Code below is from TQDragObject, to match the mimetype list....

  TQStrList fileFormats = TQImageIO::inputFormats();
  fileFormats.first();
  bool oneIsSupported = false;
  while ( fileFormats.current() )
  {
    TQCString format = fileFormats.current();
    TQCString type = "image/" + format.lower();
    if (kapp->clipboard()->data()->provides(type ) )
    {
      oneIsSupported = true;
    }
    fileFormats.next();
  }
  if (!oneIsSupported)
  {
     ok = false;
     return TQImage();
  }

  TQImage image = kapp->clipboard()->image();
  ok = !image.isNull();
  if ( ok )
  {
      image = image.convertDepth(32);
      image.setAlphaBuffer(true);
  }
  return image;
}


void TDEIconEditGrid::editSelectAll()
{
    start.setX(0);
    start.setY(0);
    end.setX(numCols()-1);
    end.setY(numRows()-1);
    isselecting = true;
    drawRect(false);
    emit newmessage(i18n("All selected"));
}

void TDEIconEditGrid::editClear()
{
    clearImage(img);
    fill(TRANSPARENT);
    update();
    setModified(true);
    p = *img;
    emit changed(p);
    emit newmessage(i18n("Cleared"));
}

TQImage TDEIconEditGrid::getSelection(bool cut)
{
    const TQRect rect = pntarray.boundingRect();
    int nx = 0, ny = 0, nw = 0, nh = 0;
    rect.rect(&nx, &ny, &nw, &nh);

    TQImage tmp(nw, nh, 32);
    tmp.setAlphaBuffer(true);
    clearImage(&tmp);

    int s = pntarray.size();

    for(int i = 0; i < s; i++)
    {
        int x = pntarray[i].x();
        int y = pntarray[i].y();
        if(img->valid(x, y) && rect.contains(TQPoint(x, y)))
        {
            *((uint*)tmp.scanLine(y-ny) + (x-nx)) = *((uint*)img->scanLine(y) + x);
            if(cut)
            {
                *((uint*)img->scanLine(y) + x) = TRANSPARENT;
                setColor( (y*numCols()) + x, TRANSPARENT, false );
            }
        }
    }

    TQPointArray a(pntarray.copy());
    pntarray.resize(0);
    drawPointArray(a, Mark);
    emit selecteddata(false);
    if(cut)
    {
        updateColors();
        update(rect.x()*cellSize(), rect.y()*cellSize(),
            rect.width()*cellSize(), rect.height()*cellSize());
        p = *img;
        emit changed(p);
        emit colorschanged(numColors(), data());
        emit newmessage(i18n("Selected area cut"));
        setModified(true);
    }
    else
        emit newmessage(i18n("Selected area copied"));

    return tmp;
}

void TDEIconEditGrid::editCopy(bool cut)
{
    kapp->clipboard()->setImage(getSelection(cut));
    isselecting = false;
}


void TDEIconEditGrid::editPaste(bool paste)
{
    bool ok = false;
    TQImage tmp = clipboardImage(ok);

    TDEIconEditProperties *props = TDEIconEditProperties::self();

    if(ok)
    {
        if( (tmp.size().width() > img->size().width())
        || (tmp.size().height() > img->size().height()) )
        {
            if(KMessageBox::warningYesNo(this,
                i18n("The clipboard image is larger than the current"
                " image!\nPaste as new image?"),TQString(),i18n("Paste"), i18n("Do Not Paste")) == 0)
            {
                editPasteAsNew();
            }
            return;
        }
        else if(!paste)
        {
            ispasting = true;
            cbsize = tmp.size();
            return;
            // emit newmessage(i18n("Pasting"));
        }
        else
        {
            //kdDebug(4640) << "TDEIconEditGrid: Pasting at: " << insrect.x() << " x " << insrect.y() << endl;
            TQApplication::setOverrideCursor(waitCursor);

            for(int y = insrect.y(), ny = 0; y < numRows() && ny < insrect.height(); y++, ny++)
            {
                uint *l = ((uint*)img->scanLine(y)+insrect.x());
                uint *cl = (uint*)tmp.scanLine(ny);
                for(int x = insrect.x(), nx = 0; x < numCols() && nx < insrect.width(); x++, nx++, l++, cl++)
                {
                    if(props->pasteTransparent())
                    {
                        *l = *cl;
                    }
                    else
                    {
                        // Porter-Duff Over composition
                        double alphaS = tqAlpha(*cl) / 255.0;
                        double alphaD = tqAlpha(*l) / 255.0;
  
                        double r = tqRed(*cl) * alphaS + (1 - alphaS) * tqRed(*l) * alphaD;
                        double g = tqGreen(*cl) * alphaS + (1 - alphaS) * tqGreen(*l) * alphaD;
                        double b = tqBlue(*cl) * alphaS + (1 - alphaS) * tqBlue(*l) * alphaD;
                        double a = alphaS + (1 - alphaS) * alphaD;
  
                        // Remove multiplication by alpha

                        if(a > 0)
                        {
                            r /= a;
                            g /= a;
                            b /= a;
                        }
                        else
                        {
                            r = 0;
                            g = 0;
                            b = 0;
                        }
                        
                        int ir = (int)(r + 0.5);

                        if(ir < 0)
                        {
                            ir = 0;
                        }
                        else
                        if(ir > 255)
                        {
                            ir = 255;
                        }
                        
                        int ig = (int)(g + 0.5);

                        if(ig < 0)
                        {
                            ig = 0;
                        }
                        else
                        if(ig > 255)
                        {
                            ig = 255;
                        }
                        
                        int ib = (int)(b + 0.5);

                        if(ib < 0)
                        {
                            ib = 0;
                        }
                        else
                        if(ib > 255)
                        {
                            ib = 255;
                        }
                        
                        int ia = (int)((a * 255) + 0.5);

                        if(ia < 0)
                        {
                            ia = 0;
                        }
                        else
                        if(ia > 255)
                        {
                            ia = 255;
                        }
                        
                        *l = tqRgba(ir, ig, ib, ia);
                    }

                    setColor((y*numCols())+x, (uint)*l, false);
                }
            }
            updateColors();
            update(insrect.x()*cellSize(), insrect.y()*cellSize(),
                insrect.width()*cellSize(), insrect.height()*cellSize());

            TQApplication::restoreOverrideCursor();

            setModified(true);
            p = *img;
            emit changed(TQPixmap(p));
            emit sizechanged(numCols(), numRows());
            emit colorschanged(numColors(), data());
            emit newmessage(i18n("Done pasting"));
        }
    }
    else
    {
        TQString msg = i18n("Invalid pixmap data in clipboard!\n");
        KMessageBox::sorry(this, msg);
    }
}


void TDEIconEditGrid::editPasteAsNew()
{
    bool ok = false;
    TQImage tmp = clipboardImage(ok);

    if(ok)
    {
        if(isModified())
        {
            TDEIconEdit *w = new TDEIconEdit(tmp);
            TQ_CHECK_PTR(w);
        }
        else
        {
            *img = tmp;
            load(img);
            setModified(true);
            //repaint(viewRect(), false);
  
            p = *img;
            emit changed(TQPixmap(p));
            emit sizechanged(numCols(), numRows());
            emit colorschanged(numColors(), data());
            emit newmessage(i18n("Done pasting"));
            history->clear();
        }
    }
    else
    {
        TQString msg = i18n("Invalid pixmap data in clipboard!\n");
        KMessageBox::error(this, msg);
    }
}


void TDEIconEditGrid::editResize()
{
    kdDebug(4640) << "TDEIconGrid::editResize" << endl;
    KResizeDialog *rs = new KResizeDialog(this, 0, TQSize(numCols(), numRows()));
    if(rs->exec())
    {
        const TQSize s = rs->getSize();
        *img = img->smoothScale(s.width(), s.height());
        load(img);

        setModified(true);
    }
    delete rs;
}


void TDEIconEditGrid::setSize(const TQSize s)
{
    kdDebug(4640) << "::setSize: " << s.width() << " x " << s.height() << endl;

    img->create(s.width(), s.height(), 32);
    img->setAlphaBuffer(true);
    clearImage(img);
    load(img);
}


void TDEIconEditGrid::createCursors()
{
  TQBitmap mask(22, 22);
  TQPixmap pix;

  cursor_normal = TQCursor(arrowCursor);

  pix = BarIcon("colorpicker-cursor");
  if(pix.isNull())
  {
    cursor_colorpicker = cursor_normal;
    kdDebug(4640) << "TDEIconEditGrid: Error loading colorpicker-cursor.xpm" << endl;
  }
  else
  {
    mask = TQPixmap(pix.createHeuristicMask());
    pix.setMask(mask);
    cursor_colorpicker = TQCursor(pix, 1, 21);
  }

  pix = BarIcon("paintbrush-cursor");
  if(pix.isNull())
  {
    cursor_paint = cursor_normal;
    kdDebug(4640) << "TDEIconEditGrid: Error loading paintbrush.xpm" << endl;
  }
  else
  {
    mask = TQPixmap(pix.createHeuristicMask());
    pix.setMask(mask);
    cursor_paint = TQCursor(pix, 0, 19);
  }

  pix = BarIcon("fill-cursor");
  if(pix.isNull())
  {
    cursor_flood = cursor_normal;
    kdDebug(4640) << "TDEIconEditGrid: Error loading fill-cursor.xpm" << endl;
  }
  else
  {
    mask = TQPixmap(pix.createHeuristicMask());
    pix.setMask(mask);
    cursor_flood = TQCursor(pix, 3, 20);
  }

  pix = BarIcon("aim-cursor");
  if(pix.isNull())
  {
    cursor_aim = cursor_normal;
    kdDebug(4640) << "TDEIconEditGrid: Error loading aim-cursor.xpm" << endl;
  }
  else
  {
    mask = TQPixmap(pix.createHeuristicMask());
    pix.setMask(mask);
    cursor_aim = TQCursor(pix, 10, 10);
  }

  pix = BarIcon("airbrush-cursor");
  if(pix.isNull())
  {
    cursor_spray = cursor_normal;
    kdDebug(4640) << "TDEIconEditGrid: Error loading airbrush-cursor.xpm" << endl;
  }
  else
  {
    mask = TQPixmap(pix.createHeuristicMask(true));
    pix.setMask(mask);
    cursor_spray = TQCursor(pix, 0, 20);
  }

  pix = BarIcon("eraser-cursor");
  if(pix.isNull())
  {
    cursor_erase = cursor_normal;
    kdDebug(4640) << "TDEIconEditGrid: Error loading eraser-cursor.xpm" << endl;
  }
  else
  {
    mask = TQPixmap(pix.createHeuristicMask(true));
    pix.setMask(mask);
    cursor_erase = TQCursor(pix, 1, 16);
  }
}



void TDEIconEditGrid::setTool(DrawTool t)
{
    btndown = false;
    tool = t;

    if(tool != SelectRect && tool != SelectCircle)
        isselecting = false;

    switch( tool )
    {
        case SelectRect:
            isselecting = true;
            setCursor(cursor_aim);
            break;
        case SelectCircle:
            isselecting = true;
            setCursor(cursor_aim);
            break;
        case Line:
        case Ellipse:
        case Circle:
        case FilledEllipse:
        case FilledCircle:
        case FilledRect:
        case Rect:
            setCursor(cursor_aim);
            break;
        case Freehand:
            setCursor(cursor_paint);
            break;
        case Spray:
            setCursor(cursor_spray);
            break;
        case Eraser:
            setCursor(cursor_erase);
            break;
        case FloodFill:
            setCursor(cursor_flood);
            break;
        case Find:
            setCursor(cursor_colorpicker);
            break;
        default:
            break;
    }
}


void TDEIconEditGrid::drawFlood(int x, int y, uint oldcolor)
{
    if((!img->valid(x, y))
    || (colorAt((y * numCols())+x) != oldcolor)
    || (colorAt((y * numCols())+x) == currentcolor))
        return;

    *((uint*)img->scanLine(y) + x) = currentcolor;
    setColor((y*numCols())+x, currentcolor, false);

    setModified(true);

    drawFlood(x, y-1, oldcolor);
    drawFlood(x, y+1, oldcolor);
    drawFlood(x-1, y, oldcolor);
    drawFlood(x+1, y, oldcolor);
    //TODO: add undo 
}


void TDEIconEditGrid::drawSpray(TQPoint point)
{
    int x = (point.x()-5);
    int y = (point.y()-5);

    //kdDebug(4640) << "drawSpray() - " << x << " X " << y << endl;

    pntarray.resize(0);
    int points = 0;
    for(int i = 1; i < 4; i++, points++)
    {
        int dx = (rand() % 10);
        int dy = (rand() % 10);
        pntarray.putPoints(points, 1, x+dx, y+dy);
    }

    drawPointArray(pntarray, Draw);
}


//This routine is from TQt sources -- it's the branch of TQPointArray::makeEllipse( int x, int y, int w, int h ) that's not normally compiled
//It seems like TDEIconEdit relied on the TQt1 semantics for makeEllipse, which broke
//the tool with reasonably recent TQt versions.
//Thankfully, TQt includes the old code #ifdef'd, which is hence included here
static void TQPA_makeEllipse(TQPointArray& ar, int x, int y, int w, int h )
{						// midpoint, 1/4 ellipse
    if ( w <= 0 || h <= 0 ) {
	if ( w == 0 || h == 0 ) {
	    ar.resize( 0 );
	    return;
	}
	if ( w < 0 ) {				// negative width
	    w = -w;
	    x -= w;
	}
	if ( h < 0 ) {				// negative height
	    h = -h;
	    y -= h;
	}
    }
    int s = (w+h+2)/2;				// max size of xx,yy array
    int *px = new int[s];			// 1/4th of ellipse
    int *py = new int[s];
    int xx, yy, i=0;
    double d1, d2;
    double a2=(w/2)*(w/2),  b2=(h/2)*(h/2);
    xx = 0;
    yy = int(h/2);
    d1 = b2 - a2*(h/2) + 0.25*a2;
    px[i] = xx;
    py[i] = yy;
    i++;
    while ( a2*(yy-0.5) > b2*(xx+0.5) ) {		// region 1
	if ( d1 < 0 ) {
	    d1 = d1 + b2*(3.0+2*xx);
	    xx++;
	} else {
	    d1 = d1 + b2*(3.0+2*xx) + 2.0*a2*(1-yy);
	    xx++;
	    yy--;
	}
	px[i] = xx;
	py[i] = yy;
	i++;
    }
    d2 = b2*(xx+0.5)*(xx+0.5) + a2*(yy-1)*(yy-1) - a2*b2;
    while ( yy > 0 ) {				// region 2
	if ( d2 < 0 ) {
	    d2 = d2 + 2.0*b2*(xx+1) + a2*(3-2*yy);
	    xx++;
	    yy--;
	} else {
	    d2 = d2 + a2*(3-2*yy);
	    yy--;
	}
	px[i] = xx;
	py[i] = yy;
	i++;
    }
    s = i;
    ar.resize( 4*s );				// make full point array
    x += w/2;
    y += h/2;
    for ( i=0; i<s; i++ ) {			// mirror
	xx = px[i];
	yy = py[i];
	ar.setPoint( s-i-1, x+xx, y-yy );
	ar.setPoint( s+i, x-xx, y-yy );
	ar.setPoint( 3*s-i-1, x-xx, y+yy );
	ar.setPoint( 3*s+i, x+xx, y+yy );
    }
    delete[] px;
    delete[] py;
}



void TDEIconEditGrid::drawEllipse(bool drawit)
{
    if(drawit)
    {
        drawPointArray(pntarray, Draw);
        p = *img;
        emit changed(p);
        return;
    }

    TQPointArray a(pntarray.copy());
    int x = start.x(), y = start.y(), cx, cy;

    if(x > end.x())
    {
        cx = x - end.x();
        x = x - cx;
    }
    else
        cx = end.x() - x;
    if(y > end.y())
    {
        cy = y - end.y();
        y = y - cy;
    }
    else
        cy = end.y() - y;

    int d = (cx > cy) ? cx : cy;

    //kdDebug(4640) << x << ", " << y << " - " << d << " " << d << endl;
    pntarray.resize(0);
    drawPointArray(a, Mark);

    if(tool == Circle || tool == FilledCircle || tool == SelectCircle)
        TQPA_makeEllipse(pntarray, x, y, d, d);
    else if(tool == Ellipse || tool == FilledEllipse)
        TQPA_makeEllipse(pntarray, x, y, cx, cy);

    if((tool == FilledEllipse) || (tool == FilledCircle)
    || (tool == SelectCircle))
    {
        int s = pntarray.size();
        int points = s;
        for(int i = 0; i < s; i++)
        {
            int x = pntarray[i].x();
            int y = pntarray[i].y();
            for(int j = 0; j < s; j++)
            {
                if((pntarray[j].y() == y) && (pntarray[j].x() > x))
                {
                    for(int k = x; k < pntarray[j].x(); k++, points++)
                        pntarray.putPoints(points, 1, k, y);
                    break;
                }
            }
        }
    }

    drawPointArray(pntarray, Mark);

    if(tool == SelectCircle && pntarray.size() > 0 && !ispasting)
        emit selecteddata(true);
}


void TDEIconEditGrid::drawRect(bool drawit)
{
    if(drawit)
    {
        drawPointArray(pntarray, Draw);
        p = *img;
        emit changed(p);
        return;
    }

    TQPointArray a(pntarray.copy());
    int x = start.x(), y = start.y(), cx, cy;

    if(x > end.x())
    {
        cx = x - end.x();
        x = x - cx;
    }
    else
        cx = end.x() - x;
    if(y > end.y())
    {
        cy = y - end.y();
        y = y - cy;
    }
    else
        cy = end.y() - y;

    //kdDebug(4640) << x << ", " << y << " - " << cx << " " << cy << endl;
    pntarray.resize(0);
    drawPointArray(a, Mark); // remove previous marking

    int points = 0;
    bool pasting = ispasting;

    if(tool == FilledRect || (tool == SelectRect))
    {
        for(int i = x; i <= x + (pasting ? cx + 1 : cx); i++)
        {
            for(int j = y; j <= y+cy; j++, points++)
            pntarray.putPoints(points, 1, i, j);
        }
    }
    else
    {
        for(int i = x; i <= x+cx; i++, points++)
            pntarray.putPoints(points, 1, i, y);
        for(int i = y; i <= y+cy; i++, points++)
            pntarray.putPoints(points, 1, x, i);
        for(int i = x; i <= x+cx; i++, points++)
            pntarray.putPoints(points, 1, i, y+cy);
        for(int i = y; i <= y+cy; i++, points++)
            pntarray.putPoints(points, 1, x+cx, i);
    }

    drawPointArray(pntarray, Mark);

    if(tool == SelectRect && pntarray.size() > 0 && !ispasting)
        emit selecteddata(true);
}


void TDEIconEditGrid::drawLine(bool drawit, bool drawStraight)
{
    if(drawit)
    {
        drawPointArray(pntarray, Draw);
        p = *img;
        emit changed(p);
        return;
    }

    TQPointArray a(pntarray.copy());
    pntarray.resize(0);

    // remove previous marking
    drawPointArray(a, Mark);

    int x, y, dx, dy, delta;

    dx = end.x() - start.x();
    dy = end.y() - start.y();
    x = start.x();
    y = start.y();

    delta = TQMAX(abs(dx), abs(dy));
    int deltaX = abs(dx);
    int deltaY = abs(dy);

    if ((drawStraight) && (delta > 0))
    {
        dx /= delta;
        dy /= delta;

        for(int i = 0; i <= delta; i++)
        {
            pntarray.putPoints(i, 1, x, y);
            x += dx;
            y += dy;
        }
    }

    else if ((delta > 0) && (deltaX >= deltaY))
    {
        for(int i = 0; i <= deltaX; i++)
        {
            pntarray.putPoints(i, 1, x, y);

            if(dx > 0)
               x++;
            else
               x--;

            if(dy >= 0)
                y = start.y() + (abs(start.x() - x) * deltaY) / deltaX;
            else
                y = start.y() - (abs(start.x() - x) * deltaY) / deltaX;
        }
    }

    else if ((delta > 0) && (deltaY > deltaX))
    {
        for(int i = 0; i <= deltaY; i++)
        {
            pntarray.putPoints(i, 1, x, y);

            if(dy > 0)
                y++;
            else
                y--;

            if(dx >= 0)
                x = start.x() + (abs(start.y() - y) * deltaX) / deltaY;
            else
                x = start.x() - (abs(start.y() - y) * deltaX) / deltaY;
        }
    }

    drawPointArray(pntarray, Mark);
}


void TDEIconEditGrid::drawPointArray(TQPointArray a, DrawAction action)
{
    TQRect area( a.boundingRect().x()*cellSize()-1, a.boundingRect().y()*cellSize()-1,
                a.boundingRect().width()*cellSize()+1, a.boundingRect().height()*cellSize()+1 );
    
    KMacroCommand* macro = 0;
    bool doupdate = false;

    if( a.size() > 0 && action == Draw ) {
	// might cause a memmory leak, if
	// macro is never used and never 
	// added to the history! TODO: Fix this
        macro = new KMacroCommand( i18n("Drawn Array") );
	RepaintCommand* rc = new RepaintCommand( area, this );
	macro->addCommand( rc );	
    }
    
    int s = a.size(); //((rect.size().width()) * (rect.size().height()));
    for(int i = 0; i < s; i++)
    {
        int x = a[i].x();
        int y = a[i].y();

        if(img->valid(x, y) && a.boundingRect().contains(a[ i ]))
        {
            //kdDebug(4640) << "x: " << x << " - y: " << y << endl;
            switch( action )
            {
                case Draw:
                {
		    DrawCommand* dc = new DrawCommand( x, y, currentcolor, img, this );
		    dc->execute();
                    //*((uint*)img->scanLine(y) + x) = currentcolor; //colors[cell]|OPAQUE;
                    //int cell = y * numCols() + x;
                    //setColor( cell, currentcolor, false );
                    doupdate = true;
                    //updateCell( y, x, FALSE );
		    macro->addCommand( dc );
                    break;
                }

                case Mark:
                case UnMark:
                    update(x*cellsize,y*cellsize, cellsize, cellsize);
                    //updateCell( y, x, true );
                    break;

                default:
                    break;
            }
        }
    }

    
    if(doupdate)
    {
        setModified( true );
        updateColors();
	RepaintCommand* rc = new RepaintCommand( area, this );
        rc->execute();
	macro->addCommand( rc );
	pntarray.resize(0);
	// add to undo/redo history	
	history->addCommand( macro, false );    }
}

void TDEIconEditGrid::updatePreviewPixmap()
{
    p = *img;
    emit changed(TQPixmap(p));
}


bool TDEIconEditGrid::isMarked(TQPoint point)
{
    return isMarked(point.x(), point.y());
}


bool TDEIconEditGrid::isMarked(int x, int y)
{
    if(((y * numCols()) + x) == selected)
        return true;

    int s = pntarray.size();
    for(int i = 0; i < s; i++)
    {
        if(y == pntarray[i].y() && x == pntarray[i].x())
            return true;
    }

    return false;
}


// Fast diffuse dither to 3x3x3 color cube
// Based on TQt's image conversion functions
static bool kdither_32_to_8( const TQImage *src, TQImage *dst )
{
    TQRgb *p;
    uchar  *b;
    int	    y;

	//printf("kconvert_32_to_8\n");

    if ( !dst->create(src->width(), src->height(), 8, 256) ) {
		kdWarning() << "OImage: destination image not valid" << endl;
		return FALSE;
	}

    int ncols = 256;

    static uint bm[16][16];
    static int init=0;
    if (!init)
    {
		// Build a Bayer Matrix for dithering
		init = 1;
		int n, i, j;

		bm[0][0]=0;

		for (n=1; n<16; n*=2)
        {
	    	for (i=0; i<n; i++)
            {
			    for (j=0; j<n; j++)
                {
		    	    bm[i][j]*=4;
		    	    bm[i+n][j]=bm[i][j]+2;
		    	    bm[i][j+n]=bm[i][j]+3;
		    	    bm[i+n][j+n]=bm[i][j]+1;
			    }
	    	}
		}

		for (i=0; i<16; i++)
	    	for (j=0; j<16; j++)
			    bm[i][j]<<=8;
    }

    dst->setNumColors( ncols );

#define MAX_R 2
#define MAX_G 2
#define MAX_B 2
#define INDEXOF(r,g,b) (((r)*(MAX_G+1)+(g))*(MAX_B+1)+(b))

	int rc, gc, bc;

	for ( rc=0; rc<=MAX_R; rc++ )		// build 2x2x2 color cube
	    for ( gc=0; gc<=MAX_G; gc++ )
		    for ( bc=0; bc<=MAX_B; bc++ )
            {
		        dst->setColor( INDEXOF(rc,gc,bc),
			    tqRgb( rc*255/MAX_R, gc*255/MAX_G, bc*255/MAX_B ) );
		    }

	int sw = src->width();
	int* line1[3];
	int* line2[3];
	int* pv[3];

	line1[0] = new int[src->width()];
	line2[0] = new int[src->width()];
	line1[1] = new int[src->width()];
	line2[1] = new int[src->width()];
	line1[2] = new int[src->width()];
	line2[2] = new int[src->width()];
	pv[0] = new int[sw];
	pv[1] = new int[sw];
	pv[2] = new int[sw];

	for ( y=0; y < src->height(); y++ )
    {
	    p = (TQRgb *)src->scanLine(y);
	    b = dst->scanLine(y);
		int endian = (TQImage::systemByteOrder() == TQImage::BigEndian);
		int x;
		uchar* q = const_cast<TQImage*>(src)->scanLine(y);
		uchar* q2 = const_cast<TQImage*>(src)->scanLine(y+1 < src->height() ? y + 1 : 0);
		for (int chan = 0; chan < 3; chan++)
        {
		    b = dst->scanLine(y);
		    int *l1 = (y&1) ? line2[chan] : line1[chan];
		    int *l2 = (y&1) ? line1[chan] : line2[chan];
		    if ( y == 0 )
            {
			    for (int i=0; i<sw; i++)
			        l1[i] = q[i*4+chan+endian];
		    }
		    if ( y+1 < src->height() )
            {
			    for (int i=0; i<sw; i++)
			        l2[i] = q2[i*4+chan+endian];
		    }
		    // Bi-directional error diffusion
		    if ( y&1 )
            {
			    for (x=0; x<sw; x++)
                {
			        int pix = TQMAX(TQMIN(2, (l1[x] * 2 + 128)/ 255), 0);
			        int err = l1[x] - pix * 255 / 2;
			        pv[chan][x] = pix;

			        // Spread the error around...
			        if ( x+1<sw )
                    {
				        l1[x+1] += (err*7)>>4;
				        l2[x+1] += err>>4;
			        }
			        l2[x]+=(err*5)>>4;
			        if (x>1)
				        l2[x-1]+=(err*3)>>4;
			    }
		    }
            else
            {
			    for (x=sw; x-->0; )
                {
			        int pix = TQMAX(TQMIN(2, (l1[x] * 2 + 128)/ 255), 0);
			        int err = l1[x] - pix * 255 / 2;
			        pv[chan][x] = pix;

			        // Spread the error around...
			        if ( x > 0 )
                    {
				        l1[x-1] += (err*7)>>4;
				        l2[x-1] += err>>4;
			        }
			        l2[x]+=(err*5)>>4;
			        if (x+1 < sw)
				        l2[x+1]+=(err*3)>>4;
			    }
		    }
		}
		if (endian)
        {
		    for (x=0; x<sw; x++)
            {
			    *b++ = INDEXOF(pv[2][x],pv[1][x],pv[0][x]);
		    }
		}
        else
        {
		    for (x=0; x<sw; x++)
            {
			    *b++ = INDEXOF(pv[0][x],pv[1][x],pv[2][x]);
		    }
		}
	}

	delete [] line1[0];
	delete [] line2[0];
	delete [] line1[1];
	delete [] line2[1];
	delete [] line1[2];
	delete [] line2[2];
	delete [] pv[0];
	delete [] pv[1];
	delete [] pv[2];

#undef MAX_R
#undef MAX_G
#undef MAX_B
#undef INDEXOF

    return TRUE;
}

// this doesn't work the way it should but the way KPixmap does.
void TDEIconEditGrid::mapToKDEPalette()
{
    TQImage dest;

    kdither_32_to_8(img, &dest);
    *img = dest.convertDepth(32);

    for(int y = 0; y < img->height(); y++)
    {
        uint *l = (uint*)img->scanLine(y);
        for(int x = 0; x < img->width(); x++, l++)
        {
            if(*l < 0xff000000)
            {
                *l = *l | 0xff000000;
            }
        }
    }

    load(img);
    return;

/*
#if [[[TQT_VERSION IS DEPRECATED]]] > 140
  *img = img->convertDepthWithPalette(32, iconpalette, 42);
  load(img);
  return;
#endif
*/

    TQApplication::setOverrideCursor(waitCursor);
    for(int y = 0; y < numRows(); y++)
    {
        uint *l = (uint*)img->scanLine(y);
        for(int x = 0; x < numCols(); x++, l++)
        {
            if(*l != TRANSPARENT)
            {
                if(!iconcolors.contains(*l))
                    *l = iconcolors.closestMatch(*l);
            }
        }
    }

    load(img);
    setModified(true);
    TQApplication::restoreOverrideCursor();
}


void TDEIconEditGrid::grayScale()
{
    for(int y = 0; y < numRows(); y++)
    {
        uint *l = (uint*)img->scanLine(y);
        for(int x = 0; x < numCols(); x++, l++)
        {
            if(*l != TRANSPARENT)
            {
                uint c = tqGray(*l);
                *l = tqRgba(c, c, c, tqAlpha(*l));
            }
        }
    }

    load(img);
    setModified(true);
}


void TDEIconEditGrid::clearImage(TQImage *image)
{
    if(image->depth() != 32)
    {
        image->fill(TRANSPARENT);
    }
    else
    {
        // TQImage::fill() does not set the alpha channel so do it
        // manually.
        for(int y = 0; y < image->height(); y++)
        {
            uint *l = (uint*)image->scanLine(y);
            for(int x = 0; x < image->width(); x++, l++)
            {
                *l = TRANSPARENT;
            }
        }
    }
}


void TDEIconEditGrid::setModified(bool m)
{
    if(m != modified)
    {
        modified = m;
        emit modifiedchanged(m);
    }
}


#include "tdeicongrid.moc"
