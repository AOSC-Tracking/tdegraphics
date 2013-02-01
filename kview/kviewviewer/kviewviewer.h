/*  This file is part of the KDE project
    Copyright (C) 1999 Simon Hausmann <hausmann@kde.org>
                  2001-2002 Matthias Kretz <kretz@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

*/
#ifndef __kviewviewer_h__
#define __kviewviewer_h__

#include "kimageviewer/viewer.h"
#include "kviewkonqextension.h"

#include <tqvaluevector.h>
#include "kviewvieweriface.h"
#include <kdemacros.h>

namespace KImageViewer { class Canvas; }
namespace TDEIO { class Job; }

class KTempFile;
class TDEAction;
class TDEActionMenu;
class TDEToggleAction;
class TDESelectAction;
class TDEAboutData;
class TQBuffer;
class TQSize;
class KDirWatch;
template<class T>
class TQCache;

class KDE_EXPORT KViewViewer : public KImageViewer::Viewer, public KViewViewerIface
{
	Q_OBJECT
  
	friend class KViewKonqExtension;

	public:
		KViewViewer( TQWidget * parentWidget, const char * widgetName,
				TQObject * parent, const char * name, const TQStringList & );
		virtual ~KViewViewer();

		KImageViewer::Canvas * canvas() const { return m_pCanvas; }
		static TDEAboutData * createAboutData();
		virtual void setReadWrite( bool readwrite = true );
		KParts::BrowserExtension * browserExtension() const { return m_pExtension; }

		bool saveAs( const KURL & );
		void setModified( bool );

	public slots:
		virtual bool openURL( const KURL & );
		virtual bool closeURL();
		virtual void newImage( const TQImage & );
		virtual void reload();

	protected:
		bool eventFilter( TQObject *, TQEvent * ); // for DnD
		void abortLoad();
		virtual bool openFile();
		virtual bool saveFile();

		virtual void setupActions();
		void guiActivateEvent( KParts::GUIActivateEvent * );

		void writeSettings();

	protected slots:
		void readSettings();
		void zoomChanged( double );
		void slotJobFinished( TDEIO::Job * );
		void slotData( TDEIO::Job *, const TQByteArray & );

		void slotSave();
		void slotSaveAs();
		void slotZoomIn();
		void slotZoomOut();
		void setZoom( const TQString & );
		void updateZoomMenu( double zoom );
		void slotFlipH();
		void slotFlipV();
		void slotRotateCCW();
		void slotRotateCW();
		void slotFitToWin();
		void slotDel();

		void slotPopupMenu( const TQPoint & );
		void slotResultSaveAs( TDEIO::Job * );

		void slotFileDirty( const TQString & );
		void slotReloadUnmodified();

		void slotToggleScrollbars();

		void loadPlugins();

		void switchBlendEffect();
		void hasImage( bool );

	private:
		TQWidget * m_pParentWidget;
		TDEIO::Job * m_pJob;
		KViewKonqExtension * m_pExtension;
		KImageViewer::Canvas * m_pCanvas;
		KTempFile * m_pTempFile;
		TQBuffer * m_pBuffer;
		KDirWatch * m_pFileWatch;

		// Actions:
		TDEAction * m_paZoomIn;
		TDEAction * m_paZoomOut;
		TDESelectAction * m_paZoom;
		TDEActionMenu * m_paFlipMenu;
		TDEAction * m_paFlipH;
		TDEAction * m_paFlipV;
		TDEAction * m_paRotateCCW;
		TDEAction * m_paRotateCW;
		TDEAction * m_paSave;
		TDEAction * m_paSaveAs;
		TDEAction * m_paFitToWin;
		TDEToggleAction * m_paShowScrollbars;

		TQString m_popupDoc;
		TQString m_mimeType;
		TQString m_newMimeType;
		TQString m_sCaption;
		
		TQValueVector<unsigned int> m_vEffects;
};

// vim:sw=4:ts=4

#endif
