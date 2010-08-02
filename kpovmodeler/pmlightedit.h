//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Andreas Zehender
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


#ifndef PMLIGHTEDIT_H
#define PMLIGHTEDIT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "pmnamedobjectedit.h"

class PMLight;
class PMVectorEdit;
class PMColorEdit;
class TQComboBox;
class PMFloatEdit;
class PMIntEdit;
class TQLabel;
class TQCheckBox;

/**
 * Dialog edit class for @ref PMLight
 */
class PMLightEdit : public PMNamedObjectEdit
{
   Q_OBJECT
   typedef PMNamedObjectEdit Base;
public:
   /**
    * Creates a PMLightEdit with parent and name
    */
   PMLightEdit( TQWidget* parent, const char* name = 0 );

   /** */
   virtual void displayObject( PMObject* o );

   /** */
   virtual bool isDataValid( );
public slots:
   void slotTypeActivated( int index );
   void slotAreaClicked( );
   void slotOrientCheck( );
   void slotFadingClicked( );
protected:
   /** */
   virtual void createTopWidgets( );
   /** */
   virtual void saveContents( );

private:
   /**
    * Returns true if orient should be enabled
    */
   bool orientEnabled( bool readOnly );

   PMLight* m_pDisplayedObject;
   PMVectorEdit* m_pLocation;
   PMColorEdit* m_pColor;
   TQComboBox* m_pType;
   
   PMFloatEdit* m_pRadius;
   TQLabel* m_pRadiusLabel;
   PMFloatEdit* m_pFalloff;
   TQLabel* m_pFalloffLabel;
   PMFloatEdit* m_pTightness;
   TQLabel* m_pTightnessLabel;
   
   PMVectorEdit* m_pPointAt;
   TQLabel* m_pPointAtLabel;
   TQCheckBox* m_pParallel;

   TQCheckBox* m_pAreaLight;
   TQLabel* m_pAreaTypeLabel;
   TQComboBox* m_pAreaType;
   PMVectorEdit* m_pAxis1;
   PMIntEdit* m_pSize1;
   PMVectorEdit* m_pAxis2;
   PMIntEdit* m_pSize2;
   TQLabel* m_pAxis1Label;
   TQLabel* m_pAxis2Label;
   TQLabel* m_pSize1Label;
   TQLabel* m_pSize2Label;
   PMIntEdit* m_pAdaptive;
   TQLabel* m_pAdaptiveLabel;
   TQCheckBox* m_pOrient;
   TQCheckBox* m_pJitter;

   TQCheckBox* m_pFading;
   PMFloatEdit* m_pFadeDistance;
   TQLabel* m_pFadeDistanceLabel;
   PMIntEdit* m_pFadePower;
   TQLabel* m_pFadePowerLabel;
   TQCheckBox* m_pMediaInteraction;
   TQCheckBox* m_pMediaAttenuation;
};


#endif
