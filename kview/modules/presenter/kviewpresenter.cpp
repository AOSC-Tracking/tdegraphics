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

/* $Id$ */

#include "kviewpresenter.h"
#include "imagelistdialog.h"
#include "imagelistitem.h"

#include <tqvbox.h>
#include <tqobjectlist.h>
#include <tqsignalslotimp.h>
#include <tqtimer.h>
#include <tqevent.h>
#include <tqdragobject.h>
#include <tqstringlist.h>

#include <kpushbutton.h>
#include <kapplication.h>
#include <kaction.h>
#include <klocale.h>
#include <kgenericfactory.h>
#include <kdebug.h>
#include <kglobal.h>
#include <kiconloader.h>
#include <knuminput.h>
#include <kfiledialog.h>
#include <kimageio.h>
#include <kimageviewer/viewer.h>
#include <kimageviewer/canvas.h>
#include <kio/netaccess.h>
#include <kmessagebox.h>
#include <ktempfile.h>
#include <kurldrag.h>

typedef KGenericFactory<KViewPresenter> KViewPresenterFactory;
K_EXPORT_COMPONENT_FACTORY( kview_presenterplugin, KViewPresenterFactory( "kviewpresenterplugin" ) )

KViewPresenter::KViewPresenter( TQObject* tqparent, const char* name, const TQStringList & )
	: Plugin( tqparent, name )
	, m_pImageList( new ImageListDialog() )
	, m_paFileOpen( 0 )
	, m_bDontAdd( false )
	, m_pCurrentItem( 0 )
	, m_pSlideshowTimer( new TQTimer( this ) )
{
	kdDebug( 4630 ) << k_funcinfo << endl;
	m_imagelist.setAutoDelete( true );

	TQObjectList * viewerList = tqparent->queryList( 0, "KImageViewer Part", false, false );
	m_pViewer = static_cast<KImageViewer::Viewer *>( viewerList->getFirst() );
	delete viewerList;
	if( m_pViewer )
	{
		( void ) new KAction( i18n( "&Image List..." ), 0, 0,
							this, TQT_SLOT( slotImageList() ),
							actionCollection(), "plugin_presenter_imageList" );
		m_paSlideshow = new KToggleAction( i18n( "Start &Slideshow" ), Key_S, actionCollection(), "plugin_presenter_slideshow" );
		( void ) new KAction( i18n( "&Previous Image in List" ), "previous", ALT+Key_Left,
							  this, TQT_SLOT( prev() ),
							  actionCollection(), "plugin_presenter_prev" );
		( void ) new KAction( i18n( "&Next Image in List" ), "next", ALT+Key_Right,
							  this, TQT_SLOT( next() ),
							  actionCollection(), "plugin_presenter_next" );

		connect( m_paSlideshow, TQT_SIGNAL( toggled( bool ) ), m_pImageList->m_pSlideshow, TQT_SLOT( setOn( bool ) ) );
		connect( m_pImageList->m_pSlideshow, TQT_SIGNAL( toggled( bool ) ), m_paSlideshow, TQT_SLOT( setChecked( bool ) ) );

		// search for file_open action
		KXMLGUIClient * parentClient = static_cast<KXMLGUIClient*>( tqparent->qt_cast( "KXMLGUIClient" ) );
		if( parentClient )
		{
			m_paFileOpen = parentClient->actionCollection()->action( "file_open" );
			m_paFileClose = parentClient->actionCollection()->action( "file_close" );
		}
		if( m_paFileClose )
			connect( m_paFileClose, TQT_SIGNAL( activated() ), this, TQT_SLOT( slotClose() ) );
		if( m_paFileOpen )
		{
			disconnect( m_paFileOpen, TQT_SIGNAL( activated() ), tqparent, TQT_SLOT( slotOpenFile() ) );
			connect( m_paFileOpen, TQT_SIGNAL( activated() ), this, TQT_SLOT( slotOpenFiles() ) );
		}
		else
		{
			(void) new KAction( i18n( "Open &Multiple Files..." ), "queue", CTRL+SHIFT+Key_O,
							this, TQT_SLOT( slotOpenFiles() ),
							actionCollection(), "plugin_presenter_openFiles" );
		}
		connect( m_pViewer, TQT_SIGNAL( imageOpened( const KURL & ) ),
				TQT_SLOT( slotImageOpened( const KURL & ) ) );
	}
	else
		kdWarning( 4630 ) << "no KImageViewer interface found - the presenter plugin won't work" << endl;

	//( void )new KViewPresenterConfModule( this );

	connect( m_pImageList->m_pListView, TQT_SIGNAL( executed( TQListViewItem* ) ),
			this, TQT_SLOT( changeItem( TQListViewItem* ) ) );
	connect( m_pImageList->m_pPrevious, TQT_SIGNAL( clicked() ),
			this, TQT_SLOT( prev() ) );
	connect( m_pImageList->m_pNext, TQT_SIGNAL( clicked() ),
			this, TQT_SLOT( next() ) );
	connect( m_pImageList->m_pListView, TQT_SIGNAL( spacePressed( TQListViewItem* ) ),
			this, TQT_SLOT( changeItem( TQListViewItem* ) ) );
	connect( m_pImageList->m_pListView, TQT_SIGNAL( returnPressed( TQListViewItem* ) ),
			this, TQT_SLOT( changeItem( TQListViewItem* ) ) );
	connect( m_pImageList->m_pSlideshow, TQT_SIGNAL( toggled( bool ) ),
			this, TQT_SLOT( slideshow( bool ) ) );
	connect( m_pImageList->m_pInterval, TQT_SIGNAL( valueChanged( int ) ),
			this, TQT_SLOT( setSlideshowInterval( int ) ) );
	connect( m_pImageList->m_pShuffle, TQT_SIGNAL( clicked() ),
			this, TQT_SLOT( shuffle() ) );
	connect( m_pImageList->m_pLoad, TQT_SIGNAL( clicked() ),
			this, TQT_SLOT( loadList() ) );
	connect( m_pImageList->m_pSave, TQT_SIGNAL( clicked() ),
			this, TQT_SLOT( saveList() ) );
	connect( m_pImageList->m_pCloseAll, TQT_SIGNAL( clicked() ),
			this, TQT_SLOT( closeAll() ) );

	// allow drop on the dialog
	m_pImageList->installEventFilter( this );
	m_pImageList->m_pListView->installEventFilter( this );
	m_pImageList->m_pListView->viewport()->installEventFilter( this );

	// grab drops on the main view
	m_pViewer->widget()->installEventFilter( this );

	connect( m_pSlideshowTimer, TQT_SIGNAL( timeout() ),
			this, TQT_SLOT( next() ) );
}

