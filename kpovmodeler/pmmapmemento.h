/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001 by Andreas Zehender
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


#ifndef PMMAPMEMENTO_H
#define PMMAPMEMENTO_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "pmmemento.h"
#include <tqvaluelist.h>


/**
 * Memento for @ref PMTextureMapBase
 */
class PMMapMemento : public PMMemento
{
public:
   /**
    * Creates a memento for the object originator
    */
   PMMapMemento( PMObject* originator );
   /**
    * Deletes the memento
    */
   virtual ~PMMapMemento( );

   /**
    * Saves the map values
    */
   void setMapValues( const TQValueList<double>& v );
   /**
    * Returns the map values
    */
   TQValueList<double> mapValues( ) const { return m_mapValues; }
   /**
    * Returns true if the map values were saved
    */
   bool mapValuesSaved( ) const { return m_bMapValuesSaved; }
   
   /**
    * Saves the removed values
    */
   void setRemovedValues( const TQValueList<double>& v );
   /**
    * Returns the removed values
    */
   TQValueList<double> removedValues( ) const { return m_removedValues; }
   /**
    * Returns true if the removed values were saved
    */
   bool removedValuesSaved( ) const { return m_bRemovedValuesSaved; }

private:   
   /**
    * The stored values
    */
   TQValueList<double> m_mapValues;
   TQValueList<double> m_removedValues;
   bool m_bMapValuesSaved, m_bRemovedValuesSaved;
};

#endif
