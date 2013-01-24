/***************************************************************************
              kookaview.cpp  -  kookas visible stuff
                             -------------------
    begin                : ?
    copyright            : (C) 1999 by Klaas Freitag
    email                : freitag@suse.de

    $Id$
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *  This file may be distributed and/or modified under the terms of the    *
 *  GNU General Public License version 2 as published by the Free Software *
 *  Foundation and appearing in the file COPYING included in the           *
 *  packaging of this file.                                                *
 *
 *  As a special exception, permission is given to link this program       *
 *  with any version of the KADMOS ocr/icr engine of reRecognition GmbH,   *
 *  Kreuzlingen and distribute the resulting executable without            *
 *  including the source code for KADMOS in the source distribution.       *
 *
 *  As a special exception, permission is given to link this program       *
 *  with any edition of TQt, and distribute the resulting executable,       *
 *  without including the source code for TQt in the source distribution.   *
 *                                                                         *
 ***************************************************************************/

#include "kookaview.h"
#include "resource.h"
#include "kscandevice.h"
#include "imgscaninfo.h"
#include "devselector.h"
#include "ksaneocr.h"
#include "img_saver.h"
#include "kookapref.h"
#include "imgnamecombo.h"
#include "thumbview.h"
#include "dwmenuaction.h"
#include "kookaimage.h"
#include "kookaimagemeta.h"
#include "ocrresedit.h"
#include "kookaprint.h"
#include "imgprintdialog.h"
#if 0
#include "paramsetdialogs.h"
#endif
#include <tqlabel.h>
#include <tqpainter.h>
#include <tqlayout.h>
#include <tqsplitter.h>
#include <tqstrlist.h>
#include <tqpaintdevice.h>
#include <tqpaintdevicemetrics.h>
#include <tqpopupmenu.h>
#include <tqwidgetstack.h>

#include <kurl.h>
#include <krun.h>
#include <kapplication.h>
#include <kstatusbar.h>
#include <kconfig.h>
#include <kdebug.h>
#include <ktrader.h>
#include <klibloader.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <keditcl.h>
#include <kled.h>
#include <kcombobox.h>
#include <kaction.h>
#include <kiconloader.h>
#include <kshortcut.h>
#include <kdockwidget.h>
#include <tqobject.h>

#include <kparts/componentfactory.h>
#include <tqimage.h>
#include <kpopupmenu.h>


#define STARTUP_IMG_SELECTION   "SelectedImageOnStartup"


