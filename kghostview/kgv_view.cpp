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

#include <tqfile.h>
#include <tqfileinfo.h>
#include <tqframe.h>
#include <tqtable.h>
#include <tqlayout.h>
#include <tqregexp.h>

#include <tdeaction.h>
#include <tdeapplication.h>
#include <tdeconfig.h>
#include <kdebug.h>
#include <kdirwatch.h>
#include <kglobalsettings.h>
#include <kiconloader.h>
#include <kinstance.h>
#include <klocale.h>
#include <tdepopupmenu.h>
#include <kstdaction.h>
#include <tdestdaccel.h>
#include <ktempfile.h>
#include <tdeio/scheduler.h>
#include <tdeaboutdata.h>


#include "kgv_view.h"
#include "kgv_miniwidget.h"
#include "kgvconfigdialog.h"
#include "kgvdocument.h"
#include "kgvpagedecorator.h"
#include "kgvpageview.h"
#include "kgvmainwidget.h"
#include "kpswidget.h"
#include "kgvfactory.h"
#include "logwindow.h"
#include "martdelist.h"
#include "scrollbox.h"
#include "version.h"
#include "configuration.h"

#include <cmath>

namespace KGV {
    /*
     * This is because TQt's iterators
     * are not standard iterators bc of missing typedefs,
     * so they are only *almost* STL compatible
     */
    template <typename T>
    unsigned distance( T a, T b ) {
	unsigned res = 0;
	while ( a != b ) {
	    ++res;
	    ++a;
	}
	return res;
    }
}

