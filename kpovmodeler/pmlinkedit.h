/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001-2002 by Andreas Zehender
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

#ifndef PMLINKEDIT_H
#define PMLINKEDIT_H

#include <tqwidget.h>
#include "pmobject.h"
#include "pmdeclare.h"

#include <tqvaluelist.h>

class TQString;
class TQLineEdit;
class TQPushButton;

/**
 * Edit widget for links with a TQLineEdit, a select and a clear button.
 */
class PMLinkEdit : public QWidget
{
   Q_OBJECT
public:
   /**
    * Creates a link edit widget with parent and name.
    *
    * Allows the selection of declares of type declareType.
    */
   PMLinkEdit( const TQString& declareType, TQWidget* parent, const char* name = 0 );
   /**
    * Creates a link edit widget with parent and name.
    *
    * Allows the selection of declares of type declares.
    */
   PMLinkEdit( const TQStringList& declares, TQWidget* parent, const char* name = 0 );
   /**
    * Creates a link edit widget with parent and name.
    */
   PMLinkEdit( TQWidget* parent, const char* name = 0 );

   /**
    * Sets the displayed object and displays the link
    */
   void setDisplayedObject( PMObject* obj );
   /**
    * Returns the selected link
    */
   PMDeclare* link( ) const { return m_pDeclare; }

   /**
    * Sets the selection possibilities
    */
   void setLinkPossibility( const TQString& t );
   /**
    * Sets the selection possibilities
    */
   void setLinkPossibilities( const TQStringList& t );
   
   /**
    * Enables or disables read only mode
    */
   void setReadOnly( bool yes = true );
signals:
   /**
    * Emitted when the link is changed
    */
   void dataChanged( );

public slots:
   /**
    * Called when the select button is clicked
    */
   void slotSelectClicked( );
   /**
    * Called when the clear button is clicked
    */
   void slotClearClicked( );

private:
   void init( );
   PMDeclare* m_pDeclare;
   PMObject* m_pDisplayedObject;
   TQStringList m_declareTypes;

   TQLineEdit* m_pIDEdit;
   TQPushButton* m_pSelectButton;
   TQPushButton* m_pClearButton;
   bool m_bReadOnly;
};

#endif
