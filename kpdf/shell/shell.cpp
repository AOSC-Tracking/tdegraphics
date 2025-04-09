/***************************************************************************
 *   Copyright (C) 2002 by Wilco Greven <greven@kde.org>                   *
 *   Copyright (C) 2002 by Chris Cheney <ccheney@cheney.cx>                *
 *   Copyright (C) 2003 by Benjamin Meyer <benjamin@csh.rit.edu>           *
 *   Copyright (C) 2003-2004 by Christophe Devriese                        *
 *                         <Christophe.Devriese@student.kuleuven.ac.be>    *
 *   Copyright (C) 2003 by Laurent Montel <montel@kde.org>                 *
 *   Copyright (C) 2003-2004 by Albert Astals Cid <tsdgeos@terra.es>       *
 *   Copyright (C) 2003 by Luboš Luňák <l.lunak@kde.org>                   *
 *   Copyright (C) 2003 by Malcolm Hunter <malcolm.hunter@gmx.co.uk>       *
 *   Copyright (C) 2004 by Dominique Devriese <devriese@kde.org>           *
 *   Copyright (C) 2004 by Dirk Mueller <mueller@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

// qt/kde includes
#include <tqcursor.h>
#include <tqtimer.h>
#include <tqtoolbutton.h>
#include <ktabwidget.h>
#include <tqptrlist.h>
#include <tdeaction.h>
#include <tdeapplication.h>
#include <kedittoolbar.h>
#include <tdefiledialog.h>
#include <klibloader.h>
#include <tdemessagebox.h>
#include <kiconloader.h>
#include <kstdaction.h>
#include <kurl.h>
#include <kdebug.h>
#include <tdelocale.h>
#include <tdemenubar.h>
#include <tdeparts/componentfactory.h>
#include <tdeparts/partmanager.h>
#include <tdeio/netaccess.h>
#include <tdemainwindowiface.h>

// local includes
#include "shell.h"

using namespace KPDF;

Shell::Shell()
  : KParts::MainWindow(0, "KPDF::Shell"),
    m_menuBarWasShown(true),
    m_toolBarWasShown(true),
    m_tabs(nullptr),
    m_tabsContextMenu(nullptr),
    m_manager(nullptr),
    m_workingTab(-1)
{
  init();
}

Shell::Shell(const KURL &url)
  : KParts::MainWindow(0, "KPDF::Shell"),
    m_menuBarWasShown(true),
    m_toolBarWasShown(true),
    m_tabs(nullptr),
    m_tabsContextMenu(nullptr),
    m_manager(nullptr),
    m_workingTab(-1)
{
  m_openUrl = url;
  init();
}

void Shell::init()
{
  // this routine will find and load our Part.  it finds the Part by
  // name which is a bad idea usually.. but it's alright in this
  // case since our Part is made for this Shell
  m_factory = (KParts::Factory *) KLibLoader::self()->factory("libkpdfpart");
  if (!m_factory)
  {
    // if we couldn't find our Part, we exit since the Shell by
    // itself can't do anything useful
    KMessageBox::error(this, i18n("Unable to find kpdf part."));
    TQTimer::singleShot(0, kapp, TQ_SLOT(quit()));
    return;
  }

  m_tabs = new KTabWidget(this);
  connect(m_tabs, TQ_SIGNAL(contextMenu(const TQPoint &)),
          this, TQ_SLOT(slotTabContextMenu(const TQPoint &)));
  connect(m_tabs, TQ_SIGNAL(contextMenu(TQWidget*, const TQPoint &)),
          TQ_SLOT(slotTabContextMenu(TQWidget*, const TQPoint &)));

  m_manager = new KParts::PartManager(this, "kpdf part manager");
  connect(m_manager, TQ_SIGNAL(activePartChanged(KParts::Part*)),
          this, TQ_SLOT(createGUI(KParts::Part*)));
  connect(m_manager, TQ_SIGNAL(activePartChanged(KParts::Part*)),
          this, TQ_SLOT(slotChangeTab(KParts::Part*)));

  setCentralWidget(m_tabs);
  setXMLFile("shell.rc");

  setupActions();
  setupGUI(Keys | Save);
  m_showToolBarAction = static_cast<TDEToggleAction*>(toolBarMenuAction());

  readSettings();
  if (!TDEGlobal::config()->hasGroup("MainWindow"))
  {
    TDEMainWindowInterface kmwi(this);
    kmwi.maximize();
  }
  setAutoSaveSettings();

  slotAddTab();
  if (m_openUrl.isValid())
  {
    TQTimer::singleShot(0, this, TQ_SLOT(delayedOpen()));
  }
}

void Shell::delayedOpen()
{
  openURL(m_openUrl);
}

Shell::~Shell()
{
  if (m_tabs)
  {
    writeSettings();
  }
}

void Shell::openURL( const KURL & url )
{
  // if the current part has no url, reuse part
  KParts::ReadOnlyPart *part = static_cast<KParts::ReadOnlyPart*>(m_manager->activePart());
  if (!part || !part->url().isEmpty())
  {
    part = createTab();
  }

  if (part)
  {
    if (url.isValid())
    {
      m_tabs->changeTab(part->widget(), url.filename());
      m_tabs->setTabToolTip(part->widget(), url.prettyURL());
      bool openOk = part->openURL(url);
      if (openOk)
      {
          m_recent->addURL(url);
      }
      else
      {
          m_recent->removeURL(url);
      }
    }
  }
}

void Shell::readSettings()
{
  m_recent->loadEntries( TDEGlobal::config() );
  m_recent->setEnabled( true ); // force enabling
  m_recent->setToolTip( i18n("Click to open a file\nClick and hold to open a recent file") );

  TDEGlobal::config()->setDesktopGroup();
  bool fullScreen = TDEGlobal::config()->readBoolEntry( "FullScreen", false );
  setFullScreen( fullScreen );
}

void Shell::writeSettings()
{
  m_recent->saveEntries( TDEGlobal::config() );
  TDEGlobal::config()->setDesktopGroup();
  TDEGlobal::config()->writeEntry( "FullScreen", m_fullScreenAction->isChecked());
  TDEGlobal::config()->sync();
}

void Shell::setupActions()
{
  TDEAction *openAction = KStdAction::open(this, TQ_SLOT(fileOpen()), actionCollection());
  m_recent = KStdAction::openRecent( this, TQ_SLOT(openURL(const KURL&)), actionCollection());
  connect(m_recent, TQ_SIGNAL(activated()), openAction, TQ_SLOT( activate()));
  m_recent->setWhatsThis(i18n("<b>Click</b> to open a file or <b>Click and hold</b> to select a recent file"));
  m_printAction = KStdAction::print(m_manager->activePart(), TQ_SLOT(slotPrint()), actionCollection());
  m_printAction->setEnabled(false);
  KStdAction::quit(this, TQ_SLOT(slotQuit()), actionCollection());

  setStandardToolBarMenuEnabled(true);

  m_showMenuBarAction = KStdAction::showMenubar(this, TQ_SLOT(slotShowMenubar()), actionCollection());
  KStdAction::configureToolbars(this, TQ_SLOT(optionsConfigureToolbars()), actionCollection());
  m_fullScreenAction = KStdAction::fullScreen(this, TQ_SLOT(slotUpdateFullScreen()), actionCollection(), this);

  TDEAction *addTab = new TDEAction(i18n("&New Tab"), SmallIcon("tab_new"), "Ctrl+Shift+N;Ctrl+T",
                                          this, TQ_SLOT(slotAddTab()), actionCollection(),
                                          "newtab");

  m_addTabButton = new TQToolButton(m_tabs);
  m_addTabButton->setIconSet(SmallIconSet("tab_new"));
  m_tabs->setCornerWidget(m_addTabButton, TQt::TopLeft);
  connect(m_addTabButton, TQ_SIGNAL(clicked()), this, TQ_SLOT(slotAddTab()));
  m_addTabButton->show();

  TDEAction *removeTab = new TDEAction(i18n("&Close Tab"), SmallIcon("tab_remove"), "Ctrl+W",
                                             this, TQ_SLOT(slotRemoveTab()), actionCollection(),
                                             "removecurrenttab");

  m_removeTabButton = new TQToolButton(m_tabs);
  m_removeTabButton->setIconSet(SmallIconSet("tab_remove"));
  m_tabs->setCornerWidget(m_removeTabButton, TQt::TopRight);
  connect(m_removeTabButton, TQ_SIGNAL(clicked()), this, TQ_SLOT(slotRemoveTab()));
  m_removeTabButton->show();

  TDEAction *duplicateTab = new TDEAction(i18n("&Duplicate Tab"), SmallIcon("tab_duplicate"), "Ctrl+Shift+D",
                                          this, TQ_SLOT(slotDuplicateTab()), actionCollection(),
                                          "duplicatecurrenttab");

  TDEAction *breakOffTab = new TDEAction(i18n("D&etach Tab"), SmallIcon("tab_breakoff"), TQString::null,
                                         this, TQ_SLOT(slotBreakOffTab()), actionCollection(),
                                         "breakoffcurrenttab");

  TDEAction *moveTabLeft = new TDEAction(i18n("Move Tab &Left"), SmallIcon("tab_move_left"), "Ctrl+Shift+Left",
                                         this, TQ_SLOT(slotMoveTabLeft()), actionCollection(),
                                         "tab_move_left");

  TDEAction *moveTabRight = new TDEAction(i18n("Move Tab &Right"), SmallIcon("tab_move_right"), "Ctrl+Shift+Right",
                                          this, TQ_SLOT(slotMoveTabRight()), actionCollection(),
                                          "tab_move_right");

  TDEAction *removeOtherTabs = new TDEAction(i18n("Close &Other Tabs"), SmallIcon("tab_remove_other"), "Ctrl+Alt+W",
                                          this, TQ_SLOT(slotRemoveOtherTabs()), actionCollection(),
                                          "removeothertabs");
}

void Shell::saveProperties(TDEConfig* config)
{
  // the 'config' object points to the session managed
  // config file.  anything you write here will be available
  // later when this app is restored
  emit saveDocumentRestoreInfo(config);
}

void Shell::readProperties(TDEConfig* config)
{
  // the 'config' object points to the session managed
  // config file.  this function is automatically called whenever
  // the app is being restored.  read in here whatever you wrote
  // in 'saveProperties'
  if (m_manager->parts()->count() > 0)
  {
    emit restoreDocument(config);
  }
}

void Shell::fileOpen()
{
  // this slot is called whenever the File->Open menu is selected,
  // the Open shortcut is pressed (usually CTRL+O) or the Open toolbar
  // button is clicked
    KURL url = KFileDialog::getOpenURL( TQString(), "application/pdf application/postscript" );//getOpenFileName();

  if (!url.isEmpty())
  {
    openURL(url);
  }
}

void Shell::optionsConfigureToolbars()
{
  KEditToolbar dlg(factory());
  connect(&dlg, TQ_SIGNAL(newToolbarConfig()), this, TQ_SLOT(applyNewToolbarConfig()));
  dlg.exec();
}

void Shell::applyNewToolbarConfig()
{
  applyMainWindowSettings(TDEGlobal::config(), "MainWindow");
}

void Shell::slotQuit()
{
    kapp->closeAllWindows();
}

// only called when starting the program
void Shell::setFullScreen( bool useFullScreen )
{
    if( useFullScreen )
        showFullScreen();
    else
        showNormal();
}

void Shell::slotUpdateFullScreen()
{
  if(m_fullScreenAction->isChecked())
  {
    m_menuBarWasShown = m_showMenuBarAction->isChecked();
    m_showMenuBarAction->setChecked(false);
    menuBar()->hide();

    m_toolBarWasShown = m_showToolBarAction->isChecked();
    m_showToolBarAction->setChecked(false);
    toolBar()->hide();

    showFullScreen();
  }
  else
  {
    if (m_menuBarWasShown)
    {
      m_showMenuBarAction->setChecked(true);
      menuBar()->show();
    }
    if (m_toolBarWasShown)
    {
      m_showToolBarAction->setChecked(true);
      toolBar()->show();
    }
    showNormal();
  }
}

void Shell::slotShowMenubar()
{
  if ( m_showMenuBarAction->isChecked() )
      menuBar()->show();
  else
      menuBar()->hide();
}

KParts::ReadOnlyPart* Shell::createTab()
{
  KParts::ReadOnlyPart *part =
    (KParts::ReadOnlyPart*)m_factory->createPart(m_tabs, "kpdf_part",
                                                 m_tabs, nullptr,
                                                 "KParts::ReadOnlyPart");
  m_tabs->addTab(part->widget(), i18n("No file"));

  connect(this, TQ_SIGNAL(restoreDocument(TDEConfig*)),
          part, TQ_SLOT(restoreDocument(TDEConfig*)));
  connect(this, TQ_SIGNAL(saveDocumentRestoreInfo(TDEConfig*)),
          part, TQ_SLOT(saveDocumentRestoreInfo(TDEConfig*)));
  connect(part, TQ_SIGNAL(enablePrintAction(bool)),
          m_printAction, TQ_SLOT(setEnabled(bool)));
  connect(part, TQ_SIGNAL(setWindowCaption(const TQString&)),
          this, TQ_SLOT(slotSetTabCaption(const TQString&)));

  part->widget()->show();
  m_manager->addPart(part, true);
  return part;
}

void Shell::slotAddTab()
{
  createTab();
}

void Shell::slotRemoveTab()
{
  if (m_workingTab == -1)
  {
    m_workingTab = m_tabs->currentPageIndex();
  }

  KParts::ReadOnlyPart *part = findPartForTab(m_workingTab);
  if (part)
  {
    m_tabs->removePage(part->widget());
    part->deleteLater();
  }

  m_workingTab = -1;
}

void Shell::slotChangeTab(KParts::Part *part)
{
  if (!part)
  {
    part = createTab();
  }

  m_tabs->showPage(part->widget());
}

void Shell::initTabContextMenu()
{
  if (m_tabsContextMenu) return;

  m_tabsContextMenu = new TQPopupMenu(this);
  m_tabsContextMenu->insertItem(SmallIcon("tab_new"),
                                i18n("&New Tab"),
                                this, TQ_SLOT(slotAddTab()),
                                action("newtab")->shortcut());
  m_tabsContextMenu->insertItem(SmallIconSet("tab_duplicate"),
                                i18n("&Duplicate Tab"),
                                this, TQ_SLOT(slotDuplicateTab()),
                                action("duplicatecurrenttab")->shortcut(),
                                TabContextMenuItem::TabDuplicate);
  m_tabsContextMenu->insertItem(SmallIconSet("tab_breakoff"),
                                i18n("D&etach Tab"),
                                this, TQ_SLOT(slotBreakOffTab()),
                                action("breakoffcurrenttab")->shortcut(),
                                TabContextMenuItem::TabBreakOff);
  m_tabsContextMenu->insertSeparator();
  m_tabsContextMenu->insertItem(SmallIconSet("1leftarrow"),
                                i18n("Move Tab &Left"),
                                this, TQ_SLOT(slotMoveTabLeft()),
                                action("tab_move_left")->shortcut(),
                                TabContextMenuItem::TabMoveLeft);
  m_tabsContextMenu->insertItem(SmallIconSet("1rightarrow"),
                                i18n("Move Tab &Right"),
                                this, TQ_SLOT(slotMoveTabRight()),
                                action("tab_move_right")->shortcut(),
                                TabContextMenuItem::TabMoveRight);
  m_tabsContextMenu->insertSeparator();
  m_tabsContextMenu->insertItem(SmallIconSet("tab_remove"),
                                i18n("&Close Tab"),
                                this, TQ_SLOT(slotRemoveTab()),
                                action("removecurrenttab")->shortcut(),
                                TabContextMenuItem::TabRemove);
  m_tabsContextMenu->insertItem(SmallIconSet("tab_remove_other"),
                                i18n("Close &Other Tabs"),
                                this, TQ_SLOT(slotRemoveOtherTabs()),
                                action("removeothertabs")->shortcut(),
                                TabContextMenuItem::TabRemoveOther);
}

void Shell::slotTabContextMenu(const TQPoint &pos)
{
  if (!m_tabsContextMenu)
  {
    initTabContextMenu();
  }

  m_tabsContextMenu->setItemEnabled(TabContextMenuItem::TabDuplicate, false);
  m_tabsContextMenu->setItemEnabled(TabContextMenuItem::TabBreakOff, false);
  m_tabsContextMenu->setItemEnabled(TabContextMenuItem::TabMoveLeft, false);
  m_tabsContextMenu->setItemEnabled(TabContextMenuItem::TabMoveRight, false);
  m_tabsContextMenu->setItemEnabled(TabContextMenuItem::TabRemove, false);
  m_tabsContextMenu->setItemEnabled(TabContextMenuItem::TabRemoveOther, false);


  m_tabsContextMenu->popup(pos);
}
void Shell::slotTabContextMenu(TQWidget *w, const TQPoint &pos)
{
  if (!m_tabsContextMenu)
  {
    initTabContextMenu();
  }

  m_tabsContextMenu->setItemEnabled(TabContextMenuItem::TabDuplicate, true);
  m_tabsContextMenu->setItemEnabled(TabContextMenuItem::TabBreakOff, m_tabs->count() > 1);
  m_tabsContextMenu->setItemEnabled(TabContextMenuItem::TabRemove, true);
  m_tabsContextMenu->setItemEnabled(TabContextMenuItem::TabRemoveOther, true);

  int idx = m_tabs->indexOf(w);
  if (idx > -1)
  {
    m_tabsContextMenu->setItemEnabled(TabContextMenuItem::TabMoveLeft, idx > 0);
    m_tabsContextMenu->setItemEnabled(TabContextMenuItem::TabMoveRight, idx + 1 < m_tabs->count());
  }
  else
  {
    m_tabsContextMenu->setItemEnabled(TabContextMenuItem::TabMoveLeft, false);
    m_tabsContextMenu->setItemEnabled(TabContextMenuItem::TabMoveRight, false);
  }

  m_workingTab = m_tabs->indexOf(w);
  m_tabsContextMenu->popup(pos);
}

KParts::ReadOnlyPart* Shell::findPartForTab(int tabIndex)
{
  if (tabIndex == -1) return nullptr;

  TQWidget *page = m_tabs->page(tabIndex);
  if (!page) return nullptr;

  TQPtrList<KParts::Part> *parts = const_cast<TQPtrList<KParts::Part>*>(m_manager->parts());
  KParts::Part *part;
  for (part = parts->first(); part; part = parts->next())
  {
    if (part->widget() == page)
    {
      return static_cast<KParts::ReadOnlyPart*>(part);
    }
  }
  return nullptr;
}

void Shell::moveTabForward(int tabIndex)
{
  if (tabIndex < m_tabs->count() - 1)
  {
    m_tabs->moveTab(tabIndex, tabIndex + 1);
  }
}

void Shell::moveTabBackward(int tabIndex)
{
  if (tabIndex > 0)
  {
    m_tabs->moveTab(tabIndex, tabIndex - 1);
  }
}

void Shell::slotDuplicateTab()
{
  if (m_workingTab == -1)
  {
    m_workingTab = m_tabs->currentPageIndex();
  }

  KParts::ReadOnlyPart *currentTab = findPartForTab(m_workingTab);
  if (currentTab)
  {
    openURL(currentTab->url());
  }

  m_workingTab = -1;
}

void Shell::slotBreakOffTab()
{
  if (m_workingTab == -1)
  {
    m_workingTab = m_tabs->currentPageIndex();
  }

  KParts::ReadOnlyPart *currentTab = findPartForTab(m_workingTab);
  if (currentTab)
  {
    KPDF::Shell* widget = new KPDF::Shell(currentTab->url());
    widget->show();
  }
  slotRemoveTab();
  m_workingTab = -1;
}

void Shell::slotMoveTabLeft()
{
  if (m_workingTab == -1)
  {
    m_workingTab = m_tabs->currentPageIndex();
  }

  if (TQApplication::reverseLayout())
  {
    moveTabForward(m_workingTab);
  }
  else
  {
    moveTabBackward(m_workingTab);
  }

  m_workingTab = -1;
}

void Shell::slotMoveTabRight()
{
  if (m_workingTab == -1)
  {
    m_workingTab = m_tabs->currentPageIndex();
  }

  if (TQApplication::reverseLayout())
  {
    moveTabBackward(m_workingTab);
  }
  else
  {
    moveTabForward(m_workingTab);
  }

  m_workingTab = -1;
}

void Shell::slotRemoveOtherTabs()
{
  if (m_workingTab == -1)
  {
    m_workingTab = m_tabs->currentPageIndex();
  }

  if (KMessageBox::warningContinueCancel(this,
        i18n("Do you really want to close all other tabs?"),
        i18n("Close Other Tabs Confirmation"),
        KGuiItem(i18n("Close &Other Tabs"), "tab_remove_other"),
        "CloseOtherTabConfirm") != KMessageBox::Continue)
  {
    m_workingTab = -1;
    return;
  }

  KParts::ReadOnlyPart *currentPart = findPartForTab(m_workingTab);
  if (!currentPart) return;

  TQPtrList<KParts::Part> *parts = const_cast<TQPtrList<KParts::Part>*>(m_manager->parts());
  KParts::Part *part;
  for (part = parts->first(); part; part = parts->next())
  {
    if (part == currentPart) continue;
    m_tabs->removePage(part->widget());
    part->deleteLater();
  }
}

void Shell::slotSetTabCaption(const TQString &caption)
{
  KParts::ReadOnlyPart *part = const_cast<KParts::ReadOnlyPart*>
    (static_cast<const KParts::ReadOnlyPart*>(TQObject::sender()));
  if (!part) return;

  m_tabs->changeTab(part->widget(), caption.isEmpty() ? i18n("No file") : caption);
  if (caption.isEmpty())
  {
    m_tabs->removeTabToolTip(part->widget());
  }
  else
  {
    m_tabs->setTabToolTip(part->widget(), part->url().pathOrURL());
  }
}

#include "shell.moc"