KViewPresenter::~KViewPresenter()
{
	kdDebug( 4630 ) << k_funcinfo << endl;
	if( m_paFileOpen )
	{
		disconnect( m_paFileOpen, TQT_SIGNAL( activated() ), this, TQT_SLOT( slotOpenFiles() ) );
		// If the tqparent() doesn't exist we either leave the "File Open" action
		// in an unusable state or KView was just shutting down and therefor we
		// can ignore this. I've only seen the second one happening and to get
		// rid of the TQObject::connect warning we do the tqparent() check.
		if( tqparent() )
			connect( m_paFileOpen, TQT_SIGNAL( activated() ), tqparent(), TQT_SLOT( slotOpenFile() ) );
	}
}

bool KViewPresenter::eventFilter( TQObject *obj, TQEvent *ev )
{
	if( TQT_BASE_OBJECT(obj) == TQT_BASE_OBJECT(m_pImageList) || TQT_BASE_OBJECT(obj) == TQT_BASE_OBJECT(m_pImageList->m_pListView) || TQT_BASE_OBJECT(obj) == TQT_BASE_OBJECT(m_pImageList->m_pListView->viewport()) || TQT_BASE_OBJECT(obj) == TQT_BASE_OBJECT(m_pViewer->widget()) )
	{
		switch( ev->type() )
		{
			case TQEvent::DragEnter:
			case TQEvent::DragMove:
			{
				// drag enter event in the image list
				//kdDebug( 4630 ) << "DragEnterEvent in the image list: " << obj->className() << endl;
				TQDragEnterEvent * e = static_cast<TQDragEnterEvent*>( ev );
				//for( int i = 0; e->format( i ); ++i )
					//kdDebug( 4630 ) << " - " << e->format( i ) << endl;
				if( KURLDrag::canDecode( e ) )// || TQImageDrag::canDecode( e ) )
				{
					e->accept();
					return true;
				}
			}
			case TQEvent::Drop:
			{
				// drop event in the image list
				kdDebug( 4630 ) << "DropEvent in the image list: " << obj->className() << endl;
				TQDropEvent * e = static_cast<TQDropEvent*>( ev );
				TQStringList l;
				//TQImage image;
				if( KURLDrag::decodeToUnicodeUris( e, l ) )
				{
					for( TQStringList::const_iterator it = l.begin(); it != l.end(); ++it )
					{
						ImageInfo * info = new ImageInfo( KURL( *it ) );
						if( ! m_imagelist.tqcontains( info ) )
						{
							m_imagelist.inSort( info );
							( void )new ImageListItem( m_pImageList->m_pListView, KURL( *it ) );
						}
						else
							delete info;
					}
					return true;
				}
				//else if( TQImageDrag::decode( e, image ) )
					//newImage( image );
			}
			default: // do nothing
				break;
		}
	}
	return KParts::Plugin::eventFilter( obj, ev );
}

