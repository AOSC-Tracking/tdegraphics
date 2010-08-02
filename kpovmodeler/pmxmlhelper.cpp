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

#include "pmxmlhelper.h"

PMXMLHelper::PMXMLHelper( const TQDomElement& e, PMPart* p, PMParser* par,
                          int majorDocumentFormat, int minorDocumentFormat )
{
   m_e = e;
   m_pPart = p;
   m_pParser = par;
   m_major = majorDocumentFormat;
   m_minor = minorDocumentFormat;
}

bool PMXMLHelper::hasAttribute( const TQString& name ) const
{
   return m_e.hasAttribute( name );
}

int PMXMLHelper::intAttribute( const TQString& name, int def ) const
{
   TQString str = m_e.attribute( name );
   bool ok;
   int res;
   
   if( str.isNull( ) )
      return def;
   res = str.toInt( &ok );
   if( ok )
      return res;
   return def;
}

double PMXMLHelper::doubleAttribute( const TQString& name, double def ) const
{
   TQString str = m_e.attribute( name );
   bool ok;
   double res;
   
   if( str.isNull( ) )
      return def;
   res = str.toDouble( &ok );
   if( ok )
      return res;
   return def;
}

bool PMXMLHelper::boolAttribute( const TQString& name, bool def ) const
{
   TQString str = m_e.attribute( name );
   bool ok;
   int res;
   
   if( str.isNull( ) )
      return def;
   res = str.toInt( &ok );
   if( ok )
      return ( res != 0 );
   return def;
}

PMThreeState PMXMLHelper::threeStateAttribute( const TQString& name ) const
{
   TQString str = m_e.attribute( name );
   bool ok;
   int res;
   
   if( str.isNull( ) )
      return PMUnspecified;
   res = str.toInt( &ok );
   if( ok )
   {
      if( res == 0 )
         return PMFalse;
      else
         return PMTrue;
   }
   return PMUnspecified;
}

TQString PMXMLHelper::stringAttribute( const TQString& name, const TQString& def ) const
{
   return m_e.attribute( name, def );
}

PMVector PMXMLHelper::vectorAttribute( const TQString& name, const PMVector& def ) const
{
   TQString str = m_e.attribute( name );

   if( str.isNull( ) )
      return def;
   else
   {
      PMVector v;
      if( v.loadXML( str ) )
         return v;
   }
   return def;
}

PMMatrix PMXMLHelper::matrixAttribute( const TQString& name, const PMMatrix& def ) const
{
   TQString str = m_e.attribute( name );

   if( str.isNull( ) )
      return def;
   else
   {
      PMMatrix m;
      if( m.loadXML( str ) )
         return m;
   }
   return def;
}

PMColor PMXMLHelper::colorAttribute( const TQString& name, const PMColor& def ) const
{
   TQString str = m_e.attribute( name );

   if( str.isNull( ) )
      return def;
   else
   {
      PMColor c;
      if( c.loadXML( str ) )
         return c;
   }
   return def;
}

TQDomElement PMXMLHelper::extraData( ) const
{
   TQDomNode c = m_e.firstChild( );
   while( !c.isNull( ) )
   {
      if( c.isElement( ) )
      {
         TQDomElement ce = c.toElement( );
         if( ce.tagName( ) == "extra_data" )
            return ce;
      }
      c = c.nextSibling( );
   }
   return TQDomElement( );
}
