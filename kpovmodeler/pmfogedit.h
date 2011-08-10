//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Luis Carvalho
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


#ifndef PMFOGEDIT_H
#define PMFOGEDIT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "pmtexturebaseedit.h"

class PMFog;
class PMVectorEdit;
class PMFloatEdit;
class PMIntEdit;
class PMColorEdit;
class TQCheckBox;
class TQComboBox;
class TQWidget;
class TQLabel;

/**
 * Dialog edit class for @ref PMFog
 */
class PMFogEdit : public PMTextureBaseEdit
{
   Q_OBJECT
  TQ_OBJECT
   typedef PMTextureBaseEdit Base;
public:
   /**
    * Creates a PMFogEdit with parent and name
    */
   PMFogEdit( TQWidget* parent, const char* name = 0 );

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
    * Slot called whenever turbulence is activated/deactivated
    */
   void slotTurbulenceClicked( );
   /**
    * Slot called whenever a new fog type is selected
    */
   void slotFogTypeChanged( int val );

private:
   PMFog* m_pDisplayedObject;

   TQComboBox* m_pFogTypeEdit;
   PMFloatEdit* m_pDistance;
   PMColorEdit* m_pColor;
   TQCheckBox* m_pTurbulenceCheck;
   TQWidget* m_pTurbulenceWidget;
   PMVectorEdit* m_pTurbulenceVector;
   PMIntEdit* m_pOctaves;
   PMFloatEdit* m_pOmega;
   PMFloatEdit* m_pLambda;
   PMFloatEdit* m_pDepth;
   TQLabel* m_pFogOffsetLabel;
   PMFloatEdit* m_pFogOffset;
   TQLabel* m_pFogAltLabel;
   PMFloatEdit* m_pFogAlt;
   TQLabel* m_pUpLabel;
   PMVectorEdit* m_pUp;
};


#endif
