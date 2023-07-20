
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


#ifndef __kptoolrectangle_h__
#define __kptoolrectangle_h__

#include <tqbrush.h>
#include <tqpen.h>
#include <tqpixmap.h>
#include <tqpoint.h>
#include <tqrect.h>

#include <kpcommandhistory.h>

#include <kptool.h>

class TQString;

class kpColor;
class kpMainWindow;
class kpToolWidgetFillStyle;
class kpToolWidgetLineWidth;
class kpViewManager;

class kpToolRectangle : public kpTool
{
TQ_OBJECT
  

public:
    // it turns out that these shapes are all really the same thing
    // (same options, same feel) - the only real difference is the
    // drawing functions (a one line change)
    enum Mode {Rectangle, RoundedRectangle, Ellipse};

    kpToolRectangle (Mode mode,
                     const TQString &text, const TQString &description,
                     int key,
                     kpMainWindow *mainWindow,
                     const char *name);
    kpToolRectangle (kpMainWindow *);
    virtual ~kpToolRectangle ();

    void setMode (Mode mode);

    virtual bool careAboutModifierState () const { return true; }

private:
    TQString haventBegunDrawUserMessage () const;

public:
    virtual void begin ();
    virtual void end ();

    virtual void beginDraw ();
private:
    void updateShape ();
public:
    virtual void draw (const TQPoint &, const TQPoint &, const TQRect &);
    virtual void cancelShape ();
    virtual void releasedAllButtons ();
    virtual void endDraw (const TQPoint &, const TQRect &);

private slots:
    void updatePens ();
    void updateBrushes ();

    virtual void slotForegroundColorChanged (const kpColor &);
    virtual void slotBackgroundColorChanged (const kpColor &);

    virtual void slotLineWidthChanged ();
    virtual void slotFillStyleChanged ();

private:
    Mode m_mode;

    kpToolWidgetLineWidth *m_toolWidgetLineWidth;
    kpToolWidgetFillStyle *m_toolWidgetFillStyle;

    void updatePen (int mouseButton);
    TQPen m_pen [2], m_maskPen [2];

    void updateBrush (int mouseButton);
    TQBrush m_brush [2], m_maskBrush [2];

    void applyModifiers ();
    TQPoint m_toolRectangleStartPoint, m_toolRectangleEndPoint;
    TQRect m_toolRectangleRectWithoutLineWidth, m_toolRectangleRect;
};

class kpToolRectangleCommand : public kpCommand
{
public:
    kpToolRectangleCommand (kpToolRectangle::Mode mode,
                            const TQPen &pen, const TQPen &maskPen,
                            const TQBrush &brush, const TQBrush &maskBrush,
                            const TQRect &rect,
                            const TQPoint &startPoint, const TQPoint &endPoint,
                            kpMainWindow *mainWindow);
    virtual ~kpToolRectangleCommand ();

    virtual TQString name () const;

    virtual int size () const;

    virtual void execute ();
    virtual void unexecute ();

private:
    kpToolRectangle::Mode m_mode;
    TQPen m_pen, m_maskPen;
    TQBrush m_brush, m_maskBrush;
    TQRect m_rect;
    TQPoint m_startPoint, m_endPoint;
    TQPixmap *m_oldPixmapPtr;
};

#endif  // __kptoolrectangle_h__
