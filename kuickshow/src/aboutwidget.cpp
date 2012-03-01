/* This file is part of the KDE project
   Copyright (C) 1998-2002 Carsten Pfeiffer <pfeiffer@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation, version 2.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include <tqdatetime.h>
#include <tqevent.h>
#include <tqglobal.h>
#include <tqgroupbox.h>
#include <tqlabel.h>

#include <twin.h>
#include <kstandarddirs.h>

#include "imlibwidget.h"
#include "kurlwidget.h"
#include "version.h"

#include "aboutwidget.h"

AboutWidget::AboutWidget( TQWidget *parent, const char *name )
    : TQVBox( parent, name, TQt::WShowModal )
{
    KWin::setType( winId(), NET::Override );
    KWin::setState( winId(), NET::SkipTaskbar );

    setFrameStyle( WinPanel | Raised );

    TQGroupBox *gBox = new TQGroupBox( 1,Qt::Horizontal, this);
    gBox->setGeometry( 10, 10, width()-20, height()-20 );
    gBox->setAlignment( AlignHCenter );
    gBox->installEventFilter( this );

    gBox->setPalette( TQPalette( TQColor( white ) ) );
    gBox->setBackgroundMode( PaletteBackground );

    int hour = TQTime::currentTime().hour();
    TQString file;

    if ( hour >= 10 && hour < 16 )
	file = locate("appdata", "pics/kuickshow-day.jpg");
    else
	file = locate("appdata", "pics/kuickshow-night.jpg");

    TQLabel *authors = new TQLabel("Kuickshow " KUICKSHOWVERSION
				 " was brought to you by", gBox);
    authors->setAlignment( AlignCenter );

    m_homepage = new KURLWidget("Carsten Pfeiffer", gBox);
    m_homepage->setURL( "http://devel-home.kde.org/~pfeiffer/kuickshow/" );
    m_homepage->setAlignment( AlignCenter );

    TQLabel *copy = new TQLabel("(C) 1998-2006", gBox);
    copy->setAlignment( AlignCenter );

    ImlibWidget *im = new ImlibWidget( 0L, gBox, "KuickShow Logo" );
    if ( im->loadImage( file ) )
	im->setFixedSize( im->width(), im->height() );
    else {
	delete im;
	im = 0L;
	tqWarning( "KuickShow: about-image not found/unreadable." );
    }
}

AboutWidget::~AboutWidget()
{
}

bool AboutWidget::eventFilter( TQObject *o, TQEvent *e )
{
    if ( e->type() == TQEvent::MouseButtonPress ) {
        TQMouseEvent *ev = TQT_TQMOUSEEVENT( e );
        if ( !m_homepage->geometry().contains( ev->pos() ) ) {
            deleteLater();
            return true;
        }
    }

    return TQVBox::eventFilter( o, e );
}
#include "aboutwidget.moc"
