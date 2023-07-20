
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


#ifndef __kptoolpolygon_h__
#define __kptoolpolygon_h__

#include <tqbrush.h>
#include <tqpen.h>
#include <tqobject.h>
#include <tqpixmap.h>
#include <tqpoint.h>
#include <tqpointarray.h>
#include <tqrect.h>

#include <kpcommandhistory.h>

#include <kpcolor.h>
#include <kptool.h>
#include <kptoolwidgetfillstyle.h>

class TQMouseEvent;
class TQPen;
class TQPoint;
class TQRect;
class TQString;

class kpView;
class kpDocument;
class kpMainWindow;

class kpToolWidgetFillStyle;
class kpToolWidgetLineWidth;
class kpViewManager;

class kpToolPolygon : public kpTool
{
TQ_OBJECT
  

public:
    enum Mode
    {
        Polygon, Polyline, Line, Curve
    };

    kpToolPolygon (Mode mode, const TQString &text, const TQString &description,
                   int key,
                   kpMainWindow *mainWindow, const char *name);
    kpToolPolygon (kpMainWindow *mainWindow);
    virtual ~kpToolPolygon ();

    void setMode (Mode mode);

    virtual bool careAboutModifierState () const { return true; }

private:
    TQString haventBegunShapeUserMessage () const;

public:
    virtual void begin ();
    virtual void end ();

    virtual void beginDraw ();
    virtual void draw (const TQPoint &, const TQPoint &, const TQRect &);
    virtual void cancelShape ();
    virtual void releasedAllButtons ();
    virtual void endDraw (const TQPoint &, const TQRect &);
    virtual void endShape (const TQPoint & = TQPoint (), const TQRect & = TQRect ());

    virtual bool hasBegunShape () const;

public slots:
    void slotLineWidthChanged (int width);
    void slotFillStyleChanged (kpToolWidgetFillStyle::FillStyle fillStyle);

protected slots:
    virtual void slotForegroundColorChanged (const kpColor &);
    virtual void slotBackgroundColorChanged (const kpColor &);

private slots:
    void updateShape ();

private:
    Mode m_mode;

    kpToolWidgetFillStyle *m_toolWidgetFillStyle;

    int m_lineWidth;
    kpToolWidgetLineWidth *m_toolWidgetLineWidth;

    int m_originatingMouseButton;

    void applyModifiers ();

    TQPoint m_toolLineStartPoint, m_toolLineEndPoint;
    TQRect m_toolLineRect;

    TQPointArray m_points;
};

class kpToolPolygonCommand : public kpNamedCommand
{
public:
    kpToolPolygonCommand (const TQString &name,
                          const TQPointArray &points,
                          const TQRect &normalizedRect,
                          const kpColor &foregroundColor, const kpColor &backgroundColor,
                          int lineWidth, Qt::PenStyle lineStyle,
                          kpToolWidgetFillStyle *toolWidgetFillStyle,
                          const TQPixmap &originalArea,
                          kpToolPolygon::Mode mode,
                          kpMainWindow *mainWindow);
    virtual ~kpToolPolygonCommand ();

    virtual int size () const;
    
    virtual void execute ();
    virtual void unexecute ();

private:
    TQPointArray m_points;
    TQRect m_normalizedRect;

    kpColor m_foregroundColor, m_backgroundColor;
    int m_lineWidth;
    Qt::PenStyle m_lineStyle;
    kpToolWidgetFillStyle *m_toolWidgetFillStyle;

    TQPixmap m_originalArea;
    kpToolPolygon::Mode m_mode;
};

#endif  // __kptoolpolygon_h__
