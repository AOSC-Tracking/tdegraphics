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

#ifndef __KGVSHELL_H__
#define __KGVSHELL_H__

#include <tqstring.h>

#include <tdeparts/mainwindow.h>

class TQTimer;

class TDERecentFilesAction;
class ScrollBox;
class KGVPart;
class TDEAction;
class TDEConfig;
class KTempFile;
class TDEPopupMenu;
class DisplayOptions;
class FullScreenFilter;

class KDE_EXPORT KGVShell : public KParts::MainWindow
{
    Q_OBJECT
  

public:
    KGVShell();
    virtual ~KGVShell();

public slots:
    void openURL( const KURL& url );
    void openStdin();
    void setDisplayOptions( const DisplayOptions& );
    void slotRMBClick();

protected slots:
    void slotFileOpen();
    void slotShowMenubar();
    void slotQuit();
    void slotMaximize();
    void slotResize();
    void slotUpdateFullScreen();
    void slotReset();
    void slotDocumentState();
    void slotConfigureToolbars();
    void slotNewToolbarConfig();

protected:
    // session management
    virtual void saveProperties( TDEConfig *config );
    virtual void readProperties( TDEConfig *config );
    
    void readSettings();
    void writeSettings();
    void enableStateDepActions( bool enable );
    void setFullScreen( bool );

private:
    
    friend class FullScreenFilter;
    
    KGVPart* m_gvpart;
    TQString cwd;

    TDEAction* openact;
    TDEToggleAction* _showMenuBarAction;
    TDEToggleAction* m_fullScreenAction;
    FullScreenFilter* m_fsFilter;
    TDEPopupMenu* _popup;
    TDERecentFilesAction* recent;
    KTempFile* _tmpFile; // Used for storing data received from stdin
};

#endif
