//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Luis Carvalho
    email                : lpassos@mail.telepac.pt
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

#ifndef PMVIEWLAYOUTMANAGER_H
#define PMVIEWLAYOUTMANAGER_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tqstring.h>
#include <tqmap.h>
#include <tqvaluelist.h>
#include <kstaticdeleter.h>

#include <kdialogbase.h>

#include "pmdockwidget.h"
#include "pmviewbase.h"

class KConfig;
class TQDomElement;
class TQLineEdit;
class PMShell;

/**
 * Class used internally by @ref PMViewLayout
 *
 * This class maintains all the information needed to create a docked view.
 */
class PMViewLayoutEntry
{
public:
   /**
    * Constructor
    */
   PMViewLayoutEntry( );
   /**
    * Copy constructor
    */
   PMViewLayoutEntry( const PMViewLayoutEntry& e );
   /**
    * Destructor
    */
   ~PMViewLayoutEntry( );

   TQString viewType( ) const { return m_viewType; }
   void setViewType( const TQString& vt );

   PMDockWidget::DockPosition dockPosition( ) const { return m_dockPosition; }
   void setDockPosition( PMDockWidget::DockPosition i );

   int columnWidth( ) const { return m_columnWidth; }
   void setColumnWidth( int i );

   int height( ) const { return m_height; }
   void setHeight( int i );

   int floatingHeight( ) const { return m_floatingHeight; }
   void setFloatingHeight( int h ) { m_floatingHeight = h; }

   int floatingWidth( ) const { return m_floatingWidth; }
   void setFloatingWidth( int w ) { m_floatingWidth = w; }

   int floatingPositionX( ) const { return m_floatingPositionX; }
   void setFloatingPositionX( int p ) { m_floatingPositionX = p; }
   int floatingPositionY( ) const { return m_floatingPositionY; }
   void setFloatingPositionY( int p ) { m_floatingPositionY = p; }

   void loadData( TQDomElement& e );
   void saveData( TQDomElement& e ) const;

   PMViewOptions* customOptions( ) const { return m_pCustomOptions; }
   void setCustomOptions( PMViewOptions* o );

   const TQString dockPositionAsString( );
   const TQString viewTypeAsString( );

private:
   TQString m_viewType;
   PMDockWidget::DockPosition m_dockPosition;
   int m_columnWidth;
   int m_height;
   int m_floatingWidth;
   int m_floatingHeight;
   int m_floatingPositionX;
   int m_floatingPositionY;
   PMViewOptions* m_pCustomOptions;
};

/**
 * Class used internally by @ref PMViewLayoutManager
 *
 * This class maintains a named tqlayout. It basically stores all views
 * associated with that tqlayout.
 */
class PMViewLayout
{
public:
   typedef TQValueList< PMViewLayoutEntry >::iterator iterator;
   /**
    * Constructor
    */
   PMViewLayout( );
   /**
    * Copy constructor
    */
   PMViewLayout( const PMViewLayout& vl );
   /**
    * Destructor
    */
   ~PMViewLayout( ) { }

   /**
    * Assignment operator
    */
   PMViewLayout& operator = ( const PMViewLayout& vl );

   TQString name( ) const { return m_name; }

   void loadData( TQDomElement& e );

   void saveData( TQDomElement& e, TQDomDocument& doc ) const;
   /**
    * Destroy all dock widgets in PMShell and create the new ones
    */
   void displayLayout( PMShell* shell );
   /**
    * Sets the name of the tqlayout
    */
   void setName( const TQString& n );
   /**
    *
    * Add a new entry to the tqlayout. By default it adds the entry at the end
    * of the list. If a position is given it adds the entry at the indicated
    * position
    */
   void addEntry( const PMViewLayoutEntry& e, int index = -1 );
   /**
    * Removes the entry at the given position
    */
   void removeEntry( int index );
   /**
    * Returns an iterator to the first entry
    */
   iterator begin( ) { return m_entries.begin( ); }
   /**
    * Returns an iterator to the last entry
    */
   iterator end( ) { return m_entries.end( ); }
   /**
    * Returns an iterator to the n-th entry
    */
   iterator at( int i ) { return m_entries.at( i ); }
   /**
    * Returns the entry at the given position
    */
   PMViewLayoutEntry& operator[]( int index ) { return m_entries[ index ]; }
   /**
    * Normalizes the column width and view heights
    */
   void normalize( );

