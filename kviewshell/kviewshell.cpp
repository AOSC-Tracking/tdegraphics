/*
 * Parts of this file are
 * Copyright 2003 Waldo Bastian <bastian@kde.org>
 *
 * These parts are free software; you can redistribute and/or modify
 * them under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2.
 */


#include <tqfileinfo.h>
#include <tqtimer.h>
#include <tqregexp.h>

#include <kiconloader.h>
#include <kstandarddirs.h>
#include <tdeapplication.h>
#include <tdeaction.h>
#include <tdeconfig.h>
#include <kdebug.h>
#include <tdelocale.h>
#include <tdemessagebox.h>
#include <tdemenubar.h>
#include <klibloader.h>
#include <kstdaction.h>
#include <kedittoolbar.h>
#include <kurldrag.h>
#include <tdeparts/partmanager.h>
#include <kmimetype.h>

#include <kprogress.h>
#include <tqlabel.h>

#include <stdlib.h>
#include <unistd.h>

#include "kviewpart_iface.h"
#include <kstatusbar.h>
#include <kkeydialog.h>
#include "kviewshell.moc"


#define StatusBar_ID_PageNr 1
#define StatusBar_ID_PageSize 2
#define StatusBar_ID_Zoom 3


KViewShell::KViewShell(const TQString& defaultMimeType)
  : KParts::MainWindow()
{
  // create the viewer part

  // Try to load
  KLibFactory *factory = KLibLoader::self()->factory("kviewerpart");
  if (factory) {
    if (defaultMimeType == TQString())
    {
      view = (KViewPart_Iface*) factory->create(this, "kviewerpart", "KViewPart");
    }
    else
    {
      TQStringList args;
      args << defaultMimeType;
      view = (KViewPart_Iface*) factory->create(this, "kviewerpart", "KViewPart", args);
    }
    if (!view)
      ::exit(-1);
  } else {
    KMessageBox::error(this, i18n("No viewing component found"));
    ::exit(-1);
  }

  setCentralWidget(view->widget());

  // file menu
  KStdAction::open(view, TQT_SLOT(slotFileOpen()), actionCollection());
  recent = KStdAction::openRecent (this, TQT_SLOT(openURL(const KURL &)), actionCollection());
  reloadAction = new TDEAction(i18n("Reload"), "reload", CTRL + Key_R, view, TQT_SLOT(reload()), actionCollection(), "reload");
  closeAction = KStdAction::close(this, TQT_SLOT(slotFileClose()), actionCollection());
  KStdAction::quit (this, TQT_SLOT(slotQuit()), actionCollection());

  connect(view, TQT_SIGNAL(fileOpened()), this, TQT_SLOT(addRecentFile()));

  // view menu
  fullScreenAction = KStdAction::fullScreen(this, TQT_SLOT(slotFullScreen()), actionCollection(), this, "fullscreen" );

  // settings menu
  createStandardStatusBarAction();

  setStandardToolBarMenuEnabled(true);

  KStdAction::keyBindings(this, TQT_SLOT(slotConfigureKeys()), actionCollection());
  KStdAction::configureToolbars(this, TQT_SLOT(slotEditToolbar()), actionCollection());

  // statusbar connects
  connect( view, TQT_SIGNAL( zoomChanged(const TQString &) ), this,TQT_SLOT( slotChangeZoomText(const TQString &) ) );
  connect( view, TQT_SIGNAL( pageChanged(const TQString &) ), this,TQT_SLOT( slotChangePageText(const TQString &) ) );
  connect( view, TQT_SIGNAL( sizeChanged(const TQString &) ), this,TQT_SLOT( slotChangeSizeText(const TQString &) ) );

  // Setup session management
  connect( this, TQT_SIGNAL( restoreDocument(const KURL &, int) ), view, TQT_SLOT( restoreDocument(const KURL &, int)));
  connect( this, TQT_SIGNAL( saveDocumentRestoreInfo(TDEConfig*) ), view, TQT_SLOT( saveDocumentRestoreInfo(TDEConfig*)));

  setXMLFile( "kviewshell.rc" );
  createGUI(view);
  readSettings();
  checkActions();
  setAcceptDrops(true);

  // If kviewshell is started when another instance of kviewshell runs
  // in fullscreen mode, the menubar is switched off by default, which
  // is a nasty surprise for the user: there is no way to access the
  // menus. To avoid such complications, we switch the menubar on
  // explicitly.
  menuBar()->show();

  // Add statusbar-widgets for zoom, pagenr and format
  statusBar()->insertItem("", StatusBar_ID_PageNr, 0, true);
  statusBar()->insertFixedItem("XXXX%", StatusBar_ID_Zoom, true);
  statusBar()->changeItem("", StatusBar_ID_Zoom);
  statusBar()->insertItem(view->pageSizeDescription(), StatusBar_ID_PageSize, 0, true);

  connect( view, TQT_SIGNAL(pluginChanged(KParts::Part*)), this, TQT_SLOT(createGUI(KParts::Part*)));
}