KGVPart::KGVPart( TQWidget* parentWidget, const char*,
                  TQObject* parent, const char* name,
                  const TQStringList &args ) :
    KParts::ReadOnlyPart( parent, name ),
    _fitTimer( new TQTimer( this ) ),
    _job( 0 ),
    _mimetypeScanner( 0 ),
    _dirtyHandler( new TQTimer( this ) ),
    _isGuiInitialized( false ),
    _isFileDirty( false ),
    _stickyOptions( false ),
    _embeddedInKGhostView( !args.contains( "KParts::ReadOnlyPart" ) ),
    _customZoomIndex( -1 )
{
    setInstance( KGVFactory::instance() );

    // Don't show the progress info dialog if we're embedded in Konqueror.
    setProgressInfoEnabled( !args.contains( "Browser/View") );

    _document = new KGVDocument( this );
    connect( _document, TQT_SIGNAL( fileChangeFailed() ),
	     this, TQT_SLOT( slotCancelWatch() ) );
    connect( _document, TQT_SIGNAL( completed() ),
	     this, TQT_SLOT( slotOpenFileCompleted() ) );
    connect( _document, TQT_SIGNAL( canceled( const TQString& ) ),
	     this, TQT_SIGNAL( canceled( const TQString& ) ) );

    _fileWatcher = new KDirWatch( this );
    connect( _fileWatcher, TQT_SIGNAL( dirty( const TQString& ) ),
	     this, TQT_SLOT( slotFileDirty( const TQString& ) ) );
    connect( _dirtyHandler, TQT_SIGNAL( timeout() ),
	     this, TQT_SLOT( slotDoFileDirty() ) );

    // Setup main widget
    _mainWidget = new KGVMainWidget( parentWidget );
    _mainWidget->setFocusPolicy( TQ_StrongFocus );
    _mainWidget->installEventFilter( this );
    _mainWidget->setAcceptDrops( true );
    connect( _mainWidget, TQT_SIGNAL( spacePressed() ),
             this, TQT_SLOT( slotReadDown() ) );
    connect( _mainWidget, TQT_SIGNAL( urlDropped( const KURL& ) ),
	    this, TQT_SLOT( openURL( const KURL& ) ) );

    TQHBoxLayout* hlay = new TQHBoxLayout( _mainWidget, 0, 0 );
    TQVBoxLayout* vlay = new TQVBoxLayout( hlay );

    const int PAGELIST_WIDTH = 75;

    _scrollBox = new ScrollBox( _mainWidget , "scrollbox" );
    _scrollBox->setFixedWidth( PAGELIST_WIDTH );
    _scrollBox->setMinimumHeight( PAGELIST_WIDTH );
    vlay->addWidget( _scrollBox );

    _divider = new TQFrame( _mainWidget, "divider" );
    _divider->setFrameStyle( TQFrame::Panel | TQFrame::Raised );
    _divider->setLineWidth( 1 );
    _divider->setMinimumWidth( 3 );
    hlay->addWidget( _divider );

    _pageView = new KGVPageView( _mainWidget, "pageview" );
    _pageView->viewport()->setBackgroundMode( TQWidget::PaletteMid );
    hlay->addWidget( _pageView, 1 );
    _mainWidget->setFocusProxy( _pageView );
    setWidget( _mainWidget );

    _pageDecorator = new KGVPageDecorator( _pageView->viewport() );
    _pageDecorator->hide();


    _psWidget = new KPSWidget( _pageDecorator );
    _psWidget->readSettings();
    _pageView->setPage( _pageDecorator );
    connect( _psWidget, TQT_SIGNAL( output( char*, int ) ),
             this, TQT_SLOT( slotGhostscriptOutput( char*, int ) ) );

    connect( _psWidget, TQT_SIGNAL( ghostscriptError( const TQString& ) ),
             this, TQT_SLOT( slotGhostscriptError( const TQString& ) ) );


    _logWindow = new LogWindow( i18n( "Ghostscript Messages" ), _mainWidget, "logwindow" );
    _showLogWindow = false;

    connect( _logWindow, TQT_SIGNAL( configureGS() ), TQT_SLOT( slotConfigure() ) );

    _docManager = new KGVMiniWidget( this );
    _docManager->setPSWidget( _psWidget );
    _docManager->setDocument( document() );

    _markList = new MarkList( _mainWidget, "martdelist", _docManager );
    _markList->setFixedWidth( PAGELIST_WIDTH );
    vlay->addWidget( _markList, 1 );
    connect( TQT_TQOBJECT(_markList), TQT_SIGNAL( contextMenuRequested ( int, int, const TQPoint& ) ),
             this, TQT_SLOT( showPopup( int, int, const TQPoint& ) ) );


    connect( TQT_TQOBJECT(_markList), TQT_SIGNAL( selected( int ) ),
	     _docManager, TQT_SLOT( goToPage( int ) ) );
    connect( _docManager, TQT_SIGNAL( newPageShown( int ) ),
	     TQT_TQOBJECT(_markList), TQT_SLOT( select( int ) ) );
    connect( _docManager, TQT_SIGNAL( setStatusBarText( const TQString& ) ),
	     this, TQT_SIGNAL( setStatusBarText( const TQString& ) ) );
    connect( _scrollBox, TQT_SIGNAL( valueChangedRelative( int, int ) ),
	     _pageView, TQT_SLOT( scrollBy( int, int ) ) );
    connect( _pageView, TQT_SIGNAL( pageSizeChanged( const TQSize& ) ),
	     _scrollBox, TQT_SLOT( setPageSize( const TQSize& ) ) );
    connect( _pageView, TQT_SIGNAL( viewSizeChanged( const TQSize& ) ),
	     _scrollBox, TQT_SLOT( setViewSize( const TQSize& ) ) );
    connect( _pageView, TQT_SIGNAL( contentsMoving( int, int ) ),
	     _scrollBox, TQT_SLOT( setViewPos( int, int ) ) );

    //-- File Menu ----------------------------------------------------------
    KStdAction::saveAs( document(), TQT_SLOT( saveAs() ),
                        actionCollection() );
    new TDEAction( i18n( "Document &Info" ), 0,
                 miniWidget(), TQT_SLOT( info() ),
                 actionCollection(), "info" );

    //-- Edit Menu -----------------------------------------------------
    _popup = new TDEPopupMenu( _markList, "martdelist_menu" );

    TDEAction *act = new TDEAction( i18n( "Mark Current Page" ), "flag", CTRL+SHIFT+Key_M,
                 TQT_TQOBJECT(_markList), TQT_SLOT( markCurrent() ),
                 actionCollection(), "mark_current" );
    act->plug( _popup );
    act = new TDEAction( i18n( "Mark &All Pages" ), 0,
                 TQT_TQOBJECT(_markList), TQT_SLOT( markAll() ),
                 actionCollection(), "mark_all" );
    act->plug( _popup );
    act = new TDEAction( i18n( "Mark &Even Pages" ), 0,
                 TQT_TQOBJECT(_markList), TQT_SLOT( markEven() ),
                 actionCollection(), "mark_even" );
    act->plug( _popup );
    act = new TDEAction( i18n( "Mark &Odd Pages" ), 0,
                 TQT_TQOBJECT(_markList), TQT_SLOT( markOdd() ),
                 actionCollection(), "mark_odd" );
    act->plug( _popup );
    act = new TDEAction( i18n( "&Toggle Page Marks" ), 0,
                 TQT_TQOBJECT(_markList), TQT_SLOT( toggleMarks() ),
                 actionCollection(), "toggle" );
    act->plug( _popup );
    act = new TDEAction( i18n("&Remove Page Marks"), 0,
                 TQT_TQOBJECT(_markList), TQT_SLOT( removeMarks() ),
                 actionCollection(), "remove" );
    act->plug( _popup );

    // TODO -- disable entry if there aren't any page names

    //-- View Menu ----------------------------------------------------------
    _selectOrientation = new TDESelectAction( i18n( "&Orientation" ), 0, 0, 0,
                                    actionCollection(), "orientation_menu" );
    _selectMedia       = new TDESelectAction( i18n( "Paper &Size" ), 0, 0, 0,
                                    actionCollection(), "media_menu" );

    _flick = new TDEToggleAction( i18n( "No &Flicker" ), 0,
                                this, TQT_SLOT( slotFlicker() ),
                                actionCollection(), "no_flicker" );
    
    TQStringList orientations;
    orientations.append( i18n( "Auto" ) );
    orientations.append( i18n( "Portrait" ) );
    orientations.append( i18n( "Landscape" ) );
    orientations.append( i18n( "Upside Down" ) );
    orientations.append( i18n( "Seascape" ) );
    _selectOrientation->setItems( orientations );

    connect( _selectOrientation, TQT_SIGNAL( activated( int ) ),
	     this, TQT_SLOT( slotOrientation( int ) ) );
    connect( _selectMedia, TQT_SIGNAL( activated( int ) ),
             this, TQT_SLOT( slotMedia( int ) ) );

    {
	TDEShortcut zoomInShort = TDEStdAccel::zoomIn();
	zoomInShort.append( KKey( CTRL+Key_Equal ) );
	_zoomIn = KStdAction::zoomIn( this, TQT_SLOT( slotZoomIn() ),
				    actionCollection(), "zoomIn" );
	_zoomIn->setShortcut( zoomInShort );
    }
    _zoomOut = KStdAction::zoomOut( this,  TQT_SLOT( slotZoomOut() ),
                                    actionCollection(), "zoomOut" );
    _zoomTo = new TDESelectAction(  i18n( "Zoom" ), "viewmag", 0, actionCollection(), "zoomTo" );
    connect(  _zoomTo, TQT_SIGNAL(  activated(  const TQString & ) ), this, TQT_SLOT(  slotZoom( const TQString& ) ) );
    _zoomTo->setEditable(  true );
    _zoomTo->clear();
    TQValueList<double> mags = DisplayOptions::normalMagnificationValues();
    TQStringList zooms;
    int idx = 0;
    int cur = 0;
    for ( TQValueList<double>::iterator first = mags.begin(), last = mags.end();
	    first != last;
	    ++first ) {
        TQString str = TQString( "%1%" ).arg( TDEGlobal::locale()->formatNumber( *first * 100.0, 2 ));
        str.remove( TDEGlobal::locale()->decimalSymbol() + "00" );
        zooms << str;
	if ( *first == 1.0 ) idx = cur;
	++cur;
    }
    _zoomTo->setItems( zooms );
    _zoomTo->setCurrentItem( idx );

    _fitWidth = new TDEAction( i18n( "&Fit to Page Width" ), 0, this,
	    TQT_SLOT( slotFitToPage() ), actionCollection(),
	    "fit_to_page");
    _fitScreen = new TDEAction( i18n( "&Fit to Screen" ), Key_S, this,
	    TQT_SLOT( slotFitToScreen() ), actionCollection(),
	    "fit_to_screen");

    _prevPage  = new TDEAction( i18n( "Previous Page" ), CTRL+Key_PageUp, this, TQT_SLOT( slotPrevPage() ),
                                    actionCollection(), "prevPage" );
    _prevPage->setWhatsThis( i18n( "Moves to the previous page of the document" ) );

    _nextPage  = new TDEAction( i18n( "Next Page" ), CTRL + Key_PageDown, this, TQT_SLOT( slotNextPage() ),
                                   actionCollection(), "nextPage" );
    _nextPage->setWhatsThis( i18n( "Moves to the next page of the document" ) );

    _firstPage = KStdAction::firstPage( this, TQT_SLOT( slotGotoStart() ),
                                   actionCollection(), "goToStart" );
    _firstPage->setWhatsThis( i18n( "Moves to the first page of the document" ) );

    _lastPage  = KStdAction::lastPage( this, TQT_SLOT( slotGotoEnd() ),
                                   actionCollection(), "goToEnd" );
    _lastPage->setWhatsThis( i18n( "Moves to the last page of the document" ) );

    TDEShortcut readUpShort = TDEStdAccel::shortcut( TDEStdAccel::Prior );
    readUpShort.append( KKey( SHIFT+Key_Space ) );
    _readUp    = new TDEAction( i18n( "Read Up" ), "up",
                              readUpShort, this, TQT_SLOT( slotReadUp() ),
                              actionCollection(), "readUp" );

    TDEShortcut readDownShort = TDEStdAccel::shortcut( TDEStdAccel::Next );
    readDownShort.append( KKey( Key_Space ) );
    _readDown  = new TDEAction( i18n( "Read Down" ), "down",
                              readDownShort, this, TQT_SLOT( slotReadDown() ),
                              actionCollection(), "readDown" );

    _gotoPage = KStdAction::gotoPage( _docManager, TQT_SLOT( goToPage() ),
                                      actionCollection(), "goToPage" );

    //-- Settings Menu ------------------------------------------------------
    _showScrollBars = new TDEToggleAction( i18n( "Show &Scrollbars" ), 0,
                                actionCollection(), "show_scrollbars" );
    _showScrollBars->setCheckedState(i18n("Hide &Scrollbars"));
    _watchFile	    = new TDEToggleAction( i18n( "&Watch File" ), 0,
                                this, TQT_SLOT( slotWatchFile() ),
                                actionCollection(), "watch_file" );
    _showPageList   = new TDEToggleAction( i18n( "Show &Page List" ), 0,
                                actionCollection(), "show_page_list" );
    _showPageList->setCheckedState(i18n("Hide &Page List"));
    _showPageLabels = new TDEToggleAction( i18n("Show Page &Labels"), 0,
                                actionCollection(), "show_page_labels" );
    _showPageLabels->setCheckedState(i18n("Hide Page &Labels"));
    KStdAction::preferences( this, TQT_SLOT( slotConfigure() ), actionCollection() );
    connect( _showScrollBars, TQT_SIGNAL( toggled( bool ) ),
             TQT_SLOT( showScrollBars( bool ) ) );
    connect( _showPageList, TQT_SIGNAL( toggled( bool ) ),
             TQT_SLOT( showMarkList( bool ) ) );
    connect( _showPageLabels, TQT_SIGNAL( toggled( bool ) ),
             TQT_SLOT( showPageLabels( bool ) ) );

    _extension = new KGVBrowserExtension( this );

    setXMLFile( "kgv_part.rc" );

    connect( miniWidget(), TQT_SIGNAL( newPageShown( int ) ),
	     this, TQT_SLOT( slotNewPage( int ) ) );
    connect( _pageView, TQT_SIGNAL( contentsMoving( int, int ) ),
	     this, TQT_SLOT( slotPageMoved( int, int ) ) );

    connect( _pageView, TQT_SIGNAL( nextPage() ), TQT_SLOT( slotNextPage() ));
    connect( _pageView, TQT_SIGNAL( prevPage() ), TQT_SLOT( slotPrevPage() ));
    connect( _pageView, TQT_SIGNAL( zoomIn() ), TQT_SLOT( slotZoomIn() ));
    connect( _pageView, TQT_SIGNAL( zoomOut() ), TQT_SLOT( slotZoomOut() ));
    connect( _pageView, TQT_SIGNAL( ReadUp() ), TQT_SLOT( slotReadUp() ));
    connect( _pageView, TQT_SIGNAL( ReadDown() ), TQT_SLOT( slotReadDown() ));

    TQStringList items = document()->mediaNames();
    items.prepend( i18n( "Auto ") );
    _selectMedia->setItems( items );

    readSettings();

    updatePageDepActions();
}

