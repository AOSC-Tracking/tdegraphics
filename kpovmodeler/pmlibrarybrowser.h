/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Luis Carvalho
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

#ifndef PMLIBRARYBROWSER_H
#define PMLIBRARYBROWSER_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tqwidget.h>
#include <tqdict.h>

#include "pmviewbase.h"
#include "pmviewfactory.h"

class TQComboBox;
class TQIconViewItem;
class TQPushButton;
class TDEConfig;
class KDirOperator;
class PMLibraryIconView;
class PMLibraryEntryPreview;
class PMLibraryHandle;
class PMPart;
namespace TDEIO{ class Job; }

typedef TQDict<TQString> TQStringDict;

/**
 * Wrapper class for the treeview/dock widget
 */
class PMLibraryBrowserView : public PMViewBase
{
   Q_OBJECT
  
public:
   /**
    * Default constructor
    */
   PMLibraryBrowserView( PMPart* part, TQWidget* parent, const char* name = 0 );

   /** */
   virtual TQString viewType( ) const { return TQString( "librarybrowserview" ); }
   /** */
   virtual TQString description( ) const;
};

/**
 * This class provides a non-modal dialog to browse the available libraries.
 */
class PMLibraryBrowserViewWidget: public TQWidget
{
   Q_OBJECT
  
public:
   PMLibraryBrowserViewWidget( TQWidget *parent, const char* name = NULL );

private slots:
   void resizeEvent( TQResizeEvent* ev );
   void slotPathSelected( const TQString& str );
   void slotSelectionChanged( TQIconViewItem* item );
   void slotSelectionExecuted( TQIconViewItem* item );
   void slotUpButtonClicked( );
   void slotDeleteClicked( );
   void slotNewObjectClicked( );
   void slotNewSubLibraryClicked( );

   void slotJobResult( TDEIO::Job * );

   // These slots provide delayed activity on the IconView
   void slotIconViewRefresh( );
   void slotIconViewSetLibrary( );

private:


   TQPushButton*           m_pUpButton;
   TQPushButton*           m_pNewSubLibraryButton;
   TQPushButton*           m_pNewObjectButton;
   TQPushButton*           m_pDeleteObjectButton;
   TQComboBox*             m_pLibraryComboBox;
   PMLibraryIconView*     m_pLibraryIconView;
   PMLibraryEntryPreview* m_pLibraryEntryPreview;
   PMLibraryHandle*       m_pCurrentLibrary;
   PMLibraryHandle*       m_pFutureLibrary;
   bool                   m_topLibraryReadOnly;
};

/**
 * Factory class for the tree view
 */
class PMLibraryBrowserViewFactory : public PMViewTypeFactory
{
public:
   PMLibraryBrowserViewFactory( ) { }
   virtual TQString viewType( ) const { return TQString( "librarybrowserview" ); }
   virtual TQString description( ) const;
   virtual TQString iconName( ) const { return TQString( "pmlibrarybrowserview" ); }
   virtual PMViewBase* newInstance( TQWidget* parent, PMPart* part ) const
   {
      return new PMLibraryBrowserView( part, parent );
   }
};

#endif
