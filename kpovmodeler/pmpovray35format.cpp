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

#include "pmpovray35format.h"
#include "pmpovray35serialization.h"

#include "pmpovrayparser.h"
#include "pmoutputdevice.h"

#include <tdelocale.h>

PMPovray35Format::PMPovray35Format( )
      : PMPovray31Format( )
{
   registerMethod( "IsoSurface", PMPov35SerIsoSurface );
   registerMethod( "Light", PMPov35SerLight );
   registerMethod( "ProjectedThrough", PMPov35SerProjectedThrough );
   registerMethod( "GlobalSettings", PMPov35SerGlobalSettings );
   registerMethod( "Radiosity", PMPov35SerRadiosity );
   registerMethod( "GlobalPhotons", PMPov35SerGlobalPhotons );
   registerMethod( "Photons", PMPov35SerPhotons );
   registerMethod( "Interior", PMPov35SerInterior );
   registerMethod( "LightGroup", PMPov35SerLightGroup );
   registerMethod( "Pattern", PMPov35SerPattern );
   registerMethod( "Normal", PMPov35SerNormal );
   registerMethod( "InteriorTexture", PMPov35SerInteriorTexture );
   registerMethod( "Warp", PMPov35SerWarp );
   registerMethod( "SphereSweep", PMPov35SerSphereSweep );
   registerMethod( "Finish", PMPov35SerFinish );
   registerMethod( "Mesh", PMPov35SerMesh );
   registerMethod( "Media", PMPov35SerMedia );
   registerMethod( "GraphicalObject", PMPov35SerGraphicalObject );
   registerMethod( "Pigment", PMPov35SerPigment );
   registerMethod( "Texture", PMPov35SerTexture );
   registerMethod( "BicubicPatch", PMPov35SerBicubicPatch );
   registerMethod( "Triangle", PMPov35SerTriangle );
}


PMPovray35Format::~PMPovray35Format( )
{

}

PMParser* PMPovray35Format::newParser( PMPart* part, TQIODevice* dev ) const
{
   return new PMPovrayParser( part, dev );
}

PMParser* PMPovray35Format::newParser( PMPart* part, const TQByteArray& data ) const
{
   return new PMPovrayParser( part, data );
}

PMSerializer* PMPovray35Format::newSerializer( TQIODevice* dev )
{
   return new PMOutputDevice( dev, this );
}

PMRenderer* PMPovray35Format::newRenderer( PMPart* ) const
{
   // TODO
   return 0;
}

TQString PMPovray35Format::mimeType( ) const
{
   return TQString( "text/plain" );
}

TQStringList PMPovray35Format::importPatterns( ) const
{
   TQStringList result;
   result.push_back( TQString( "*.pov *.inc|" )
                     + i18n( "POV-Ray 3.5 Files (*.pov, *.inc)" ) );
   return result;
}

TQStringList PMPovray35Format::exportPatterns( ) const
{
   TQStringList result;
   result.push_back( TQString( "*.pov|" ) + i18n( "POV-Ray 3.5 Files (*.pov)" ) );
   result.push_back( TQString( "*.ini|" ) + i18n( "POV-Ray 3.5 Include Files (*.ini)" ) );
   return result;

}
