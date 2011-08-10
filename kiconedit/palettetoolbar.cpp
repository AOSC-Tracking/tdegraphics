/*
    kiconedit - a small graphics drawing program for the KDE
    Copyright ( C ) 1998  Thomas Tanghus (  tanghus@kde.org )
    Copyright ( C ) 2002  Nadeem Hasan (  nhasan@kde.org )

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (  at your option ) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include <tqlabel.h>
#include <tqlayout.h>
#include <tqwhatsthis.h>
#include <tqpainter.h>

#include <klocale.h>
#include <kdebug.h>

#include "kiconcolors.h"
#include "palettetoolbar.h"

PaletteToolBar::PaletteToolBar( TQWidget *tqparent, const char *name )
    : KToolBar( tqparent, name )
{
  TQWidget *base = new TQWidget( this );

  TQBoxLayout::Direction d = orientation() == Qt::Horizontal? 
      TQBoxLayout::LeftToRight : TQBoxLayout::TopToBottom;
  m_layout = new TQBoxLayout( base, d, 2, 6 );

  m_lblPreview = new TQLabel( base );
  m_lblPreview->setFrameStyle( TQFrame::Panel|TQFrame::Sunken );
  m_lblPreview->setFixedHeight( 64 );
  m_lblPreview->tqsetAlignment( TQt::AlignHCenter|TQt::AlignVCenter );
  TQWhatsThis::add(m_lblPreview, i18n( "Preview\n\nThis is a 1:1 preview"
      " of the current icon" ) );
  m_layout->addWidget( m_lblPreview );

  m_currentColorView = new TQLabel( base );
  m_currentColorView->setFrameStyle( TQFrame::Panel|TQFrame::Sunken );
  m_currentColorView->setFixedHeight( 24 );
  m_currentColorView->tqsetAlignment( TQt::AlignHCenter|TQt::AlignVCenter );
  TQWhatsThis::add(m_currentColorView, i18n( "Current color\n\nThis is the currently selected color" ) );
  m_layout->addWidget( m_currentColorView );

  TQVBoxLayout *vtqlayout = new TQVBoxLayout( m_layout, 0 );
  TQLabel *l = new TQLabel( i18n( "System colors:" ), base );
  vtqlayout->addWidget( l );
  m_sysColors = new KSysColors( base );
  TQWhatsThis::add(m_sysColors, i18n( "System colors\n\nHere you can select"
      " colors from the KDE icon palette" ) );

  vtqlayout->addWidget( m_sysColors );

  connect( m_sysColors, TQT_SIGNAL( newColor(uint) ), 
      TQT_SIGNAL( newColor(uint) ) );

  vtqlayout = new TQVBoxLayout( m_layout, 0 );
  l = new TQLabel( i18n( "Custom colors:" ), base );
  vtqlayout->addWidget( l );
  m_customColors = new KCustomColors( base );
  TQWhatsThis::add(m_customColors, i18n( "Custom colors\n\nHere you can"
      " build a palette of custom colors.\nDouble-click on a box to edit"
      " the color" ) );

  vtqlayout->addWidget( m_customColors );

  connect( m_customColors, TQT_SIGNAL( newColor(uint) ), 
      TQT_SIGNAL( newColor(uint) ) );
  connect( this, TQT_SIGNAL( newColor(uint)),
      this, TQT_SLOT(currentColorChanged(uint)));
  currentColorChanged(OPAQUE_MASK|0);

  setEnableContextMenu( false );
  setMovingEnabled( false );
}

void PaletteToolBar::setOrientation( Qt::Orientation o )
{
   if(  barPos() == Floating )
     o = o == Qt::Vertical ? Qt::Horizontal : Qt::Vertical;

  TQBoxLayout::Direction d = o == Qt::Horizontal? TQBoxLayout::LeftToRight
        : TQBoxLayout::TopToBottom;
  m_layout->setDirection( d );

  TQDockWindow::setOrientation( o );
}

void PaletteToolBar::previewChanged(  const TQPixmap &p )
{
  m_lblPreview->setPixmap( p );
}

void PaletteToolBar::addColors( uint n, uint *c )
{
    m_customColors->clear();
    for( uint i = 0; i < n; i++ )
        addColor( c[ i ] );
}

void PaletteToolBar::addColor( uint color )
{
    if( !m_sysColors->contains( color ) )
        m_customColors->addColor( color );
}

void PaletteToolBar::setPreviewBackground( TQPixmap pixmap )
{
    m_lblPreview->setBackgroundPixmap(pixmap);
}

void PaletteToolBar::setPreviewBackground( const TQColor& colour )
{
    m_lblPreview->setBackgroundColor(colour);
}

void PaletteToolBar::currentColorChanged(uint color)
{
  if(tqAlpha(color) == 255)
  {
    m_currentColorView->setBackgroundColor(color);
  }
  else
  {
    // Show the colour as if drawn over a checkerboard pattern
    const int squareWidth = 8;
    const uint lightColour = tqRgb(255, 255, 255);
    const uint darkColour = tqRgb(127, 127, 127);

    TQPixmap pm(2 * squareWidth, 2 * squareWidth);
    TQPainter p(&pm);

    double alpha = tqAlpha(color) / 255.0;

    int r = int(tqRed(color) * alpha + (1 - alpha) * tqRed(lightColour) + 0.5);
    int g = int(tqGreen(color) * alpha + (1 - alpha) * tqGreen(lightColour) + 0.5);
    int b = int(tqBlue(color) * alpha + (1 - alpha) * tqBlue(lightColour) + 0.5);

    uint squareColour = tqRgb(r, g, b);

    p.setPen(TQt::NoPen);
    p.setBrush(TQColor(squareColour));
    p.drawRect(0, 0, squareWidth, squareWidth);
    p.drawRect(squareWidth, squareWidth, squareWidth, squareWidth);

    r = int(tqRed(color) * alpha + (1 - alpha) * tqRed(darkColour) + 0.5);
    g = int(tqGreen(color) * alpha + (1 - alpha) * tqGreen(darkColour) + 0.5);
    b = int(tqBlue(color) * alpha + (1 - alpha) * tqBlue(darkColour) + 0.5);

    squareColour = tqRgb(r, g, b);

    p.setBrush(TQColor(squareColour));
    p.drawRect(squareWidth, 0, squareWidth, squareWidth);
    p.drawRect(0, squareWidth, squareWidth, squareWidth);

    p.end();

    m_currentColorView->setBackgroundPixmap(pm);
  }
}

#include "palettetoolbar.moc"

/* vim: et sw=2 ts=2
*/

