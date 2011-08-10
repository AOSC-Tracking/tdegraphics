//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Luis Carvalho
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


#ifndef PMWARPEDIT_H
#define PMWARPEDIT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "pmdialogeditbase.h"

class PMWarp;
class PMVectorEdit;
class PMIntEdit;
class PMFloatEdit;
class TQComboBox;
class TQCheckBox;
class TQLabel;

/**
 * Dialog edit class for @ref PMWarp.
 */
class PMWarpEdit : public PMDialogEditBase
{
   Q_OBJECT
  TQ_OBJECT
   typedef PMDialogEditBase Base;
public:
   /**
    * Creates a PMWarpEdit with parent and name
    */
   PMWarpEdit( TQWidget* parent, const char* name = 0 );

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
   /** */
   void slotComboChanged( int c );

private:
   PMWarp* m_pDisplayedObject;
   TQComboBox*    m_pWarpTypeEdit;
   PMVectorEdit* m_pDirectionEdit;
   TQLabel*       m_pDirectionLabel;
   PMVectorEdit* m_pOffsetEdit;
   TQLabel*       m_pOffsetLabel;
   PMVectorEdit* m_pFlipEdit;
   TQLabel*       m_pFlipLabel;

   PMVectorEdit* m_pLocationEdit;
   TQLabel*       m_pLocationLabel;
   PMFloatEdit*  m_pRadiusEdit;
   TQLabel*       m_pRadiusLabel;
   PMFloatEdit*  m_pStrengthEdit;
   TQLabel*       m_pStrengthLabel;
   PMFloatEdit*  m_pFalloffEdit;
   TQLabel*       m_pFalloffLabel;
   TQCheckBox*    m_pInverseEdit;
   PMVectorEdit* m_pRepeatEdit;
   TQLabel*       m_pRepeatLabel;
   PMVectorEdit* m_pTurbulenceEdit;
   TQLabel*       m_pTurbulenceLabel;

   PMVectorEdit* m_pValueVectorEdit;
   TQLabel*       m_pValueVectorLabel;
   PMIntEdit*    m_pOctavesEdit;
   TQLabel*       m_pOctavesLabel;
   PMFloatEdit*  m_pOmegaEdit;
   TQLabel*       m_pOmegaLabel;
   PMFloatEdit*  m_pLambdaEdit;
   TQLabel*       m_pLambdaLabel;

   PMVectorEdit* m_pOrientationEdit;
   PMFloatEdit*  m_pDistExpEdit;
   PMFloatEdit*  m_pMajorRadiusEdit;
   TQLabel*       m_pMajorRadiusLabel;

   TQWidget* m_pRepeatWidget;
   TQWidget* m_pBlackHoleWidget;
   TQWidget* m_pTurbulenceWidget;
   TQWidget* m_pMappingWidget;
};


#endif
