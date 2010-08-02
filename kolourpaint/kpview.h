
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


#ifndef KP_VIEW_H
#define KP_VIEW_H


#include <tqwidget.h>

#include <kpdefs.h>


class kpDocument;
class kpSelection;
class kpTool;
class kpToolToolBar;
class kpViewManager;
class kpViewScrollableContainer;


/**
 * @short Abstract base class for all views.
 *
 * This is the abstract base class for all views.  A view is a widget that
 * renders a possibly zoomed onscreen representation of a document.
 *
 * 1 view corresponds to 1 document.
 * 1 document corresponds to 0 or more views.
 *
 * @see kpViewManager
 *
 * @author Clarence Dang <dang@kde.org>
 */
class kpView : public QWidget
{
Q_OBJECT

public:
    /**
     * Constructs a view.
     *
     * @param document The document this view is representing.
     * @param toolToolBar The tool tool bar.
     * @param viewManager The view manager.
     * @param buddyView The view this view watches over (e.g. a thumbnail
     *                  view would watch over the main view).  May be 0.
     *                  See for example, highlightBuddyViewRectangle().
     * @param scrollableContainer This view's scrollable container.
     *                            May be 0.
     *
     * You must call adjustEnvironment() at the end of your constructor.
     */
    kpView (kpDocument *document,
            kpToolToolBar *toolToolBar,
            kpViewManager *viewManager,
            kpView *buddyView,
            kpViewScrollableContainer *scrollableContainer,
            TQWidget *parent, const char *name);

    /**
     * Destructs this view.  Informs the viewManager() that the mouse
     * cursor is no longer above this view.
     */
    virtual ~kpView ();


    /**
     * @returns the document.
     */
    kpDocument *document () const;

protected:
    /**
     * @returns the document's selection.
     */
    kpSelection *selection () const;

public:
    /**
     * @returns the tool tool bar.
     */
    kpToolToolBar *toolToolBar () const;

protected:
    /**
     * @returns the currently selected tool.
     */
    kpTool *tool () const;

public:
    /**
     * @returns the view manager.
     */
    kpViewManager *viewManager () const;

    /**
     * @returns the buddy view.
     */
    kpView *buddyView () const;

    /**
     * @returns the buddyView()'s scrollable container.
     */
    kpViewScrollableContainer *buddyViewScrollableContainer () const;

    /**
     * @returns this view's scrollable container.
     */
    kpViewScrollableContainer *scrollableContainer () const;


    /**
     * @returns the horizontal zoom level (100 is unzoomed).
     */
    int zoomLevelX (void) const;

    /**
     * @returns the vertical zoom level (100 is unzoomed).
     */
    int zoomLevelY (void) const;

    /**
     * Sets the horizontal and vertical zoom levels.
     *
     * @param hzoom Horizontal zoom level.
     * @param vzoom Vertical zoom level.
     *
     * If reimplementing, you must call this base implementation.
     */
    virtual void setZoomLevel (int hzoom, int vzoom);


    /**
     * @returns in views coordinates, where the top-left document() pixel
     *          will be rendered (default: (0,0)).
     */
    TQPoint origin () const;

    /**
     * Sets the origin.
     *
     * @param origin New origin.
     *
     * If reimplementing, you must call this base implementation.
     */
    virtual void setOrigin (const TQPoint &origin);


    /**
     * @returns whether at this zoom level, the grid can be enabled.
     *          This is based on whether the grid can be sensibly rendered.
     */
    bool canShowGrid () const;

    /**
     * @returns whether the grid is currently shown.
     */
    bool isGridShown () const;

    /**
     * Turns on/off the grid.
     *
     * @param yes Whether to enable the grid.
     */
    void showGrid (bool yes = true);


    /**
     * @returns whether to draw a rectangle highlighting the area of
     *          buddyView() visible through buddyViewScrollableContainer().
     */
    bool isBuddyViewScrollableContainerRectangleShown () const;

    /**
     * Turns on/off the rectangle highlighting the area of buddyView()
     * visible through buddyViewScrollableContainer() and redraws.
     *
     * @param yes Whether to turn on the rectangle.
     */
    void showBuddyViewScrollableContainerRectangle (bool yes = true);

protected:
    /**
     * @returns the current rectangle highlighting the area of buddyView()
     *          visible through buddyViewScrollableContainer(), that is being
     *          rendered by this view.
     */
    TQRect buddyViewScrollableContainerRectangle () const;

protected slots:
    /**
     * Updates the buddyViewScrollableContainerRectangle() and redraws
     * appropriately.
     *
     * This is already connected to zoomLevelChanged() and originChanged();
     * buddyView() and buddyViewScrollableContainer() signals.  There is probably no
     * need to call this function directly.
     */
    void updateBuddyViewScrollableContainerRectangle ();


public:

    /**
     * @param viewX Horizontal position in view coordinates.
     *
     * @returns viewX transformed to document coordinates, based on the
     *                origin() and zoomLevelX().
     */
    double transformViewToDocX (double viewX) const;

