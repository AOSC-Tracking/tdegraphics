/* This file is part of the KDE project
   Copyright (C) 1998-2002 Carsten Pfeiffer <pfeiffer@kde.org>

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
#include <tqlabel.h>
#include <tqlayout.h>
#include <tqtooltip.h>
#include <tqvgroupbox.h>

#include <kapplication.h>
#include <kcolorbutton.h>
#include <kdialog.h>
#include <kiconloader.h>
#include <klineedit.h>
#include <klocale.h>
#include <knuminput.h>
#include <kurllabel.h>

#include "generalwidget.h"

GeneralWidget::GeneralWidget( TQWidget *parent, const char *name )
  : TQWidget( parent, name )
{
  TQVBoxLayout *tqlayout = new TQVBoxLayout( this );
  tqlayout->setSpacing( KDialog::spacingHint() );

  TQPixmap pixmap = UserIcon( "logo" );
  KURLLabel *logo = new KURLLabel( this );
  logo->setURL( "http://devel-home.kde.org/~pfeiffer/kuickshow/" );
  logo->setPixmap( pixmap );
  logo->setFixedSize( pixmap.size() );
  logo->setTipText( i18n( "Open KuickShow Website" ) );
  logo->setUseTips( true );

  connect( logo, TQT_SIGNAL( leftClickedURL( const TQString & ) ),
            TQT_SLOT( slotURLClicked( const TQString & ) ) );

  tqlayout->addWidget( logo, 0, AlignRight );

  cbFullscreen = new TQCheckBox( i18n("Fullscreen mode"), this, "boscreen" );

  cbPreload = new TQCheckBox( i18n("Preload next image"), this, "preload");
  cbLastdir = new TQCheckBox( i18n("Remember last folder"), this, "restart_lastdir");

  TQGridLayout *gridLayout = new TQGridLayout( 2, 2 );
  gridLayout->setSpacing( KDialog::spacingHint() );
  TQLabel *l0 = new TQLabel( i18n("Background color:"), this );
  colorButton = new KColorButton( this );

  TQLabel *l1 = new TQLabel( i18n("Show only files with extension: "), this, "label" );
  editFilter = new KLineEdit( this, "filteredit" );

  gridLayout->addWidget( l0, 0, 0 );
  gridLayout->addWidget( colorButton, 0, 1 );
  gridLayout->addWidget( l1, 1, 0 );
  gridLayout->addWidget( editFilter, 1, 1 );

  tqlayout->addWidget( cbFullscreen );
  tqlayout->addWidget( cbPreload );
  tqlayout->addWidget( cbLastdir );
  tqlayout->addLayout( gridLayout );

  ////////////////////////////////////////////////////////////////////////

  TQVGroupBox *gbox2 = new TQVGroupBox( i18n("Quality/Speed"),
				     this, "qualitybox" );
  tqlayout->addWidget( gbox2 );
  tqlayout->addStretch();

  cbSmoothScale = new TQCheckBox( i18n("Smooth scaling"), gbox2, "smoothscale" );
  cbFastRender = new TQCheckBox( i18n("Fast rendering"), gbox2, "fastrender" );
  cbDither16bit = new TQCheckBox( i18n("Dither in HiColor (15/16bit) modes"),
				 gbox2, "dither16bit" );

  cbDither8bit = new TQCheckBox( i18n("Dither in LowColor (<=8bit) modes"),
				gbox2, "dither8bit" );

  cbOwnPalette = new TQCheckBox( i18n("Use own color palette"),
                                gbox2, "pal");
  connect( cbOwnPalette, TQT_SIGNAL( clicked() ), this, TQT_SLOT( useOwnPalette() ) );

  cbFastRemap = new TQCheckBox( i18n("Fast palette remapping"), gbox2, "remap");

  maxCacheSpinBox = new KIntNumInput( gbox2, "editmaxcache" );
  maxCacheSpinBox->setLabel( i18n("Maximum cache size: "), AlignVCenter );
  maxCacheSpinBox->setSuffix( i18n( " MB" ) );
  maxCacheSpinBox->setSpecialValueText( i18n( "Unlimited" ) );
  maxCacheSpinBox->setRange( 0, 400, 1 );

  loadSettings( *kdata );
  cbFullscreen->setFocus();
}

GeneralWidget::~GeneralWidget()
{
}

void GeneralWidget::slotURLClicked( const TQString & url )
{
  kapp->invokeBrowser( url );
}

void GeneralWidget::loadSettings( const KuickData& data )
{
    ImData *idata = data.idata;

    colorButton->setColor( data.backgroundColor );
    editFilter->setText( data.fileFilter );
    cbFullscreen->setChecked( data.fullScreen );
    cbPreload->setChecked( data.preloadImage );
    cbLastdir->setChecked( data.startInLastDir );
    cbFastRemap->setChecked( idata->fastRemap );
    cbOwnPalette->setChecked( idata->ownPalette );
    cbSmoothScale->setChecked( idata->smoothScale );
    cbFastRender->setChecked( idata->fastRender );
    cbDither16bit->setChecked( idata->dither16bit );
    cbDither8bit->setChecked( idata->dither8bit );
    maxCacheSpinBox->setValue( idata->maxCache / 1024 );

    useOwnPalette(); // enable/disable remap-checkbox
}

void GeneralWidget::applySettings( KuickData& data)
{
    ImData *idata = data.idata;

    data.backgroundColor = colorButton->color();
    data.fileFilter      = editFilter->text();
    data.fullScreen  	  = cbFullscreen->isChecked();
    data.preloadImage	  = cbPreload->isChecked();
    data.startInLastDir   = cbLastdir->isChecked();

    idata->smoothScale    = cbSmoothScale->isChecked();
    idata->fastRemap 	  = cbFastRemap->isChecked();
    idata->ownPalette 	  = cbOwnPalette->isChecked();
    idata->fastRender 	  = cbFastRender->isChecked();
    idata->dither16bit 	  = cbDither16bit->isChecked();
    idata->dither8bit 	  = cbDither8bit->isChecked();

    idata->maxCache	  = (uint) maxCacheSpinBox->value() * 1024;
}

void GeneralWidget::useOwnPalette()
{
    cbFastRemap->setEnabled( cbOwnPalette->isChecked() );
}

#include "generalwidget.moc"
