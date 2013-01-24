/* This file is part of the KDE Project
   Copyright (C) 2001 Nikolas Zimmermann <wildfox@kde.org>
                      Klaas Freitag <freitag@suse.de>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include <tqlabel.h>
#include <tqlayout.h>
#include <tqstringlist.h>
#include <tqstrlist.h>
#include <tqtooltip.h>
#include <tqsizepolicy.h>
#include <tqapplication.h>
#include <tqcheckbox.h>

#include <kglobal.h>
#include <klocale.h>
#include <kdebug.h>
#include <kconfig.h>
#include <ksimpleconfig.h>
#include <kled.h>
#include <kglobalsettings.h>
#include <kscandevice.h>

// libkscan stuff
#include "scanparams.h"
#include "devselector.h"
#include "img_canvas.h"
#include "previewer.h"
#include "scandialog.h"

#define SCANDIA_SPLITTER_SIZES "ScanDialogSplitter %1"

extern "C" {
    void * init_libkscan() {
        return new ScanDialogFactory;
    }
}

ScanDialogFactory::ScanDialogFactory( TQObject *parent, const char *name )
    : KScanDialogFactory( parent, name )
{
    setName( "ScanDialogFactory" );
    TDEGlobal::locale()->insertCatalogue( TQString::fromLatin1("libkscan") );
}

KScanDialog * ScanDialogFactory::createDialog( TQWidget *parent,
					       const char *name, bool modal)
{
    return new ScanDialog( parent, name, modal );
}


///////////////////////////////////////////////////////////////////


ScanDialog::ScanDialog( TQWidget *parent, const char *name, bool modal )
   : KScanDialog( Tabbed, Close|Help, parent, name, modal ),
     good_scan_connect(false)
{
    TQVBox *page = addVBoxPage( i18n("&Scanning") );

    splitter = new TQSplitter(Qt::Horizontal, page, "splitter" );
    TQ_CHECK_PTR( splitter );

    m_scanParams = 0;
    m_device = new KScanDevice( TQT_TQOBJECT(this) );
    connect(m_device, TQT_SIGNAL(sigNewImage(TQImage *, ImgScanInfo*)),
            this, TQT_SLOT(slotFinalImage(TQImage *, ImgScanInfo *)));

    connect( m_device, TQT_SIGNAL(sigScanStart()), this, TQT_SLOT(slotScanStart()));
    connect( m_device, TQT_SIGNAL(sigScanFinished(KScanStat)),
	     this, TQT_SLOT(slotScanFinished(KScanStat)));
    connect( m_device, TQT_SIGNAL(sigAcquireStart()), this, TQT_SLOT(slotAcquireStart()));
    /* Create a preview widget to the right side of the splitter */
    m_previewer = new Previewer( splitter );
    TQ_CHECK_PTR(m_previewer );

    /* ... and connect to the selector-slots. They communicate user's
     * selection to the scanner parameter engine */
    /* a new preview signal */
    connect( m_device, TQT_SIGNAL( sigNewPreview( TQImage*, ImgScanInfo* )),
             this, TQT_SLOT( slotNewPreview( TQImage* )));

    m_previewer->setEnabled( false ); // will be enabled in setup()

    /* Options-page */
    createOptionsTab( );

}


