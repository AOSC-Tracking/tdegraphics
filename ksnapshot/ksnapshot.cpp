/*
 * KSnapshot
 *
 * (c) Richard J. Moore 1997-2002
 * (c) Matthias Ettrich 2000
 * (c) Aaron J. Seigo 2002
 * (c) Nadeem Hasan 2003
 * (c) Bernd Brandstetter 2004
 * (c) Emanoil Kotsev 2023
 *
 * Released under the LGPL see file LICENSE for details.
 */


#include <tdelocale.h>
#include <kimageio.h>
#include <tdefiledialog.h>
#include <kimagefilepreview.h>
#include <tdemessagebox.h>
#include <kdebug.h>
#include <tdeapplication.h>
#include <kprinter.h>
#include <tdeio/netaccess.h>
#include <ksavefile.h>
#include <tdetempfile.h>

#include <tqbitmap.h>
#include <tqdragobject.h>
#include <tqimage.h>
#include <tqclipboard.h>
#include <tqvbox.h>

#include <tdeaccel.h>
#include <knotifyclient.h>
#include <khelpmenu.h>
#include <tdepopupmenu.h>
#include <kpushbutton.h>
#include <tdestartupinfo.h>
#include <kiconloader.h>
#include <kprocess.h>
#include <krun.h>

#include <tqcursor.h>
#include <tqregexp.h>
#include <tqpainter.h>
#include <tqpaintdevicemetrics.h>
#include <tqwhatsthis.h>

#include <stdlib.h>

#include "ksnapshot.h"
#include "regiongrabber.h"
#include "windowgrabber.h"
#include "ksnapshotwidget.h"

#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include <config.h>

#include <tdeglobal.h>

#define kApp TDEApplication::kApplication()

