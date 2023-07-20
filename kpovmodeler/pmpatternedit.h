/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001 by Luis Carvalho
    email                : lpassos@mail.telepac.pt
    copyright            : (C) 2001 by Andreas Zehender
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


#ifndef PMPATTERNEDIT_H
#define PMPATTERNEDIT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "pmdialogeditbase.h"

class PMPattern;
class PMVectorEdit;
class TQComboBox;
class PMFloatEdit;
class PMIntEdit;
class TQLabel;
class TQCheckBox;
class TQWidget;
class TQLineEdit;
class TQPushButton;

/**
 * Dialog edit class for @ref PMPattern.
 */
class PMPatternEdit : public PMDialogEditBase
{
   TQ_OBJECT
  
   typedef PMDialogEditBase Base;
public:
   /**
    * Creates a PMPatternEdit with parent and name
    */
   PMPatternEdit( TQWidget* parent, const char* name = 0 );

   /** */
   virtual void displayObject( PMObject* o );

   /** */
   virtual bool isDataValid( );
protected:
   /** */
   virtual void createTopWidgets( );
   /** */
   virtual void saveContents( );

protected slots:
   /**
    * Slot called whenever a new pattern type is selected.
    */
   void slotComboChanged( int c );
   /**
    * Slot called when the browse button is pressed for selection of the
    * density file
    */
   void slotDensityFileBrowseClicked( );
   /**
    * Slot called when fractal magnet clicked
    */
   void slotFractalMagnetClicked( );
   /**
    * Slot called when the slope altitude flag is clicked
    */
   void slotSlopeAltFlagClicked( );
   /**
    * Slot called when turbulence is activated/deactivated.
    */
   void slotTurbulenceClicked( );

private:
   /**
    * Set's the combo box and enables/disables widgets.
    */
   void setPatternType( int i );
   /**
    * m_noDepth is false is the editor must show the depth field
    */
   bool m_noDepth;
   PMPattern*     m_pDisplayedObject;
   TQComboBox*     m_pTypeCombo;

   TQLabel*        m_pAgateTurbulenceLabel;
   PMFloatEdit*   m_pAgateTurbulenceEdit;

   TQWidget*       m_pCrackleWidget;
   PMVectorEdit*  m_pCrackleForm;
   PMIntEdit*     m_pCrackleMetric;
   PMFloatEdit*   m_pCrackleOffset;
   TQCheckBox*     m_pCrackleSolid;

   TQWidget*       m_pDensityWidget;
   TQComboBox*     m_pDensityInterpolate;
   TQLineEdit*     m_pDensityFile;
   TQPushButton*   m_pDensityFileBrowse;

   TQLabel*        m_pGradientLabel;
   PMVectorEdit*  m_pGradientEdit;

   TQLabel*        m_pJuliaComplexLabel;
   PMVectorEdit*  m_pJuliaComplex;
   TQWidget*       m_pFractalWidget;
   TQCheckBox*     m_pFractalMagnet;
   TQComboBox*     m_pFractalMagnetType;
   PMIntEdit*     m_pMaxIterationsEdit;
   TQLabel*        m_pFractalExponentLabel;
   PMIntEdit*     m_pFractalExponent;
   TQComboBox*     m_pFractalExtType;
   PMFloatEdit*   m_pFractalExtFactor;
   TQComboBox*     m_pFractalIntType;
   PMFloatEdit*   m_pFractalIntFactor;

   TQLabel*        m_pQuiltControlsLabel;
   PMFloatEdit*   m_pQuiltControl0Edit;
   PMFloatEdit*   m_pQuiltControl1Edit;

   TQWidget*       m_pSlopeWidget;
   PMVectorEdit*  m_pSlopeDirection;
   PMFloatEdit*   m_pSlopeLoSlope;
   PMFloatEdit*   m_pSlopeHiSlope;
   TQCheckBox*     m_pSlopeAltFlag;
   PMVectorEdit*  m_pSlopeAltitude;
   TQLabel*        m_pSlopeLoAltLabel;
   PMFloatEdit*   m_pSlopeLoAlt;
   TQLabel*        m_pSlopeHiAltLabel;
   PMFloatEdit*   m_pSlopeHiAlt;

   TQLabel*        m_pSpiralNumberLabel;
   PMIntEdit*     m_pSpiralNumberEdit;

   TQLabel*        m_pNoiseGeneratorLabel;
   TQComboBox*     m_pNoiseGenerator;

   TQCheckBox*     m_pEnableTurbulenceEdit;
   TQWidget*       m_pTurbulenceWidget;
   PMVectorEdit*  m_pValueVectorEdit;
   PMIntEdit*     m_pOctavesEdit;
   PMFloatEdit*   m_pOmegaEdit;
   PMFloatEdit*   m_pLambdaEdit;

   TQLabel*        m_pDepthLabel;
   PMFloatEdit*   m_pDepthEdit;
};


#endif
