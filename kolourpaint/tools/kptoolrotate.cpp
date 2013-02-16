
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

#define DEBUG_KP_TOOL_ROTATE 0


#include <kptoolrotate.h>

#include <tqapplication.h>
#include <tqbuttongroup.h>
#include <tqgroupbox.h>
#include <tqlabel.h>
#include <tqlayout.h>
#include <tqpushbutton.h>
#include <tqradiobutton.h>
#include <tqwmatrix.h>

#include <kdebug.h>
#include <kiconloader.h>
#include <knuminput.h>
#include <tdelocale.h>

#include <kpdefs.h>
#include <kpdocument.h>
#include <kpmainwindow.h>
#include <kppixmapfx.h>
#include <kpselection.h>
#include <kptool.h>
#include <kpviewmanager.h>


kpToolRotateCommand::kpToolRotateCommand (bool actOnSelection,
                                          double angle,
                                          kpMainWindow *mainWindow)
    : kpCommand (mainWindow),
      m_actOnSelection (actOnSelection),
      m_angle (angle),
      m_backgroundColor (mainWindow ? mainWindow->backgroundColor (actOnSelection) : kpColor::invalid),
      m_losslessRotation (kpPixmapFX::isLosslessRotation (angle))
{
}

kpToolRotateCommand::~kpToolRotateCommand ()
{
}


// public virtual [base kpCommand]
TQString kpToolRotateCommand::name () const
{
    TQString opName = i18n ("Rotate");

    if (m_actOnSelection)
        return i18n ("Selection: %1").arg (opName);
    else
        return opName;
}


// public virtual [base kpCommand]
int kpToolRotateCommand::size () const
{
    return kpPixmapFX::pixmapSize (m_oldPixmap) +
           m_oldSelection.size ();
}


// public virtual [base kpCommand]
void kpToolRotateCommand::execute ()
{
    kpDocument *doc = document ();
    if (!doc)
        return;


    TQApplication::setOverrideCursor (TQt::waitCursor);


    if (!m_losslessRotation)
        m_oldPixmap = *doc->pixmap (m_actOnSelection);


    TQPixmap newPixmap = kpPixmapFX::rotate (*doc->pixmap (m_actOnSelection),
                                            m_angle,
                                            m_backgroundColor);


    if (m_actOnSelection)
    {
        kpSelection *sel = doc->selection ();

        // Save old selection
        m_oldSelection = *sel;
        m_oldSelection.setPixmap (TQPixmap ());


        // Calculate new top left (so selection rotates about centre)
        // (the Times2 trickery is used to reduce integer division error without
        //  resorting to the troublesome world of floating point)
        TQPoint oldCenterTimes2 (sel->x () * 2 + sel->width (),
                                sel->y () * 2 + sel->height ());
        TQPoint newTopLeftTimes2 (oldCenterTimes2 - TQPoint (newPixmap.width (), newPixmap.height ()));
        TQPoint newTopLeft (newTopLeftTimes2.x () / 2, newTopLeftTimes2.y () / 2);


        // Calculate rotated points
        TQPointArray currentPoints = sel->points ();
        currentPoints.translate (-currentPoints.boundingRect ().x (),
                                 -currentPoints.boundingRect ().y ());
        TQWMatrix rotateMatrix = kpPixmapFX::rotateMatrix (*doc->pixmap (m_actOnSelection), m_angle);
        currentPoints = rotateMatrix.map (currentPoints);
        currentPoints.translate (-currentPoints.boundingRect ().x () + newTopLeft.x (),
                                 -currentPoints.boundingRect ().y () + newTopLeft.y ());


        if (currentPoints.boundingRect ().width () == newPixmap.width () &&
            currentPoints.boundingRect ().height () == newPixmap.height ())
        {
            doc->setSelection (kpSelection (currentPoints, newPixmap,
                                            m_oldSelection.transparency ()));
        }
        else
        {
            // TODO: fix the latter "victim of" problem in kpSelection by
            //       allowing the border width & height != pixmap width & height
            //       Or maybe autocrop?
        #if DEBUG_KP_TOOL_ROTATE
            kdDebug () << "kpToolRotateCommand::execute() currentPoints.boundingRect="
                       << currentPoints.boundingRect ()
                       << " newPixmap: w=" << newPixmap.width ()
                       << " h=" << newPixmap.height ()
                       << " (victim of rounding error and/or rotated-a-(rectangular)-pixmap-that-was-transparent-in-the-corners-making-sel-uselessly-bigger-than-needs-be)"
                       << endl;
        #endif
            doc->setSelection (kpSelection (kpSelection::Rectangle,
                                            TQRect (newTopLeft.x (), newTopLeft.y (),
                                                   newPixmap.width (), newPixmap.height ()),
                                            newPixmap,
                                            m_oldSelection.transparency ()));
        }

        if (m_mainWindow->tool ())
            m_mainWindow->tool ()->somethingBelowTheCursorChanged ();
    }
    else
        doc->setPixmap (newPixmap);


    TQApplication::restoreOverrideCursor ();
}

