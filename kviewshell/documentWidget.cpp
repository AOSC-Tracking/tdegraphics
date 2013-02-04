//
// Class: DocumentWidget
//
// Widget for displaying TeX DVI files.
// Part of KDVI- A previewer for TeX DVI files.
//
// (C) 2001 Stefan Kebekus
// Copyright (C) 2004-2005 Wilfried Huss <Wilfried.Huss@gmx.at>
// Distributed under the GPL
//

#include <config.h>

#include <tdeaction.h>
#include <kapplication.h>
#include <kdebug.h>
#include <kglobalsettings.h>
#include <kiconloader.h>
#include <klocale.h>
#include <tqclipboard.h>
#include <tqcursor.h>
#include <tqimage.h>
#include <tqpainter.h>
#include <tqpixmap.h>

#include "documentWidget.h"
#include "pageView.h"
#include "documentPageCache.h"
#include "hyperlink.h"
#include "renderedDocumentPagePixmap.h"
#include "textBox.h"

#include "kvsprefs.h"

//#define DEBUG_DOCUMENTWIDGET

const int DocumentWidget::bottom_right_corner[16] =
  { 61, 71, 85, 95,
    71, 78, 89, 96,
    85, 89, 95, 98,
    95, 97, 98, 99 };

const int DocumentWidget::bottom_left_corner[16] =
  { 95, 85, 71, 61,
    97, 89, 78, 71,
    98, 95, 89, 85,
    99, 98, 96, 95 };

const int DocumentWidget::shadow_strip[4] =
  { 56, 67, 83, 94 };

TQColor DocumentWidget::backgroundColorForCorners;

namespace {

/** Holds the icon used as a overlay on pages which are not drawn yet. */
TQPixmap* busyIcon = 0;

/** Internal storages used in the shadow drawing routines in the
    drawContents method.*/
TQPixmap* URShadow = 0;
TQPixmap* BRShadow = 0;
TQPixmap* BLShadow = 0;

} // namespace anon


DocumentWidget::DocumentWidget(TQWidget *parent, PageView *sv, DocumentPageCache *cache, const char *name )
  : TQWidget( parent, name ), indexOfUnderlinedLink(-1)
{
  moveTool = true;

  selectionNeedsUpdating = false;

  // Variables used in animation.
  animationCounter = 0;
  timerIdent       = 0;
  documentCache    = cache;
  scrollView       = sv;

  pixmapRequested = false;

  scrollGuide = -1;

  setMouseTracking(true);
  setFocusPolicy(TQ_ClickFocus);

  connect(&clearStatusBarTimer, TQT_SIGNAL(timeout()), this, TQT_SLOT(clearStatusBar()));
  setBackgroundMode(TQt::NoBackground);

  if (!busyIcon)
  {
    busyIcon = new TQPixmap(TDEGlobal::iconLoader()->loadIcon("gear", TDEIcon::NoGroup, TDEIcon::SizeMedium));

    URShadow = new TQPixmap();
    BRShadow = new TQPixmap();
    BLShadow = new TQPixmap();

    URShadow->resize(4,4);
    BRShadow->resize(4,4);
    BLShadow->resize(4,4);
  }
}


void DocumentWidget::setPageNumber(TQ_UINT16 nr)
{
  pageNr = nr;

  selectionNeedsUpdating = true;

  // We have to reset this, because otherwise we might crash in the mouseMoveEvent
  // After switching pages in SinglePageMode or OverviewMode.
  indexOfUnderlinedLink = -1;

  // Resize Widget if the size of the new page is different than the size of the old page.
  TQSize _pageSize = documentCache->sizeOfPageInPixel(pageNr);
  if (_pageSize != pageSize())
  {
    setPageSize(_pageSize);
  }
  update();
}

TQRect DocumentWidget::linkFlashRect()
{
  int width = pageSize().width()/(11 - animationCounter);
  int height = pageSize().height()/(60 - 4 * animationCounter);
  return TQRect((pageSize().width()-width)/2, flashOffset - height/2, width, height);
}

void DocumentWidget::timerEvent( TQTimerEvent *e )
{
  if (animationCounter == 0) {
    killTimer(e->timerId());
    timerIdent = startTimer(50); // Proceed with the animation in 1/10s intervals
  }

  animationCounter++;

  TQRect flashRect = linkFlashRect();
  flashRect.addCoords(-1, -1, 1, 1);

  if (animationCounter >= 10) {
    killTimer(e->timerId());
    timerIdent       = 0;
    animationCounter = 0;
  }

  repaint(flashRect, false);
}