KGVPart::~KGVPart()
{
    if ( _job ) _job -> kill();
    delete _mimetypeScanner;
    writeSettings();
}

TDEAboutData* KGVPart::createAboutData()
{
    TDEAboutData* about = new TDEAboutData( "kghostview", I18N_NOOP( "KGhostView"),
            KGHOSTVIEW_VERSION,
            I18N_NOOP( "Viewer for PostScript (.ps, .eps) and Portable Document Format (.pdf) files"),
            TDEAboutData::License_GPL,
            "(C) 1998 Mark Donohoe, (C) 1999-2000 David Sweet, "
            "(C) 2000-2003 Wilco Greven",
            I18N_NOOP( "KGhostView displays, prints, and saves "
                       "PostScript and PDF files.\n"
                       "Based on original work by Tim Theisen." ) );
    about->addAuthor( "Luís Pedro Coelho",
                      I18N_NOOP( "Current maintainer" ),
                      "luis@luispedro.org",
                      "http://luispedro.org" );
    about->addAuthor( "Wilco Greven",
                      I18N_NOOP( "Maintainer 2000-2003" ),
                      "greven@kde.org" );
    about->addAuthor( "David Sweet",
                      I18N_NOOP( "Maintainer 1999-2000" ),
                      "dsweet@kde.org",
                      "http://www.andamooka.org/~dsweet" );
    about->addAuthor( "Mark Donohoe",
	 	      I18N_NOOP( "Original author" ),
                      "donohoe@kde.org" );
    about->addAuthor( "David Faure",
		      I18N_NOOP( "Basis for shell"),
                      "faure@kde.org" );
    about->addAuthor( "Daniel Duley",
		      I18N_NOOP( "Port to KParts" ),
                      "mosfet@kde.org" );
    about->addAuthor( "Espen Sand",
		      I18N_NOOP( "Dialog boxes" ),
                      "espen@kde.org" );
    about->addCredit( "Russell Lang of Ghostgum Software Pty Ltd",
		      I18N_NOOP( "for contributing GSView's DSC parser." ),
                      0,
                      "http://www.ghostgum.com.au/" );
    about->addCredit( "The Ghostscript authors",
                      0, 0,
		      "http://www.cs.wisc.edu/~ghost/" );
    return about;
}

