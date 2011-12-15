
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

#define DEBUG_KP_TOOL_RESIZE_SCALE_COMMAND 0
#define DEBUG_KP_TOOL_RESIZE_SCALE_DIALOG 0


#include <kptoolresizescale.h>

#include <math.h>

#include <tqaccel.h>
#include <tqapplication.h>
#include <tqbuttongroup.h>
#include <tqcheckbox.h>
#include <tqgroupbox.h>
#include <tqhbox.h>
#include <tqlabel.h>
#include <layout.h>
#include <tqpoint.h>
#include <tqpointarray.h>
#include <tqpushbutton.h>
#include <tqrect.h>
#include <tqsize.h>
#include <tqtoolbutton.h>
#include <tqwhatsthis.h>
#include <tqwmatrix.h>

#include <kapplication.h>
#include <kcombobox.h>
#include <kconfig.h>
#include <kdebug.h>
#include <kglobal.h>
#include <kiconeffect.h>
#include <kiconloader.h>
#include <klocale.h>
#include <knuminput.h>

#include <kpdefs.h>
#include <kpdocument.h>
#include <kpmainwindow.h>
#include <kppixmapfx.h>
#include <kpselection.h>
#include <kptool.h>


/*
 * kpToolResizeScaleCommand
 */

kpToolResizeScaleCommand::kpToolResizeScaleCommand (bool actOnSelection,
                                                    int newWidth, int newHeight,
                                                    Type type,
                                                    kpMainWindow *mainWindow)
    : kpCommand (mainWindow),
      m_actOnSelection (actOnSelection),
      m_type (type),
      m_backgroundColor (mainWindow ? mainWindow->backgroundColor () : kpColor::invalid),
      m_oldSelection (0)
{
    kpDocument *doc = document ();

    m_oldWidth = doc->width (m_actOnSelection);
    m_oldHeight = doc->height (m_actOnSelection);

    m_actOnTextSelection = (m_actOnSelection &&
                            doc && doc->selection () &&
                            doc->selection ()->isText ());

    resize (newWidth, newHeight);

    // If we have a selection _border_ (but not a floating selection),
    // then scale the selection with the document
    m_scaleSelectionWithImage = (!m_actOnSelection &&
                                 (m_type == Scale || m_type == SmoothScale) &&
                                 document ()->selection () &&
                                 !document ()->selection ()->pixmap ());
}

kpToolResizeScaleCommand::~kpToolResizeScaleCommand ()
{
    delete m_oldSelection;
}


// public virtual [base kpCommand]
TQString kpToolResizeScaleCommand::name () const
{
    if (m_actOnSelection)
    {
        if (m_actOnTextSelection)
        {
            if (m_type == Resize)
                return i18n ("Text: Resize Box");
        }
        else
        {
            if (m_type == Scale)
                return i18n ("Selection: Scale");
            else if (m_type == SmoothScale)
                return i18n ("Selection: Smooth Scale");
        }
    }
    else
    {
        switch (m_type)
        {
        case Resize:
            return i18n ("Resize");
        case Scale:
            return i18n ("Scale");
        case SmoothScale:
            return i18n ("Smooth Scale");
        }
    }

    return TQString();
}

// public virtual [base kpCommand]
int kpToolResizeScaleCommand::size () const
{
    return kpPixmapFX::pixmapSize (m_oldPixmap) +
           kpPixmapFX::pixmapSize (m_oldRightPixmap) +
           kpPixmapFX::pixmapSize (m_oldBottomPixmap) +
           (m_oldSelection ? m_oldSelection->size () : 0);
}


// public
int kpToolResizeScaleCommand::newWidth () const
{
    return m_newWidth;
}

// public
void kpToolResizeScaleCommand::setNewWidth (int width)
{
    resize (width, newHeight ());
}


// public
int kpToolResizeScaleCommand::newHeight () const
{
    return m_newHeight;
}

// public
void kpToolResizeScaleCommand::setNewHeight (int height)
{
    resize (newWidth (), height);
}


// public
TQSize kpToolResizeScaleCommand::newSize () const
{
    return TQSize (newWidth (), newHeight ());
}

// public virtual
void kpToolResizeScaleCommand::resize (int width, int height)
{
    m_newWidth = width;
    m_newHeight = height;

    m_isLosslessScale = ((m_type == Scale) &&
                         (m_newWidth / m_oldWidth * m_oldWidth == m_newWidth) &&
                         (m_newHeight / m_oldHeight * m_oldHeight == m_newHeight));
}


// public
bool kpToolResizeScaleCommand::scaleSelectionWithImage () const
{
    return m_scaleSelectionWithImage;
}


