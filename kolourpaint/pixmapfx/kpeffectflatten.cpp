
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

#define DEBUG_KP_EFFECT_FLATTEN 0


#include <kpeffectflatten.h>

#include <tqcheckbox.h>
#include <tqimage.h>
#include <tqlayout.h>
#include <tqpixmap.h>
#include <tqvbox.h>

#include <kcolorbutton.h>
#include <tdeconfig.h>
#include <kdialog.h>
#include <kdebug.h>
#include <tdeglobal.h>
#include <kimageeffect.h>
#include <tdelocale.h>

#include <kpdefs.h>
#include <kppixmapfx.h>


//
// kpEffectFlattenCommand
//

kpEffectFlattenCommand::kpEffectFlattenCommand (const TQColor &color1,
                                                const TQColor &color2,
                                                bool actOnSelection,
                                                kpMainWindow *mainWindow)
    : kpColorEffectCommand (i18n ("Flatten"), actOnSelection, mainWindow),
      m_color1 (color1), m_color2 (color2)
{
}

kpEffectFlattenCommand::~kpEffectFlattenCommand ()
{
}


// public static
void kpEffectFlattenCommand::apply (TQPixmap *destPixmapPtr,
                                    const TQColor &color1, const TQColor &color2)
{
    if (!destPixmapPtr)
        return;

    TQImage image = kpPixmapFX::convertToImage (*destPixmapPtr);
    apply (&image, color1, color2);
    *destPixmapPtr = kpPixmapFX::convertToPixmap (image);
}

// public static
TQPixmap kpEffectFlattenCommand::apply (const TQPixmap &pm,
                                       const TQColor &color1, const TQColor &color2)
{
    TQImage image = kpPixmapFX::convertToImage (pm);
    apply (&image, color1, color2);
    return kpPixmapFX::convertToPixmap (image);
}

// public static
void kpEffectFlattenCommand::apply (TQImage *destImagePtr,
                                    const TQColor &color1, const TQColor &color2)
{
    if (!destImagePtr)
        return;

    KImageEffect::flatten (*destImagePtr/*ref*/, color1, color2);
}

// public static
TQImage kpEffectFlattenCommand::apply (const TQImage &img,
                                      const TQColor &color1, const TQColor &color2)
{
    TQImage retImage = img;
    apply (&retImage, color1, color2);
    return retImage;
}


//
// kpEffectFlattenCommand implements kpColorEffectCommand interface
//

// protected virtual [base kpColorEffectCommand]
TQPixmap kpEffectFlattenCommand::applyColorEffect (const TQPixmap &pixmap)
{
    return apply (pixmap, m_color1, m_color2);
}


//
// kpEffectFlattenWidget
//

// public static
// Don't initialise globally when we probably don't have a colour
// allocation context.  This way, the colours aren't sometimes invalid
// (e.g. at 8-bit).
TQColor kpEffectFlattenWidget::s_lastColor1;
TQColor kpEffectFlattenWidget::s_lastColor2;

kpEffectFlattenWidget::kpEffectFlattenWidget (bool actOnSelection,
                                              kpMainWindow *mainWindow,
                                              TQWidget *parent,
                                              const char *name)
    : kpColorEffectWidget (actOnSelection, mainWindow, parent, name)
{
    if (!s_lastColor1.isValid () || !s_lastColor2.isValid ())
    {
        TDEConfigGroupSaver cfgGroupSaver (TDEGlobal::config (), kpSettingsGroupFlattenEffect);
        TDEConfigBase *cfg = cfgGroupSaver.config ();

        s_lastColor1 = cfg->readColorEntry (kpSettingFlattenEffectColor1);
        if (!s_lastColor1.isValid ())
            s_lastColor1 = TQt::red;

        s_lastColor2 = cfg->readColorEntry (kpSettingFlattenEffectColor2);
        if (!s_lastColor2.isValid ())
            s_lastColor2 = TQt::blue;
    }


    m_enableCheckBox = new TQCheckBox (i18n ("E&nable"), this);

    TQVBox *colorButtonContainer = new TQVBox (this);
    colorButtonContainer->setMargin (KDialog::marginHint () / 2);
    colorButtonContainer->setSpacing (spacingHint ());
    m_color1Button = new KColorButton (s_lastColor1, colorButtonContainer);
    m_color2Button = new KColorButton (s_lastColor2, colorButtonContainer);


    m_color1Button->setEnabled (false);
    m_color2Button->setEnabled (false);


    TQVBoxLayout *lay = new TQVBoxLayout (this, marginHint (), spacingHint ());
    lay->addWidget (m_enableCheckBox);
    lay->addWidget (colorButtonContainer);


    connect (m_enableCheckBox, TQT_SIGNAL (toggled (bool)),
             this, TQT_SLOT (slotEnableChanged (bool)));

    connect (m_color1Button, TQT_SIGNAL (changed (const TQColor &)),
             this, TQT_SIGNAL (settingsChanged ()));
    connect (m_color2Button, TQT_SIGNAL (changed (const TQColor &)),
             this, TQT_SIGNAL (settingsChanged ()));
}

kpEffectFlattenWidget::~kpEffectFlattenWidget ()
{
    s_lastColor1 = color1 ();
    s_lastColor2 = color2 ();


    TDEConfigGroupSaver cfgGroupSaver (TDEGlobal::config (), kpSettingsGroupFlattenEffect);
    TDEConfigBase *cfg = cfgGroupSaver.config ();

    cfg->writeEntry (kpSettingFlattenEffectColor1, s_lastColor1);
    cfg->writeEntry (kpSettingFlattenEffectColor2, s_lastColor2);
    cfg->sync ();
}


// public
TQColor kpEffectFlattenWidget::color1 () const
{
    return m_color1Button->color ();
}

// public
TQColor kpEffectFlattenWidget::color2 () const
{
    return m_color2Button->color ();
}


//
// kpEffectFlattenWidget implements kpColorEffectWidget interface
//

// public virtual [base kpColorEffectWidget]
TQString kpEffectFlattenWidget::caption () const
{
    return i18n ("Colors");
}


// public virtual [base kpColorEffectWidget]
bool kpEffectFlattenWidget::isNoOp () const
{
    return !m_enableCheckBox->isChecked ();
}

// public virtual [base kpColorEffectWidget]
TQPixmap kpEffectFlattenWidget::applyColorEffect (const TQPixmap &pixmap)
{
#if DEBUG_KP_EFFECT_FLATTEN
    kdDebug () << "kpEffectFlattenWidget::applyColorEffect() nop="
               << isNoOp () << endl;
#endif

    if (isNoOp ())
        return pixmap;

    return kpEffectFlattenCommand::apply (pixmap, color1 (), color2 ());
}


// public virtual [base kpColorEffectWidget]
kpColorEffectCommand *kpEffectFlattenWidget::createCommand () const
{
    return new kpEffectFlattenCommand (color1 (), color2 (),
                                       m_actOnSelection,
                                       m_mainWindow);
}


// protected slot:
void kpEffectFlattenWidget::slotEnableChanged (bool enable)
{
#if DEBUG_KP_EFFECT_FLATTEN
    kdDebug () << "kpEffectFlattenWidget::slotEnableChanged(" << enable
               << ") enableButton=" << m_enableCheckBox->isChecked ()
               << endl;
#endif

    m_color1Button->setEnabled (enable);
    m_color2Button->setEnabled (enable);

    emit settingsChanged ();
}


#include <kpeffectflatten.moc>

