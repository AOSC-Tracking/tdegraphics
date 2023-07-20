
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

#ifndef __kp_tool_h__
#define __kp_tool_h__

#include <tqobject.h>
#include <tqpoint.h>
#include <tqrect.h>
#include <tqsize.h>
#include <tqstring.h>

#include <kpdefs.h>


class TQIconSet;
class TQPixmap;

class KKeySequence;
class TDEShortcut;

class kpColor;
class kpColorToolBar;
class kpCommandHistory;
class kpDocument;
class kpView;
class kpViewManager;
class kpMainWindow;
class kpToolAction;
class kpToolToolBar;


// Base class for all tools
class kpTool : public TQObject
{
TQ_OBJECT
  

public:
    kpTool (const TQString &text, const TQString &description,
            int key,
            kpMainWindow *mainWindow, const char *name);
    virtual ~kpTool ();

private:
    void init (const TQString &text, const TQString &description,
               int key,
               kpMainWindow *mainWindow, const char *name);


protected:
    void createAction ();

    int m_key;
    kpToolAction *m_action;

signals:
    void actionToolTipChanged (const TQString &string);

protected slots:
    void slotActionToolTipChanged (const TQString &string);

public:
    TQString text () const;
    void setText (const TQString &text);

    static TQString toolTipForTextAndShortcut (const TQString &text,
        const TDEShortcut &shortcut);
    TQString toolTip () const;

    TQString description () const;
    void setDescription (const TQString &description);

    int key () const;
    void setKey (int key);

    // Given a single <key>, returns a shortcut with <key>
    // (disabled when the user is editing text) and as an alternate,
    // <some modifiers>+<key>.
    static TDEShortcut shortcutForKey (int key);
    TDEShortcut shortcut () const;

    static bool keyIsText (int key);
    static bool containsSingleKeyTrigger (const KKeySequence &seq);
    static bool containsSingleKeyTrigger (const TDEShortcut &shortcut,
        TDEShortcut *shortcutWithoutSingleKeyTriggers);

    bool singleKeyTriggersEnabled () const;
    void enableSingleKeyTriggers (bool enable = true);

    const char *name () const;


    static TQRect neededRect (const TQRect &rect, int lineWidth);
    static TQPixmap neededPixmap (const TQPixmap &pixmap, const TQRect &boundingRect);

    bool hasCurrentPoint () const;
    // Returns the position of the cursor relative to the topleft point of
    // the current view (viewUnderStartPoint() or viewUnderCursor() otherwise).
    //
    // If neither viewUnderStartPoint() nor viewUnderCursor()
    // (i.e. !hasCurrentPoint()), then it returns KP_INVALID_POINT.
    //
    // If <zoomToDoc> is set (default), then it returns the position in the
    // document.  This theoretically == m_currentPoint (when m_currentPoint
    // is defined) but I wouldn't bet on it.  This function is useful when
    // m_currentPoint isn't necessarily defined (outside of beginDraw(),draw()
    // and hover()).
    //
    // If <zoomToDoc> is not set, then it returns an unzoomed view coordinate.
    //
    // Keep in mind that if viewUnderStartPoint(), this can return coordinates
    // outside of the document/view.
    TQPoint currentPoint (bool zoomToDoc = true) const;

public slots:
    // Call this when something below the mouse cursor may have changed
    // and/or if the view has moved relative to the cursor (as opposed to
    // the cursor moving relative to the view, which would trigger a
    // mouseMoveEvent and all would be well without such hacks)
    // e.g. when zooming or scrolling the view or when deleting a selection.
    //
    // This calls hover() or draw() to let the tool know.  The Brush Tool
    // can then update the position of the Brush Cursor.  The Selection
    // Tool can update the real cursor.  The Line Tool can update the current
    // line.  The statubar gets correct coordinates.  etc. etc.
    void somethingBelowTheCursorChanged ();

private:
    // Same as above except that you claim you know better than currentPoint()
    void somethingBelowTheCursorChanged (const TQPoint &currentPoint_,
                                         const TQPoint &currentViewPoint_);

public:
    // called when the tool is selected
    virtual void begin ();

    // called when the tool is deselected
    virtual void end ();

    // set after begin() has been called, unset after end() has been called
    bool hasBegun () const { return m_began; }

    bool hasBegunDraw () const { return m_beganDraw; }

    virtual bool hasBegunShape () const { return hasBegunDraw (); }

    // called when user double-left-clicks on Tool Icon (not view)
    virtual void globalDraw ();

    // called when the user clicks on the Tool Icon even though it's already
    // the current tool (used by the selection tools to deselect)
    virtual void reselect ();

signals:
    // emitted after beginDraw() has been called
    void beganDraw (const TQPoint &point);

