/* This file is part of the KDE Project
   Copyright (C) 2000 Klaas Freitag <freitag@suse.de>

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
#include <tqpushbutton.h>
#include <tqslider.h>
#include <tqlineedit.h>
#include <tqcombobox.h>

#include <kscanslider.h>
#include <tdelocale.h>
#include <kdebug.h>

#include "gammadialog.h"

GammaDialog::GammaDialog( TQWidget *parent ) :
   KDialogBase( parent,  "GammaDialog", true, i18n("Custom Gamma Tables"),
		 Ok|Cancel|Apply, Ok, true )
{
    gt = new KGammaTable();
    TQWidget *page = new TQWidget( this );

    TQ_CHECK_PTR( page );
    setMainWidget( page );

    /* This connect is for recalculating the table every time a new
     * Bright., Contrast or Gamma-Value is set */
    connect( gt, TQ_SIGNAL(tableChanged()), gt, TQ_SLOT(getTable()));

    gtDisp = new DispGamma( page );
    gtDisp->setValueRef( gt->getArrayPtr() );
    gtDisp->resize( 280, 280 );

    connect( gt, TQ_SIGNAL(tableChanged()), gtDisp, TQ_SLOT( repaint()));

    // setCaption( i18n( "Gamma Table" ));

    // Layout-Boxes
    TQVBoxLayout *bigdad    = new TQVBoxLayout( page, 10 );
    TQHBoxLayout *lhMiddle  = new TQHBoxLayout( 5 );
    TQVBoxLayout *lvSliders = new TQVBoxLayout( 10 );

    TQLabel *l_top = new TQLabel( i18n( "<B>Edit the custom gamma table</B><BR>This gamma table is passed to the scanner hardware." ), page );
    bigdad->addWidget( l_top, 1 );
    bigdad->addLayout( lhMiddle, 6 );

    lhMiddle->addLayout( lvSliders, 3);
    lhMiddle->addWidget( gtDisp, 2 );

    /* Slider Widgets for gamma, brightness, contrast */
    wBright   = new KScanSlider ( page, i18n("Brightness"), -50.0, 50.0 );
    TQ_CHECK_PTR(wBright);
    wBright->slSetSlider( 0 );
    connect( wBright, TQ_SIGNAL(valueChanged(int)), gt, TQ_SLOT(setBrightness(int)));

    wContrast = new KScanSlider ( page, i18n("Contrast") , -50.0, 50.0 );
    TQ_CHECK_PTR(wContrast);
    wContrast->slSetSlider( 0 );
    connect( wContrast, TQ_SIGNAL(valueChanged(int)), gt, TQ_SLOT(setContrast(int)));

    wGamma    = new KScanSlider ( page, i18n("Gamma"),  30.0, 300.0 );
    TQ_CHECK_PTR(wGamma);
    wGamma->slSetSlider(100);
    connect( wGamma, TQ_SIGNAL(valueChanged(int)), gt, TQ_SLOT(setGamma(int)));

    /* and add the Sliders */
    lvSliders->addWidget( wBright,   1 );
    lvSliders->addWidget( wContrast, 1 );
    lvSliders->addWidget( wGamma,    1 );

    // Finished and Activate !
    bigdad->activate();
    resize( 480, 300 );
}


void GammaDialog::setGt(KGammaTable& ngt)
{
   *gt = ngt;

   if( wBright ) wBright->slSetSlider( gt->getBrightness() );
   if( wContrast ) wContrast->slSetSlider( gt->getContrast() );
   if( wGamma ) wGamma->slSetSlider( gt->getGamma() );

}

void GammaDialog::slotApply()
{
   /* and call a signal */
   KGammaTable *myTable = getGt();
   emit( gammaToApply( myTable ));
}

GammaDialog::~GammaDialog()
{

}
#include "gammadialog.moc"
