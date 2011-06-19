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


#ifndef PMLATHEEDIT_H
#define PMLATHEEDIT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "pmsolidobjectedit.h"
#include "pmvectoredit.h"
#include <tqptrlist.h>
#include <tqvaluelist.h>

class PMLathe;
class PMVectorListEdit;
class TQVBoxLayout;
class TQComboBox;
class TQCheckBox;
class TQPushButton;
class TQLabel;

/**
 * Dialog edit class for @ref PMLathe
 */
class PMLatheEdit : public PMSolidObjectEdit
{
   Q_OBJECT
  TQ_OBJECT
   typedef PMSolidObjectEdit Base;
public:
   /**
    * Creates a PMLatheEdit with tqparent and name
    */
   PMLatheEdit( TQWidget* tqparent, const char* name = 0 );

   /** */
   virtual void displayObject( PMObject* o );
   /** */
   virtual void updateControlPointSelection( );

   /** */
   virtual bool isDataValid( );
   
protected:
   /** */
   virtual void createTopWidgets( );
   /** */
   virtual void createBottomWidgets( );
   /** */
   virtual void saveContents( );

protected slots:
   void slotTypeChanged( int );
   void slotAddPointAbove( );
   void slotAddPointBelow( );
   void slotRemovePoint( );
   void slotSelectionChanged( );

private:
   void updatePointButtons( );
   
   PMLathe* m_pDisplayedObject;
   TQComboBox* m_pSplineType;
   TQCheckBox* m_pSturm;
   PMVectorListEdit* m_pPoints;
   TQPushButton* m_pAddAbove;
   TQPushButton* m_pAddBelow;
   TQPushButton* m_pRemove;   
};


#endif