    // Emitted just before draw() is called in mouseMoveEvent().  Slots
    // connected to this signal should return in <scrolled> whether the
    // mouse pos may have changed.  Used by drag scrolling.
    void movedAndAboutToDraw (const TQPoint &currentPoint, const TQPoint &lastPoint,
                              int zoomLevel,
                              bool *scrolled);

    // emitted after endDraw() has been called
    void endedDraw (const TQPoint &point);

    // emitted after cancelShape() has been called
    void cancelledShape (const TQPoint &point);


public:
    TQIconSet iconSet (int forceSize = 0) const;
    TQString iconName () const;
    kpToolAction *action ();

signals:
    // User clicked on the tool's action - i.e. select this tool
    void actionActivated ();

protected slots:
    void slotActionActivated ();


protected:
    virtual bool returnToPreviousToolAfterEndDraw () const { return false; }
    virtual bool careAboutModifierState () const { return false; }
    virtual bool careAboutColorsSwapped () const { return false; }

    virtual void beginDraw ();

    // mouse move without button pressed
    // (only m_currentPoint & m_currentViewPoint is defined)
    virtual void hover (const TQPoint &point);

    // this is useful for "instant" tools like the Pen & Eraser
    virtual void draw (const TQPoint &thisPoint, const TQPoint &lastPoint,
                        const TQRect &normalizedRect);

    // (m_mouseButton will not change from beginDraw())
    virtual void cancelShape ();
    virtual void releasedAllButtons ();

    virtual void endDraw (const TQPoint &thisPoint, const TQRect &normalizedRect);

    virtual void endShape (const TQPoint &thisPoint = TQPoint (),
                           const TQRect &normalizedRect = TQRect ())
    {
        endDraw (thisPoint, normalizedRect);
    }

    kpMainWindow *mainWindow () const;
    kpDocument *document () const;
    kpViewManager *viewManager () const;
    kpToolToolBar *toolToolBar () const;
    kpView *viewUnderStartPoint () const { return m_viewUnderStartPoint; }
    kpView *viewUnderCursor () const;
    kpCommandHistory *commandHistory () const;

    kpColor color (int which) const;

    kpColor foregroundColor () const;
    kpColor backgroundColor () const;

    double colorSimilarity () const;
    int processedColorSimilarity () const;

protected:
    int m_ignoreColorSignals;

protected slots:
    void slotColorsSwappedInternal (const kpColor &newForegroundColor,
                                    const kpColor &newBackgroundColor);
    void slotForegroundColorChangedInternal (const kpColor &color);
    void slotBackgroundColorChangedInternal (const kpColor &color);
    void slotColorSimilarityChangedInternal (double similarity, int processedSimilarity);

protected slots:  // TODO: there is no reason why these should be slots
    virtual void slotColorsSwapped (const kpColor & /*newForegroundColor*/, const kpColor & /*newBackgroundColor*/) {}
    virtual void slotForegroundColorChanged (const kpColor & /*color*/) {}
    virtual void slotBackgroundColorChanged (const kpColor & /*color*/) {}
    virtual void slotColorSimilarityChanged (double /*similarity*/, int /*processedSimilarity*/) {};

protected:
    // (only valid in slots connected to the respective signals above)
    kpColor oldForegroundColor () const;
    kpColor oldBackgroundColor () const;
    double oldColorSimilarity () const;

protected:
    // returns true if m_currentPoint <= 1 pixel away from m_lastPoint
    // or if there was no lastPoint
    bool currentPointNextToLast () const;  // (includes diagonal adjacency)
    bool currentPointCardinallyNextToLast () const;  // (only cardinally adjacent i.e. horiz & vert; no diag)

    int m_mouseButton;  /* 0 = left, 1 = right */
    bool m_shiftPressed, m_controlPressed, m_altPressed;  // m_altPressed is unreliable
    bool m_beganDraw;  // set after beginDraw() is called, unset before endDraw() is called
    TQPoint m_startPoint,
           m_currentPoint, m_currentViewPoint,
           m_lastPoint;

protected:
    friend class kpCommandHistory;
    friend class kpMainWindow;
    friend class kpToolToolBar;
    void beginInternal ();
    void endInternal ();

    void beginDrawInternal ();
    void endDrawInternal (const TQPoint &thisPoint, const TQRect &normalizedRect,
                          bool wantEndShape = false);
    void cancelShapeInternal ();
    void endShapeInternal (const TQPoint &thisPoint = TQPoint (),
                           const TQRect &normalizedRect = TQRect ());

    friend class kpView;

