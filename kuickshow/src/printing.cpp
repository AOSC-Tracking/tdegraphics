/* This file is part of the KDE project
   Copyright (C) 2002 Carsten Pfeiffer <pfeiffer@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation, version 2.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include <tqcheckbox.h>
#include <tqfont.h>
#include <tqfontmetrics.h>
#include <tqgrid.h>
#include <tqhbox.h>
#include <tqlayout.h>
#include <tqimage.h>
#include <kimageeffect.h>
#include <tqpaintdevicemetrics.h>
#include <tqpainter.h>
#include <tqradiobutton.h>
#include <tqvbuttongroup.h>
#include <tqcolor.h>

#include <kcombobox.h>
#include <kdialog.h>
#include <klocale.h>
#include <kdebug.h>
#include <kglobalsettings.h>
#include <knuminput.h>
#include <kprinter.h>
#include <ktempfile.h>

#include "imagewindow.h"
#include "printing.h"
#include "version.h"

bool Printing::printImage( ImageWindow& imageWin, TQWidget *tqparent )
{
    TQString imageURL = imageWin.url().prettyURL();
    KPrinter printer;
    printer.setDocName( imageURL );
    printer.setCreator( "KuickShow-" KUICKSHOWVERSION );

    KPrinter::addDialogPage( new KuickPrintDialogPage( tqparent, "kuick page"));

    if ( printer.setup( tqparent, i18n("Print %1").tqarg(printer.docName().section('/', -1)) ) )
    {
        KTempFile tmpFile( TQString(), ".png" );
        if ( tmpFile.status() == 0 )
        {
            tmpFile.setAutoDelete( true );
            if ( imageWin.saveImage( tmpFile.name(), true ) )
                return printImageWithTQt( tmpFile.name(), printer,
                                         imageURL );
        }

        return false;
    }

    return true; // user aborted
}

bool Printing::printImageWithTQt( const TQString& filename, KPrinter& printer,
                                 const TQString& originalFileName )
{
    TQImage image( filename );
    if ( image.isNull() ) {
        kdWarning() << "Can't load image: " << filename << " for printing.\n";
        return false;
    }

    TQPainter p;
    p.begin( &printer );

    TQPaintDeviceMetrics metrics( &printer );
    p.setFont( KGlobalSettings::generalFont() );
    TQFontMetrics fm = p.fontMetrics();

    int w = metrics.width();
    int h = metrics.height();

    TQString t = "true";
    TQString f = "false";

    // Black & white print?
    if ( printer.option( "app-kuickshow-blackwhite" ) != f) {
        image = image.convertDepth( 1, Qt::MonoOnly | Qt::ThresholdDither | Qt::AvoidDither );
    }

    int filenameOffset = 0;
    bool printFilename = printer.option( "app-kuickshow-printFilename" ) != f;
    if ( printFilename ) {
        filenameOffset = fm.lineSpacing() + 14;
        h -= filenameOffset; // filename goes into one line!
    }

    //
    // shrink image to pagesize, if necessary
    //
    bool shrinkToFit = (printer.option( "app-kuickshow-shrinkToFit" ) != f);
    TQSize imagesize = image.size();
    if ( shrinkToFit && (image.width() > w || image.height() > h) ) {
        imagesize.tqscale( w, h, TQSize::ScaleMin );
    }


    //
    // align image
    //
    bool ok = false;
    int tqalignment = printer.option("app-kuickshow-tqalignment").toInt( &ok );
    if ( !ok )
        tqalignment = TQt::AlignCenter; // default

    int x = 0;
    int y = 0;

    // ### need a GUI for this in KuickPrintDialogPage!
    // x - tqalignment
    if ( tqalignment & TQt::AlignHCenter )
        x = (w - imagesize.width())/2;
    else if ( tqalignment & TQt::AlignLeft )
        x = 0;
    else if ( tqalignment & TQt::AlignRight )
        x = w - imagesize.width();

    // y - tqalignment
    if ( tqalignment & TQt::AlignVCenter )
        y = (h - imagesize.height())/2;
    else if ( tqalignment & TQt::AlignTop )
        y = 0;
    else if ( tqalignment & TQt::AlignBottom )
        y = h - imagesize.height();

    //
    // perform the actual drawing
    //
    p.drawImage( TQRect( x, y, imagesize.width(), imagesize.height()), image );

    if ( printFilename )
    {
        TQString fname = minimizeString( originalFileName, fm, w );
        if ( !fname.isEmpty() )
        {
            int fw = fm.width( fname );
            int x = (w - fw)/2;
            int y = metrics.height() - filenameOffset/2;
            p.drawText( x, y, fname );
        }
    }

    p.end();

    return true;
}

TQString Printing::minimizeString( TQString text, const TQFontMetrics&
                                  metrics, int maxWidth )
{
    if ( text.length() <= 5 )
        return TQString(); // no sense to cut that tiny little string

    bool changed = false;
    while ( metrics.width( text ) > maxWidth )
    {
        int mid = text.length() / 2;
        text.remove( mid, 2 ); // remove 2 characters in the middle
        changed = true;
    }

    if ( changed ) // add "..." in the middle
    {
        int mid = text.length() / 2;
        if ( mid <= 5 ) // sanity check
            return TQString();

        text.tqreplace( mid - 1, 3, "..." );
    }

    return text;
}


///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////


KuickPrintDialogPage::KuickPrintDialogPage( TQWidget *tqparent, const char *name )
    : KPrintDialogPage( tqparent, name )
{
    setTitle( i18n("Image Settings") );

    TQVBoxLayout *tqlayout = new TQVBoxLayout( this );
    tqlayout->setMargin( KDialog::marginHint() );
    tqlayout->setSpacing( KDialog::spacingHint() );

    m_addFileName = new TQCheckBox( i18n("Print fi&lename below image"), this);
    m_addFileName->setChecked( true );
    tqlayout->addWidget( m_addFileName );

    m_blackwhite = new TQCheckBox ( i18n("Print image in &black and white"), this);
    m_blackwhite->setChecked( false );
    tqlayout->addWidget (m_blackwhite );

    TQVButtonGroup *group = new TQVButtonGroup( i18n("Scaling"), this );
    group->setRadioButtonExclusive( true );
    tqlayout->addWidget( group );
    // m_shrinkToFit = new TQRadioButton( i18n("Shrink image to &fit, if necessary"), group );
    m_shrinkToFit = new TQCheckBox( i18n("Shrink image to &fit, if necessary"), group );
    m_shrinkToFit->setChecked( true );

    TQWidget *widget = new TQWidget( group );
    TQGridLayout *grid = new TQGridLayout( widget, 3, 3 );
    grid->addColSpacing( 0, 30 );
    grid->setColStretch( 0, 0 );
    grid->setColStretch( 1, 1 );
    grid->setColStretch( 2, 10 );

    m_scale = new TQRadioButton( i18n("Print e&xact size: "), widget );
    m_scale->setEnabled( false ); // ###
    grid->addMultiCellWidget( m_scale, 0, 0, 0, 1 );
    group->insert( m_scale );
    connect( m_scale, TQT_SIGNAL( toggled( bool )), TQT_SLOT( toggleScaling( bool )));

    m_units = new KComboBox( false, widget, "unit combobox" );
    grid->addWidget( m_units, 0, 2, AlignLeft );
    m_units->insertItem( i18n("Millimeters") );
    m_units->insertItem( i18n("Centimeters") );
    m_units->insertItem( i18n("Inches") );

    m_width = new KIntNumInput( widget, "exact width" );
    grid->addWidget( m_width, 1, 1 );
    m_width->setLabel( i18n("&Width:" ) );
    m_width->setMinValue( 1 );

    m_height = new KIntNumInput( widget, "exact height" );
    grid->addWidget( m_height, 2, 1 );
    m_height->setLabel( i18n("&Height:" ) );
    m_height->setMinValue( 1 );
}

KuickPrintDialogPage::~KuickPrintDialogPage()
{
}

void KuickPrintDialogPage::getOptions( TQMap<TQString,TQString>& opts,
                                       bool /*incldef*/ )
{
    TQString t = "true";
    TQString f = "false";

//    ### opts["app-kuickshow-tqalignment"] = ;
    opts["app-kuickshow-printFilename"] = m_addFileName->isChecked() ? t : f;
    opts["app-kuickshow-blackwhite"] = m_blackwhite->isChecked() ? t : f;
    opts["app-kuickshow-shrinkToFit"] = m_shrinkToFit->isChecked() ? t : f;
    opts["app-kuickshow-scale"] = m_scale->isChecked() ? t : f;
    opts["app-kuickshow-scale-unit"] = m_units->currentText();
    opts["app-kuickshow-scale-width-pixels"] = TQString::number( scaleWidth() );
    opts["app-kuickshow-scale-height-pixels"] = TQString::number( scaleHeight() );
}

