// KPrintDialogPage_PageOptions.cpp
//
// Part of KVIEWSHELL - A framework for multipage text/gfx viewers
//
// (C) 2005 Stefan Kebekus
// Distributed under the GPL

// Add header files alphabetically

#include <config.h>

#include <klocale.h>
#include <tqbuttongroup.h>
#include <tqcheckbox.h>
#include <tqlayout.h>
#include <tqtooltip.h>
#include <tqwhatsthis.h>
#include <kdebug.h>

#include "kprintDialogPage_pageoptions.h"

  
KPrintDialogPage_PageOptions::KPrintDialogPage_PageOptions( TQWidget *parent, const char *name )
  : KPrintDialogPage( parent, name )
{
  setTitle( i18n("Page Size & Placement") );

  kprintDialogPage_pageoptions_baseLayout = 0;
  checkBox_center = 0;
  checkBox_rotate = 0;
  checkBox_shrink = 0;
  checkBox_expand = 0;

  kprintDialogPage_pageoptions_baseLayout = new TQVBoxLayout( this, 11, 6, "kprintDialogPage_pageoptions_baseLayout"); 
  if (kprintDialogPage_pageoptions_baseLayout == 0) {
    kdError(1223) << "KPrintDialogPage_PageOptions::KPrintDialogPage_PageOptions() cannot create tqlayout" << endl;
    return;
  }
    
  checkBox_center = new TQCheckBox( this, "checkBox_center" );
  if (checkBox_center != 0) {
    checkBox_center->setText( i18n( "Center the page on paper" ) );
    TQToolTip::add( checkBox_center, i18n( "If this option is enabled, the pages are centered on the paper." ) );
    TQWhatsThis::add( checkBox_center, i18n( "<qt><p>If this option is enabled, the pages will be printed centered on the paper; this makes "
					    "more visually-appealing printouts.</p>"
					    "<p>If the option is not enabled, all pages will be placed in the top-left corner of the paper.</p></qt>" ) );
    kprintDialogPage_pageoptions_baseLayout->addWidget( checkBox_center );
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
  
  TQFrame *line = new TQFrame( this, "line1" );
  if (line != 0) {
    line->setFrameShape( TQFrame::HLine );
    line->setFrameShadow( TQFrame::Sunken );
    line->setFrameShape( TQFrame::HLine );
    kprintDialogPage_pageoptions_baseLayout->addWidget( line );
  }
  
  checkBox_shrink = new TQCheckBox( this, "checkBox_shrink" );
  if (checkBox_shrink != 0) {
    checkBox_shrink->setText( i18n( "Shrink oversized pages to fit paper size" ) );
    TQToolTip::add( checkBox_shrink, i18n( "If this option is enabled, large pages that would not fit the printer's paper size will be shrunk." ) );
    TQWhatsThis::add( checkBox_shrink, i18n( "<qt><p>If this option is enabled, large pages that would not fit the printer's paper size will be "
					    "shrunk so that edges won't be cut off during printing.</p>"
					    "<p><b>Note:</b> If this option is enabled, and if the pages in your document have different sizes, "
					    "then different pages might be shrunk by different scaling factors.</p></qt>" ) );
    kprintDialogPage_pageoptions_baseLayout->addWidget( checkBox_shrink );
  }
    
  checkBox_expand = new TQCheckBox( this, "checkBox_expand" );
  if (checkBox_expand != 0) {
    checkBox_expand->setText( i18n( "Expand small pages to fit paper size" ) );
    TQToolTip::add( checkBox_expand, i18n( "If this option is enabled, small pages will be enlarged so that they fit the printer's paper size." ) );
    TQWhatsThis::add( checkBox_expand, i18n( "<qt><p>If this option is enabled, small pages will be enlarged so that they fit the printer's "
					    "paper size.</p>"
					    "<p><b>Note:</b> If this option is enabled, and if the pages in your document have different sizes, "
					    "then different pages might be expanded by different scaling factors.</p></qt>" ) );
    kprintDialogPage_pageoptions_baseLayout->addWidget( checkBox_expand );
  }

  
  resize( TQSize(319, 166).expandedTo(minimumSizeHint()) );
  clearWState( WState_Polished );
}



void KPrintDialogPage_PageOptions::getOptions( TQMap<TQString,TQString>& opts, bool incldef )
{
  // Save options, taking default values into consideration: by
  // default "center" is checked, "expand" and "shrink" are
  // not. Warning: The default values are also coded into setOptions()
  // and kmultipage::print(..).
  
  if (checkBox_center != 0)
    if ( incldef || !checkBox_center->isChecked() )
      if (checkBox_center->isChecked())
	opts[ "kde-kviewshell-centerpage" ] = "true";
      else
	opts[ "kde-kviewshell-centerpage" ] = "false";

  if (checkBox_rotate != 0)
    if ( incldef || !checkBox_rotate->isChecked() )
      if (checkBox_rotate->isChecked())
	opts[ "kde-kviewshell-rotatepage" ] = "true";
      else
	opts[ "kde-kviewshell-rotatepage" ] = "false";
  
  if (checkBox_shrink != 0)
    if ( incldef || checkBox_shrink->isChecked() )
      if (checkBox_shrink->isChecked())
	opts[ "kde-kviewshell-shrinkpage" ] = "true";
      else
	opts[ "kde-kviewshell-shrinkpage" ] = "false";
  
  if (checkBox_expand != 0)
    if ( incldef || checkBox_expand->isChecked() )  
      if (checkBox_expand->isChecked())
	opts[ "kde-kviewshell-expandpage" ] = "true";
      else
	opts[ "kde-kviewshell-expandpage" ] = "false";
}


void KPrintDialogPage_PageOptions::setOptions( const TQMap<TQString,TQString>& opts )
{
  // Sets the centering option. By default, this option is
  // checked. Warning: All default values are also coded into
  // getOptions() and kmultipage::print(..).
  TQString op = opts[ "kde-kviewshell-centerpage" ];
  if (checkBox_center != 0)
    checkBox_center->setChecked( op != "false" );

  // same for rotation
  op = opts[ "kde-kviewshell-rotatepage" ];
  if (checkBox_rotate != 0)
    checkBox_rotate->setChecked( op != "false" );
  
  // Sets the shrink option. By default, this option is not checked
  op = opts[ "kde-kviewshell-shrinkpage" ];
  if (checkBox_shrink != 0)
    checkBox_shrink->setChecked( op == "true" );
  
  // Sets the expand option. By default, this option is not checked
  op = opts[ "kde-kviewshell-expandpage" ];
  if (checkBox_expand != 0)
    checkBox_expand->setChecked( op == "true" );
}


bool KPrintDialogPage_PageOptions::isValid( TQString& )
{
  return true;
}
