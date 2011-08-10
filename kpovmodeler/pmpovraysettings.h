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

#ifndef PMPOVRAYSETTINGS_H
#define PMPOVRAYSETTINGS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "pmsettingsdialog.h"

class TQLineEdit;
class TQPushButton;
class TQComboBox;
class TQListBox;

/**
 * Povray configuration dialog page
 */
class PMPovraySettings : public PMSettingsDialogPage
{
   Q_OBJECT
  TQ_OBJECT
public:
   /**
    * Default constructor
    */
   PMPovraySettings( TQWidget* parent, const char* name = 0 );
   /** */
   virtual void displaySettings( );
   /** */
   virtual bool validateData( );
   /** */
   virtual void applySettings( );
   /** */
   virtual void displayDefaults( );
   
protected slots:
   void slotAddPath( );
   /**
    * Called when the remove button is clicked
    */
   void slotRemovePath( );
   /**
    * Called when the up button is clicked
    */
   void slotPathUp( );
   /**
    * Called when the down button is clicked
    */
   void slotPathDown( );
   /**
    * Called when the edit button is clicked
    */
   void slotEditPath( );
   /**
    * Called when a path is selected in the list view
    */
   void slotPathSelected( int index );
   /**
    * Called when the browse button for the povray command is clicked
    */
   void slotBrowsePovrayCommand( );
   /**
    * Called when the browse button for the povray documentation is clicked
    */
   void slotBrowsePovrayDocumentation( );
   
private:
   TQLineEdit* m_pPovrayCommand;
   TQPushButton* m_pBrowsePovrayCommand;
   TQLineEdit* m_pDocumentationPath;
   TQPushButton* m_pBrowseDocumentationPath;
   TQComboBox* m_pDocumentationVersion;
   TQListBox* m_pLibraryPaths;
   TQPushButton* m_pAddLibraryPath;
   TQPushButton* m_pRemoveLibraryPath;
   TQPushButton* m_pChangeLibraryPath;
   TQPushButton* m_pLibraryPathUp;
   TQPushButton* m_pLibraryPathDown;
   int m_selectionIndex;
};

#endif
