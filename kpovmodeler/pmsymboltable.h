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


#ifndef PMSYMBOLTABLE_H
#define PMSYMBOLTABLE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "pmvalue.h"
class PMObject;
class PMDeclare;

#include <tqdict.h>
#include <tqstring.h>

/**
 * Entry in a @ref PMSymbolTable. Can be a @ref PMValue or a @ref PMObject
 */

class PMSymbol
{
public:
   enum PMSymbolType { Value, Object };
   /**
    * Creates a entry for an object
    */
   PMSymbol( const TQString& id, PMDeclare* o );
   /**
    * Creates a entry for a value
    */
   PMSymbol( const TQString& id, const PMValue& v );
   /**
    * Destructor
    */
   ~PMSymbol( );

   /**
    * Returns the type of the symbol
    */
   PMSymbolType type( ) const { return m_type; }
   /**
    * Returns the id of the symbol
    */
   TQString id( ) const { return m_id; }
   /**
    * Sets the id
    */
   void setId( const TQString& id );
   /**
    * Returns the stored object
    */
   PMDeclare* object( ) const;
   /**
    * Returns the stored value
    */
   PMValue value( ) const;
   /**
    * The maximum length for povray ids
    */
   static const unsigned int MaxIDLength = 40;
   /**
    * If the parser parses a declare object with this id, the
    * new id is renamed. This function returns a pointer to the
    * renamed symbol.
    */
   PMSymbol* renamedSymbol( ) const { return m_pRenamedSymbol; }
   /**
    * Sets the renamed declare object to symbol
    */
   void setRenamedSymbol( PMSymbol* symbol ) { m_pRenamedSymbol = symbol; }
    
private:
   PMSymbolType m_type;
   PMDeclare* m_pObj;
   PMSymbol* m_pRenamedSymbol;
   PMValue* m_pVal;
   TQString m_id;
};

/**
 * Symbol table for povray #declare statements
 */
class PMSymbolTable : public TQDict<PMSymbol>
{
public:
   /**
    * Constructor
    */
   PMSymbolTable( );
   /**
    * Destructor
    */
   ~PMSymbolTable( );
   /**
    * Returns a free id with prefix prefix and a number as suffix
    */
   TQString findNewID( const TQString& prefix );
   /**
    * Returns a free id with prefix prefix and a number as suffix and assigns
    * it to the object.
    *
    * Does NOT add the object to the symbol table.
    *
    * Returns the new symbol.*/
   PMSymbol* findNewID( const TQString& prefix, PMDeclare* obj );
private:
   TQDict<int> m_lastID;
};

#endif