// public virtual [base kpCommand]
void kpToolRotateCommand::unexecute ()
{
    kpDocument *doc = document ();
    if (!doc)
        return;


    TQApplication::setOverrideCursor (TQt::waitCursor);


    TQPixmap oldPixmap;

    if (!m_losslessRotation)
    {
        oldPixmap = m_oldPixmap;
        m_oldPixmap.resize (0, 0);
    }
    else
    {
        oldPixmap = kpPixmapFX::rotate (*doc->pixmap (m_actOnSelection),
                                        360 - m_angle,
                                        m_backgroundColor);
    }


    if (!m_actOnSelection)
        doc->setPixmap (oldPixmap);
    else
    {
        kpSelection oldSelection = m_oldSelection;
        oldSelection.setPixmap (oldPixmap);
        doc->setSelection (oldSelection);

        if (m_mainWindow->tool ())
            m_mainWindow->tool ()->somethingBelowTheCursorChanged ();
    }


    TQApplication::restoreOverrideCursor ();
}


/*
 * kpToolRotateDialog
 */


// private static
int kpToolRotateDialog::s_lastWidth = -1,
    kpToolRotateDialog::s_lastHeight = -1;

// private static
bool kpToolRotateDialog::s_lastIsClockwise = true;
int kpToolRotateDialog::s_lastAngleRadioButtonID = 3;
int kpToolRotateDialog::s_lastAngleCustom = 0;


kpToolRotateDialog::kpToolRotateDialog (bool actOnSelection,
                                        kpMainWindow *mainWindow,
                                        const char *name)
    : kpToolPreviewDialog (kpToolPreviewDialog::AllFeatures,
                           false/*don't reserve top row*/,
                           actOnSelection ? i18n ("Rotate Selection") : i18n ("Rotate Image"),
                           i18n ("After Rotate:"),
                           actOnSelection, mainWindow, name)
{
    // Too confusing - disable for now
    s_lastAngleRadioButtonID = 3;
    s_lastAngleCustom = 0;


    createDirectionGroupBox ();
    createAngleGroupBox ();


    if (s_lastWidth > 0 && s_lastHeight > 0)
        resize (s_lastWidth, s_lastHeight);


    slotAngleCustomRadioButtonToggled (m_angleCustomRadioButton->isChecked ());
    slotUpdate ();
}

kpToolRotateDialog::~kpToolRotateDialog ()
{
    s_lastWidth = width (), s_lastHeight = height ();
}


