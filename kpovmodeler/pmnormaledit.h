//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Luis Carvalho
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


#ifndef PMNORMALEDIT_H
#define PMNORMALEDIT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "pmtexturebaseedit.h"

class PMNormal;
class TQCheckBox;
class PMFloatEdit;

/**
 * Dialog edit class for @ref PMNormal
 */
class PMNormalEdit : public PMTextureBaseEdit
{
   Q_OBJECT
  TQ_OBJECT
   typedef PMTextureBaseEdit Base;
public:
   /**
    * Creates a PMNormalEdit with parent and name
    */
   PMNormalEdit( TQWidget* parent, const char* name = 0 );

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
   void slotBumpSizeClicked( );

private:
   PMNormal* m_pDisplayedObject;

   TQCheckBox* m_pBumpSizeCheck;
   PMFloatEdit* m_pBumpSizeEdit;
   PMFloatEdit* m_pAccuracy;
   TQCheckBox* m_pUVMapping;
};


#endif
