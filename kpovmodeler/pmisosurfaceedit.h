//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Andreas Zehender
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


#ifndef PMISOSURFACEEDIT_H
#define PMISOSURFACEEDIT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "pmsolidobjectedit.h"

class PMIsoSurface;
class PMVectorEdit;
class PMFloatEdit;
class PMIntEdit;
class TQCheckBox;
class TQComboBox;
class TQLabel;
class TQLineEdit;

/**
 * Dialog edit class for @ref PMIsoSurface
 */
class PMIsoSurfaceEdit : public PMSolidObjectEdit
{
   Q_OBJECT
   typedef PMSolidObjectEdit Base;
public:
   /**
    * Creates a PMIsoSurfaceEdit with parent and name
    */
   PMIsoSurfaceEdit( TQWidget* parent, const char* name = 0 );

   /** */
   virtual void displayObject( PMObject* o );

   /** */
   virtual bool isDataValid( );
protected:
   /** */
   virtual void createTopWidgets( );
   /** */
   virtual void saveContents( );

private slots:
   void textChanged( const TQString& );
   void currentChanged( int i );
   void evaluateToggled( bool );
   void allToggled( bool );
   void toggled( bool );

private:
   PMIsoSurface* m_pDisplayedObject;
   TQLineEdit* m_pFunction;
   TQComboBox* m_pContainedBy;
   PMVectorEdit* m_pCorner1;
   PMVectorEdit* m_pCorner2;
   PMVectorEdit* m_pCenter;
   PMFloatEdit* m_pRadius;
   TQLabel* m_pCorner1Label;
   TQLabel* m_pCorner2Label;
   TQLabel* m_pCenterLabel;
   TQLabel* m_pRadiusLabel;
   PMFloatEdit* m_pThreshold;
   PMFloatEdit* m_pAccuracy;
   PMFloatEdit* m_pMaxGradient;
   TQCheckBox* m_pEvaluate;
   PMFloatEdit* m_pEvaluateValue[3];
   TQCheckBox* m_pOpen;
   PMIntEdit* m_pMaxTrace;
   TQCheckBox* m_pAllIntersections;
};


#endif
