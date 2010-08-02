/***************************************************************************
 *   Copyright (C) 2005 by Stefan Kebekus                                  *
 *   kebekus@kde.org                                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
 ***************************************************************************/

#include <klocale.h>
#include <tqbuttongroup.h>
#include <tqcheckbox.h>
#include <tqlayout.h>
#include <tqtooltip.h>
#include <tqwhatsthis.h>
#include <kdebug.h>

#include "kprintDialogPage_DJVUpageoptions.h"

KPrintDialogPage_DJVUPageOptions::KPrintDialogPage_DJVUPageOptions( TQWidget *parent, const char *name )
  : KPrintDialogPage( parent, name )
{
  setTitle( i18n("Page Size & Placement") );

  kprintDialogPage_pageoptions_baseLayout = 0;
  checkBox_rotate = 0;
  checkBox_fitpage = 0;


  kprintDialogPage_pageoptions_baseLayout = new TQVBoxLayout( this, 11, 6, "kprintDialogPage_pageoptions_baseLayout");
  if (kprintDialogPage_pageoptions_baseLayout == 0) {
    kdError(1223) << "KPrintDialogPage_DJVUPageOptions::KPrintDialogPage_DJVUPageOptions() cannot create layout" << endl;
    return;
  }

  checkBox_rotate = new TQCheckBox( this, "checkBox_rotate" );
  if (checkBox_rotate != 0) {
    checkBox_rotate->setText( i18n( "Automatically choose landscape or portrait orientation" ) );
    TQToolTip::add( checkBox_rotate, i18n( "If this option is enabled, some pages might be rotated to better fit the paper size." ) );
    TQWhatsThis::add( checkBox_rotate, i18n( "<qt><p>If this option is enabled, landscape or portrait orientation are automatically chosen on a "
					    "page-by-page basis. This makes better use of the paper and gives more visually-"
					    "appealing printouts.</p>"
					    "<p><b>Note:</b> This option overrides the Portrait/Landscape option chosen in the printer "
					    "properties. If this option is enabled, and if the pages in your document have different sizes, "
					    "then some pages might be rotated while others are not.</p></qt>" ) );
    kprintDialogPage_pageoptions_baseLayout->addWidget( checkBox_rotate );
  }

  checkBox_fitpage = new TQCheckBox( this, "checkBox_shrink" );
  if (checkBox_fitpage != 0) {
    checkBox_fitpage->setText( i18n( "Scale pages to fit paper size" ) );
    TQToolTip::add( checkBox_fitpage, i18n( "If this option is enabled, all pages will be scaled to optimally fit the printer's paper size." ) );
    TQWhatsThis::add( checkBox_fitpage, i18n( "<qt><p>If this option is enabled, all pages will be scaled to optimally fit the printer's "
					    "paper size.</p>"
					    "<p><b>Note:</b> If this option is enabled, and if the pages in your document have different sizes, "
					    "then different pages might be scaled by different scaling factors.</p></qt>" ) );
    kprintDialogPage_pageoptions_baseLayout->addWidget( checkBox_fitpage );
  }

  kprintDialogPage_pageoptions_baseLayout->addStretch();

  resize( TQSize(319, 166).expandedTo(minimumSizeHint()) );
  clearWState( WState_Polished );
}



void KPrintDialogPage_DJVUPageOptions::getOptions( TQMap<TQString,TQString>& opts, bool )
{
  // Save options, taking default values into consideration. Warning:
  // The default values are also coded into setOptions() and
  // kmultipage::print(..).

  if (checkBox_rotate != 0)
    if (checkBox_rotate->isChecked())
      opts[ "kde-kviewshell-rotatepage" ] = "true";
    else
      opts[ "kde-kviewshell-rotatepage" ] = "false";

  if (checkBox_fitpage != 0)
    if (checkBox_fitpage->isChecked())
      opts[ "kde-kdjvu-fitpage" ] = "true";
    else
      opts[ "kde-kdjvu-fitpage" ] = "false";
}


void KPrintDialogPage_DJVUPageOptions::setOptions( const TQMap<TQString,TQString>& opts )
{
  // Warning: All default values are also coded into getOptions() and
  // kmultipage::print(..).

  // same for rotation
  TQString op = opts[ "kde-kviewshell-rotatepage" ];
  if (checkBox_rotate != 0)
    checkBox_rotate->setChecked( op != "false" );

  // Sets the fitpage option. By default, this option is not checked
  op = opts[ "kde-kdjvu-fitpage" ];
  if (checkBox_fitpage != 0)
    checkBox_fitpage->setChecked( op == "true" );
}


bool KPrintDialogPage_DJVUPageOptions::isValid( TQString& )
{
  return true;
}
