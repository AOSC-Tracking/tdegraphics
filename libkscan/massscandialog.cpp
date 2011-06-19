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

#include <tqlayout.h>
#include <tqlabel.h>
#include <tqprogressbar.h>
#include <tqgroupbox.h>
#include <tqframe.h>

#include <klocale.h>
#include <kdebug.h>
#include <kpushbutton.h>
#include <kstdguiitem.h>

#include "massscandialog.h"

MassScanDialog::MassScanDialog( TQWidget *tqparent )
   :TQDialog( tqparent, "MASS_SCAN", true )
{
   setCaption( i18n( "ADF Scanning" ));
   kdDebug(29000) << "Starting MassScanDialog!" << endl;
   // Layout-Boxes
   TQVBoxLayout *bigdad = new TQVBoxLayout( this, 5 );
   // TQHBoxLayout *hl1= new TQHBoxLayout( );      // Caption
   TQHBoxLayout *l_but  = new TQHBoxLayout( 10 );  // Buttons
 	
 	/* Caption */
 	TQLabel *l1 = new TQLabel( i18n( "<B>Mass Scanning</B>" ), this);
   bigdad->addWidget( l1, 1);
 	
 	/* Scan parameter information */
 	TQGroupBox *f1 = new TQGroupBox( i18n("Scan Parameter"), this );
 	f1->setFrameStyle( TQFrame::Box | TQFrame::Sunken );
 	f1->setMargin(5);
 	f1->setLineWidth( 1 );
   TQVBoxLayout *l_main = new TQVBoxLayout( f1, f1->frameWidth()+3, 3 );
 	bigdad->addWidget( f1, 6 );
 	
   scanopts = i18n("Scanning <B>%s</B> with <B>%d</B> dpi");
 	l_scanopts = new TQLabel( scanopts, f1 );
 	l_main->addWidget( l_scanopts );

   tofolder = i18n("Storing new images in folder <B>%s</B>");
 	l_tofolder = new TQLabel( tofolder, f1 );
 	l_main->addWidget( l_tofolder );
 	
 	/* Scan Progress information */
 	TQGroupBox *f2 = new TQGroupBox( i18n("Scan Progress"), this );
 	f2->setFrameStyle( TQFrame::Box | TQFrame::Sunken );
 	f2->setMargin(15);
 	f2->setLineWidth( 1 );
   TQVBoxLayout *l_pro = new TQVBoxLayout( f2, f2->frameWidth()+3, 3 );
 	bigdad->addWidget( f2, 6 );

 	TQHBoxLayout *l_scanp = new TQHBoxLayout( );
 	l_pro->addLayout( l_scanp, 5 );
   progress = i18n("Scanning page %1");
   l_progress = new TQLabel( progress, f2 );
   l_scanp->addWidget( l_progress, 3 );
 	l_scanp->addStretch( 1 );
   TQPushButton *pb_cancel_scan = new TQPushButton( i18n("Cancel Scan"), f2);
   l_scanp->addWidget( pb_cancel_scan,3 );

   progressbar = new TQProgressBar( 1000, f2 );
   l_pro->addWidget( progressbar, 3 );

 	/* Buttons to start scanning and close the Window */
  	bigdad->addLayout( l_but );

   TQPushButton *b_start = new TQPushButton( i18n("Start Scan"), this, "ButtOK" );
   connect( b_start, TQT_SIGNAL(clicked()), this, TQT_SLOT( slStartScan()) );

   TQPushButton *b_cancel = new TQPushButton( i18n("Stop"), this, "ButtCancel" );
   connect( b_cancel, TQT_SIGNAL(clicked()), this, TQT_SLOT(slStopScan()) );

   TQPushButton *b_finish = new KPushButton( KStdGuiItem::close(), this, "ButtFinish" );
   connect( b_finish, TQT_SIGNAL(clicked()), this, TQT_SLOT(slFinished()) );

   l_but->addWidget( b_start );
   l_but->addWidget( b_cancel );
   l_but->addWidget( b_finish );

   bigdad->activate();
   show();
}

MassScanDialog::~MassScanDialog()
{

}

void MassScanDialog::slStartScan( void )
{

}

void MassScanDialog::slStopScan( void )
{

}

void MassScanDialog::slFinished( void )
{
    delete this;
}

#include "massscandialog.moc"