bool KGVPart::closeURL()
{
    document()->close();
    _psWidget->stopInterpreter();
    _docManager->getThumbnailService()->reset();
    _markList->clear();
    _pageDecorator->hide();
    _scrollBox->clear();
    _isFileDirty = false;
    if ( _job )
    {
	_job -> kill();
	_job = 0;
    }
    if( _mimetypeScanner != 0 )
	_mimetypeScanner->abort();
    if( !m_file.isEmpty() )
        _fileWatcher->removeFile( m_file );
    _mimetype = TQString();
    updatePageDepActions();
    stateChanged( "initState" );
    return KParts::ReadOnlyPart::closeURL();
}

void KGVPart::writeSettings()
{
    TDEConfigGroup general( KGVFactory::instance()->config(), "General" );
    if ( !_embeddedInKGhostView )
        general.writeEntry( "Display Options", DisplayOptions::toString( miniWidget()->displayOptions() ) );
    general.sync();
}

void KGVPart::readSettings()
{
    TDEConfigGroup general( KGVFactory::instance()->config(), "General" );
    
    _showScrollBars->setChecked( Configuration::showScrollBars() );
    showScrollBars( _showScrollBars->isChecked() );
    
    _watchFile->setChecked( Configuration::watchFile() );
    slotWatchFile();
    
    _showPageList->setChecked( Configuration::showPageList() );
    showMarkList( _showPageList->isChecked() );
    
    _showPageLabels->setChecked( Configuration::watchFile() );
    showPageLabels( _showPageLabels->isChecked() );

    _showLogWindow = Configuration::messages();
    if ( !_embeddedInKGhostView ) {
        DisplayOptions options;
        if ( DisplayOptions::fromString( options, general.readEntry( "Display Options" ) ) )
            setDisplayOptions( options );
    }
    _psWidget->readSettings();
}