KookaView::KookaView( KParts::DockMainWindow *parent, const TQCString& deviceToUse)
   : TQObject(),
     m_ocrResultImg(0),
     ocrFabric(0),
     m_mainDock(0),
     m_dockScanParam(0),
     m_dockThumbs(0),
     m_dockPackager(0),
     m_dockRecent(0),
     m_dockPreview(0),
     m_dockOCRText(0),
     m_mainWindow(parent),
     m_ocrResEdit(0)
{
   KIconLoader *loader = TDEGlobal::iconLoader();
   scan_params = 0L;
   preview_canvas = 0L;

   m_mainDock = parent->createDockWidget( "Kookas MainDock",
                                          loader->loadIcon( "folder_image", KIcon::Small ),
                                          0L, i18n("Image Viewer"));
   m_mainDock->setEnableDocking(KDockWidget::DockNone );
   m_mainDock->setDockSite( KDockWidget::DockFullSite );

   parent->setView( m_mainDock);
   parent->setMainDockWidget( m_mainDock);

   img_canvas  = new ImageCanvas( m_mainDock );
   img_canvas->setMinimumSize(100,200);
   img_canvas->enableContextMenu(true);
   connect( img_canvas, TQT_SIGNAL( imageReadOnly(bool)),
	    this, TQT_SLOT(slViewerReadOnly(bool)));
   
   KPopupMenu *ctxtmenu = static_cast<KPopupMenu*>(img_canvas->contextMenu());
   if( ctxtmenu )
       ctxtmenu->insertTitle(i18n("Image View"));
   m_mainDock->setWidget( img_canvas );

   /** Thumbview **/
   m_dockThumbs = parent->createDockWidget( "Thumbs",
					    loader->loadIcon( "thumbnail", KIcon::Small ),
					    0L,  i18n("Thumbnails"));
   m_dockThumbs->setDockSite(KDockWidget::DockFullSite );

   /* thumbnail viewer widget */
   m_thumbview = new ThumbView( m_dockThumbs);
   m_dockThumbs->setWidget( m_thumbview );

   m_dockThumbs->manualDock( m_mainDock,              // dock target
			     KDockWidget::DockBottom, // dock site
			     20 );                  // relation target/this (in percent)

   /** Packager Dock **/
   /* A new packager to contain the already scanned images */
   m_dockPackager = parent->createDockWidget( "Scanpackager",
					    loader->loadIcon( "palette_color", KIcon::Small ),
					    0L, i18n("Gallery"));
   m_dockPackager->setDockSite(KDockWidget::DockFullSite);
   packager = new ScanPackager( m_dockPackager );
   m_dockPackager->setWidget( packager );
   m_dockPackager->manualDock( m_mainDock,              // dock target
                         KDockWidget::DockLeft, // dock site
                         30 );                  // relation target/this (in percent)


   connect( packager, TQT_SIGNAL(showThumbnails( KFileTreeViewItem* )),
	    this, TQT_SLOT( slShowThumbnails( KFileTreeViewItem* )));
   connect( m_thumbview, TQT_SIGNAL( selectFromThumbnail( const KURL& )),
	    packager, TQT_SLOT( slSelectImage(const KURL&)));

   /*
    * Create a Kombobox that holds the last folders visible even on the preview page
    */
   m_dockRecent  = parent->createDockWidget( "Recent",
					     loader->loadIcon( "image", KIcon::Small ),
					     0L, i18n("Gallery Folders"));

   m_dockRecent->setDockSite(KDockWidget::DockFullSite);

   TQHBox *recentBox = new TQHBox( m_dockRecent );
   recentBox->setMargin(KDialog::marginHint());
   TQLabel *lab = new TQLabel( i18n("Gallery:"), recentBox );
   lab->setSizePolicy( TQSizePolicy(TQSizePolicy::Fixed, TQSizePolicy::Fixed) );
   recentFolder = new ImageNameCombo( recentBox );

   m_dockRecent->setWidget( recentBox );
   m_dockRecent->manualDock( m_dockPackager,              // dock target
                         KDockWidget::DockBottom, // dock site
                         5 );                  // relation target/this (in percent)



   connect( packager,  TQT_SIGNAL( galleryPathSelected( KFileTreeBranch*, const TQString&)),
	    recentFolder, TQT_SLOT( slotGalleryPathChanged( KFileTreeBranch*, const TQString& )));

   connect( packager,  TQT_SIGNAL( directoryToRemove( KFileTreeBranch*, const TQString&)),
	    recentFolder, TQT_SLOT(   slotPathRemove( KFileTreeBranch*, const TQString& )));

   connect( recentFolder, TQT_SIGNAL(activated( const TQString& )),
	    packager, TQT_SLOT(slotSelectDirectory( const TQString& )));

   /* the object from the kscan lib to handle low level scanning */
   m_dockScanParam = parent->createDockWidget( "Scan Parameter",
 					     loader->loadIcon( "folder", KIcon::Small ),
 					     0L, i18n("Scan Parameter"));
   //
   m_dockScanParam->setDockSite(KDockWidget::DockFullSite);

   m_dockScanParam->setWidget( 0 ); // later
   sane = new KScanDevice( TQT_TQOBJECT(m_dockScanParam) );
   TQ_CHECK_PTR(sane);

   m_dockScanParam->manualDock( m_dockRecent,              // dock target
				KDockWidget::DockBottom, // dock site
				20 );                  // relation target/this (in percent)
   m_dockScanParam->hide();

   /* select the scan device, either user or from config, this creates and assembles
    * the complete scanner options dialog
    * scan_params must be zero for that */

   m_dockPreview = parent->createDockWidget( "Preview ",
					   loader->loadIcon( "viewmag", KIcon::Small ),
					   0L, i18n("Scan Preview"));

   preview_canvas = new Previewer( m_dockPreview );
   {
       preview_canvas->setMinimumSize( 100,100);

      /* since the scan_params will be created in slSelectDevice, do the
       * connections later
       */
   }
   m_dockPreview->setWidget( preview_canvas );
   m_dockPreview->manualDock( m_mainDock,              // dock target
			      KDockWidget::DockCenter, // dock site
			      100 );                  // relation target/this (in percent)

   /* Create a text editor part for ocr results */

   m_dockOCRText = parent->createDockWidget( "OCRResults",
                                             loader->loadIcon("edit", KIcon::Small ),
                                             0L, i18n("OCR Result Text"));
   // m_textEdit
   m_ocrResEdit  = new ocrResEdit( m_dockOCRText );

   if( m_ocrResEdit )
   {
       m_dockOCRText->setWidget( m_ocrResEdit ); // m_textEdit->widget() );
       m_dockOCRText->manualDock( m_dockThumbs,              // dock target
                                  KDockWidget::DockCenter, // dock site
                                  100 );                  // relation target/this (in percent)

       m_ocrResEdit->setTextFormat( TQt::PlainText );
       m_ocrResEdit->setWordWrap( TQTextEdit::NoWrap );
       // m_dockOCRText->hide();
   }

   if( slSelectDevice(deviceToUse))
   {
      /* Load from config which tab page was selected last time */
   }

   /* New image created after scanning */
   connect(sane, TQT_SIGNAL(sigNewImage(TQImage*,ImgScanInfo*)), this, TQT_SLOT(slNewImageScanned(TQImage*,ImgScanInfo*)));
   /* New preview image */
   connect(sane, TQT_SIGNAL(sigNewPreview(TQImage*,ImgScanInfo *)), this, TQT_SLOT( slNewPreview(TQImage*,ImgScanInfo *)));

   connect( sane, TQT_SIGNAL( sigScanStart() ), this, TQT_SLOT( slScanStart()));
   connect( sane, TQT_SIGNAL( sigScanFinished(KScanStat)), this, TQT_SLOT(slScanFinished(KScanStat)));
   connect( sane, TQT_SIGNAL( sigAcquireStart()), this, TQT_SLOT( slAcquireStart()));
   /* Image canvas should show a new document */
   connect( packager, TQT_SIGNAL( showImage( KookaImage* )),
            this,       TQT_SLOT( slShowAImage( KookaImage*)));

   connect( packager, TQT_SIGNAL( aboutToShowImage(const KURL&)),
	    this,       TQT_SLOT( slStartLoading( const KURL& )));

   /* Packager unloads the image */
   connect( packager, TQT_SIGNAL( unloadImage( KookaImage* )),
            this,       TQT_SLOT( slUnloadAImage( KookaImage*)));

   /* a image changed mostly through a image manipulation method like rotate */
   connect( packager,  TQT_SIGNAL( fileChanged( KFileItem* )),
	    m_thumbview, TQT_SLOT( slImageChanged( KFileItem* )));

   connect( packager, TQT_SIGNAL( fileRenamed( KFileItem*, const KURL& )),
            m_thumbview, TQT_SLOT( slImageRenamed( KFileItem*, const KURL& )));

   connect( packager,  TQT_SIGNAL( fileDeleted( KFileItem* )),
	    m_thumbview, TQT_SLOT( slImageDeleted( KFileItem* )));


   packager->openRoots();

   /* Status Bar */
   KStatusBar *statBar = m_mainWindow->statusBar();

   // statBar->insertItem(TQString("1"), SBAR_ZOOM,  0, true );
   statBar->insertItem( TQString("-"), StatusImage,  0, true );

   /* Set a large enough size */
   int w = statBar->fontMetrics().
           width(img_canvas->imageInfoString(2000, 2000, 48));
   kdDebug(28000) << "Fixed size for status bar: " << w << " from string " << img_canvas->imageInfoString(2000, 2000, 48) << endl;
   statBar->setItemFixed( StatusImage, w );

}


