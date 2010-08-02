//-*-C++-*-
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

#ifndef PMLIBRARYOBJECT_H
#define PMLIBRARYOBJECT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tqstring.h>
#include <tqmap.h>
#include <tqvaluelist.h>
#include <kstaticdeleter.h>
#include <tqstringlist.h>

class KURL;
class KArchive;
class KTar;
class TQImage;

/**
 * This class implements a library object.
 *
 * A library object has a name, a textual description, a graphical 
 * preview and the object data, of course. It also contains a collection of
 * keywords.
 *
 * When an instance of PMLibraryObject is created, the objects description is
 * loaded.
 *
 * The graphical preview and the objects data are loaded only if needed.
 *
 */
class PMLibraryObject
{
public:
   /** 
    * Constructor for the library object. Creates an empty library object. 
    */
   PMLibraryObject( );
   /** 
    * Constructor for the library object. 
    * Loads the object data from the specified library object file.
    */
   PMLibraryObject( KURL u );
   /** 
    * Destructor 
    */
   ~PMLibraryObject( );

   /** 
    * Name of the library object. 
    */
   TQString name( ) const { return m_name; }
   /** 
    * Textual description of the library object. 
    */
   TQString description( ) const { return m_description; }
   /** 
    * List of keywords for search of the library object. 
    */
   TQString keywords( ) const { return m_keywords; }
   /** 
    * Graphical Preview. 
    */
   TQImage* preview( );
   /**
    * True is the preview has been loaded.
    */
   bool isPreviewLoaded( ) const { return m_previewLoaded; }
   /** 
    * Objects for the scene 
    */
   TQByteArray* objects( );
   bool areObjectsLoaded( ) const { return m_objectsLoaded; }

   /** 
    * Set the library object name 
    */
   void setName( const TQString& str );
   /** 
    * Set the library object description 
    */
   void setDescription( const TQString& str );
   /** 
    * Set the library object keywords 
    */
   void setKeywords( const TQString& str );
   /** 
    * Set the preview image 
    */
   void setPreview( const TQImage& img );
   /** 
    * Set the object data 
    */
   void setObjects( const TQByteArray& obj );

   /** 
    * Save the library object to a file 
    */
   void save( const TQString& fileName );

private:
   void loadLibraryInfo( );
   void saveLibraryInfo( );
   void savePreview( );
   void saveObjects( );

   bool        m_previewLoaded;
   bool        m_objectsLoaded;
   TQString     m_name;
   TQString     m_description;
   TQString     m_keywords;
   KTar*       m_data;
   TQImage*     m_preview;
   TQByteArray* m_objects;
   TQStringList m_extraFiles;
};

#endif