void DocumentWidget::flash(int fo)
{
  if (timerIdent != 0)
  {
    killTimer(timerIdent);
    // Delete old flash rectangle
    animationCounter = 10;
    TQRect flashRect = linkFlashRect();
    flashRect.addCoords(-1, -1, 1, 1);
    repaint(flashRect, false);
  }
  animationCounter = 0;
  flashOffset      = fo;
  timerIdent       = startTimer(50); // Start the animation. The animation proceeds in 1/10s intervals
}


void DocumentWidget::paintEvent(TQPaintEvent *e)
{
#ifdef DEBUG_DOCUMENTWIDGET
  kdDebug(1223) << "DocumentWidget::paintEvent() called" << endl;
#endif

  // Check if this widget is really visible to the user. If not, there
  // is nothing to do. Remark: if we don't do this, then under QT
  // 3.2.3 the following happens: when the user changes the zoom
  // value, all those widgets are updated which the user has EVER
  // seen, not just those that are visible at the moment. If the
  // document contains several thousand pages, it is easily possible
  // that this means that a few hundred of these are re-painted (which
  // takes substantial time) although perhaps only three widgets are
  // visible and *should* be updated. I believe this is some error in
  // QT, but I am not positive about that ---Stefan Kebekus.
  if (!isVisible())
  {
    //kdDebug() << "widget of page " << pageNr << " is not visible. Abort rendering" << endl;
    kapp->processEvents();
    return;
  }

  TQPainter p(this);
  p.setClipRegion(e->region());

  // Paint a black border around the widget
  p.setRasterOp(TQt::CopyROP);
  p.setBrush(NoBrush);
  p.setPen(TQt::black);
  TQRect outlineRect = pageRect();
  outlineRect.addCoords(-1, -1, 1, 1);
  p.drawRect(outlineRect);

  // Paint page shadow
  TQColor backgroundColor = colorGroup().mid();

  // (Re-)generate the Pixmaps for the shadow corners, if necessary
  if (backgroundColor != backgroundColorForCorners)
  {
    backgroundColorForCorners = backgroundColor;
    TQImage tmp(4, 4, 32);
    for(int x=0; x<4; x++)
      for(int y=0; y<4; y++)
        tmp.setPixel(x, y, backgroundColor.light(bottom_right_corner[x+4*y]).rgb() );
    BRShadow->convertFromImage(tmp);

    for(int x=0; x<4; x++)
      for(int y=0; y<4; y++)
        tmp.setPixel(x, y, backgroundColor.light(bottom_left_corner[x+4*y]).rgb() );
    BLShadow->convertFromImage(tmp);

    URShadow->convertFromImage(tmp.mirror(true, true));
  }

  // Draw right and bottom shadows
  for(int i=0; i<4; i++)
  {
    p.setPen(backgroundColor.light(shadow_strip[i]));
    // Right shadow
    p.drawLine(pageSize().width()+i+2, 8, pageSize().width()+i+2, pageSize().height()+2);
    // Bottom shadow
    p.drawLine(8, pageSize().height()+i+2, pageSize().width()+2, pageSize().height()+i+2);
  }
  // Draw shadow corners
  p.drawPixmap(pageSize().width()+2, pageSize().height()+2, *BRShadow);
  p.drawPixmap(4, pageSize().height()+2, *BLShadow);
  p.drawPixmap(pageSize().width()+2, 4, *URShadow);

  // Draw corners
  p.fillRect(0, pageSize().height()+2, 4, 4, backgroundColor);
  p.fillRect(pageSize().width()+2, 0, 4, 4, backgroundColor);

  if (!documentCache->isPageCached(pageNr, pageSize()))
  {
    TQRect destRect = e->rect().intersect(pageRect());
    if (KVSPrefs::changeColors() && KVSPrefs::renderMode() == KVSPrefs::EnumRenderMode::Paper)
      p.fillRect(destRect, KVSPrefs::paperColor());
    else
      p.fillRect(destRect, TQt::white);

    // Draw busy indicator.
    // Im not really sure if this is a good idea.
    // While it is nice to see an indication that something is happening for pages which
    // take long to redraw, it gets quite annoing for fast redraws.
    // TODO: Disable or find something less distractiong.
    p.drawPixmap(10, 10, *busyIcon);

    if (!pixmapRequested)
    {
      // Request page pixmap.
      pixmapRequested = true;
      TQTimer::singleShot(50, this, TQT_SLOT(delayedRequestPage()));
    }
    return;
  }

  RenderedDocumentPagePixmap *pageData = documentCache->getPage(pageNr);
  if (pageData == 0) {
#ifdef DEBUG_DOCUMENTWIDGET
    kdDebug(1223) << "DocumentWidget::paintEvent: no documentPage generated" << endl;
#endif
    return;
  }

  TQMemArray<TQRect> damagedRects = TQRegion(e->region()).rects();
  for (unsigned int i = 0; i < damagedRects.count(); i++)
  {
    // Paint the page where it intersects with the damaged area.
    TQRect destRect = damagedRects[i].intersect(pageRect());

    // The actual page starts at point (1,1) because of the outline.
    // Therefore we need to shift the destination rectangle.
    TQRect pixmapRect = destRect;
    pixmapRect.moveBy(-1,-1);

    if (KVSPrefs::changeColors() && KVSPrefs::renderMode() != KVSPrefs::EnumRenderMode::Paper)
    {
      // Paint widget contents with accessibility changes.
      TQPixmap pdap = pageData->accessiblePixmap();
      bitBlt ( this, destRect.topLeft(), &pdap, pixmapRect, CopyROP);
    }
    else
    {
      // Paint widget contents
      bitBlt ( this, destRect.topLeft(), pageData, pixmapRect, CopyROP);
    }
  }
  // Underline hyperlinks
  if (KVSPrefs::underlineLinks() == KVSPrefs::EnumUnderlineLinks::Enabled ||
      KVSPrefs::underlineLinks() == KVSPrefs::EnumUnderlineLinks::OnlyOnHover)
  {
    int h = 2; // Height of line.
    for(int i = 0; i < (int)pageData->hyperLinkList.size(); i++) 
    {
      if (KVSPrefs::underlineLinks() == KVSPrefs::EnumUnderlineLinks::OnlyOnHover && 
          i != indexOfUnderlinedLink)
        continue;
      int x = pageData->hyperLinkList[i].box.left();
      int w = pageData->hyperLinkList[i].box.width();
      int y = pageData->hyperLinkList[i].baseline;

      TQRect hyperLinkRect(x, y, w, h);
      if (hyperLinkRect.intersects(e->rect()))
      {
#ifdef DEBUG_DOCUMENTWIDGET
        kdDebug(1223) << "Underline hyperlink \"" << pageData->hyperLinkList[i].linkText << "\"" << endl;
#endif
        p.fillRect(x, y, w, h, TDEGlobalSettings::linkColor());
      }
    }
  }

  // Paint flashing frame, if appropriate
  if (animationCounter > 0 && animationCounter < 10)
  {
    int gbChannel = 255 - (9-animationCounter)*28;
    p.setPen(TQPen(TQColor(255, gbChannel, gbChannel), 3));
    p.drawRect(linkFlashRect());
  }

  // Mark selected text.
  TextSelection selection = documentCache->selectedText();
  if ((selection.getPageNumber() != 0) && (selection.getPageNumber() == pageNr))
  {
    if (selectionNeedsUpdating)
    {
      //The zoom value has changed, therefore we need to recalculate
      //the selected region.
      selectedRegion = pageData->selectedRegion(selection);
      selectionNeedsUpdating = false;
    }

    p.setPen(NoPen);
    p.setBrush(white);
    p.setRasterOp(TQt::XorROP);

    TQMemArray<TQRect> selectionRects = selectedRegion.rects();

    for (unsigned int i = 0; i < selectionRects.count(); i++)
      p.drawRect(selectionRects[i]);
  }

  // Draw scroll Guide
  if (scrollGuide >= 0)
  {
    // Don't draw over the page shadow
    p.setClipRegion(e->region().intersect(pageRect()));
    p.setRasterOp(TQt::CopyROP);
    p.setPen(TQt::red);
    p.drawLine(1, scrollGuide, pageSize().width(), scrollGuide);
  }
}