KookaView::~KookaView()
{
   saveProperties( TDEGlobal::config () );
   delete preview_canvas;

   kdDebug(28000)<< "Finished saving config data" << endl;
}

void KookaView::slViewerReadOnly( bool )
{
    /* retrieve actions that could change the image */
}


bool KookaView::slSelectDevice( const TQCString& useDevice )
{

   kdDebug(28000) << "Kookaview: select a device!" << endl;
   bool haveConnection = false;

   TQCString selDevice;
   /* in case useDevice is the term 'gallery', the user does not want to
    * connect to a scanner, but only work in gallery mode. Otherwise, try
    * to read the device to use from config or from a user dialog */
   if( useDevice != "gallery" )
   {
      selDevice =  useDevice;
      if( selDevice.isEmpty())
      {
	 selDevice = userDeviceSelection();
      }
   }

   if( !selDevice.isEmpty() )
   {
      kdDebug(28000) << "Opening device " << selDevice << endl;

      if( connectedDevice == selDevice ) {
	 kdDebug( 28000) << "Device " << selDevice << " is already selected!" << endl;
	 return( true );
      }

      if( scan_params )
      {
	 /* This deletes the existing scan_params^-object */
	 slCloseScanDevice();
      }

      /* This connects to the selected scanner */
      scan_params = new ScanParams( m_dockScanParam );
      TQ_CHECK_PTR(scan_params);

      if( sane->openDevice( selDevice ) == KSCAN_OK )
      {
         connect( scan_params,    TQT_SIGNAL( scanResolutionChanged( int, int )),
                  preview_canvas, TQT_SLOT( slNewScanResolutions( int, int )));

	 if( ! scan_params->connectDevice( sane ) )
	 {
	    kdDebug(28000) << "Connecting to the scanner failed :( ->TODO" << endl;
	 }
	 else
	 {
	    haveConnection = true;
	    connectedDevice = selDevice;

	    /* New Rectangle selection in the preview, now scanimge exists */
	    ImageCanvas *previewCanvas = preview_canvas->getImageCanvas();
	    connect( previewCanvas , TQT_SIGNAL( newRect(TQRect)),
		     scan_params, TQT_SLOT(slCustomScanSize(TQRect)));
	    connect( previewCanvas, TQT_SIGNAL( noRect()),
		     scan_params, TQT_SLOT(slMaximalScanSize()));
	    // connect( scan_params,    TQT_SIGNAL( scanResolutionChanged( int, int )),
            // 		     preview_canvas, TQT_SLOT( slNewScanResolutions( int, int )));
	    /* load the preview image */
	    if( preview_canvas )
	    {
	       preview_canvas->setPreviewImage( sane->loadPreviewImage() );

               /* Call this after the devic is actually open */
               preview_canvas->slConnectScanner( sane );
	    }
	 }
      }
      else
      {
	 kdDebug(28000) << "Could not open device <" << selDevice << ">" << endl;
	 scan_params->connectDevice(0);
      }

      /* show the widget again */

      m_dockScanParam->setWidget( scan_params );

      m_dockScanParam->show();
   }
   else
   {
      // no devices available or starting in gallery mode
      if( scan_params )
	 scan_params->connectDevice( 0L );
   }
   return( haveConnection );
}

