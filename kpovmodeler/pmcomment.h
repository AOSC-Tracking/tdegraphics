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


#ifndef PMCOMMENT_H
#define PMCOMMENT_H

#include "pmobject.h"
#include <tqstring.h>


/**
 * Class for povray comments
 */

class PMComment : public PMObject
{
   typedef PMObject Base;
public:
   /**
    * Creates an enpty comment
    */
   PMComment( PMPart* part );
   /**
    * Creates a comment with text t
    */
   PMComment( PMPart* part, const TQString& t );
   /**
    * Copy constructor
    */
   PMComment( const PMComment& c );
   /**
    * Deletes the comment
    */
   ~PMComment( );

   /**
    * Sets the comment text
    */
   void setText( const TQString& text );
   /**
    * Returns the comment text
    */
   TQString text( ) const { return m_text; }

   /** */
   virtual PMObject* copy( ) const { return new PMComment( *this ); }

   /** */
   virtual PMMetaObject* tqmetaObject( ) const;
   /** */
   virtual void cleanUp( ) const;

   /** */
   virtual TQString description( ) const;

   /** */
   virtual void serialize( TQDomElement& e, TQDomDocument& doc ) const;
   /** */
   virtual void readAttributes( const PMXMLHelper& h );

   /** */
   virtual PMDialogEditBase* editWidget( TQWidget* parent ) const;
   /** */
   virtual TQString pixmap( ) const { return TQString( "pmcomment" ); }
   /** */
   virtual void restoreMemento( PMMemento* s );
private:
   /**
    * IDs for @ref PMMementoData
    */
   enum PMCommentMementoID { PMTextID };
   TQString m_text;
   static PMMetaObject* s_pMetaObject;
};

#endif
