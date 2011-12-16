//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002-2003 by Andreas Zehender
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

#ifndef PMINSERTRULESYSTEM_H
#define PMINSERTRULESYSTEM_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "pmobject.h"

#include <tqstringlist.h>
#include <tqptrlist.h>
#include <tqdict.h>
#include <tqdom.h>

class PMInsertRuleSystem;
class PMPrototypeManager;

/**
 * Base class for all object categories (class and group)
 * for the insert rule system
 */
class PMRuleCategory
{
public:
   /**
    * Default constructor
    */
   PMRuleCategory( ) { }
   /**
    * Destructor
    */
   virtual ~PMRuleCategory( ) { }
   /**
    * Returns true if the given class types matches the category.
    */
   virtual bool matches( const TQString& className ) = 0;
};

/**
 * Represents a class for the insert rule system
 */
class PMRuleClass : public PMRuleCategory
{
public:
   /**
    * Workaround to tell a created instance which
    * prototype manager to use.
    */
   static PMPrototypeManager* s_pPrototypeManager;
   /**
    * Reads the data from the TQDomElement.
    */
   PMRuleClass( TQDomElement& e );
   /** */
   virtual bool matches( const TQString& className );
private:
   TQString m_className;
   PMPrototypeManager* m_pPrototypeManager;
};

/**
 * Groups together multiple classes or groups to form a new
 * category for the insert rule system.
 */
class PMRuleDefineGroup
{
public:
   /**
    * Reads the data from the TQDomElement.
    */
   PMRuleDefineGroup( TQDomElement& e, TQPtrList<PMRuleDefineGroup>& globalGroups,
                      TQPtrList<PMRuleDefineGroup>& localGroups );
   /**
    * Destructor
    */
   virtual ~PMRuleDefineGroup( );
   /**
    * Returns true if the given class types matches the category.
    */
   virtual bool matches( const TQString& className );
   /**
    * Returns the group's name
    */
   TQString name( ) const { return m_name; }
private:
   TQPtrList<PMRuleCategory> m_categories;
   TQString m_name;
};

/**
 * Represents a group for the insert rule system
 */
class PMRuleGroup : public PMRuleCategory
{
public:
   /**
    * Reads the data from the TQDomElement.
    */
   PMRuleGroup( TQDomElement& e, TQPtrList<PMRuleDefineGroup>& globalGroups,
                TQPtrList<PMRuleDefineGroup>& localGroups );
   /** */
   bool matches( const TQString& className );
private:
   PMRuleDefineGroup* m_pGroup;
};


/**
 * Base class for all nodes for the insert rule system
 */
class PMRuleBase
{
public:
   /**
    * Default constructor.
    */
   PMRuleBase( ) { }
   /**
    * Destructor
    */
   virtual ~PMRuleBase( );
   /**
    * Returns the node type.
    */
   virtual TQString type( ) const = 0;
   /**
    * Calls countChildProtected for this node and all child nodes
    */
   void countChild( const TQString& className, bool afterInsertPoint );
   /**
    * Calls resetProtected for this node and all child nodes
    */
   void reset( );
   /**
    * Returns an iterator to all child nodes
    */
   TQPtrListIterator<PMRuleBase> childIterator( ) const
   {
      return TQPtrListIterator<PMRuleBase>( m_tqchildren );
   }
protected:
   /**
    * Reimplement this method if the nodes value depends
    * on already inserted child objects.
    */
   virtual void countChildProtected( const TQString&, bool ) { }
   /**
    * Reset all cached data (like counted child objects) here.
    */
   virtual void resetProtected( ) { }

   /**
    * Add all child nodes to this list.
    */
   TQPtrList<PMRuleBase> m_tqchildren;
};

/**
 * Base class for values used by the rule system
 */
class PMRuleValue : public PMRuleBase
{
public:
   /**
    * Default constructor
    */
   PMRuleValue( ) : PMRuleBase( ) { }
   /**
    * Returns the node's value as variant.
    *
    * Reimplement this method in sub classes.
    */
   virtual PMVariant evaluate( const PMObject* o ) = 0;
};


/**
 * Value node for object properties
 */