void DocumentWidget::drawScrollGuide(int ycoord)
{
  //kdDebug() << "draw scroll guide for page " << pageNr << " at y = " << ycoord << endl;
  scrollGuide = ycoord;
  update(TQRect(1, scrollGuide, pageSize().width(), 1));
  TQTimer::singleShot(1000, this, TQT_SLOT(clearScrollGuide()));
}

void DocumentWidget::clearScrollGuide()
{
  //kdDebug() << "clear scroll guide for page " << pageNr << " at y = " << scrollGuide << endl;
  int temp = scrollGuide;
  scrollGuide = -1;
  update(TQRect(1, temp, pageSize().width(), 1));
}

void DocumentWidget::select(const TextSelection& newSelection)
{
  // Get a pointer to the page contents
  RenderedDocumentPage *pageData = documentCache->getPage(pageNr);
  if (pageData == 0) {
    kdDebug(1223) << "DocumentWidget::select() pageData for page #" << pageNr << " is empty" << endl;
    return;
  }

  documentCache->selectText(newSelection);

  selectedRegion = pageData->selectedRegion(documentCache->selectedText());
  selectionNeedsUpdating = false;

  update();
}

void DocumentWidget::selectAll()
{
  // pageNr == 0 indicated an invalid page (e.g. page number not yet
  // set)
  if (pageNr == 0)
    return;

  // Get a pointer to the page contents
  RenderedDocumentPage *pageData = documentCache->getPage(pageNr);
  if (pageData == 0) {
    kdDebug(1223) << "DocumentWidget::selectAll() pageData for page #" << pageNr << " is empty" << endl;
    return;
  }

  TextSelection selection;
  // mark everything as selected
  TQString selectedText("");
  for(unsigned int i = 0; i < pageData->textBoxList.size(); i++) {
    selectedText += pageData->textBoxList[i].text;
    selectedText += "\n";
  }
  selection.set(pageNr, 0, pageData->textBoxList.size()-1, selectedText);

  selectedRegion = pageData->selectedRegion(selection);

  documentCache->selectText(selection);

  // Re-paint
  update();
}


