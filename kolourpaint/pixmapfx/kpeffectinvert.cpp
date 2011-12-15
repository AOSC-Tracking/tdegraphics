
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

#define DEBUG_KP_EFFECT_INVERT 0


#include <kpeffectinvert.h>

#include <tqcheckbox.h>
#include <tqimage.h>
#include <layout.h>
#include <tqpixmap.h>

#include <kdebug.h>
#include <klocale.h>

#include <kppixmapfx.h>


//
// kpEffectInvertCommand
//

kpEffectInvertCommand::kpEffectInvertCommand (int channels,
                                              bool actOnSelection,
                                              kpMainWindow *mainWindow)
    : kpColorEffectCommand (channels == RGB ?
                                i18n ("Invert Colors") : i18n ("Invert"),
                            actOnSelection, mainWindow),
      m_channels (channels)
{
}

kpEffectInvertCommand::kpEffectInvertCommand (bool actOnSelection,
                                              kpMainWindow *mainWindow)
    : kpColorEffectCommand (i18n ("Invert Colors"), actOnSelection, mainWindow),
      m_channels (RGB)
{
}

kpEffectInvertCommand::~kpEffectInvertCommand ()
{
}


// public static
void kpEffectInvertCommand::apply (TQPixmap *destPixmapPtr, int channels)
{
    TQImage image = kpPixmapFX::convertToImage (*destPixmapPtr);
    apply (&image, channels);
    *destPixmapPtr = kpPixmapFX::convertToPixmap (image);
}

// public static
TQPixmap kpEffectInvertCommand::apply (const TQPixmap &pm, int channels)
{
    TQImage image = kpPixmapFX::convertToImage (pm);
    apply (&image, channels);
    return kpPixmapFX::convertToPixmap (image);
}

// public static
void kpEffectInvertCommand::apply (TQImage *destImagePtr, int channels)
{
    TQRgb mask = tqRgba ((channels & Red) ? 0xFF : 0,
                       (channels & Green) ? 0xFF : 0,
                       (channels & Blue) ? 0xFF : 0,
                       0/*don't invert alpha*/);
#if DEBUG_KP_EFFECT_INVERT
    kdDebug () << "kpEffectInvertCommand::apply(channels=" << channels
               << ") mask=" << (int *) mask
               << endl;
#endif

    if (destImagePtr->depth () > 8)
    {
    #if 0
        // SYNC: TODO: TQt BUG - invertAlpha argument is inverted!!!
        destImagePtr->invertPixels (true/*no invert alpha (TQt 3.2)*/);
    #else
        // Above version works for TQt 3.2 at least.
        // But this version will always work (slower, though) and supports
        // inverting particular channels.
        for (int y = 0; y < destImagePtr->height (); y++)
        {
            for (int x = 0; x < destImagePtr->width (); x++)
            {
                destImagePtr->setPixel (x, y, destImagePtr->pixel (x, y) ^ mask);
            }
        }
    #endif
    }
    else
    {
        for (int i = 0; i < destImagePtr->numColors (); i++)
        {
            destImagePtr->setColor (i, destImagePtr->color (i) ^ mask);
        }
    }
}

// public static
TQImage kpEffectInvertCommand::apply (const TQImage &img, int channels)
{
    TQImage retImage = img;
    apply (&retImage, channels);
    return retImage;
}


//
// kpEffectInvertCommand implements kpColorEffectCommand interface
//

// protected virtual [base kpColorEffectCommand]
TQPixmap kpEffectInvertCommand::applyColorEffect (const TQPixmap &pixmap)
{
    return apply (pixmap, m_channels);
}


//
// kpEffectInvertWidget
//

