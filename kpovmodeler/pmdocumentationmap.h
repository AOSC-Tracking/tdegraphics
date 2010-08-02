//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Andreas Zehender
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

#ifndef PMDOCUMENTATIONMAP_H
#define PMDOCUMENTATIONMAP_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tqstring.h>
#include <tqmap.h>
#include <tqptrlist.h>
#include <tqvaluelist.h>
#include <kstaticdeleter.h>

class KConfig;
class TQDomElement;

/**
 * Class used internally by @ref PMDocumentationMap
 */
class PMDocumentationVersion
{
public:
   /**
    * Constructor
    */
   PMDocumentationVersion( ) { }
   /**
    * Destructor
    */
   ~PMDocumentationVersion( ) { }

   TQString version( ) const { return m_version; }
   void setVersion( const TQString& str ) { m_version = str; }
   TQString index( ) const { return m_index; }
   void setIndex( const TQString& str ) { m_index = str; }

   TQString documentation( const TQString& className ) const;

   void loadData( TQDomElement& e );
      
private:
   TQString m_version;
   TQString m_index;
   TQMap< TQString, TQString > m_map;
};

/**
 * Class that maps the class name to the corresponding povray
 * user documentation file
 */
class PMDocumentationMap
{
public:
   /**
    * Destructor
    */
   ~PMDocumentationMap( );
   /**
    * Returns the map instance (singleton)
    */
   static PMDocumentationMap* theMap( );

   /**
    * Sets the path to the povray user documentation
    */
   void setPovrayDocumentationPath( const TQString& s )
   {
      m_documentationPath = s;
   }
   /**
    * Returns the path to the povray user documentation
    */
   TQString povrayDocumentationPath( )
   {
      return m_documentationPath;
   }
   
   /**
    * Sets the povray documentation version
    */
   void setDocumentationVersion( const TQString& str );
   /**
    * Returns the povray documentation version
    */
   TQString documentationVersion( ) const { return m_currentVersion; }

   /**
    * Returns the list of available documentation versions
    */
   TQValueList<TQString> availableVersions( );

   /**
    * Returns the povray documentation file for the
    * given object type
    */
   TQString documentation( const TQString& objectName );
   
   void saveConfig( KConfig* cfg );
   void restoreConfig( KConfig* cfg );

private:
   
   /**
    * Constructor
    */
   PMDocumentationMap( );
   void loadMap( );
   void findVersion( );

   TQString m_documentationPath;
   bool m_mapLoaded;
   TQPtrList< PMDocumentationVersion > m_maps;
   PMDocumentationVersion* m_pCurrentVersion;
   TQString m_currentVersion;

   static PMDocumentationMap* s_pInstance;
   static KStaticDeleter<PMDocumentationMap> s_staticDeleter;   
};

#endif
