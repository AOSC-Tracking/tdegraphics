
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

#define DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET 0


#include <kpdocumentsaveoptionswidget.h>

#include <tqapplication.h>
#include <tqbuffer.h>
#include <tqimage.h>
#include <tqlabel.h>
#include <tqlayout.h>
#include <tqtimer.h>

#include <kcombobox.h>
#include <tdeconfig.h>
#include <kdebug.h>
#include <kdialog.h>
#include <kdialogbase.h>
#include <tdeglobal.h>
#include <kimageio.h>
#include <tdelocale.h>
#include <knuminput.h>
#include <kpushbutton.h>

#include <kpdefs.h>
#include <kpdocument.h>
#include <kppixmapfx.h>
#include <kpresizesignallinglabel.h>
#include <kpselection.h>
#include <kptoolpreviewdialog.h>
#include <kpwidgetmapper.h>


// protected static
const TQSize kpDocumentSaveOptionsPreviewDialog::s_pixmapLabelMinimumSize (25, 25);


kpDocumentSaveOptionsPreviewDialog::kpDocumentSaveOptionsPreviewDialog (
        TQWidget *parent,
        const char *name)
    : TQWidget (parent, name,
               TQt::WType_TopLevel |
               TQt::WStyle_Customize |
                   TQt::WStyle_DialogBorder |
                   TQt::WStyle_Title),
#if 0
KDialogBase (parent, name, false/*non-modal*/,
                   i18n ("Save Preview"),
                   0/*no buttons*/),
#endif
      m_filePixmap (0),
      m_fileSize (0)
{
    setCaption (i18n ("Save Preview"));

    TQWidget *baseWidget = this;//new TQWidget (this);
    //setMainWidget (baseWidget);


    TQGridLayout *lay = new TQGridLayout (baseWidget, 2, 1,
                                        KDialog::marginHint (), KDialog::spacingHint ());

    m_filePixmapLabel = new kpResizeSignallingLabel (baseWidget);
    m_fileSizeLabel = new TQLabel (baseWidget);


    m_filePixmapLabel->setMinimumSize (s_pixmapLabelMinimumSize);


    lay->addWidget (m_filePixmapLabel, 0, 0);
    lay->addWidget (m_fileSizeLabel, 1, 0, TQt::AlignHCenter);


    lay->setRowStretch (0, 1);


    connect (m_filePixmapLabel, TQT_SIGNAL (resized ()),
             this, TQT_SLOT (updatePixmapPreview ()));
}

kpDocumentSaveOptionsPreviewDialog::~kpDocumentSaveOptionsPreviewDialog ()
{
    delete m_filePixmap;
}


// public
TQSize kpDocumentSaveOptionsPreviewDialog::preferredMinimumSize () const
{
    const int contentsWidth = 180;
    const int totalMarginsWidth = 2 * KDialog::marginHint ();

    return TQSize (contentsWidth + totalMarginsWidth,
                  contentsWidth * 3 / 4 + totalMarginsWidth);
}


// public slot
void kpDocumentSaveOptionsPreviewDialog::setFilePixmapAndSize (const TQPixmap &pixmap,
                                                              int fileSize)
{
    delete m_filePixmap;
    m_filePixmap = new TQPixmap (pixmap);

    updatePixmapPreview ();

    m_fileSize = fileSize;

    const int pixmapSize = kpPixmapFX::pixmapSize (pixmap);
    const int percent = pixmapSize ?
                            TQMAX (1, fileSize * 100 / pixmapSize) :
                            0;
#if DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET
    kdDebug () << "kpDocumentSaveOptionsPreviewDialog::setFilePixmapAndSize()"
               << " pixmapSize=" << pixmapSize
               << " fileSize=" << fileSize
               << " raw fileSize/pixmapSize%="
               << (pixmapSize ? fileSize * 100 / pixmapSize : 0)
               << endl;
#endif

    // HACK: I don't know if the percentage thing will work well and we're
    //       really close to the message freeze so provide alt. texts to choose
    //       from during the message freeze :)
    const TQString alternateText0 = i18n ("%1 bytes");
    const TQString alternateText1 = i18n ("%1 bytes (%2%)");
    const TQString alternateText2 = i18n ("%1 B");
    const TQString alternateText3 = i18n ("%1 B (%2%)");
    const TQString alternateText4 = i18n ("%1 B (approx. %2%)");
    const TQString alternateText5 = i18n ("%1B");
    const TQString alternateText6 = i18n ("%1B (%2%)");
    const TQString alternateText7 = i18n ("%1B (approx. %2%)");
    m_fileSizeLabel->setText (i18n ("%1 bytes (approx. %2%)")
                                   .arg (TDEGlobal::locale ()->formatLong (m_fileSize))
                                   .arg (percent));
}

