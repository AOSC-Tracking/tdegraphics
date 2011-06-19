//-*-C++-*-
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


#ifndef PMBOX_H
#define PMBOX_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "pmsolidobject.h"
#include "pmvector.h"

class PMViewStructure;

/**
 * Class for povray boxes.
 */

class PMBox : public PMSolidObject
{
   typedef PMSolidObject Base;
public:
   /**
    * Creates an empty PMBox
    */
   PMBox( PMPart* part );
   /**
    * Copy constructor
    */
   PMBox( const PMBox& b );
   /**
    * deletes the PMBox
    */
   virtual ~PMBox( );

   /** */
   virtual PMObject* copy( ) const { return new PMBox( *this ); }
   /** */
   virtual TQString description( ) const;

   /** */
   virtual PMMetaObject* tqmetaObject( ) const;

   /** */
   virtual void serialize( TQDomElement& e, TQDomDocument& doc ) const;
   /** */
   virtual void readAttributes( const PMXMLHelper& h );
   /**
    * Returns a new @ref PMBoxEdit
    */
   virtual PMDialogEditBase* editWidget( TQWidget* tqparent ) const;
   /**
    * Returns the name of the pixmap that is displayed in the tree view
    * and dialog view
    */
   virtual TQString pixmap( ) const { return TQString( "pmbox" ); }

   /**
    * Returns Corner_1
    */
   PMVector corner1( ) const { return m_corner1; }
   /**
    * Sets Corner_1
    */
   void setCorner1( const PMVector& p );
   /**
    * Returns Corner_2
    */
   PMVector corner2( ) const { return m_corner2; }
   /**
    * Sets Corner_2
    */
   void setCorner2( const PMVector& p );

   /** */
   virtual void restoreMemento( PMMemento* s );
   /** */
   virtual void controlPoints( PMControlPointList& list );
   /** */
   virtual void controlPointsChanged( PMControlPointList& list );
   /** */
   virtual void cleanUp( ) const;

protected:
   /** */
   virtual bool isDefault( );
   /** */
   virtual void createViewStructure( );
   /** */
   virtual PMViewStructure* defaultViewStructure( ) const;

private:
   /**
    * IDs for @ref PMMementoData
    */
   enum PMBoxMementoID { PMCorner1ID, PMCorner2ID };
   PMVector m_corner1, m_corner2;

   /**
    * The default view structure. It can be shared between boxes
    */
   static PMViewStructure* s_pDefaultViewStructure;
   static PMMetaObject* s_pMetaObject;
};

#endif
