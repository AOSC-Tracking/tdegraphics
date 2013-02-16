
/*
   Copyright (c) 2003,2004,2005 Clarence Dang <dang@kde.org>
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
   IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
   IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
   NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
   THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <kpmainwindow.h>
#include <kpmainwindow_p.h>

#include <dcopclient.h>
#include <tdeaction.h>
#include <kactivelabel.h>
#include <tdeapplication.h>
#include <tdeconfig.h>
#include <kdebug.h>
#include <kdialogbase.h>
#include <krun.h>
#include <tdelocale.h>
#include <tdeshortcut.h>

#include <kptool.h>


// private
void kpMainWindow::setupHelpMenuActions ()
{
    TDEActionCollection *ac = actionCollection ();


    // Explanation for action name:
    // "Taking" is like a digital camera when you record the image and is
    // analogous to pressing PrintScreen.  However, "Acquiring" is when
    // the image is brought into KolourPaint, just as you would acquire
    // from a digital camera in future versions of KolourPaint.  Hence
    // "Acquiring" is more appropriate.
    // -- Thurston
    d->m_actionHelpTakingScreenshots = new TDEAction (
        i18n ("Acquiring &Screenshots"), 0,
        TQT_TQOBJECT(this), TQT_SLOT (slotHelpTakingScreenshots ()),
        ac, "help_taking_screenshots");


    enableHelpMenuDocumentActions (false);
}

// private
void kpMainWindow::enableHelpMenuDocumentActions (bool /*enable*/)
{
}


// SYNC: tdebase/twin/twinbindings.cpp
static TQString printScreenShortcutString ()
{
    TDEConfigGroupSaver cfgGroupSaver (TDEGlobal::config (), "Global Shortcuts");
    TDEConfigBase *cfg = cfgGroupSaver.config ();

    // TODO: i18n() entry name?  twinbindings.cpp seems to but it doesn't
    //       make sense.
    const TQString cfgEntryString = cfg->readEntry ("Desktop Screenshot");


    // (only use 1st key sequence, if it exists)
    const TQString humanReadableShortcut =
        TDEShortcut (cfgEntryString).seq (0).toString ();

    if (!humanReadableShortcut.isEmpty ())
    {
        return humanReadableShortcut;
    }
    else
    {
        // (localised)
        return KKey (TQt::CTRL + TQt::Key_Print).toString ();
    }
}


// private slot
void kpMainWindow::slotHelpTakingScreenshots ()
{
#if DEBUG_KP_MAIN_WINDOW
    kdDebug () << "kpMainWindow::slotHelpTakingScreenshots()" << endl;
#endif

    if (toolHasBegunShape ())
        tool ()->endShapeInternal ();


    // TODO: Totally bogus logic if twin not running under same user as KolourPaint.
    // SYNC: KWin contains PrintScreen key logic
    QCStringList dcopApps = TDEApplication::dcopClient ()->registeredApplications ();
#if DEBUG_KP_MAIN_WINDOW
    kdDebug () << "\tdcopApps=" << dcopApps << endl;
#endif
    bool isRunningKDE = (dcopApps.findIndex ("twin") >= 0);

#if 0
{
    int i = 0;
    FILE *fp = fopen ("/home/kdevel/kolourpaint.tmp", "rt");
    if (fp && fscanf (fp, "Hello: %d", &i) == 1)
        isRunningKDE = i, fclose (fp);
}
#endif

    TQString message;
    if (isRunningKDE)
    {
        message = i18n
        (
            "<p>"
            "To acquire a screenshot, press <b>%1</b>."
            "  The screenshot will be placed into the clipboard"
            " and you will be able to paste it in KolourPaint."
            "</p>"

            "<p>"
            "You may configure the <b>Desktop Screenshot</b> shortcut"
            " in the TDE Control Center"
            " module <a href=\"configure kde shortcuts\">Keyboard Shortcuts</a>."
            "</p>"

            "<p>Alternatively, you may try the application"
            " <a href=\"run ksnapshot\">KSnapshot</a>."
            "</p>"
        );
    }
    else
    {
        message = i18n
        (
            "<p>"
            "You do not appear to be running KDE."
            "</p>"

            // We tell them this much even though they aren't running KDE
            // to entice them to use KDE since it's so easy.
            "<p>"
            "Once you have loaded KDE:<br>"
            "<blockquote>"
            "To acquire a screenshot, press <b>%1</b>."
            "  The screenshot will be placed into the clipboard"
            " and you will be able to paste it in KolourPaint."
            "</blockquote>"
            "</p>"

            "<p>Alternatively, you may try the application"
            " <a href=\"run ksnapshot\">KSnapshot</a>."
            "</p>"
        );
    }

    // TODO: Totally bogus logic if twin not running under same user as KolourPaint.
    message = message.arg (::printScreenShortcutString ());

    // Add extra vertical space
    message += "<p>&nbsp;</p>";


    KDialogBase dlg (this, "helpTakingScreenshotsDialog", true/*modal*/,
        i18n ("Acquiring Screenshots"),
        KDialogBase::Close, KDialogBase::Close/*default btn*/,
        true/*separator line*/);

    KActiveLabel *messageLabel = new KActiveLabel (message, &dlg);
    disconnect (messageLabel, TQT_SIGNAL (linkClicked (const TQString &)),
                messageLabel, TQT_SLOT (openLink (const TQString &)));
    connect (messageLabel, TQT_SIGNAL (linkClicked (const TQString &)),
             TQT_TQOBJECT(this), TQT_SLOT (slotHelpTakingScreenshotsFollowLink (const TQString &)));

    dlg.setMainWidget (messageLabel);

    dlg.exec ();
}

// private
void kpMainWindow::slotHelpTakingScreenshotsFollowLink (const TQString &link)
{
#if DEBUG_KP_MAIN_WINDOW
    kdDebug () << "kpMainWindow::slotHelpTakingScreenshotsFollowLink("
               << link << ")" << endl;
#endif

    if (link == "configure kde shortcuts")
    {
        KRun::runCommand ("tdecmshell keys");
    }
    else if (link == "run ksnapshot")
    {
        KRun::runCommand ("ksnapshot");
    }
    else
    {
        kdError () << "kpMainWindow::slotHelpTakingScreenshotsFollowLink("
                   << link << ")" << endl;
    }
}
