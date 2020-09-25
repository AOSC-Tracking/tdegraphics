/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2002 by Andreas Zehender
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


#ifndef PMPTMANAGER_H
#define PMPTMANAGER_H

#include "pmobject.h"
#include <tqdict.h>
#include <tqmap.h>
#include <tqvaluelist.h>

class PMPart;

/**
 * Description class for declarations types, used by @ref PMPrototypeManager
 * and @ref PMDeclare
 */
class PMDeclareDescription
{
public:
   PMDeclareDescription( ) { }
   PMDeclareDescription( const PMDeclareDescription& d )
   {
      type = d.type;
      description = d.description;
      pixmap = d.pixmap;
   }
   PMDeclareDescription( PMMetaObject* t, const TQString& d, const TQString& p )
   {
      type = t;
      description = d;
      pixmap = p;
   }
   PMDeclareDescription& operator=( const PMDeclareDescription& d )
   {
      type = d.type;
      description = d.description;
      pixmap = d.pixmap;
      return *this;
   }
   PMMetaObject* type;
   TQString description;
   TQString pixmap;
};

/**
 * Prototype manager for @ref PMObject.
 *
 * This class stores class and inheritance information for each
 * available object type.
 *
 * Each @ref PMPart class holds one instance of this class. The
 * available objects depend on the loaded plugins.
 *
 * Patterns: Prototype
 */
class PMPrototypeManager
{
public:
   /**
    * Creates a prototype manager for the part.
    */
   PMPrototypeManager( PMPart* part );
   /**
    * Deletes the prototype manager
    */
   ~PMPrototypeManager( );
   /**
    * Adds the object to the list of prototypes. The prototype becomes
    * the owner of the object and will be delete immediately by the
    * prototype manager.
    */
   void addPrototype( PMObject* obj );
   /**
    * Adds a declaration type. Needed information is the class type,
    * the @ref description( ) and the @ref pixmap( )
    */
   void addDeclarationType( const TQString& className,
                            const TQString& description,
                            const TQString& pixmap );
   /**
    * Returns an iterator to the list of available objects
    */
   TQPtrListIterator<PMMetaObject> prototypeIterator( ) const;
   /**
    * Returns an iterator to the list of available declaration types
    */
   const TQValueList<PMDeclareDescription>& declarationTypes( ) const;
   /**
    * Returns a new PMObject by class name
    */
   PMObject* newObject( const TQString& name ) const;
   /**
    * Returns the meta object by class name or 0 if this class does
    * not exist.
    * @see PMMetaObject
    */
   PMMetaObject* metaObject( const TQString& name ) const;
   /**
    * Returns true if the class exists
    */
   bool existsClass( const TQString& name ) const
   {
      return metaObject( name );
   }
   /**
    * Returns true if the second class is a base class for
    * the first class
    */
   bool isA( const TQString& className, const TQString& baseClassName ) const;
   /**
    * Returns true if the second class is a base class for
    * the first class
    */
   bool isA( PMMetaObject* c, const TQString& baseClassName ) const;
   /**
    * Returns the real class if only the lower case version is know.
    * Used by the xml parser
    */
   TQString className( const TQString& lowercase ) const;
   /**
    * Returns a pointer to the part
    */
   PMPart* part( ) const { return m_pPart; }

private:
   TQPtrList<PMMetaObject> m_prototypes;
   TQDict<PMMetaObject> m_metaDict;
   TQMap<TQString, TQString> m_lowerCaseDict;
   TQValueList<PMDeclareDescription> m_declareDescriptions;
   PMPart* m_pPart;
};
#endif
