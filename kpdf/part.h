/***************************************************************************
 *   Copyright (C) 2002 by Wilco Greven <greven@kde.org>                   *
 *   Copyright (C) 2003-2004 by Christophe Devriese                        *
 *                         <Christophe.Devriese@student.kuleuven.ac.be>    *
 *   Copyright (C) 2003 by Andy Goossens <andygoossens@telenet.be>         *
 *   Copyright (C) 2003 by Laurent Montel <montel@kde.org>                 *
 *   Copyright (C) 2004 by Dominique Devriese <devriese@kde.org>           *
 *   Copyright (C) 2004-2006 by Albert Astals Cid <tsdgeos@terra.es>       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef _KPDF_PART_H_
#define _KPDF_PART_H_

#include <tdeparts/browserextension.h>
#include <tdeparts/part.h>
#include <tqguardedptr.h>
#include "core/document.h"
#include "core/observer.h"
#include "dcop.h"

class TQWidget;
class TQSplitter;
class TQToolBox;

class KURL;
class TDEAction;
class TDEConfig;
class KDirWatch;
class TDEToggleAction;
class TDESelectAction;
class TDEAboutData;
class KPrinter;

class ThumbnailList;
class ThumbnailController;
class PageView;
class PresentationWidget;
class SearchWidget;
class TOC;
class MiniBar;

namespace KPDF {

class BrowserExtension;

/**
 * This is a "Part".  It that does all the real work in a KPart
 * application.
 *
 * @short Main Part
 * @author Wilco Greven <greven@kde.org>
 * @version 0.2
 */
class Part : public KParts::ReadOnlyPart, public DocumentObserver, virtual public kpdf_dcop
{
Q_OBJECT
  

public:
	// Default constructor
	Part(TQWidget* parentWidget, const char* widgetName,
	     TQObject* parent, const char* name, const TQStringList& args);

	// Destructor
	~Part();

    // inherited from DocumentObserver
    uint observerId() const { return PART_ID; }
    void notifyViewportChanged( bool smoothMove );

	static TDEAboutData* createAboutData();

	ASYNC goToPage(uint page);
	ASYNC openDocument(KURL doc);
	uint pages();
	uint currentPage();
	KURL currentDocument();

signals:
	void enablePrintAction(bool enable);

protected:
	// reimplemented from KParts::ReadOnlyPart
	bool openFile();
	bool openURL(const KURL &url);
	bool closeURL();
  // filter that watches for splitter size changes
  bool eventFilter( TQObject * watched, TQEvent * e );

protected slots:
	void openURLFromDocument(const KURL &url);
	// connected to actions
	void slotGoToPage();
	void slotPreviousPage();
	void slotNextPage();
	void slotGotoFirst();
	void slotGotoLast();
	void slotHistoryBack();
	void slotHistoryNext();
	void slotFind();
	void slotFindNext();
	void slotSaveFileAs();
	void slotPreferences();
	void slotNewConfig();
	void slotPrintPreview();
	void slotShowMenu(const KPDFPage *page, const TQPoint &point);
	void slotShowProperties();
	void slotShowLeftPanel();
	void slotShowPresentation();
	void slotHidePresentation();
	void slotTogglePresentation();
	void close();
	// can be connected to widget elements
	void updateViewActions();
	void enableTOC(bool enable);
	void psTransformEnded();
	void cannotQuit();
	void saveSplitterSize();
	void setMimeTypes(TDEIO::Job *job);
	void readMimeType(TDEIO::Job *job, const TQString &mime);
	void emitWindowCaption();

public slots:
	// connected to Shell action (and browserExtension), not local one
	void slotPrint();
	void restoreDocument(TDEConfig* config);
	void saveDocumentRestoreInfo(TDEConfig* config);
	void slotFileDirty( const TQString& );
	void slotDoFileDirty();

private:
	void doPrint( KPrinter& printer );

	// the document
	KPDFDocument * m_document;
	TQString m_temporaryLocalFile;

	// main widgets
	TQSplitter *m_splitter;
	TQWidget *m_leftPanel;
	TQToolBox *m_toolBox;
	SearchWidget *m_searchWidget;
	TQGuardedPtr<ThumbnailList> m_thumbnailList;
	TQGuardedPtr<PageView> m_pageView;
	TQGuardedPtr<TOC> m_tocFrame;
	TQGuardedPtr<MiniBar> m_miniBar;
	TQGuardedPtr<PresentationWidget> m_presentationWidget;

	// static instances counter
	static unsigned int m_count;

	// this is a hack because we can not use writeConfig on part destructor
	// and we don't want to writeconfig every time someone moves the splitter
	// so we use a TQTimer each 500 ms
	TQTimer *m_saveSplitterSizeTimer;

	KDirWatch *m_watcher;
	TQTimer *m_dirtyHandler;
	DocumentViewport m_viewportDirty;
	bool m_wasPresentationOpen;
	int m_dirtyToolboxIndex;
	
	// Remember the search history
	TQStringList m_searchHistory;
	
	// mimetype got from the job
	TQString m_jobMime;

	// actions
	TDEAction *m_gotoPage;
	TDEAction *m_prevPage;
	TDEAction *m_nextPage;
	TDEAction *m_firstPage;
	TDEAction *m_lastPage;
	TDEAction *m_historyBack;
	TDEAction *m_historyNext;
	TDEAction *m_find;
	TDEAction *m_findNext;
	TDEAction *m_saveAs;
	TDEAction *m_printPreview;
	TDEAction *m_showProperties;
	TDEAction *m_showPresentation;
	TDEToggleAction* m_showMenuBarAction;
	TDEToggleAction* m_showLeftPanel;
	TDEToggleAction* m_showFullScreenAction;
	bool m_actionsSearched;
	bool m_searchStarted;
	BrowserExtension *m_bExtension;
};


class BrowserExtension : public KParts::BrowserExtension
{
Q_OBJECT
  

public:
	BrowserExtension(Part*);

public slots:
	// Automatically detected by the host.
	void print();
};

}

#endif

// vim:ts=2:sw=2:tw=78:et
