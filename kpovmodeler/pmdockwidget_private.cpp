/* This file is part of the KDE libraries
   Copyright (C) 2000 Max Judin <novaprint@mtu-net.ru>
   Modified 2002 Andreas Zehender <zehender@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#include "pmdockwidget.h"
#include "pmdockwidget_private.h"

#include <tqpainter.h>
#include <tqcursor.h>

PMDockSplitter::PMDockSplitter(TQWidget *parent, const char *name, Qt::Orientation orient, int pos, bool highResolution)
: TQWidget(parent, name)
{
  divider = 0L;
  child0 = 0L;
  child1 = 0L;
  orientation = orient;
  mOpaqueResize = false;
  mKeepSize = false;
  mHighResolution = highResolution;
  setSeparatorPos( pos, false );
  initialised = false;
}

void PMDockSplitter::activate(TQWidget *c0, TQWidget *c1)
{
  if ( c0 ) child0 = c0;
  if ( c1 ) child1 = c1;

  setupMinMaxSize();

  if (divider) delete divider;
  divider = new TQFrame(this, "pannerdivider");
  divider->setFrameStyle(TQFrame::Panel | TQFrame::Raised);
  divider->setLineWidth(1);
  divider->raise();

  if (orientation ==Qt::Horizontal)
    divider->setCursor(TQCursor(sizeVerCursor));
  else
    divider->setCursor(TQCursor(sizeHorCursor));

  divider->installEventFilter(this);

  initialised= true;

  updateName();

  divider->show();
  resizeEvent(0);
}

void PMDockSplitter::setupMinMaxSize()
{
  // Set the minimum and maximum sizes
  int minx, maxx, miny, maxy;
  if (orientation ==Qt::Horizontal) {
    miny = child0->minimumSize().height() + child1->minimumSize().height()+4;
    maxy = child0->maximumSize().height() + child1->maximumSize().height()+4;
    minx = (child0->minimumSize().width() > child1->minimumSize().width()) ? child0->minimumSize().width() : child1->minimumSize().width();
    maxx = (child0->maximumSize().width() > child1->maximumSize().width()) ? child0->maximumSize().width() : child1->maximumSize().width();

    miny = (miny > 4) ? miny : 4;
    maxy = (maxy < 32000) ? maxy : 32000;
    minx = (minx > 2) ? minx : 2;
    maxx = (maxx < 32000) ? maxx : 32000;
  } else {
    minx = child0->minimumSize().width() + child1->minimumSize().width()+4;
    maxx = child0->maximumSize().width() + child1->maximumSize().width()+4;
    miny = (child0->minimumSize().height() > child1->minimumSize().height()) ? child0->minimumSize().height() : child1->minimumSize().height();
    maxy = (child0->maximumSize().height() > child1->maximumSize().height()) ? child0->maximumSize().height() : child1->maximumSize().height();

    minx = (minx > 4) ? minx : 4;
    maxx = (maxx < 32000) ? maxx : 32000;
    miny = (miny > 2) ? miny : 2;
    maxy = (maxy < 32000) ? maxy : 32000;
  }
  setMinimumSize(minx, miny);
  setMaximumSize(maxx, maxy);
}

void PMDockSplitter::deactivate()
{
  if (divider) delete divider;
  divider = 0L;
  initialised= false;
}

void PMDockSplitter::setSeparatorPos(int pos, bool do_resize)
{
  xpos = pos;
  if (do_resize)
    resizeEvent(0);
}

int PMDockSplitter::separatorPos() const
{
  return xpos;
}

void PMDockSplitter::resizeEvent(TQResizeEvent *ev)
{
  if (initialised){
    int factor = (mHighResolution)? 10000:100;
    // real resize event, recalculate xpos
    if (ev && mKeepSize && isVisible()) {
      if (orientation ==Qt::Horizontal) {
        if (ev->oldSize().height() != ev->size().height())
          xpos = factor * checkValue( child0->height()+1 ) / height();
      } else {
        if (ev->oldSize().width() != ev->size().width())
          xpos = factor * checkValue( child0->width()+1 ) / width();
      }
    }
    int position = checkValue( (orientation ==Qt::Vertical ? width() : height()) * xpos/factor );
    if (orientation ==Qt::Horizontal){
      child0->setGeometry(0, 0, width(), position);
      child1->setGeometry(0, position+4, width(), height()-position-4);
      divider->setGeometry(0, position, width(), 4);
    } else {
      child0->setGeometry(0, 0, position, height());
      child1->setGeometry(position+4, 0, width()-position-4, height());
      divider->setGeometry(position, 0, 4, height());
    }
  }
}

int PMDockSplitter::checkValue( int position ) const
{
  if (initialised){
    if (orientation ==Qt::Vertical){
      if (position < (child0->minimumSize().width()))
        position = child0->minimumSize().width();
      if ((width()-4-position) < (child1->minimumSize().width()))
        position = width() - (child1->minimumSize().width()) -4;
    } else {
      if (position < (child0->minimumSize().height()))
        position = (child0->minimumSize().height());
      if ((height()-4-position) < (child1->minimumSize().height()))
        position = height() - (child1->minimumSize().height()) -4;
    }
  }

  if (position < 0) position = 0;

  if ((orientation ==Qt::Vertical) && (position > width()))
    position = width();
  if ((orientation ==Qt::Horizontal) && (position > height()))
    position = height();

  return position;
}

bool PMDockSplitter::eventFilter(TQObject *o, TQEvent *e)
{
  TQMouseEvent *mev;
  bool handled = false;
  int factor = (mHighResolution)? 10000:100;

  switch (e->type()) {
    case TQEvent::MouseMove:
      mev= (TQMouseEvent*)e;
      child0->setUpdatesEnabled(mOpaqueResize);
      child1->setUpdatesEnabled(mOpaqueResize);
      if (orientation ==Qt::Horizontal) {
        if (!mOpaqueResize) {
          int position = checkValue( mapFromGlobal(mev->globalPos()).y() );
          divider->move( 0, position );
        } else {
          xpos = factor * checkValue( mapFromGlobal(mev->globalPos()).y() ) / height();
          resizeEvent(0);
          divider->repaint(true);
        }
      } else {
        if (!mOpaqueResize) {
          int position = checkValue( mapFromGlobal(TQCursor::pos()).x() );
          divider->move( position, 0 );
        } else {
          xpos = factor * checkValue( mapFromGlobal( mev->globalPos()).x() ) / width();
          resizeEvent(0);
          divider->repaint(true);
        }
      }
      handled= true;
      break;
    case TQEvent::MouseButtonRelease:
      child0->setUpdatesEnabled(true);
      child1->setUpdatesEnabled(true);
      mev= (TQMouseEvent*)e;
      if (orientation ==Qt::Horizontal){
        xpos = factor* checkValue( mapFromGlobal(mev->globalPos()).y() ) / height();
        resizeEvent(0);
        divider->repaint(true);
      } else {
        xpos = factor* checkValue( mapFromGlobal(mev->globalPos()).x() ) / width();
        resizeEvent(0);
        divider->repaint(true);
      }
      handled= true;
      break;
    default:
      break;
  }
  return (handled) ? true : TQWidget::eventFilter( o, e );
}

bool PMDockSplitter::event( TQEvent* e )
{
  if ( e->type() == TQEvent::LayoutHint ){
    // change children min/max size
    setupMinMaxSize();
    setSeparatorPos(xpos);
  }
  return TQWidget::event(e);
}

TQWidget* PMDockSplitter::getAnother( TQWidget* w ) const
{
  return ( w == child0 ) ? child1 : child0;
}

void PMDockSplitter::updateName()
{
  if ( !initialised ) return;

  TQString new_name = TQString( child0->name() ) + "," + child1->name();
  parentWidget()->setName( new_name.latin1() );
  parentWidget()->setCaption( child0->caption() + "," + child1->caption() );
  parentWidget()->repaint( false );

  ((PMDockWidget*)parentWidget())->firstName = child0->name();
  ((PMDockWidget*)parentWidget())->lastName = child1->name();
  ((PMDockWidget*)parentWidget())->splitterOrientation = orientation;

  TQWidget* p = parentWidget()->parentWidget();
  if ( p != 0L && p->inherits("PMDockSplitter" ) )
    ((PMDockSplitter*)p)->updateName();
}

void PMDockSplitter::setOpaqueResize(bool b)
{
  mOpaqueResize = b;
}

bool PMDockSplitter::opaqueResize() const
{
  return mOpaqueResize;
}

void PMDockSplitter::setKeepSize(bool b)
{
  mKeepSize = b;
}

bool PMDockSplitter::keepSize() const
{
  return mKeepSize;
}

void PMDockSplitter::setHighResolution(bool b)
{
  if (mHighResolution) {
    if (!b) xpos = xpos/100;
  } else {
    if (b) xpos = xpos*100;
  }
  mHighResolution = b;
}

bool PMDockSplitter::highResolution() const
{
  return mHighResolution;
}


/*************************************************************************/
PMDockButton_Private::PMDockButton_Private( TQWidget *parent, const char * name )
:TQPushButton( parent, name )
{
  moveMouse = false;
  setFocusPolicy( TQ_NoFocus );
}

