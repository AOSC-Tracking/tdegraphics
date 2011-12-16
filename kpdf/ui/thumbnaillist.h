/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef _KPDF_THUMBNAILLIST_H_
#define _KPDF_THUMBNAILLIST_H_

#include <tqscrollview.h>
#include <tqvaluevector.h>
#include <tqvbox.h>
#include <ktoolbar.h>
#include "core/observer.h"

class TQTimer;
class KActionCollection;

class KPDFDocument;
class ThumbnailWidget;

/**
 * @short A scrollview that displays pages pixmaps previews (aka thumbnails).
 *
 * ...
 */
class ThumbnailList : public TQScrollView, public DocumentObserver
{
Q_OBJECT
  TQ_OBJECT
	public:
		ThumbnailList(TQWidget *parent, KPDFDocument *document);
		~ThumbnailList();

        // inherited: return thumbnails observer id
        uint observerId() const { return THUMBNAILS_ID; }
        // inherited: create thumbnails ( inherited as a DocumentObserver )
        void notifySetup( const TQValueVector< KPDFPage * > & pages, bool documentChanged );
        // inherited: hilihght current thumbnail ( inherited as DocumentObserver )
        void notifyViewportChanged( bool smoothMove );
        // inherited: redraw thumbnail ( inherited as DocumentObserver )
        void notifyPageChanged( int pageNumber, int changedFlags );
        // inherited: request all visible pixmap (due to a global shange or so..)
        void notifyContentsCleared( int changedFlags );
        // inherited: tell if pixmap is hidden and can be unloaded
        bool canUnloadPixmap( int pageNumber );

        // redraw visible widgets (useful for refreshing contents...)
        void updateWidgets();

        // called by ThumbnailWidgets to send (forward) rightClick signals
        void forwardRightClick( const KPDFPage *, const TQPoint & );
        // called by ThumbnailWidgets to get the overlay bookmark pixmap
        const TQPixmap * getBookmarkOverlay() const;

    public slots:
        // these are connected to ThumbnailController buttons
        void slotFilterBookmarks( bool filterOn );

	protected:
		// scroll up/down the view
		void keyPressEvent( TQKeyEvent * e );

		// select a thumbnail by clicking on it
		void contentsMousePressEvent( TQMouseEvent * );

		// resize thumbnails to fit the width
		void viewportResizeEvent( TQResizeEvent * );

		// file drop related events (an url may be dropped even here)
		void dragEnterEvent( TQDragEnterEvent* );
		void dropEvent( TQDropEvent* );

	signals:
		void urlDropped( const KURL& );
		void rightClick( const KPDFPage *, const TQPoint & );

	private:
		void delayedRequestVisiblePixmaps( int delayMs = 0 );
		KPDFDocument *m_document;
		ThumbnailWidget *m_selected;
		TQTimer *m_delayTimer;
		TQPixmap *m_bookmarkOverlay;
		TQValueVector<ThumbnailWidget *> m_thumbnails;
		TQValueList<ThumbnailWidget *> m_visibleThumbnails;
		int m_vectorIndex;

	private slots:
		// make requests for generating pixmaps for visible thumbnails
		void slotRequestVisiblePixmaps( int newContentsX = -1, int newContentsY = -1 );
		// delay timeout: resize overlays and requests pixmaps
		void slotDelayTimeout();
};

/**
 * @short A vertical boxed container with zero size hint (for insertion on left toolbox)
 */
class ThumbnailsBox : public TQVBox
{
	public:
		ThumbnailsBox( TQWidget * parent ) : TQVBox( parent ) {};
		TQSize tqsizeHint() const { return TQSize(); }
};

/**
 * @short A toolbar thar set ThumbnailList properties when clicking on items
 *
 * This class is the small tolbar that resides in the bottom of the
 * ThumbnailsBox container (below ThumbnailList and the SearchLine) and
 * emits signals whenever a button is pressed. A click action results
 * in invoking some method (or slot) in ThumbnailList.
 */
class ThumbnailController : public KToolBar
{
    public:
        ThumbnailController( TQWidget * parent, ThumbnailList * thumbnailList );
};

#endif