TQCString KookaView::userDeviceSelection( ) const
{
   /* Human readable scanner descriptions */
   TQStringList hrbackends;

   /* a list of backends the scan backend knows */
   TQStrList backends = sane->getDevices();
   TQStrListIterator  it( backends );

   TQCString selDevice;
   if( backends.count() > 0 )
   {
      while( it )
      {
	 kdDebug( 28000 ) << "Found backend: " << it.current() << endl;
	 hrbackends.append( sane->getScannerName( it.current() ));
	 ++it;
      }

      /* allow the user to select one */
       DeviceSelector ds( 0, backends, hrbackends );
       selDevice = ds.getDeviceFromConfig( );

       if( selDevice.isEmpty() || selDevice.isNull() )
       {
	  kdDebug(29000) << "selDevice not found - starting selector!" << selDevice << endl;
	  if ( ds.exec() == TQDialog::Accepted )
	  {
	     selDevice = ds.getSelectedDevice();
	  }
       }
   }
   return( selDevice );
}


void KookaView::loadStartupImage( void )
{
   kdDebug( 28000) << "Starting to load startup image" << endl;

   /* Now set the configured stuff */
   KConfig *konf = TDEGlobal::config ();
   if( konf )
   {
      konf->setGroup(GROUP_STARTUP);
      bool wantReadOnStart = konf->readBoolEntry( STARTUP_READ_IMAGE, true );

      if( wantReadOnStart )
      {
	 TQString startup = konf->readPathEntry( STARTUP_IMG_SELECTION );

	 if( !startup.isEmpty() )
	 {
	    kdDebug(28000) << "Loading startup image !" << endl;
	    packager->slSelectImage( KURL(startup) );
	 }
      }
      else
      {
	 kdDebug(28000) << "Do not load startup image due to config value" << endl;
      }
   }
}


