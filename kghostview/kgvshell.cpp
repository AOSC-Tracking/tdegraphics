/**
 * Copyright (C) 2000-2002 the KGhostView authors. See file AUTHORS.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <assert.h>
#include <stdlib.h>

#include <tdeaction.h>
#include <tdeapplication.h>
#include <tdefiledialog.h>
#include <kiconloader.h>
#include <klibloader.h>
#include <tdelocale.h>
#include <tdemessagebox.h>
#include <kstandarddirs.h>
#include <kstatusbar.h>
#include <kstdaction.h>
#include <tdestdaccel.h>
#include <tdetempfile.h>
#include <tdemenubar.h>
#include <kedittoolbar.h>
#include <kdebug.h>

#include <kicontheme.h>
#include <tdeglobal.h>
#include <tdepopupmenu.h>
#include <tdeparts/componentfactory.h>

#include <twin.h>

#include <tqcursor.h>

#include "kgv_miniwidget.h"
#include "kgv_view.h"
#include "kgvpageview.h"
#include "displayoptions.h"
#include "fullscreenfilter.h"

#undef Always  // avoid X11/TQt namespace clash
#include "kgvshell.moc"

//TODO -- disable GUI when no file
//TODO -- don't stay open when no file, go directly to KFileDialog

KGVShell::KGVShell() :
    _tmpFile( 0 )
{
    m_gvpart = KParts::ComponentFactory::createPartInstanceFromLibrary< KGVPart >( "libkghostviewpart", this, "kgvpart",
                                                                                   TQT_TQOBJECT(this), "kgvpart" );

    /*---- File -----------------------------------------------------------*/
    openact =
	    KStdAction::open( TQT_TQOBJECT(this), TQT_SLOT( slotFileOpen() ),
			      actionCollection() );
    recent =
	    KStdAction::openRecent( TQT_TQOBJECT(this), TQT_SLOT( openURL( const KURL& ) ),
				    actionCollection() );
	    KStdAction::print( m_gvpart->document(), TQT_SLOT( print() ),
			       actionCollection() );
    (void)
	    KStdAction::quit( TQT_TQOBJECT(this), TQT_SLOT( slotQuit() ), actionCollection() );

    /*---- View -----------------------------------------------------------*/
            new TDEAction( i18n(  "&Reload" ), "reload",
		    TDEStdAccel::shortcut( TDEStdAccel::Reload ),
		    m_gvpart, TQT_SLOT(  reloadFile() ),
		    actionCollection(), "reload" );
	    new TDEAction( i18n( "&Maximize" ), Key_M, TQT_TQOBJECT(this),
			 TQT_SLOT( slotMaximize() ), actionCollection(),
			 "maximize");
    _showMenuBarAction = KStdAction::showMenubar( TQT_TQOBJECT(this), TQT_SLOT( slotShowMenubar() ), actionCollection() );

    /*---- Settings -------------------------------------------------------*/
#if TDE_VERSION >= TDE_MAKE_VERSION(3,1,90)
    createStandardStatusBarAction();
#endif
    setAutoSaveSettings();
    setStandardToolBarMenuEnabled(true);
#if TDE_VERSION >= TDE_MAKE_VERSION(3,1,90)
    m_fullScreenAction = KStdAction::fullScreen( TQT_TQOBJECT(this), TQT_SLOT( slotUpdateFullScreen() ), actionCollection(), this );
#else
    m_fullScreenAction = new TDEToggleAction( this, TQT_SLOT( slotUpdateFullScreen() ) );
