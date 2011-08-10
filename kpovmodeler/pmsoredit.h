//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Andreas Zehender
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


#ifndef PMSOREDIT_H
#define PMSOREDIT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "pmsolidobjectedit.h"
#include "pmvectoredit.h"
#include <tqptrlist.h>
#include <tqvaluelist.h>

class PMSurfaceOfRevolution;
class TQVBoxLayout;
class TQComboBox;
class TQCheckBox;
class TQPushButton;
class TQLabel;
class PMVectorListEdit;

/**
 * Dialog edit class for @ref PMSurfaceOfRevolution
 */
class PMSurfaceOfRevolutionEdit : public PMSolidObjectEdit
{
   Q_OBJECT
  TQ_OBJECT
   typedef PMSolidObjectEdit Base;
public:
   /**
    * Creates a PMSurfaceOfRevolutionEdit with parent and name
    */
   PMSurfaceOfRevolutionEdit( TQWidget* parent, const char* name = 0 );

   /** */
   virtual void displayObject( PMObject* o );
   /** */
   virtual void updateControlPointSelection( );

   /** */
   virtual bool isDataValid( );
   
protected:
   /** */
   virtual void createBottomWidgets( );
   /** */
   virtual void saveContents( );

protected slots:
   void slotAddPointAbove( );
   void slotAddPointBelow( );
   void slotRemovePoint( );
   void slotSelectionChanged( );
   
private:
   void updatePointButtons( );
   
   PMSurfaceOfRevolution* m_pDisplayedObject;
   PMVectorListEdit* m_pPoints;
   TQCheckBox* m_pOpen;
   TQCheckBox* m_pSturm;
   TQPushButton* m_pAddAbove;
   TQPushButton* m_pAddBelow;
   TQPushButton* m_pRemove;   
};


#endif