class PMRuleProperty : public PMRuleValue
{
public:
   /**
    * Reads the data from the TQDomElement.
    */
   PMRuleProperty( TQDomElement& e );
   /** */
   virtual TQString type( ) const { return TQString( "Property" ); }
   /** */
   virtual PMVariant evaluate( const PMObject* o );
private:
   TQString m_property;
};


/**
 * Simple constant
 */
class PMRuleConstant : public PMRuleValue
{
public:
   /**
    * Reads the data from the TQDomElement.
    */
   PMRuleConstant( TQDomElement& e );
   /** */
   virtual TQString type( ) const { return TQString( "Constant" ); }
   /** */
   virtual PMVariant evaluate( const PMObject* );
   /**
    * Converts the constant to the given type and returns true if
    * successful.
    */
   bool convertTo( PMVariant::PMVariantDataType type );
private:
   PMVariant m_value;
};

/**
 * Rule that counts child objects of certaint types
 */
class PMRuleCount : public PMRuleValue
{
public:
   /**
    * Reads the data from the TQDomElement.
    */
   PMRuleCount( TQDomElement& e, TQPtrList<PMRuleDefineGroup>& globalGroups,
                TQPtrList<PMRuleDefineGroup>& localGroups );
   /**
    * Destructor
    */
   virtual ~PMRuleCount( );
   /** */
   virtual TQString type( ) const { return TQString( "Count" ); }
   /** */
   virtual PMVariant evaluate( const PMObject* );
protected:
   /** */
   virtual void countChildProtected( const TQString& className,
                                     bool afterInsertPoint );
   /**
    * Reset all cached data (like counted child objects) here.
    */
   virtual void resetProtected( );
private:
   TQPtrList<PMRuleCategory> m_categories;
   int m_number;
};

/**
 * Base class for conditions
 */
class PMRuleCondition : public PMRuleBase
{
public:
   /**
    * Default constructor
    */
   PMRuleCondition( ) : PMRuleBase( ) { }
   /**
    * Returns the condition's value.
    *
    * Reimplement this method in sub classes.
    */
   virtual bool evaluate( const PMObject* object ) = 0;
};


/**
 * Logical negation
 */
class PMRuleNot : public PMRuleCondition
{
public:
   /**
    * Reads the data from the TQDomElement.
    */
   PMRuleNot( TQDomElement& e, TQPtrList<PMRuleDefineGroup>& globalGroups,
              TQPtrList<PMRuleDefineGroup>& localGroups );
   /** */
   virtual TQString type( ) const { return TQString( "Not" ); }
   /** */
   virtual bool evaluate( const PMObject* object );
private:
   PMRuleCondition* m_pChild;
};


/**
 * Logical and
 */
class PMRuleAnd : public PMRuleCondition
{
public:
   /**
    * Reads the data from the TQDomElement.
    */
   PMRuleAnd( TQDomElement& e, TQPtrList<PMRuleDefineGroup>& globalGroups,
              TQPtrList<PMRuleDefineGroup>& localGroups );
   /** */
   virtual TQString type( ) const { return TQString( "And" ); }
   /** */
   virtual bool evaluate( const PMObject* object );
private:
   TQPtrList<PMRuleCondition> m_conditions;
};

/**
 * Logical or
 */
class PMRuleOr : public PMRuleCondition
{
public:
   /**
    * Reads the data from the TQDomElement.
    */
   PMRuleOr( TQDomElement& e, TQPtrList<PMRuleDefineGroup>& globalGroups,
             TQPtrList<PMRuleDefineGroup>& localGroups );
   /** */
   virtual TQString type( ) const { return TQString( "Or" ); }
   /** */
   virtual bool evaluate( const PMObject* object );
private:
   TQPtrList<PMRuleCondition> m_conditions;
};

/**
 * Condition. Value is true if the object already contains
 * objects of certaint classes before the insert point.
 */
class PMRuleBefore : public PMRuleCondition
{
public:
   /**
    * Reads the data from the TQDomElement.
    */
   PMRuleBefore( TQDomElement& e, TQPtrList<PMRuleDefineGroup>& globalGroups,
                 TQPtrList<PMRuleDefineGroup>& localGroups );
   /**
    * Destructor
    */
   virtual ~PMRuleBefore( );
   /** */
   virtual TQString type( ) const { return TQString( "Before" ); }
   /** */
   virtual bool evaluate( const PMObject* object );
protected:
   /** */
   virtual void countChildProtected( const TQString& className,
                                     bool afterInsertPoint );
   /** */
   virtual void resetProtected( );
private:
   TQPtrList<PMRuleCategory> m_categories;
   bool m_contains;
};

