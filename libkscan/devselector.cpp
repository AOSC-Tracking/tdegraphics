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

#include <stdlib.h>

#include <tqbuttongroup.h>
#include <tqcheckbox.h>
#include <tqcstring.h>
#include <tqlayout.h>
#include <tqlabel.h>
#include <tqfile.h>
#include <tqpushbutton.h>
#include <tqradiobutton.h>
#include <tqstrlist.h>
#include <tqstringlist.h>

#include <kapplication.h>
#include <kconfig.h>
#include <kdebug.h>
#include <kglobal.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <ksimpleconfig.h>

#include "devselector.h"


DeviceSelector::DeviceSelector( TQWidget *parent, TQStrList& devList,
				const TQStringList& hrdevList )
    : KDialogBase( parent,  "DeviceSel", true, i18n("Welcome to Kooka"),
		   Ok|Cancel, Ok, true )
{
   kdDebug(29000) << "Starting DevSelector!" << endl;
   // Layout-Boxes
   TQWidget *page = new TQWidget( this );
   Q_CHECK_PTR( page );
   setMainWidget( page );

   TQVBoxLayout *topLayout = new TQVBoxLayout( page, marginHint(), spacingHint() );
   TQLabel *label = new TQLabel( page, "captionImage" );
   Q_CHECK_PTR( label );
   label->setPixmap( TQPixmap( "kookalogo.png" ));
   label->resize( 100, 350 );
   topLayout->addWidget( label );

   selectBox = new TQButtonGroup( 1, Horizontal, i18n( "Select Scan Device" ),
				 page, "ButtonBox");
   Q_CHECK_PTR( selectBox );
   selectBox->setExclusive( true );
   topLayout->addWidget( selectBox );
   setScanSources( devList, hrdevList );

   cbSkipDialog = new TQCheckBox( i18n("&Do not ask on startup again, always use this device"),
				 page, "CBOX_SKIP_ON_START" );

   KConfig *gcfg = KGlobal::config();
   gcfg->setGroup(TQString::fromLatin1(GROUP_STARTUP));
   bool skipDialog = gcfg->readBoolEntry( STARTUP_SKIP_ASK, false );
   cbSkipDialog->setChecked( skipDialog );

   topLayout->addWidget(cbSkipDialog);
   
}

TQCString DeviceSelector::getDeviceFromConfig( void ) const
{
   KConfig *gcfg = KGlobal::config();
   gcfg->setGroup(TQString::fromLatin1(GROUP_STARTUP));
   bool skipDialog = gcfg->readBoolEntry( STARTUP_SKIP_ASK, false );
   
   TQCString result;

   /* in this case, the user has checked 'Do not ask me again' and does not
    * want to be bothered any more.
    */
   result = TQFile::encodeName(gcfg->readEntry( STARTUP_SCANDEV, "" ));
   kdDebug(29000) << "Got scanner from config file to use: " << result << endl;
   
   /* Now check if the scanner read from the config file is available !
    * if not, ask the user !
    */
   if( skipDialog && devices.find( result ) > -1 )
   {
      kdDebug(29000) << "Scanner from Config file is available - fine." << endl;
   }
   else
   {
      kdDebug(29000) << "Scanner from Config file is _not_ available" << endl;
      result = TQCString();
   }
   
   return( result );
}

bool DeviceSelector::getShouldSkip( void ) const
{
   return( cbSkipDialog->isChecked());
}

TQCString DeviceSelector::getSelectedDevice( void ) const
{
   unsigned int selID = selectBox->id( selectBox->selected() );

   int dcount = devices.count();
   kdDebug(29000) << "The Selected ID is <" << selID << ">/" << dcount << endl;

   const char * dev = devices.at( selID );

   kdDebug(29000) << "The selected device: <" << dev << ">" << endl;

   /* Store scanner selection settings */
   KConfig *c = KGlobal::config();
   c->setGroup(TQString::fromLatin1(GROUP_STARTUP));
   /* Write both the scan device and the skip-start-dialog flag global. */
   c->writeEntry( STARTUP_SCANDEV, dev, true, true );
   c->writeEntry( STARTUP_SKIP_ASK, getShouldSkip(), true, true );
   c->sync();
   
   return dev;
}


void DeviceSelector::setScanSources( const TQStrList& sources,
				     const TQStringList& hrSources )
{
   bool default_ok = false;
   KConfig *gcfg = KGlobal::config();
   gcfg->setGroup(TQString::fromLatin1(GROUP_STARTUP));
   TQCString defstr = gcfg->readEntry( STARTUP_SCANDEV, "" ).local8Bit();

   /* Selector-Stuff*/
   uint nr = 0;
   int  checkDefNo = 0;

   TQStrListIterator it( sources );
   TQStringList::ConstIterator it2 = hrSources.begin();
   for ( ; it.current(); ++it, ++it2 )
   {
      TQString text = TQString::fromLatin1("&%1. %2\n%3").arg(1+nr).arg( TQString::fromLocal8Bit(*it) ).arg( *it2 );
      TQRadioButton *rb = new TQRadioButton( text, selectBox );
      selectBox->insert( rb );

      devices.append( *it );

      if( *it == defstr )
	 checkDefNo = nr;

      nr++;
   }

   /* Default still needs to be set */
   if( ! default_ok )
   {
      /* if no default found, set the first */
      TQRadioButton *rb = (TQRadioButton*) selectBox->find( checkDefNo );
      if ( rb )
	  rb->setChecked( true );
   }
}

DeviceSelector::~DeviceSelector()
{

}

/* The End ;) */
#include "devselector.moc"