void KViewShell::checkActions()
{
  bool doc = !view->url().isEmpty();

  closeAction->setEnabled(doc);
  reloadAction->setEnabled(doc);
  fullScreenAction->setEnabled(doc);
}


KViewShell::~KViewShell()
{
  writeSettings();
  delete view;
}


void KViewShell::slotQuit()
{
  // If we are to quit the application while we operate in fullscreen
  // mode, we need to restore the visibility properties of the
  // statusbar, toolbar and the menus because these properties are
  // saved automatically ... and we don't want to start next time
  // without having menus.
  if (fullScreenAction->isChecked()) {
    kdDebug() << "Switching off fullscreen mode before quitting the application" << endl;
    showNormal();
    if (isStatusBarShownInNormalMode)
      statusBar()->show();
    if (isToolBarShownInNormalMode)
      toolBar()->show();
    menuBar()->show();
    view->slotSetFullPage(false); 
  }
  kapp->closeAllWindows();
  kapp->quit();
}


void KViewShell::readSettings()
{
  resize(600, 300); // default size if the config file specifies no size
  setAutoSaveSettings( "General" ); // apply mainwindow settings (size, toolbars, etc.)

  TDEConfig *config = kapp->config();
  config->setGroup("General");

  recent->loadEntries(config, "Recent Files");

  // Constant source of annoyance in KDVI < 1.0: the 'recent-files'
  // menu contains lots of files which don't exist (any longer). Thus,
  // we'll sort out the non-existent files here.
  TQStringList items = recent->items();
  for ( TQStringList::Iterator it = items.begin(); it != items.end(); ++it ) {
    KURL url(*it);
    if (url.isLocalFile()) {
      TQFileInfo info(url.path());
      if (!info.exists())
        recent->removeURL(url);
    }
  }

}


void KViewShell::writeSettings()
{
  TDEConfig *config = kapp->config();
  config->setGroup( "General" );
  recent->saveEntries(config, "Recent Files");

  config->sync();
}


void KViewShell::saveProperties(TDEConfig* config)
{
  // the 'config' object points to the session managed
  // config file.  anything you write here will be available
  // later when this app is restored
  emit saveDocumentRestoreInfo(config);
}


void KViewShell::readProperties(TDEConfig* config)
{
  // the 'config' object points to the session managed
  // config file.  this function is automatically called whenever
  // the app is being restored.  read in here whatever you wrote
  // in 'saveProperties'
  if (view)
  {
    KURL url (config->readPathEntry("URL"));
    if (url.isValid())
      emit restoreDocument(url, config->readNumEntry("Page", 1));
  }
}


void KViewShell::addRecentFile()
{
  // Get the URL of the opened file from the kviewpart.
  KURL actualURL = view->url();
  // To store the URL in the list of recent files, we remove the
  // reference part.
  actualURL.setRef(TQString());
  recent->addURL(actualURL);
  checkActions();
}

void KViewShell::openURL(const KURL& url)
{
  view->openURL(url);
}

