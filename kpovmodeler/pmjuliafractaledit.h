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


#ifndef PMJULIAFRACTALEDIT_H
#define PMJULIAFRACTALEDIT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "pmsolidobjectedit.h"

class PMJuliaFractal;
class PMVectorEdit;
class PMIntEdit;
class PMFloatEdit;
class TQComboBox;
class TQLabel;

/**
 * Dialog edit class for @ref PMJuliaFractal
 */
class PMJuliaFractalEdit : public PMSolidObjectEdit
{
   Q_OBJECT
  TQ_OBJECT
   typedef PMSolidObjectEdit Base;
public:
   /**
    * Creates a PMJuliaFractalEdit with parent and name
    */
   PMJuliaFractalEdit( TQWidget* parent, const char* name = 0 );

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
   void slotAlgebraTypeSelected( int );
   void slotFunctionTypeSelected( int );

private:
   PMJuliaFractal* m_pDisplayedObject;
   PMVectorEdit* m_pJuliaParameter;
   TQComboBox* m_pAlgebraType;
   TQComboBox* m_pFunctionType;
   PMVectorEdit* m_pExponents;
   TQLabel* m_pExponentsLabel;
   PMIntEdit* m_pMaxIterations;
   PMFloatEdit* m_pPrecision;
   PMVectorEdit* m_pSliceNormal;
   PMFloatEdit* m_pSliceDistance;
};


#endif