KSnapshot::KSnapshot(TQWidget *parent, const char *name, bool grabCurrent)
  : DCOPObject("interface"), 
    KDialogBase(parent, name, true, TQString(), Help|User1, User1, 
    true, KStdGuiItem::quit() )
{
    grabber = new TQWidget( 0, 0, WStyle_Customize | WX11BypassWM );
    grabber->move( -1000, -1000 );
    grabber->installEventFilter( this );

    TDEStartupInfo::appStarted();

    TQVBox *vbox = makeVBoxMainWidget();
    mainWidget = new KSnapshotWidget( vbox, "mainWidget" );

    connect(mainWidget, TQT_SIGNAL(startImageDrag()), TQT_SLOT(slotDragSnapshot()));
    connect(mainWidget, TQT_SIGNAL(newClicked()), TQT_SLOT(slotGrab()));
    connect(mainWidget, TQT_SIGNAL(saveClicked()), TQT_SLOT(slotSaveAs()));
    connect(mainWidget, TQT_SIGNAL(printClicked()), TQT_SLOT(slotPrint()));
    connect(mainWidget, TQT_SIGNAL(copyClicked()), TQT_SLOT(slotCopy()));
    connect(mainWidget, TQT_SIGNAL(openWithKPClicked()), TQT_SLOT(slotOpenWithKP()));
    connect(tqApp,      TQT_SIGNAL(aboutToQuit()), TQT_SLOT(slotAboutToQuit()));

    grabber->show();
    grabber->grabMouse( waitCursor );

    if ( !grabCurrent )
	snapshot = TQPixmap::grabWindow( tqt_xrootwin() );
    else {
	mainWidget->setMode( WindowUnderCursor );
	mainWidget->setIncludeDecorations( true );
	performGrab();
    }

    updatePreview();
    grabber->releaseMouse();
    grabber->hide();

    TDEConfig *conf=TDEGlobal::config();
    conf->setGroup("GENERAL");
    mainWidget->setDelay(conf->readNumEntry("delay",0));
    mainWidget->setMode( conf->readNumEntry( "mode", 0 ) );
    mainWidget->setIncludeDecorations(conf->readBoolEntry("includeDecorations",true));
    filename = KURL::fromPathOrURL( conf->readPathEntry( "filename", TQDir::currentDirPath()+"/"+i18n("snapshot")+"1.png" ));

    // Make sure the name is not already being used
    while(TDEIO::NetAccess::exists( filename, false, this )) {
	autoincFilename();
    }

    connect( &grabTimer, TQT_SIGNAL( timeout() ), TQT_TQOBJECT(this), TQT_SLOT(  grabTimerDone() ) );
    connect( &updateTimer, TQT_SIGNAL( timeout() ), TQT_TQOBJECT(this), TQT_SLOT(  updatePreview() ) );
    TQTimer::singleShot( 0, TQT_TQOBJECT(this), TQT_SLOT( updateCaption() ) );

    KHelpMenu *helpMenu = new KHelpMenu(this, TDEGlobal::instance()->aboutData(), false);

    TQPushButton *helpButton = actionButton( Help );
    helpButton->setPopup(helpMenu->menu());

    // Populate Open With... menu
    TDEPopupMenu *popupOpenWith = new TDEPopupMenu(this);
    openWithOffers = TDETrader::self()->query("image/png", "Type == 'Application'");
    int i = 0;
    for (TDETrader::OfferList::Iterator it = openWithOffers.begin(); it != openWithOffers.end(); ++it)
    {
        popupOpenWith->insertItem(SmallIcon((*it)->icon()), (*it)->name(), i);
        ++i; // we need menu ids to match with OfferList indexes
    }
    mainWidget->btnOpenWith->setPopup(popupOpenWith);
    connect(popupOpenWith, SIGNAL(activated(int)), this, SLOT(slotOpenWith(int)));

    // Check for KolourPaint availability
    KService::Ptr kpaint = KService::serviceByDesktopName("kolourpaint");
    if (!kpaint) {
        mainWidget->btnOpenWithKP->hide();
    }

    TDEAccel* accel = new TDEAccel(this);
    accel->insert(TDEStdAccel::Quit, TQT_TQOBJECT(kapp), TQT_SLOT(quit()));
    accel->insert( "QuickSave", i18n("Quick Save Snapshot &As..."),
		   i18n("Save the snapshot to the file specified by the user without showing the file dialog."),
		   CTRL+SHIFT+Key_S, TQT_TQOBJECT(this), TQT_SLOT(slotSave()));
    accel->insert(TDEStdAccel::Save, TQT_TQOBJECT(this), TQT_SLOT(slotSaveAs()));
//    accel->insert(TDEShortcut(CTRL+Key_A), TQT_TQOBJECT(this), TQT_SLOT(slotSaveAs()));
    accel->insert( "SaveAs", i18n("Save Snapshot &As..."),
		   i18n("Save the snapshot to the file specified by the user."),
		   CTRL+Key_A, TQT_TQOBJECT(this), TQT_SLOT(slotSaveAs()));
    accel->insert(TDEStdAccel::Print, TQT_TQOBJECT(this), TQT_SLOT(slotPrint()));
    accel->insert(TDEStdAccel::New, TQT_TQOBJECT(this), TQT_SLOT(slotGrab()));
    accel->insert(TDEStdAccel::Copy, TQT_TQOBJECT(this), TQT_SLOT(slotCopy()));

    accel->insert( "Quit2", Key_Q, TQT_TQOBJECT(this), TQT_SLOT(slotSave()));
    accel->insert( "Save2", Key_S, TQT_TQOBJECT(this), TQT_SLOT(slotSaveAs()));
    accel->insert( "Print2", Key_P, TQT_TQOBJECT(this), TQT_SLOT(slotPrint()));
    accel->insert( "New2", Key_N, TQT_TQOBJECT(this), TQT_SLOT(slotGrab()));
    accel->insert( "New3", Key_Space, TQT_TQOBJECT(this), TQT_SLOT(slotGrab()));

    setEscapeButton( User1 );
    connect( this, TQT_SIGNAL( user1Clicked() ), TQT_SLOT( reject() ) );

    mainWidget->btnNew->setFocus();
}

KSnapshot::~KSnapshot()
{
}

