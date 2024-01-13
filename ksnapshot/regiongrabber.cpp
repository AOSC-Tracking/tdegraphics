/*
  Copyright (C) 2003 Nadeem Hasan <nhasan@kde.org>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or ( at your option ) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this library; see the file COPYING.  If not, write to
  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301, USA.
*/

#include "regiongrabber.h"

#include <tqapplication.h>
#include <tqpainter.h>
#include <tqpalette.h>
#include <tqstyle.h>
#include <tqtimer.h>
#include <tqtooltip.h>

#include <tdeglobalsettings.h>

SizeTip::SizeTip( TQWidget *parent, const char *name )
    : TQLabel( parent, name, WStyle_Customize | WX11BypassWM |
      WStyle_StaysOnTop | WStyle_NoBorder | WStyle_Tool )
{
  setMargin( 2 );
  setIndent( 0 );
  setFrameStyle( TQFrame::Plain | TQFrame::Box );

  setPalette( TQToolTip::palette() );
}

void SizeTip::setTip( const TQRect &rect )
{
  TQString tip = TQString( "%1x%2" ).arg( rect.width() )
      .arg( rect.height() );

  setText( tip );
  adjustSize();

  positionTip( rect );
}

void SizeTip::positionTip( const TQRect &rect )
{
  TQRect tipRect = geometry();
  tipRect.moveTopLeft( TQPoint( 0, 0 ) );

  if ( rect.intersects( tipRect ) )
  {
    TQRect deskR = TDEGlobalSettings::desktopGeometry( TQPoint( 0, 0 ) );

    tipRect.moveCenter( TQPoint( deskR.width()/2, deskR.height()/2 ) );
    if ( !rect.contains( tipRect, true ) && rect.intersects( tipRect ) )
      tipRect.moveBottomRight( geometry().bottomRight() );
  }

  move( tipRect.topLeft() );
}

RegionGrabber::RegionGrabber()
  : TQWidget( 0, 0, WStyle_Customize | WX11BypassWM ),
    mouseDown( false ), sizeTip( 0L )
{
  sizeTip = new SizeTip( ( TQWidget * )0L );

  tipTimer = new TQTimer( this );
  connect( tipTimer, TQ_SIGNAL( timeout() ), TQ_SLOT( updateSizeTip() ) );

  TQTimer::singleShot( 200, this, TQ_SLOT( initGrabber() ) );
}

RegionGrabber::~RegionGrabber()
{
  delete sizeTip;
}

void RegionGrabber::initGrabber()
{
  pixmap = TQPixmap::grabWindow( tqt_xrootwin() );
  setPaletteBackgroundPixmap( pixmap );

  TQDesktopWidget desktopWidget;
  TQRect desktopSize;
  if ( desktopWidget.isVirtualDesktop() )
    desktopSize = desktopWidget.geometry();
  else
    desktopSize = desktopWidget.screenGeometry( tqt_xrootwin() );

  setGeometry( desktopSize );
  showFullScreen();

  TQApplication::setOverrideCursor( crossCursor );
}

void RegionGrabber::mousePressEvent( TQMouseEvent *e )
{
  if ( e->button() == TQt::LeftButton )
  {
    mouseDown = true;
    grabRect = TQRect( e->pos(), e->pos() );
  }
}

void RegionGrabber::mouseMoveEvent( TQMouseEvent *e )
{
  if ( mouseDown )
  {
    sizeTip->hide();
    tipTimer->start( 250, true );

    drawRubber();
    grabRect.setBottomRight( e->pos() );
    drawRubber();
  }
}

void RegionGrabber::mouseReleaseEvent( TQMouseEvent *e )
{
  mouseDown = false;
  drawRubber();
  sizeTip->hide();

  grabRect.setBottomRight( e->pos() );
  grabRect = grabRect.normalize();

  TQPixmap region = TQPixmap::grabWindow( winId(), grabRect.x(), grabRect.y(),
      grabRect.width(), grabRect.height() );

  TQApplication::restoreOverrideCursor();

  emit regionGrabbed( region );
}

void RegionGrabber::keyPressEvent( TQKeyEvent *e )
{
  if ( e->key() == Key_Escape )
  {
    TQApplication::restoreOverrideCursor();
    emit regionGrabbed( TQPixmap() );
  }
  else
    e->ignore();
}

void RegionGrabber::updateSizeTip()
{
  TQRect rect = grabRect.normalize();

  sizeTip->setTip( rect );
  sizeTip->show();
}

void RegionGrabber::drawRubber()
{
  TQPainter p;
  p.begin( this );
  p.setRasterOp( NotROP );
  p.setPen( TQPen( color0, 1 ) );
  p.setBrush( NoBrush );

  style().drawPrimitive( TQStyle::PE_FocusRect, &p, grabRect, colorGroup(),
      TQStyle::Style_Default, TQStyleOption( colorGroup().base() ) );

  p.end();
}

#include "regiongrabber.moc"