void KookaView::print()
{
    /* For now, print a single file. Later, print multiple images to one page */
    KookaImage *img = packager->getCurrImage();
    if ( !img )
        return;
    KPrinter printer; // ( true, pMode );
    printer.setUsePrinterResolution(true);
    printer.addDialogPage( new ImgPrintDialog( img ));

    if( printer.setup( m_mainWindow, i18n("Print %1").arg(img->localFileName().section('/', -1)) ))
    {
	KookaPrint kookaprint( &printer );
	kookaprint.printImage(img);
    }
}

void KookaView::slNewPreview( TQImage *new_img, ImgScanInfo * )
{
   if( new_img )
   {
      if( ! new_img->isNull() )
      {
	 /* flip preview to front */
	 m_dockPreview->makeDockVisible();
      }
      preview_canvas->newImage( new_img );
   }
}


bool KookaView::ToggleVisibility( int item )
{
   TQWidget *w = 0;
   bool    ret = false;

   switch( item )
   {
      case ID_VIEW_SCANPARAMS:
	 w = scan_params;
	 break;
      case ID_VIEW_POOL:
	 w = preview_canvas;
	 break;
      default:
	 w = 0;
   }

   if( w )
   {
      if( w->isVisible() )
      {
	 w->hide();
	 ret = false;
      }
      else
      {
	 w->show();
	 ret = true;
      }
   }
   return ret;
}


void KookaView::doOCRonSelection( void )
{
   emit( signalChangeStatusbar( i18n("Starting OCR on selection" )));

   KookaImage img;

   if( img_canvas->selectedImage(&img) )
   {
      startOCR( &img );
   }
   emit( signalCleanStatusbar() );
}

/* Does OCR on the entire picture */
void KookaView::doOCR( void )
{
   emit( signalChangeStatusbar( i18n("Starting OCR on the entire image" )));
    KookaImage *img = packager->getCurrImage();
   startOCR( img );
   emit( signalCleanStatusbar( ));
}

void KookaView::startOCR( KookaImage *img )
{
   if( img && ! img->isNull() )
   {
      if( ocrFabric == 0L )
      {
          ocrFabric = new KSANEOCR( m_mainDock, TDEGlobal::config() );
          ocrFabric->setImageCanvas( img_canvas );

          connect( ocrFabric, TQT_SIGNAL( newOCRResultText( const TQString& )),
                   m_ocrResEdit, TQT_SLOT(setText( const TQString& )));

	  connect( ocrFabric, TQT_SIGNAL( newOCRResultText( const TQString& )),
		   m_dockOCRText, TQT_SLOT( show() ));
	  
          connect( ocrFabric, TQT_SIGNAL( repaintOCRResImage( )),
                   img_canvas, TQT_SLOT(repaint()));

	  connect( ocrFabric, TQT_SIGNAL( clearOCRResultText()),
		   m_ocrResEdit, TQT_SLOT(clear()));

          connect( ocrFabric,    TQT_SIGNAL( updateWord(int, const TQString&, const TQString& )),
                   m_ocrResEdit, TQT_SLOT( slUpdateOCRResult( int, const TQString&, const TQString& )));

          connect( ocrFabric,    TQT_SIGNAL( ignoreWord(int, const ocrWord&)),
                   m_ocrResEdit, TQT_SLOT( slIgnoreWrongWord( int, const ocrWord& )));

          connect( ocrFabric, TQT_SIGNAL( markWordWrong(int, const ocrWord& )),
                   m_ocrResEdit, TQT_SLOT( slMarkWordWrong( int, const ocrWord& )));

          connect( ocrFabric,    TQT_SIGNAL( readOnlyEditor( bool )),
                   m_ocrResEdit, TQT_SLOT( setReadOnly( bool )));

          connect( ocrFabric,    TQT_SIGNAL( selectWord( int, const ocrWord& )),
                   m_ocrResEdit, TQT_SLOT( slSelectWord( int, const ocrWord& )));

      }

      TQ_CHECK_PTR( ocrFabric );
      ocrFabric->slSetImage( img );

      if( !ocrFabric->startOCRVisible(m_mainDock) )
      {
	 KMessageBox::sorry(0, i18n("Could not start OCR-Process.\n"
				    "Probably there is already one running." ));

      }
   }
}


