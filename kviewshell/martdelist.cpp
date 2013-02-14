/* This file is part of the KDE project
   Copyright (C) 2004 Wilfried Huss <Wilfried.Huss@gmx.at>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include <config.h>

#include <tqcheckbox.h>
#include <tqimage.h>
#include <tqlabel.h>
#include <tqlayout.h>
#include <tqtooltip.h>
#include <tqlabel.h>
#include <tqwhatsthis.h>
#include <tqpainter.h>
#include <tqtimer.h>

#include <tdeapplication.h>
#include <kglobalsettings.h>
#include <klocale.h>
#include <tdepopupmenu.h>
#include <kiconloader.h>
#include <kdebug.h>

#include "documentPageCache.h"
#include "kvsprefs.h"
#include "martdelist.h"


#include "martdelist.moc"


namespace {

/** Holds the icon used as a overlay on pages which are not drawn yet. */
TQPixmap* waitIcon = 0;

} // namespace anon


/****** ThumbnailWidget ******/

ThumbnailWidget::ThumbnailWidget(MarkListWidget* _parent, const PageNumber& _pageNumber, DocumentPageCache* _pageCache)
  : TQWidget(_parent), pageNumber(_pageNumber), pageCache(_pageCache), parent(_parent)
{
  setBackgroundMode(TQt::NoBackground);

  needsUpdating = true;

  if (!waitIcon)
  {
    waitIcon = new TQPixmap(TDEGlobal::iconLoader()->loadIcon("gear", TDEIcon::NoGroup, TDEIcon::SizeMedium));
  }
}

void ThumbnailWidget::paintEvent(TQPaintEvent* e)
{
  // Only repaint if the widget is really visible. We need to check this because TQt
  // sends paintEvents to all widgets that have ever been visible in the Scrollview
  // whenever the ScrollView is resized. This also increases the percieved performance
  // only thumbnails that are really needed are rendered.
  if (!parent->isVisible())
  {
    //kdDebug() << "Abort Thumbnail drawing for page " << pageNumber << endl;
    return;
  }

  TQPainter p(this);
  p.setClipRect(e->rect());

  // Paint a black border around the widget
  p.setRasterOp(TQt::CopyROP);
  p.setBrush(NoBrush);
  p.setPen(TQt::black);
  p.drawRect(rect());

  // Remove 1 pixel from all sides of the rectangle, to eliminate overdraw with
  // the black border.
  TQRect thumbRect = rect();
  thumbRect.addCoords(1,1,-1,-1);

  // If the thumbnail is empty or has been marked for updating generate a new thumbnail.
  if (thumbnail.isNull() || needsUpdating)
  {
    if (KVSPrefs::changeColors() && KVSPrefs::renderMode() == KVSPrefs::EnumRenderMode::Paper)
      p.fillRect(thumbRect, KVSPrefs::paperColor());
    else
      p.fillRect(thumbRect, TQt::white);

    // Draw busy indicator.
    // Im not really sure if this is a good idea.
    // While it is nice to see an indication that something is happening for pages which
    // take long to redraw, it gets quite annoing for fast redraws.
    // TODO: Disable or find something less distractiong.
    p.drawPixmap(10, 10, *waitIcon);

    TQTimer::singleShot(50, this, TQT_SLOT(setThumbnail()));
    return;
  }

  // Safety check
  if (thumbnail.isNull())
  {
    kdDebug(1223) << "No Thumbnail for page " << pageNumber << " created." << endl;
    return;
  }


  // The actual page starts at point (1,1) because of the outline.
  // Therefore we need to shift the destination rectangle.
  TQRect pixmapRect = thumbRect;
  pixmapRect.moveBy(-1,-1);

  // Paint widget
  bitBlt (this, thumbRect.topLeft(), &thumbnail, pixmapRect, CopyROP);
}

void ThumbnailWidget::resizeEvent(TQResizeEvent*)
{
  thumbnail.resize(width(), height());
  // Generate a new thumbnail in the next paintEvent.
  needsUpdating = true;
}