void KGVPart::slotScrollLeft()
{
    _pageView->scrollLeft();
}

void KGVPart::slotFlicker()
{
    if ( _psWidget ) _psWidget->setDoubleBuffering( _flick->isChecked() );
}

void KGVPart::slotScrollRight()
{
    _pageView->scrollRight();
}

void KGVPart::slotScrollUp()
{
    _pageView->scrollUp();
}

void KGVPart::slotScrollDown()
{
    _pageView->scrollDown();
}

void KGVPart::slotReadUp()
{
    if( !( document() && document()->isOpen() ) )
	return;

    if( !_pageView->readUp() ) {
	if (_docManager->prevPage())
	    _pageView->scrollBottom();
    }
}

void KGVPart::slotReadDown()
{
    if( !( document() && document()->isOpen() ) )
	return;

    if( !_pageView->readDown() ) {
	if( _docManager->nextPage() )
	   _pageView->scrollTop();
    }
}

void KGVPart::slotPrevPage()
{
    if( !document() || !document()->isOpen() ) return;
    _docManager->prevPage();
}

void KGVPart::slotNextPage()
{
    if( !document() || !document()->isOpen() ) return;
    _docManager->nextPage();
}

void KGVPart::slotGotoStart()
{
    _docManager->firstPage();
    _pageView->scrollTop();
}

void KGVPart::slotGotoEnd()
{
    _docManager->lastPage();
    _pageView->scrollTop();
}

void KGVPart::slotWatchFile()
{
    if( _watchFile->isChecked() )
	_fileWatcher->startScan();
    else {
	_dirtyHandler->stop();
	_fileWatcher->stopScan();
    }
}

void KGVPart::slotCancelWatch()
{
    _fileWatcher->stopScan();
    _watchFile->setChecked( false );
}

/*
void KGVPart::slotFitWidth()
{
    _docManager->fitWidth( pageView()->viewport()->width() -
         2*( pageDecorator()->margin() + pageDecorator()->borderWidth() ) );
}
*/

