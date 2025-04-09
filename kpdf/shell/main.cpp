/***************************************************************************
 *   Copyright (C) 2002 by Wilco Greven <greven@kde.org>                   *
 *   Copyright (C) 2003 by Christophe Devriese                             *
 *                         <Christophe.Devriese@student.kuleuven.ac.be>    *
 *   Copyright (C) 2003 by Laurent Montel <montel@kde.org>                 *
 *   Copyright (C) 2003-2004 by Albert Astals Cid <tsdgeos@terra.es>       *
 *   Copyright (C) 2004 by Andy Goossens <andygoossens@telenet.be>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "shell.h"
#include <tdeapplication.h>
#include <tdeaboutdata.h>
#include <tdecmdlineargs.h>
#include <tdeconfig.h>
#include <tdelocale.h>
#include <dcopclient.h>
#include <dcopref.h>
#include <kdebug.h>

static const char description[] =
I18N_NOOP("KPDF, a TDE PDF viewer based on XPDF");

static const char version[] = "0.5.10";

static TDECmdLineOptions options[] =
{
    { "new-instance", I18N_NOOP("Don't reuse existing instance"), 0 },
    { "+[URL]", I18N_NOOP("Document to open"), 0 },
    TDECmdLineLastOption
};

int main(int argc, char** argv)
{
    TDEAboutData about(
        "kpdf",
        I18N_NOOP("KPDF"),
        version,
        description,
        TDEAboutData::License_GPL,
        "(C) 2002 Wilco Greven, Christophe Devriese\n(C) 2004-2005 Albert Astals Cid, Enrico Ros");

    about.addAuthor("Wilco Greven", 0, "greven@kde.org");
    about.addAuthor("Christophe Devriese", 0, "oelewapperke@oelewapperke.org");
    about.addAuthor("Laurent Montel", 0, "montel@kde.org");
    about.addAuthor("Albert Astals Cid", I18N_NOOP("Current mantainer"), "astals11@terra.es");
    about.addAuthor("Enrico Ros", 0, "eros.kde@email.it");

    about.addCredit("Derek Noonburg", I18N_NOOP("Xpdf author"), 0, "http://www.foolabs.com/xpdf/");
    about.addCredit("Marco Martin", I18N_NOOP("Icon"), 0, "m4rt@libero.it");

    TDECmdLineArgs::init(argc, argv, &about);
    TDECmdLineArgs::addCmdLineOptions( options );
    TDEApplication app;

    // see if we are starting with session management
    if (app.isRestored())
    {
        RESTORE(KPDF::Shell);
    } else {
        // no session.. just start up normally
        TDECmdLineArgs* args = TDECmdLineArgs::parsedArgs();

        DCOPClient *client = kapp->dcopClient();
        if (!client->attach())
        {
          kdError() << "KPDF::Shell cannot attach DCOP client" << endl;
          return 2;
        }

        bool reuseKPDF;

        if (args->isSet("new-instance"))
        {
          reuseKPDF = false;
        }
        else
        {
          TDEConfig cfg("kpdfpartrc");
          cfg.setGroup("General");
          reuseKPDF = cfg.readBoolEntry("OpenInExistingKPDF", false);
        }

        TQCString kpdfInstance = "";
        TQCString regName = client->registerAs(kapp->name(), true);

        if (reuseKPDF && args->count())
        {
          QCStringList allClients = client->registeredApplications();
          for (int c = 0; c < allClients.count(); ++c)
          {
            if (allClients[c].left(5) == "kpdf-" && allClients[c] != regName)
            {
              kpdfInstance = allClients[c];
            }
          }
        }

        if (kpdfInstance.isEmpty())
        {
          KPDF::Shell* widget = new KPDF::Shell;
          for (int i = 0; i < args->count(); ++i)
          {
              widget->openURL(args->url(i));
          }
          widget->show();
        }
        else
        {
          for (int i = 0; i < args->count(); ++i)
          {
            DCOPRef ref(kpdfInstance, "KPDFShellDCOPIface");
            ref.call("openURL", args->url(i));
          }
          DCOPRef ref(kpdfInstance, "KPDF::Shell");
          ref.call("raise");
          return 1;
        }
        args->clear();
    }

    return app.exec();
}
