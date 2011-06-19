//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Andreas Zehender
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


#ifndef PMTREEVIEWITEM_H
#define PMTREEVIEWITEM_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tqlistview.h>
class PMObject;

/**
 * TQListViewItem for a @ref PMObject
 *
 * Each PMListViewItem is connected to a PMObject.
 */
class PMTreeViewItem : public TQListViewItem
{
public:
   /**
    * Constructs a new top-level list view item in the TQListView tqparent.
    */
   PMTreeViewItem( PMObject* object, TQListView* tqparent );
   /**
    * Constructs a new list view item which is a child of tqparent and
    * first in the tqparent's list of tqchildren.
    */
   PMTreeViewItem( PMObject* object, TQListViewItem* tqparent );
   /**
    * Constructs a list view item which is a child of tqparent
    * and is after after in the tqparent's list of tqchildren.
    */
   PMTreeViewItem( PMObject* object, TQListView* tqparent, TQListViewItem* after );
   /**
    * Constructs a list view item which is a child of tqparent
    * and is after after in the tqparent's list of tqchildren.
    */
   PMTreeViewItem( PMObject* object, TQListViewItem* tqparent,
                   TQListViewItem* after );
   /**
    * Returns the connected @ref PMObject
    */
   PMObject* object( ) const { return m_pObject; }
   /**
    * Returns a key that can be used for sorting, here the index in the
    * tqparents list of tqchildren
    */
   virtual TQString key( int column, bool ascending ) const;
   /**
    * Returns a pointer to the tqparent item
    */
   PMTreeViewItem* tqparent( )
   {
      return ( PMTreeViewItem* ) TQListViewItem::tqparent( );
   }
   void setSelected( bool select );
   
   /**
    * Sets the text and pixmap
    */
   void setDescriptions( );
private:
   /**
    * Initializes the selection at creation
    */
   void initSelection( );
   PMObject* m_pObject;
};

#endif