void KViewPresenter::slotImageOpened( const KURL & url )
{
	kdDebug( 4630 ) << k_funcinfo << endl;
	if( ! m_bDontAdd )
	{
		kdDebug( 4630 ) << k_funcinfo << "imagelist:" << endl;
		ImageInfo * info = new ImageInfo( url );
		if( ! m_imagelist.tqcontains( info ) )
		{
			m_imagelist.inSort( info );
			TQListViewItem * item = new ImageListItem( m_pImageList->m_pListView, url );
			makeCurrent( item );
		}
		else
			delete info;
	}
}

void KViewPresenter::slotImageList()
{
	kdDebug( 4630 ) << k_funcinfo << endl;
	m_pImageList->show();
}

void KViewPresenter::slotOpenFiles()
{
	kdDebug( 4630 ) << k_funcinfo << endl;
	KURL::List urls = KFileDialog::getOpenURLs( ":load_image", KImageIO::pattern( KImageIO::Reading ), m_pViewer->widget() );

	if( urls.isEmpty() )
		return;

	KURL::List::Iterator it = urls.begin();
	m_pViewer->openURL( *it );
	for( ++it; it != urls.end(); ++it )
	{
		ImageInfo * info = new ImageInfo( *it );
		if( ! m_imagelist.tqcontains( info ) )
		{
			m_imagelist.inSort( info );
			( void )new ImageListItem( m_pImageList->m_pListView, *it );
		}
		else
			delete info;
	}
}

void KViewPresenter::slotClose()
{
	TQListViewItem * next = m_pCurrentItem->itemBelow() ? m_pCurrentItem->itemBelow() : m_pImageList->m_pListView->firstChild();
	if( next == m_pCurrentItem )
		next = 0;

	ImageInfo info( m_pCurrentItem->url() );
	m_imagelist.remove( &info );
	delete m_pCurrentItem;
	m_pCurrentItem = 0;

	if( next )
		changeItem( next );
}

void KViewPresenter::changeItem( TQListViewItem * qitem )
{
	kdDebug( 4630 ) << k_funcinfo << endl;
	if( qitem->rtti() == 48294 )
	{
		ImageListItem * item = static_cast<ImageListItem*>( qitem );
		if( ! item->url().isEmpty() )
		{
			if( item->url().isLocalFile() && ! TQFile::exists( item->url().path() ) )
			{
				kdDebug( 4630 ) << "file doesn't exist. removed." << endl;
				ImageInfo info( item->url() );
				m_imagelist.remove( &info );
				if( m_pCurrentItem == item )
				{
					TQListViewItem * next = m_pCurrentItem->itemBelow() ? m_pCurrentItem->itemBelow() : m_pImageList->m_pListView->firstChild();
					if( next->rtti() != 48294 )
						kdWarning( 4630 ) << "unknown ListView item" << endl;
					else
						m_pCurrentItem = static_cast<ImageListItem*>( next );

					if( m_pCurrentItem == item )
						m_pCurrentItem = 0; // don't create a dangling pointer
					delete item;
					if( m_pCurrentItem )
						changeItem( m_pCurrentItem );
				}
				else
				{
					delete item;
					next();
				}
				return;
			}
			kdDebug( 4630 ) << "got url" << endl;
			makeCurrent( qitem );

			bool dontadd = m_bDontAdd;
			m_bDontAdd = true;
			m_pViewer->openURL( item->url() );
			m_bDontAdd = dontadd;
		}
		else
			kdWarning( 4630 ) << "got nothing" << endl;
	}
	else
		kdWarning( 4630 ) << "unknown ListView item" << endl;
}

void KViewPresenter::prev()
{
	kdDebug( 4630 ) << k_funcinfo << endl;
	if( m_pCurrentItem )
	{
		TQListViewItem * prev = m_pCurrentItem->itemAbove() ? m_pCurrentItem->itemAbove() : m_pImageList->m_pListView->lastItem();
		if( prev )
			changeItem( prev );
	}
}

void KViewPresenter::next()
{
	kdDebug( 4630 ) << k_funcinfo << endl;
	if( m_pCurrentItem )
	{
		TQListViewItem * next = m_pCurrentItem->itemBelow() ? m_pCurrentItem->itemBelow() : m_pImageList->m_pListView->firstChild();
		if( next )
			changeItem( next );
	}
}