void ScanDialog::createOptionsTab( void )
{

   TQVBox *page = addVBoxPage( i18n("&Options"));
   setMainWidget(page);

   TQGroupBox *gb = new TQGroupBox( 1, Qt::Horizontal, i18n("Startup Options"), page, "GB_STARTUP" );
   TQLabel *label = new TQLabel( i18n( "Note: changing these options will affect the scan plugin on next start." ),
			       gb );
   label->setSizePolicy( TQSizePolicy(TQSizePolicy::Expanding, TQSizePolicy::Fixed ) );

   /* Checkbox for asking for scanner on startup */
   cb_askOnStart = new TQCheckBox( i18n( "&Ask for the scan device on plugin startup"), gb );
   TQToolTip::add( cb_askOnStart,
		  i18n("You can uncheck this if you do not want to be asked which scanner to use on startup."));
   TQ_CHECK_PTR( cb_askOnStart );

   /* Checkbox for network access */
   cb_network = new TQCheckBox( i18n( "&Query the network for scan devices"), gb );
   TQToolTip::add( cb_network,
		  i18n("Check this if you want to query for configured network scan stations."));
   TQ_CHECK_PTR( cb_network );


   /* Read settings for startup behavior */
   KConfig *gcfg = TDEGlobal::config();
   gcfg->setGroup(TQString::fromLatin1(GROUP_STARTUP));
   bool skipDialog  = gcfg->readBoolEntry( STARTUP_SKIP_ASK, false );
   bool onlyLocal   = gcfg->readBoolEntry( STARTUP_ONLY_LOCAL, false );

   /* Note: flag must be inverted because of question is 'the other way round' */
   cb_askOnStart->setChecked( !skipDialog );
   connect( cb_askOnStart, TQT_SIGNAL(toggled(bool)), this, TQT_SLOT(slotAskOnStartToggle(bool)));

   cb_network->setChecked( !onlyLocal );
   connect( cb_network, TQT_SIGNAL(toggled(bool)), this, TQT_SLOT(slotNetworkToggle(bool)));


   TQWidget *spaceEater = new TQWidget( page );
   TQ_CHECK_PTR( spaceEater );
   spaceEater->setSizePolicy( TQSizePolicy(TQSizePolicy::Expanding, TQSizePolicy::Expanding) );

}

void ScanDialog::slotNetworkToggle( bool state)
{
   bool writestate = !state;

   kdDebug(29000) << "slotNetworkToggle: Writing state " << writestate << endl;
   KConfig *c = TDEGlobal::config();
   c->setGroup(TQString::fromLatin1(GROUP_STARTUP));
   c->writeEntry( STARTUP_ONLY_LOCAL, writestate, true, true );
}

void ScanDialog::slotAskOnStartToggle(bool state)
{
   bool writestate = !state;

   kdDebug(29000) << "slotAskOnStartToggle: Writing state " << writestate << endl;
   KConfig *c = TDEGlobal::config();
   c->setGroup(TQString::fromLatin1(GROUP_STARTUP));
   c->writeEntry( STARTUP_SKIP_ASK, writestate, true, true );
}

void ScanDialog::slotScanStart( )
{
   if( m_scanParams )
   {
      m_scanParams->setEnabled( false );
      KLed *led = m_scanParams->operationLED();
      if( led )
      {
	 led->setColor( TQt::red );
	 led->setState( KLed::On );
      }

   }
}

void ScanDialog::slotAcquireStart( )
{
   if( m_scanParams )
   {
      KLed *led = m_scanParams->operationLED();
      if( led )
      {
	 led->setColor( TQt::green );
      }

   }
}

void ScanDialog::slotScanFinished( KScanStat status )
{
   kdDebug(29000) << "Scan finished with status " << status << endl;
   if( m_scanParams )
   {
      m_scanParams->setEnabled( true );
      KLed *led = m_scanParams->operationLED();
      if( led )
      {
	 led->setColor( TQt::green );
	 led->setState( KLed::Off );
      }

   }
}