void KSnapshot::resizeEvent( TQResizeEvent *event)
{
	if( !updateTimer.isActive() )
		updateTimer.start(200, true);
	else	
		updateTimer.changeInterval(200);
}

bool KSnapshot::save( const TQString &filename )
{
    return save( KURL::fromPathOrURL( filename ));
}

bool KSnapshot::save( const KURL& url )
{
    if ( TDEIO::NetAccess::exists( url, false, this ) ) {
        const TQString title = i18n( "File Exists" );
        const TQString text = i18n( "<qt>Do you really want to overwrite <b>%1</b>?</qt>" ).arg(url.prettyURL());
        if (KMessageBox::Continue != KMessageBox::warningContinueCancel( this, text, title, i18n("Overwrite") ) ) 
        {
            return false;
        }
    }

    TQString type( KImageIO::type(url.path()) );
    if ( type.isNull() )
	type = "PNG";

    bool ok = false;

    if ( url.isLocalFile() ) {
	KSaveFile saveFile( url.path() );
	if ( saveFile.status() == 0 ) {
	    if ( snapshot.save( saveFile.file(), type.latin1() ) )
		ok = saveFile.close();
	}
    }
    else {
	KTempFile tmpFile;
        tmpFile.setAutoDelete( true );
	if ( tmpFile.status() == 0 ) {
	    if ( snapshot.save( tmpFile.file(), type.latin1() ) ) {
		if ( tmpFile.close() )
		    ok = TDEIO::NetAccess::upload( tmpFile.name(), url, this );
	    }
	}
    }

    TQApplication::restoreOverrideCursor();
    if ( !ok ) {
	kdWarning() << "KSnapshot was unable to save the snapshot" << endl;

	TQString caption = i18n("Unable to save image");
	TQString text = i18n("KSnapshot was unable to save the image to\n%1.")
	               .arg(url.prettyURL());
	KMessageBox::error(this, text, caption);
    }

    return ok;
}

void KSnapshot::slotSave()
{
    if ( save(filename) ) {
        modified = false;
        autoincFilename();
    }
}

void KSnapshot::slotSaveAs()
{
    TQStringList mimetypes = KImageIO::mimeTypes( KImageIO::Writing );
    KFileDialog dlg( filename.url(), mimetypes.join(" "), this, "filedialog", true);

    dlg.setOperationMode( KFileDialog::Saving );
    dlg.setCaption( i18n("Save As") );

    KImageFilePreview *ip = new KImageFilePreview( &dlg );
    dlg.setPreviewWidget( ip );

    if ( !dlg.exec() )
        return;

    KURL url = dlg.selectedURL();
    if ( !url.isValid() )
        return;

    if ( save(url) ) {
        filename = url;
        modified = false;
        autoincFilename();
    }
}

void KSnapshot::slotCopy()
{
    TQClipboard *cb = TQApplication::clipboard();
    cb->setPixmap( snapshot );
}

void KSnapshot::slotDragSnapshot()
{
    TQDragObject *drobj = new TQImageDrag(snapshot.convertToImage(), this);
    drobj->setPixmap(mainWidget->preview());
    drobj->dragCopy();
}

void KSnapshot::slotGrab()
{
    hide();

    if ( mainWidget->delay() && mainWidget->mode() != Region )
	grabTimer.start( mainWidget->delay() * 1000, true );
    else {
	if ( mainWidget->mode() == Region ) {
	    rgnGrab = new RegionGrabber();
	    connect( rgnGrab, TQT_SIGNAL( regionGrabbed( const TQPixmap & ) ),
		     TQT_SLOT( slotRegionGrabbed( const TQPixmap & ) ) );
	}
	else {
	    grabber->show();
	    grabber->grabMouse( crossCursor );
	}
    }
}