void KookaView::slOCRResultImage( const TQPixmap& pix )
{
    kdDebug(28000) << "Showing OCR Result Image" << endl;
    if( ! img_canvas ) return;

    if( m_ocrResultImg )
    {
        img_canvas->newImage(0L);
        delete m_ocrResultImg;
    }

    m_ocrResultImg = new TQImage();
    *m_ocrResultImg = pix;
    img_canvas->newImage( m_ocrResultImg );
    img_canvas->setReadOnly(true); // ocr result images should be read only.
}

void KookaView::slScanStart( )
{
   kdDebug(28000) << "Scan starts " << endl;
   if( scan_params )
   {
      scan_params->setEnabled( false );
      KLed *led = scan_params->operationLED();
      if( led )
      {
	 led->setColor( TQt::red );
	 led->setState( KLed::On );
      }
   }
}

void KookaView::slAcquireStart( )
{
   kdDebug(28000) << "Acquire starts " << endl;
   if( scan_params )
   {
      KLed *led = scan_params->operationLED();
      if( led )
      {
	 led->setColor( TQt::green );
      }
   }
}

void KookaView::slNewImageScanned( TQImage* img, ImgScanInfo* si )
{
    KookaImageMeta *meta = new KookaImageMeta;
    meta->setScanResolution(si->getXResolution(), si->getYResolution());
    packager->slAddImage(img, meta);
}



void KookaView::slScanFinished( KScanStat stat )
{
   kdDebug(28000) << "Scan finished with status " << stat << endl;
   if( scan_params )
   {
      scan_params->setEnabled( true );
      KLed *led = scan_params->operationLED();
      if( led )
      {
	 led->setColor( TQt::green );
	 led->setState( KLed::Off );
      }
   }
}


void KookaView::slCloseScanDevice( )
{
   kdDebug(28000) << "Scanner Device closes down !" << endl;
   if( scan_params ) {
      delete scan_params;
      scan_params = 0;
      m_dockScanParam->setWidget(0L);
      m_dockScanParam->hide();
   }

   sane->slCloseDevice();
}

void KookaView::slCreateNewImgFromSelection()
{
   if( img_canvas->rootImage() )
   {
      emit( signalChangeStatusbar( i18n("Create new image from selection" )));
      TQImage img;
      if( img_canvas->selectedImage( &img ) )
      {
	 packager->slAddImage( &img );
      }
      emit( signalCleanStatusbar( ));
   }

}


void KookaView::slRotateImage(int angle)
{
    // TQImage *img = (TQImage*) img_canvas->rootImage();
   KookaImage *img = packager->getCurrImage();
   bool doUpdate = true;

   if( img )
   {
      TQImage resImg;

      TQApplication::setOverrideCursor(waitCursor);
      switch( angle )
      {
	 case 90:
	    emit( signalChangeStatusbar( i18n("Rotate image 90 degrees" )));
	    resImg = rotateRight( img );
	    break;
	 case 180:
	    emit( signalChangeStatusbar( i18n("Rotate image 180 degrees" )));
	    resImg = rotate180( img );
	    break;
	 case 270:
	 case -90:
	    emit( signalChangeStatusbar( i18n("Rotate image -90 degrees" )));
	    resImg = rotateLeft( img );

	    break;
	 default:
	    kdDebug(28000) << "Not supported yet !" << endl;
	    doUpdate = false;

	    break;
      }
      TQApplication::restoreOverrideCursor();

      /* updateCurrImage does the status-bar cleanup */
      if( doUpdate )
	 updateCurrImage( resImg );
      else
	 emit(signalCleanStatusbar());
   }

}



void KookaView::slMirrorImage( MirrorType m )
{
   const TQImage *img = img_canvas->rootImage();
   bool doUpdate = true;

   if( img )
   {
      TQImage resImg;

      TQApplication::setOverrideCursor(waitCursor);
      switch( m )
      {
	 case MirrorVertical:
	    emit( signalChangeStatusbar( i18n("Mirroring image vertically" )));
	    resImg = img->mirror();
	    break;
	 case MirrorHorizontal:
	    emit( signalChangeStatusbar( i18n("Mirroring image horizontally" )));
	    resImg = img->mirror( true, false );
	    break;
	 case MirrorBoth:
	    emit( signalChangeStatusbar( i18n("Mirroring image in both directions" )));
	    resImg = img->mirror( true, true );
	    break;
	 default:
	    kdDebug(28000) << "Mirroring: no way ;)" << endl;
	    doUpdate = false;
      }
      TQApplication::restoreOverrideCursor();

      /* updateCurrImage does the status-bar cleanup */
      if( doUpdate )
	 updateCurrImage( resImg );
      else
	 emit(signalCleanStatusbar());

      // img_canvas->newImage(  );
   }
}