void KViewPresenter::makeCurrent( TQListViewItem * item )
{
	if( m_pCurrentItem )
		m_pCurrentItem->setPixmap( 0, TQPixmap() );
	if( item->rtti() != 48294 )
		kdWarning( 4630 ) << "unknown ListView item" << endl;
	else
	{
		m_pCurrentItem = static_cast<ImageListItem*>( item );
		m_pCurrentItem->setPixmap( 0, KGlobal::iconLoader()->loadIcon( "1rightarrow", KIcon::Small ) );
		m_pImageList->m_pListView->ensureItemVisible( m_pCurrentItem );
	}
}

void KViewPresenter::slideshow( bool running )
{
	if( running )
	{
		m_pSlideshowTimer->start( m_pImageList->m_pInterval->value() );
		actionCollection()->action( "plugin_presenter_slideshow" )->setText( i18n( "Stop &Slideshow" ) );
		m_pImageList->m_pSlideshow->setText( i18n( "Stop &Slideshow" ) );
	}
	else
	{
		m_pSlideshowTimer->stop();
		actionCollection()->action( "plugin_presenter_slideshow" )->setText( i18n( "Start &Slideshow" ) );
		m_pImageList->m_pSlideshow->setText( i18n( "Start &Slideshow" ) );
	}
}

void KViewPresenter::setSlideshowInterval( int msec )
{
	if( m_pSlideshowTimer->isActive() )
		m_pSlideshowTimer->changeInterval( msec );
}

void KViewPresenter::shuffle()
{
	m_pImageList->noSort();
	KListView * listview = m_pImageList->m_pListView;
	TQPtrList<TQListViewItem> items;
	for( TQListViewItem * item = listview->firstChild(); item; item = listview->firstChild() )
	{
		items.append( item );
		listview->takeItem( item );
	}
	while( ! items.isEmpty() )
		listview->insertItem( items.take( KApplication::random() % items.count() ) );
}

void KViewPresenter::closeAll()
{
	m_imagelist.clear();
	m_pImageList->m_pListView->clear();
	m_pCurrentItem = 0;
	if( m_pViewer->closeURL() )
		m_pViewer->canvas()->clear();
}

void KViewPresenter::loadList()
{
	KURL url = KFileDialog::getOpenURL( ":load_list", TQString(), m_pImageList );
	if( url.isEmpty() )
		return;

	TQString tempfile;
	if( ! KIO::NetAccess::download( url, tempfile, m_pViewer->widget() ) )
	{
		KMessageBox::error( m_pImageList, i18n( "Could not load\n%1" ).tqarg( url.prettyURL() ) );
		return;
	}
	TQFile file( tempfile );
	if( file.open( IO_ReadOnly ) )
	{
		TQTextStream t( &file );
		if( t.readLine() == "[KView Image List]" )
		{
			//clear old image list
			closeAll();

			TQStringList list;
			if( ! t.eof() )
				m_pViewer->openURL( KURL( t.readLine() ) );
			while( ! t.eof() )
			{
				KURL url ( t.readLine() );
				ImageInfo * info = new ImageInfo( url );
				if( ! m_imagelist.tqcontains( info ) )
				{
					m_imagelist.inSort( info );
					( void )new ImageListItem( m_pImageList->m_pListView, url );
				}
				else
					delete info;
			}
		}
		else
		{
			KMessageBox::error( m_pImageList, i18n( "Wrong format\n%1" ).tqarg( url.prettyURL() ) );
		}
		file.close();
	}
	KIO::NetAccess::removeTempFile( tempfile );
}

void KViewPresenter::saveList()
{
	KURL url = KFileDialog::getSaveURL( ":save_list", TQString(), m_pImageList );

	if( url.isEmpty() )
		return;

	TQString tempfile;
	if( url.isLocalFile() )
		tempfile = url.path();
	else
	{
		KTempFile ktempf;
		tempfile = ktempf.name();
	}

	TQFile file( tempfile );
	if( file.open( IO_WriteOnly ) )
	{
		TQTextStream t( &file );
		// write header
		t << "[KView Image List]" << endl;
		TQListViewItem * item = m_pImageList->m_pListView->firstChild();
		while( item )
		{
			if( item->rtti() == 48294 )
				t << static_cast<ImageListItem*>( item )->url().url() << endl;
			item = item->itemBelow();
		}
		file.close();

		if( ! url.isLocalFile() )
		{
			KIO::NetAccess::upload( tempfile, url, m_pViewer->widget() );
			KIO::NetAccess::removeTempFile( tempfile );
		}
	}
}

// vim:sw=4:ts=4
#include "kviewpresenter.moc"