    /**
     * @param viewY Vertical position in view coordinates.
     *
     * @returns viewY transformed to document coordinates, based on the
     *                origin() and zoomLevelY().
     */
    double transformViewToDocY (double viewY) const;

    /**
     * @param viewPoint Position in view coordinates.
     *
     * @returns viewPoint transformed to document coordinates, based on the
     *                    origin(), zoomLevelX() and zoomLevelY().
     */
    TQPoint transformViewToDoc (const TQPoint &viewPoint) const;

    /**
     * @param viewRect Rectangle in view coordinates.
     *
     * @returns viewRect transformed to document coordinates based on the
     *                   origin(), zoomLevelX() and zoomLevelY().
     *
     * For bounding rectangles, you should use this function instead of
     * transformViewToDocX(), transformViewToDocY() or
     * transformViewToDoc(const TQPoint &) which act on coordinates only.
     */
    TQRect transformViewToDoc (const TQRect &viewRect) const;


    /**
     * @param docX Horizontal position in document coordinates.
     *
     * @returns docX transformed to view coordinates, based on the origin()
     *               and zoomLevelX().
     */
    double transformDocToViewX (double docX) const;

    /**
     * @param docY Vertical position in document coordinates.
     *
     * @returns docY transformed to view coordinates, based on the origin()
     *               and zoomLevelY().
     */
    double transformDocToViewY (double docY) const;

    /**
     * @param docPoint Position in document coordinates.
     *
     * @returns docPoint transformed to view coordinates, based on the
     *                   origin(), zoomLevelX(), zoomLevelY().
     */
    TQPoint transformDocToView (const TQPoint &docPoint) const;

    /**
     * @param docRect Rectangle in document coordinates.
     *
     * @return docRect transformed to view coordinates, based on the
     *                 origin(), zoomLevelX() and zoomLevelY().
     *
     * For bounding rectangles, you should use this function instead of
     * transformDocToViewX(), transformDocToViewY() or
     * transformDocToView(const TQPoint &) which act on coordinates only.
     */
    TQRect transformDocToView (const TQRect &docRect) const;


    /**
     * @param viewPoint Position in view coordinates.
     * @param otherView View whose coordinate system the return value will
     *                  be in.
     *
     * @returns viewPoint transformed to the coordinate system of
     *          @param otherView based on this and otherView's origin(),
     *          zoomLevelX() and zoomLevelY().  This has less rounding
     *          error than otherView->transformDocToView (transformViewToDoc (viewPoint)).
     */
    TQPoint transformViewToOtherView (const TQPoint &viewPoint,
                                     const kpView *otherView);
    
                                        
    /**
     * @returns the approximate view width required to display the entire
     *          document(), based on the zoom level only.
     */
    int zoomedDocWidth () const;

    /**
     * @returns the approximate view height required to display the entire
     *          document(), based on the zoom level only.
     */
    int zoomedDocHeight () const;


protected:
    /**
     * Updates the viewManager() on whether or not the mouse is directly
     * above this view.  Among other things, this ensures the brush cursor
     * is updated.
     *
     * This should be called in event handlers.
     *
     * @param yes Whether the mouse is directly above this view.
     */
    void setHasMouse (bool yes = true);


public:
    /**
     * Adds a region (in view coordinates) to the dirty area that is
     * repainted when the parent @ref kpViewManager is set not to queue
     * updates.
     *
     * @param region Region (in view coordinates) that needs repainting.
     */
    void addToQueuedArea (const TQRegion &region);

    /**
     * Convenience function.  Same as above.
     *
     * Adds a rectangle (in view coordinates) to the dirty area that is
     * repainted when the parent @ref kpViewManager is set not to queue
     * updates.
     *
     * @param rect Rectangle (in view coordinates) that needs repainting.
     */
     void addToQueuedArea (const TQRect &rect);

    /**
     * Removes the dirty region that has been queued for updating.
     * Does not update the view.
     */
    void invalidateQueuedArea ();

    /**
     * Updates the part of the view described by dirty region and then
     * calls invalidateQueuedArea().  Does nothing if @ref kpViewManager
     * is set to queue updates.
     */
    void updateQueuedArea ();
    
    void updateMicroFocusHint (const TQRect &microFocusHint);


public slots:
    /**
     * Call this when the "environment" (e.g. document size) changes.  The
     * environment is defined by the caller and should be based on the type
     * of view.  For instance, an unzoomed thumbnail view would also
     * include in its environment the contents position of an associated
     * scrollable container.
     *
     * This is never called by the kpView base class.
     *
     * Implementors should change whatever state is neccessary for their
     * type of view.  For instance, an unzoomed view would resize itself;
     * a zoomed thumbnail would change the zoom level.
     */
    virtual void adjustToEnvironment () = 0;


public:
    TQRect selectionViewRect () const;

