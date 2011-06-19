/***************************************************************************
                imgprintdialog.h  - Kooka's Image Printing
                             -------------------
    begin                : May 2003
    copyright            : (C) 1999 by Klaas Freitag
    email                : freitag@suse.de
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
#include "imgprintdialog.h"

#include <klocale.h>
#include <knuminput.h>
#include <kdialog.h>

#include <tqstring.h>
#include <tqmap.h>
#include <tqlayout.h>
#include <tqvbuttongroup.h>
#include <tqcheckbox.h>
#include <tqradiobutton.h>
#include "kookaimage.h"
#include <tqvgroupbox.h>
#include <tqpaintdevicemetrics.h>
#include <tqlabel.h>
#include <tqtooltip.h>
#include <kdebug.h>

#define ID_SCREEN 0
#define ID_ORIG   1
#define ID_CUSTOM 2
#define ID_FIT_PAGE 3

ImgPrintDialog::ImgPrintDialog( KookaImage *img, TQWidget *tqparent, const char* name )
    : KPrintDialogPage( tqparent, name ),
      m_image(img),
      m_ignoreSignal(false)
{
    setTitle(i18n("Image Printing"));
    TQVBoxLayout *tqlayout = new TQVBoxLayout( this );
    // tqlayout->setMargin( KDialog::marginHint() );
    // tqlayout->setSpacing( KDialog::spacingHint() );

    m_scaleRadios = new TQButtonGroup( 2, Qt::Vertical, i18n("Image Print Size"), this );
    m_scaleRadios->setRadioButtonExclusive(true);
    connect( m_scaleRadios, TQT_SIGNAL(clicked(int)), TQT_SLOT(slScaleChanged(int)));

    m_rbScreen = new TQRadioButton( i18n("Scale to same size as on screen"),
                                       m_scaleRadios );
    TQToolTip::add( m_rbScreen, i18n("Screen scaling. That prints according to the screen resolution."));

    m_scaleRadios->insert( m_rbScreen, ID_SCREEN );

    m_rbOrigSize = new TQRadioButton( i18n("Original size (calculate from scan resolution)"),
                                     m_scaleRadios );
    TQToolTip::add( m_rbOrigSize,
		   i18n("Calculates the print size from the scan resolution. Enter the scan resolution in the dialog field below." ));
    m_scaleRadios->insert( m_rbOrigSize, ID_ORIG );


    m_rbScale    = new TQRadioButton( i18n("Scale image to custom dimension"), m_scaleRadios );
    TQToolTip::add( m_rbScale,
		   i18n("Set the print size yourself in the dialog below. The image is centered on the paper."));
    
    m_scaleRadios->insert( m_rbScale, ID_CUSTOM );

    m_rbFitPage = new TQRadioButton( i18n("Scale image to fit to page"), m_scaleRadios );
    TQToolTip::add( m_rbFitPage, i18n("Printout uses maximum space on the selected pager. Aspect ratio is maintained."));
    m_scaleRadios->insert( m_rbFitPage, ID_FIT_PAGE );

    tqlayout->addWidget( m_scaleRadios );


    TQHBoxLayout *hbox = new TQHBoxLayout( this );
    tqlayout->addLayout( hbox );

    /** Box for Image Resolutions **/
    TQVGroupBox *group1 = new TQVGroupBox( i18n("Resolutions"), this );
    hbox->addWidget( group1 );

    /* Postscript generation resolution  */
    m_psDraft = new TQCheckBox( i18n("Generate low resolution PostScript (fast draft print)"),
				      group1, "cbPostScriptRes" );
    m_psDraft->setChecked( false );


    /* Scan resolution of the image */
    m_dpi = new KIntNumInput( group1 );
    m_dpi->setLabel( i18n("Scan resolution (dpi) " ), AlignVCenter );
    m_dpi->setValue( 300 );
    m_dpi->setSuffix( i18n(" dpi"));

    /* Label for displaying the screen Resolution */
    m_screenRes = new TQLabel( group1 );

    /** Box for Image Print Size **/
    TQVGroupBox *group = new TQVGroupBox( i18n("Image Print Size"), this );
    hbox->addWidget( group );

    m_sizeW = new KIntNumInput( group );
    m_sizeW->setLabel( i18n("Image width:"), AlignVCenter );
    m_sizeW->setSuffix( i18n(" mm"));
    connect( m_sizeW, TQT_SIGNAL(valueChanged(int)), TQT_SLOT(slCustomWidthChanged(int)));
    m_sizeH = new KIntNumInput( m_sizeW, AlignVCenter, group  );
    m_sizeH->setLabel( i18n("Image height:"), AlignVCenter);
    m_sizeH->setSuffix( i18n(" mm"));
    connect( m_sizeH, TQT_SIGNAL(valueChanged(int)), TQT_SLOT(slCustomHeightChanged(int)));

    m_ratio = new TQCheckBox( i18n("Maintain aspect ratio"), group, "cbAspectRatio" );
    m_ratio->setChecked(true);


    TQWidget *spaceEater = new TQWidget( this );
    spaceEater->tqsetSizePolicy( TQSizePolicy( TQSizePolicy::Ignored, TQSizePolicy::Ignored ));
    tqlayout->addWidget( spaceEater );

    /* Set start values */
    m_rbScreen->setChecked(true);
    slScaleChanged( ID_SCREEN );
}