#endif
    KStdAction::configureToolbars( TQT_TQOBJECT(this), TQT_SLOT( slotConfigureToolbars() ), actionCollection() );
    KStdAction::keyBindings(guiFactory(), TQT_SLOT(configureShortcuts()),
actionCollection());

    //_popup = new TDEPopupMenu( i18n( "Full Screen Options" ), this, "rmb popup" );
    _popup = new TDEPopupMenu( this, "rmb popup" );
    _popup->insertTitle( i18n( "Full Screen Options" ) );
    m_fullScreenAction->plug( _popup );
    _showMenuBarAction->plug( _popup );

    m_fsFilter = new FullScreenFilter( *this );

    // Just save them automatically is destructor. (TODO: of kgv_view!)
    //KStdAction::saveOptions ( this, TQT_SLOT (slotWriteSettings()), actionCollection());

    setXMLFile( "kghostviewui.rc" );

    // We could, at the user's option, make this connection and kghostview
    // will always resize to fit the width of the page.  But, for now,
    // let's not.
    // connect ( m_gvpart->widget(), TQT_SIGNAL (sizeHintChanged()),	    this, TQT_SLOT (slotResize ()) );

    setCentralWidget( m_gvpart->widget() );
    createGUI( m_gvpart );

    connect( m_gvpart->pageView(), TQT_SIGNAL( rightClick() ),TQT_SLOT( slotRMBClick() ) );
    connect( m_gvpart, TQT_SIGNAL( canceled(const TQString&) ),TQT_SLOT( slotReset() ) );
    connect( m_gvpart, TQT_SIGNAL( completed() ), TQT_SLOT( slotDocumentState() ) );

    if (!initialGeometrySet())
        resize(640,400);

    readSettings();
    stateChanged( "initState" );

    // Make sure the view has the keyboard focus.
    m_gvpart->widget()->setFocus();
}

KGVShell::~KGVShell()
{
    writeSettings();

    if( _tmpFile )
    {
	_tmpFile->setAutoDelete( true );
	delete _tmpFile;
	_tmpFile = 0;
    }

    // delete m_gvpart;
}

void
KGVShell::slotQuit()
{
    kapp->closeAllWindows();
}

void KGVShell::slotShowMenubar()
{
    if ( _showMenuBarAction->isChecked() ) menuBar()->show();
    else menuBar()->hide();
}

void
KGVShell::setDisplayOptions( const DisplayOptions& options )
{
    m_gvpart->setDisplayOptions( options );
}

void KGVShell::slotReset()
{
    kdDebug( 4500 ) << "KGVShell::slotReset()" << endl;
    stateChanged( "initState" );
}

void
KGVShell::readProperties( TDEConfig *config )
{
    KURL url = KURL::fromPathOrURL( config->readPathEntry( "URL" ) );
    if ( url.isValid() ) {
        openURL( url );
	DisplayOptions options;
	if ( DisplayOptions::fromString( options, config->readEntry( "Display Options" ) ) ) m_gvpart->setDisplayOptions( options );
    }
}

void
KGVShell::saveProperties( TDEConfig* config )
{
    config->writePathEntry( "URL", m_gvpart->url().prettyURL() );
    config->writeEntry( "Display Options", DisplayOptions::toString( m_gvpart->miniWidget()->displayOptions() ) );
}

void
KGVShell::readSettings()
{
    recent->loadEntries( TDEGlobal::config() );
    TQStringList items = recent->items();

// Code copied from kviewshell.cpp:
// Constant source of annoyance in KDVI < 1.0: the 'recent-files'
// menu contains lots of files which don't exist (any longer). Thus,
// we'll sort out the non-existent files here.

    for ( TQStringList::Iterator it = items.begin(); it != items.end(); ++it ) {
        KURL url(*it);
        if (url.isLocalFile()) {
            TQFileInfo info(url.path());
            if (!info.exists())
                recent->removeURL(url);
        }
    }

    applyMainWindowSettings(TDEGlobal::config(), "MainWindow");

    TDEGlobal::config()->setDesktopGroup();
    bool fullScreen = TDEGlobal::config()->readBoolEntry( "FullScreen", false );
    setFullScreen( fullScreen );
    _showMenuBarAction->setChecked( menuBar()->isVisible() );
}

void
KGVShell::writeSettings()
{
    saveMainWindowSettings(TDEGlobal::config(), "MainWindow");

    recent->saveEntries( TDEGlobal::config() );

    TDEGlobal::config()->setDesktopGroup();
    TDEGlobal::config()->writeEntry( "FullScreen", m_fullScreenAction->isChecked());

    TDEGlobal::config()->sync();
}

void
KGVShell::openURL( const KURL & url )
{
    if( m_gvpart->openURL( url ) ) recent->addURL (url);
}

