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

#ifndef PMPOVRAY35_FORMAT_H
#define PMPOVRAY35_FORMAT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "pmpovray31format.h"

/**
 * Description class for POV-Ray 3.5
 */
class PMPovray35Format : public PMPovray31Format
{
public:
   /**
    * Default constructor
    */
   PMPovray35Format( );
   /**
    * Destructor
    */
   virtual ~PMPovray35Format( );

   /** */
   virtual TQString name( ) const { return "povray35"; };
   /** */
   virtual TQString description( ) const { return "POV-Ray 3.5"; }
   /** */
   virtual int services( ) const { return AllServices; }
   /** */
   virtual PMParser* newParser( PMPart*, TQIODevice* ) const;
   /** */
   virtual PMParser* newParser( PMPart*, const TQByteArray& ) const;
   /** */
   virtual PMSerializer* newSerializer( TQIODevice* );
   /** */
   virtual PMRenderer* newRenderer( PMPart* ) const;
   /** */
   virtual TQString mimeType( ) const;
   /** */
   virtual TQStringList importPatterns( ) const;
   /** */
   virtual TQStringList exportPatterns( ) const;
};

#endif
