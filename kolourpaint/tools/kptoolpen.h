
/*
   Copyright (c) 2003,2004,2005 Clarence Dang <dang@kde.org>
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
   IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
   IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
   NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
   THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#ifndef __kptoolpen_h__
#define __kptoolpen_h__

#include <tqpixmap.h>
#include <tqrect.h>

#include <kpcommandhistory.h>
#include <kptool.h>

class TQPoint;
class TQString;

class kpColor;
class kpMainWindow;
class kpToolPenCommand;
class kpToolWidgetBrush;
class kpToolWidgetEraserSize;
class kpViewManager;

class kpToolPen : public kpTool
{
TQ_OBJECT
  

public:
    enum Mode
    {
        // tool properties
        DrawsPixels = (1 << 0), DrawsPixmaps = (1 << 1), WashesPixmaps = (1 << 2),
        NoBrushes = 0, SquareBrushes = (1 << 3), DiverseBrushes = (1 << 4),
        NormalColors = 0, SwappedColors = (1 << 5),

        // tools:
        //
        // Pen = draws pixels, "interpolates" by "sweeping" pixels along a line (no brushes)
        // Brush = draws pixmaps, "interpolates" by "sweeping" pixmaps along a line (interesting brushes)
        // Eraser = Brush but with foreground & background colors swapped (a few square brushes)
        // Color Washer = Brush that replaces/washes the foreground color with the background color
        //
        // (note the capitalization of "brush" here :))
        Pen = DrawsPixels | NoBrushes | NormalColors,
        Brush = DrawsPixmaps | DiverseBrushes | NormalColors,
        Eraser = DrawsPixmaps | SquareBrushes | SwappedColors,
        ColorWasher = WashesPixmaps | SquareBrushes | SwappedColors
    };

    kpToolPen (Mode mode, const TQString &text, const TQString &description,
               int key,
               kpMainWindow *mainWindow, const char *name);
    kpToolPen (kpMainWindow *mainWindow);
    virtual ~kpToolPen ();

    void setMode (Mode mode);

private:
    TQString haventBegunDrawUserMessage () const;
    
public:
    virtual void begin ();
    virtual void end ();

    virtual void beginDraw ();
    virtual void hover (const TQPoint &point);
    virtual void globalDraw ();
    virtual void draw (const TQPoint &thisPoint, const TQPoint &lastPoint, const TQRect &);
    virtual void cancelShape ();
    virtual void releasedAllButtons ();
    virtual void endDraw (const TQPoint &, const TQRect &);

private slots:
    virtual void slotForegroundColorChanged (const kpColor &col);
    virtual void slotBackgroundColorChanged (const kpColor &col);

    void slotBrushChanged (const TQPixmap &pixmap, bool isDiagonalLine);
    void slotEraserSizeChanged (int size);

private:
    bool wash (TQPainter *painter, TQPainter *maskPainter,
               const TQImage &image,
               const kpColor &colorToReplace,
               const TQRect &imageRect, int plotx, int ploty);
    bool wash (TQPainter *painter, TQPainter *maskPainter,
               const TQImage &image,
               const kpColor &colorToReplace,
               const TQRect &imageRect, const TQRect &drawRect);

    kpColor color (int which);

    TQPoint hotPoint () const;
    TQPoint hotPoint (int x, int y) const;
    TQPoint hotPoint (const TQPoint &point) const;
    TQRect hotRect () const;
    TQRect hotRect (int x, int y) const;
    TQRect hotRect (const TQPoint &point) const;

    Mode m_mode;

    void updateBrushCursor (bool recalc = true);

    kpToolWidgetBrush *m_toolWidgetBrush;
    kpToolWidgetEraserSize *m_toolWidgetEraserSize;
    TQPixmap m_brushPixmap [2];
    TQPixmap m_cursorPixmap;
    bool m_brushIsDiagonalLine;

    kpToolPenCommand *m_currentCommand;
};

class kpToolPenCommand : public kpNamedCommand
{
public:
    kpToolPenCommand (const TQString &name, kpMainWindow *mainWindow);
    virtual ~kpToolPenCommand ();

    virtual int size () const;
    
    virtual void execute ();
    virtual void unexecute ();

    // interface for TDEToolPen
    void updateBoundingRect (const TQPoint &point);
    void updateBoundingRect (const TQRect &rect);
    void finalize ();
    void cancel ();

private:
    void swapOldAndNew ();

    TQPixmap m_pixmap;
    TQRect m_boundingRect;
};

#endif  // __kptoolpen_h__
