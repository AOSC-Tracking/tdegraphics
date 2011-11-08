/*  This file is part of the KDE project
    Copyright (C) 2001-2003 Matthias Kretz <kretz@kde.org>

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

#include "kview.h"
#include "kimageviewer/viewer.h"
#include "kimageviewer/canvas.h"

#include <ksettings/dialog.h>
#include <ksettings/dispatcher.h>
#include <kurl.h>
#include <klibloader.h>
#include <kmessagebox.h>
#include <kparts/componentfactory.h>
#include <kparts/plugin.h>
#include <kparts/part.h>
#include <kapplication.h>
#include <kaction.h>
#include <kstdaction.h>
#include <kimageio.h>
#include <kfiledialog.h>
#include <klocale.h>
#include <twinmodule.h>
#include <kmenubar.h>
#include <kstatusbar.h>
#include <kdebug.h>
#include <kedittoolbar.h>
#include <kstdaccel.h>
#include <kprogress.h>

#include <tqimage.h>
#include <tqsize.h>
#include <tqscrollbar.h>
#include <tqstyle.h>
#include <tqclipboard.h>
#include <tqdragobject.h>
#include <tqvaluelist.h>
#include <tqtimer.h>
#include <tqdockarea.h>

#include <assert.h>
#include <kplugininfo.h>

KView::KView()
    : KParts::MainWindow( 0, "KView" )
	, m_pViewer( 0 )
	, m_pCanvas( 0 )
	, m_pWinModule( new KWinModule( TQT_TQOBJECT(this), KWinModule::INFO_DESKTOP ) )
	, m_bImageSizeChangedBlocked( false )
	, m_bFullscreen( false )
{
	KParts::ReadWritePart * part = KParts::ComponentFactory::createPartInstanceFromLibrary<KParts::ReadWritePart>(
			"libkviewviewer", this, "KViewViewer Widget", TQT_TQOBJECT(this), "KImageViewer Part" );
	if( part )
	{
		m_pViewer = static_cast<KImageViewer::Viewer *>( part );
		if( m_pViewer )
			m_pCanvas = m_pViewer->canvas();
	}

	kdDebug( 4600 ) << "m_pViewer = " << m_pViewer << endl;

	if( m_pCanvas )
	{
		setupActions( part );

		setCentralWidget( part->widget() );

		setStandardToolBarMenuEnabled( true );

		connect( part->widget(), TQT_SIGNAL( imageSizeChanged( const TQSize & ) ),
				TQT_SLOT( imageSizeChanged( const TQSize & ) ) );
		connect( part->widget(), TQT_SIGNAL( selectionChanged( const TQRect & ) ),
				TQT_SLOT( selectionChanged( const TQRect & ) ) );
		connect( part->widget(), TQT_SIGNAL( contextPress( const TQPoint & ) ),
				TQT_SLOT( contextPress( const TQPoint & ) ) );

		connect( TQApplication::tqclipboard(), TQT_SIGNAL( dataChanged() ),
				TQT_SLOT( clipboardDataChanged() ) );

		connect( m_pViewer, TQT_SIGNAL( started( KIO::Job * ) ),
				this, TQT_SLOT( jobStarted( KIO::Job * ) ) );
		connect( m_pViewer, TQT_SIGNAL( completed() ),
				this, TQT_SLOT( jobCompleted() ) );
		connect( m_pViewer, TQT_SIGNAL( completed( bool ) ),
				this, TQT_SLOT( jobCompleted( bool ) ) );
		connect( m_pViewer, TQT_SIGNAL( canceled( const TQString & ) ),
				this, TQT_SLOT( jobCanceled( const TQString & ) ) );
		connect( m_pViewer, TQT_SIGNAL( imageOpened( const KURL & ) ),
				m_paRecent, TQT_SLOT( addURL( const KURL & ) ) );

		connect( m_pCanvas->widget(), TQT_SIGNAL( cursorPos( const TQPoint & ) ), TQT_SLOT( cursorPos( const TQPoint & ) ) );

		m_paRecent->loadEntries( KGlobal::config() );
		if (!initialGeometrySet())
			resize(500, 350);
		readSettings();
		m_pViewer->widget()->installEventFilter( this );

		// reload configuration when it's changed by the conf dlg
		KSettings::Dispatcher::self()->registerInstance( instance(), TQT_TQOBJECT(this), TQT_SLOT( readSettings() ) );

		setPluginLoadingMode( LoadPluginsIfEnabled );
		createGUI( part );

		// create status bar (hidden by default)
		statusBar()->insertItem( "", STATUSBAR_SPEED_ID, 0, true );
		statusBar()->setItemFixed( STATUSBAR_SPEED_ID,
				8 + fontMetrics().width( i18n( "%1/s" ).tqarg( KIO::convertSize( 999000 ) ) ) );
		statusBar()->insertItem( "", STATUSBAR_CURSOR_ID, 0, true );
		statusBar()->setItemFixed( STATUSBAR_CURSOR_ID, 8 + fontMetrics().width( "8888, 8888" ) );
		statusBar()->insertItem( "", STATUSBAR_SIZE_ID, 0, true );
		statusBar()->setItemFixed( STATUSBAR_SIZE_ID, 8 + fontMetrics().width( "8888 x 8888" ) );

		statusBar()->insertItem( TQString(), STATUSBAR_SELECTION_ID );

		m_pProgressBar = new KProgress( statusBar() );
		m_pProgressBar->setFixedSize( 140, fontMetrics().height() );
		statusBar()->addWidget( m_pProgressBar, 0, true );
		m_pProgressBar->hide();

		setAutoSaveSettings();
		m_paShowMenubar->setChecked( ! menuBar()->isHidden() );

		// show progress info dialog if the statusbar is hidden
		m_pViewer->setProgressInfoEnabled( statusBar()->isHidden() );

		// set small minimum size
		setMinimumSize( 0, 0 );
	}
	else
	{
		KMessageBox::error( this, i18n( "An error occurred while loading the KViewViewer KPart. Check your installation." ) );
		TQTimer::singleShot( 0, kapp, TQT_SLOT( quit() ) );
	}
}

KView::~KView()
{
	saveSettings( KGlobal::config() );
	KGlobal::config()->sync();
}

void KView::load( const KURL & url )
{
	if( m_pViewer )
	{
		m_pViewer->openURL( url );
		if( url.isLocalFile() )
		{
			// XXX: this code is what
			//KRecentDirs::add( TQString::tqfromLatin1( ":load_image" ), url.directory() );
			// would do:
			TQString directory = url.directory();
			TQString key = TQString::tqfromLatin1( "load_image" );
			KConfig * config = KGlobal::config();

			config->setGroup( TQString::tqfromLatin1( "Recent Dirs" ) );
			TQStringList result = config->readPathListEntry( key );
			// make sure the dir is first in history
			result.remove( directory );
			result.prepend( directory );
			while( result.count() > 3 )
				result.remove( result.fromLast() );
			config->writePathEntry( key, result );
			config->sync();
		}
	}
}

void KView::loadFromStdin()
{
	if( m_pViewer )
	{
		TQFile file;
		file.open( IO_ReadOnly, stdin );
		TQImage image( file.readAll() );
		file.close();
		m_pViewer->newImage( image );
	}
}

TQSize KView::sizeForCentralWidgetSize( TQSize size )
{
	// add size of the dockareas
	kdDebug( 4600 ) << "sizeForCentralWidgetSize " << size << endl;
	size.rheight() += topDock()->height() + bottomDock()->height();
	size.rwidth()  += leftDock()->width() + rightDock()->width() - 2;
	kdDebug( 4600 ) << "added Dockareas:         " << size << endl;
	KStatusBar * sb = statusBar();
	size.rheight() += sb->isHidden() ? 0 : sb->height();
	kdDebug( 4600 ) << "added Statusbar:         " << size << endl;
	KMenuBar * mb = menuBar();
	if( ! mb->isHidden() )
	{
		size.rheight() += mb->heightForWidth( width() );
		if( tqstyle().tqstyleHint( TQStyle::SH_MainWindow_SpaceBelowMenuBar, this ) )
			size.rheight() += dockWindowsMovable() ? 1 : 2;
	}
	kdDebug( 4600 ) << "added Menubar:           " << size << endl;
	return size;
}

bool KView::queryClose()
{
	return m_pViewer->closeURL();
}

void KView::saveProperties( KConfig * /*config*/ )
{
	// save session data:
	// What URL is currently open
	// somehow the plugins have to get a chance to store their data
}

