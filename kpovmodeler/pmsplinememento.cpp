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

#include "pmsplinememento.h"
#include "pmdebug.h"

PMSplineMemento::PMSplineMemento( PMObject* originator )
      : PMMemento( originator )
{
   m_bSplinePointsSaved = false;
}

PMSplineMemento::~PMSplineMemento( )
{
}

void PMSplineMemento::setSplinePoints( const TQValueList<PMVector>& v )
{
   if( !m_bSplinePointsSaved )
   {
      // Direct assignment does not work with TQt 2.3.x
      // The list will be changed later in a graphical
      // change because TQValueList::detach( ) is called
      // too late!
      // Copy the list by hand.

      TQValueList<PMVector>::ConstIterator it = v.begin( );
      for( ; it != v.end( ); ++it )
         m_splinePoints.append( *it );

      m_bSplinePointsSaved = true;
      addChange( PMCData );
   }
}

TQValueList<PMVector> PMSplineMemento::splinePoints( ) const
{
   if( !m_bSplinePointsSaved )
      kdError( PMArea ) << "Spline points not saved in PMSplineMemento::splinePoints\n";
   
   return m_splinePoints;
}

   
