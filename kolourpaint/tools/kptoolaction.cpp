
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


#include <kptoolaction.h>

#include <kptool.h>


kpToolAction::kpToolAction (const TQString &text,
                            const TQString &pic, const TDEShortcut &shortcut,
                            const TQObject *receiver, const char *slot,
                            TQObject *parent, const char *name)
    : TDEToggleAction (text,
                     pic, shortcut,
                     receiver, slot,
                     parent, name)
{
    updateToolTip ();
}

kpToolAction::~kpToolAction ()
{
}


// protected
void kpToolAction::updateToolTip ()
{
    const TQString newToolTip =
        kpTool::toolTipForTextAndShortcut (text (), shortcut ());
    if (newToolTip == toolTip ())
        return;

    setToolTip (newToolTip);
    emit toolTipChanged (newToolTip);
}


//
// TDEToggleAction interface
//

// public slot virtual [base TDEAction]
void kpToolAction::setText (const TQString &text)
{
    TDEToggleAction::setText (text);
    updateToolTip ();
}

// public slot virtual [base TDEAction]
bool kpToolAction::setShortcut (const TDEShortcut &shortcut)
{
    bool ret = TDEToggleAction::setShortcut (shortcut);
    updateToolTip ();
    return ret;
}


//
// TDEToggleAction implements kpSingleKeyTriggersActionInterface
//

// public virtual [base kpSingleKeyTriggersActionInterface]
const char *kpToolAction::actionName () const
{
    return name ();
}

// public virtual [base kpSingleKeyTriggersActionInterface]
TDEShortcut kpToolAction::actionShortcut () const
{
    return shortcut ();
}

// public virtual [base kpSingleKeyTriggersActionInterface]
void kpToolAction::actionSetShortcut (const TDEShortcut &shortcut)
{
    setShortcut (shortcut);
}


#include <kptoolaction.moc>
