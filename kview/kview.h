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

#ifndef KVIEW_H
#define KVIEW_H

#include <config.h>

#include <tdeapplication.h>
#include <tdeparts/mainwindow.h>

namespace KParts {
	class ReadWritePart;
}
namespace KImageViewer {
	class Canvas;
	class Viewer;
}
class TQSize;
class TQRect;
class TDEAction;
class TDEToggleAction;
class TDERecentFilesAction;
class KWinModule;
class TQStringList;
class KProgress;

class KView : public KParts::MainWindow
{
	TQ_OBJECT
  
	public:
		KView();
		virtual ~KView();
		void load( const KURL & url );
		void loadFromStdin();
		TQSize sizeForCentralWidgetSize( TQSize );
		KImageViewer::Viewer * viewer() const { return m_pViewer; }

	protected:
		bool queryClose();
		void saveProperties( TDEConfig * );
		void readProperties( TDEConfig * );
		void saveSettings( TDEConfig * );

		virtual bool eventFilter( TQObject *, TQEvent * );

	protected slots:
		void readSettings(); //TDEConfig * );
		void imageSizeChanged( const TQSize & );
		void selectionChanged( const TQRect & );
		void contextPress( const TQPoint & );
		void slotOpenFile();
		void slotOpenRecent( const KURL & );
		void slotClose();
		void slotCopy();
		void slotPaste();
		void slotCrop();
		void slotUpdateFullScreen( bool );
		void slotToggleMenubar();
		void slotPreferences();
		void slotConfigureToolbars();
		void slotNewToolbarConfig();
		void reloadConfig();
		void enableAction( const char *, bool );
		void clipboardDataChanged();
		void jobStarted( TDEIO::Job * );
		void jobCompleted();
		void jobCompleted( bool );
		void jobCanceled( const TQString & );
		void loadingProgress( TDEIO::Job *, unsigned long );
		void speedProgress( TDEIO::Job *, unsigned long );
		void slotSetStatusBarText( const TQString & );
		void cursorPos( const TQPoint & ); // write the cursor pos to the statusbar
		void loadPlugins();
		void statusbarToggled( bool );

	private:
		enum BarSizeFrom { FromImageSize, FromWidgetSize };
		enum ResizeMode { ResizeWindow = 0, ResizeImage = 1, NoResize = 2, BestFit = 3 };
		enum StatusBarItem { STATUSBAR_SPEED_ID, STATUSBAR_CURSOR_ID, STATUSBAR_SIZE_ID, STATUSBAR_SELECTION_ID };
		void setupActions( TQObject * );
		void handleResize();
		void fitWindowToImage();
		TQSize barSize( int, BarSizeFrom );
		TQSize maxCanvasSize();

		KImageViewer::Viewer * m_pViewer;
		KImageViewer::Canvas * m_pCanvas;
		KWinModule * m_pWinModule;

		// Actions:
		TDEAction * m_paOpenFile;
		TDEAction * m_paPaste;
		TDERecentFilesAction * m_paRecent;
		TDEAction * m_paQuit;
		TDEToggleFullScreenAction * m_paShowFullScreen;
		TDEToggleAction * m_paShowMenubar;
		TDEToggleAction * m_paShowStatusBar;

		int m_nResizeMode;
		bool m_bImageSizeChangedBlocked;
		bool m_bFullscreen;

		KProgress * m_pProgressBar;
};
#endif // KVIEW_H