void ThumbnailWidget::setThumbnail()
{
  if (!parent->isVisible())
  {
    // We only want to calculate the thumbnail for widgets that are currently visible.
    // When we are fast scrolling thru the document. Many paint events are created, that
    // are often not needed anymore at the time the eventloop executes them.
    //kdDebug() << "Delayed request Abort Thumbnail drawing for page " << pageNumber << endl;
    kapp->processEvents();
    return;
  }

  needsUpdating = false;

  // Draw Thumbnail
  thumbnail = pageCache->createThumbnail(pageNumber, width() - 2);

  if (thumbnail.height() != height() + 2)
    setFixedHeight(thumbnail.height() + 2);

  update();
  kapp->processEvents();
}


/****** MarkListWidget ******/


MarkListWidget::MarkListWidget(TQWidget* _parent, MarkList* _markList, const PageNumber& _pageNumber,
                               DocumentPageCache* _pageCache, bool _showThumbnail)
  : TQWidget(_parent), showThumbnail(_showThumbnail), pageNumber(_pageNumber),
    pageCache(_pageCache), markList(_markList)
{
  TQBoxLayout* layout = new TQVBoxLayout(this, margin);

  thumbnailWidget = 0;
  if (showThumbnail)
  {
    thumbnailWidget = new ThumbnailWidget(this, pageNumber, pageCache);
    layout->addWidget(thumbnailWidget, 1, TQt::AlignTop);
  }

  TQBoxLayout* bottomLayout = new TQHBoxLayout(layout);

  checkBox = new TQCheckBox(TQString(), this );
  checkBox->setFocusPolicy(TQ_NoFocus);
  TQToolTip::add(checkBox, i18n("Select for printing"));
  bottomLayout->addWidget(checkBox, 0, TQt::AlignAuto);

  pageLabel = new TQLabel(TQString("%1").arg(pageNumber), this);
  bottomLayout->addWidget(pageLabel, 1);

  _backgroundColor = TDEGlobalSettings::baseColor();

  // Alternate between colors.
  if ((pageNumber % 2 == 0) && TDEGlobalSettings::alternateBackgroundColor().isValid())
    _backgroundColor = TDEGlobalSettings::alternateBackgroundColor();

  setPaletteBackgroundColor( _backgroundColor );

  show();
}

bool MarkListWidget::isChecked() const
{
    return checkBox->isChecked();
}

void MarkListWidget::toggle()
{
    checkBox->toggle();
}

void MarkListWidget::setChecked( bool checked )
{
    checkBox->setChecked(checked);
}

void MarkListWidget::setSelected( bool selected )
{
    if (selected)
      setPaletteBackgroundColor( TQApplication::palette().active().highlight() );
    else
      setPaletteBackgroundColor( _backgroundColor );
}

int MarkListWidget::setNewWidth(int width)
{
  int height = TQMAX(checkBox->height(), pageLabel->height()) + 2*margin;
  if (showThumbnail)
  {
    // Calculate size of Thumbnail
    int thumbnailWidth = TQMIN(width, KVSPrefs::maxThumbnailWidth());
    int thumbnailHeight = (int)((thumbnailWidth - 2*margin - 2) / pageCache->sizeOfPage(pageNumber).aspectRatio() + 0.5) + 2;

    // Resize Thumbnail if necessary
    if (thumbnailWidget->size() != TQSize(thumbnailWidth, thumbnailHeight))
      thumbnailWidget->setFixedSize(thumbnailWidth - 2*margin, thumbnailHeight);

    height += thumbnailHeight + 2*margin;
  }

  setFixedSize(width, height);
  return height;
}

bool MarkListWidget::isVisible()
{
  TQRect visibleRect(markList->contentsX(), markList->contentsY(),
                   markList->visibleWidth(), markList->visibleHeight());
  TQRect widgetRect(markList->childX(this), markList->childY(this), width(), height());

  if (widgetRect.intersects(visibleRect))
    return true;

  return false;
}


void MarkListWidget::mousePressEvent(TQMouseEvent* e)
{
  // Select Page
  if (e->button() == Qt::LeftButton)
  {
    emit selected(pageNumber);
  }
  else if (e->button() == Qt::RightButton)
  {
    emit showPopupMenu(pageNumber, e->globalPos());
  }
}


/****** MarkList ******/


MarkList::MarkList(TQWidget* parent, const char* name)
  : TQScrollView(parent, name), clickedThumbnail(0), showThumbnails(true), contextMenu(0)
{
  currentPage = PageNumber::invalidPage;
  widgetList.setAutoDelete(true);
  setFocusPolicy( TQ_StrongFocus );
  //viewport()->setFocusPolicy( TQ_WheelFocus );
  setResizePolicy(TQScrollView::Manual);

  setVScrollBarMode(TQScrollView::AlwaysOn);
  setHScrollBarMode(TQScrollView::AlwaysOff);

  setSizePolicy(TQSizePolicy::MinimumExpanding, TQSizePolicy::MinimumExpanding);

  viewport()->setBackgroundMode(TQt::PaletteBase);
  enableClipper(true);
}