// private
void kpToolResizeScaleCommand::scaleSelectionRegionWithDocument ()
{
#if DEBUG_KP_TOOL_RESIZE_SCALE_COMMAND
    kdDebug () << "kpToolResizeScaleCommand::scaleSelectionRegionWithDocument"
               << endl;
#endif

    if (!m_oldSelection)
    {
        kdError () << "kpToolResizeScaleCommand::scaleSelectionRegionWithDocument()"
                   << " without old sel" << endl;
        return;
    }

    if (m_oldSelection->pixmap ())
    {
        kdError () << "kpToolResizeScaleCommand::scaleSelectionRegionWithDocument()"
                   << " old sel has pixmap" << endl;
        return;
    }


    const double horizScale = double (m_newWidth) / double (m_oldWidth);
    const double vertScale = double (m_newHeight) / double (m_oldHeight);

    const int newX = (int) (m_oldSelection->x () * horizScale);
    const int newY = (int) (m_oldSelection->y () * vertScale);


    TQPointArray currentPoints = m_oldSelection->points ();
    currentPoints.detach ();

    currentPoints.translate (-currentPoints.boundingRect ().x (),
                             -currentPoints.boundingRect ().y ());

    // TODO: refactor into kpPixmapFX
    TQWMatrix scaleMatrix;
    scaleMatrix.scale (horizScale, vertScale);
    currentPoints = scaleMatrix.map (currentPoints);

    currentPoints.translate (
        -currentPoints.boundingRect ().x () + newX,
        -currentPoints.boundingRect ().y () + newY);

    document ()->setSelection (kpSelection (currentPoints, TQPixmap (),
                                            m_oldSelection->transparency ()));


    if (m_mainWindow->tool ())
        m_mainWindow->tool ()->somethingBelowTheCursorChanged ();
}


// public virtual [base kpCommand]
void kpToolResizeScaleCommand::execute ()
{
#if DEBUG_KP_TOOL_RESIZE_SCALE_COMMAND
    kdDebug () << "kpToolResizeScaleCommand::execute() type="
               << (int) m_type
               << " oldWidth=" << m_oldWidth
               << " oldHeight=" << m_oldHeight
               << " newWidth=" << m_newWidth
               << " newHeight=" << m_newHeight
               << endl;
#endif

    if (m_oldWidth == m_newWidth && m_oldHeight == m_newHeight)
        return;

    if (m_type == Resize)
    {
        if (m_actOnSelection)
        {
            if (!m_actOnTextSelection)
            {
                kdError () << "kpToolResizeScaleCommand::execute() resizing sel doesn't make sense" << endl;
                return;
            }
            else
            {
                TQApplication::setOverrideCursor (TQt::waitCursor);
                document ()->selection ()->textResize (m_newWidth, m_newHeight);

                if (m_mainWindow->tool ())
                    m_mainWindow->tool ()->somethingBelowTheCursorChanged ();

                TQApplication::restoreOverrideCursor ();
            }
        }
        else
        {
            TQApplication::setOverrideCursor (TQt::waitCursor);


            if (m_newWidth < m_oldWidth)
            {
                m_oldRightPixmap = kpPixmapFX::getPixmapAt (
                    *document ()->pixmap (),
                    TQRect (m_newWidth, 0,
                        m_oldWidth - m_newWidth, m_oldHeight));
            }

            if (m_newHeight < m_oldHeight)
            {
                m_oldBottomPixmap = kpPixmapFX::getPixmapAt (
                    *document ()->pixmap (),
                    TQRect (0, m_newHeight,
                        m_newWidth, m_oldHeight - m_newHeight));
            }

            document ()->resize (m_newWidth, m_newHeight, m_backgroundColor);


            TQApplication::restoreOverrideCursor ();
        }
    }
    else
    {
        TQApplication::setOverrideCursor (TQt::waitCursor);


        TQPixmap oldPixmap = *document ()->pixmap (m_actOnSelection);

        if (!m_isLosslessScale)
            m_oldPixmap = oldPixmap;

        TQPixmap newPixmap = kpPixmapFX::scale (oldPixmap, m_newWidth, m_newHeight,
                                               m_type == SmoothScale);


        if (!m_oldSelection && document ()->selection ())
        {
            // Save sel border
            m_oldSelection = new kpSelection (*document ()->selection ());
            m_oldSelection->setPixmap (TQPixmap ());
        }

        if (m_actOnSelection)
        {
            TQRect newRect = TQRect (m_oldSelection->x (), m_oldSelection->y (),
                                   newPixmap.width (), newPixmap.height ());

            // Not possible to retain non-rectangular selection borders on scale
            // (think about e.g. a 45 deg line as part of the border & 2x scale)
            document ()->setSelection (
                kpSelection (kpSelection::Rectangle, newRect, newPixmap,
                             m_oldSelection->transparency ()));

            if (m_mainWindow->tool ())
                m_mainWindow->tool ()->somethingBelowTheCursorChanged ();
        }
        else
        {
            document ()->setPixmap (newPixmap);

            if (m_scaleSelectionWithImage)
            {
                scaleSelectionRegionWithDocument ();
            }
        }


        TQApplication::restoreOverrideCursor ();
    }
}