void KSnapshot::slotPrint()
{
    KPrinter printer;

    if (printer.setup(this, i18n("Print Screenshot")))
    {
	tqApp->processEvents();

        TQPainter painter(&printer);
        TQPaintDeviceMetrics metrics(painter.device());

	float w = snapshot.width();
	float dw = w - metrics.width();
	float h = snapshot.height();
	float dh = h - metrics.height();
	bool scale = false;

	if ( (dw > 0.0) || (dh > 0.0) )
	    scale = true;

	if ( scale ) {

	    TQImage img = snapshot.convertToImage();
	    tqApp->processEvents();

	    float newh, neww;
	    if ( dw > dh ) {
		neww = w-dw;
		newh = neww/w*h;
	    }
	    else {
		newh = h-dh;
		neww = newh/h*w;
	    }

	    img = img.smoothScale( int(neww), int(newh), TQ_ScaleMin );
	    tqApp->processEvents();

	    int x = (metrics.width()-img.width())/2;
	    int y = (metrics.height()-img.height())/2;

	    painter.drawImage( x, y, img);
	}
	else {
	    int x = (metrics.width()-snapshot.width())/2;
	    int y = (metrics.height()-snapshot.height())/2;
	    painter.drawPixmap( x, y, snapshot );
	}
    }

    tqApp->processEvents();
}

void KSnapshot::slotRegionGrabbed( const TQPixmap &pix )
{
  if ( !pix.isNull() )
  {
    snapshot = pix;
    updatePreview();
    modified = true;
    updateCaption();
  }

  delete rgnGrab;
  TQApplication::restoreOverrideCursor();
  show();
}

void KSnapshot::slotWindowGrabbed( const TQPixmap &pix )
{
    if ( !pix.isNull() )
    {
        snapshot = pix;
        updatePreview();
        modified = true;
        updateCaption();
    }

    TQApplication::restoreOverrideCursor();
    show();
}

void KSnapshot::slotOpenWith(int id)
{
    openWithExternalApp(*openWithOffers[id]);
}

void KSnapshot::slotOpenWithKP() {
    KService::Ptr kpaint = KService::serviceByDesktopName("kolourpaint");
    if (kpaint) {
        openWithExternalApp(*kpaint);
    }
}

void KSnapshot::openWithExternalApp(const KService &service) {
    // Write snapshot to temporary file
    bool ok = false;
    KTempFile *tmpFile = new KTempFile;
    if (tmpFile->status() == 0) {
        if (snapshot.save(tmpFile->file(), "PNG")) {
            if (tmpFile->close()) {
                ok = true;
            }
        }
    }

    if (!ok) {
        KMessageBox::error(this, i18n("KSnapshot was unable to create temporary file."),
                                 i18n("Unable to save image"));
        delete tmpFile;
        return;
    }

    // Launch application
    KURL::List list;
    list.append(tmpFile->name());
    TQStringList args = KRun::processDesktopExec(service, list, false, false);

    TDEProcess *externalApp = new TDEProcess;
    *externalApp << args;
    connect(externalApp, SIGNAL(processExited(TDEProcess*)),
            this, SLOT(slotExternalAppClosed(TDEProcess*)));

    if (!externalApp->start(TDEProcess::OwnGroup)) {
        KMessageBox::error(this, i18n("Cannot start %1!").arg(service.name()));
        delete tmpFile;
        return;
    }

    m_tmpFiles[externalApp] = tmpFile;
}

void KSnapshot::slotExternalAppClosed(TDEProcess *process)
{
	if (process && m_tmpFiles.contains(process))
	{
		KTempFile *tmpFile = m_tmpFiles[process];
		if (tmpFile)
		{
			snapshot.load(tmpFile->name());
			updatePreview();
			tmpFile->unlink();
			delete tmpFile;
		}
		m_tmpFiles.remove(process);
	}
}

void KSnapshot::slotAboutToQuit()
{
	for (KTempFile *tmpFile : m_tmpFiles)
	{
		tmpFile->unlink();
		delete tmpFile;
	}
	m_tmpFiles.clear();

	TDEConfig *conf=TDEGlobal::config();
	conf->setGroup("GENERAL");
	conf->writeEntry("delay",mainWidget->delay());
	conf->writeEntry("mode",mainWidget->mode());
	conf->writeEntry("includeDecorations",mainWidget->includeDecorations());
	KURL url = filename;
	url.setPass( TQString() );
	conf->writePathEntry("filename",url.url());
}