void DocumentWidget::mousePressEvent ( TQMouseEvent * e )
{
#ifdef DEBUG_DOCUMENTWIDGET
  kdDebug(1223) << "DocumentWidget::mousePressEvent(...) called" << endl;
#endif
  
  // Make sure the event is passed on to the higher-level widget;
  // otherwise QT gets the coordinated in the mouse move events wrong
  e->ignore();

  // pageNr == 0 indicated an invalid page (e.g. page number not yet
  // set)
  if (pageNr == 0)
    return;

  // Get a pointer to the page contents
  RenderedDocumentPage *pageData = documentCache->getPage(pageNr);
  if (pageData == 0) {
    kdDebug(1223) << "DocumentWidget::selectAll() pageData for page #" << pageNr << " is empty" << endl;
    return;
  }

  // Check if the mouse is pressed on a regular hyperlink
  if (e->button() == Qt::LeftButton) {
    if (pageData->hyperLinkList.size() > 0)
      for(unsigned int i = 0; i < pageData->hyperLinkList.size(); i++) {
        if (pageData->hyperLinkList[i].box.contains(e->pos())) {
          emit(localLink(pageData->hyperLinkList[i].linkText));
          return;
        }
      }
    if (moveTool)
      setCursor(TQt::SizeAllCursor);
    else
      setCursor(TQt::IbeamCursor);
  }

  if (e->button() == Qt::RightButton || (!moveTool && e->button() == Qt::LeftButton))
  {
    setCursor(TQt::IbeamCursor);
    // If Shift is not pressed clear the current selection,
    // otherwise modify the existing selection.
    if (!(e->state() & ShiftButton))
    {
      firstSelectedPoint = e->pos();
      selectedRectangle = TQRect();
      selectedRegion = TQRegion();
      emit clearSelection();
    }
  }
}


void DocumentWidget::mouseReleaseEvent ( TQMouseEvent *e )
{
  // Make sure the event is passed on to the higher-level widget;
  // otherwise the mouse cursor in the centeringScrollview is wrong
  e->ignore();

  if (e->button() == Qt::RightButton || (!moveTool && e->button() == Qt::LeftButton))
  {
    // If the selectedRectangle is empty then there was only a single right click.
    if (firstSelectedPoint == e->pos())
    {
      if (pageNr == 0)
        return;

      // Get a pointer to the page contents
      RenderedDocumentPage* pageData = documentCache->getPage(pageNr);
      if (pageData == 0) 
      {
        kdDebug(1223) << "DocumentWidget::mouseReleaseEvent() pageData for page #" << pageNr << " is empty" << endl;
        return;
      }

      TextSelection newTextSelection = pageData->select(firstSelectedPoint);
      updateSelection(newTextSelection);
    }
  }

  //Reset the cursor to the usual arrow if we use the move tool, and
  // The textselection cursor if we use the selection tool.
  setStandardCursor();
}