// public virtual [base kpCommand]
void kpToolResizeScaleCommand::unexecute ()
{
#if DEBUG_KP_TOOL_RESIZE_SCALE_COMMAND
    kdDebug () << "kpToolResizeScaleCommand::unexecute() type="
               << m_type << endl;
#endif

    if (m_oldWidth == m_newWidth && m_oldHeight == m_newHeight)
        return;

    kpDocument *doc = document ();
    if (!doc)
        return;

    if (m_type == Resize)
    {
        if (m_actOnSelection)
        {
            if (!m_actOnTextSelection)
            {
                kdError () << "kpToolResizeScaleCommand::unexecute() resizing sel doesn't make sense" << endl;
                return;
            }
            else
            {
                TQApplication::setOverrideCursor (TQt::waitCursor);
                document ()->selection ()->textResize (m_oldWidth, m_oldHeight);

                if (m_mainWindow->tool ())
                    m_mainWindow->tool ()->somethingBelowTheCursorChanged ();

                TQApplication::restoreOverrideCursor ();
            }
        }
        else
        {
            TQApplication::setOverrideCursor (TQt::waitCursor);


            TQPixmap newPixmap (m_oldWidth, m_oldHeight);

            kpPixmapFX::setPixmapAt (&newPixmap, TQPoint (0, 0),
                                    *doc->pixmap ());

            if (m_newWidth < m_oldWidth)
            {
                kpPixmapFX::setPixmapAt (&newPixmap,
                                        TQPoint (m_newWidth, 0),
                                        m_oldRightPixmap);
            }

            if (m_newHeight < m_oldHeight)
            {
                kpPixmapFX::setPixmapAt (&newPixmap,
                                        TQPoint (0, m_newHeight),
                                        m_oldBottomPixmap);
            }

            doc->setPixmap (newPixmap);


            TQApplication::restoreOverrideCursor ();
        }
    }
    else
    {
        TQApplication::setOverrideCursor (TQt::waitCursor);


        TQPixmap oldPixmap;

        if (!m_isLosslessScale)
            oldPixmap = m_oldPixmap;
        else
            oldPixmap = kpPixmapFX::scale (*doc->pixmap (m_actOnSelection),
                                           m_oldWidth, m_oldHeight);


        if (m_actOnSelection)
        {
            kpSelection oldSelection = *m_oldSelection;
            oldSelection.setPixmap (oldPixmap);
            doc->setSelection (oldSelection);

            if (m_mainWindow->tool ())
                m_mainWindow->tool ()->somethingBelowTheCursorChanged ();
        }
        else
        {
            doc->setPixmap (oldPixmap);

            if (m_scaleSelectionWithImage)
            {
                doc->setSelection (*m_oldSelection);

                if (m_mainWindow->tool ())
                    m_mainWindow->tool ()->somethingBelowTheCursorChanged ();
            }
        }


        TQApplication::restoreOverrideCursor ();
    }
}


/*
 * kpToolResizeScaleDialog
 */

#define SET_VALUE_WITHOUT_SIGNAL_EMISSION(knuminput_instance,value)    \
{                                                                      \
    knuminput_instance->blockSignals (true);                           \
    knuminput_instance->setValue (value);                              \
    knuminput_instance->blockSignals (false);                          \
}

#define IGNORE_KEEP_ASPECT_RATIO(cmd) \
{                                     \
    m_ignoreKeepAspectRatio++;        \
    cmd;                              \
    m_ignoreKeepAspectRatio--;        \
}


// private static
kpToolResizeScaleCommand::Type kpToolResizeScaleDialog::s_lastType =
    kpToolResizeScaleCommand::Resize;

// private static
double kpToolResizeScaleDialog::s_lastPercentWidth = 100,
       kpToolResizeScaleDialog::s_lastPercentHeight = 100;


kpToolResizeScaleDialog::kpToolResizeScaleDialog (kpMainWindow *mainWindow)
    : KDialogBase ((TQWidget *) mainWindow,
                   0/*name*/,
                   true/*modal*/,
                   i18n ("Resize / Scale")/*caption*/,
                   KDialogBase::Ok | KDialogBase::Cancel),
      m_mainWindow (mainWindow),
      m_ignoreKeepAspectRatio (0)
{
    // Using the percentage from last time become too confusing so disable for now
    s_lastPercentWidth = 100, s_lastPercentHeight = 100;


    TQWidget *baseWidget = new TQWidget (this);
    setMainWidget (baseWidget);


    createActOnBox (baseWidget);
    createOperationGroupBox (baseWidget);
    createDimensionsGroupBox (baseWidget);


    TQVBoxLayout *baseLayout = new TQVBoxLayout (baseWidget, 0/*margin*/, spacingHint ());
    baseLayout->addWidget (m_actOnBox);
    baseLayout->addWidget (m_operationGroupBox);
    baseLayout->addWidget (m_dimensionsGroupBox);


    slotActOnChanged ();

    m_newWidthInput->setEditFocus ();

    //enableButtonOK (!isNoOp ());
}

