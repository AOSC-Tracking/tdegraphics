/*
    KDE Draw - a small graphics drawing program for the KDE.
    Copyright (C) 1998  Thomas Tanghus (tanghus@kde.org)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

// A lot of this code is lifted from KMail. Thanks, guys!

#include <stdlib.h>

#include <klocale.h>
#include <tdeconfig.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <kdebug.h>

#include "kiconedit.h"
#include "config.h"
#include "properties.h"

static const char description[] =
	I18N_NOOP("TDE Icon Editor");

static KCmdLineOptions options[] =
{
  { "+[file(s)]" , I18N_NOOP("Icon file(s) to open"), 0 },
  KCmdLineLastOption
};

int main(int argc, char **argv)
{
  TDEAboutData aboutData( "kiconedit", I18N_NOOP("TDEIconEdit"),
    VERSION, description, TDEAboutData::License_GPL,
    "(c) 1998, Thomas Tanghus");

  aboutData.addAuthor("Thomas Tanghus",0, "tanghus@kde.org");
  aboutData.addAuthor("John Califf",0, "jcaliff@compuzone.net");
  aboutData.addAuthor("Laurent Montel",0, "lmontel@mandrakesoft.com");
  aboutData.addAuthor("Aaron J. Seigo",0, "aseigo@olympusproject.org");
  aboutData.addCredit( "Nadeem Hasan", "Rewrote UI to use XMLGUI\n"
      "Lots of fixes and cleanup", "nhasan@nadmm.com" );
  aboutData.addCredit( "Adrian Page", I18N_NOOP("Bug fixes and GUI tidy up"),
      "Adrian.Page@tesco.net" );
  TDECmdLineArgs::init( argc, argv, &aboutData );
  TDECmdLineArgs::addCmdLineOptions( options ); // Add our own options.

  TDEApplication a;

//  setSignalHandler(signalHandler);

  if (a.isRestored())
  {
    RESTORE(TDEIconEdit);
  }
  else
  {
    TDECmdLineArgs *args = TDECmdLineArgs::parsedArgs();
    for(int i = 0; i < args->count(); i++)
    {
        TDEIconEdit *ki = new TDEIconEdit(args->url(i), "kiconedit");
        TQ_CHECK_PTR(ki);
    }

    if (args->count() == 0)
    {
        TDEIconEdit *ki = new TDEIconEdit;
        TQ_CHECK_PTR(ki);
    }
    args->clear();
  }

  int rc = a.exec();
  delete TDEIconEditProperties::self();
  return rc;
}