void KGVPart::updateZoomActions()
{
    if( !( document() && document()->isOpen() ) )
	return;

    _zoomIn->setEnabled(!_docManager->atMaxZoom());
    _zoomOut->setEnabled(!_docManager->atMinZoom());
    _zoomTo->setEnabled( true );
    TQStringList items = _zoomTo->items();
    bool updateItems = false;
    if (_customZoomIndex != -1)
    {
        items.remove(items.at(_customZoomIndex));
        _customZoomIndex = -1;
        updateItems = true;
    }
    double zoom = floor(miniWidget()->displayOptions().magnification()*1000.0) / 10.0;
    unsigned idx = 0;
    for ( TQStringList::iterator first = items.begin(), last = items.end();
	    first != last;
	    ++first ) {
	TQString cur = *first;
	cur.remove(  cur.find(  '%' ), 1 );
	cur = cur.simplifyWhiteSpace();
	bool ok = false;
	double z = cur.toDouble(&ok);
	if ( ok ) {
            if (std::abs( z - zoom ) < 0.1 ) {
                if (updateItems)
                    _zoomTo->setItems( items );
	        _zoomTo->setCurrentItem( idx );
	        return;
            }
            if ( z > zoom )
                break;
	}
	++idx;
    }

    // Show percentage that isn't predefined
    TQString str = TQString( "%1%" ).arg( TDEGlobal::locale()->formatNumber( zoom, 2 ));
    str.remove( TDEGlobal::locale()->decimalSymbol() + "00" );
    items.insert( items.at(idx), 1, str );
    _zoomTo->setItems( items );
    _zoomTo->setCurrentItem( idx );
    _customZoomIndex = idx;
}

void KGVPart::updatePageDepActions()
{
    bool hasDoc = document() && document()->isOpen();

    _fitWidth->setEnabled( hasDoc );
    _fitScreen->setEnabled( hasDoc );

    _prevPage->setEnabled( hasDoc && !_docManager->atFirstPage() );
    _firstPage->setEnabled( hasDoc && !_docManager->atFirstPage() );
    _nextPage->setEnabled( hasDoc && !_docManager->atLastPage() );
    _lastPage->setEnabled( hasDoc && !_docManager->atLastPage() );
    _gotoPage->setEnabled( hasDoc &&
          !(_docManager->atFirstPage() && _docManager->atLastPage()) );

    updateReadUpDownActions();
}

void KGVPart::updateReadUpDownActions()
{
    if( !( document() && document()->isOpen() ) )
    {
	_readUp->setEnabled( false );
	_readDown->setEnabled( false );
	return;
    }

    if( _docManager->atFirstPage() && _pageView->atTop() )
	_readUp->setEnabled( false );
    else
	_readUp->setEnabled( true );

    if( _docManager->atLastPage() && _pageView->atBottom() )
	_readDown->setEnabled( false );
    else
	_readDown->setEnabled( true );
}

bool KGVPart::openURL( const KURL& url )
{
    if( !url.isValid() )
	return false;
    if( !closeURL() )
	return false;

    m_url = url;
    if ( !_stickyOptions ) _options.reset();

    emit setWindowCaption( m_url.prettyURL() );

    _mimetypeScanner = new KGVRun( m_url, 0, m_url.isLocalFile(), false );
    connect( _mimetypeScanner, TQT_SIGNAL( finished( const TQString& ) ),
             TQT_SLOT( slotMimetypeFinished( const TQString& ) ) );
    connect( _mimetypeScanner, TQT_SIGNAL( error() ),
             TQT_SLOT( slotMimetypeError() ) );

    return true;
}

void KGVPart::openURLContinue()
{
    kdDebug(4500) << "KGVPart::openURLContinue()" << endl;
    if( m_url.isLocalFile() )
    {
	emit started( 0 );
	m_file = m_url.path();
	document()->openFile( m_file, _mimetype );
    }
    else
    {
	m_bTemp = true;
	// Use same extension as remote file. This is important for
	// mimetype-determination (e.g. koffice)
	TQString extension;
	TQString fileName = m_url.fileName();
	int extensionPos = fileName.findRev( '.' );
	if( extensionPos != -1 )
	    extension = fileName.mid( extensionPos ); // keep the '.'
	KTempFile tempFile( TQString(), extension );
	m_file = tempFile.name();
	_tmpFile.setName( m_file );
	_tmpFile.open( IO_ReadWrite );

	/*
	d->m_job = TDEIO::file_copy( m_url, m_file, 0600, true, false, d->m_showProgressInfo );
	emit started( d->m_job );
	connect( d->m_job, TQT_SIGNAL( result( TDEIO::Job * ) ), this, TQT_SLOT( slotJobFinished ( TDEIO::Job * ) ) );
	*/

	_job = TDEIO::get( m_url, false, isProgressInfoEnabled() );

	connect( _job, TQT_SIGNAL( data( TDEIO::Job*, const TQByteArray& ) ),
		 TQT_SLOT( slotData( TDEIO::Job*, const TQByteArray& ) ) );
	connect( _job, TQT_SIGNAL( result( TDEIO::Job* ) ),
		 TQT_SLOT( slotJobFinished( TDEIO::Job* ) ) );

	emit started( _job );
    }
}

bool KGVPart::openFile()
{
    return false;
}