void ImgPrintDialog::setImage( KookaImage *img )
{
    if( ! img ) return;

    // TODO: get scan resolution out of the image

}

void ImgPrintDialog::setOptions(const TQMap<TQString,TQString>& opts)
{
    // m_autofit->setChecked(opts["app-img-autofit"] == "1");
    TQString scale = opts[OPT_SCALING];

    kdDebug(28000) << "In setOption" << endl;

    if( scale == "scan" )
        m_rbOrigSize->setChecked(true);
    else if( scale == "custom" )
        m_rbScale->setChecked(true);
    else
        m_rbScreen->setChecked(true);

    int help  = opts[OPT_SCAN_RES].toInt();
    m_dpi->setValue( help );

    help = opts[OPT_WIDTH].toInt();
    m_sizeW->setValue( help );

    help = opts[OPT_HEIGHT].toInt();
    m_sizeH->setValue( help );

    help = opts[OPT_SCREEN_RES].toInt();
    m_screenRes->setText(i18n( "Screen resolution: %1 dpi").tqarg(help));

    help = opts[OPT_PSGEN_DRAFT].toInt();
    m_psDraft->setChecked( help == 1 );

    help = opts[OPT_RATIO].toInt();
    m_ratio->setChecked( help == 1 );

}


void ImgPrintDialog::getOptions(TQMap<TQString,TQString>& opts, bool )
{
    // TODO: Check for meaning of include_def !
    // kdDebug(28000) << "In getOption with include_def: "  << include_def << endl;

    TQString scale = "screen";
    if( m_rbOrigSize->isChecked() )
        scale = "scan";
    else if( m_rbScale->isChecked() )
        scale = "custom";
    else if( m_rbFitPage->isChecked() )
	scale = "fitpage";

    opts[OPT_SCALING] = scale;

    opts[OPT_SCAN_RES]    = TQString::number( m_dpi->value()         );
    opts[OPT_WIDTH]       = TQString::number( m_sizeW->value()       );
    opts[OPT_HEIGHT]      = TQString::number( m_sizeH->value()       );
    opts[OPT_PSGEN_DRAFT] = TQString::number( m_psDraft->isChecked() );
    opts[OPT_RATIO]       = TQString::number( m_ratio->isChecked()   );

    {
	TQPaintDeviceMetrics metric( this );
	opts[OPT_SCREEN_RES] = TQString::number( metric.logicalDpiX());
    }
}

bool ImgPrintDialog::isValid(TQString& msg)
{
    /* check if scan reso is higher than 0 in case its needed */
    int id = m_scaleRadios->id( m_scaleRadios->selected());
    if( id == ID_ORIG && m_dpi->value() == 0 )
    {
        msg = i18n("Please specify a scan resolution larger than 0");
        return false;
    }
    else if( id == ID_CUSTOM && (m_sizeW->value() == 0 || m_sizeH->value() == 0 )  )
    {
        msg = i18n("For custom printing, a valid size should be specified.\n"
                   "At least one dimension is zero.");
    }

    return true;
}

void ImgPrintDialog::slScaleChanged( int id )
{
    if( id == ID_SCREEN )
    {
	/* disalbe size, scan res. */
	m_dpi->setEnabled(false);
        m_ratio->setEnabled(false);
        m_sizeW->setEnabled(false);
        m_sizeH->setEnabled(false);
    }
    else if( id == ID_ORIG )
    {
	/* disable size */
	m_dpi->setEnabled(true);
        m_ratio->setEnabled(false);
        m_sizeW->setEnabled(false);
        m_sizeH->setEnabled(false);
    }
    else if( id == ID_CUSTOM )
    {
	m_dpi->setEnabled(false);
        m_ratio->setEnabled(true);
        m_sizeW->setEnabled(true);
        m_sizeH->setEnabled(true);
    }
    else if( id == ID_FIT_PAGE )
    {
	m_dpi->setEnabled(false);
        m_ratio->setEnabled(true);
        m_sizeW->setEnabled(false);
        m_sizeH->setEnabled(false);	
    }
}

void ImgPrintDialog::slCustomWidthChanged( int val )
{
    if( m_ignoreSignal )
    {
        m_ignoreSignal = false;
        return;
    }

    /* go out here if scaling is not custom */
    if( m_scaleRadios->id( m_scaleRadios->selected()) != ID_CUSTOM ) return;

    /* go out here if maintain aspect ration is off */
    if( ! m_ratio->isChecked() ) return;

    m_ignoreSignal = true;
    kdDebug(28000) << "Setting value to horizontal size" << endl;
    m_sizeH->setValue( int( double(val) *
                            double(m_image->height())/double(m_image->width()) ) );

}

void ImgPrintDialog::slCustomHeightChanged( int val )
{
    if( m_ignoreSignal )
    {
        m_ignoreSignal = false;
        return;
    }

    /* go out here if scaling is not custom */
    if( m_scaleRadios->id( m_scaleRadios->selected()) != ID_CUSTOM ) return;

    /* go out here if maintain aspect ration is off */
    if( ! m_ratio->isChecked() ) return;

    m_ignoreSignal = true;
    kdDebug(28000) << "Setting value to vertical size" << endl;
    m_sizeW->setValue( int( double(val) *
                            double(m_image->width())/double(m_image->height()) ) );

}

#include "imgprintdialog.moc"