void DocumentWidget::mouseMoveEvent ( TQMouseEvent * e )
{
#ifdef DEBUG_DOCUMENTWIDGET
  kdDebug(1223) << "DocumentWidget::mouseMoveEvent(...) called" << endl;
#endif


  // pageNr == 0 indicated an invalid page (e.g. page number not yet
  // set)
  if (pageNr == 0)
    return;

  // Get a pointer to the page contents
  RenderedDocumentPage *pageData = documentCache->getPage(pageNr);
  if (pageData == 0) {
    kdDebug(1223) << "DocumentWidget::selectAll() pageData for page #" << pageNr << " is empty" << endl;
    return;
  }

  // If no mouse button pressed
  if (e->state() == 0) {
    // Remember the index of the underlined link.
    int lastUnderlinedLink = indexOfUnderlinedLink;
    // go through hyperlinks
    for(unsigned int i = 0; i < pageData->hyperLinkList.size(); i++) {
      if (pageData->hyperLinkList[i].box.contains(e->pos())) {
        clearStatusBarTimer.stop();
        setCursor(pointingHandCursor);
        TQString link = pageData->hyperLinkList[i].linkText;
        if ( link.startsWith("#") )
          link = link.remove(0,1);

        emit setStatusBarText( i18n("Link to %1").arg(link) );

        indexOfUnderlinedLink = i;
        if (KVSPrefs::underlineLinks() == KVSPrefs::EnumUnderlineLinks::OnlyOnHover &&
            indexOfUnderlinedLink != lastUnderlinedLink)
        {
          TQRect newUnderline = pageData->hyperLinkList[i].box;
          // Increase Rectangle so that the whole line really lines in it.
          newUnderline.addCoords(0, 0, 0, 2);
          // Redraw widget
          update(newUnderline);

          if (lastUnderlinedLink != -1 && lastUnderlinedLink < pageData->hyperLinkList.size())
          {
            // Erase old underline
            TQRect oldUnderline = pageData->hyperLinkList[lastUnderlinedLink].box;
            oldUnderline.addCoords(0, 0, 0, 2);
            update(oldUnderline);
          }
        }
        return;
      }
    }
    // Whenever we reach this the mouse hovers no link.
    indexOfUnderlinedLink = -1;
    if (KVSPrefs::underlineLinks() == KVSPrefs::EnumUnderlineLinks::OnlyOnHover &&
        lastUnderlinedLink != -1 && lastUnderlinedLink < pageData->hyperLinkList.size())
    {
      // Erase old underline
      TQRect oldUnderline = pageData->hyperLinkList[lastUnderlinedLink].box;
      // Increase Rectangle so that the whole line really lines in it.
      oldUnderline.addCoords(0, 0, 0, 2);
      // Redraw widget
      update(oldUnderline);
    }
    // Cursor not over hyperlink? Then let the cursor be the usual arrow if we use the move tool, and
    // The textselection cursor if we use the selection tool.
    setStandardCursor();
  }

  if (!clearStatusBarTimer.isActive())
    clearStatusBarTimer.start(200, true); // clear the statusbar after 200 msec.

  // Left mouse button pressed -> Text scroll function
  if ((e->state() & Qt::LeftButton) != 0 && moveTool)
  {
    // Pass the mouse event on to the owner of this widget ---under
    // normal circumstances that is the centeringScrollView which will
    // then scroll the scrollview contents
    e->ignore();
  }

  // Right mouse button pressed -> Text copy function
  if ((e->state() & Qt::RightButton) != 0 || (!moveTool && (e->state() & Qt::LeftButton != 0)))
  {
    if (selectedRectangle.isEmpty()) {
      firstSelectedPoint = e->pos();
      selectedRectangle.setRect(e->pos().x(),e->pos().y(),1,1);
    } else {
      int lx = e->pos().x() < firstSelectedPoint.x() ? e->pos().x() : firstSelectedPoint.x();
      int rx = e->pos().x() > firstSelectedPoint.x() ? e->pos().x() : firstSelectedPoint.x();
      int ty = e->pos().y() < firstSelectedPoint.y() ? e->pos().y() : firstSelectedPoint.y();
      int by = e->pos().y() > firstSelectedPoint.y() ? e->pos().y() : firstSelectedPoint.y();
      selectedRectangle.setCoords(lx,ty,rx,by);
    }

    // Now that we know the rectangle, we have to find out which words
    // intersect it!
    TextSelection newTextSelection = pageData->select(selectedRectangle);

    updateSelection(newTextSelection);
  }
}