kpToolResizeScaleDialog::~kpToolResizeScaleDialog ()
{
}


// private
kpDocument *kpToolResizeScaleDialog::document () const
{
    return m_mainWindow ? m_mainWindow->document () : 0;
}

// private
kpSelection *kpToolResizeScaleDialog::selection () const
{
    return document () ? document ()->selection () : 0;
}


// private
void kpToolResizeScaleDialog::createActOnBox (TQWidget *baseWidget)
{
    m_actOnBox = new TQHBox (baseWidget);
    m_actOnBox->setSpacing (spacingHint () * 2);


    m_actOnLabel = new TQLabel (i18n ("Ac&t on:"), m_actOnBox);
    m_actOnCombo = new KComboBox (m_actOnBox);


    m_actOnLabel->setBuddy (m_actOnCombo);

    m_actOnCombo->insertItem (i18n ("Entire Image"), Image);
    if (selection ())
    {
        TQString selName = i18n ("Selection");

        if (selection ()->isText ())
            selName = i18n ("Text Box");

        m_actOnCombo->insertItem (selName, Selection);
        m_actOnCombo->setCurrentItem (Selection);
    }
    else
    {
        m_actOnLabel->setEnabled (false);
        m_actOnCombo->setEnabled (false);
    }


    m_actOnBox->setStretchFactor (m_actOnCombo, 1);


    connect (m_actOnCombo, TQT_SIGNAL (activated (int)),
             this, TQT_SLOT (slotActOnChanged ()));
}


static TQIconSet toolButtonIconSet (const TQString &iconName)
{
    TQIconSet iconSet = UserIconSet (iconName);


    // No "disabled" pixmap is generated by UserIconSet() so generate it
    // ourselves:

    TQPixmap disabledIcon = KGlobal::iconLoader ()->iconEffect ()->apply (
        UserIcon (iconName),
        KIcon::Toolbar, KIcon::DisabledState);

    const TQPixmap iconSetNormalIcon = iconSet.pixmap (TQIconSet::Small,
                                                      TQIconSet::Normal);

    // I bet past or future versions of KIconEffect::apply() resize the
    // disabled icon if we claim it's in group KIcon::Toolbar.  So resize
    // it to match the TQIconSet::Normal icon, just in case.
    disabledIcon = kpPixmapFX::scale (disabledIcon,
                                      iconSetNormalIcon.width (),
                                      iconSetNormalIcon.height (),
                                      true/*smooth scale*/);


    iconSet.setPixmap (disabledIcon,
                       TQIconSet::Small, TQIconSet::Disabled);

    return iconSet;
}

static void toolButtonSetLook (TQToolButton *button,
                               const TQString &iconName,
                               const TQString &name)
{
    button->setIconSet (toolButtonIconSet (iconName));
    button->setUsesTextLabel (true);
    button->setTextLabel (name, false/*no tooltip*/);
    button->setAccel (TQAccel::shortcutKey (name));
    button->setFocusPolicy (TQ_StrongFocus);
    button->setToggleButton (true);
}


