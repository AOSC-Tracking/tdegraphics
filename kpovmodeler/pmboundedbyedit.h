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


#ifndef PMBOUNDEDBYEDIT_H
#define PMBOUNDEDBYEDIT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "pmdialogeditbase.h"

class PMBoundedBy;
class TQLabel;

/**
 * Dialog edit class for @ref PMBoundedBy
 */
class PMBoundedByEdit : public PMDialogEditBase
{
   TQ_OBJECT
  
   typedef PMDialogEditBase Base;
public:
   /**
    * Creates a PMBoundedByEdit with parent and name
    */
   PMBoundedByEdit( TQWidget* parent, const char* name = 0 );

   /** */
   virtual void displayObject( PMObject* o );

protected:
   /** */
   virtual void createTopWidgets( );
   
private:
   PMBoundedBy* m_pDisplayedObject;
   TQLabel* m_pChildLabel;
   TQLabel* m_pClippedByLabel;
};


#endif