// private
void kpToolRotateDialog::createDirectionGroupBox ()
{
    TQGroupBox *directionGroupBox = new TQGroupBox (i18n ("Direction"), mainWidget ());
    addCustomWidget (directionGroupBox);


    TQLabel *antiClockwisePixmapLabel = new TQLabel (directionGroupBox);
    antiClockwisePixmapLabel->setPixmap (UserIcon ("image_rotate_anticlockwise"));

    TQLabel *clockwisePixmapLabel = new TQLabel (directionGroupBox);
    clockwisePixmapLabel->setPixmap (UserIcon ("image_rotate_clockwise"));


    m_antiClockwiseRadioButton = new TQRadioButton (i18n ("Cou&nterclockwise"), directionGroupBox);
    m_clockwiseRadioButton = new TQRadioButton (i18n ("C&lockwise"), directionGroupBox);


    m_antiClockwiseRadioButton->setChecked (!s_lastIsClockwise);
    m_clockwiseRadioButton->setChecked (s_lastIsClockwise);


    TQButtonGroup *buttonGroup = new TQButtonGroup (directionGroupBox);
    buttonGroup->hide ();

    buttonGroup->insert (m_antiClockwiseRadioButton);
    buttonGroup->insert (m_clockwiseRadioButton);


    TQGridLayout *directionLayout = new TQGridLayout (directionGroupBox,
                                                    2, 2, marginHint () * 2, spacingHint ());
    directionLayout->addWidget (antiClockwisePixmapLabel, 0, 0, TQt::AlignCenter);
    directionLayout->addWidget (clockwisePixmapLabel, 0, 1, TQt::AlignCenter);
    directionLayout->addWidget (m_antiClockwiseRadioButton, 1, 0, TQt::AlignCenter);
    directionLayout->addWidget (m_clockwiseRadioButton, 1, 1, TQt::AlignCenter);


    connect (m_antiClockwiseRadioButton, TQT_SIGNAL (toggled (bool)),
             this, TQT_SLOT (slotUpdate ()));
    connect (m_clockwiseRadioButton, TQT_SIGNAL (toggled (bool)),
             this, TQT_SLOT (slotUpdate ()));
}

// private
void kpToolRotateDialog::createAngleGroupBox ()
{
    TQGroupBox *angleGroupBox = new TQGroupBox (i18n ("Angle"), mainWidget ());
    addCustomWidget (angleGroupBox);


    m_angle90RadioButton = new TQRadioButton (i18n ("90 &degrees"), angleGroupBox);
    m_angle180RadioButton = new TQRadioButton (i18n ("180 d&egrees"), angleGroupBox);
    m_angle270RadioButton = new TQRadioButton (i18n ("270 de&grees"), angleGroupBox);

    m_angleCustomRadioButton = new TQRadioButton (i18n ("C&ustom:"), angleGroupBox);
    m_angleCustomInput = new KIntNumInput (s_lastAngleCustom, angleGroupBox);
    m_angleCustomInput->setMinValue (-359);
    m_angleCustomInput->setMaxValue (+359);
    TQLabel *degreesLabel = new TQLabel (i18n ("degrees"), angleGroupBox);


    m_angleButtonGroup = new TQButtonGroup (angleGroupBox);
    m_angleButtonGroup->hide ();

    m_angleButtonGroup->insert (m_angle90RadioButton);
    m_angleButtonGroup->insert (m_angle180RadioButton);
    m_angleButtonGroup->insert (m_angle270RadioButton);

    m_angleButtonGroup->insert (m_angleCustomRadioButton);

    m_angleButtonGroup->setButton (s_lastAngleRadioButtonID);


    TQGridLayout *angleLayout = new TQGridLayout (angleGroupBox,
                                                6, 3,
                                                marginHint () * 2, spacingHint ());

    angleLayout->addMultiCellWidget (m_angle90RadioButton, 0, 0, 0, 2);
    angleLayout->addMultiCellWidget (m_angle180RadioButton, 1, 1, 0, 2);
    angleLayout->addMultiCellWidget (m_angle270RadioButton, 2, 2, 0, 2);

    angleLayout->addWidget (m_angleCustomRadioButton, 3, 0);
    angleLayout->addWidget (m_angleCustomInput, 3, 1);
    angleLayout->addWidget (degreesLabel, 3, 2);

    angleLayout->setColStretch (1, 2);  // Stretch Custom Angle Input


    connect (m_angle90RadioButton, TQT_SIGNAL (toggled (bool)),
             this, TQT_SLOT (slotUpdate ()));
    connect (m_angle180RadioButton, TQT_SIGNAL (toggled (bool)),
             this, TQT_SLOT (slotUpdate ()));
    connect (m_angle270RadioButton, TQT_SIGNAL (toggled (bool)),
             this, TQT_SLOT (slotUpdate ()));

    connect (m_angleCustomRadioButton, TQT_SIGNAL (toggled (bool)),
             this, TQT_SLOT (slotAngleCustomRadioButtonToggled (bool)));
    connect (m_angleCustomRadioButton, TQT_SIGNAL (toggled (bool)),
             this, TQT_SLOT (slotUpdate ()));

    connect (m_angleCustomInput, TQT_SIGNAL (valueChanged (int)),
             this, TQT_SLOT (slotUpdate ()));
}