void KuickPrintDialogPage::setOptions( const TQMap<TQString,TQString>& opts )
{
    TQString t = "true";
    TQString f = "false";

    m_addFileName->setChecked( opts["app-kuickshow-printFilename"] != f );
    // This sound strange, but if I copy the code on the line above, the checkbox
    // was always checked. And this isn't the wanted behavior. So, with this works.
    // KPrint magic ;-)
    m_blackwhite->setChecked ( false );
    m_shrinkToFit->setChecked( opts["app-kuickshow-shrinkToFit"] != f );
    m_scale->setChecked( opts["app-kuickshow-scale"] == t );

    m_units->setCurrentItem( opts["app-kuickshow-scale-unit"] );

    bool ok;
    int val = opts["app-kuickshow-scale-width-pixels"].toInt( &ok );
    if ( ok )
        setScaleWidth( val );
    val = opts["app-kuickshow-scale-height-pixels"].toInt( &ok );
    if ( ok )
        setScaleHeight( val );

    if ( m_scale->isChecked() == m_shrinkToFit->isChecked() )
        m_shrinkToFit->setChecked( !m_scale->isChecked() );

    // ### re-enable when implementednn
     toggleScaling( false && m_scale->isChecked() );
}

void KuickPrintDialogPage::toggleScaling( bool enable )
{
    m_width->setEnabled( enable );
    m_height->setEnabled( enable );
    m_units->setEnabled( enable );
}

int KuickPrintDialogPage::scaleWidth() const
{
    return fromUnitToPixels( m_width->value() );
}

int KuickPrintDialogPage::scaleHeight() const
{
    return fromUnitToPixels( m_height->value() );
}

void KuickPrintDialogPage::setScaleWidth( int pixels )
{
    m_width->setValue( (int) pixelsToUnit( pixels ) );
}

void KuickPrintDialogPage::setScaleHeight( int pixels )
{
    m_width->setValue( (int) pixelsToUnit( pixels ) );
}

int KuickPrintDialogPage::fromUnitToPixels( float /*value*/ ) const
{
    return 1; // ###
}

float KuickPrintDialogPage::pixelsToUnit( int /*pixels*/ ) const
{
    return 1.0; // ###
}

#include "printing.moc"