void KookaView::slSaveOCRResult()
{
    if( ! m_ocrResEdit ) return;
    m_ocrResEdit->slSaveText();

}


void KookaView::slLoadScanParams( )
{
   if( ! sane ) return;
#if 0
   /* not yet cooked */
   LoadSetDialog loadDialog( m_mainDock, sane->shortScannerName(), sane );
   if( loadDialog.exec())
   {
      kdDebug(28000)<< "Executed successfully" << endl;
   }
#endif
}

void KookaView::slSaveScanParams( )
{
   if( !sane ) return;

   /* not yet cooked */
#if 0
   KScanOptSet optSet( "SaveSet" );

   sane->getCurrentOptions( &optSet );
   SaveSetDialog dialog( m_mainDock /* this */ , &optSet );
   if( dialog.exec())
   {
      kdDebug(28000)<< "Executed successfully" << endl;
      TQString name = dialog.paramSetName();
      TQString desc = dialog.paramSetDescription();
      sane->slSaveScanConfigSet( name, desc );
   }
#endif
}

void KookaView::slShowAImage( KookaImage *img )
{
   kdDebug(28000) << "Show new Image" << endl;
   if( img_canvas )
   {
      img_canvas->newImage( img );
      img_canvas->setReadOnly(false);
   }

   /* tell ocr about */
   if( ocrFabric )
   {
       ocrFabric->slSetImage( img );
   }

   /* Status Bar */
   KStatusBar *statBar = m_mainWindow->statusBar();
   if( img_canvas )
       statBar->changeItem( img_canvas->imageInfoString(), StatusImage );
}

void KookaView::slUnloadAImage( KookaImage * )
{
   kdDebug(28000) << "Unloading Image" << endl;
   if( img_canvas )
   {
      img_canvas->newImage( 0L );
   }
}


void KookaView::slShowThumbnails(KFileTreeViewItem *dirKfi, bool forceRedraw )
{
   /* If no item is specified, use the current one */
   if( ! dirKfi )
   {
      /* do on the current visible dir */
      KFileTreeViewItem *kftvi = packager->currentKFileTreeViewItem();
      if ( !kftvi )
      {
          return;
      }
      
      if( kftvi->isDir())
      {
          dirKfi = kftvi;
      }
      else
      {
	 kftvi = static_cast<KFileTreeViewItem*>(static_cast<TQListViewItem*>(kftvi)->parent());
	 dirKfi = kftvi;
	 forceRedraw = true;
	 packager->setSelected( static_cast<TQListViewItem*>(dirKfi), true );
      }
   }

   kdDebug(28000) << "Showing thumbs for " << dirKfi->url().prettyURL() << endl;

   /* Only do the new thumbview if the old is on another dir */
   if( m_thumbview && (forceRedraw || m_thumbview->currentDir() != dirKfi->url()) )
   {
      m_thumbview->clear();
      /* Find a list of child KFileItems */
      if( forceRedraw ) m_thumbview->readSettings();

      KFileItemList fileItemsList;

      TQListViewItem * myChild = dirKfi->firstChild();
      while( myChild )
      {
         fileItemsList.append( static_cast<KFileTreeViewItem*>(myChild)->fileItem());
         myChild = myChild->nextSibling();
      }

      m_thumbview->slNewFileItems( fileItemsList );
      m_thumbview->setCurrentDir( dirKfi->url());
      // m_thumbview->arrangeItemsInGrid();
   }

}

/* this slot is called when the user clicks on an image in the packager
 * and loading of the image starts
 */
void KookaView::slStartLoading( const KURL& url )
{
   emit( signalChangeStatusbar( i18n("Loading %1" ).arg( url.prettyURL() ) ));

   // if( m_stack->visibleWidget() != img_canvas )
   // {
   //    m_stack->raiseWidget( img_canvas );
   // }

}