void KView::readProperties( KConfig * /*config*/ )
{
	// read session data
}

void KView::saveSettings( KConfig * config )
{
	// write settings to config/kviewrc
	kdDebug( 4600 ) << k_funcinfo << endl;
	m_paRecent->saveEntries( config );
}

void KView::readSettings() // KConfig * config )
{
	// read settings from config/kviewrc
	kdDebug( 4600 ) << k_funcinfo << endl;
	KConfigGroup cfgGroup( KGlobal::config(), "KView General" );
	m_nResizeMode = cfgGroup.readNumEntry( "Resize Mode", 2 );
	kdDebug( 4600 ) << "m_nResizeMode = " << m_nResizeMode << endl;
	loadPlugins();
}

bool KView::eventFilter( TQObject * obj, TQEvent * ev )
{
	if( TQT_BASE_OBJECT(obj) == TQT_BASE_OBJECT(m_pViewer->widget()) && ev->type() == TQEvent::Resize )
	{
		if( m_nResizeMode == ResizeImage )
			handleResize();
	}
	return KParts::MainWindow::eventFilter( obj, ev );
}

void KView::imageSizeChanged( const TQSize & /*size*/ )
{
	TQSize size = m_pCanvas->imageSize();
	statusBar()->changeItem( TQString( "%1 x %2" ).tqarg( size.width() ).tqarg( size.height() ), STATUSBAR_SIZE_ID );
	handleResize();
}

