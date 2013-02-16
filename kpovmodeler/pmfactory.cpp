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


#include <tdelocale.h>
#include <kinstance.h>
#include <tdeaboutdata.h>

#include "pmfactory.h"
#include "pmpart.h"
#include "pmdebug.h"
#include "version.h"

extern "C"
{
   void* init_libkpovmodelerpart( )
   {
      return new PMFactory( );
   }
}

static const char description[] = I18N_NOOP( "Modeler for POV-Ray Scenes" );

TDEInstance* PMFactory::s_instance = 0L;
TDEAboutData* PMFactory::s_aboutData = 0L;

PMFactory::PMFactory( )
{
   kdDebug( ) << "PMFactory::PMFactory( )\n";
}

PMFactory::~PMFactory( )
{
   if( s_instance )
      delete s_instance;
   if( s_aboutData )
      delete s_aboutData;

   s_instance = 0L;
   s_aboutData = 0L;
}

KParts::Part* PMFactory::createPartObject( TQWidget* parentWidget,
                                     const char* widgetName,
                                     TQObject* parent, const char* name,
                                     const char* classname,
                                     const TQStringList& /*args*/ )
{
   kdDebug( ) << "PMFactory: Created new part\n";

   bool readwrite = !( ( strcmp( classname, "Browser/View" ) == 0 )
          || ( strcmp( classname, "KParts::ReadOnlyPart" ) == 0 ) );

   KParts::ReadWritePart *part = new PMPart( parentWidget, widgetName,
                                             parent, name, readwrite );

   return part;
}

TDEInstance* PMFactory::instance( )
{
   if( !s_instance )
      s_instance = new TDEInstance( aboutData( ) );
   return s_instance;
}

const TDEAboutData* PMFactory::aboutData( )
{
   if( !s_aboutData )
   {
      s_aboutData =
         new TDEAboutData( "kpovmodeler", I18N_NOOP( "KPovModeler" ),
                         KPOVMODELER_VERSION, description,
                         TDEAboutData::License_GPL, "(c) 2001-2006, Andreas Zehender" );
      s_aboutData->addAuthor( "Andreas Zehender", 0,
                              "zehender@kde.org", "http://www.azweb.de" );
      s_aboutData->addAuthor( "Luis Passos Carvalho", I18N_NOOP( "Textures" ),
                              "lpassos@mail.telepac.pt" );
      s_aboutData->addAuthor( "Leon Pennington", I18N_NOOP( "POV-Ray 3.5 objects" ),
                              "leon@leonscape.co.uk" );
      s_aboutData->addAuthor( "Philippe Van Hecke", I18N_NOOP( "Some graphical objects" ),
                              "lephiloux@tiscalinet.be" );
      s_aboutData->addAuthor( "Leonardo Skorianez", I18N_NOOP( "Some graphical objects" ),
                              "skorianez@bol.com.br" );
   }
   return s_aboutData;
}
#include "pmfactory.moc"
