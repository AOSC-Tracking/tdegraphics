
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


#ifndef KP_VIEW_SCROLLABLE_CONTAINER_H
#define KP_VIEW_SCROLLABLE_CONTAINER_H


#include <tqpoint.h>
#include <tqscrollview.h>
#include <tqsize.h>


class TQCursor;
class TQRect;
class TQTimer;

class kpGrip;
class kpView;
class kpMainWindow;


// TODO: refactor by sharing iface's with kpTool
class kpGrip : public TQWidget
{
TQ_OBJECT
  

public:
    enum GripType
    {
        Right = 1, Bottom = 2,
        BottomRight = Right | Bottom
    };

    kpGrip (GripType type,
            TQWidget *parent, const char *name = 0);
    virtual ~kpGrip ();

    GripType type () const;

    static const TQCursor &cursorForType (GripType type);

    TQRect hotRect (bool toGlobal = false) const;

    bool isDrawing () const;

signals:
    void beganDraw ();
    void continuedDraw (int viewDX, int viewDY, bool dueToDragScroll);
    void cancelledDraw ();
    void endedDraw (int viewDX, int viewDY);

    void statusMessageChanged (const TQString &string);

    void releasedAllButtons ();

public:
    TQString haventBegunDrawUserMessage () const;

    TQString userMessage () const;
    void setUserMessage (const TQString &message);

protected:
    void updatePixmap ();
    void cancel ();

protected:
    virtual void keyReleaseEvent (TQKeyEvent *e);
    virtual void mousePressEvent (TQMouseEvent *e);
public:
    TQPoint viewDeltaPoint () const;
    void mouseMovedTo (const TQPoint &point, bool dueToDragScroll);
protected:
    virtual void mouseMoveEvent (TQMouseEvent *e);
    virtual void mouseReleaseEvent (TQMouseEvent *e);
    virtual void resizeEvent (TQResizeEvent *e);

    virtual void enterEvent (TQEvent *e);
    virtual void leaveEvent (TQEvent *e);

    virtual void paintEvent (TQPaintEvent *e);

protected:
    GripType m_type;
    TQPoint m_startPoint, m_currentPoint;
    TQString m_userMessage;
    bool m_shouldReleaseMouseButtons;
};


class kpViewScrollableContainer : public TQScrollView
{
TQ_OBJECT
  

public:
    kpViewScrollableContainer (kpMainWindow *parent, const char *name = 0);
    virtual ~kpViewScrollableContainer ();

    // Same as contentsX() and contentsY() except that after
    // contentsMovingSoon() is emitted and before the scrollview actually
    // scrolls, they return the would be values of contentsX() and
    // contentsY() after scrolling.
    int contentsXSoon ();
    int contentsYSoon ();

signals:
    // connect to this instead of contentsMoving(int,int) so that
    // contentsXSoon() and contentsYSoon() work
    void contentsMovingSoon (int contentsX, int contentsY);

    void beganDocResize ();
    void continuedDocResize (const TQSize &size);
    void cancelledDocResize ();
    void endedDocResize (const TQSize &size);

    // (string.isEmpty() if kpViewScrollableContainer has nothing to say)
    void statusMessageChanged (const TQString &string);

    void resized ();

public:
    TQSize newDocSize () const;
    bool haveMovedFromOriginalDocSize () const;
    TQString statusMessage () const;
    void clearStatusMessage ();

protected:
    void connectGripSignals (kpGrip *grip);

    TQSize newDocSize (int viewDX, int viewDY) const;

    void calculateDocResizingGrip ();
    kpGrip *docResizingGrip () const;

    int bottomResizeLineWidth () const;
    int rightResizeLineWidth () const;

    TQRect bottomResizeLineRect () const;
    TQRect rightResizeLineRect () const;
    TQRect bottomRightResizeLineRect () const;

    TQPoint mapViewToViewport (const TQPoint &viewPoint);
    TQRect mapViewToViewport (const TQRect &viewRect);

    TQRect mapViewportToGlobal (const TQRect &viewportRect);
    TQRect mapViewToGlobal (const TQRect &viewRect);

    void repaintWidgetAtResizeLineViewRect (TQWidget *widget,
                                            const TQRect &resizeLineViewRect);
    void repaintWidgetAtResizeLines (TQWidget *widget);
    void eraseResizeLines ();

    void drawResizeLines ();

    void updateResizeLines (int viewX, int viewY,
                            int viewDX, int viewDY);

protected slots:
    void slotGripBeganDraw ();
    void slotGripContinuedDraw (int viewDX, int viewDY, bool dueToScrollView);
    void slotGripCancelledDraw ();
    void slotGripEndedDraw (int viewDX, int viewDY);

    void slotGripStatusMessageChanged (const TQString &string);

public slots:
    void recalculateStatusMessage ();

protected slots:
    void slotContentsMoving (int x, int y);
    void slotContentsMoved ();

protected:
    void disconnectViewSignals ();
    void connectViewSignals ();

public:
    // Calls setView(<widget>) after adding <widget> if it's a kpView.
    virtual void addChild (TQWidget *widget, int x = 0, int y = 0);

    kpView *view () const;
    void setView (kpView *view);

public slots:
    void updateGrips ();
protected slots:
    void slotViewDestroyed ();

public slots:
    // TODO: Why the TQPoint's?
    //       Why the need for view's zoomLevel?  We have the view() anyway.
    bool beginDragScroll (const TQPoint &, const TQPoint &,
                          int zoomLevel,
                          bool *didSomething);
    bool beginDragScroll (const TQPoint &, const TQPoint &,
                          int zoomLevel);
    bool endDragScroll ();

protected slots:
    bool slotDragScroll (bool *didSomething);
    bool slotDragScroll ();

protected:
    TQRect noDragScrollRect () const;

    virtual void contentsDragMoveEvent (TQDragMoveEvent *e);
    virtual void contentsMouseMoveEvent (TQMouseEvent *e);
    virtual void contentsWheelEvent (TQWheelEvent *e);
    virtual void mouseMoveEvent (TQMouseEvent *e);
    virtual bool eventFilter (TQObject *watchedObject, TQEvent *e);
    virtual void viewportPaintEvent (TQPaintEvent *e);
    virtual void paintEvent (TQPaintEvent *e);
    virtual void resizeEvent (TQResizeEvent *e);

protected:
    kpMainWindow *m_mainWindow;
    int m_contentsXSoon, m_contentsYSoon;
    kpView *m_view;
    kpGrip *m_bottomGrip, *m_rightGrip, *m_bottomRightGrip;
    kpGrip *m_docResizingGrip;
    TQTimer *m_dragScrollTimer;
    int m_zoomLevel;
    bool m_scrollTimerRunOnce;
    int m_resizeRoundedLastViewX, m_resizeRoundedLastViewY;
    int m_resizeRoundedLastViewDX, m_resizeRoundedLastViewDY;
    bool m_haveMovedFromOriginalDocSize;
    TQString m_gripStatusMessage;
};


#endif  // KP_VIEW_SCROLLABLE_CONTAINER_H
