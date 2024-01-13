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
#include <tqpushbutton.h>
#include <tqspinbox.h>
#include <tqtooltip.h>
#include <tqcombobox.h>
#include <tqlabel.h>
#include <tqslider.h>
#include <tqlineedit.h>

#include <kiconloader.h>
#include <tdelocale.h>
#include <kdebug.h>
#include "kscanslider.h"

KScanSlider::KScanSlider( TQWidget *parent, const TQString& text,
			  double min, double max, bool haveStdButt,
			  int stdValue )
   : TQFrame( parent ),
     m_stdValue( stdValue ),
     m_stdButt(0)
{
    TQHBoxLayout *hb = new TQHBoxLayout( this );
    l1 = new TQLabel( text, this, "AUTO_SLIDER_LABEL" );
    hb->addWidget( l1,20 );

    if( haveStdButt )
    {
       TDEIconLoader *loader = TDEGlobal::iconLoader();
       m_stdButt = new TQPushButton( this );
       m_stdButt->setPixmap( loader->loadIcon( "edit-undo",TDEIcon::Small ));

       /* connect the button click to setting the value */
       connect( m_stdButt, TQ_SIGNAL(clicked()),
		this, TQ_SLOT(slRevertValue()));

       TQToolTip::add( m_stdButt,
		      i18n( "Revert value back to its standard value %1" ).arg( stdValue ));
       hb->addWidget( m_stdButt, 0 );
       hb->addSpacing( 4 );
    }

    slider = new TQSlider( (int) min, (int)max, 1, (int)min, TQt::Horizontal, this, "AUTO_SLIDER_" );
    slider->setTickmarks( TQSlider::Below );
    slider->setTickInterval( int(TQMAX( (max-min)/10, 1 )) );
    slider->setSteps( int(TQMAX( (max-min)/20, 1) ), int(TQMAX( (max-min)/10, 1) ) );
    slider->setMinimumWidth( 140 );
    /* set a buddy */
    l1->setBuddy( slider );

    /* create a spinbox for displaying the values */
    m_spin = new TQSpinBox( (int) min, (int) max,
			   1, // step
			   this );


    /* make spin box changes change the slider */
    connect( m_spin, TQ_SIGNAL(valueChanged(int)), this, TQ_SLOT(slSliderChange(int)));

    /* Handle internal number display */
    // connect(slider, TQ_SIGNAL(valueChanged(int)), numdisp, TQ_SLOT( setNum(int) ));
    connect(slider, TQ_SIGNAL(valueChanged(int)), this, TQ_SLOT( slSliderChange(int) ));

    /* set Value 0 to the widget */
    slider->setValue( (int) min -1 );

    /* Add to layout widget and activate */
    hb->addWidget( slider, 36 );
    hb->addSpacing( 4 );
    hb->addWidget( m_spin, 0 );

    hb->activate();

}

void KScanSlider::setEnabled( bool b )
{
    if( slider )
	slider->setEnabled( b );
    if( l1 )
	l1->setEnabled( b );
    if( m_spin )
	m_spin->setEnabled( b );
    if( m_stdButt )
       m_stdButt->setEnabled( b );
}

void KScanSlider::slSetSlider( int value )
{
    /* Important to check value to avoid recursive signals ;) */
    // debug( "Slider val: %d -> %d", value, slider_val );
    kdDebug(29000) << "Setting Slider with " << value << endl;

    if( value == slider->value() )
    {
      kdDebug(29000) << "Returning because slider value is already == " << value << endl;
      return;
    }
    slider->setValue( value );
    slSliderChange( value );

}

void KScanSlider::slSliderChange( int v )
{
   // kdDebug(29000) << "Got slider val: " << v << endl;
    // slider_val = v;
    int spin = m_spin->value();
    if( v != spin )
       m_spin->setValue(v);
    int slid = slider->value();
    if( v != slid )
       slider->setValue(v);

    emit( valueChanged( v ));
}

void KScanSlider::slRevertValue()
{
   if( m_stdButt )
   {
      /* Only if stdButt is non-zero, the default value is valid */
      slSetSlider( m_stdValue );
   }
}


KScanSlider::~KScanSlider()
{
}

/* ====================================================================== */

