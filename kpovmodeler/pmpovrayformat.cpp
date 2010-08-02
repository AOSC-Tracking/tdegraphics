/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Andreas Zehender
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

#include "pmpovrayformat.h"
#include "pmdebug.h"

PMPovrayFormat::PMPovrayFormat( )
{
   m_methodDict.setAutoDelete( true );
}


PMPovrayFormat::~PMPovrayFormat( )
{

}

void PMPovrayFormat::registerMethod( const TQString& className,
                                     PMPovraySerializeMethod method )
{
   PMPovraySerializeMethodInfo* info = m_methodDict.find( className );
   if( info )
      kdWarning( PMArea ) << "Serialization method for " << className
                          << " shadows old implementation" << endl;
   info = new PMPovraySerializeMethodInfo( method );
   m_methodDict.insert( className, info );
}

void PMPovrayFormat::removeMethod( const TQString& className )
{
   m_methodDict.remove( className );
}

const PMPovraySerializeMethodInfo* PMPovrayFormat::serializationMethod(
   const TQString& className )
{
   return m_methodDict.find( className );
}
