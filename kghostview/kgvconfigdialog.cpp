/**
 * Copyright (C) 2000-2002 the KGhostView authors. See file AUTHORS.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


// Add header files alphabetically

#include <tqlayout.h>

#include <tdeconfig.h>
#include <kdebug.h>
#include <tdeglobal.h>
#include <kiconloader.h>
#include <kinstance.h>
#include <tdelocale.h>
#include <tdemessagebox.h>
#include <kprocess.h>
#include <kstandarddirs.h>
#include <tdeconfigdialog.h>

#include "configuration.h"
#include "kgv_view.h"
#include "kgvfactory.h"

#include "generalsettingswidget.h"
#include "gssettingswidget.h"

#include "kgvconfigdialog.h"


namespace {
    TQString getGSVersion( TQString fullPathToExec )
    {
	TQString res;
	TQString chkVersion = TDEProcess::quote(fullPathToExec) + " --version";
	FILE* p = popen( TQFile::encodeName(chkVersion), "r" );
	if( p ) {
	    // FIXME: a badly configured interpreter can hang us
	    TQFile qp;
	    qp.open( IO_ReadOnly, p );
	    qp.readLine( res, 80 );
	    qp.close();
	    pclose( p );
	    res = res.stripWhiteSpace();
	}
	kdDebug(4500) << "kgvconfigdialog.cpp::{unamed}::getGSVersion() returning " << res <<  endl;
	return res;
    }


    /* A mechanism for triggering redetection of gs versions:
     *
     * The idea is that whenever we find we need to workaround a certain version of gs,
     * we cannot rely that there will solelly be a new kghostviewrc, but that users will
     * upgrade kghostview. Therefore, whenever we want to trigger redection on a new version,
     * we increment the counter below. It should have the old value from the previous version
     * of kghostview and this function will get called.
     * 
     */

    /* On a related note:
     * We don't detect upgrades (or downgrades, for that matter) of gs.
     * I am seeing if I can get the version out of gs as a side effect to displaying a file.
     * This way, using tdeconfig:/Ghostscript/GS Version we will see whether the version has changed
     * and trigger a redetection without the trouble of running "gs --version" on each launch.
     *
     * LPC (9 April 2003)
     */
    /* currentRedetection value log:
     * 
     * 1- remove -dMaxBitmap for gs 6.5x
     * 2- see if it supports .setsafe ( see bug:57291 ).
     */
    const int currentRedetection = 2;


    /* Here are the issues we found so far in version of gs:
     *
     * - gs before 6.50 uses device x11alpha instead of x11
     * - gs 6.5x does not work well with -dMaxBitmap=...
     * - gs 6.52 and earlier as well as 7.0x for x < 4 don't support the .setsafe operator
     *
     */

    TQString recommendSetSafe( TQString version )
    {
	if ( version < TQString::number( 6.53 ) ) return TQString::number( 6.53 );
	if ( version[ 0 ] == '7' && version < TQString::number( 7.04 ) ) return TQString::number( 7.05 );
	return TQString();
    }
    // This function should contain all the gs version specific workarounds.
    void redoGSDetection() 
    {
	kdDebug(4500) << "kgvconfigdialog.cpp::{unnamed}::redoGSDetection()" << endl;
	TQString version = getGSVersion( Configuration::interpreter() );
	TQString recommended = recommendSetSafe( version );
	if ( !recommended.isNull() ) {
	    KMessageBox::sorry( 0,
		    i18n( "Your version of gs (version %1) is too old, since it has security issues "
			"which are impossible to resolve. Please upgrade to a newer version.\n"
			"KGhostView will try to work with it, but it may not display any files at all.\n"
			"Version %2 seems to be appropriate on your system, although newer versions will work as well." )
		    .arg( version )
		    .arg( recommended ) );
	}
	if ( version < TQString::number( 7.00 ) ) 
	{
	    TQStringList arguments = TQStringList::split( ' ', Configuration::antialiasingArguments() );
	    arguments.remove( TQString::fromLatin1( "-dMaxBitmap=10000000" ) );
	    TQString antiAliasArgs = arguments.join( " " );

	    Configuration::setAntialiasingArguments( antiAliasArgs );
	}

	Configuration::setRedetectionCounter( currentRedetection );
	Configuration::setVersion( version );
    }
} // namespace

void  ConfigDialog::showSettings( KGVPart* main ) {
    const char* name = "kghostview-settings";
    if ( TDEConfigDialog::showDialog( name ) ) return;

    if ( Configuration::redetectionCounter() < currentRedetection ) redoGSDetection();

    TDEConfigDialog* dialog = new TDEConfigDialog( 0, name,
	    Configuration::self(), KDialogBase::IconList );
    dialog->addPage( new GeneralSettingsWidget( 0, "general-settings" ),
	    i18n( "General" ), TQString::fromLatin1( "kghostview" ) );
    GSSettingsWidget *gssw = new GSSettingsWidget( 0, "gs-settings" );
    dialog->addPage( gssw, i18n( "Ghostscript\nConfiguration" ), TQString::fromLatin1( "application-pdf" ) );

    gssw->setDetectedVersion(Configuration::version());

    TQObject::connect( dialog, TQT_SIGNAL( settingsChanged() ), main, TQT_SLOT( slotConfigurationChanged() ) );
    dialog->show();
}