// private
void kpToolResizeScaleDialog::createOperationGroupBox (TQWidget *baseWidget)
{
    m_operationGroupBox = new TQGroupBox (i18n ("Operation"), baseWidget);
    TQWhatsThis::add (m_operationGroupBox,
        i18n ("<qt>"
              "<ul>"
                  "<li><b>Resize</b>: The size of the picture will be"
                  " increased"
                  " by creating new areas to the right and/or bottom"
                  " (filled in with the background color) or"
                  " decreased by cutting"
                  " it at the right and/or bottom.</li>"

                  "<li><b>Scale</b>: The picture will be expanded"
                  " by duplicating pixels or squashed by dropping pixels.</li>"

                  "<li><b>Smooth Scale</b>: This is the same as"
                  " <i>Scale</i> except that it blends neighboring"
                  " pixels to produce a smoother looking picture.</li>"
              "</ul>"
              "</qt>"));

    // TODO: ALT+R doesn't select the button.
    m_resizeButton = new TQToolButton (m_operationGroupBox);
    toolButtonSetLook (m_resizeButton,
                       TQString::fromLatin1 ("resize"),
                       i18n ("&Resize"));

    m_scaleButton = new TQToolButton (m_operationGroupBox);
    toolButtonSetLook (m_scaleButton,
                       TQString::fromLatin1 ("scale"),
                       i18n ("&Scale"));

    m_smoothScaleButton = new TQToolButton (m_operationGroupBox);
    toolButtonSetLook (m_smoothScaleButton,
                       TQString::fromLatin1 ("smooth_scale"),
                       i18n ("S&mooth Scale"));


    //m_resizeLabel = new TQLabel (i18n ("&Resize"), m_operationGroupBox);
    //m_scaleLabel = new TQLabel (i18n ("&Scale"), m_operationGroupBox);
    //m_smoothScaleLabel = new TQLabel (i18n ("S&mooth scale"), m_operationGroupBox);


    //m_resizeLabel->setAlignment (m_resizeLabel->alignment () | TQt::ShowPrefix);
    //m_scaleLabel->setAlignment (m_scaleLabel->alignment () | TQt::ShowPrefix);
    //m_smoothScaleLabel->setAlignment (m_smoothScaleLabel->alignment () | TQt::ShowPrefix);


    TQButtonGroup *resizeScaleButtonGroup = new TQButtonGroup (baseWidget);
    resizeScaleButtonGroup->setExclusive (true);
    resizeScaleButtonGroup->hide ();

    resizeScaleButtonGroup->insert (m_resizeButton);
    resizeScaleButtonGroup->insert (m_scaleButton);
    resizeScaleButtonGroup->insert (m_smoothScaleButton);


    TQGridLayout *operationLayout = new TQGridLayout (m_operationGroupBox,
                                                    1, 2,
                                                    marginHint () * 2/*don't overlap groupbox title*/,
                                                    spacingHint ());

    operationLayout->addWidget (m_resizeButton, 0, 0, TQt::AlignCenter);
    //operationLayout->addWidget (m_resizeLabel, 1, 0, TQt::AlignCenter);

    operationLayout->addWidget (m_scaleButton, 0, 1, TQt::AlignCenter);
    //operationLayout->addWidget (m_scaleLabel, 1, 1, TQt::AlignCenter);

    operationLayout->addWidget (m_smoothScaleButton, 0, 2, TQt::AlignCenter);
    //operationLayout->addWidget (m_smoothScaleLabel, 1, 2, TQt::AlignCenter);


    connect (m_resizeButton, TQT_SIGNAL (toggled (bool)),
             this, TQT_SLOT (slotTypeChanged ()));
    connect (m_scaleButton, TQT_SIGNAL (toggled (bool)),
             this, TQT_SLOT (slotTypeChanged ()));
    connect (m_smoothScaleButton, TQT_SIGNAL (toggled (bool)),
             this, TQT_SLOT (slotTypeChanged ()));
}

// private
void kpToolResizeScaleDialog::createDimensionsGroupBox (TQWidget *baseWidget)
{
    m_dimensionsGroupBox = new TQGroupBox (i18n ("Dimensions"), baseWidget);

    TQLabel *widthLabel = new TQLabel (i18n ("Width:"), m_dimensionsGroupBox);
    widthLabel->setAlignment (widthLabel->alignment () | TQt::AlignHCenter);
    TQLabel *heightLabel = new TQLabel (i18n ("Height:"), m_dimensionsGroupBox);
    heightLabel->setAlignment (heightLabel->alignment () | TQt::AlignHCenter);

    TQLabel *originalLabel = new TQLabel (i18n ("Original:"), m_dimensionsGroupBox);
    m_originalWidthInput = new KIntNumInput (
        document ()->width ((bool) selection ()),
        m_dimensionsGroupBox);
    TQLabel *xLabel0 = new TQLabel (i18n ("x"), m_dimensionsGroupBox);
    m_originalHeightInput = new KIntNumInput (
        document ()->height ((bool) selection ()),
        m_dimensionsGroupBox);

    TQLabel *newLabel = new TQLabel (i18n ("&New:"), m_dimensionsGroupBox);
    m_newWidthInput = new KIntNumInput (m_dimensionsGroupBox);
    TQLabel *xLabel1 = new TQLabel (i18n ("x"), m_dimensionsGroupBox);
    m_newHeightInput = new KIntNumInput (m_dimensionsGroupBox);

    TQLabel *percentLabel = new TQLabel (i18n ("&Percent:"), m_dimensionsGroupBox);
    m_percentWidthInput = new KDoubleNumInput (0.01/*lower*/, 1000000/*upper*/,
                                               100/*value*/, 1/*step*/,
                                               2/*precision*/,
                                               m_dimensionsGroupBox);
    m_percentWidthInput->setSuffix (i18n ("%"));
    TQLabel *xLabel2 = new TQLabel (i18n ("x"), m_dimensionsGroupBox);
    m_percentHeightInput = new KDoubleNumInput (0.01/*lower*/, 1000000/*upper*/,
                                                100/*value*/, 1/*step*/,
                                                2/*precision*/,
                                                m_dimensionsGroupBox);
    m_percentHeightInput->setSuffix (i18n ("%"));

    m_keepAspectRatioCheckBox = new TQCheckBox (i18n ("Keep &aspect ratio"),
                                               m_dimensionsGroupBox);


    m_originalWidthInput->setEnabled (false);
    m_originalHeightInput->setEnabled (false);
    originalLabel->setBuddy (m_originalWidthInput);
    newLabel->setBuddy (m_newWidthInput);
    m_percentWidthInput->setValue (s_lastPercentWidth);
    m_percentHeightInput->setValue (s_lastPercentHeight);
    percentLabel->setBuddy (m_percentWidthInput);


    TQGridLayout *dimensionsLayout = new TQGridLayout (m_dimensionsGroupBox,
                                                     5, 4, marginHint () * 2, spacingHint ());
    dimensionsLayout->setColStretch (1/*column*/, 1);
    dimensionsLayout->setColStretch (3/*column*/, 1);


    dimensionsLayout->addWidget (widthLabel, 0, 1);
    dimensionsLayout->addWidget (heightLabel, 0, 3);

    dimensionsLayout->addWidget (originalLabel, 1, 0);
    dimensionsLayout->addWidget (m_originalWidthInput, 1, 1);
    dimensionsLayout->addWidget (xLabel0, 1, 2);
    dimensionsLayout->addWidget (m_originalHeightInput, 1, 3);

    dimensionsLayout->addWidget (newLabel, 2, 0);
    dimensionsLayout->addWidget (m_newWidthInput, 2, 1);
    dimensionsLayout->addWidget (xLabel1, 2, 2);
    dimensionsLayout->addWidget (m_newHeightInput, 2, 3);

    dimensionsLayout->addWidget (percentLabel, 3, 0);
    dimensionsLayout->addWidget (m_percentWidthInput, 3, 1);
    dimensionsLayout->addWidget (xLabel2, 3, 2);
    dimensionsLayout->addWidget (m_percentHeightInput, 3, 3);

    dimensionsLayout->addMultiCellWidget (m_keepAspectRatioCheckBox, 4, 4, 0, 3);
    dimensionsLayout->setRowStretch (4/*row*/, 1);
    dimensionsLayout->setRowSpacing (4/*row*/, dimensionsLayout->rowSpacing (4) * 2);


    connect (m_newWidthInput, TQT_SIGNAL (valueChanged (int)),
             this, TQT_SLOT (slotWidthChanged (int)));
    connect (m_newHeightInput, TQT_SIGNAL (valueChanged (int)),
             this, TQT_SLOT (slotHeightChanged (int)));

    connect (m_percentWidthInput, TQT_SIGNAL (valueChanged (double)),
             this, TQT_SLOT (slotPercentWidthChanged (double)));
    connect (m_percentHeightInput, TQT_SIGNAL (valueChanged (double)),
             this, TQT_SLOT (slotPercentHeightChanged (double)));

    connect (m_keepAspectRatioCheckBox, TQT_SIGNAL (toggled (bool)),
             this, TQT_SLOT (setKeepAspectRatio (bool)));
}