void KGVPart::slotOpenFileCompleted()
{
    _docManager->getThumbnailService()->setEnabled( true );
    if( _isFileDirty )
    {
	_docManager->redisplay();
	_isFileDirty = false;
    }
    else
    {
	if ( !_stickyOptions ) setDisplayOptions( DisplayOptions() );
	_stickyOptions = false;

	stateChanged( "documentState" );
	if ( !_fileWatcher->contains( m_file ) )
	    _fileWatcher->addFile( m_file );
	slotWatchFile();
	updateZoomActions();
	emit completed();
    }
}

void KGVPart::slotGhostscriptOutput( char* data, int len )
{
    _logWindow->append( TQString::fromLocal8Bit( data, len ) );
    if( _showLogWindow )
	_logWindow->show();
}


void KGVPart::slotGhostscriptError( const TQString& error )
{
    _logWindow->setLabel( i18n( "<qt>An error occurred in rendering.<br>"
				"<strong>%1</strong><br>"
				"The display may contain errors.<br>"
				"Below are any error messages which were received from Ghostscript "
				"(<nobr><strong>%2</strong></nobr>) "
				"which may help you.</qt>" )
		    .arg( error )
		    .arg( Configuration::interpreter() ),
	   true );
    // The true above makes it show a "configure gs" option, but maybe we
    // should trigger an auto-redetection?
    // LPC (13 Apr 2003)
    _logWindow->show();
}


void KGVPart::guiActivateEvent( KParts::GUIActivateEvent* event )
{
    if( event->activated() && !_isGuiInitialized )
    {
	stateChanged( "initState" );
	_isGuiInitialized = true;
    }
    KParts::ReadOnlyPart::guiActivateEvent( event );
}

void KGVPart::slotData( TDEIO::Job* job, const TQByteArray& data )
{
    Q_ASSERT( _job == job );

    kdDebug(4500) << "KGVPart::slotData: received " << data.size() << " bytes." << endl;

    _tmpFile.writeBlock( data );
}

void KGVPart::slotMimetypeFinished( const TQString& type )
{
    kdDebug(4500) << "KGVPart::slotMimetypeFinished( " << type << " )" << endl;
    _mimetype = type;
    if( !_mimetypeScanner || _mimetypeScanner->hasError() )
	emit canceled( TQString() );
    else
	openURLContinue();
    _mimetypeScanner = 0;
}

void KGVPart::slotMimetypeError()
{
    kdDebug(4500) << "KGVPart::slotMimetypeError()" << endl;
    _mimetypeScanner = 0;
    emit started( 0 );
    //kapp->processEvents();
    emit canceled( TQString() );
}

void KGVPart::slotJobFinished( TDEIO::Job* job )
{
    Q_ASSERT( _job == job );

    kdDebug(4500) << "KGVPart::slotJobFinished" << endl;

    _job = 0;

    _tmpFile.close();

    if( job->error() )
	emit canceled( job->errorString() );
    else
	document()->openFile( m_file, _mimetype );
}

void KGVPart::slotFileDirty( const TQString& fileName )
{
    // The beauty of this is that each start cancels the previous one.
    // This means that timeout() is only fired when there have
    // no changes to the file for the last 750 milisecs.
    // This is supposed to ensure that we don't update on every other byte
    // that gets written to the file.
    if ( fileName == m_file )
    {
	_dirtyHandler->start( 750, true );
    }
}

void KGVPart::slotDoFileDirty()
{
	kdDebug(4500) << "KGVPart::File changed" << endl;
	_isFileDirty = true;
	reloadFile();
}

void KGVPart::slotNewPage( int )
{
    updatePageDepActions();
    //media->setCurrentItem (miniWidget()->getSize()-1);
    //orientation->setCurrentItem (miniWidget()->getQt::Orientation()-1);
    //TODO -- zoom
}

void KGVPart::slotPageMoved( int, int )
{
    updateReadUpDownActions();
}

void KGVPart::slotOrientation( int id )
{
    switch( id ) {
    case 0: miniWidget()->restoreOverrideOrientation();              break;
    case 1: miniWidget()->setOverrideOrientation( CDSC_PORTRAIT );   break;
    case 2: miniWidget()->setOverrideOrientation( CDSC_LANDSCAPE );  break;
    case 3: miniWidget()->setOverrideOrientation( CDSC_UPSIDEDOWN ); break;
    case 4: miniWidget()->setOverrideOrientation( CDSC_SEASCAPE );   break;
    default: ;
    }
}

void KGVPart::slotMedia( int id )
{
    if( id == 0 )
	miniWidget()->restoreOverridePageMedia();
    else
	miniWidget()->setOverridePageMedia( _document->mediaNames()[id-1] );
}

void KGVPart::showScrollBars( bool show )
{
    _pageView->enableScrollBars( show );
}

void KGVPart::showMarkList( bool show )
{
    _markList->setShown( show );
    _scrollBox->setShown( show );
    _divider->setShown( show );
}

