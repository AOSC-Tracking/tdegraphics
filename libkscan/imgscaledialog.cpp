/* This file is part of the KDE Project
   Copyright (C) 1999 Klaas Freitag <freitag@suse.de>

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

#include <klocale.h>
#include <kdebug.h>
#include <tqbuttongroup.h>
#include <tqradiobutton.h>
#include <knumvalidator.h>
#include "imgscaledialog.h"

/* ############################################################################## */


ImgScaleDialog::ImgScaleDialog( TQWidget *tqparent, int curr_sel,
				const char *name )
   :KDialogBase( tqparent,  name , true, i18n("Zoom"),
                 Ok|Cancel, Ok, true )
{
   // setCaption (i18n ("Image Zoom"));
   selected = curr_sel;
   int        one_is_selected = false;
   enableButtonSeparator( false );

   // (void) new TQLabel( , main, "Page");
   //
   // makeMainWidget();
   TQButtonGroup *radios = new TQButtonGroup ( 2, Qt::Horizontal, this );
   setMainWidget(radios);
   Q_CHECK_PTR(radios);
   radios->setTitle( i18n("Select Image Zoom") );

   connect( radios, TQT_SIGNAL( clicked( int )),
	    this, TQT_SLOT( setSelValue( int )));

   // left gap: smaller Image
   TQRadioButton *rb25 = new TQRadioButton (i18n ("25 %"), radios);
   if( curr_sel == 25 ){
      rb25->setChecked( true );
      one_is_selected = true;
   }

   TQRadioButton *rb50 = new TQRadioButton (i18n ("50 %"), radios );
   if( curr_sel == 50 ){
      rb50->setChecked( true );
      one_is_selected = true;
   }

   TQRadioButton *rb75 = new TQRadioButton (i18n ("75 %"), radios );
   if( curr_sel == 75 ) {
      rb75->setChecked( true );
      one_is_selected = true;
   }

   TQRadioButton *rb100 = new TQRadioButton (i18n ("100 %"), radios);
   if( curr_sel == 100 ) {
      rb100->setChecked( true );
      one_is_selected = true;
   }

   TQRadioButton *rb150 = new TQRadioButton (i18n ("150 %"), radios);
   if( curr_sel == 150 ) {
      rb150->setChecked( true );
      one_is_selected = true;
   }

   TQRadioButton *rb200 = new TQRadioButton (i18n ("200 %"), radios );
   if( curr_sel == 200 ) {
      rb200->setChecked( true );
      one_is_selected = true;
   }

   TQRadioButton *rb300 = new TQRadioButton (i18n ("300 %"), radios );
   if( curr_sel == 300 ) {
      rb300->setChecked( true );
      one_is_selected = true;
   }

   TQRadioButton *rb400 = new TQRadioButton (i18n ("400 %"), radios);
   if( curr_sel == 400 ) {
      rb400->setChecked( true );
      one_is_selected = true;
   }

   // Custom Scaler at the bottom
   TQRadioButton *rbCust = new TQRadioButton (i18n ("Custom scale factor:"),
					    radios);
   if( ! one_is_selected )
      rbCust->setChecked( true );


   leCust = new TQLineEdit( radios );
   TQString sn;
   sn.setNum(curr_sel );
   leCust->setValidator( new KIntValidator( leCust ) );
   leCust->setText(sn );
   connect( leCust, TQT_SIGNAL( textChanged( const TQString& )),
	    this, TQT_SLOT( customChanged( const TQString& )));
   connect( rbCust, TQT_SIGNAL( toggled( bool )),
	    this, TQT_SLOT(enableAndFocus(bool)));
   leCust->setEnabled( rbCust->isChecked());


}

void ImgScaleDialog::customChanged( const TQString& s )
{
   bool ok;
   int  okval = s.toInt( &ok );
   if( ok && okval > 5 && okval < 1000 ) {
      selected = okval;
      emit( customScaleChange( okval ));
   } else {
      kdDebug(29000) << "ERR: To large, to smale, or whatever shitty !" << endl;
   }
}

// This slot is called, when the user changes the Scale-Selection
// in the button group. The value val is the index of the active
// button which is translated to the Scale-Size in percent.
// If custom size is selected, the ScaleSize is read from the
// TQLineedit.
//
void ImgScaleDialog::setSelValue( int val )
{
   const int translator[]={ 25, 50, 75, 100, 150,200,300, 400, -1 };
   const size_t translator_size = sizeof( translator ) / sizeof(int);
   int   old_sel = selected;

   // Check if value is in Range
   if( val >= 0 && val < (int) translator_size ) {
      selected = translator[val];

      // Custom size selected
      if( selected == -1 ) {
	 TQString s = leCust->text();

	 bool ok;
	 int  okval = s.toInt( &ok );
	 if( ok ) {
	    selected = okval;
	    emit( customScaleChange( okval ));
	 } else {
	    selected = old_sel;
	 }
      } // Selection is not custom
   } else {
      kdDebug(29000) << "ERR: Invalid size selected !" << endl;
   }
   // debug( "SetValue: Selected Scale is %d", selected );
}


int ImgScaleDialog::getSelected() const
{
   return( selected );
}


/* ############################################################################## */


#include "imgscaledialog.moc"
