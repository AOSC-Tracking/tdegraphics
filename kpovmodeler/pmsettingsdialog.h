/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2003 by Andreas Zehender
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

#ifndef PMSETTINGSDIALOG_H
#define PMSETTINGSDIALOG_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <kdialogbase.h>
#include <tqvaluelist.h>
#include <tqvaluevector.h>

class TQFrame;
class TQCheckBox;
class TQLineEdit;
class TQListBox;
class TQListView;
class TQButtonGroup;
class TQComboBox;
class KColorButton;
class TDEConfig;
class PMIntEdit;
class PMFloatEdit;
class PMPart;
class PMViewLayout;
class PMViewLayoutEntry;

/**
 * Base class for configuration dialog pages.
 *
 * All base classes have to implement the pure virtual
 * methods @ref displaySettings, @ref displayDefaults, @ref validateData
 * and @ref applySettings
 */
class PMSettingsDialogPage : public TQWidget
{
   TQ_OBJECT
  
public:
   /**
    * Constructor
    */
   PMSettingsDialogPage( TQWidget* parent, const char* name = 0 );
   /**
    * Display the settings here.
    *
    * Base classes have to implement this method.
    */
   virtual void displaySettings( ) = 0;
   /**
    * Validate the changed data here and return true
    * if the data is valid. Display an error message
    * and return false otherwise.
    *
    * Base classes have to implement this method.
    */
   virtual bool validateData( ) = 0;
   /**
    * Make the changes permanent here.
    *
    * Base classes have to implement this method.
    */
   virtual void applySettings( ) = 0;
   /**
    * Display the default values.
    *
    * Base classes have to implement this method.
    */
   virtual void displayDefaults( ) = 0;
signals:
   /**
    * Emit this signal if a parameter was changed
    * that influences the wire frame rendering.
    */
   void repaintViews( );
   /**
    * Tells the settings dialog to show this page.
    */
   void showMe( );
};


/**
 * Helper class, used internally by @ref PMSettingsDialog
 */
class PMRegisteredSettingsPage
{
public:
   PMRegisteredSettingsPage( )
   {
      topPage = 0;
      page = 0;
      index = 0;
   }
   PMRegisteredSettingsPage( TQWidget* top, PMSettingsDialogPage* p,
                             int i )
   {
      topPage = top;
      page = p;
      index = i;
   }
   TQWidget* topPage;
   PMSettingsDialogPage* page;
   int index;
};

/**
 * Configuration dialog
 */
class PMSettingsDialog : public KDialogBase
{
   TQ_OBJECT
  
public:
   /**
    * Standard constructor
    */
   PMSettingsDialog( PMPart* part, TQWidget* parent = 0, const char* name = 0 );
   /**
    * Registers a new settings page.
    *
    * @param topPage The page created with addVBoxPage
    * @param page The internal settings page
    */
   void registerPage( TQWidget* topPage, PMSettingsDialogPage* page );

   static void saveConfig( TDEConfig* cfg );
   static void restoreConfig( TDEConfig* cfg );

protected:
   virtual void resizeEvent( TQResizeEvent* ev );

protected slots:
   /**
    * Validates the data and makes the changes permanent.
    */
   virtual void slotApply( );
   /**
    * Validates the data, makes the changes permanent and closes the dialog.
    */
   virtual void slotOk( );
   /**
    * Displays the default values.
    */
   virtual void slotDefault( );
   /**
    * Closes the dialog without saving the data.
    */
   virtual void slotCancel( );

   /**
    * Repaints the opengl views
    */
   void slotRepaint( );
   /**
    * Shows the sender page
    */
   void slotShowPage( );
   
private:
   void displaySettings( );
   bool validateData( );
   void saveSettings( );
   int findPage( const PMSettingsDialogPage* page );
   bool m_repaint;
   TQValueList<PMRegisteredSettingsPage> m_pages;
   PMPart* m_pPart;
   
   static TQSize s_size;
};

#endif