void KGVPart::showPageLabels( bool show )
{
    _docManager->enablePageLabels( show );
}

void KGVPart::slotZoomIn()
{
    _docManager->zoomIn();
    updateZoomActions();
}

void KGVPart::slotZoomOut()
{
    _docManager->zoomOut();
    updateZoomActions();
}

void KGVPart::slotZoom( const TQString& nz )
{
    TQString z = nz;
    double zoom;
    z.remove(  z.find(  '%' ), 1 );
    zoom = TDEGlobal::locale()->readNumber(  z ) / 100;
    kdDebug( 4500 ) << "ZOOM = "  << nz << ", setting zoom = " << zoom << endl;

    DisplayOptions options = miniWidget()->displayOptions();
    options.setMagnification( zoom );
    miniWidget()->setDisplayOptions( options );
    miniWidget()->redisplay();
    _mainWidget->setFocus();
    updateZoomActions();
}

void KGVPart::slotFitToPage()
{
    kdDebug(4500) << "KGVPart::slotFitToPage()" << endl;
    if( pageView()->page() )
	miniWidget()->fitWidth( pageView()->viewport()->width() - 16 );
    // We subtract 16 pixels because of the page decoration.
    updateZoomActions();
}

void KGVPart::slotFitToScreen()
{
    kdDebug(4500) << "KGVPart::slotFitToScreen()" << endl;
    if ( _fitTimer->isActive() ) {
	disconnect( _fitTimer, TQT_SIGNAL( timeout() ), this, 0 );
	connect( _fitTimer, TQT_SIGNAL( timeout() ), TQT_SLOT( slotDoFitToScreen() ) );
    }
    else slotDoFitToScreen();
}

void KGVPart::slotDoFitToScreen()
{
    kdDebug(4500) << "KGVPart::slotDoFitToScreen()" << endl;
    if( pageView()->page() )
	miniWidget()->fitWidthHeight( pageView()->viewport()->width() - 16,
					pageView()->viewport()->height() - 16 );
    updateZoomActions();
}

void KGVPart::reloadFile()
{
    _psWidget->stopInterpreter();
    _docManager->getThumbnailService()->reset();
    document()->openFile( m_file, _mimetype );
}

void KGVPart::slotConfigure()
{
    ConfigDialog::showSettings( this );
}

void KGVPart::slotConfigurationChanged()
{
    readSettings();
    _psWidget->readSettings();
    miniWidget()->redisplay();
}

void KGVPart::setDisplayOptions( const DisplayOptions& options )
{
    kdDebug(4500) << "KGVPart::setDisplayOptions()" << endl;
    _stickyOptions = true;
    _markList->select( options.page() );
    _docManager->setDisplayOptions( options );
    _selectOrientation->setCurrentItem( options.overrideOrientation()  );
    TQStringList medias = document()->mediaNames();
    TQStringList::Iterator now = medias.find( options.overridePageMedia() );
    if ( now != medias.end() ){
	// The options are displayed in inverted order.
	// Therefore, size() - index gets you the display index
	_selectMedia->setCurrentItem(  medias.size() - KGV::distance( medias.begin(), now ) );
    } else {
	_selectMedia->setCurrentItem( 0 );
    }
}


KGVBrowserExtension::KGVBrowserExtension( KGVPart *parent ) :
    KParts::BrowserExtension( parent, "KGVBrowserExtension" )
{
    emit enableAction( "print", true );
    setURLDropHandlingEnabled( true );
}

void KGVBrowserExtension::print()
{
    ((KGVPart *)parent())->document()->print();
}


KGVRun::KGVRun( const KURL& url, mode_t mode, bool isLocalFile,
                bool showProgressInfo ) :
    KRun( url, mode, isLocalFile, showProgressInfo )
{
    connect( this, TQT_SIGNAL( finished() ), TQT_SLOT( emitFinishedWithMimetype() ) );
}

KGVRun::~KGVRun()
{}

void KGVRun::foundMimeType( const TQString& mimetype )
{
    kdDebug(4500) << "KGVRun::foundMimeType( " << mimetype << " )" << endl;

    if( m_job && m_job->inherits( "TDEIO::TransferJob" ) )
    {
	TDEIO::TransferJob *job = static_cast< TDEIO::TransferJob* >( m_job );
	job->putOnHold();
	m_job = 0;
    }

    _mimetype = mimetype;

    m_bFinished = true;
    m_timer.start( 0, true );
}


void KGVPart::updateFullScreen( bool fs )
{
    if ( fs ) showMarkList( false );
    else showMarkList( _showPageList->isChecked() );
}

void KGVPart::showPopup( int, int, const TQPoint& pos )
{
    _popup->exec( pos );
}

#include "kgv_view.moc"


// vim:sw=4:sts=4:ts=8:sta:tw=78:noet