// public slot
void kpDocumentSaveOptionsPreviewDialog::updatePixmapPreview ()
{
#if DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET
    kdDebug () << "kpDocumentSaveOptionsPreviewDialog::updatePreviewPixmap()"
               << " filePixmapLabel.size=" << m_filePixmapLabel->size ()
               << " filePixmap.size=" << m_filePixmap->size ()
               << endl;
#endif

    if (m_filePixmap)
    {
        int maxNewWidth = TQMIN (m_filePixmap->width (),
                                m_filePixmapLabel->width ()),
            maxNewHeight = TQMIN (m_filePixmap->height (),
                                 m_filePixmapLabel->height ());

        double keepsAspect = kpToolPreviewDialog::aspectScale (
            maxNewWidth, maxNewHeight,
            m_filePixmap->width (), m_filePixmap->height ());
    #if DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET
        kdDebug () << "\tmaxNewWidth=" << maxNewWidth
                   << " maxNewHeight=" << maxNewHeight
                   << " keepsAspect=" << keepsAspect
                   << endl;
    #endif


        const int newWidth = kpToolPreviewDialog::scaleDimension (
            m_filePixmap->width (),
            keepsAspect,
            1,
            maxNewWidth);
        const int newHeight = kpToolPreviewDialog::scaleDimension (
            m_filePixmap->height (),
            keepsAspect,
            1,
            maxNewHeight);
    #if DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET
        kdDebug () << "\tnewWidth=" << newWidth
                   << " newHeight=" << newHeight
                   << endl;
    #endif


        TQPixmap transformedPixmap =
            kpPixmapFX::scale (*m_filePixmap,
                               newWidth, newHeight);


        TQPixmap labelPixmap (m_filePixmapLabel->width (),
                             m_filePixmapLabel->height ());
        kpPixmapFX::fill (&labelPixmap, kpColor::transparent);
        kpPixmapFX::setPixmapAt (&labelPixmap,
            (labelPixmap.width () - transformedPixmap.width ()) / 2,
            (labelPixmap.height () - transformedPixmap.height ()) / 2,
            transformedPixmap);


        m_filePixmapLabel->setPixmap (labelPixmap);
    }
    else
    {
        m_filePixmapLabel->setPixmap (TQPixmap ());
    }
}


// protected virtual [base TQWidget]
void kpDocumentSaveOptionsPreviewDialog::closeEvent (TQCloseEvent *e)
{
#if DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET
    kdDebug () << "kpDocumentSaveOptionsPreviewDialog::closeEvent()" << endl;
#endif

    TQWidget::closeEvent (e);

    emit finished ();
}

// protected virtual [base TQWidget]
void kpDocumentSaveOptionsPreviewDialog::moveEvent (TQMoveEvent *e)
{
#if DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET
    kdDebug () << "kpDocumentSaveOptionsPreviewDialog::moveEvent()" << endl;
#endif

    TQWidget::moveEvent (e);

    emit moved ();
}

// protected virtual [base TQWidget]
void kpDocumentSaveOptionsPreviewDialog::resizeEvent (TQResizeEvent *e)
{
#if DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET
    kdDebug () << "kpDocumentSaveOptionsPreviewDialog::resizeEvent()" << endl;
#endif

    TQWidget::resizeEvent (e);

    emit resized ();
}


kpDocumentSaveOptionsWidget::kpDocumentSaveOptionsWidget (
        const TQPixmap &docPixmap,
        const kpDocumentSaveOptions &saveOptions,
        const kpDocumentMetaInfo &metaInfo,
        TQWidget *parent, const char *name)
    : TQWidget (parent, name),
      m_visualParent (parent)
{
    init ();
    setDocumentSaveOptions (saveOptions);
    setDocumentPixmap (docPixmap);
    setDocumentMetaInfo (metaInfo);
}

