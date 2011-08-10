/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Andreas Zehender
    email                : zehender@kde.org
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/

#ifndef PMPREVIEWSETTINGS_H
#define PMPREVIEWSETTINGS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "pmsettingsdialog.h"

class PMIntEdit;
class PMFloatEdit;
class TQCheckBox;
class KColorButton;

/**
 * Texture preview configuration dialog page
 */
class PMPreviewSettings : public PMSettingsDialogPage
{
   Q_OBJECT
  TQ_OBJECT
public:
   /**
    * Default constructor
    */
   PMPreviewSettings( TQWidget* parent, const char* name = 0 );
   /** */
   virtual void displaySettings( );
   /** */
   virtual bool validateData( );
   /** */
   virtual void applySettings( );
   /** */
   virtual void displayDefaults( );

private:
   PMIntEdit* m_pPreviewSize;
   TQCheckBox* m_pPreviewSphere;
   TQCheckBox* m_pPreviewCylinder;
   TQCheckBox* m_pPreviewBox;
   TQCheckBox* m_pPreviewAA;
   PMIntEdit* m_pPreviewAALevel;
   PMFloatEdit* m_pPreviewAAThreshold;
   TQCheckBox* m_pPreviewWall;
   TQCheckBox* m_pPreviewFloor;
   KColorButton* m_pFloorColor1;
   KColorButton* m_pFloorColor2;
   KColorButton* m_pWallColor1;
   KColorButton* m_pWallColor2;
   PMFloatEdit* m_pPreviewGamma;
};

#endif
