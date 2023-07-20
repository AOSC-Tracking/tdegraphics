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


#ifndef PMPOLYNOMEDIT_H
#define PMPOLYNOMEDIT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "pmsolidobjectedit.h"
#include <tqptrlist.h>

class PMPolynom;
class PMFloatEdit;
class PMFormulaLabel;
class TQSpinBox;
class TQCheckBox;

/**
 * Dialog edit class for @ref PMPolynom
 */
class PMPolynomEdit : public PMSolidObjectEdit
{
   TQ_OBJECT
  
   typedef PMSolidObjectEdit Base;
public:
   /**
    * Creates a PMPolynomEdit with parent and name
    */
   PMPolynomEdit( TQWidget* parent, const char* name = 0 );

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
   void slotOrderChanged( int );

private:
   void displayCoefficients( const PMVector& co, int cOrder, int dOrder );
   PMVector coefficients( ) const;
   
   PMPolynom* m_pDisplayedObject;
   
   int m_currentOrder;
   
   TQWidget* m_pPolyWidget;
   TQPtrList<TQWidget> m_labels;
   TQPtrList<PMFloatEdit> m_edits;
   TQCheckBox* m_pSturm;
   TQSpinBox* m_pOrder;
   bool m_readOnly;
};


#endif
