/***************************************************************************
 *   Copyright (C) 2002 by Wilco Greven <greven@kde.org>                   *
 *   Copyright (C) 2003 by Benjamin Meyer <benjamin@csh.rit.edu>           *
 *   Copyright (C) 2003 by Laurent Montel <montel@kde.org>                 *
 *   Copyright (C) 2003 by Luboš Luňák <l.lunak@kde.org>                   *
 *   Copyright (C) 2004 by Christophe Devriese                             *
 *                         <Christophe.Devriese@student.kuleuven.ac.be>    *
 *   Copyright (C) 2004 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef _KPDF_SHELL_H_
#define _KPDF_SHELL_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tdeparts/mainwindow.h>

class TQToolButton;
class TQPopupMenu;
class KTabWidget;

namespace KParts
{
  class Factory;
  class PartManager;
}

namespace KPDF
{
    class Part;
  /**
   * This is the application "Shell".  It has a menubar and a toolbar
   * but relies on the "Part" to do all the real work.
   *
   * @short Application Shell
   * @author Wilco Greven <greven@kde.org>
   * @version 0.1
   */
  class Shell : public KParts::MainWindow
  {
    TQ_OBJECT


  public:
    /**
     * Default Constructor
     */
    Shell();

    /**
     * Open an url
     */
    Shell(const KURL &url);

    /**
     * Default Destructor
     */
    virtual ~Shell();

    enum TabContextMenuItem
    {
      TabDuplicate = 100,
      TabBreakOff,
      TabMoveLeft,
      TabMoveRight,
      TabRemove,
      TabRemoveOther
    };

  protected:
    /**
     * This method is called when it is time for the app to save its
     * properties for session management purposes.
     */
    void saveProperties(TDEConfig*);

    /**
     * This method is called when this app is restored.  The TDEConfig
     * object points to the session management config file that was saved
     * with @ref saveProperties
     */
    void readProperties(TDEConfig*);
    void readSettings();
    void writeSettings();
    void setFullScreen( bool );

  public slots:
    void openURL(const KURL & url);
    void slotAddTab();
    void slotRemoveTab();
    void slotQuit();

  private slots:
    void fileOpen();

    void optionsConfigureToolbars();
    void applyNewToolbarConfig();
    void slotUpdateFullScreen();
    void slotShowMenubar();
    void delayedOpen();

    void slotChangeTab(KParts::Part *part);
    void slotTabContextMenu(const TQPoint &pos);
    void slotTabContextMenu(TQWidget *w, const TQPoint &pos);

    void slotDuplicateTab();
    void slotBreakOffTab();
    void slotMoveTabLeft();
    void slotMoveTabRight();
    void slotRemoveOtherTabs();

  signals:
    void restoreDocument(TDEConfig* config);
    void saveDocumentRestoreInfo(TDEConfig* config);


  private:
    void setupAccel();
    void setupActions();
    void init();
    void initTabContextMenu();
    KParts::ReadOnlyPart *createTab();
    KParts::ReadOnlyPart *findPartForTab(int tabIndex);
    void moveTabForward(int tabIndex);
    void moveTabBackward(int tabIndex);

  private:
    KTabWidget *m_tabs;
    KParts::Factory *m_factory;
    KParts::PartManager *m_manager;
    TDERecentFilesAction* m_recent;
    TDEAction *m_printAction, *m_addTabAction, *m_closeTabAction;
    TDEToggleAction *m_fullScreenAction, *m_showMenuBarAction, *m_showToolBarAction;
    TQToolButton *m_addTabButton, *m_removeTabButton;
    TQPopupMenu *m_tabsContextMenu;
    bool m_menuBarWasShown, m_toolBarWasShown;
    KURL m_openUrl; // delayed open
    int m_workingTab;
  };

}

#endif
