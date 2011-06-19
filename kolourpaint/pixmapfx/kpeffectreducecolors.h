
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


#ifndef KP_EFFECT_REDUCE_COLORS_H
#define KP_EFFECT_REDUCE_COLORS_H


#include <kpcoloreffect.h>


class TQRadioButton;
class TQImage;

class kpMainWindow;


TQImage convertImageDepth (const TQImage &image, int depth, bool dither);


class kpEffectReduceColorsCommand : public kpColorEffectCommand
{
public:
    // depth must be 1 or 8
    kpEffectReduceColorsCommand (int depth, bool dither,
                                 bool actOnSelection,
                                 kpMainWindow *mainWindow);
    virtual ~kpEffectReduceColorsCommand ();

    TQString commandName (int depth, int dither) const;

    // (always preserves tqmask)
    static void apply (TQPixmap *destPixmapPtr, int depth, bool dither);
    static TQPixmap apply (const TQPixmap &pm, int depth, bool dither);


    //
    // kpColorEffectCommand interface
    //

protected:
    virtual TQPixmap applyColorEffect (const TQPixmap &pixmap);

    int m_depth;
    bool m_dither;
};


class kpEffectReduceColorsWidget : public kpColorEffectWidget
{
Q_OBJECT
  TQ_OBJECT

public:
    kpEffectReduceColorsWidget (bool actOnSelection,
                                kpMainWindow *mainWindow,
                                TQWidget *tqparent, const char *name = 0);
    virtual ~kpEffectReduceColorsWidget ();


    int depth () const;
    bool dither () const;


    //
    // kpColorEffectWidget interface
    //

    virtual TQString caption () const;

    virtual bool isNoOp () const;
    virtual TQPixmap applyColorEffect (const TQPixmap &pixmap);

    virtual kpColorEffectCommand *createCommand () const;

protected:
    TQRadioButton *m_blackAndWhiteRadioButton,
                 *m_blackAndWhiteDitheredRadioButton,
                 *m_8BitRadioButton,
                 *m_8BitDitheredRadioButton,
                 *m_24BitRadioButton;
    TQRadioButton *m_defaultRadioButton;
};



#endif  // KP_EFFECT_REDUCE_COLORS_H