void KookaView::updateCurrImage( TQImage& img )
{
    if( ! img_canvas->readOnly() )
    {
	emit( signalChangeStatusbar( i18n("Storing image changes" )));
	packager->slotCurrentImageChanged( &img );
	emit( signalCleanStatusbar());
    }
    else
    {
	emit( signalChangeStatusbar( i18n("Can not save image, it is write protected!")));
	kdDebug(28000) << "Image is write protected, no saving!" << endl;
    }
}


void KookaView::saveProperties(KConfig *config)
{
   kdDebug(28000) << "Saving Properties for KookaView !" << endl;
   config->setGroup( GROUP_STARTUP );
   /* Get with path */
   config->writePathEntry( STARTUP_IMG_SELECTION, packager->getCurrImageFileName(true));

}


void KookaView::slOpenCurrInGraphApp( void )
{
   TQString file;

   if( packager )
   {
      KFileTreeViewItem *ftvi = packager->currentKFileTreeViewItem();

      if( ! ftvi ) return;

      kdDebug(28000) << "Trying to open <" << ftvi->url().prettyURL()<< ">" << endl;
      KURL::List urllist;

      urllist.append( ftvi->url());

      KRun::displayOpenWithDialog( urllist );
   }
}


TQImage KookaView::rotateLeft( TQImage *m_img )
{
   TQImage rot;
   
   if( m_img )
   {
       TQWMatrix m;

       m.rotate(-90);
       rot = m_img->xForm(m);
   }
   return( rot );
}

TQImage KookaView::rotateRight( TQImage *m_img )
{
   TQImage rot;
   
   if( m_img )
   {
       TQWMatrix m;

       m.rotate(+90);
       rot = m_img->xForm(m);
   }
   return( rot );
}

TQImage KookaView::rotate180( TQImage *m_img )
{
   TQImage rot;
   
   if( m_img )
   {
       TQWMatrix m;

       m.rotate(+180);
       rot = m_img->xForm(m);
   }
   return( rot );
}



void KookaView::connectViewerAction( KAction *action )
{
   TQPopupMenu *popup = img_canvas->contextMenu();
   kdDebug(29000) << "This is the popup: " << popup << endl;
   if( popup && action )
   {
      action->plug( popup );
   }
}

void KookaView::connectGalleryAction( KAction *action )
{
   TQPopupMenu *popup = packager->contextMenu();

   if( popup && action )
   {
      action->plug( popup );
   }
}

void KookaView::slFreshUpThumbView()
{
   if( m_thumbview )
   {
      /* readSettings returns true if something changes */
      if( m_thumbview->readSettings() )
      {
	 kdDebug(28000) << "Thumbview-Settings changed, readraw thumbs" << endl;
	 /* new settings */
	 slShowThumbnails(0, true);
      }
   }
}

void KookaView::createDockMenu( KActionCollection *col, KDockMainWindow *mainWin, const char * name )
{
   KActionMenu *actionMenu = new KActionMenu( i18n("Tool Views"), "view_icon", col, name );

   actionMenu->insert( new dwMenuAction( i18n("Show Image Viewer"),
					 KShortcut(), m_mainDock, col,
					 mainWin, "dock_viewer" ));

   actionMenu->insert( new dwMenuAction( i18n("Show Preview"),
					 KShortcut(), m_dockPreview, col,
					 mainWin, "dock_preview" ));

   actionMenu->insert( new dwMenuAction( i18n("Show Recent Gallery Folders"),
					 KShortcut(), m_dockRecent, col,
					 mainWin, "dock_recent" ));
   actionMenu->insert( new dwMenuAction( i18n("Show Gallery"),
					 KShortcut(), m_dockPackager, col,
					 mainWin, "dock_gallery" ));

   actionMenu->insert( new dwMenuAction( i18n("Show Thumbnail Window"),
					 KShortcut(), m_dockThumbs, col,
					 mainWin, "dock_thumbs" ));

   actionMenu->insert( new dwMenuAction( i18n("Show Scan Parameters"),
					 KShortcut(), m_dockScanParam, col,
					 mainWin, "dock_scanparams" ));

   actionMenu->insert( new dwMenuAction( i18n("Show OCR Results"),
					 KShortcut(), m_dockOCRText, col,
					 mainWin, "dock_ocrResults" ));
}


#include "kookaview.moc"
