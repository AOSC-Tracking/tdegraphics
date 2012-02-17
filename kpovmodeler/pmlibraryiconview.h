//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Luis Carvalho
    email                : lpassos@oninetspeed.pt
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/

#ifndef PMLIBRARYICONVIEW_H
#define PMLIBRARYICONVIEW_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <kiconview.h>

class PMLibraryHandle;

/**
 * This class is the drag and drop object for the icon view
 */
class PMLibraryIconDrag : public TQIconDrag
{
   Q_OBJECT
  
public:
   /** Constructor */
   PMLibraryIconDrag( TQWidget * dragSource, const char* name = 0 ) : TQIconDrag( dragSource, name ) {}

   /** @return The ith format, or NULL. */
   const char* format( int i ) const;
   /** @return The encoded payload of this object, in the specified MIME format. */
   TQByteArray encodedData( const char* mime ) const;
   /** @return True if the information in e can be decoded */
   static bool canDecode( TQMimeSource* e );
   /**
    * Attempts to decode the data in e
    * @return True if successful otherwise returns false
    */
   static bool decode( TQMimeSource* e, TQStringList& StrList, TQValueList<bool>& subLibList );
   /** Adds a item to the object */
   void append( const TQIconDragItem &item, const TQRect &pr, const TQRect &tr, const TQString &path, bool subLib );

private:
   TQStringList m_paths;
   TQValueList<bool> m_subLibs;
};

/**
 * This class provides a view to browse objects, showing their previews.
 */
class PMLibraryIconView: public KIconView
{
   Q_OBJECT
  
public:
   PMLibraryIconView( TQWidget *parent, const char* name = NULL );

   /**
    * Set the library base path
    */
   void setLibrary( PMLibraryHandle* h );
   /**
    * Returns the library in view
    */
   PMLibraryHandle* library( ) const { return m_pLibrary; }

public slots:
   /**
    * refresh the icon view
    */
   void refresh( );

   /** called when an Item is dropped onto the view */
   void slotDropped( TQDropEvent *e, const TQValueList<TQIconDragItem>& );

protected:
   /** @return a TQDragObject for drag and drop */
   virtual TQDragObject* dragObject( );

private:
   PMLibraryHandle *m_pLibrary;
   PMLibraryHandle *m_pCurrentLibrary;
};

/**
 * This class holds a library object's icon. It also remembers the path
 * where the file is.
 */
class PMLibraryIconViewItem: public KIconViewItem
{
public:
   PMLibraryIconViewItem( TQIconView *parent, const TQString& text, const TQString& path, bool isSubLibrary );
   PMLibraryIconViewItem( TQIconView *parent, const TQString& text, const TQImage& image, const TQString& path, bool isSubLibrary );

   /** Get the path of the entry */
   TQString path( ) const { return m_path; }
   /** Is the entry a sublib library? */
   bool isSubLibrary( ) const { return m_isSubLibrary; }
   /** Reimplement accept drop to take items */
   virtual bool acceptDrop( const TQMimeSource *mime ) const;

protected:
   /** Tokes a dropped item */
   void dropped( TQDropEvent *evt, const TQValueList<TQIconDragItem>& );
   /** Checks for the existance of oldpath and generates a new path as required */
   TQString newPath( const TQString oldPath, bool subLib );

private:
   TQString m_path;
   bool    m_isSubLibrary;
};

#endif
