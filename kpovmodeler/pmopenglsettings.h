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

#ifndef PMOPENGLSETTINGS_H
#define PMOPENGLSETTINGS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "pmsettingsdialog.h"

class TQCheckBox;

/**
 * OpenGL configuration dialog page
 */
class PMOpenGLSettings : public PMSettingsDialogPage
{
   Q_OBJECT
  TQ_OBJECT
public:
   /**
    * Default constructor
    */
   PMOpenGLSettings( TQWidget* tqparent, const char* name = 0 );
   /** */
   virtual void displaySettings( );
   /** */
   virtual bool validateData( );
   /** */
   virtual void applySettings( );
   /** */
   virtual void displayDefaults( );

private:
   TQCheckBox* m_pDirect;
};


#endif
