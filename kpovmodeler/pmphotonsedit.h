//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Leon Pennington
    email                : leon@leonscape.co.uk
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#ifndef PMPHOTONSEDIT_H
#define PMPHOTONSEDIT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "pmdialogeditbase.h"

class PMPhotons;
class PMFloatEdit;
class TQCheckBox;
class TQLabel;

/**
 * Dialog edit class for @ref PMPhotons.
 */
class PMPhotonsEdit : public PMDialogEditBase
{
   Q_OBJECT
   typedef PMDialogEditBase Base;
public:
   /**
    * Creates a PMPhotonsEdit with parent and name
    */
   PMPhotonsEdit( TQWidget* parent, const char* name = 0 );

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
    * Slot Called whenever target is clicked
    */
   void slotTargetClicked( );

private:
   PMPhotons* m_pDisplayedObject;

   TQWidget* m_pLayoutWidget;
   TQCheckBox* m_pTarget;
   PMFloatEdit* m_pSpacingMulti;
   TQLabel* m_pSpacingMultiLabel;
   TQCheckBox* m_pRefraction;
   TQCheckBox* m_pReflection;
   TQCheckBox* m_pCollect;
   TQCheckBox* m_pPassThrough;
   TQCheckBox* m_pAreaLight;
};


#endif