/**
 * Condition. Value is true if the object already contains
 * objects of certaint classes after the insert point.
 */
class PMRuleAfter : public PMRuleCondition
{
public:
   /**
    * Reads the data from the TQDomElement.
    */
   PMRuleAfter( TQDomElement& e, TQPtrList<PMRuleDefineGroup>& globalGroups,
                TQPtrList<PMRuleDefineGroup>& localGroups );
   /**
    * Destructor
    */
   virtual ~PMRuleAfter( );
   /** */
   virtual TQString type( ) const { return TQString( "After" ); }
   /** */
   virtual bool evaluate( const PMObject* object );
protected:
   /** */
   virtual void countChildProtected( const TQString& className,
                                     bool afterInsertPoint );
   /** */
   virtual void resetProtected( );
private:
   TQPtrList<PMRuleCategory> m_categories;
   bool m_contains;
};

/**
 * Condition. Value is true if the object already contains
 * objects of certaint classes.
 */
class PMRuleContains : public PMRuleCondition
{
public:
   /**
    * Reads the data from the TQDomElement.
    */
   PMRuleContains( TQDomElement& e, TQPtrList<PMRuleDefineGroup>& globalGroups,
                   TQPtrList<PMRuleDefineGroup>& localGroups );
   /**
    * Destructor
    */
   virtual ~PMRuleContains( );
   /** */
   virtual TQString type( ) const { return TQString( "Contains" ); }
   /** */
   virtual bool evaluate( const PMObject* object );
protected:
   /** */
   virtual void countChildProtected( const TQString& className,
                                     bool afterInsertPoint );
   /** */
   virtual void resetProtected( );
private:
   TQPtrList<PMRuleCategory> m_categories;
   bool m_contains;
};

/**
 * Base class for comparisons
 */
class PMRuleCompare : public PMRuleCondition
{
public:
   /**
    * Reads the data from the TQDomElement.
    */
   PMRuleCompare( TQDomElement& e, TQPtrList<PMRuleDefineGroup>& globalGroups,
                  TQPtrList<PMRuleDefineGroup>& localGroups );
   /** */
   virtual bool evaluate( const PMObject* object );
protected:
   /**
    * Compares the two variants. The variants have the same type.
    *
    * Reimplement this method in sub classes.
    */
   virtual bool compare( const PMVariant& v1, const PMVariant& v2 ) = 0;
private:
   PMRuleValue* m_pValue[2];
};

/**
 * Less than comparison
 */
class PMRuleLess : public PMRuleCompare
{
public:
   /**
    * Reads the data from the TQDomElement.
    */
   PMRuleLess( TQDomElement& e, TQPtrList<PMRuleDefineGroup>& globalGroups,
               TQPtrList<PMRuleDefineGroup>& localGroups );
   /** */
   virtual TQString type( ) const { return TQString( "Less" ); }
protected:
   /** */
   virtual bool compare( const PMVariant& v1, const PMVariant& v2 );
};

/**
 * Greater than comparison
 */
class PMRuleGreater : public PMRuleCompare
{
public:
   /**
    * Reads the data from the TQDomElement.
    */
   PMRuleGreater( TQDomElement& e, TQPtrList<PMRuleDefineGroup>& globalGroups,
                  TQPtrList<PMRuleDefineGroup>& localGroups );
   /** */
   virtual TQString type( ) const { return TQString( "Greater" ); }
protected:
   /** */
   virtual bool compare( const PMVariant& v1, const PMVariant& v2 );
};

/**
 * Equal comparison
 */
class PMRuleEqual : public PMRuleCompare
{
public:
   /**
    * Reads the data from the TQDomElement.
    */
   PMRuleEqual( TQDomElement& e, TQPtrList<PMRuleDefineGroup>& globalGroups,
                TQPtrList<PMRuleDefineGroup>& localGroups );
   /** */
   virtual TQString type( ) const { return TQString( "Equal" ); }
protected:
   /** */
   virtual bool compare( const PMVariant& v1, const PMVariant& v2 );
};