// public virtual [base kpToolPreviewDialog]
bool kpToolRotateDialog::isNoOp () const
{
    return (angle () == 0);
}

// public
int kpToolRotateDialog::angle () const
{
    int retAngle;


    if (m_angle90RadioButton->isChecked ())
        retAngle = 90;
    else if (m_angle180RadioButton->isChecked ())
        retAngle = 180;
    else if (m_angle270RadioButton->isChecked ())
        retAngle = 270;
    else // if (m_angleCustomRadioButton->isChecked ())
        retAngle = m_angleCustomInput->value ();


    if (m_antiClockwiseRadioButton->isChecked ())
        retAngle *= -1;


    if (retAngle < 0)
        retAngle += ((0 - retAngle) / 360 + 1) * 360;

    if (retAngle >= 360)
        retAngle -= ((retAngle - 360) / 360 + 1) * 360;


    return retAngle;
}


// private virtual [base kpToolPreviewDialog]
TQSize kpToolRotateDialog::newDimensions () const
{
    TQWMatrix matrix = kpPixmapFX::rotateMatrix (m_oldWidth, m_oldHeight, angle ());
    // TODO: Should we be using TQWMatrix::Areas?
    TQRect rect = matrix.map (TQRect (0, 0, m_oldWidth, m_oldHeight));
    return rect.size ();
}

// private virtual [base kpToolPreviewDialog]
TQPixmap kpToolRotateDialog::transformPixmap (const TQPixmap &pixmap,
                                             int targetWidth, int targetHeight) const
{
    return kpPixmapFX::rotate (pixmap, angle (),
                               m_mainWindow ? m_mainWindow->backgroundColor (m_actOnSelection) : kpColor::invalid,
                               targetWidth, targetHeight);
}


// private slot
void kpToolRotateDialog::slotAngleCustomRadioButtonToggled (bool isChecked)
{
    m_angleCustomInput->setEnabled (isChecked);

    if (isChecked)
        m_angleCustomInput->setEditFocus ();
}

// private slot virtual [base kpToolPreviewDialog]
void kpToolRotateDialog::slotUpdate ()
{
    s_lastIsClockwise = m_clockwiseRadioButton->isChecked ();
    s_lastAngleRadioButtonID = m_angleButtonGroup->selectedId ();
    s_lastAngleCustom = m_angleCustomInput->value ();

    kpToolPreviewDialog::slotUpdate ();
}


// private slot virtual [base KDialogBase]
void kpToolRotateDialog::slotOk ()
{
    TQString message, caption, continueButtonText;

    if (document ()->selection ())
    {
        if (!document ()->selection ()->isText ())
        {
            message =
                i18n ("<qt><p>Rotating the selection to %1x%2"
                      " may take a substantial amount of memory."
                      " This can reduce system"
                      " responsiveness and cause other application resource"
                      " problems.</p>"

                      "<p>Are you sure want to rotate the selection?</p></qt>");

            caption = i18n ("Rotate Selection?");
            continueButtonText = i18n ("Rotat&e Selection");
        }
    }
    else
    {
        message =
            i18n ("<qt><p>Rotating the image to %1x%2"
                  " may take a substantial amount of memory."
                  " This can reduce system"
                  " responsiveness and cause other application resource"
                  " problems.</p>"

                  "<p>Are you sure want to rotate the image?</p></qt>");

        caption = i18n ("Rotate Image?");
        continueButtonText = i18n ("Rotat&e Image");
    }


    const int newWidth = newDimensions ().width ();
    const int newHeight = newDimensions ().height ();

    if (kpTool::warnIfBigImageSize (m_oldWidth,
            m_oldHeight,
            newWidth, newHeight,
            message.arg (newWidth).arg (newHeight),
            caption,
            continueButtonText,
            this))
    {
        KDialogBase::slotOk ();
    }
}

#include <kptoolrotate.moc>