void KViewShell::slotFullScreen()
{
  if (fullScreenAction->isChecked()) {
    // In fullscreen mode, menu- tool- and statusbar are hidden. Save
    // the visibility flags of these objects here, so that they can
    // later be properly restored when we switch back to normal mode,
    // or before we leave the application in slotQuit()
    isStatusBarShownInNormalMode = statusBar()->isShown();
    statusBar()->hide();
    isToolBarShownInNormalMode = toolBar()->isShown();
    toolBar()->hide();
    menuBar()->hide();
    view->slotSetFullPage(true); 

    // Go to fullscreen mode
    showFullScreen();

    KMessageBox::information(this, i18n("Use the Escape key to leave the fullscreen mode."), i18n("Entering Fullscreen Mode"), "leavingFullScreen");
  } else {
    showNormal();
    if (isStatusBarShownInNormalMode)
      statusBar()->show();
    if (isToolBarShownInNormalMode)
      toolBar()->show();
    menuBar()->show();
    view->slotSetFullPage(false); 
  }
}


void KViewShell::slotFileClose()
{
  view->closeURL_ask();

  checkActions();
}

void KViewShell::slotConfigureKeys()
{
  KKeyDialog dlg( true, this );

  dlg.insert( actionCollection() );
  dlg.insert( view->actionCollection() );

  dlg.configure();
}

void KViewShell::slotEditToolbar()
{
  saveMainWindowSettings( TDEGlobal::config(), autoSaveGroup() );
  KEditToolbar dlg(factory());
  connect( &dlg, TQT_SIGNAL( newToolbarConfig() ), TQT_SLOT( slotNewToolbarConfig() ) );
  dlg.exec();
}


void KViewShell::slotNewToolbarConfig()
{
  applyMainWindowSettings( TDEGlobal::config(), autoSaveGroup() );
}

void KViewShell::dragEnterEvent(TQDragEnterEvent *event)
{
  if (KURLDrag::canDecode(event))
  {
    KURL::List urls;
    KURLDrag::decode(event, urls);
    if (!urls.isEmpty())
    {
      KURL url = urls.first();

      // Always try to open remote files
      if (!url.isLocalFile())
      {
        event->accept();
        return;
      }

      // For local files we only accept a drop, if we have a plugin for its
      // particular mimetype
      KMimeType::Ptr mimetype = KMimeType::findByURL(url);
      kdDebug() << "[dragEnterEvent] Dragged URL is of type " << mimetype->comment() << endl;

      // Safety check
      if (view)
      {
        TQStringList mimetypeList = view->supportedMimeTypes();
        kdDebug() << "[dragEnterEvent] Supported mime types: " << mimetypeList << endl;

        for (TQStringList::Iterator it = mimetypeList.begin(); it != mimetypeList.end(); ++it)
        {
          if (mimetype->is(*it))
          {
            kdDebug() << "[dragEnterEvent] Found matching mimetype: " << *it << endl;
            event->accept();
            return;
          }
        }
        kdDebug() << "[dragEnterEvent] no matching mimetype found" << endl;
      }
    }
    event->ignore();
  }
}


void KViewShell::dropEvent(TQDropEvent *event)
{
  KURL::List urls;
  if (KURLDrag::decode(event, urls) && !urls.isEmpty())
    view->openURL(urls.first());
}


void KViewShell::keyPressEvent(TQKeyEvent *event)
{
  // The Escape Key is used to return to normal mode from fullscreen
  // mode
  if ((event->key() == TQt::Key_Escape) && (fullScreenAction->isChecked())) {
    showNormal();
    return;
  }
  // If we can't use the key event, pass it on
  event->ignore();
}


void KViewShell::slotChangePageText(const TQString &message)
{
  statusBar()->changeItem(" "+message+" ",StatusBar_ID_PageNr);
}


void KViewShell::slotChangeSizeText(const TQString &message)
{
  statusBar()->changeItem(" "+message+" ",StatusBar_ID_PageSize);
}


void KViewShell::slotChangeZoomText(const TQString &message)
{
  statusBar()->changeItem(" "+message+" ",StatusBar_ID_Zoom);
}
