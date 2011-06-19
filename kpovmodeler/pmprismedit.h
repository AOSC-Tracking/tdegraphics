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


#ifndef PMPRISMEDIT_H
#define PMPRISMEDIT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "pmsolidobjectedit.h"
#include "pmvectoredit.h"
#include <tqptrlist.h>
#include <tqvaluelist.h>


class PMPrism;
class PMFloatEdit;
class PMVectorListEdit;
class TQVBoxLayout;
class TQComboBox;
class TQCheckBox;
class TQPushButton;
class TQLabel;

/**
 * Dialog edit class for @ref PMPrism
 */
class PMPrismEdit : public PMSolidObjectEdit
{
   Q_OBJECT
  TQ_OBJECT
   typedef PMSolidObjectEdit Base;
public:
   /**
    * Creates a PMPrismEdit with tqparent and name
    */
   PMPrismEdit( TQWidget* tqparent, const char* name = 0 );
   /**
    * Destructor
    */
   virtual ~PMPrismEdit( );
   /** */
   virtual void displayObject( PMObject* o );
   /** */
   void updateControlPointSelection( );

   /** */
   virtual bool isDataValid( );
   
protected:
   /** */
   virtual void createTopWidgets( );
   /** */
   virtual void createBottomWidgets( );
   /** */
   virtual void saveContents( );

private:
   /**
    * Displays the spline points
    */
   void displayPoints( const TQValueList< TQValueList<PMVector> >& list );
   /**
    * Returns the spline points from the vector edits
    */
   TQValueList< TQValueList<PMVector> > splinePoints( );
   /**
    * Deletes the spline point edits
    */
   void deleteEdits( );
   /**
    * Creates the edits for the points
    */
   void createEdits( const TQValueList< TQValueList<PMVector> >& points );

protected slots:
   void slotTypeChanged( int );
   void slotSweepChanged( int );
   void slotAddSubPrism( );
   void slotRemoveSubPrism( );
   void slotAddPointAbove( );
   void slotAddPointBelow( );
   void slotRemovePoint( );
   void slotSelectionChanged( );
   
private:   
   PMPrism* m_pDisplayedObject;
   TQPtrList< TQLabel > m_labels;
   TQPtrList< TQPushButton > m_subPrismAddButtons;
   TQPtrList< TQPushButton > m_subPrismRemoveButtons;
   TQPtrList< TQPushButton > m_addAboveButtons;
   TQPtrList< TQPushButton > m_addBelowButtons;
   TQPtrList< TQPushButton > m_removeButtons;
   TQPtrList< PMVectorListEdit> m_points;
   TQWidget* m_pEditWidget;
   TQComboBox* m_pSplineType;
   TQComboBox* m_pSweepType;
   TQCheckBox* m_pSturm;
   TQCheckBox* m_pOpen;
   PMFloatEdit* m_pHeight1;
   PMFloatEdit* m_pHeight2;
   int m_lastSplineType;
};


#endif