MarkList::~MarkList()
{
  delete contextMenu;
}

void MarkList::setPageCache(DocumentPageCache* _pageCache)
{
  pageCache = _pageCache;
}

TQValueList<int> MarkList::selectedPages() const
{
  TQValueList<int> list;
  MarkListWidget* item;
  for(unsigned int i = 0; i < widgetList.count(); i++)
  {
    item = widgetList[i];
    if (item->isChecked()) 
        list << (i + 1);
  }
  return list;
}

void MarkList::setNumberOfPages(int numberOfPages, bool _showThumbnails)
{
  showThumbnails = _showThumbnails;

  widgetList.resize(numberOfPages);

  int y = 0;

  for (int page = 1; page <= numberOfPages; page++)
  {
    MarkListWidget* item = new MarkListWidget(viewport(), this, page, pageCache, showThumbnails);

    connect(item, TQT_SIGNAL(selected(const PageNumber&)), this, TQT_SLOT(thumbnailSelected(const PageNumber&)));
    connect(item, TQT_SIGNAL(showPopupMenu(const PageNumber&, const TQPoint&)), this, TQT_SLOT(showPopupMenu(const PageNumber&, const TQPoint&)));

    widgetList.insert(page - 1, item);

    int height = item->setNewWidth(visibleWidth());
    addChild(item, 0, y);

    y += height;
  }
  resizeContents(visibleWidth(), y);
  viewport()->update();
}

void MarkList::thumbnailSelected(const PageNumber& pageNumber)
{
  // This variable is set to remember that the next call to setCurrentPageNumber
  // has been initiated with a left click on the thumbnail of page pageNumber.
  clickedThumbnail = pageNumber;
  emit selected(pageNumber);
}

void MarkList::setCurrentPageNumber(const PageNumber& pageNumber)
{
  if (!pageNumber.isValid() || pageNumber > (int)widgetList.count())
  {
    clickedThumbnail = 0;
    return;
  }

  if (currentPage == pageNumber)
    return;

  MarkListWidget* item;

  // Clear old selection
  if (currentPage.isValid() && currentPage <= (int)widgetList.count())
  {
    item = widgetList[currentPage - 1];
    item->setSelected(false);
  }

  // Draw new selection
  item = widgetList[pageNumber - 1];
  item->setSelected(true);

  // Make selected page visible if the current page has not been set with a mouseclick
  // in the thumbnail list. (We use this because it is a bit confusing if the element that
  // you have just clicked on, is scrolled away under the mouse cursor)
  if (clickedThumbnail != pageNumber)
    ensureVisible(childX(item), childY(item), 0, item->height());

  clickedThumbnail = 0;

  currentPage = pageNumber;
}

void MarkList::clear()
{
  currentPage = PageNumber::invalidPage;
  widgetList.resize(0);
}

void MarkList::selectAll()
{
  MarkListWidget* item;
  for (unsigned int i = 0; i < widgetList.count(); i++)
  {
    item = widgetList[i];
    item->setChecked(true);
  }
}

void MarkList::selectEven()
{
  MarkListWidget* item;
  for (unsigned int i = 1; i < widgetList.count(); i = i + 2)
  {
    item = widgetList[i];
    item->setChecked(true);
  }
}

void MarkList::selectOdd()
{
  MarkListWidget* item;
  for (unsigned int i = 0; i < widgetList.count(); i = i + 2)
  {
    item = widgetList[i];
    item->setChecked(true);
  }
}

void MarkList::toggleSelection()
{
  MarkListWidget* item;
  for (unsigned int i = 0; i < widgetList.count(); i++)
  {
    item = widgetList[i];
    item->toggle();
  }
}

void MarkList::removeSelection()
{
  MarkListWidget* item;
  for (unsigned int i = 0; i < widgetList.count(); i++)
  {
    item = widgetList[i];
    item->setChecked(false);
  }
}

