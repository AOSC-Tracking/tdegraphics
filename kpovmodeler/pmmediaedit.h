/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001 by Luis Carvalho
    email                : lpassos@mail.telepac.pt
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#ifndef PMMEDIAEDIT_H
#define PMMEDIAEDIT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "pmtexturebaseedit.h"

class PMMedia;
class PMIntEdit;
class PMFloatEdit;
class PMColorEdit;
class TQCheckBox;
class TQLabel;
class TQComboBox;

/**
 * Dialog edit class for @ref PMMedia
 */
class PMMediaEdit : public PMTextureBaseEdit
{
   TQ_OBJECT
  
   typedef PMTextureBaseEdit Base;
public:
   /**
    * Creates a PMMediaEdit with parent and name
    */
   PMMediaEdit( TQWidget* parent, const char* name = 0 );

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
   void slotMethodChanged( int c );
   void slotAbsorptionClicked( );
   void slotEmissionClicked( );
   void slotScatteringClicked( );
   void slotScatteringTypeChanged( int c );

private:
   PMMedia* m_pDisplayedObject;

   TQComboBox* m_pMethodEdit;
   PMIntEdit* m_pIntervalsEdit;
   PMIntEdit* m_pSamplesMinEdit;
   TQLabel* m_pSamplesMaxLabel;
   PMIntEdit* m_pSamplesMaxEdit;
   PMFloatEdit* m_pConfidenceEdit;
   PMFloatEdit* m_pVarianceEdit;
   PMFloatEdit* m_pRatioEdit;

   TQWidget* m_pAAWidget;
   PMIntEdit* m_pAALevelEdit;
   PMFloatEdit* m_pAAThresholdEdit;

   TQCheckBox* m_pEnableAbsorptionEdit;
   PMColorEdit* m_pAbsorptionEdit;
   TQLabel* m_pAbsorptionLabel;

   TQCheckBox* m_pEnableEmissionEdit;
   PMColorEdit* m_pEmissionEdit;
   TQLabel* m_pEmissionLabel;

   TQCheckBox* m_pEnableScatteringEdit;
   TQWidget* m_pScatteringWidget;
   TQComboBox* m_pScatteringTypeEdit;
   PMColorEdit* m_pScatteringColorEdit;
   TQLabel* m_pScatteringEccentricityLabel;
   PMFloatEdit* m_pScatteringEccentricityEdit;
   PMFloatEdit* m_pScatteringExtinctionEdit;
};


#endif
