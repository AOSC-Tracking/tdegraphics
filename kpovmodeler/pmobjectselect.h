//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2002 by Andreas Zehender
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

#ifndef PMOBJECTSELECT_H
#define PMOBJECTSELECT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <kdialogbase.h>
#include <tqlistbox.h>
#include <tqsize.h>
#include <tqvaluelist.h>
#include <tqstringlist.h>
#include "pmobject.h"
#include "pmdeclare.h"

/**
 * TQListBoxItem for @ref PMObject
 */
class PMListBoxObject : public TQListBoxPixmap
{
public:
   /**
    * Constructs a list box item in listbox listbox showing the
    * object obj. The item gets inserted after the item after
    */
   PMListBoxObject( TQListBox* listbox, PMObject* obj, TQListBoxItem* after );
   /**
    * Constructs a list box item in listbox listbox showing the object obj
    */
   PMListBoxObject( TQListBox* listbox, PMObject* obj );
   /**
    * Constructs a list box item showing the object obj
    */
   PMListBoxObject( PMObject* obj );
   /**
    * Constructs a list box item in listbox listbox showing the
    * text text. The item gets inserted after the item after
    */
   PMListBoxObject( TQListBox* listbox, PMObject* obj, const TQString& text,
                    TQListBoxItem* after );
   /**
    * Constructs a list box item in listbox listbox showing the text text
    */
   PMListBoxObject( TQListBox* listbox, PMObject* obj, const TQString& text );
   /**
    * Constructs a list box item showing the text text
    */
   PMListBoxObject( PMObject* obj, const TQString& text );
   /**
    * Deletes the item
    */
   ~PMListBoxObject( );

   /**
    * Returns a pointer to the object
    */
   PMObject* object( ) const { return m_pObject; }
private:
   static TQString checkName( const TQString& text );
   PMObject* m_pObject;
};

/**
 * A PMObject selection widget
 */
class PMObjectSelect : public KDialogBase
{
   Q_OBJECT
  
public:
   /**
    * Creates a selection widget with parent and name
    */
   PMObjectSelect( TQWidget* parent = 0, const char* name = 0,
                   bool modal = true );
   /**
    * Deletes the dialog
    */
   ~PMObjectSelect( );

   /**
    * Appends the object to the list of choosable objects
    */
   void addObject( PMObject* obj );
   
   /**
    * Returns the currently selected object
    */
   PMObject* selectedObject( ) const { return m_pSelectedObject; }
   
   /**
    * Create a modal dialog and let the user choose an declare object
    * of type t.
    *
    * Only objects above the object link  are listed.
    */
   static int selectObject( PMObject* link, const TQString& t, PMObject* & obj,
                            TQWidget* parent = 0 );
   static int selectObject( PMObject* link, const TQStringList& t,
                            PMObject* & obj, TQWidget* parent = 0 );
   static int selectDeclare( PMObject* link, const TQString& declareType,
                             PMObject* & obj, TQWidget* parent = 0 );
   static int selectDeclare( PMObject* link, const TQStringList& dt,
                             PMObject* & obj, TQWidget* parent = 0 );
   
protected slots:
   void slotHighlighted( TQListBoxItem* lbi );
   void slotSelected( TQListBoxItem* lbi );
private:
   TQListBox* m_pListBox;
   PMObject* m_pSelectedObject;
   static TQSize s_size;
};

#endif
