//-*-C++-*-
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

#ifndef PMDIALOGEDITVIEW_H
#define PMDIALOGEDITVIEW_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tqwidget.h>
#include <tqscrollview.h>
#include <kpushbutton.h>

#include "pmviewbase.h"
#include "pmviewfactory.h"

class PMDialogEditBase;
class PMObject;
class PMPart;
class TQGroupBox;
class TQBoxLayout;
class TQLabel;

/**
 * Helper class for @ref PMDialogView
 */
class PMDialogEditContent : public TQScrollView
{
   Q_OBJECT
  TQ_OBJECT
public:
   PMDialogEditContent( TQWidget* tqparent, const char* name = 0 );
   void setContents( TQWidget* wid );
   void calculateSize( );
protected:
   void resizeEvent( TQResizeEvent* );
private:
   TQWidget* m_pContents;
};

/**
 * View to display and modify attributes of objects.
 *
 * This class creates the corresponding widget
 * (subclass of @ref PMDialogEditBase) for the active object and
 * displays it.
 */
class PMDialogView : public PMViewBase
{
   Q_OBJECT
  TQ_OBJECT
public:
   /**
    * Creates a new PMDialogView widget
    */
   PMDialogView( PMPart* part, TQWidget* tqparent, const char* name = 0 );
   /**
    * Deletes the widget
    */
   ~PMDialogView( );

   /** */
   virtual TQString viewType( ) const { return TQString( "dialogview" ); }
   /** */
   virtual TQString description( ) const;

public slots:
   /**
    * Called when an object is changed.
    * Mode is a bit combination of @ref PMChange constants
    */
   void slotObjectChanged( PMObject* obj, const int mode, TQObject* sender );
   /**
    * Called before the scene is rendered
    */
   void slotAboutToRender( );
   /**
    * Called when the whole documents contents have changed
    */
   void slotRefresh( );
   /**
    * Clears all data
    */
   void slotClear( );
signals:
   /**
    * Signal that is emitted when an object is changed.
    * Mode is a bit combination of @ref PMChange constants.
    */
   void objectChanged( PMObject* obj, const int mode, TQObject* sender );
   /**
    * Emitted in the destructor
    */
   void destroyed( PMDialogView* v );

protected:
   virtual void keyPressEvent( TQKeyEvent* );

private slots:
   void slotApply( );
   void slotHelp( );
   void slotCancel( );
   void slotDataChanged( );
   void slotSizeChanged( );
   void slotControlPointSelectionChanged( );

private:
   /**
    * Asks the user to save unsaved data in the displayed widget
    */
   bool shouldSaveData( );
   /**
    * Displays the object
    */
   void displayObject( PMObject* obj, bool updateDescription = true );

   PMDialogEditBase* m_pDisplayedWidget;
   TQBoxLayout* m_pLayout;

   KPushButton* m_pApplyButton;
   KPushButton* m_pCancelButton;
   KPushButton* m_pHelpButton;

   TQLabel* m_pPixmapLabel;
   TQLabel* m_pObjectTypeLabel;

   PMDialogEditContent* m_pHelper;

   bool m_unsavedData;
   PMPart* m_pPart;
};

/**
 * Factory class for dialog views
 */
class PMDialogViewFactory : public PMViewTypeFactory
{
public:
   PMDialogViewFactory( ) { }
   virtual TQString viewType( ) const { return TQString( "dialogview" ); }
   virtual TQString description( ) const;
   virtual TQString iconName( ) const { return TQString( "pmdialogview" ); }
   virtual PMViewBase* newInstance( TQWidget* tqparent, PMPart* part ) const
   {
      return new PMDialogView( part, tqparent );
   }
};

#endif