void KView::selectionChanged( const TQRect & rect )
{
	kdDebug( 4600 ) << k_funcinfo << rect << endl;
	if( rect.isNull() )
		statusBar()->changeItem( TQString(), STATUSBAR_SELECTION_ID );
	else
		statusBar()->changeItem( TQString( "%1, %2 - %3 x %4" ).tqarg( rect.x() ).tqarg( rect.y() ).tqarg( rect.width() ).tqarg( rect.height() ), STATUSBAR_SELECTION_ID );
	action( "crop" )->setEnabled( ! rect.isNull() );
}

void KView::contextPress( const TQPoint & point )
{
	TQPopupMenu * pop = ( TQPopupMenu* )factory()->container( "popupmenu", this );
	pop->popup( point );
}

void KView::slotOpenFile()
{
	KURL url = KFileDialog::getImageOpenURL( ":load_image", this );
	load( url );
}

void KView::slotOpenRecent( const KURL & url )
{
	load( url );
}

void KView::slotClose()
{
	if( m_pViewer->closeURL() )
		m_pCanvas->clear();
}

void KView::slotCopy()
{
	TQClipboard *cb = TQApplication::tqclipboard();
	cb->setSelectionMode( false );

	TQRect selectarea = m_pCanvas->selection();
	if( selectarea.isEmpty() )
	{
		kdDebug( 4600 ) << k_funcinfo << " copy whole image" << endl;
		cb->setImage( *m_pCanvas->image() );
	}
	else
	{
		kdDebug( 4600 ) << k_funcinfo << " copy selected area of image" << endl;
		cb->setImage( m_pCanvas->image()->copy( selectarea ) );
	}
}

void KView::slotPaste()
{
	// Get TQImage from clipboard and create a new image.
	TQClipboard *cb = TQApplication::tqclipboard();
    TQImage img = cb->image();
	if( ! img.isNull() )
		m_pViewer->newImage( img );
}

void KView::slotCrop()
{
	TQRect selectarea = m_pCanvas->selection();
	kdDebug( 4600 ) << "Crop following area: " << selectarea.x() << ", " << selectarea.y() << ", " << selectarea.width() << ", " << selectarea.height() << endl;

	if( selectarea.isNull() )
		return;
	const TQImage * origimg = m_pCanvas->image();
	if( origimg == 0 )
		return;

	m_pCanvas->setImage( origimg->copy( selectarea ) );
	m_pViewer->setModified( true );
}

void KView::slotUpdateFullScreen( bool set )
{
	m_bFullscreen = set;
	if( set )
	{ // switch to FullScreen mode
		saveMainWindowSettings( KGlobal::config(), "nonFullScreen MainWindow" );
		showFullScreen();
		applyMainWindowSettings( KGlobal::config(), "FullScreen MainWindow" );
		m_paShowMenubar->setChecked( ! menuBar()->isHidden() );
	}
	else
	{ // leave FullScreen mode
		saveMainWindowSettings( KGlobal::config(), "FullScreen MainWindow" );
		showNormal();
		applyMainWindowSettings( KGlobal::config(), "nonFullScreen MainWindow" );
		m_paShowMenubar->setChecked( ! menuBar()->isHidden() );
		handleResize();
	}
}