kpDocumentSaveOptionsWidget::kpDocumentSaveOptionsWidget (
        TQWidget *parent, const char *name)
    : TQWidget (parent, name),
      m_visualParent (parent)
{
    init ();
}

// private
void kpDocumentSaveOptionsWidget::init ()
{
    m_documentPixmap = 0;
    m_previewDialog = 0;
    m_visualParent = 0;


    m_colorDepthLabel = new TQLabel (i18n ("Convert &to:"), this);
    m_colorDepthCombo = new KComboBox (this);

    m_colorDepthSpaceWidget = new TQWidget (this);

    m_qualityLabel = new TQLabel (i18n ("Quali&ty:"), this);
    m_qualityInput = new KIntNumInput (this);
    // Note that we set min to 1 not 0 since "0 Quality" is a bit misleading
    // and 101 quality settings would be weird.  So we lose 1 quality setting
    // according to TQImage::save().
    // TODO: 100 quality is also misleading since that implies perfect quality.
    m_qualityInput->setRange (1, 100, 1/*step*/, true/*slider*/);

    m_previewButton = new KPushButton (i18n ("&Preview"), this);
    m_previewButton->setToggleButton (true);


    m_colorDepthLabel->setBuddy (m_colorDepthCombo);

    m_qualityLabel->setBuddy (m_qualityInput);


    TQHBoxLayout *lay = new TQHBoxLayout (this, 0/*margin*/, KDialog::spacingHint ());

    lay->addWidget (m_colorDepthLabel, 0/*stretch*/, TQt::AlignLeft);
    lay->addWidget (m_colorDepthCombo, 0/*stretch*/);

    lay->addWidget (m_colorDepthSpaceWidget, 1/*stretch*/);

    lay->addWidget (m_qualityLabel, 0/*stretch*/, TQt::AlignLeft);
    lay->addWidget (m_qualityInput, 2/*stretch*/);

    lay->addWidget (m_previewButton, 0/*stretch*/, TQt::AlignRight);


    connect (m_colorDepthCombo, TQT_SIGNAL (activated (int)),
             this, TQT_SLOT (slotColorDepthSelected ()));
    connect (m_colorDepthCombo, TQT_SIGNAL (activated (int)),
             this, TQT_SLOT (updatePreview ()));

    connect (m_qualityInput, TQT_SIGNAL (valueChanged (int)),
             this, TQT_SLOT (updatePreviewDelayed ()));

    connect (m_previewButton, TQT_SIGNAL (toggled (bool)),
             this, TQT_SLOT (showPreview (bool)));


    m_updatePreviewDelay = 200/*ms*/;

    m_updatePreviewTimer = new TQTimer (this);
    connect (m_updatePreviewTimer, TQT_SIGNAL (timeout ()),
             this, TQT_SLOT (updatePreview ()));

    m_updatePreviewDialogLastRelativeGeometryTimer = new TQTimer (this);
    connect (m_updatePreviewDialogLastRelativeGeometryTimer, TQT_SIGNAL (timeout ()),
             this, TQT_SLOT (updatePreviewDialogLastRelativeGeometry ()));


    setMode (None);

    slotColorDepthSelected ();
}

kpDocumentSaveOptionsWidget::~kpDocumentSaveOptionsWidget ()
{
#if DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET
    kdDebug () << "kpDocumentSaveOptionsWidget::<dtor>()" << endl;
#endif
    hidePreview ();

    delete m_documentPixmap;
}


// public
void kpDocumentSaveOptionsWidget::setVisualParent (TQWidget *visualParent)
{
#if DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET
    kdDebug () << "kpDocumentSaveOptionsWidget::setVisualParent("
               << visualParent << ")" << endl;
#endif

    m_visualParent = visualParent;
}


// protected
bool kpDocumentSaveOptionsWidget::mimeTypeHasConfigurableColorDepth () const
{
    return kpDocumentSaveOptions::mimeTypeHasConfigurableColorDepth (mimeType ());
}

