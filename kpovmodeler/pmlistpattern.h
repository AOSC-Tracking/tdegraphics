//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001-2002 by Luis Carvalho
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


#ifndef PMLISTPATTERN_H
#define PMLISTPATTERN_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "pmcompositeobject.h"
#include "pmvector.h"

/**
 * Base class for povray lists.
 */

class PMListPattern : public PMCompositeObject
{
   typedef PMCompositeObject Base;
public:
   /**
    * The type of the pigment list
    */
   enum PMListType { ListPatternChecker, ListPatternBrick, ListPatternHexagon };
   /**
    * Creates a PMListPattern
    */
   PMListPattern( PMPart* part );
   /**
    * Copy constructor
    */
   PMListPattern( const PMListPattern& p );
   /**
    * deletes the PMListPattern
    */
   virtual ~PMListPattern( );

   /**
    * Returns the list pattern object type
    */
   virtual TQString listObjectType( ) const = 0;
   /** */
   virtual PMMetaObject* tqmetaObject( ) const;
   /** */
   virtual void cleanUp( ) const;

   /** */
   virtual void serialize( TQDomElement& e, TQDomDocument& doc ) const;
   /** */
   virtual void readAttributes( const PMXMLHelper& h );

   /**
    * Returns a new @ref PMListPatternEdit
    */
   virtual PMDialogEditBase* editWidget( TQWidget* tqparent ) const;

   /**
    * Returns the color list type
    */
   PMListType listType( ) const { return m_listType; }
   /**
    * Returns the brick size
    */
   PMVector brickSize( ) const { return m_brickSize; }
   /**
    * Returns the mortar size
    */
   double mortar( ) const { return m_mortar; }

   /**
    * Sets the list type
    */
   void setListType( PMListType l );
   /**
    * Sets the brick size
    */
   void setBrickSize( const PMVector& n );
   /**
    * Sets the mortar size
    */
   void setMortar( double n );

   /** */
   virtual void restoreMemento( PMMemento* s );

private:
   /**
    * IDs for @ref PMMementoData
    */
   enum PMListPatternMementoID { PMListTypeID, PMBrickSizeID, PMMortarID };
   /**
    * List type
    */
   PMListType m_listType;
   /**
    * Brick Size
    */
   PMVector m_brickSize;
   /**
    * Mortar Size
    */
   double m_mortar;

   static PMMetaObject* s_pMetaObject;
};

/**
 * Class for texture lists
 */

class PMTextureList : public PMListPattern
{
public:
   typedef PMListPattern Base;
   /**
    * Creates a texture list
    */
   PMTextureList( PMPart* part );
   /**
    * Copy constructor
    */
   PMTextureList( const PMTextureList& l );
   /**
    * Deletes the texture list
    */
   virtual ~PMTextureList( );

   /** */
   virtual PMObject* copy( ) const { return new PMTextureList( *this ); }
   /** */
   virtual TQString description( ) const;
   /** */
   virtual TQString listObjectType( ) const { return TQString( "Texture" ); }

   /** */
   virtual PMMetaObject* tqmetaObject( ) const;
   /** */
   virtual void cleanUp( ) const;

   /** */
   virtual TQString pixmap( ) const { return TQString( "pmtexturelist" ); }

   static PMMetaObject* s_pMetaObject;
};

/**
 * Class for pigment lists
 */

class PMPigmentList : public PMListPattern
{
public:
   typedef PMListPattern Base;
   /**
    * Creates a pigment list
    */
   PMPigmentList( PMPart* part );
   /**
    * Copy constructor
    */
   PMPigmentList( const PMPigmentList& l );
   /**
    * Deletes the pigment list
    */
   virtual ~PMPigmentList( );

   /** */
   virtual PMObject* copy( ) const { return new PMPigmentList( *this ); }
   /** */
   virtual TQString description( ) const;
   /** */
   virtual TQString listObjectType( ) const { return TQString( "Pigment" ); }

   /** */
   virtual PMMetaObject* tqmetaObject( ) const;
   /** */
   virtual void cleanUp( ) const;

   /** */
   virtual TQString pixmap( ) const { return TQString( "pmpigmentlist" ); }

   static PMMetaObject* s_pMetaObject;
};

/**
 * Class for color lists
 */

class PMColorList : public PMListPattern
{
public:
   typedef PMListPattern Base;
   /**
    * Creates a color list
    */
   PMColorList( PMPart* part );
   /**
    * Copy constructor
    */
   PMColorList( const PMColorList& l );
   /**
    * Deletes the color list
    */
   virtual ~PMColorList( );

   /** */
   virtual PMObject* copy( ) const { return new PMColorList( *this ); }
   /** */
   virtual TQString description( ) const;
   /** */
   virtual TQString listObjectType( ) const { return TQString( "SolidColor" ); }

   /** */
   virtual PMMetaObject* tqmetaObject( ) const;
   /** */
   virtual void cleanUp( ) const;

   /** */
   virtual TQString pixmap( ) const { return TQString( "pmcolorlist" ); }

   static PMMetaObject* s_pMetaObject;
};

/**
 * Class for density lists
 */

class PMDensityList : public PMListPattern
{
public:
   typedef PMListPattern Base;
   /**
    * Creates a density list
    */
   PMDensityList( PMPart* part );
   /**
    * Copy constructor
    */
   PMDensityList( const PMDensityList& l );
   /**
    * Deletes the density list
    */
   virtual ~PMDensityList( );

   /** */
   virtual PMObject* copy( ) const { return new PMDensityList( *this ); }
   /** */
   virtual TQString description( ) const;
   /** */
   virtual TQString listObjectType( ) const { return TQString( "Density" ); }

   /** */
   virtual PMMetaObject* tqmetaObject( ) const;
   /** */
   virtual void cleanUp( ) const;

   /** */
   virtual TQString pixmap( ) const { return TQString( "pmdensitylist" ); }

   static PMMetaObject* s_pMetaObject;
};

/**
 * Class for normal lists
 */

class PMNormalList : public PMListPattern
{
public:
   typedef PMListPattern Base;
   /**
    * Creates a normal list
    */
   PMNormalList( PMPart* part );
   /**
    * Copy constructor
    */
   PMNormalList( const PMNormalList& l );
   /**
    * Deletes the normal list
    */
   virtual ~PMNormalList( );

   /** */
   virtual PMObject* copy( ) const { return new PMNormalList( *this ); }
   /** */
   virtual TQString description( ) const;
   /** */
   virtual TQString listObjectType( ) const { return TQString( "Normal" ); }

   /** */
   virtual PMMetaObject* tqmetaObject( ) const;
   /** */
   virtual void cleanUp( ) const;

   /**
    * Returns a new @ref PMListPatternEdit
    */
   virtual PMDialogEditBase* editWidget( TQWidget* tqparent ) const;

   /** */
   virtual TQString pixmap( ) const { return TQString( "pmnormallist" ); }

   /** */
   virtual void serialize( TQDomElement& e, TQDomDocument& doc ) const;
   /** */
   virtual void readAttributes( const PMXMLHelper& h );

   /** */
   virtual void restoreMemento( PMMemento* s );

   /**
    * Returns the normal's depth
    */
   double depth( ) const { return m_depth; }
   /**
    * Sets the normal's depth
    */
   void setDepth( double d );

private:
   /**
    * IDs for @ref PMMementoData
    */
   enum PMNormalListMementoID { PMDepthID };
   /**
    * Normal Depth
    */
   double m_depth;

   static PMMetaObject* s_pMetaObject;
};

#endif
