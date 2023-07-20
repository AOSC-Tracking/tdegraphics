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


#ifndef PMINTERIOREDIT_H
#define PMINTERIOREDIT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "pmtexturebaseedit.h"

class PMInterior;
class PMIntEdit;
class PMFloatEdit;
class PMColorEdit;
class TQCheckBox;
class TQLabel;

/**
 * Dialog edit class for @ref PMInterior
 */
class PMInteriorEdit : public PMTextureBaseEdit
{
   TQ_OBJECT
  
   typedef PMTextureBaseEdit Base;
public:
   /**
    * Creates a PMInteriorEdit with parent and name
    */
   PMInteriorEdit( TQWidget* parent, const char* name = 0 );

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
   void slotIorClicked( );
   void slotCausticsClicked( );
   void slotDispersionClicked( );
   void slotDispSamplesClicked( );
   void slotFadePowerClicked( );
   void slotFadeDistanceClicked( );

private:
   PMInterior* m_pDisplayedObject;
   PMFloatEdit* m_pIorEdit;
   PMFloatEdit* m_pCausticsEdit;
   PMFloatEdit* m_pDispersionEdit;
   PMIntEdit* m_pDispSamplesEdit;
   PMFloatEdit* m_pFadeDistanceEdit;
   PMFloatEdit* m_pFadePowerEdit;
   TQCheckBox* m_pEnableIorEdit;
   TQCheckBox* m_pEnableCausticsEdit;
   TQCheckBox* m_pEnableDispersionEdit;
   TQCheckBox* m_pEnableDispSamplesEdit;
   TQCheckBox* m_pEnableFadeDistanceEdit;
   TQCheckBox* m_pEnableFadePowerEdit;
};


#endif
