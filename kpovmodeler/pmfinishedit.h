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


#ifndef PMFINISHEDIT_H
#define PMFINISHEDIT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "pmtexturebaseedit.h"

class PMFinish;
class PMFloatEdit;
class PMColorEdit;
class TQCheckBox;
class TQLabel;
class TQWidget;

/**
 * Dialog edit class for @ref PMFinish
 */
class PMFinishEdit : public PMTextureBaseEdit
{
   Q_OBJECT
  
   typedef PMTextureBaseEdit Base;
public:
   /**
    * Creates a PMFinishEdit with parent and name
    */
   PMFinishEdit( TQWidget* parent, const char* name = 0 );

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
   void slotIridClicked( );
   void slotAmbientClicked( );
   void slotPhongClicked( );
   void slotPhongSizeClicked( );
   void slotBrillianceClicked( );
   void slotDiffuseClicked( );
   void slotMetallicClicked( );
   void slotCrandClicked( );
   void slotSpecularClicked( );
   void slotRoughnessClicked( );
   void slotReflectionClicked( );
   void slotReflectionMinClicked( );
   void slotRefFalloffClicked( );
   void slotRefExponentClicked( );
   void slotRefMetallicClicked( );

private:
   PMFinish* m_pDisplayedObject;
   PMColorEdit* m_pAmbientColorEdit;
   TQLabel* m_pAmbientColorLabel;
   PMFloatEdit* m_pDiffuseEdit;
   PMFloatEdit* m_pBrillianceEdit;
   PMFloatEdit* m_pCrandEdit;
   TQCheckBox* m_pConserveEnergyEdit;
   PMFloatEdit* m_pPhongEdit;
   PMFloatEdit* m_pPhongSizeEdit;
   PMFloatEdit* m_pMetallicEdit;
   PMFloatEdit* m_pSpecularEdit;
   PMFloatEdit* m_pRoughnessEdit;
   TQCheckBox* m_pIridEdit;
   PMFloatEdit* m_pIridAmountEdit;
   PMFloatEdit* m_pIridThicknessEdit;
   PMFloatEdit* m_pIridTurbulenceEdit;
   TQLabel* m_pIridAmountLabel;
   TQLabel* m_pIridThicknessLabel;
   TQLabel* m_pIridTurbulenceLabel;
   TQCheckBox* m_pEnableAmbientEdit;
   TQCheckBox* m_pEnablePhongEdit;
   TQCheckBox* m_pEnablePhongSizeEdit;
   TQCheckBox* m_pEnableDiffuseEdit;
   TQCheckBox* m_pEnableBrillianceEdit;
   TQCheckBox* m_pEnableCrandEdit;
   TQCheckBox* m_pEnableSpecularEdit;
   TQCheckBox* m_pEnableRoughnessEdit;
   TQCheckBox* m_pEnableMetallicEdit;

   TQWidget* m_pReflectionWidget;
   PMColorEdit* m_pReflectionColorEdit;
   PMColorEdit* m_pReflectionMinColorEdit;
   TQCheckBox* m_pReflectionFresnelEdit;
   PMFloatEdit* m_pReflectionFalloffEdit;
   PMFloatEdit* m_pReflectionExponentEdit;
   PMFloatEdit* m_pReflectionMetallicEdit;
   TQCheckBox* m_pEnableReflectionEdit;
   TQCheckBox* m_pEnableReflectionMinEdit;
   TQCheckBox* m_pEnableRefFalloffEdit;
   TQCheckBox* m_pEnableRefExponentEdit;
   TQCheckBox* m_pEnableRefMetallicEdit;
};


#endif