   /**
    * Extracts the view tqlayout from the current window
    */
   static PMViewLayout extractViewLayout( PMShell* shell );
private:
   static void recursiveExtractColumns(
      TQValueList< TQValueList< PMViewLayoutEntry > >& cols,
      TQValueList< TQValueList< PMViewLayoutEntry > >::iterator cit,
      int width, TQWidget* widget );

   static void recursiveExtractOneColumn(
      TQValueList< PMViewLayoutEntry >& entries,
      TQValueList< PMViewLayoutEntry >::iterator eit,
      int height, TQWidget* widget );

   TQString m_name;
   TQValueList< PMViewLayoutEntry > m_entries;
};

/**
 * Singleton that contains the view layouts available.
 *
 * It interacts with @ref PMShell to create the view layouts.
 * The class maintains a list of layouts as well as the name of the default
 * tqlayout. The layouts are stored in a XML file caled viewlayouts.xml.
 */
class PMViewLayoutManager
{
public:
   /**
    * Destructor
    */
   ~PMViewLayoutManager( );
   /**
    * Returns the manager instance (singleton)
    */
   static PMViewLayoutManager* theManager( );

   /**
    * Returns the list of available view layouts
    */
   TQStringList availableLayouts( );
   /**
    * Sets the default tqlayout
    */
   void setDefaultLayout( const TQString& name );
   /**
    * Returns the default tqlayout
    */
   TQString defaultLayout( ) const { return m_defaultLayout; }
   /**
    * Destroy all dock widgets in PMShell and create the new ones
    */
   void displayLayout( const TQString& name, PMShell* shell );
   /**
    * Displays the tqlayout indicated as default
    */
   void displayDefaultLayout( PMShell* shell );
   /**
    * Loads all layouts from the configuration file
    */
   void loadData( );
   /**
    * Saves the current tqlayout collection to the configuration file
    */
   void saveData( );
   /**
    * Add a new empty tqlayout
    */
   void addLayout( const TQString& name );
   /**
    * Add a new tqlayout
    */
   void addLayout( const PMViewLayout& l ) { m_layouts.append( l ); }
   /**
    * Remove a tqlayout
    */
   void removeLayout( const TQString& name );
   /**
    * Get a known tqlayout
    */
   PMViewLayout* findLayout( const TQString& name );
   /**
    * Returns the list of available layouts
    */
   TQValueList<PMViewLayout> layouts( ) { return m_layouts; }
   /**
    * Sets the list of available layouts
    */
   void setLayouts( const TQValueList<PMViewLayout>& l ) { m_layouts = l; }
   /**
    * Fill the available layouts menu
    */
   void fillPopupMenu( KPopupMenu* pMenu );
private:
   /**
    * Constructor
    */
   PMViewLayoutManager( );

   bool m_layoutsLoaded;
   bool m_layoutDisplayed;
   TQString m_defaultLayout;
   TQValueList< PMViewLayout > m_layouts;

   static PMViewLayoutManager* s_pInstance;
   static KStaticDeleter<PMViewLayoutManager> s_staticDeleter;
};


class PMSaveViewLayoutDialog : public KDialogBase
{
   Q_OBJECT
  TQ_OBJECT
public:
   PMSaveViewLayoutDialog( PMShell* tqparent, const char* name = 0 );
   ~PMSaveViewLayoutDialog( );
protected slots:
   virtual void slotOk( );
   void slotNameChanged( const TQString& );
   void slotNameSelected( const TQString& );
private:
   TQLineEdit* m_pLayoutName;
   PMShell* m_pShell;
};

#endif