    // If you're reimplementing any of these, you probably don't know what
    // you're doing - reimplement begin(),beginDraw(),draw(),cancelShape(),
    // endDraw() etc. instead.
    virtual void mousePressEvent (TQMouseEvent *e);
    virtual void mouseMoveEvent (TQMouseEvent *e);
    virtual void mouseReleaseEvent (TQMouseEvent *e);
    virtual void wheelEvent (TQWheelEvent *e);
    
    virtual void keyPressEvent (TQKeyEvent *e);
    virtual void keyReleaseEvent (TQKeyEvent *e);
    
    virtual void imStartEvent(TQIMEvent *){}
    virtual void imComposeEvent(TQIMEvent *){}
    virtual void imEndEvent(TQIMEvent *){}
    
private:
    void keyUpdateModifierState (TQKeyEvent *e);
    void notifyModifierStateChanged ();
protected:
    virtual void setShiftPressed (bool pressed);
    virtual void setControlPressed (bool pressed);
    virtual void setAltPressed (bool pressed);
    virtual void focusInEvent (TQFocusEvent *e);
    virtual void focusOutEvent (TQFocusEvent *e);
    virtual void enterEvent (TQEvent *e);
    virtual void leaveEvent (TQEvent *e);

    // 0 = left, 1 = right, -1 = other (none, left+right, mid)
    static int mouseButton (const TQt::ButtonState &buttonState);

    TQString m_text, m_description;
    const char *m_name;

    kpMainWindow *m_mainWindow;
    bool m_began;

    kpView *m_viewUnderStartPoint;


    /*
     * User Notifications (Status Bar)
     */

public:
    // Returns "(Left|Right) click to cancel." where Left or Right is chosen
    // depending on which one is the _opposite_ of <mouseButton>
    static TQString cancelUserMessage (int mouseButton);
    TQString cancelUserMessage () const;

    TQString userMessage () const;
    void setUserMessage (const TQString &userMessage = TQString());

    TQPoint userShapeStartPoint () const;
    TQPoint userShapeEndPoint () const;
    static int calculateLength (int start, int end);
    void setUserShapePoints (const TQPoint &startPoint = KP_INVALID_POINT,
                             const TQPoint &endPoint = KP_INVALID_POINT,
                             bool setSize = true);

    TQSize userShapeSize () const;
    int userShapeWidth () const;
    int userShapeHeight () const;
    void setUserShapeSize (const TQSize &size = KP_INVALID_SIZE);
    void setUserShapeSize (int width, int height);

signals:
    void userMessageChanged (const TQString &userMessage);
    void userShapePointsChanged (const TQPoint &startPoint = KP_INVALID_POINT,
                                 const TQPoint &endPoint = KP_INVALID_POINT);
    void userShapeSizeChanged (const TQSize &size);
    void userShapeSizeChanged (int width, int height);

protected:
    TQString m_userMessage;
    TQPoint m_userShapeStartPoint, m_userShapeEndPoint;
    TQSize m_userShapeSize;


public:
    // Call this before the user tries to cause the document or selection
    // to resize from <oldWidth>x<oldHeight> to <newWidth>x<newHeight>.
    // If at least one dimension increases, the new dimensions will take a
    // large amount of memory (which causes thrashing, instability) and
    // the old dimensions did not take a large amount of memory, ask the
    // user if s/he really wants to perform the operation.
    //
    // Returns true if the operation should proceed, false otherwise.
    //
    // In order to make the translators' lives possible, this function cannot
    // generate the <text>,<caption> nor <continueButtonText> (without
    // concantenating sentences and words with tense).  However, it is
    // recommended that you give them the following values:
    //
    // e.g.:
    // text = i18n ("<qt><p>(Rotating|Skewing) the (image|selection) to"
    //              " %1x%2 may take a substantial amount of memory."
    //              " This can reduce system"
    //              " responsiveness and cause other application resource"
    //              " problems.</p>").arg (newWidth, newHeight)
    //
    //              "<p>Are you sure want to (rotate|skew) the"
    //              " (image|selection)?</p></qt>");
    // caption = i18n ("Rotate (Image|Selection)?");
    // continueButtonText = i18n ("Rotat&e (Image|Selection)");
    static bool warnIfBigImageSize (int oldWidth, int oldHeight,
                                    int newWidth, int newHeight,
                                    const TQString &text,
                                    const TQString &caption,
                                    const TQString &continueButtonText,
                                    TQWidget *parent);


protected:
    // There is no need to maintain binary compatibility at this stage.
    // The d-pointer is just so that you can experiment without recompiling
    // the kitchen sink.
    class kpToolPrivate *d;
};

#endif  // __kp_tool_h__