void KView::slotToggleMenubar()
{
	if( menuBar()->isVisible() )
		menuBar()->hide();
	else
		menuBar()->show();
	handleResize();
}

void KView::slotPreferences()
{
	// construct KCD for KView app
	static KSettings::Dialog * dlg = 0;
	if( ! dlg )
	{
		dlg = new KSettings::Dialog( this );
		//dlg = new KConfigureDialog( KConfigureDialog::Configurable, this );
		//dlg->addPluginInfos( KPluginInfo::fromKPartsInstanceName( instance()->instanceName(), KGlobal::config(), "KParts Plugins" ) );
	}
	dlg->show();
}

void KView::slotConfigureToolbars()
{
	saveMainWindowSettings( KGlobal::config(), "MainWindow" );
	KEditToolbar dlg( factory() );
	connect( &dlg, TQT_SIGNAL( newToolbarConfig() ), TQT_SLOT( slotNewToolbarConfig() ) );
	dlg.exec();
}

void KView::slotNewToolbarConfig()
{
	applyMainWindowSettings( KGlobal::config(), "MainWindow" );
}

void KView::reloadConfig()
{
	readSettings(); //KGlobal::config() );
}

void KView::enableAction( const char * name, bool b )
{
	KAction * a = actionCollection()->action( name );
	if( a )
		a->setEnabled( b );
	else
		kdWarning( 4600 ) << k_funcinfo << " unknown action" << endl;
}

void KView::clipboardDataChanged()
{
	TQClipboard * cb = TQApplication::tqclipboard();
	cb->setSelectionMode( false );
	bool hasImage = TQImageDrag::canDecode( cb->data( TQClipboard::Clipboard ) );
	m_paPaste->setEnabled( hasImage );
}

void KView::jobStarted( KIO::Job * job )
{
	if( job )
	{
		connect( job, TQT_SIGNAL( percent( KIO::Job *, unsigned long ) ), this, TQT_SLOT( loadingProgress( KIO::Job *, unsigned long ) ) );
		connect( job, TQT_SIGNAL( speed(   KIO::Job *, unsigned long ) ), this, TQT_SLOT( speedProgress(   KIO::Job *, unsigned long ) ) );
		//connect( job, TQT_SIGNAL( infoMessage( KIO::Job *, const TQString & ) ), this, TQT_SLOT() );
		loadingProgress( job, 0 );
		speedProgress( job, 0 );
	}
}

void KView::jobCompleted()
{
	jobCompleted( false );
}

void KView::jobCompleted( bool /*hasPending*/ )
{
	loadingProgress( 0, 101 );
	statusBar()->changeItem( "", STATUSBAR_SPEED_ID );
}

void KView::jobCanceled( const TQString & errorMsg )
{
	statusBar()->message( errorMsg );
	jobCompleted();
}

void KView::loadingProgress( KIO::Job *, unsigned long percent )
{
	if( percent > 100 )
	{
		m_pProgressBar->hide();
		return;
	}

	if( ! m_pProgressBar->isVisible() )
		m_pProgressBar->show();

	m_pProgressBar->setValue( percent );
}

void KView::speedProgress( KIO::Job *, unsigned long bytesPerSecond )
{
	TQString sizeStr;

	if( bytesPerSecond > 0 )
		sizeStr = i18n( "%1/s" ).tqarg( KIO::convertSize( bytesPerSecond ) );
	else
		sizeStr = i18n( "Stalled" );

	statusBar()->changeItem( sizeStr, STATUSBAR_SPEED_ID );
}

void KView::slotSetStatusBarText( const TQString & msg )
{
	kdDebug( 4600 ) << k_funcinfo << endl;
	statusBar()->message( msg );
	if( statusBar()->isHidden() )
		KMessageBox::information( this, msg );
}

void KView::cursorPos( const TQPoint & pos )
{
	statusBar()->changeItem( TQString( "%1, %2" ).tqarg( pos.x() ).tqarg( pos.y() ), STATUSBAR_CURSOR_ID );
}

