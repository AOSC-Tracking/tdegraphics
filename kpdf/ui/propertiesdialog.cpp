/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

// qt/kde includes
#include <tqlayout.h>
#include <tqlabel.h>
#include <klistview.h>
#include <klocale.h>
#include <ksqueezedtextlabel.h>
#include <kglobalsettings.h>

// local includes
#include "propertiesdialog.h"
#include "core/document.h"

PropertiesDialog::PropertiesDialog(TQWidget *tqparent, KPDFDocument *doc)
  : KDialogBase( Tabbed, i18n( "Unknown File" ), Ok, Ok, tqparent, 0, true, true )
{
  // Properties
  TQFrame *page = addPage(i18n("Properties"));
  TQGridLayout *tqlayout = new TQGridLayout( page, 2, 2, marginHint(), spacingHint() );

  // get document info, if not present display blank data and a warning
  const DocumentInfo * info = doc->documentInfo();
  if ( !info ) {
    tqlayout->addWidget( new TQLabel( i18n( "No document opened." ), page ), 0, 0 );
    return;
  }

  // mime name based on mimetype id
  TQString mimeName = info->get( "mimeType" ).section( '/', -1 ).upper();
  setCaption( i18n("%1 Properties").tqarg( mimeName ) );

  TQDomElement docElement = info->documentElement();

  int row = 0;
  int valMaxWidth = 100;
  for ( TQDomNode node = docElement.firstChild(); !node.isNull(); node = node.nextSibling() ) {
    TQDomElement element = node.toElement();

    TQString titleString = element.attribute( "title" );
    TQString valueString = element.attribute( "value" );
    if ( titleString.isEmpty() || valueString.isEmpty() )
        continue;

    // create labels and tqlayout them
    TQLabel *key = new TQLabel( i18n( "%1:" ).tqarg( titleString ), page );
    TQLabel *value = new KSqueezedTextLabel( valueString, page );
    tqlayout->addWidget( key, row, 0, AlignRight );
    tqlayout->addWidget( value, row, 1 );
    row++;

    // refine maximum width of 'value' labels
    valMaxWidth = TQMAX( valMaxWidth, fontMetrics().width( valueString ) );
  }

  // add the number of pages if the generator hasn't done it already
  TQDomNodeList list = docElement.elementsByTagName( "pages" );
  if ( list.count() == 0 ) {
    TQLabel *key = new TQLabel( i18n( "Pages:" ), page );
    TQLabel *value = new TQLabel( TQString::number( doc->pages() ), page );

    tqlayout->addWidget( key, row, 0 );
    tqlayout->addWidget( value, row, 1 );
  }

  // Fonts
  TQVBoxLayout *page2Layout = 0;
  if (doc->hasFonts())
  {
    TQFrame *page2 = addPage(i18n("Fonts"));
    page2Layout = new TQVBoxLayout(page2, 0, KDialog::spacingHint());
    KListView *lv = new KListView(page2);
    page2Layout->add(lv);
    doc->putFontInfo(lv);
  }

  // current width: left column + right column + dialog borders
  int width = tqlayout->tqminimumSize().width() + valMaxWidth + marginHint() + spacingHint() + marginHint() + 30;
  if (page2Layout)
  {
    width = TQMAX( width, page2Layout->tqsizeHint().width() + marginHint() + spacingHint() + 31 );
  }
  // stay inside the 2/3 of the screen width
  TQRect screenContainer = KGlobalSettings::desktopGeometry( this );
  width = TQMIN( width, 2*screenContainer.width()/3 );
  resize(width, 1);
}