// private
void kpToolResizeScaleDialog::widthFitHeightToAspectRatio ()
{
    if (m_keepAspectRatioCheckBox->isChecked () && !m_ignoreKeepAspectRatio)
    {
        // width / height = oldWidth / oldHeight
        // height = width * oldHeight / oldWidth
        const int newHeight = tqRound (double (imageWidth ()) * double (originalHeight ())
                                      / double (originalWidth ()));
        IGNORE_KEEP_ASPECT_RATIO (m_newHeightInput->setValue (newHeight));
    }
}

// private
void kpToolResizeScaleDialog::heightFitWidthToAspectRatio ()
{
    if (m_keepAspectRatioCheckBox->isChecked () && !m_ignoreKeepAspectRatio)
    {
        // width / height = oldWidth / oldHeight
        // width = height * oldWidth / oldHeight
        const int newWidth = tqRound (double (imageHeight ()) * double (originalWidth ())
                                     / double (originalHeight ()));
        IGNORE_KEEP_ASPECT_RATIO (m_newWidthInput->setValue (newWidth));
    }
}


// private
bool kpToolResizeScaleDialog::resizeEnabled () const
{
    return (!actOnSelection () ||
            (actOnSelection () && selection ()->isText ()));
}

// private
bool kpToolResizeScaleDialog::scaleEnabled () const
{
    return (!(actOnSelection () && selection ()->isText ()));
}

// private
bool kpToolResizeScaleDialog::smoothScaleEnabled () const
{
    return scaleEnabled ();
}