void KView::setupActions( TQObject * partobject )
{
	// File
	KStdAction::open( TQT_TQOBJECT(this), TQT_SLOT( slotOpenFile() ), actionCollection() );
	m_paRecent = KStdAction::openRecent( TQT_TQOBJECT(this), TQT_SLOT( slotOpenRecent( const KURL & ) ), actionCollection() );
	KAction * aClose = KStdAction::close( TQT_TQOBJECT(this), TQT_SLOT( slotClose() ), actionCollection() );
	aClose->setEnabled( false );
	connect( m_pViewer->widget(), TQT_SIGNAL( hasImage( bool ) ), aClose, TQT_SLOT( setEnabled( bool ) ) );

	TQObject * extension = partobject->child( 0, "KParts::BrowserExtension", false );
	if( extension )
	{
		TQStrList slotNames = extension->tqmetaObject()->slotNames();
		if( slotNames.contains( "print()" ) )
			KStdAction::print( extension, TQT_SLOT( print() ), actionCollection(), "print" );
		if( slotNames.contains( "del()" ) )
			( void )new KAction( i18n( "&Delete" ), "editdelete", SHIFT+Key_Delete,
								 extension, TQT_SLOT( del() ), actionCollection(), "del" );
		connect( extension, TQT_SIGNAL( enableAction( const char *, bool ) ), TQT_SLOT( enableAction( const char *, bool ) ) );
	}
	KStdAction::quit( TQT_TQOBJECT(this), TQT_SLOT( close() ), actionCollection() );

	// Edit
	KAction * aCopy = KStdAction::copy( TQT_TQOBJECT(this), TQT_SLOT( slotCopy() ), actionCollection() );
	aCopy->setEnabled( false );
	connect( m_pViewer->widget(), TQT_SIGNAL( hasImage( bool ) ), aCopy, TQT_SLOT( setEnabled( bool ) ) );
	m_paPaste = KStdAction::paste( TQT_TQOBJECT(this), TQT_SLOT( slotPaste() ), actionCollection() );
	clipboardDataChanged(); //enable or disable paste
	KAction * aCrop = new KAction( i18n( "Cr&op" ), Key_C, TQT_TQOBJECT(this), TQT_SLOT( slotCrop() ), actionCollection(), "crop" );
	aCrop->setEnabled( false );

	KAction * aReload = new KAction( i18n( "&Reload" ), "reload", KStdAccel::shortcut( KStdAccel::Reload ), partobject,
			TQT_SLOT( reload() ), actionCollection(), "reload" );
	aReload->setEnabled( false );
	connect( m_pViewer->widget(), TQT_SIGNAL( hasImage( bool ) ), aReload, TQT_SLOT( setEnabled( bool ) ) );

	// Settings
	m_paShowMenubar = KStdAction::showMenubar( TQT_TQOBJECT(this), TQT_SLOT( slotToggleMenubar() ), actionCollection() );
	createStandardStatusBarAction();
	m_paShowStatusBar = ::tqqt_cast<KToggleAction*>( action( "options_show_statusbar" ) );
	if( m_paShowStatusBar )
		connect( m_paShowStatusBar, TQT_SIGNAL( toggled( bool ) ), TQT_SLOT( statusbarToggled( bool ) ) );
	m_paShowFullScreen = KStdAction::fullScreen( 0, 0, actionCollection(), this );
	connect( m_paShowFullScreen, TQT_SIGNAL( toggled( bool )), this, TQT_SLOT( slotUpdateFullScreen( bool )));
	KStdAction::preferences( TQT_TQOBJECT(this), TQT_SLOT( slotPreferences() ), actionCollection() );
	KStdAction::keyBindings(guiFactory(), TQT_SLOT(configureShortcuts()), 
actionCollection());
	KStdAction::configureToolbars( TQT_TQOBJECT(this), TQT_SLOT( slotConfigureToolbars() ), actionCollection() );
}