PMDockButton_Private::~PMDockButton_Private()
{
}

void PMDockButton_Private::drawButton( TQPainter* p )
{
  p->fillRect( 0,0, width(), height(), TQBrush(colorGroup().brush(TQColorGroup::Background)) );
  p->drawPixmap( (width() - pixmap()->width()) / 2, (height() - pixmap()->height()) / 2, *pixmap() );
  if ( moveMouse && !isDown() ){
    p->setPen( white );
    p->moveTo( 0, height() - 1 );
    p->lineTo( 0, 0 );
    p->lineTo( width() - 1, 0 );

    p->setPen( colorGroup().dark() );
    p->lineTo( width() - 1, height() - 1 );
    p->lineTo( 0, height() - 1 );
  }
  if ( isOn() || isDown() ){
    p->setPen( colorGroup().dark() );
    p->moveTo( 0, height() - 1 );
    p->lineTo( 0, 0 );
    p->lineTo( width() - 1, 0 );

    p->setPen( white );
    p->lineTo( width() - 1, height() - 1 );
    p->lineTo( 0, height() - 1 );
  }
}

void PMDockButton_Private::enterEvent( TQEvent * )
{
  moveMouse = true;
  repaint();
}

void PMDockButton_Private::leaveEvent( TQEvent * )
{
  moveMouse = false;
  repaint();
}

/*************************************************************************/
PMDockWidgetPrivate::PMDockWidgetPrivate()
  : TQObject()
  ,index(-1)
  ,splitPosInPercent(50)
  ,pendingFocusInEvent(false)
  ,blockHasUndockedSignal(false)
{
#ifndef NO_KDE2
  windowType = NET::Normal;
#endif

  _parent = 0L;
  transient = false;
}

PMDockWidgetPrivate::~PMDockWidgetPrivate()
{
}

void PMDockWidgetPrivate::slotFocusEmbeddedWidget(TQWidget* w)
{
   if (w) {
      TQWidget* embeddedWdg = ((PMDockWidget*)w)->getWidget();
      if (embeddedWdg && ((embeddedWdg->focusPolicy() == TQ_ClickFocus) || (embeddedWdg->focusPolicy() == TQ_StrongFocus))) {
         embeddedWdg->setFocus();
      }
   }
}

#ifndef NO_INCLUDE_MOCFILES // for TQt-only projects, because tmake doesn't take this name
#include "pmdockwidget_private.moc"
#endif