class PMRule : public PMRuleBase
{
public:
   /**
    * Reads the data from the TQDomElement.
    */
   PMRule( TQDomElement& e, TQPtrList<PMRuleDefineGroup>& globalGroups,
           TQPtrList<PMRuleDefineGroup>& localGroups );
   /**
    * Destructor
    */
   virtual ~PMRule( );
   /** */
   virtual TQString type( ) const { return TQString( "Rule" ); }
   /**
    * Returns true if this rule matches for the given class.
    */
   bool matches( const TQString& className );
   /**
    * Returns the conditions value.
    */
   bool evaluate( const PMObject* parent );

private:
   TQPtrList<PMRuleCategory> m_categories;
   PMRuleCondition* m_pCondition;
};

/**
 * Class used internally by the insert rule system
 *
 * Stores all rules for one class.
 */
class PMRuleTargetClass
{
public:
   /**
    * Reads the data from the TQDomElement.
    */
   PMRuleTargetClass( TQDomElement& e,
                      TQPtrList<PMRuleDefineGroup>& globalGroups );
   /**
    * Destructor
    */
   ~PMRuleTargetClass( );

   /**
    * Reads rules and groups from the TQDomELement and appends
    * them to the local ones.
    */
   void appendRules( TQDomElement& e, TQPtrList<PMRuleDefineGroup>& globalGroups );
   /**
    * Returns an iterator to the rules
    */
   TQPtrListIterator<PMRule> rules( ) const
   {
      return TQPtrListIterator<PMRule>( m_rules );
   }
   /**
    * Returns the class name
    */
   TQString name( ) const { return m_class; }
   /**
    * Returns a list of exceptions for this rule.
    */
   TQStringList exceptions( ) const { return m_exceptions; }
private:
   TQPtrList<PMRuleDefineGroup> m_groups;
   TQPtrList<PMRule> m_rules;
   TQString m_class;
   TQStringList m_exceptions;
};

/**
 * Rule based system that checks which objects can be inserted as child
 * into another object.
 *
 * The rules are defined in the file "pmbaseinsertrules.xml".
 * The file "pminsertrules.dtd" is a DTD file that can be used
 * to validate rule files.
 *
 * Plugins can add additional rules to the system.
 */
class PMInsertRuleSystem
{
public:
   /**
    * Constructor
    */
   PMInsertRuleSystem( PMPart* part );
   /**
    * Destructor
    */
   ~PMInsertRuleSystem( );

   /**
    * Tells the system to load the rules from a file.
    *
    * Rules are never loaded twice for the same file. It is save
    * to call this method twice for the same file.
    */
   void loadRules( const TQString& fileName );

   /**
    * Returns true if an object of the given class can be inserted as child
    * after the object after.
    *
    * The parser uses the third parameter for top level objects. These objects
    * have to be treated as if they are inserted after the object after.
    */
   bool canInsert( const PMObject* parentObject, const TQString& className,
                   const PMObject* after, const PMObjectList* objectsBetween = 0 );

   /**
    * Returns true if the object can be inserted as child
    * after the object after.
    *
    * The parser uses the third parameter for top level objects. These objects
    * have to be treated as if they are inserted after the object after.
    *
    * Same as canInsert( parentObject, object->class( ), after, objectsBetween )
    */
   bool canInsert( const PMObject* parentObject, const PMObject* object,
                   const PMObject* after, const PMObjectList* objectsBetween = 0 );

   /**
    * Returns the number of objects that can be inserted at that position
    */
   int canInsert( const PMObject* parentObject, const PMObjectList& list,
                  const PMObject* after );
   /**
    * Returns the number of objects that can be inserted at that position
    */
   int canInsert( const PMObject* parentObject, const TQStringList& listOfClasses,
                  const PMObject* after );
   /**
    * Returns a pointer to the part
    */
   PMPart* part( ) const { return m_pPart; }

private:
   /**
    * List of all rules.
    */
   TQPtrList<PMRuleTargetClass> m_classRules;
   /**
    * List of global groups
    */
   TQPtrList<PMRuleDefineGroup> m_groups;
   /**
    * Dictionary that maps from the class name
    * to a list of rules that match.
    */
   TQDict<PMRuleTargetClass> m_rulesDict;
   /**
    * List of already loaded files
    */
   TQStringList m_loadedFiles;
   PMPart* m_pPart;
};

#endif
