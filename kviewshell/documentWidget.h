//
// Class: documentWidet
//
// Widget for displaying TeX DVI files.
// Part of KDVI- A previewer for TeX DVI files.
//
// (C) 2004 Stefan Kebekus.
// Copyright (C) 2004-2005 Wilfried Huss <Wilfried.Huss@gmx.at>
//
// Distributed under the GPL.

#ifndef _documentwidget_h_
#define _documentwidget_h_

#include "selection.h"

#include <tqregion.h>
#include <tqtimer.h>
#include <tqwidget.h>

class DocumentPageCache;
class PageView;
class TQMouseEvent;
class TQPaintEvent;


/* DocumentWidget */

class DocumentWidget : public TQWidget
{
  Q_OBJECT
  

public:
  DocumentWidget(TQWidget *parent, PageView *sv, DocumentPageCache *cache, const char *name);

  void          setPageNumber(TQ_UINT16 pageNr);
  TQ_UINT16      getPageNumber() const {return pageNr;}

  /** Returns the size of the widget without the page shadow. */
  TQSize pageSize() const;
  /** Returns the bounding rectangle of the widget without the page shadow. */
  TQRect pageRect() const;

  /** Draw a red vertical line at y-coordinate ycoord. The line is removed again 
      after one second. This is used to make it easier to regain reading focus if
      the whole page is scrolled up or down. */
  void drawScrollGuide(int ycoord);

  /** Checks if the page is currently visible in the PageView. */
  bool isVisible();

public slots:
  void          slotEnableMoveTool(bool enable);

  void          select(const TextSelection&);
  void          selectAll();
  void          flash(int);

  /** Sets the size of the widget so that the page is of the given size.
      The widget gets slightly bigger because of the page shadow. */
  void setPageSize(const TQSize&);
  void setPageSize(int width, int height);

signals:
  /** Passed through to the top-level kpart. */
  void          setStatusBarText( const TQString& );
  void          localLink( const TQString& );

  /** This signal is emitted when the widget resizes itself */
  void          resized();

  /** This signal is emitted when the selection needs to be cleared. */
  void clearSelection();

protected:
  virtual void  paintEvent (TQPaintEvent *);
  virtual void  mousePressEvent ( TQMouseEvent * e );
  virtual void  mouseReleaseEvent (TQMouseEvent *);

  /** This method is used by the DocumentWidget to find out of the
      mouse pointer hovers over a hyperlink, and to update the
      statusbar accordingly. Scrolling with the left mouse button
      pressed, and the text copy functions are also implemented here. 
      Re-implementations of this method should do the following:
      
      0) Immediately return if pageNr == 0, i.e. if no page number has
         been set

      1) Call the standard implementation using
	  
	  DocumentWidget::mouseMoveEvent(e);

      2) Ignore the TQMouseEvent if a mouse button is pressed

      3) If no mouse button is pressed, analyze the mouse movement and
         take appropriate actions. To set statusbar text, do
	 
	 clearStatusBarTimer.stop();
	 emit setStatusBarText( i18n("Whatever string") );

	 To clear the statusbar, use the following code

	 if (!clearStatusBarTimer.isActive())
	   clearStatusBarTimer.start(200, true);

	 This clears the statusbar after 200 msec and avoids awful
	 flickering when the mouse is swiftly moved across various
	 areas in the widget.

  */
  virtual void  mouseMoveEvent (TQMouseEvent *);

protected:
  void updateSelection(const TextSelection& newTextSelection);

  /** Methods and counters used for the animation to mark the target of
      an hyperlink. */
  int           timerIdent;
  void          timerEvent( TQTimerEvent *e );
  int           animationCounter;
  int           flashOffset;

  TQ_UINT16      pageNr;

  /* This timer is used to delay clearing of the statusbar. Clearing
     the statusbar is delayed to avoid awful flickering when the mouse
     moves over a block of text that contains source hyperlinks. The
     signal timeout() is connected to the method clearStatusBar() of
     *this. */
  TQTimer        clearStatusBarTimer;

  /* Data structures used for marking text with the mouse */
  TQPoint       firstSelectedPoint;
  TQRect        selectedRectangle;

  /** Pointer to the PageView that contains this
      widget. This pointer is used in the re-implementation of the
      paintEvent() method ---see the explanation there. */
  PageView *scrollView;
  DocumentPageCache *documentCache;

  /** Currently selected Region */
  TQRegion selectedRegion;

  /** This is set to the index of the link over which the mouse pointer currently resides,
      and -1 if the no link is hovered.
      Is used when "Underline Links" is set to "Only on Hover". */
  int indexOfUnderlinedLink;

  /** True if there is already a request for this page to the renderer. */
  bool pixmapRequested;

  /** Sets the cursor to an arrow if the move tool is selected, and to the text selection
      cursor if the selection tool is active. */
  virtual void setStandardCursor();

private slots:
  /** This slot emits the signal setStatusBarText(TQString()) to
      clear the status bar. It is connected to the timeout slot of the
      clearStatusBarTimer. */
  void clearStatusBar();

  void delayedRequestPage();

  /** Hide the scroll guide. This slot is called one second after drawScrollGuide(). */
  void clearScrollGuide();

private:
  TQRect linkFlashRect();

  /** If this variable is positive draw a vertical line at this y-coordinate. */
  int scrollGuide;

  /** Color used by in the shadow drawing to check if the background color has been changed. */
  static TQColor backgroundColorForCorners;

  /** The following tables store grey values for roundish shadow
      corners. They were shamelessly stolen from tdelibs/tdefx/tdestyle.cpp. */
  static const int bottom_right_corner[16];
  static const int bottom_left_corner[16];
  static const int shadow_strip[4];

  bool moveTool;

  /** If this is true the zoomlevel has changed and we need to update the
      selected region. */
  bool selectionNeedsUpdating;
};



#endif
