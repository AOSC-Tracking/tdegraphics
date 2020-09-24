/*  This file is part of the KDE project
    Copyright (C) 2002 Matthias Kretz <kretz@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

*/
// $Id$

#include "kviewbrowser.h"
#include "kmyfileitemlist.h"

#include <tqcursor.h>

#include <kdirlister.h>
#include <tdeaction.h>
#include <tdelocale.h>
#include <kgenericfactory.h>
#include <kdebug.h>
#include <kimageviewer/viewer.h>
#include <kimageviewer/canvas.h>
#include <tdeparts/browserextension.h>
#include <tdeapplication.h>
#include <kimageio.h>

typedef KGenericFactory<KViewBrowser> KViewBrowserFactory;
K_EXPORT_COMPONENT_FACTORY( kview_browserplugin, KViewBrowserFactory( "kviewbrowserplugin" ) )

KViewBrowser::KViewBrowser( TQObject* parent, const char* name, const TQStringList & )
	: Plugin( parent, name )
	, m_pDirLister( 0 )
	, m_pFileItemList( 0 )
	, m_bShowCurrent( false )
{
	m_pViewer = static_cast<KImageViewer::Viewer *>( parent );
	if( m_pViewer )
	{
		m_paBack    = KStdAction::back   ( this, TQT_SLOT( slotBack()    ), actionCollection(), "previous_image" );
		m_paBack->setShortcut( SHIFT+Key_Left );
		m_paForward = KStdAction::forward( this, TQT_SLOT( slotForward() ), actionCollection(), "next_image"     );
		m_paForward->setShortcut( SHIFT+Key_Right );
		m_pExtension = m_pViewer->browserExtension();
	}
	else
		kdWarning( 4630 ) << "no KImageViewer interface found - the browser plugin won't work" << endl;
}

KViewBrowser::~KViewBrowser()
{
	delete m_pDirLister;
	delete m_pFileItemList;
}

void KViewBrowser::openURL(const KURL &u)
{
	if (m_pViewer)
	{
		// Opening new URL resets zoom, so remember it.
		double oldZoom = m_pViewer->canvas()->zoom();
		m_pViewer->openURL(u);
		m_pViewer->canvas()->setZoom(oldZoom);
	}
	if( m_pExtension )
	{
		emit m_pExtension->setLocationBarURL( u.prettyURL() );
	}
}

void KViewBrowser::slotBack()
{
	setupDirLister();
	if( ! m_pFileItemList )
		return;

	KFileItem * item = m_pFileItemList->prev();
	if( ! item )
		item = m_pFileItemList->last();
	if( item )
	{
		kdDebug( 4630 ) << item->url().prettyURL() << endl;
		openURL( item->url() );
	}
	else
		kdDebug( 4630 ) << "no file found" << endl;
	m_bShowCurrent = false;
}

void KViewBrowser::slotForward()
{
	setupDirLister();
	if( ! m_pFileItemList )
		return;

	KFileItem * item = m_bShowCurrent ? m_pFileItemList->current() : m_pFileItemList->next();
	if( ! item )
		item = m_pFileItemList->first();
	if( item )
	{
		kdDebug( 4630 ) << item->url().prettyURL() << endl;
		openURL( item->url() );
	}
	else
		kdDebug( 4630 ) << "no file found" << endl;
	m_bShowCurrent = false;
}

void KViewBrowser::slotNewItems( const KFileItemList & items )
{
	kdDebug( 4630 ) << k_funcinfo << endl;
	delete m_pFileItemList;
	m_pFileItemList = new KMyFileItemList( items );
	m_pFileItemList->sort();

	// set the current pointer on the currently open image
	KFileItem * item = m_pFileItemList->first();
	for( ; item; item = m_pFileItemList->next() )
		if( item->url() == m_pViewer->url() )
			break;
}

void KViewBrowser::slotDeleteItem( KFileItem * item )
{
	bool setToFirst = false;
	if( m_pFileItemList->current() == item )
	{
		// The current image is being removed
		// we have to take care, that the next slotForward call returns the new current item
		m_bShowCurrent = true;

		if( m_pFileItemList->getLast() == item )
			// The the current image is the last image - wrap around to the first
			setToFirst = true;
	}

	m_pFileItemList->remove( item );

	if( setToFirst )
		( void )m_pFileItemList->first();
}

void KViewBrowser::setupDirLister()
{
	if( ! m_pDirLister )
	{
		kdDebug( 4630 ) << "create new KDirLister" << endl;
		m_pDirLister = new KDirLister();
		m_pDirLister->setMimeFilter( KImageIO::mimeTypes( KImageIO::Reading ) );
		m_pDirLister->setShowingDotFiles( true );
		connect( m_pDirLister, TQT_SIGNAL( newItems( const KFileItemList & ) ), TQT_SLOT( slotNewItems( const KFileItemList & ) ) );
		connect( m_pDirLister, TQT_SIGNAL( deleteItem( KFileItem * ) ), TQT_SLOT( slotDeleteItem( KFileItem * ) ) );
	}
	if( m_pDirLister->url() != KURL( m_pViewer->url().directory( true, false ) ) )
	{
		TQApplication::setOverrideCursor( WaitCursor );
		TQString url = m_pViewer->url().prettyURL();
		int pos = url.findRev( "/" );
		url = url.left( (unsigned int)pos );
		kdDebug( 4630 ) << "open KDirLister for " << url << endl;
		m_pDirLister->openURL( KURL( url ));
		while( ! m_pDirLister->isFinished() )
			kapp->processEvents();
		//while( ! m_pFileItemList )
			//kapp->processEvents();
		TQApplication::restoreOverrideCursor();
	}
}
#include "kviewbrowser.moc"
