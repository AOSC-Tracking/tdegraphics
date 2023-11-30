/* This file is part of the KDE project
   Copyright (C) 1998-2003 Carsten Pfeiffer <pfeiffer@kde.org>

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

#include <tqkeycode.h>

#include <tdeversion.h>
#include <tdeglobal.h>
#include <tdeglobalsettings.h>
#include <tdelocale.h>
#include <tdepopupmenu.h>
#include <kpropertiesdialog.h>
#include <kurlcompletion.h>

#include "filefinder.h"
#include "filewidget.h"
#include "kuickdata.h"
#include "kuickshow.h"

#ifdef KeyPress
#undef KeyPress
#endif

FileWidget::FileWidget( const KURL& url, TQWidget *parent, const char *name )
    : KDirOperator( url, parent, name ),
      m_validCompletion( false ),
      m_fileFinder( 0L )
{
    setEnableDirHighlighting( true );

#if TDE_VERSION >= 310
    setViewConfig( TDEGlobal::config(), "Filebrowser" );
#endif
    readConfig( TDEGlobal::config(), "Filebrowser" );
    setView( KFile::Default );

    // setOnlyDoubleClickSelectsFiles( true );
    reloadConfiguration();

    completionObject()->setCompletionMode( TDEGlobalSettings::CompletionAuto );
    dirCompletionObject()->setCompletionMode( TDEGlobalSettings::CompletionAuto);

    slotViewChanged();
    connect( this, TQT_SIGNAL( viewChanged( KFileView * )),
	     TQT_SLOT( slotViewChanged() ));

    connect( dirLister(), TQT_SIGNAL( clear() ), TQT_SLOT( slotItemsCleared() ));
    connect( dirLister(), TQT_SIGNAL( deleteItem( KFileItem * ) ),
	     TQT_SLOT( slotItemDeleted( KFileItem *) ));

    connect( this, TQT_SIGNAL( fileHighlighted( const KFileItem * )),
	     TQT_SLOT( slotHighlighted( const KFileItem * )));

    connect( this, TQT_SIGNAL(urlEntered(const KURL&)),
             TQT_SLOT( slotURLEntered( const KURL& )));

    // should actually be KDirOperator's job!
    connect( this, TQT_SIGNAL( finishedLoading() ), TQT_SLOT( slotFinishedLoading() ));
}

FileWidget::~FileWidget()
{
    delete m_fileFinder;
}

void FileWidget::initActions()
{
    int index = 0;
    TDEActionCollection *coll = actionCollection();
    TDEActionSeparator *sep = new TDEActionSeparator( coll, "kuicksep" );
    TDEActionMenu *menu = static_cast<TDEActionMenu*>( coll->action("popupMenu") );

    menu->insert( coll->action("kuick_showInOtherWindow"), index++ );
    menu->insert( coll->action("kuick_showInSameWindow"), index++ );
    menu->insert( coll->action("kuick_showFullscreen"), index++ );
    menu->insert( sep, index++ );

    // support for older tdelibs, remove somewhen...
    if ( coll->action("kuick_delete") )
        menu->insert( coll->action("kuick_delete"), 9 );

    // properties dialog is now in tdefile, but not at the right position,
    // so we move it to the real bottom
    menu->remove( coll->action( "properties" ) );

    TQPopupMenu *pMenu = menu->popupMenu();
    int lastItemId = pMenu->idAt( pMenu->count() - 1 );
    TQMenuItem *mItem = pMenu->findItem( lastItemId );
    if ( mItem && !mItem->isSeparator() )
        menu->insert( sep );

    // those at the bottom
    menu->insert( coll->action("kuick_print") );
    menu->insert( sep );
    menu->insert( coll->action("properties") );
}

void FileWidget::reloadConfiguration()
{
    if ( kdata->fileFilter != nameFilter() ) {
	// At first, our list must have folders
	TQStringList mimes;
	mimes.append("inode/directory");

	// Then, all the images!
	KMimeType::List l = KMimeType::allMimeTypes();
	for (KMimeType::List::iterator it = l.begin(); it != l.end(); ++it)
	    if ((*it)->name().startsWith( "image/" ))
		mimes.append( (*it)->name() );
	
	// Ok, show what we've done
	setMimeFilter (mimes);
	updateDir();
    }
}

bool FileWidget::hasFiles() const
{
    return (numFiles() > 0);
}

void FileWidget::activatedMenu( const KFileItem *item, const TQPoint& pos )
{
    bool image = isImage( item );
    actionCollection()->action("kuick_showInSameWindow")->setEnabled( image );
    actionCollection()->action("kuick_showInOtherWindow")->setEnabled( image );
    actionCollection()->action("kuick_showFullscreen")->setEnabled( image );
    actionCollection()->action("kuick_print")->setEnabled( image );
    actionCollection()->action("properties")->setEnabled( item );

    bool hasSelection = (item != 0L);
    if ( actionCollection()->action("kuick_delete") )
        actionCollection()->action("kuick_delete")->setEnabled( hasSelection );

    KDirOperator::activatedMenu( item, pos );
}

void FileWidget::findCompletion( const TQString& text )
{
    if ( text.at(0) == '/' || text.at(0) == '~' ||
	 text.find('/') != -1 ) {
	TQString t = m_fileFinder->completion()->makeCompletion( text );

	if (m_fileFinder->completionMode() == TDEGlobalSettings::CompletionPopup ||
            m_fileFinder->completionMode() == TDEGlobalSettings::CompletionPopupAuto)
	    m_fileFinder->setCompletedItems(
			      m_fileFinder->completion()->allMatches() );
	else
            if ( !t.isNull() )
                m_fileFinder->setCompletedText( t );

	return;
    }
	
    TQString file = makeDirCompletion( text );
    if ( file.isNull() )
	file = makeCompletion( text );

    m_validCompletion = !file.isNull();

    if ( m_validCompletion )
	KDirOperator::setCurrentItem( file );
}

bool FileWidget::eventFilter( TQObject *o, TQEvent *e )
{
    if ( e->type() == TQEvent::KeyPress ) {
	TQKeyEvent *k = static_cast<TQKeyEvent*>( e );
	
	if ( (k->state() & (ControlButton | AltButton)) == 0 ) {
	    int key = k->key();
 	    if ( actionCollection()->action("delete")->shortcut().contains( key ) )
            {
                k->accept();
		KFileItem *item = getCurrentItem( false );
		if ( item ) {
                    KFileItemList list;
                    list.append( item );
		    del( list, (k->state() & ShiftButton) == 0 );
                }
		return true;
	    }
	
	    const TQString& text = k->text();
	    if ( !text.isEmpty() && text.unicode()->isPrint() ) {
                k->accept();
		
                if ( !m_fileFinder ) {
		    m_fileFinder = new FileFinder( this, "file finder" );
		    connect( m_fileFinder, TQT_SIGNAL( completion(const TQString&)),
			     TQT_SLOT( findCompletion( const TQString& )));
		    connect( m_fileFinder,
			     TQT_SIGNAL( enterDir( const TQString& ) ),
			     TQT_SLOT( slotReturnPressed( const TQString& )));
		    m_fileFinder->move( width()  - m_fileFinder->width(),
					height() - m_fileFinder->height() );
		}

		bool first = m_fileFinder->isHidden();

		m_fileFinder->setText( text );
		m_fileFinder->raise();
		m_fileFinder->show();
		m_fileFinder->setFocus();
		if ( first )
		    findCompletion( text );
		
		return true;
	    }
        }

        k->ignore();
    }
    return KDirOperator::eventFilter( o, e );
}


// TDEIO::NetAccess::stat() does NOT give us the right mimetype, while
// TDEIO::NetAccess::mimetype() does. So we have this hacklet to tell
// showImage that the KFileItem is really an image.
#define IS_IMAGE 5
#define MY_TYPE 55

bool FileWidget::isImage( const KFileItem *item )
{
//     return item && !item->isDir();
    if ( item )
    {
        return item->isReadable() && item->mimetype().startsWith( "image/") ||
            item->extraData( (void*) MY_TYPE ) == (void*) IS_IMAGE;
    }
    return false;
}
	
void FileWidget::setImage( KFileItem& item, bool enable )
{
    if ( enable )
        item.setExtraData( (void*) MY_TYPE, (void*) IS_IMAGE );
    else
        item.removeExtraData( (void*) MY_TYPE );
}

KFileItem * FileWidget::gotoFirstImage()
{
    KFileItemListIterator it( *(fileView()->items()) );

    while ( it.current() ) {
	if ( isImage( it.current() ) ) {
	    setCurrentItem( it.current() );
	    return it.current();
	}
	++it;
    }

    return 0L;
}

KFileItem * FileWidget::gotoLastImage()
{
    KFileItemListIterator it( *(fileView()->items()) );
    it.toLast();

    while ( it.current() ) {
	if ( isImage( it.current() ) ) {
	    setCurrentItem( it.current() );
	    return it.current();
	}
	--it;
    }

    return 0L;
}

KFileItem * FileWidget::getNext( bool go )
{
    KFileItem *item = getItem( Next, true );
    if ( item ) {
	if ( go )
	    setCurrentItem( item );
	return item;
    }

    return 0L;
}

KFileItem * FileWidget::getPrevious( bool go )
{
    KFileItem *item = getItem( Previous, true );
    if ( item ) {
	if ( go )
	    setCurrentItem( item );
	return item;
    }

    return 0L;
}

// returns 0L when there is no previous/next item/image
// this sucks! Use KFileView::currentFileItem() when implemented
KFileItem * FileWidget::getItem( WhichItem which, bool onlyImage ) const
{
    KFileItemListIterator it( *(fileView()->items()) );

    while ( it.current() ) { // find the iterator to the current item
	if ( it.current()->url() == m_currentURL )
	    break;

	++it;
    }

    if ( it.current() ) {
	switch ( which ) {
	case Previous: {
	    --it;
	    while ( it.current() ) {
		if ( isImage( it.current() ) || !onlyImage )
		    return it.current();
		--it;
	    }
	    return 0L; // no previous item / image
	}

	case Next: {
	    ++it;
	    while ( it.current() ) {
		if ( isImage( it.current() ) || !onlyImage )
		    return it.current();
		++it;
	    }
	    return 0L; // no further item / image
	}
	
	case Current:
	default:
	    return it.current();
	}
    }

    return 0L;
}

void FileWidget::slotViewChanged()
{
    fileView()->widget()->installEventFilter( this );
}

void FileWidget::slotItemsCleared()
{
    m_currentURL = TQString();
}

void FileWidget::slotItemDeleted( KFileItem *item )
{
    KFileItem *current = getCurrentItem( false );
    if ( item != current ) {
	return; // all ok, we already have a new current item
    }

    KFileItem *next = getNext();
    if ( !next )
	next = getPrevious();

    if ( next )
	m_currentURL = next->url().url();
}

void FileWidget::slotHighlighted( const KFileItem *item )
{
    m_currentURL = item->url().url();
}

void FileWidget::slotReturnPressed( const TQString& t )
{
    // we need a / at the end, otherwise replacedPath() will cut off the dir,
    // assuming it is a filename
    TQString text = t;
    if ( text.at( text.length()-1 ) != '/' )
	text += '/';

    if ( text.at(0) == '/' || text.at(0) == '~' ) {
	TQString dir = m_fileFinder->completion()->replacedPath( text );
	
	KURL url;
	url.setPath( dir );
	setURL( url, true );
    }

    else if ( text.find('/') != (int) text.length() -1 ) { // relative path
	TQString dir = m_fileFinder->completion()->replacedPath( text );
	KURL u( url(), dir );
	setURL( u, true );
    }

    else if ( m_validCompletion ) {
	KFileItem *item = getCurrentItem( true );

	if ( item ) {
	    if ( item->isDir() )
		setURL( item->url(), true );
	    else
		emit fileSelected( item );
	}
    }
}

void FileWidget::setCurrentItem( const KFileItem *item )
{
    if ( item ) {
        fileView()->setCurrentItem( item );
	fileView()->ensureItemVisible( item );
    }
}

void FileWidget::setInitialItem( const TQString& filename )
{
    m_initialName = filename;
}

void FileWidget::slotURLEntered( const KURL& url )
{
    if ( m_fileFinder )
        m_fileFinder->completion()->setDir( url.path() );
}

void FileWidget::slotFinishedLoading()
{
    KFileItem *current = getCurrentItem( false );
    if ( !m_initialName.isEmpty() )
	setCurrentItem( m_initialName );
    else if ( !current )
	setCurrentItem( view()->items()->getFirst() );

    m_initialName = TQString();
    emit finished();
}

TQSize FileWidget::sizeHint() const
{
  return TQSize( 300, 300 );
}
   
void FileWidget::resizeEvent( TQResizeEvent *e )
{
    KDirOperator::resizeEvent( e );
    if ( m_fileFinder )
	m_fileFinder->move( width()  - m_fileFinder->width(),
			    height() - m_fileFinder->height() );
}

#include "filewidget.moc"