// protected
bool kpDocumentSaveOptionsWidget::mimeTypeHasConfigurableQuality () const
{
    return kpDocumentSaveOptions::mimeTypeHasConfigurableQuality (mimeType ());
}


// public
TQString kpDocumentSaveOptionsWidget::mimeType () const
{
    return m_baseDocumentSaveOptions.mimeType ();
}

// public slots
void kpDocumentSaveOptionsWidget::setMimeType (const TQString &string)
{
#if DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET
    kdDebug () << "kpDocumentSaveOptionsWidget::setMimeType(" << string
               << ") maxColorDepth="
               << kpDocumentSaveOptions::mimeTypeMaximumColorDepth (string)
               << endl;
#endif

    const int newMimeTypeMaxDepth =
        kpDocumentSaveOptions::mimeTypeMaximumColorDepth (string);

#if DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET
    kdDebug () << "\toldMimeType=" << mimeType ()
               << " maxColorDepth="
               << kpDocumentSaveOptions::mimeTypeMaximumColorDepth (
                      mimeType ())
               << endl;
#endif

    if (mimeType ().isEmpty () ||
        kpDocumentSaveOptions::mimeTypeMaximumColorDepth (mimeType ()) !=
        newMimeTypeMaxDepth)
    {
        m_colorDepthCombo->clear ();

        m_colorDepthCombo->insertItem (i18n ("Monochrome"), 0);
        m_colorDepthCombo->insertItem (i18n ("Monochrome (Dithered)"), 1);

        if (newMimeTypeMaxDepth >= 8)
        {
            m_colorDepthCombo->insertItem (i18n ("256 Color"), 2);
            m_colorDepthCombo->insertItem (i18n ("256 Color (Dithered)"), 3);
        }

        if (newMimeTypeMaxDepth >= 24)
        {
            m_colorDepthCombo->insertItem (i18n ("24-bit Color"), 4);
        }

        if (m_colorDepthComboLastSelectedItem >= 0 &&
            m_colorDepthComboLastSelectedItem < m_colorDepthCombo->count ())
        {
        #if DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET
            kdDebug () << "\tsetting colorDepthCombo to "
                       << m_colorDepthComboLastSelectedItem << endl;
        #endif

            m_colorDepthCombo->setCurrentItem (m_colorDepthComboLastSelectedItem);
        }
        else
        {
        #if DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET
            kdDebug () << "\tsetting colorDepthCombo to max item since"
                       << " m_colorDepthComboLastSelectedItem="
                       << m_colorDepthComboLastSelectedItem
                       << " out of range" << endl;
        #endif

            m_colorDepthCombo->setCurrentItem (m_colorDepthCombo->count () - 1);
        }
    }


    m_baseDocumentSaveOptions.setMimeType (string);

    if (mimeTypeHasConfigurableColorDepth ())
        setMode (ColorDepth);
    else if (mimeTypeHasConfigurableQuality ())
        setMode (Quality);
    else
        setMode (None);

    updatePreview ();
}


// public
int kpDocumentSaveOptionsWidget::colorDepth () const
{
    if (mode () & ColorDepth)
    {
        switch (m_colorDepthCombo->currentItem ())
        {
        case 0:
        case 1:
            return 1;

        case 2:
        case 3:
            return 8;

        case 4:
            return 32;

        default:
            return kpDocumentSaveOptions::invalidColorDepth ();
        }
    }
    else
    {
        return m_baseDocumentSaveOptions.colorDepth ();
    }
}

// public
bool kpDocumentSaveOptionsWidget::dither () const
{
    if (mode () & ColorDepth)
    {
        return (m_colorDepthCombo->currentItem () == 1 ||
                m_colorDepthCombo->currentItem () == 3);
    }
    else
    {
        return m_baseDocumentSaveOptions.dither ();
    }
}

