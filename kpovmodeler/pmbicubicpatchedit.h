/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001-2002 by Andreas Zehender
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


#ifndef PMBICUBICPATCHEDIT_H
#define PMBICUBICPATCHEDIT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "pmgraphicalobjectedit.h"

class PMBicubicPatch;
class PMIntEdit;
class PMFloatEdit;
class PMVectorListEdit;
class TQComboBox;
class TQCheckBox;

/**
 * Dialog edit class for @ref PMBicubicPatch
 */
class PMBicubicPatchEdit : public PMGraphicalObjectEdit
{
   TQ_OBJECT
  
   typedef PMGraphicalObjectEdit Base;
public:
   /**
    * Creates a PMBicubicPatchEdit with parent and name
    */
   PMBicubicPatchEdit( TQWidget* parent, const char* name = 0 );

   /** */
   virtual void displayObject( PMObject* o );
   /** */
   virtual void updateControlPointSelection( );

   /** */
   virtual bool isDataValid( );

protected slots:
   void slotTypeSelected( int i );
   void slotSelectionChanged( );
   void slotUVEnabledClicked( );
protected:
   /** */
   virtual void createTopWidgets( );
   /** */
   virtual void saveContents( );

private:
   TQComboBox* m_pType;
   PMIntEdit* m_pUSteps;
   PMIntEdit* m_pVSteps;
   PMFloatEdit* m_pFlatness;
   PMVectorListEdit* m_pPoints;
   TQCheckBox* m_pUVEnabled;
   PMVectorListEdit* m_pUVVectors;

   PMBicubicPatch* m_pDisplayedObject;
};


#endif
