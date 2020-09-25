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


#ifndef PMIMAGEMAPEDIT_H
#define PMIMAGEMAPEDIT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "pmpalettevalueedit.h"
#include "pmdialogeditbase.h"

class PMImageMap;
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
 * Dialog edit class for @ref PMImageMap.
 */
class PMImageMapEdit : public PMDialogEditBase
{
   Q_OBJECT
  
   typedef PMDialogEditBase Base;
public:
   /**
    * Creates a PMImageMapEdit with parent and name
    */
   PMImageMapEdit( TQWidget* parent, const char* name = 0 );

   /** */
   virtual void displayObject( PMObject* o );

   /** */
   virtual bool isDataValid( );
protected:
   /** */
   virtual void createTopWidgets( );
   /** */
   virtual void createBottomWidgets( );
   /** */
   virtual void saveContents( );
   /** */
   TQValueList<PMPaletteValue> filters( );
   /** */
   TQValueList<PMPaletteValue> transmits( );
   /** */
   void displayPaletteEntries( const TQValueList<PMPaletteValue>& filters,
                               const TQValueList<PMPaletteValue>& transmits );

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
   /** */
   void slotFilterAllClicked( );
   /** */
   void slotTransmitAllClicked( );
   /** */
   void slotAddFilterEntry( );
   /** */
   void slotRemoveFilterEntry( );
   /** */
   void slotAddTransmitEntry( );
   /** */
   void slotRemoveTransmitEntry( );
private:
   PMImageMap*  m_pDisplayedObject;
   TQComboBox*   m_pImageFileTypeEdit;
   TQLineEdit*   m_pImageFileNameEdit;
   TQPushButton* m_pImageFileNameBrowse;
   TQCheckBox*   m_pOnceEdit;
   TQComboBox*   m_pMapTypeEdit;
   TQComboBox*   m_pInterpolateTypeEdit;
   TQCheckBox*   m_pEnableFilterAllEdit;
   TQCheckBox*   m_pEnableTransmitAllEdit;
   PMFloatEdit* m_pFilterAllEdit;
   PMFloatEdit* m_pTransmitAllEdit;
   TQWidget*     m_pFiltersWidget;
   TQWidget*     m_pTransmitsWidget;

   TQPtrList<PMPaletteValueEdit> m_filterEntries;
   TQPtrList<TQPushButton>        m_filterAddButtons;
   TQPtrList<TQPushButton>        m_filterRemoveButtons;
   TQPtrList<PMPaletteValueEdit> m_transmitEntries;
   TQPtrList<TQPushButton>        m_transmitAddButtons;
   TQPtrList<TQPushButton>        m_transmitRemoveButtons;
};

#endif
