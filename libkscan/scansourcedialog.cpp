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

#include "scansourcedialog.h"
#include "kscanslider.h"

#include <tdelocale.h>
#include <kdebug.h>

#include <tqlabel.h>
#include <tqpushbutton.h>
#include <tqlayout.h>
#include <tqvbox.h>
#include <tqhbox.h>
#include <tqradiobutton.h>
#include <tqslider.h>
#include <tqlineedit.h>
#include <tqcombobox.h>

#include <tqvbuttongroup.h>
#include <tqbuttongroup.h>


extern "C"{
#include <sane/saneopts.h>
}
#ifndef SANE_NAME_DOCUMENT_FEEDER
#define SANE_NAME_DOCUMENT_FEEDER "Automatic Document Feeder"
#endif


ScanSourceDialog::ScanSourceDialog( TQWidget *parent, const TQStrList list, ADF_BEHAVE adfBehave )
 : KDialogBase( parent, "SOURCE_DIALOG", true, i18n("Scan Source Selection"),
		Ok|Cancel,Ok, true)
{
   TQVBox *vbox = makeVBoxMainWidget();

   (void) new TQLabel( i18n("<B>Source selection</B><P>"
			   "Note that you may see more sources than actually exist"), vbox );

   /* Combo Box for sources */
   const TQStrList xx = list;
   sources = new KScanCombo( vbox,
			     i18n("Select the Scanner document source:"),
			     xx);
   connect( sources, TQ_SIGNAL( activated(int)), this, TQ_SLOT( slChangeSource(int)));


   /* Button Group for ADF-Behaviour */
   bgroup = 0;
   adf = ADF_OFF;

   if( sourceAdfEntry() > -1 )
   {
      bgroup = new TQVButtonGroup( i18n("Advanced ADF-Options"), vbox, "ADF_BGROUP" );

      connect( bgroup, TQ_SIGNAL(clicked(int)), this, TQ_SLOT( slNotifyADF(int)));

      /* Two buttons inside */
      TQRadioButton *rbADFTillEnd = new TQRadioButton( i18n("Scan until ADF reports out of paper"),
						  bgroup );
      bgroup->insert( rbADFTillEnd, ADF_SCAN_ALONG );

      TQRadioButton *rbADFOnce = new TQRadioButton( i18n("Scan only one sheet of ADF per click"),
					       bgroup );
      bgroup->insert( rbADFOnce, ADF_SCAN_ONCE );

      switch ( adfBehave )
      {
	 case ADF_OFF:
	    bgroup->setButton( ADF_SCAN_ONCE );
	    bgroup->setEnabled( false );
	    adf = ADF_OFF;
	    break;
	 case ADF_SCAN_ONCE:
	    bgroup->setButton( ADF_SCAN_ONCE );
	    adf = ADF_SCAN_ONCE;
	    break;
	 case ADF_SCAN_ALONG:
	    bgroup->setButton( ADF_SCAN_ALONG );
	    adf = ADF_SCAN_ALONG;
	    break;
	 default:
	    kdDebug(29000) << "Undefined Source !" << endl;
	    // Hmmm.
	    break;
      }
   }
}

TQString  ScanSourceDialog::getText( void ) const
{
   return( sources->currentText() );
}

void ScanSourceDialog::slNotifyADF( int )
{
   // debug( "reported adf-select %d", adf_group );
   /* this seems to be broken, adf_text is a visible string?
   *  needs rework if SANE 2 comes up which supports i18n */
#if 0
  TQString adf_text = getText();

  adf = ADF_OFF;

  if( adf_text == "Automatic Document Feeder" ||
      adf_text == "ADF" )
    {
      if( adf_group == 0 )
	adf = ADF_SCAN_ALONG;
      else
	adf = ADF_SCAN_ONCE;
    }
#endif
}


void ScanSourceDialog::slChangeSource( int i )
{
   if( ! bgroup ) return;

   if( i == sourceAdfEntry())
   {
      /* Adf was switched on */
      bgroup->setEnabled(  true );
      bgroup->setButton( 0 );
      adf = ADF_SCAN_ALONG;
      adf_enabled = true;
   }
   else
   {
      bgroup->setEnabled( false );
      // adf = ADF_OFF;
      adf_enabled = false;
   }
}



int ScanSourceDialog::sourceAdfEntry( void ) const
{
   if( ! sources ) return( -1 );

   int cou = sources->count();

   for( int i = 0; i < cou; i++ )
   {
      TQString q = sources->text( i );

#if 0
      if( q == "ADF" || q == SANE_NAME_DOCUMENT_FEEDER )
         return( i );
#endif

   }
   return( -1 );
}



void ScanSourceDialog::slSetSource( const TQString source )
{
   if( !sources  ) return;
   kdDebug(29000) << "Setting <" << source << "> as source" << endl;

   if( bgroup )
      bgroup->setEnabled( false );
   adf_enabled = false ;


   for( int i = 0; i < sources->count(); i++ )
   {
      if( sources->text( i ) == source )
      {
         sources->setCurrentItem( i );
         if( source == TQString::number(sourceAdfEntry()) )
         {
	    if( bgroup )
	       bgroup->setEnabled( true );
            adf_enabled = true;
         }
         break;
      }
   }

}


ScanSourceDialog::~ScanSourceDialog()
{

}

/* EOF */
#include "scansourcedialog.moc"