// protected static
int kpDocumentSaveOptionsWidget::colorDepthComboItemFromColorDepthAndDither (
    int depth, bool dither)
{
    if (depth == 1)
    {
        if (!dither)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    else if (depth == 8)
    {
        if (!dither)
        {
            return 2;
        }
        else
        {
            return 3;
        }
    }
    else if (depth == 32)
    {
        return 4;
    }
    else
    {
        return -1;
    }
}

// public slots
void kpDocumentSaveOptionsWidget::setColorDepthDither (int newDepth, bool newDither)
{
#if DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET
    kdDebug () << "kpDocumentSaveOptionsWidget::setColorDepthDither("
               << "depth=" << newDepth
               << ",dither=" << newDither
               << ")" << endl;
#endif

    m_baseDocumentSaveOptions.setColorDepth (newDepth);
    m_baseDocumentSaveOptions.setDither (newDither);


    const int comboItem = colorDepthComboItemFromColorDepthAndDither (
                              newDepth, newDither);
    // TODO: Ignoring when comboItem >= m_colorDepthCombo->count() is wrong.
    //       This happens if this mimeType has configurable colour depth
    //       and an incorrect maximum colour depth (less than a TQImage of
    //       this mimeType, opened by kpDocument).
    if (comboItem >= 0 && comboItem < m_colorDepthCombo->count ())
        m_colorDepthCombo->setCurrentItem (comboItem);


    slotColorDepthSelected ();
}


// protected slot
void kpDocumentSaveOptionsWidget::slotColorDepthSelected ()
{
    if (mode () & ColorDepth)
    {
        m_colorDepthComboLastSelectedItem = m_colorDepthCombo->currentItem ();
    }
    else
    {
        m_colorDepthComboLastSelectedItem =
            colorDepthComboItemFromColorDepthAndDither (
                m_baseDocumentSaveOptions.colorDepth (),
                m_baseDocumentSaveOptions.dither ());
    }

#if DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET
    kdDebug () << "kpDocumentSaveOptionsWidget::slotColorDepthSelected()"
               << " mode&ColorDepth=" << (mode () & ColorDepth)
               << " colorDepthComboLastSelectedItem="
               << m_colorDepthComboLastSelectedItem
               << endl;
#endif
}


// public
int kpDocumentSaveOptionsWidget::quality () const
{
    if (mode () & Quality)
    {
        return m_qualityInput->value ();
    }
    else
    {
        return m_baseDocumentSaveOptions.quality ();
    }
}

// public
void kpDocumentSaveOptionsWidget::setQuality (int newQuality)
{
#if DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET
    kdDebug () << "kpDocumentSaveOptionsWidget::setQuality("
               << newQuality << ")" << endl;
#endif

    m_baseDocumentSaveOptions.setQuality (newQuality);
    m_qualityInput->setValue (newQuality == -1/*TQImage::save() default*/ ?
                                  75 :
                                  newQuality);
}


// public
kpDocumentSaveOptions kpDocumentSaveOptionsWidget::documentSaveOptions () const
{
    return kpDocumentSaveOptions (mimeType (), colorDepth (), dither (), quality ());
}

// public
void kpDocumentSaveOptionsWidget::setDocumentSaveOptions (
    const kpDocumentSaveOptions &saveOptions)
{
    setMimeType (saveOptions.mimeType ());
    setColorDepthDither (saveOptions.colorDepth (), saveOptions.dither ());
    setQuality (saveOptions.quality ());
}


// public
void kpDocumentSaveOptionsWidget::setDocumentPixmap (const TQPixmap &documentPixmap)
{
    delete m_documentPixmap;
    m_documentPixmap = new TQPixmap (documentPixmap);

    updatePreview ();
}

// public
void kpDocumentSaveOptionsWidget::setDocumentMetaInfo (
    const kpDocumentMetaInfo &metaInfo)
{
    m_documentMetaInfo = metaInfo;

    updatePreview ();
}


// public
kpDocumentSaveOptionsWidget::Mode kpDocumentSaveOptionsWidget::mode () const
{
    return m_mode;
}

// public
void kpDocumentSaveOptionsWidget::setMode (Mode mode)
{
    m_mode = mode;


    // If mode == None, we show still show the Color Depth widgets but disabled
    m_colorDepthLabel->setShown (mode != Quality);
    m_colorDepthCombo->setShown (mode != Quality);
    m_colorDepthSpaceWidget->setShown (mode != Quality);

    m_qualityLabel->setShown (mode == Quality);
    m_qualityInput->setShown (mode == Quality);


    m_colorDepthLabel->setEnabled (mode == ColorDepth);
    m_colorDepthCombo->setEnabled (mode == ColorDepth);

    m_qualityLabel->setEnabled (mode == Quality);
    m_qualityInput->setEnabled (mode == Quality);


    // SYNC: HACK: When changing between color depth and quality widgets,
    //       we change the height of "this", causing the text on the labels
    //       to move but the first instance of the text doesn't get erased.
    //       TQt bug.
    TQTimer::singleShot (0, this, TQT_SLOT (repaintLabels ()));
}

// protected slot
void kpDocumentSaveOptionsWidget::repaintLabels ()
{
    if (mode () != Quality)
        m_colorDepthLabel->repaint ();
    if (mode () == Quality)
        m_qualityLabel->repaint ();
}


// protected slot
void kpDocumentSaveOptionsWidget::showPreview (bool yes)
{
#if DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET
    kdDebug () << "kpDocumentSaveOptionsWidget::showPreview(" << yes << ")"
               << " m_previewDialog=" << bool (m_previewDialog)
               << endl;
#endif

    if (yes == bool (m_previewDialog))
        return;

    if (!m_visualParent)
        return;

    if (yes)
    {
        m_previewDialog = new kpDocumentSaveOptionsPreviewDialog (m_visualParent, "previewSaveDialog");
        updatePreview ();

        connect (m_previewDialog, TQT_SIGNAL (finished ()),
                 this, TQT_SLOT (hidePreview ()));


        TDEConfigGroupSaver cfgGroupSaver (TDEGlobal::config (), kpSettingsGroupPreviewSave);
        TDEConfigBase *cfg = cfgGroupSaver.config ();

        if (cfg->hasKey (kpSettingPreviewSaveUpdateDelay))
        {
            m_updatePreviewDelay = cfg->readNumEntry (kpSettingPreviewSaveUpdateDelay);
        }
        else
        {
            cfg->writeEntry (kpSettingPreviewSaveUpdateDelay, m_updatePreviewDelay);
            cfg->sync ();
        }

        if (m_updatePreviewDelay < 0)
            m_updatePreviewDelay = 0;
    #if DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET
        kdDebug () << "\tread cfg preview dialog update delay="
                   << m_updatePreviewDelay
                   << endl;
    #endif


        if (m_previewDialogLastRelativeGeometry.isEmpty ())
        {
        #if DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET
            kdDebug () << "\tread cfg preview dialog last rel geometry" << endl;
        #endif
            TDEConfigGroupSaver cfgGroupSaver (TDEGlobal::config (), kpSettingsGroupPreviewSave);
            TDEConfigBase *cfg = cfgGroupSaver.config ();

            m_previewDialogLastRelativeGeometry = cfg->readRectEntry (
                kpSettingPreviewSaveGeometry);
        }

    #if DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET
        kdDebug () << "\tpreviewDialogLastRelativeGeometry="
                   << m_previewDialogLastRelativeGeometry
                   << " visualParent->rect()=" << m_visualParent->rect ()
                   << endl;
    #endif

        TQRect relativeGeometry;
        if (!m_previewDialogLastRelativeGeometry.isEmpty () &&
            m_visualParent->rect ().intersects (m_previewDialogLastRelativeGeometry))
        {
        #if DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET
            kdDebug () << "\tok" << endl;
        #endif
            relativeGeometry = m_previewDialogLastRelativeGeometry;
        }
        else
        {
        #if DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET
            kdDebug () << "\t\tinvalid" << endl;
        #endif
            const int margin = 20;

            relativeGeometry =
                TQRect (m_visualParent->width () -
                           m_previewDialog->preferredMinimumSize ().width () -
                               margin,
                       margin * 2,  // Avoid folder combo
                       m_previewDialog->preferredMinimumSize ().width (),
                       m_previewDialog->preferredMinimumSize ().height ());
        }


        const TQRect globalGeometry =
            kpWidgetMapper::toGlobal (m_visualParent,
                                      relativeGeometry);
    #if DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET
        kdDebug () << "\trelativeGeometry=" << relativeGeometry
                   << " globalGeometry=" << globalGeometry
                   << endl;
    #endif

        m_previewDialog->resize (globalGeometry.size ());
        m_previewDialog->move (globalGeometry.topLeft ());


        m_previewDialog->show ();


    #if DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET
        kdDebug () << "\tgeometry after show="
                   << TQRect (m_previewDialog->x (), m_previewDialog->y (),
                              m_previewDialog->width (), m_previewDialog->height ())
                   << endl;
    #endif

        updatePreviewDialogLastRelativeGeometry ();

        connect (m_previewDialog, TQT_SIGNAL (moved ()),
                 this, TQT_SLOT (updatePreviewDialogLastRelativeGeometry ()));
        connect (m_previewDialog, TQT_SIGNAL (resized ()),
                 this, TQT_SLOT (updatePreviewDialogLastRelativeGeometry ()));

        m_updatePreviewDialogLastRelativeGeometryTimer->start (200/*ms*/);
    }
    else
    {
        m_updatePreviewDialogLastRelativeGeometryTimer->stop ();

        TDEConfigGroupSaver cfgGroupSaver (TDEGlobal::config (), kpSettingsGroupPreviewSave);
        TDEConfigBase *cfg = cfgGroupSaver.config ();

        cfg->writeEntry (kpSettingPreviewSaveGeometry, m_previewDialogLastRelativeGeometry);
        cfg->sync ();

    #if DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET
        kdDebug () << "\tsaving preview geometry "
                   << m_previewDialogLastRelativeGeometry
                   << " (TQt would have us believe "
                   << kpWidgetMapper::fromGlobal (m_visualParent,
                          TQRect (m_previewDialog->x (), m_previewDialog->y (),
                                 m_previewDialog->width (), m_previewDialog->height ()))
                   << ")"
                   << endl;
    #endif

        m_previewDialog->deleteLater ();
        m_previewDialog = 0;
    }
}

// protected slot
void kpDocumentSaveOptionsWidget::hidePreview ()
{
    if (m_previewButton->isOn ())
        m_previewButton->toggle ();
}


// protected slot
void kpDocumentSaveOptionsWidget::updatePreviewDelayed ()
{
    m_updatePreviewTimer->start (m_updatePreviewDelay, true/*single shot*/);
}

// protected slot
void kpDocumentSaveOptionsWidget::updatePreview ()
{
    if (!m_previewDialog || !m_documentPixmap)
        return;


    m_updatePreviewTimer->stop ();


    TQApplication::setOverrideCursor (TQt::waitCursor);

    TQByteArray data;

    TQBuffer buffer (data);
    buffer.open (IO_WriteOnly);
    kpDocument::savePixmapToDevice (*m_documentPixmap,
                                    TQT_TQIODEVICE(&buffer),
                                    documentSaveOptions (),
                                    m_documentMetaInfo,
                                    false/*no lossy prompt*/,
                                    this);
    buffer.close ();


    TQImage image;
    image.loadFromData (data,
        KImageIO::typeForMime (mimeType ()).latin1 ());

    // TODO: merge with kpDocument::getPixmapFromFile()
    m_previewDialog->setFilePixmapAndSize (
        kpPixmapFX::convertToPixmapAsLosslessAsPossible (image),
        data.size ());

    TQApplication::restoreOverrideCursor ();
}

// protected slot
void kpDocumentSaveOptionsWidget::updatePreviewDialogLastRelativeGeometry ()
{
#if DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET
    kdDebug () << "kpDocumentSaveOptionsWidget::"
               << "updatePreviewDialogLastRelativeGeometry()"
               << endl;
#endif

    if (m_previewDialog && m_previewDialog->isVisible ())
    {
        m_previewDialogLastRelativeGeometry =
            kpWidgetMapper::fromGlobal (m_visualParent,
                TQRect (m_previewDialog->x (), m_previewDialog->y (),
                       m_previewDialog->width (), m_previewDialog->height ()));
    #if DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET
        kdDebug () << "\tcaching pos = "
                   << m_previewDialogLastRelativeGeometry
                   << endl;
    #endif
    }
    else
    {
    #if DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET
        kdDebug () << "\tnot visible - ignoring geometry" << endl;
    #endif
    }
}


#include <kpdocumentsaveoptionswidget.moc>