    // (if <viewPoint> is KP_INVALID_POINT, it uses TQCursor::pos())

    TQPoint mouseViewPoint (const TQPoint &returnViewPoint = KP_INVALID_POINT) const;
    TQPoint mouseViewPointRelativeToSelection (const TQPoint &viewPoint = KP_INVALID_POINT) const;
    bool mouseOnSelection (const TQPoint &viewPoint = KP_INVALID_POINT) const;

    int textSelectionMoveBorderAtomicSize () const;
    bool mouseOnSelectionToMove (const TQPoint &viewPoint = KP_INVALID_POINT) const;

protected:
    bool selectionLargeEnoughToHaveResizeHandlesIfAtomicSize (int atomicSize) const;
public:
    int selectionResizeHandleAtomicSize () const;
    bool selectionLargeEnoughToHaveResizeHandles () const;

    TQRegion selectionResizeHandlesViewRegion (bool forRenderer = false) const;

    enum SelectionResizeType
    {
        None = 0,
        Left = 1,
        Right = 2,
        Top = 4,
        Bottom = 8
    };

    // Returns a bitwise OR of the SelectionResizeType's
    int mouseOnSelectionResizeHandle (const TQPoint &viewPoint = KP_INVALID_POINT) const;

    bool mouseOnSelectionToSelectText (const TQPoint &viewPoint = KP_INVALID_POINT) const;


signals:
    /**
     * Emitted after all zooming code has been executed.
     *
     * @param zoomLevelX New zoomLevelX()
     * @param zoomLevelY New zoomLevelY()
     */
    void zoomLevelChanged (int zoomLevelX, int zoomLevelY);

    /**
     * Emitted after all resizing code has been executed.
     *
     * @param size New view size.
     */
    void sizeChanged (const TQSize &size);

    /**
     * Convenience signal - same as above.
     *
     * Emitted after all resizing code has been executed.
     *
     * @param width New view width.
     * @param height New view height.
     */
    void sizeChanged (int width, int height);

    /**
     * Emitted after all origin changing code has been executed.
     *
     * @param origin The new origin.
     */
    void originChanged (const TQPoint &origin);

protected:
    virtual void mouseMoveEvent (TQMouseEvent *e);
    virtual void mousePressEvent (TQMouseEvent *e);
    virtual void mouseReleaseEvent (TQMouseEvent *e);
public:
    // (needs to be public as it may also get event from
    //  TQScrollView::contentsWheelEvent())
    virtual void wheelEvent (TQWheelEvent *e);

protected:
    virtual void keyPressEvent (TQKeyEvent *e);
    virtual void keyReleaseEvent (TQKeyEvent *e);

    virtual void focusInEvent (TQFocusEvent *e);
    virtual void focusOutEvent (TQFocusEvent *e);

    virtual void enterEvent (TQEvent *e);
    virtual void leaveEvent (TQEvent *e);

    virtual void dragEnterEvent (TQDragEnterEvent *);
    virtual void dragLeaveEvent (TQDragLeaveEvent *);
    
    virtual void imStartEvent (TQIMEvent *e);
    virtual void imComposeEvent (TQIMEvent *e);
    virtual void imEndEvent (TQIMEvent *e);

public:
    virtual void resize (int w, int h);
protected:
    virtual void resizeEvent (TQResizeEvent *e);


protected:
    TQRect paintEventGetDocRect (const TQRect &viewRect) const;
public:
    /**
     * Draws an opaque background representing transparency.  Currently,
     * it draws a checkerboard.
     *
     * @param painter Painter.
     * @param viewWidth Total width of the view visible to the user.
     * @param viewHeight Total height of the view visible to the user.
     * @param rect Rectangle to paint in relative to the painter.  Note
     *             that this function does not clip and may draw slightly
     *             more than the requested rectangle.  TODO: why not?
     * @param isPreview Whether the view is for a preview as opposed to
     *                  e.g. editing.  If set, this function may render
     *                  slightly differently.
     */
    static void drawTransparentBackground (TQPainter *painter,
                                           int viewWidth, int viewHeight,
                                           const TQRect &rect,
                                           bool isPreview = false);
protected:
    void paintEventDrawCheckerBoard (TQPainter *painter, const TQRect &viewRect);
    void paintEventDrawSelection (TQPixmap *destPixmap, const TQRect &docRect);
    bool selectionResizeHandleAtomicSizeCloseToZoomLevel () const;
    void paintEventDrawSelectionResizeHandles (TQPainter *painter, const TQRect &viewRect);
    void paintEventDrawTempPixmap (TQPixmap *destPixmap, const TQRect &docRect);
    void paintEventDrawGridLines (TQPainter *painter, const TQRect &viewRect);

    void paintEventDrawRect (const TQRect &viewRect);
    virtual void paintEvent (TQPaintEvent *e);


private:
    struct kpViewPrivate *d;
};


#endif  // KP_VIEW_H