void DocumentWidget::updateSelection(const TextSelection& newTextSelection)
{
  if (newTextSelection != documentCache->selectedText())
  {
    if (newTextSelection.isEmpty())
    {
      // Clear selection
      documentCache->deselectText();
      selectedRectangle = TQRect();
      selectedRegion = TQRegion();
      update();
    }
    else
    {
      if (pageNr == 0)
        return;

      // Get a pointer to the page contents
      RenderedDocumentPage* pageData = documentCache->getPage(pageNr);
      if (pageData == 0) 
      {
        kdDebug(1223) << "DocumentWidget::mouseReleaseEvent() pageData for page #" << pageNr << " is empty" << endl;
        return;
      }

      documentCache->selectText(newTextSelection);

      TQRegion newlySelectedRegion = pageData->selectedRegion(documentCache->selectedText());

      // Compute the region that needs to be updated
      TQRegion updateRegion;
      if(!selectedRegion.isEmpty())
      {
        updateRegion = newlySelectedRegion.eor(selectedRegion);
      }
      else
      {
        updateRegion = newlySelectedRegion;
      }

      selectedRegion = newlySelectedRegion;

      TQMemArray<TQRect> rectangles = updateRegion.rects();
      for (unsigned int i = 0; i < rectangles.count(); i++)
      {
        repaint(rectangles[i]);
      }
    }
  }
}


void DocumentWidget::clearStatusBar()
{
  emit setStatusBarText( TQString() );
}


void DocumentWidget::delayedRequestPage()
{
  if (!isVisible())
  {
    // We only want to calculate the page pixmap for widgets that are currently visible.
    // When we are fast scrolling thru the document many paint events are created, that
    // are often not needed anymore at the time the eventloop executes them.

    //kdDebug() << "delayedRequest: widget of page " << pageNr << " is not visible. Abort rendering" << endl;
    pixmapRequested = false;
    kapp->processEvents();
    return;
  }

  documentCache->getPage(pageNr);
  pixmapRequested = false;
  update();

  // If more widgets need updateing at the some time, the next line allows them to be
  // displayed one after another. Widthout it all widgets are updated after all the rendering
  // is completed. This is especially noticable in overview mode. After the change to a seperate
  // rendering thread this will probably not be needed anymore.
  kapp->processEvents();
}

TQSize DocumentWidget::pageSize() const
{
  // Substract size of the shadow.
  return size() - TQSize(6, 6);
}

TQRect DocumentWidget::pageRect() const
{
  TQRect boundingRect = rect();
  // Substract the shadow.
  boundingRect.addCoords(1,1,-5,-5);
  return boundingRect;
}

void DocumentWidget::setPageSize(const TQSize& pageSize)
{
  // When the page size changes this means either the paper size
  // has been changed, or the zoomlevel has been changed.
  // If the zoomlevel changes we need to recalculate the selected
  // region. We do this always, just to be on the safe side.
  selectionNeedsUpdating = true;

  // Add size of the shadow.
  resize(pageSize + TQSize(6,6));
}

void DocumentWidget::setPageSize(int width, int height)
{
  setPageSize(TQSize(width, height));
}


void DocumentWidget::slotEnableMoveTool(bool enable)
{
  moveTool = enable;

  setStandardCursor();
}


void DocumentWidget::setStandardCursor()
{
  if (moveTool)
  {
    setCursor(TQt::arrowCursor);
  }
  else
  {
    setCursor(TQt::IbeamCursor);
  }
}


bool DocumentWidget::isVisible()
{
  TQRect visibleRect(scrollView->contentsX(), scrollView->contentsY(), scrollView->visibleWidth(), scrollView->visibleHeight());
  TQRect widgetRect(scrollView->childX(this), scrollView->childY(this), width(), height());
  return widgetRect.intersects(visibleRect);
}

#include "documentWidget.moc"