kpEffectInvertWidget::kpEffectInvertWidget (bool actOnSelection,
                                            kpMainWindow *mainWindow,
                                            TQWidget *parent,
                                            const char *name)
    : kpColorEffectWidget (actOnSelection, mainWindow, parent, name)
{
    TQVBoxLayout *topLevelLay = new TQVBoxLayout (this, marginHint (), spacingHint ());


    TQWidget *centerWidget = new TQWidget (this);
    topLevelLay->addWidget (centerWidget, 0/*stretch*/, TQt::AlignCenter);


    TQVBoxLayout *centerWidgetLay = new TQVBoxLayout (centerWidget,
                                                    0/*margin*/,
                                                    spacingHint ());


    m_redCheckBox = new TQCheckBox (i18n ("&Red"), centerWidget);
    m_greenCheckBox = new TQCheckBox (i18n ("&Green"), centerWidget);
    m_blueCheckBox = new TQCheckBox (i18n ("&Blue"), centerWidget);

    TQWidget *spaceWidget = new TQWidget (centerWidget);
    spaceWidget->setFixedSize (1, spacingHint ());

    m_allCheckBox = new TQCheckBox (i18n ("&All"), centerWidget);


    m_redCheckBox->setChecked (false);
    m_greenCheckBox->setChecked (false);
    m_blueCheckBox->setChecked (false);

    m_allCheckBox->setChecked (false);


    centerWidgetLay->addWidget (m_redCheckBox);
    centerWidgetLay->addWidget (m_greenCheckBox);
    centerWidgetLay->addWidget (m_blueCheckBox);

    centerWidgetLay->addWidget (spaceWidget);

    centerWidgetLay->addWidget (m_allCheckBox);


    m_inSignalHandler = false;
    connect (m_redCheckBox, TQT_SIGNAL (toggled (bool)),
             this, TQT_SLOT (slotRGBCheckBoxToggled ()));
    connect (m_greenCheckBox, TQT_SIGNAL (toggled (bool)),
             this, TQT_SLOT (slotRGBCheckBoxToggled ()));
    connect (m_blueCheckBox, TQT_SIGNAL (toggled (bool)),
             this, TQT_SLOT (slotRGBCheckBoxToggled ()));

    connect (m_allCheckBox, TQT_SIGNAL (toggled (bool)),
             this, TQT_SLOT (slotAllCheckBoxToggled ()));
}

kpEffectInvertWidget::~kpEffectInvertWidget ()
{
}


// public
int kpEffectInvertWidget::channels () const
{
#if DEBUG_KP_EFFECT_INVERT
    kdDebug () << "kpEffectInvertWidget::channels()"
               << " isChecked: r=" << m_redCheckBox->isChecked ()
               << " g=" << m_greenCheckBox->isChecked ()
               << " b=" << m_blueCheckBox->isChecked ()
               << endl;
#endif

    int channels = 0;


    if (m_redCheckBox->isChecked ())
        channels |= kpEffectInvertCommand::Red;

    if (m_greenCheckBox->isChecked ())
        channels |= kpEffectInvertCommand::Green;

    if (m_blueCheckBox->isChecked ())
        channels |= kpEffectInvertCommand::Blue;


#if DEBUG_KP_EFFECT_INVERT
    kdDebug () << "\treturning channels=" << (int *) channels << endl;
#endif
    return channels;
}


//
// kpEffectInvertWidget implements kpColorEffectWidget interface
//

// public virtual [base kpColorEffectWidget]
TQString kpEffectInvertWidget::caption () const
{
    return i18n ("Channels");
}


// public virtual [base kpColorEffectWidget]
bool kpEffectInvertWidget::isNoOp () const
{
    return (channels () == kpEffectInvertCommand::None);
}

// public virtual [base kpColorEffectWidget]
TQPixmap kpEffectInvertWidget::applyColorEffect (const TQPixmap &pixmap)
{
    return kpEffectInvertCommand::apply (pixmap, channels ());
}


// public virtual [base kpColorEffectWidget]
kpColorEffectCommand *kpEffectInvertWidget::createCommand () const
{
    return new kpEffectInvertCommand (channels (),
                                      m_actOnSelection,
                                      m_mainWindow);
}


// protected slots
void kpEffectInvertWidget::slotRGBCheckBoxToggled ()
{
    if (m_inSignalHandler)
        return;

    m_inSignalHandler = true;

    //blockSignals (true);
    m_allCheckBox->setChecked (m_redCheckBox->isChecked () &&
                               m_blueCheckBox->isChecked () &&
                               m_greenCheckBox->isChecked ());
    //blockSignals (false);

    emit settingsChanged ();

    m_inSignalHandler = false;
}

// protected slot
void kpEffectInvertWidget::slotAllCheckBoxToggled ()
{
    if (m_inSignalHandler)
        return;

    m_inSignalHandler = true;

    //blockSignals (true);
    m_redCheckBox->setChecked (m_allCheckBox->isChecked ());
    m_greenCheckBox->setChecked (m_allCheckBox->isChecked ());
    m_blueCheckBox->setChecked (m_allCheckBox->isChecked ());
    //blockSignals (false);

    emit settingsChanged ();

    m_inSignalHandler = false;
}


#include <kpeffectinvert.moc>