void KView::handleResize()
{
	if( m_bImageSizeChangedBlocked )
		return;
	m_bImageSizeChangedBlocked = true;
	setUpdatesEnabled( false );
	switch( m_nResizeMode )
	{
		case ResizeWindow:
			fitWindowToImage();
			fitWindowToImage();
			break;
		case ResizeImage:
			m_pCanvas->boundImageTo( m_pViewer->widget()->size() );
			break;
		case BestFit:
			TQSize imageSize = m_pCanvas->imageSize();
			if( imageSize.isEmpty() )
				return;

			// Compare the image size and the maximum available space in the
			// display canvas i.e. will the image fit without resizing ?
			TQSize maxCanvas = maxCanvasSize();
			if( ( maxCanvas.height() >= imageSize.height() )
					&& ( maxCanvas.width() >= imageSize.width() ) )
			{
				//Image can be displayed at full size
				m_pCanvas->setZoom( 1.0 );
			}
			else
			{
				// Image is too big for the available canvas.
				m_pCanvas->boundImageTo( maxCanvas );
			}
			// We assume the displayed image size has changed and we must resize
			// the window around it (using the code for ResizeWindow).
			fitWindowToImage();
			fitWindowToImage();
			break;
	}
	setUpdatesEnabled( true );
	m_bImageSizeChangedBlocked = false;
}

void KView::fitWindowToImage()
{
	if( m_bFullscreen ) // don't do anything in fullscreen mode
		return;

	bool centeredOrig = m_pCanvas->centered();
	m_pCanvas->setCentered( false );

	TQSize imagesize = m_pCanvas->currentSize();
	if( imagesize.isEmpty() )
		return;

	TQSize winsize = sizeForCentralWidgetSize( imagesize );
	TQRect workarea = m_pWinModule->workArea();

	TQScrollBar * sb = new TQScrollBar( Qt::Horizontal, this );
	int scrollbarwidth = sb->height();
	delete sb;

	if( winsize.width() > workarea.width() )
	{
		winsize.setWidth( workarea.width() );
		winsize.rheight() += scrollbarwidth;
		if( winsize.height() > workarea.height() )
			winsize.setHeight( workarea.height() );
	}
	else if( winsize.height() > workarea.height() )
	{
		winsize.setHeight( workarea.height() );
		winsize.rwidth() += scrollbarwidth;
		if( winsize.width() > workarea.width() )
			winsize.setWidth( workarea.width() );
	}

	TQRect winrect( tqgeometry() );
	winrect.setSize( winsize );

	int xdiff = winrect.x() + winrect.width()  - workarea.x() - workarea.width();
	int ydiff = winrect.y() + winrect.height() - workarea.y() - workarea.height();

	if( xdiff > 0 )
	{
		winrect.rLeft() -= xdiff;
		winrect.rRight() -= xdiff;
	}
	if( ydiff > 0 )
	{
		winrect.rTop() -= ydiff;
		winrect.rBottom() -= ydiff;
	}

	setGeometry( winrect );

	m_pCanvas->setCentered( centeredOrig );
}

TQSize KView::barSize( int mainwinwidth, BarSizeFrom from )
{
	int height = 0;
	int width = 0;
	if( toolBar()->isVisibleTo( this ) )
	{
		switch( toolBar()->barPos() )
		{
			case KToolBar::Top:
			case KToolBar::Bottom:
				height += toolBar()->height();
				break;
			case KToolBar::Left:
			case KToolBar::Right:
				width += toolBar()->width();
				break;
			case KToolBar::Flat:
				height += kapp->tqstyle().tqpixelMetric( TQStyle::PM_DockWindowHandleExtent );
				break;
			case KToolBar::Floating:
				break;
			case KToolBar::Unmanaged:
				break;
		}
	}
	if( menuBar()->isVisibleTo( this ) && ( ! menuBar()->isTopLevelMenu() ) )
		height += menuBar()->heightForWidth( mainwinwidth + ( ( from == FromImageSize ) ? width : 0 ) );
	if( statusBar()->isVisibleTo( this ) )
		height += statusBar()->height();

	return TQSize( width, height );
}

TQSize KView::maxCanvasSize()
{
	TQSize workarea = m_pWinModule->workArea().size();
	TQSize framesize = frameSize() - size();
	TQSize maxcanvassize = workarea - framesize - barSize( workarea.width() - framesize.width(), FromWidgetSize );
	kdDebug( 4600 ) << "maxcanvassize = " << maxcanvassize.width() << "x" << maxcanvassize.height() << endl;
	return maxcanvassize;
}


void KView::loadPlugins()
{
	createGUI( 0 );
	createShellGUI( false );
	createGUI( m_pViewer );
}

void KView::statusbarToggled( bool sbvisible )
{
	kdDebug( 4600 ) << k_funcinfo << sbvisible << endl;
	m_pViewer->setProgressInfoEnabled( !sbvisible );
}

// vim:sw=4:ts=4

#include "kview.moc"

