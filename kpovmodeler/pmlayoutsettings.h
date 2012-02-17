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

#ifndef PMLAYOUTSETTINGS_H
#define PMLAYOUTSETTINGS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "pmsettingsdialog.h"
#include "pmviewlayoutmanager.h"

#include <tqvaluelist.h>

class PMIntEdit;
class PMViewOptionsWidget;
class TQComboBox;
class TQListBox;
class TQPushButton;
class TQLabel;

/**
 * View layout configuration dialog page
 */
class PMLayoutSettings : public PMSettingsDialogPage
{
   Q_OBJECT
  
public:
   /**
    * Default constructor
    */
   PMLayoutSettings( TQWidget* parent, const char* name = 0 );
   /** */
   virtual void displaySettings( );
   /** */
   virtual bool validateData( );
   /** */
   virtual void applySettings( );
   /** */
   virtual void displayDefaults( );
   
protected slots:
   /**
    * Called when the browse add layout button is clicked
    */
   void slotAddLayout( );
   /**
    * Called when the remove layout button is clicked
    */
   void slotRemoveLayout( );
   /**
    * Called when the selected layout changes
    */
   void slotLayoutSelected( int i );
   /**
    * Called when the layout name changes
    */
   void slotLayoutNameChanged( const TQString& text );
   /**
    * Called when the selected view entry changes
    */
   void slotViewEntrySelected( TQListViewItem* text );
   /**
    * Called when the view type field changes value
    */
   void slotViewTypeChanged( int index );
   /**
    * Called when the gl view type field changes value
    */
   //void slotGLViewTypeChanged( int index );
   /**
    * Called when the dock position field changes value
    */
   void slotDockPositionChanged( int index );
   /**
    * Called when the view height field changes value
    */
   void slotViewHeightChanged( const TQString& text );
   /**
    * Called when the column width field changes value
    */
   void slotColumnWidthChanged( const TQString& text );
   /**
    * Called when the floating height changes value
    */
   void slotFloatingHeightChanged( const TQString& text );
   /**
    * Called when the floating width changes value
    */
   void slotFloatingWidthChanged( const TQString& text );
   /**
    * Called when the floating position x changes value
    */
   void slotFloatingPosXChanged( const TQString& text );
   /**
    * Called when the floating position y changes value
    */
   void slotFloatingPosYChanged( const TQString& text );
   /**
    * Called when the add view entry button is clicked
    */
   void slotAddViewEntryClicked( );
   /**
    * Called when the remove view entry button is clicked
    */
   void slotRemoveViewEntryClicked( );
   /**
    * Called when the move up view entry button is clicked
    */
   void slotMoveUpViewEntryClicked( );
   /**
    * Called when the move down view entry button is clicked
    */
   void slotMoveDownViewEntryClicked( );
   /**
    * Called when the view type description has changed
    */
   void slotViewTypeDescriptionChanged( );

private:
   void displayLayoutList( );
   void displayCustomOptions( );
   
   TQComboBox* m_pDefaultLayout;
   TQListBox* m_pViewLayouts;
   TQPushButton* m_pAddLayout;
   TQPushButton* m_pRemoveLayout;
   TQLineEdit* m_pViewLayoutName;
   TQListView* m_pViewEntries;
   TQPushButton* m_pAddEntry;
   TQPushButton* m_pRemoveEntry;
   TQPushButton* m_pMoveUpEntry;
   TQPushButton* m_pMoveDownEntry;
   TQComboBox* m_pViewTypeEdit;
   TQComboBox* m_pDockPositionEdit;
   PMIntEdit* m_pColumnWidthEdit;
   TQLabel* m_pColumnWidthLabel;
   PMIntEdit* m_pViewHeightEdit;
   TQLabel* m_pViewHeightLabel;
   
   PMIntEdit* m_pFloatingHeight;
   PMIntEdit* m_pFloatingWidth;
   TQLabel* m_pFloatingHeightLabel;
   TQLabel* m_pFloatingWidthLabel;
   PMIntEdit* m_pFloatingPosX;
   PMIntEdit* m_pFloatingPosY;
   TQLabel* m_pFloatingPosXLabel;
   TQLabel* m_pFloatingPosYLabel;

   PMViewOptionsWidget* m_pCustomOptionsWidget;
   TQWidget* m_pCustomOptionsHolder;
   
   TQValueList<PMViewLayout> m_viewLayouts;
   TQValueListIterator<PMViewLayout> m_currentViewLayout;
   TQValueListIterator<PMViewLayout> m_defaultViewLayout;
   TQValueListIterator<PMViewLayoutEntry> m_currentViewEntry;
};

#endif
