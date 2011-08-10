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


#ifndef PMBUMPMAPEDIT_H
#define PMBUMPMAPEDIT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "pmpalettevalueedit.h"
#include "pmdialogeditbase.h"

class PMBumpMap;
class PMPaletteValue;
class PMVectorEdit;
class TQComboBox;
class PMFloatEdit;
class PMIntEdit;
class TQLabel;
class TQCheckBox;
class TQWidget;
class TQLineEdit;
class TQPushButton;

/**
 * Dialog edit class for @ref PMBumpMap.
 */
class PMBumpMapEdit : public PMDialogEditBase
{
   Q_OBJECT
  TQ_OBJECT
   typedef PMDialogEditBase Base;
public:
   /**
    * Creates a PMBumpMapEdit with parent and name
    */
   PMBumpMapEdit( TQWidget* parent, const char* name = 0 );
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
   PMBumpMap*   m_pDisplayedObject;
   TQComboBox*   m_pImageFileTypeEdit;
   TQLineEdit*   m_pImageFileNameEdit;
   TQPushButton* m_pImageFileNameBrowse;
   TQCheckBox*   m_pOnceEdit;
   TQComboBox*   m_pMapTypeEdit;
   TQComboBox*   m_pInterpolateTypeEdit;
   TQCheckBox*   m_pUseIndexEdit;
   PMFloatEdit* m_pBumpSizeEdit;
};

#endif
