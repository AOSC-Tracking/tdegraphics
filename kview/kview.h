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

#include <kapplication.h>
#include <kparts/mainwindow.h>

namespace KParts {
	class ReadWritePart;
}
namespace KImageViewer {
	class Canvas;
	class Viewer;
}
class TQSize;
class TQRect;
class KAction;
class KToggleAction;
class KRecentFilesAction;
class KWinModule;
class TQStringList;
class KProgress;

class KView : public KParts::MainWindow
{
	Q_OBJECT
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
		void saveProperties( KConfig * );
		void readProperties( KConfig * );
		void saveSettings( KConfig * );

		virtual bool eventFilter( TQObject *, TQEvent * );

	protected slots:
		void readSettings(); //KConfig * );
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
		void jobStarted( KIO::Job * );
		void jobCompleted();
		void jobCompleted( bool );
		void jobCanceled( const TQString & );
		void loadingProgress( KIO::Job *, unsigned long );
		void speedProgress( KIO::Job *, unsigned long );
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
		KAction * m_paOpenFile;
		KAction * m_paPaste;
		KRecentFilesAction * m_paRecent;
		KAction * m_paQuit;
		KToggleFullScreenAction * m_paShowFullScreen;
		KToggleAction * m_paShowMenubar;
		KToggleAction * m_paShowStatusBar;

		int m_nResizeMode;
		bool m_bImageSizeChangedBlocked;
		bool m_bFullscreen;

		KProgress * m_pProgressBar;
};

// vim:sw=4:ts=4

#endif // KVIEW_H
