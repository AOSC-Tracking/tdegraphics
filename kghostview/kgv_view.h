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

#ifndef __KGV_VIEW_H
#define __KGV_VIEW_H

#include <tqcstring.h> // TQByteArray
#include <tqfile.h>

#include <kio/job.h>
#include <tdeparts/browserextension.h>
#include <krun.h>

#include "displayoptions.h"

class TQFrame;
class TQWidget;

class TDEAboutData;
class KAction;
class KActionCollection;
class KDirWatch;
class TDEInstance;
class KPopupMenu;
class KSelectAction;
class KToggleAction;

class KGVBrowserExtension;
class KGVConfigDialog;
class KGVDocument;
class KGVMiniWidget;
class KGVPageView;
class KGVPageDecorator;
class KGVRun;
class KPSWidget;
class LogWindow;
class MarkList;
class ScrollBox;

class KGVPart: public KParts::ReadOnlyPart
{
  Q_OBJECT
  
public:
    KGVPart( TQWidget* parentWidget, const char* widgetName,
             TQObject* parent, const char* name,
             const TQStringList& args = TQStringList() );

    virtual ~KGVPart();

    KGVMiniWidget*    miniWidget()    const { return _docManager;    }
    MarkList*         markList()      const { return _markList;      }
    ScrollBox*        scrollBox()           { return _scrollBox;     }
    KGVPageView*      pageView()      const { return _pageView;      }
    KGVPageDecorator* pageDecorator() const { return _pageDecorator; }
    KGVDocument*      document()      const { return _document;      }


    /**
     * Reimplemented from ReadOnlyPart in order to delete the file from
     * KDirWatch's list.
     */
    virtual bool closeURL();

    KDE_EXPORT static TDEAboutData* createAboutData();

public slots:
    /**
     * Reimplemented from ReadOnlyPart so that incoming data can be sent
     * through the DSC parser immediately on arrival.
     */
    virtual bool openURL( const KURL& );
    virtual void openURLContinue();

    /**
     * Reloads the current file.
     * No action if no file is loaded
     */
    void reloadFile();

    void updateFullScreen( bool );

    void showPopup( int, int, const TQPoint &pos );

    void slotScrollLeft();
    void slotScrollRight();
    void slotScrollUp();
    void slotScrollDown();
    void slotReadDown();
    void slotFlicker();
    void slotReadUp();
    void slotPrevPage();
    void slotNextPage();
    void slotGotoStart();
    void slotGotoEnd();

    void slotFitToPage();
    void slotFitToScreen();
    void slotDoFitToScreen();

    void showScrollBars( bool );
    void slotCancelWatch();
    void showMarkList( bool );
    void showPageLabels( bool );

    void slotZoomIn();
    void slotZoomOut();

    void slotZoom( const TQString& );

    void slotConfigure();
    void slotConfigurationChanged();

    /**
     * Sets the display options in a sticky way.
     * This means that the file being opened or the next one to be open will
     * get these options. This is useful for session management or commandline
     * arguments
     */
    void setDisplayOptions( const DisplayOptions& opts );

protected slots:
    void slotData( TDEIO::Job*, const TQByteArray& );
    void slotJobFinished( TDEIO::Job* );

    void slotMimetypeFinished( const TQString& );
    void slotMimetypeError();

    void slotFileDirty( const TQString& );
    void slotDoFileDirty();

    void slotOrientation (int);
    void slotMedia (int);
    void slotNewPage( int );
    void slotPageMoved( int, int );
    void slotWatchFile();

    void slotOpenFileCompleted();

protected:
    virtual void guiActivateEvent( KParts::GUIActivateEvent* );

    // reimplemented from ReadOnlyPart
    virtual bool openFile();

    void updatePageDepActions();
    void updateZoomActions();
    void updateReadUpDownActions();

    void readSettings();
    void writeSettings();

private slots:
    void slotGhostscriptOutput( char* data, int len );
    void slotGhostscriptError( const TQString& );

private:
    KGVBrowserExtension* _extension;

    KGVDocument* _document;

    TQWidget*	      _mainWidget;
    KGVPageView*      _pageView;
    KGVPageDecorator* _pageDecorator;
    KPSWidget*        _psWidget;
    ScrollBox*	      _scrollBox;
    TQFrame*	      _divider;
    MarkList*	      _markList;
    KGVMiniWidget*    _docManager;

    LogWindow*       _logWindow;

    TQTimer*		_fitTimer;

    KSelectAction* _selectOrientation;
    KSelectAction* _selectMedia;
    KAction*       _zoomIn;
    KAction*       _zoomOut;
    KSelectAction* _zoomTo;
    KAction *      _fitWidth;
    KAction *      _fitScreen;
    KAction*       _prevPage;
    KAction*       _nextPage;
    KAction*       _firstPage;
    KAction*       _lastPage;
    KAction*       _readUp;
    KAction*       _readDown;
    KAction*       _gotoPage;
    KToggleAction* _showScrollBars;
    KToggleAction* _watchFile;
    KToggleAction* _flick;
    KToggleAction* _showPageList;
    KToggleAction* _showPageLabels;
    KPopupMenu*    _popup;

    TQFile             _tmpFile;
    TDEIO::TransferJob* _job;
    KDirWatch*        _fileWatcher;
    KGVRun*           _mimetypeScanner;
    TQTimer*           _dirtyHandler;

    TQString _mimetype;

    bool _isGuiInitialized : 1;
    bool _isFileDirty      : 1;
    bool _showLogWindow    : 1;
    bool _stickyOptions    : 1;
    bool _embeddedInKGhostView : 1;

    int _customZoomIndex;

    DisplayOptions _options;
};


class KGVBrowserExtension : public KParts::BrowserExtension
{
  Q_OBJECT
  
    friend class KGVPart; // emits our signals
public:
    KGVBrowserExtension( KGVPart* parent );
    virtual ~KGVBrowserExtension() {}

public slots:
    // Automatically detected by konqueror
    void print();
};

class KGVRun : public KRun
{
    Q_OBJECT
  

public:
    KGVRun( const KURL& url, mode_t mode = 0,
            bool isLocalFile = false, bool showProgressInfo = true );

    virtual ~KGVRun();

signals:
    void finished( const TQString& mimetype );

protected:
    void foundMimeType( const TQString& mimetype );

protected slots:
    void emitFinishedWithMimetype() { emit finished( _mimetype ); }

private:
    TQString _mimetype;
};

#endif

// vim:sw=4:sts=4:ts=8:sta:tw=78:noet