bool ScanDialog::setup()
{
   if( ! m_device )
   {
      good_scan_connect = false;
      return(false);
   }
   // The scan device is now closed on closing the scan dialog. That means
   // that more work to open it needs to be done in the setup slot like opening
   // the selector if necessary etc.

   if( m_scanParams )
   {
      /* if m_scanParams exist it means, that the dialog is already open */
      return true;
   }

   m_scanParams = new ScanParams( splitter );
   connect( m_previewer->getImageCanvas(), TQT_SIGNAL( newRect(TQRect)),
	    m_scanParams, TQT_SLOT(slCustomScanSize(TQRect)));
   connect( m_previewer->getImageCanvas(), TQT_SIGNAL( noRect()),
	    m_scanParams, TQT_SLOT(slMaximalScanSize()));

   connect( m_scanParams, TQT_SIGNAL( scanResolutionChanged( int, int )),
	    m_previewer, TQT_SLOT( slNewScanResolutions( int, int )));


   /* continue to attach a real scanner */
   /* first, get the list of available devices from libkscan */
   TQStringList scannerNames;
   TQStrList backends = m_device->getDevices();;
   TQStrListIterator it( backends );

   while ( it.current() ) {
      scannerNames.append( m_device->getScannerName( it.current() ));
      ++it;
   }

   /* ..if there are devices.. */
   TQCString configDevice;
   good_scan_connect = true;
   if( scannerNames.count() > 0 )
   {
      /* allow the user to select one */
      DeviceSelector ds( this, backends, scannerNames );
      configDevice = ds.getDeviceFromConfig( );

      if( configDevice.isEmpty() || configDevice.isNull() )
      {
	 kdDebug(29000) << "configDevice is not valid - starting selector!" << configDevice << endl;
	 if ( ds.exec() == TQDialog::Accepted )
	 {
	    configDevice = ds.getSelectedDevice();
	 }
      }

      /* If a device was selected, */
      if( ! configDevice.isNull() )
      {
	 /* ..open it (init sane with that) */
	 m_device->openDevice( configDevice );

	 /* ..and connect to the gui (create the gui) */
	 if ( !m_scanParams->connectDevice( m_device ) )
	 {
	    kdDebug(29000) << "ERR: Could not connect scan device" << endl;
	    good_scan_connect = false;
	 }
      }
   }

   if( configDevice.isNull() || configDevice.isEmpty() )
   {
      /* No scanner found, open with information */
      m_scanParams->connectDevice( 0L );
      good_scan_connect = false;
      /* Making the previewer gray */
      /* disabling is much better than hiding for 'advertising' ;) */
   }

   /* Move the scan params to the left, for backward compatibility.
    * However, having it on the right looks a bit better IMHO */
   if( splitter && m_scanParams )
      splitter->moveToFirst( m_scanParams );

   if( good_scan_connect )
   {
      m_previewer->setEnabled( true );
      m_previewer->setPreviewImage( m_device->loadPreviewImage() );
      m_previewer->slConnectScanner( m_device );
   }

    /* set initial sizes */
    setInitialSize( configDialogSize( GROUP_STARTUP ));

    KConfig *kfg = TDEGlobal::config();
    if( kfg )
    {
       TQRect r = TDEGlobalSettings::desktopGeometry(this);

       kfg->setGroup( GROUP_STARTUP );
       /* Since this is a vertical splitter, only the width is important */
       TQString key = TQString::fromLatin1( SCANDIA_SPLITTER_SIZES ).arg( r.width());
       kdDebug(29000) << "Read Splitter-Sizes " << key  << endl;
       splitter->setSizes( kfg->readIntListEntry( key ));
    }

   return true;
}

void ScanDialog::slotClose()
{
   /* Save the dialog start size to global configuration */
   saveDialogSize( GROUP_STARTUP, true );

   if( splitter )
   {
      KConfig *kfg = TDEGlobal::config();
      if( kfg )
      {
         TQRect r = TDEGlobalSettings::desktopGeometry(this);

	 kfg->setGroup( GROUP_STARTUP );
	 /* Since this is a vertical splitter, only the width is important */
	 TQString key = TQString::fromLatin1( SCANDIA_SPLITTER_SIZES ).arg( r.width());
	 kfg->writeEntry( key, splitter->sizes(), true, true);
      }
   }

   if( m_scanParams )
   {
      delete m_scanParams;
      m_scanParams =0;
   }
   if( m_device )
      m_device->slCloseDevice();
   else
      kdDebug(29000) << "ERR: no device exists :(" << endl;
      // bullshit happend
   accept();
}

void ScanDialog::slotNewPreview( TQImage *image )
{
    if( image )
    {
        m_previewImage = *image;
        // hmmm - dont know, if conversion of the bit-depth is necessary.
        // m_previewImage.convertDepth(32);

        /* The previewer does not copy the image data ! */
        m_previewer->newImage( &m_previewImage );
    }

}

ScanDialog::~ScanDialog()
{
}

void ScanDialog::slotFinalImage(TQImage *image, ImgScanInfo *)
{
    emit finalImage(*image, nextId());
}

#include "scandialog.moc"
