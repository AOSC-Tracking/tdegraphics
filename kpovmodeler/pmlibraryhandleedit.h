//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Luis Carvalho
    email                : lpassos@oninetspeed.pt
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/

#ifndef PMLIBRARYHANDLEEDIT_H
#define PMLIBRARYHANDLEEDIT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <kdialogbase.h>

class TQLineEdit;
class TQTextEdit;
class TQListBox;
class TQCheckBox;
class PMLibraryHandle;

/** 
 * This class provides a dialog to edit the definitions of a library.
 */
class PMLibraryHandleEdit: public KDialogBase
{
   Q_OBJECT
  
public:
   /**
    * Construct a dialog to edit the properties of lib. The library will be
    * modified only if Ok is pressed.
    */
   PMLibraryHandleEdit( PMLibraryHandle* lib, TQWidget *parent = NULL, const char* name = NULL );

   static void saveConfig( TDEConfig* cfg );
   static void restoreConfig( TDEConfig* cfg );
   static TQSize s_size;

private slots:
   void slotOk( );
   void slotEditsChanged( const TQString& );
   void slotDescriptionChanged( );
   void slotReadOnlyChanged( );
   void resizeEvent( TQResizeEvent *ev );

private:
   PMLibraryHandle* m_pLibrary;
   TQLineEdit*       m_pNameEdit;
   TQLineEdit*       m_pAuthorEdit;
   TQTextEdit*       m_pDescriptionEdit;
   TQCheckBox*     m_pReadOnlyEdit;
};

#endif
