//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Luis Passos Carvalho
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


#ifndef PMMATERIALMAPEDIT_H
#define PMMATERIALMAPEDIT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "pmdialogeditbase.h"

class PMMaterialMap;
class TQComboBox;
class PMFloatEdit;
class PMIntEdit;
class TQLabel;
class TQCheckBox;
class TQWidget;
class TQLineEdit;
class TQPushButton;

/**
 * Dialog edit class for @ref PMMaterialMap.
 */
class PMMaterialMapEdit : public PMDialogEditBase
{
   Q_OBJECT
  
   typedef PMDialogEditBase Base;
public:
   /**
    * Creates a PMMaterialMapEdit with parent and name
    */
   PMMaterialMapEdit( TQWidget* parent, const char* name = 0 );
   /** */
   virtual void displayObject( PMObject* o );
   /** */
   virtual bool isDataValid( );
protected:
   /** */
   virtual void createTopWidgets( );
   /** */
   virtual void saveContents( );
private slots:
   /** */
   void slotImageFileTypeChanged( int a );
   /** */
   void slotMapTypeChanged( int a );
   /** */
   void slotInterpolateTypeChanged( int a );
   /** */
   void slotImageFileNameChanged( const TQString& a );
   /** */
   void slotImageFileBrowseClicked( );
private:
   PMMaterialMap*   m_pDisplayedObject;
   TQComboBox*   m_pImageFileTypeEdit;
   TQLineEdit*   m_pImageFileNameEdit;
   TQPushButton* m_pImageFileNameBrowse;
   TQCheckBox*   m_pOnceEdit;
   TQComboBox*   m_pMapTypeEdit;
   TQComboBox*   m_pInterpolateTypeEdit;
};

#endif
