/* This file is part of the KDE project
   Copyright (C) 1998-2006 Carsten Pfeiffer <pfeiffer@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation, version 2.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef KUICKSHOW_H
#define KUICKSHOW_H

#include <tqevent.h>
#include <tqguardedptr.h>
#include <tqstring.h>
#include <tqvaluelist.h>

#include <kfileitem.h>
#include <kmainwindow.h>
#include <kurl.h>

#include <Imlib.h>

#include "aboutwidget.h"

class FileWidget;
class ImageWindow;
class ImData;
class KuickConfigDialog;

class KAccel;
class KConfig;
class KToggleAction;
class AboutWidget;

class KURL;
class KURLComboBox;

class KuickFile;

class DelayedRepeatEvent
{
public:
    DelayedRepeatEvent( ImageWindow *view, TQKeyEvent *ev ) {
        viewer = view;
        event  = ev;
    }
    DelayedRepeatEvent( ImageWindow *view, int action, void *data ) {
        this->viewer = view;
        this->action = action;
        this->data   = data;
        this->event  = 0L;
    }

    ~DelayedRepeatEvent() {
        delete event;
    }

    enum Action
    {
        DeleteCurrentFile,
        TrashCurrentFile,
        AdvanceViewer
    };

    ImageWindow *viewer;
    TQKeyEvent *event;
    int action;
    void *data;
};


class KuickShow : public KMainWindow
{
    Q_OBJECT
  TQ_OBJECT

public:
    KuickShow( const char *name=0 );
    ~KuickShow();

    virtual void 	show();
    static TQValueList<ImageWindow*>  s_viewers;

    // overridden to make KDCOPActionProxy work -- all our actions are not
    // in the mainwindow's collection, but in the filewidget's.
    virtual KActionCollection* actionCollection() const;


protected:
    virtual void	readProperties( KConfig * );
    void 		initImlibParams( ImData *, ImlibInitParams * );
    void                tryShowNextImage();

private slots:
    void                toggleBrowser();
    void 		slotQuit() { delete this; }
    void 		slotPrint();
    void 		slotConfigApplied();
    void 		slotConfigClosed();
    void 		messageCantLoadImage( const KuickFile * file, const TQString& message);
    bool         	showImage(const KFileItem *, bool newWindow = false,
                                  bool fullscreen = false, bool moveToTopLeft = true );
    void 		showFileItem( ImageWindow *, const KFileItem * );
    void		slotHighlighted( const KFileItem * );
    void 		slotSelected( const KFileItem * );
    void		dirSelected( const KURL& );
    void		configuration();
    void	      	about();
    void 		startSlideShow();
    void                pauseSlideShow();
    void 		nextSlide();
    void                nextSlide( KFileItem *item );
    void		viewerDeleted();
    void 		slotDropped( const KFileItem *, TQDropEvent *, const KURL::List &);
    void 		slotSetActiveViewer( ImageWindow *i ) { m_viewer = i; }
    void                slotAdvanceImage( ImageWindow *, int steps );

    void 		slotShowInSameWindow();
    void 		slotShowInOtherWindow();
    void                slotShowFullscreen();

    void		slotReplayEvent();
    void                slotOpenURL();
    void		slotSetURL( const KURL& );
    void		slotURLComboReturnPressed();
//     void                tqinvalidateImages( const KFileItemList& items );
    void		slotDeleteCurrentImage(ImageWindow *viewer);
    void		slotTrashCurrentImage(ImageWindow *viewer);
    void                slotDeleteCurrentImage();
    void                slotTrashCurrentImage();

    void                doReplay();

private:
    void 		initGUI( const KURL& startDir );
    bool	       	eventFilter( TQObject *, TQEvent * );
    void 		initImlib();
    void 		saveProperties( KConfig * );
    void 		saveSettings();
    bool 		haveBrowser() const;
    void 		delayedRepeatEvent( ImageWindow *, TQKeyEvent * );
    void		abortDelayedEvent();
    void                deleteAllViewers();
    void                redirectDeleteAndTrashActions(KActionCollection *coll);

    void                delayAction(DelayedRepeatEvent *event);
    void                replayAdvance(DelayedRepeatEvent *event);

    void                performDeleteCurrentImage(TQWidget *tqparent);
    void                performTrashCurrentImage(TQWidget *tqparent);

    uint 		viewItem, renameItem, deleteItem, printItem;
    uint                m_slideshowCycle;

    FileWidget   	*fileWidget;
    KURLComboBox	*cmbPath;
    KuickConfigDialog 	*dialog;
    ImlibData           *id;
    ImageWindow 	*m_viewer;
    KToggleAction 	*oneWindowAction;
    KAccel 		*m_accel;
    DelayedRepeatEvent  *m_delayedRepeatItem;
    TQTimer              *m_slideTimer;
    bool                m_slideShowStopped;
    KToggleAction       *m_toggleBrowserAction;
    TQGuardedPtr<AboutWidget> aboutWidget;
};

#endif