// public slot
void kpToolResizeScaleDialog::slotActOnChanged ()
{
#if DEBUG_KP_TOOL_RESIZE_SCALE_DIALOG && 1
    kdDebug () << "kpToolResizeScaleDialog::slotActOnChanged()" << endl;
#endif

    m_resizeButton->setEnabled (resizeEnabled ());
    //m_resizeLabel->setEnabled (resizeEnabled ());

    m_scaleButton->setEnabled (scaleEnabled ());
    //m_scaleLabel->setEnabled (scaleEnabled ());

    m_smoothScaleButton->setEnabled (smoothScaleEnabled ());
    //m_smoothScaleLabel->setEnabled (smoothScaleEnabled ());


    // TODO: somehow share logic with (resize|*scale)Enabled()
    if (actOnSelection ())
    {
        if (selection ()->isText ())
        {
            m_resizeButton->setOn (true);
        }
        else
        {
            if (s_lastType == kpToolResizeScaleCommand::Scale)
                m_scaleButton->setOn (true);
            else
                m_smoothScaleButton->setOn (true);
        }
    }
    else
    {
        if (s_lastType == kpToolResizeScaleCommand::Resize)
            m_resizeButton->setOn (true);
        else if (s_lastType == kpToolResizeScaleCommand::Scale)
            m_scaleButton->setOn (true);
        else
            m_smoothScaleButton->setOn (true);
    }


    m_originalWidthInput->setValue (originalWidth ());
    m_originalHeightInput->setValue (originalHeight ());


    m_newWidthInput->blockSignals (true);
    m_newHeightInput->blockSignals (true);

    m_newWidthInput->setMinValue (actOnSelection () ?
                                      selection ()->minimumWidth () :
                                      1);
    m_newHeightInput->setMinValue (actOnSelection () ?
                                       selection ()->minimumHeight () :
                                       1);

    m_newWidthInput->blockSignals (false);
    m_newHeightInput->blockSignals (false);


    IGNORE_KEEP_ASPECT_RATIO (slotPercentWidthChanged (m_percentWidthInput->value ()));
    IGNORE_KEEP_ASPECT_RATIO (slotPercentHeightChanged (m_percentHeightInput->value ()));

    setKeepAspectRatio (m_keepAspectRatioCheckBox->isChecked ());
}


// public slot
void kpToolResizeScaleDialog::slotTypeChanged ()
{
    s_lastType = type ();
}

// public slot
void kpToolResizeScaleDialog::slotWidthChanged (int width)
{
#if DEBUG_KP_TOOL_RESIZE_SCALE_DIALOG && 1
    kdDebug () << "kpToolResizeScaleDialog::slotWidthChanged("
               << width << ")" << endl;
#endif
    const double newPercentWidth = double (width) * 100 / double (originalWidth ());

    SET_VALUE_WITHOUT_SIGNAL_EMISSION (m_percentWidthInput, newPercentWidth);

    widthFitHeightToAspectRatio ();

    //enableButtonOK (!isNoOp ());
    s_lastPercentWidth = newPercentWidth;
}

// public slot
void kpToolResizeScaleDialog::slotHeightChanged (int height)
{
#if DEBUG_KP_TOOL_RESIZE_SCALE_DIALOG && 1
    kdDebug () << "kpToolResizeScaleDialog::slotHeightChanged("
               << height << ")" << endl;
#endif
    const double newPercentHeight = double (height) * 100 / double (originalHeight ());

    SET_VALUE_WITHOUT_SIGNAL_EMISSION (m_percentHeightInput, newPercentHeight);

    heightFitWidthToAspectRatio ();

    //enableButtonOK (!isNoOp ());
    s_lastPercentHeight = newPercentHeight;
}

// public slot
void kpToolResizeScaleDialog::slotPercentWidthChanged (double percentWidth)
{
#if DEBUG_KP_TOOL_RESIZE_SCALE_DIALOG && 1
    kdDebug () << "kpToolResizeScaleDialog::slotPercentWidthChanged("
               << percentWidth << ")" << endl;
#endif

    SET_VALUE_WITHOUT_SIGNAL_EMISSION (m_newWidthInput,
                                       tqRound (percentWidth * originalWidth () / 100.0));

    widthFitHeightToAspectRatio ();

    //enableButtonOK (!isNoOp ());
    s_lastPercentWidth = percentWidth;
}

// public slot
void kpToolResizeScaleDialog::slotPercentHeightChanged (double percentHeight)
{
#if DEBUG_KP_TOOL_RESIZE_SCALE_DIALOG && 1
    kdDebug () << "kpToolResizeScaleDialog::slotPercentHeightChanged("
               << percentHeight << ")" << endl;
#endif

    SET_VALUE_WITHOUT_SIGNAL_EMISSION (m_newHeightInput,
                                       tqRound (percentHeight * originalHeight () / 100.0));

    heightFitWidthToAspectRatio ();

    //enableButtonOK (!isNoOp ());
    s_lastPercentHeight = percentHeight;
}

// public
bool kpToolResizeScaleDialog::keepAspectRatio () const
{
    return m_keepAspectRatioCheckBox->isChecked ();
}

// public slot
void kpToolResizeScaleDialog::setKeepAspectRatio (bool on)
{
#if DEBUG_KP_TOOL_RESIZE_SCALE_DIALOG && 1
    kdDebug () << "kpToolResizeScaleDialog::setKeepAspectRatio("
               << on << ")" << endl;
#endif
    if (on != m_keepAspectRatioCheckBox->isChecked ())
        m_keepAspectRatioCheckBox->setChecked (on);

    if (on)
        widthFitHeightToAspectRatio ();
}

#undef IGNORE_KEEP_ASPECT_RATIO
#undef SET_VALUE_WITHOUT_SIGNAL_EMISSION