void KSnapshot::closeEvent( TQCloseEvent * e )
{
	e->accept();
}

bool KSnapshot::eventFilter( TQObject* o, TQEvent* e)
{
    if ( TQT_BASE_OBJECT(o) == TQT_BASE_OBJECT(grabber) && e->type() == TQEvent::MouseButtonPress ) {
	TQMouseEvent* me = (TQMouseEvent*) e;
	if ( TQWidget::mouseGrabber() != grabber )
	    return false;
	if ( me->button() == Qt::LeftButton )
	    performGrab();
    }
    return false;
}

void KSnapshot::autoincFilename()
{
    // Extract the filename from the path
    TQString name= filename.fileName();

    // If the name contains a number then increment it
    TQRegExp numSearch("[0-9]+");

    // Does it have a number?
    int start = numSearch.search(name);
    if (start != -1) {
        // It has a number, increment it
        int len = numSearch.matchedLength();
	TQString numAsStr= name.mid(start, len);
	TQString number = TQString::number(numAsStr.toInt() + 1);
	number = number.rightJustify( len, '0');
	name.replace(start, len, number );
    }
    else {
        // no number
        start = name.findRev('.');
        if (start != -1) {
            // has a . somewhere, e.g. it has an extension
            name.insert(start, '1');
        }
        else {
            // no extension, just tack it on to the end
            name += '1';
        }
    }

    //Rebuild the path 
    KURL newURL = filename;
    newURL.setFileName( name );
    setURL( newURL.url() );
}

void KSnapshot::updatePreview()
{
    mainWidget->setPreview( snapshot );
}

void KSnapshot::grabTimerDone()
{
    if ( mainWidget->mode() == Region ) {
        rgnGrab = new RegionGrabber();
        connect( rgnGrab, TQT_SIGNAL( regionGrabbed( const TQPixmap & ) ),
            TQT_SLOT( slotRegionGrabbed( const TQPixmap & ) ) );
    }
    else {
	performGrab();
    }
    KNotifyClient::beep(i18n("The screen has been successfully grabbed."));
}

void KSnapshot::performGrab()
{
    grabber->releaseMouse();
    grabber->hide();
    grabTimer.stop();
    if ( mainWidget->mode() == ChildWindow ) {
	WindowGrabber wndGrab;
	connect( &wndGrab, TQT_SIGNAL( windowGrabbed( const TQPixmap & ) ),
	    TQT_SLOT( slotWindowGrabbed( const TQPixmap & ) ) );
	wndGrab.exec();
	}
    else if ( mainWidget->mode() == WindowUnderCursor ) {
	snapshot = WindowGrabber::grabCurrent( mainWidget->includeDecorations() );
    }
    else {
	snapshot = TQPixmap::grabWindow( tqt_xrootwin() );
    }
    updatePreview();
    TQApplication::restoreOverrideCursor();
    modified = true;
    updateCaption();
    show();
}

void KSnapshot::setTime(int newTime)
{
    mainWidget->setDelay(newTime);
}

int KSnapshot::timeout()
{
    return mainWidget->delay();
}

void KSnapshot::setURL( const TQString &url )
{
    KURL newURL = KURL::fromPathOrURL( url );
    if ( newURL == filename )
	return;

    filename = newURL;
    updateCaption();
}

void KSnapshot::setGrabMode( int m )
{
    mainWidget->setMode( m );
}

int KSnapshot::grabMode()
{
    return mainWidget->mode();
}

void KSnapshot::updateCaption()
{
    setCaption( kApp->makeStdCaption( filename.fileName(), true, modified ) );
}

void KSnapshot::slotMovePointer(int x, int y)
{
    TQCursor::setPos( x, y );
}

void KSnapshot::exit()
{
    reject();
}
#include "ksnapshot.moc"
