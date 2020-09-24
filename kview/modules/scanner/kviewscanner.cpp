/*  This file is part of the KDE project
    Copyright (C) 2002 Matthias Kretz <kretz@kde.org>

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

// $Id$

#include "kviewscanner.h"

#include <tqimage.h>
#include <tqobjectlist.h>

#include <tdeaction.h>
#include <kinstance.h>
#include <tdelocale.h>
#include <kgenericfactory.h>
#include <kscan.h>
#include <tdemessagebox.h>
#include <kdebug.h>
#include <kimageviewer/viewer.h>

typedef KGenericFactory<KViewScanner> KViewScannerFactory;
K_EXPORT_COMPONENT_FACTORY( kview_scannerplugin, KViewScannerFactory( "kviewscannerplugin" ) )

KViewScanner::KViewScanner( TQObject* parent, const char* name,
	                                  const TQStringList & )
  : Plugin( parent, name ),
  m_pScandlg( 0 ),
  m_pViewer( 0 )
{
	TQObjectList * viewerList = parent->queryList( 0, "KImageViewer Part", false, false );
	m_pViewer = static_cast<KImageViewer::Viewer *>( viewerList->getFirst() );
	delete viewerList;
	if( m_pViewer )
	{
		(void) new TDEAction( i18n( "&Scan Image..." ), "scanner", 0,
							this, TQT_SLOT( slotScan() ),
							actionCollection(), "plugin_scan" );
	}
	else
		kdWarning( 4630 ) << "no KImageViewer interface found - the scanner plugin won't work" << endl;
}

KViewScanner::~KViewScanner()
{
}

void KViewScanner::slotScan()
{
	if( ! m_pScandlg )
	{
		m_pScandlg = KScanDialog::getScanDialog();
		if( m_pScandlg )
		{
			m_pScandlg->setMinimumSize( 300, 300 );

			connect( m_pScandlg, TQT_SIGNAL( finalImage( const TQImage &, int ) ),
					this, TQT_SLOT( slotImgScanned( const TQImage & ) ) );
		}
		else
		{
			KMessageBox::sorry( 0L,
					i18n( "You do not appear to have SANE support, or your scanner "
						"is not attached properly. Please check these items before "
						"scanning again." ),
					i18n( "No Scan-Service Available" ) );
			kdDebug( 4630 ) << "*** No Scan-service available, aborting!" << endl;
			return;
		}
	}

	if( m_pScandlg->setup() )
		m_pScandlg->show();
}

void KViewScanner::slotImgScanned( const TQImage & img )
{
	kdDebug( 4630 ) << "received an image from the scanner" << endl;
	m_pViewer->newImage( img );
}
#include "kviewscanner.moc"