// private
int kpToolResizeScaleDialog::originalWidth () const
{
    return document ()->width (actOnSelection ());
}

// private
int kpToolResizeScaleDialog::originalHeight () const
{
    return document ()->height (actOnSelection ());
}


// public
int kpToolResizeScaleDialog::imageWidth () const
{
    return m_newWidthInput->value ();
}

// public
int kpToolResizeScaleDialog::imageHeight () const
{
    return m_newHeightInput->value ();
}

// public
bool kpToolResizeScaleDialog::actOnSelection () const
{
    return (m_actOnCombo->currentItem () == Selection);
}

// public
kpToolResizeScaleCommand::Type kpToolResizeScaleDialog::type () const
{
    if (m_resizeButton->isOn ())
        return kpToolResizeScaleCommand::Resize;
    else if (m_scaleButton->isOn ())
        return kpToolResizeScaleCommand::Scale;
    else
        return kpToolResizeScaleCommand::SmoothScale;
}

// public
bool kpToolResizeScaleDialog::isNoOp () const
{
    return (imageWidth () == originalWidth () &&
            imageHeight () == originalHeight ());
}


// private slot virtual [base KDialogBase]
void kpToolResizeScaleDialog::slotOk ()
{
    enum { eText, eSelection, eImage } actionTarget = eText;

    if (actOnSelection ())
    {
        if (selection ()->isText ())
        {
            actionTarget = eText;
        }
        else
        {
            actionTarget = eSelection;
        }
    }
    else
    {
        actionTarget = eImage;
    }


    TQString message, caption, continueButtonText;

    // Note: If eText, can't Scale nor SmoothScale.
    //       If eSelection, can't Resize.

    switch (type ())
    {
    default:
    case kpToolResizeScaleCommand::Resize:
        if (actionTarget == eText)
        {
            message =
                i18n ("<qt><p>Resizing the text box to %1x%2"
                      " may take a substantial amount of memory."
                      " This can reduce system"
                      " responsiveness and cause other application resource"
                      " problems.</p>"

                      "<p>Are you sure you want to resize the text box?</p></qt>");

            caption = i18n ("Resize Text Box?");
            continueButtonText = i18n ("R&esize Text Box");
        }
        else if (actionTarget == eImage)
        {
            message =
                i18n ("<qt><p>Resizing the image to %1x%2"
                      " may take a substantial amount of memory."
                      " This can reduce system"
                      " responsiveness and cause other application resource"
                      " problems.</p>"

                      "<p>Are you sure you want to resize the image?</p></qt>");

            caption = i18n ("Resize Image?");
            continueButtonText = i18n ("R&esize Image");
        }

        break;

    case kpToolResizeScaleCommand::Scale:
        if (actionTarget == eImage)
        {
            message =
                i18n ("<qt><p>Scaling the image to %1x%2"
                      " may take a substantial amount of memory."
                      " This can reduce system"
                      " responsiveness and cause other application resource"
                      " problems.</p>"

                      "<p>Are you sure you want to scale the image?</p></qt>");

            caption = i18n ("Scale Image?");
            continueButtonText = i18n ("Scal&e Image");
        }
        else if (actionTarget == eSelection)
        {
            message =
                i18n ("<qt><p>Scaling the selection to %1x%2"
                      " may take a substantial amount of memory."
                      " This can reduce system"
                      " responsiveness and cause other application resource"
                      " problems.</p>"

                      "<p>Are you sure you want to scale the selection?</p></qt>");

            caption = i18n ("Scale Selection?");
            continueButtonText = i18n ("Scal&e Selection");
        }

        break;

    case kpToolResizeScaleCommand::SmoothScale:
        if (actionTarget == eImage)
        {
            message =
                i18n ("<qt><p>Smooth Scaling the image to %1x%2"
                      " may take a substantial amount of memory."
                      " This can reduce system"
                      " responsiveness and cause other application resource"
                      " problems.</p>"

                      "<p>Are you sure you want to smooth scale the image?</p></qt>");

            caption = i18n ("Smooth Scale Image?");
            continueButtonText = i18n ("Smooth Scal&e Image");
        }
        else if (actionTarget == eSelection)
        {
            message =
                i18n ("<qt><p>Smooth Scaling the selection to %1x%2"
                      " may take a substantial amount of memory."
                      " This can reduce system"
                      " responsiveness and cause other application resource"
                      " problems.</p>"

                      "<p>Are you sure you want to smooth scale the selection?</p></qt>");

            caption = i18n ("Smooth Scale Selection?");
            continueButtonText = i18n ("Smooth Scal&e Selection");
        }

        break;
    }


    if (kpTool::warnIfBigImageSize (originalWidth (),
            originalHeight (),
            imageWidth (), imageHeight (),
            message.arg (imageWidth ()).arg (imageHeight ()),
            caption,
            continueButtonText,
            this))
    {
        KDialogBase::slotOk ();
    }
}


#include <kptoolresizescale.moc>