void
KGVShell::openStdin()
{
    if( _tmpFile )
    {
	_tmpFile->setAutoDelete( true );
	delete _tmpFile;
    }

    _tmpFile = new KTempFile;
    _tmpFile->setAutoDelete( true );

    if( _tmpFile->status() != 0 ) {
	KMessageBox::error( this,
		i18n( "Could not create temporary file: %1" )
		.arg( strerror( _tmpFile->status() ) ) );
	return;
    }

    TQByteArray buf( BUFSIZ );
    int read = 0, wrtn = 0;
    while( ( read = fread( buf.data(), sizeof(char), buf.size(), stdin ) )
	    > 0 ) {
	wrtn = _tmpFile->file()->writeBlock( buf.data(), read );
	if( read != wrtn )
	    break;
	kapp->processEvents();
    }

    if( read != 0 ) {
	KMessageBox::error( this,
		i18n( "Could not open standard input stream: %1" )
		.arg( strerror( errno ) ) );
	return;
    }

    _tmpFile->close();

    if( m_gvpart->openURL( KURL::fromPathOrURL( _tmpFile->name() ) ) ) setCaption( "stdin" );
}

void KGVShell::slotFileOpen()
{
    KURL url = KFileDialog::getOpenURL( cwd, i18n(
		    "*.ps *.ps.bz2 *.ps.gz *.eps *.eps.gz *.pdf|All Document Files\n"
		    "*.ps *.ps.bz2 *.ps.gz|PostScript Files\n"
		    "*.pdf *.pdf.gz *.pdf.bz2|Portable Document Format (PDF) Files\n"
		    "*.eps *.eps.gz *.eps.bz2|Encapsulated PostScript Files\n"
		    "*|All Files" ) );

    if( !url.isEmpty() ) {
	openURL( url );
    }
}

void KGVShell::slotDocumentState()
{
    stateChanged( "documentState" );
}


void KGVShell::slotMaximize()
{
    kdDebug(4500) << "KGVShell::slotMaximize()" << endl;
    KWin::setState( winId(), NET::MaxHoriz | NET::MaxVert );
    // If we do it now, it comes to nothing since it would work
    // on the current (non-maximized) size
    TQTimer::singleShot( 800, m_gvpart, TQT_SLOT( slotFitToPage() ) );
}

void KGVShell::slotResize()
{
    resize( m_gvpart->pageView()->sizeHint().width(), height() );
}

void KGVShell::setFullScreen( bool useFullScreen )
{
    if( useFullScreen )
        showFullScreen();
    else if( isFullScreen())
        showNormal();
}

void KGVShell::slotUpdateFullScreen()
{
    if( m_fullScreenAction->isChecked())
    {
	menuBar()->hide();
	statusBar()->hide();
	toolBar()->hide();
	m_gvpart->updateFullScreen( true );
	showFullScreen();
	kapp->installEventFilter( m_fsFilter );
	if ( m_gvpart->document()->isOpen() )
		m_gvpart->slotFitToPage();
    }
    else
    {
	kapp->removeEventFilter( m_fsFilter );
	m_gvpart->updateFullScreen( false );
	menuBar()->show();
#if TDE_VERSION >= TDE_MAKE_VERSION(3,1,90)
	TDEToggleAction *statusbarAction = dynamic_cast<TDEToggleAction *>(actionCollection()->action(KStdAction::name(KStdAction::ShowStatusbar)));
	assert( statusbarAction );
	if (statusbarAction->isChecked()) statusBar()->show();
#endif
	toolBar()->show();
	showNormal();
    }
}

void KGVShell::slotConfigureToolbars()
{
    saveMainWindowSettings( TDEGlobal::config(), "MainWindow" );
    KEditToolbar dlg( factory() );
    connect(&dlg,TQT_SIGNAL(newToolbarConfig()),this,TQT_SLOT(slotNewToolbarConfig()));
    dlg.exec();
}

void KGVShell::slotNewToolbarConfig()
{
    applyMainWindowSettings( TDEGlobal::config(), "MainWindow" );
}

void KGVShell::slotRMBClick()
{
    _popup->exec( TQCursor::pos() );
}


// vim:sw=4:sts=4:ts=8:noet