void MarkList::viewportResizeEvent(TQResizeEvent*)
{
  MarkListWidget* item;

  int yold = contentsHeight();
  int y = 0;

  for (unsigned int i = 0; i < widgetList.count(); i++)
  {
    item = widgetList[i];
    int height = item->setNewWidth(visibleWidth());
    moveChild(item, 0, y);

    y += height;
  }
  resizeContents(visibleWidth(), y);

  // If the height of the content has changed
  if (yold != contentsHeight())
  {
    // Make sure the selected item is still visible.
    if (currentPage.isValid() && currentPage <= (int)widgetList.count())
    {
      item = widgetList[currentPage-1];
      ensureVisible(childX(item), childY(item), 0, item->height());
    }
  }

  viewport()->update();
}


void MarkList::updateWidgetSize(const PageNumber& pageNumber)
{
  // safety checks
  if (!pageNumber.isValid() || pageNumber > widgetList.count())
  {
    kdError() << "MarkList::updateWidgetSize called with invalid pageNumber " << pageNumber << endl;
    return;
  }

  MarkListWidget* item;

  // Resize the changed widget
  item = widgetList[pageNumber - 1];
  int height = item->setNewWidth(visibleWidth());
  int y = childY(item) + height;

  // Move the rest of the widgets
  for (unsigned int i = pageNumber; i < widgetList.count(); i++)
  {
    item = widgetList[i];
    int height = item->height();
    moveChild(item, 0, y);

    y += height;
  }
  resizeContents(contentsWidth(), y);

  viewport()->update();
}

void MarkList::mousePressEvent(TQMouseEvent* e)
{
  if (e->button() == Qt::RightButton)
  {
    // We call showPopupMenu with an invalid pageNumber to indicate that
    // the mouse does not point at a thumbnailWidget.
    showPopupMenu(PageNumber::invalidPage, e->globalPos());
  }
}

void MarkList::slotShowThumbnails(bool show)
{
  if (show != showThumbnails)
  {
    int numOfPages = widgetList.count();

    if (numOfPages == 0)
      return;

    // Save current page.
    PageNumber _currentPage = currentPage;

    // Save page selections.
    TQValueVector<bool> selections;
    selections.resize(widgetList.count());
    for (unsigned int i = 0; i < widgetList.count(); i++)
      selections[i] = widgetList[i]->isChecked();

    // Rebuild thumbnail widgets.
    clear();
    setNumberOfPages(numOfPages, show);

    // Restore current page.
    setCurrentPageNumber(_currentPage);

    // Restore page selections
    for (unsigned int i = 0; i < widgetList.count(); i++)
      widgetList[i]->setChecked(selections[i]);
  }
}


void MarkList::repaintThumbnails()
{
  bool show = showThumbnails;
  int numOfPages = widgetList.count();

  // Rebuild thumbnail widgets.
  clear();
  setNumberOfPages(numOfPages, show);
}


void MarkList::showPopupMenu(const PageNumber& pageNumber, const TQPoint& position)
{
  if (contextMenu == 0)
  {
    // Initialize Contextmenu
    contextMenu = new TDEPopupMenu(this, "markListContext");

    contextMenu->insertItem(i18n("Select &Current Page"), 0);
    contextMenu->insertItem(i18n("Select &All Pages"), 1);
    contextMenu->insertItem(i18n("Select &Even Pages"), 2);
    contextMenu->insertItem(i18n("Select &Odd Pages"), 3);
    contextMenu->insertItem(i18n("&Invert Selection"), 4);
    contextMenu->insertItem(i18n("&Deselect All Pages"), 5);
  }

  if (widgetList.count() == 0)
  {
    for (int i = 0; i <= 5; i++)
      contextMenu->setItemEnabled(i, false);
  }
  else
  {
    for (int i = 0; i <= 5; i++)
      contextMenu->setItemEnabled(i, true);
  }

  // Only allow to select the current page if we got a valid pageNumber.
  if (pageNumber.isValid() && pageNumber <= (int)widgetList.count())
    contextMenu->setItemEnabled(0, true);
  else
    contextMenu->setItemEnabled(0, false);

  // Show Contextmenu
  switch(contextMenu->exec(position))
  {
    case 0:
      widgetList[pageNumber - 1]->toggle();
      break;

    case 1:
      selectAll();
      break;

    case 2:
      selectEven();
      break;

    case 3:
      selectOdd();
      break;

    case 4:
      toggleSelection();
      break;

    case 5:
      removeSelection();
      break;
  }
}