KScanEntry::KScanEntry( TQWidget *parent, const TQString& text )
 : TQFrame( parent )
{
    TQHBoxLayout *hb = new TQHBoxLayout( this );

    TQLabel *l1 = new TQLabel( text, this, "AUTO_ENTRYFIELD" );
    hb->addWidget( l1,1 );

    entry = new TQLineEdit( this, "AUTO_ENTRYFIELD_E" );
    l1->setBuddy( entry );
    connect( entry, TQ_SIGNAL( textChanged(const TQString& )),
	     this, TQ_SLOT( slEntryChange(const TQString&)));
    connect( entry, TQ_SIGNAL( returnPressed()),
	     this,  TQ_SLOT( slReturnPressed()));

    hb->addWidget( entry,3 );
    hb->activate();
}

TQString  KScanEntry::text( void ) const
{
   TQString str = TQString();
   // kdDebug(29000) << "entry is "<< entry << endl;
   if(entry)
   {
       str = entry->text();
      if( ! str.isNull() && ! str.isEmpty())
      {
	 kdDebug(29000) << "KScanEntry returns <" << str << ">" << endl;
      }
      else
      {
	 kdDebug(29000) << "KScanEntry:  nothing entered !" << endl;
      }
   }
   else
   {
      kdDebug(29000) << "KScanEntry ERR: member var entry not defined!" << endl;
   }
   return ( str );
}

void KScanEntry::slSetEntry( const TQString& t )
{
    if( t == entry->text() )
	return;
    /* Important to check value to avoid recursive signals ;) */

    entry->setText( t );
}

void KScanEntry::slEntryChange( const TQString& t )
{
    emit valueChanged( TQCString( t.latin1() ) );
}

void KScanEntry::slReturnPressed( void )
{
   TQString t = text();
   emit returnPressed( TQCString( t.latin1()));
}



KScanCombo::KScanCombo( TQWidget *parent, const TQString& text,
			const TQStrList& list )
    : TQHBox( parent ),
      combo(0)
{
    createCombo( text );
    if( combo )
        combo->insertStrList( list);
    combolist = list;
}

KScanCombo::KScanCombo( TQWidget *parent, const TQString& text,
			const TQStringList& list )
    : TQHBox( parent ),
      combo(0)
{
    createCombo( text );
    if( combo )
        combo->insertStringList( list );

    for ( TQStringList::ConstIterator it = list.begin(); it != list.end(); ++it ) {
        combolist.append( (*it).local8Bit() );
    }
}


void KScanCombo::createCombo( const TQString& text )
{
    setSpacing( 12 );
    setMargin( 2 );


    (void) new TQLabel( text, this, "AUTO_COMBOLABEL" );

    combo = new TQComboBox( this, "AUTO_COMBO" );

    connect( combo, TQ_SIGNAL(activated( const TQString &)), this,
             TQ_SLOT( slComboChange( const TQString &)));
    connect( combo, TQ_SIGNAL(activated( int )),
	     this,  TQ_SLOT(slFireActivated(int)));

}


void KScanCombo::slSetEntry( const TQString &t )
{
    if( t.isNull() ) 	return;
    int i = combolist.find( t.local8Bit() );

    /* Important to check value to avoid recursive signals ;) */
    if( i == combo->currentItem() )
	return;

    if( i > -1 )
	combo->setCurrentItem( i );
    else
	kdDebug(29000) << "Combo item not in list !" << endl;
}

void KScanCombo::slComboChange( const TQString &t )
{
    emit valueChanged( TQCString( t.latin1() ) );
    kdDebug(29000) << "Combo: valueChanged emitted!" << endl;
}

void KScanCombo::slSetIcon( const TQPixmap& pix, const TQString& str)
{
   for( int i=0; i < combo->count(); i++ )
   {
      if( combo->text(i) == str )
      {
	 combo->changeItem( pix, str, i );
	 break;
      }
   }
}

TQString KScanCombo::currentText( void ) const
{
   return( combo->currentText() );
}


TQString KScanCombo::text( int i ) const
{
   return( combo->text(i) );
}

void    KScanCombo::setCurrentItem( int i )
{
   combo->setCurrentItem( i );
}

int KScanCombo::count( void ) const
{
   return( combo->count() );
}

void KScanCombo::slFireActivated( int i )
{
   emit( activated( i ));
}

#include "kscanslider.moc"
