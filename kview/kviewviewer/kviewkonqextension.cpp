/*  This file is part of the KDE project
    Copyright (C) 1999 Simon Hausmann <hausmann@kde.org>
                  2001 Matthias Kretz <kretz@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

*/

#include "kviewkonqextension.h"
#include "kviewviewer.h"
#include "kimageviewer/canvas.h"
#include "imagesettings.h"

#include <tqpainter.h>
#include <tqimage.h>
#include <tqpaintdevicemetrics.h>

#include <kprinter.h>
#include <kdebug.h>
#include <kglobal.h>
#include <klocale.h>

KViewKonqExtension::KViewKonqExtension( KImageViewer::Canvas * canvas,
		KViewViewer *tqparent, const char *name )
	: KParts::BrowserExtension( tqparent, name ),
	m_pViewer( tqparent ),
	m_pCanvas( canvas )
{
    KGlobal::locale()->insertCatalogue("kview");
}

void KViewKonqExtension::setXYOffset( int x, int y )
{
	m_pCanvas->setXYOffset( x, y );
}

int KViewKonqExtension::xOffset()
{
	return m_pCanvas->xOffset();
}

int KViewKonqExtension::yOffset()
{
	return m_pCanvas->yOffset();
}

void KViewKonqExtension::print()
{
	if( ! m_pCanvas->image() )
	{
		kdError( 4610 ) << "No image to print" << endl;
		return;
	}

	KPrinter printer;//( true, TQPrinter::ScreenResolution );
	printer.addDialogPage( new ImageSettings );
	printer.setDocName( "KView: " + m_pViewer->url().fileName( false ) );

	if ( !printer.setup( ((KViewViewer *)tqparent())->widget(), i18n("Print %1").tqarg(m_pViewer->url().fileName( false )) ) )
		return;

	TQPainter painter;
	painter.begin( &printer );

	TQPaintDeviceMetrics metrics( painter.device() );
	kdDebug( 4610 ) << "metrics: " << metrics.width() << "x" << metrics.height() << endl;
	TQPoint pos( 0, 0 );

	TQImage imagetoprint;
	if( printer.option( "app-kviewviewer-fitimage" ) == "1" )
		imagetoprint = m_pCanvas->image()->smoothScale( metrics.width(), metrics.height(), TQ_ScaleMin );
	else
		imagetoprint = *m_pCanvas->image();

	if( printer.option( "app-kviewviewer-center" ) == "1" )
	{
		pos.setX( ( metrics.width() - imagetoprint.width() ) / 2 );
		pos.setY( ( metrics.height() - imagetoprint.height() ) / 2 );
	}

	painter.drawImage( pos, imagetoprint );
	painter.end();
}

void KViewKonqExtension::del()
{
	m_pViewer->slotDel();
}

// vim:sw=4:ts=4

#include "kviewkonqextension.moc"

