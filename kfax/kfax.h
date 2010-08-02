    /*

    $Id$

    Requires the Qt widget libraries, available at no cost at
    http://www.troll.no

    Copyright (C) 1997 Bernd Johannes Wuebben
                       wuebben@math.cornell.edu


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

#ifndef _KFAX_H_
#define _KFAX_H_

#include <kapplication.h>
#include <kmainwindow.h>
#include <tqptrlist.h>
#include <kurl.h>

#define Pimage(p)	((XImage *)(p)->extra)

class TQPopupMenu;
class KConfig;
class KStatusBar;
class KAction;
class KRecentFilesAction;
class KPrinter;
class TQPainter;
class TQScrollBar;

typedef KToolBar::BarPosition BarPosition;

class MyApp:public KApplication {
public:
   virtual bool x11EventFilter( XEvent * );
};

class TopLevel : public KMainWindow
{
    Q_OBJECT


public:

    TopLevel( TQWidget *parent=0, const char *name=0 );
    ~TopLevel();

    void openNetFile( const KURL& _url);
    void saveNetFile( const KURL& _url );

    static TQPtrList<TopLevel> windowList;
    TQPopupMenu *right_mouse_button, *colors;

    void handle_X_event(XEvent Event);
    void putImage();

protected:

    void resizeEvent( TQResizeEvent * );
    void wheelEvent( TQWheelEvent * );
    void dragEnterEvent( TQDragEnterEvent * event );
    void dropEvent( TQDropEvent * event );

    void updateActions();
    void updateGoActions();
    void updateZoomActions();

    void zoom( int );

    void readSettings();
    void writeSettings();
    void setupActions();
    void setupMenuBar();
    void setupToolBar();
    void setupEditWidget();
    void setupStatusBar();

    bool queryClose();

private:

    TQFrame *faxqtwin;

    int indentID;
    TQColor forecolor;
    TQColor backcolor;

    KURL fileURL;
    KStatusBar *statusbar;

    KRecentFilesAction *actRecent;
    KAction *actAdd, *actSave, *actSaveAs, *actPrint;
    KAction *actSize, *actZoomIn, *actZoomOut, *actRotate, *actMirror;
    KAction *actFlip, *actNext, *actPrev, *actFirst, *actLast;

    int open_mode;

    KConfig *config;

    TQScrollBar *hsb;
    TQScrollBar *vsb;
    TQFrame* 	mainpane;

    void printIt( KPrinter &printer, TQPainter &painter );
    bool loadAllPages( int &numpages, int &currentpage );

public slots:

    void faxOpen( const KURL &  );
    void faxOpen();
    void faxAdd();
    void faxAdd( const KURL & );
    void faxClose();
    void print();
    void zoomin();
    void zoomout();
    void actualSize();
    void resizeView();
    void faxSave();
    void faxSaveAs();

    void faxoptions();
    void rotatePage();
    void mirrorPage();
    void flipPage();
    void nextPage();
    void prevPage();
    void newPage();
    void firstPage();
    void lastPage();
    void uiUpdate();

    void openadd(TQString filename);
    void FreeFax();
    void scrollHorz(int);
    void scrollVert(int);
};

void kfaxerror(const TQString&, const TQString&);
void loadfile(TQString filename);

#endif // _KFAX_H_

