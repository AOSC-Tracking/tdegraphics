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


#ifndef PMFACTORY_H
#define PMFACTORY_H

#include <tdeparts/factory.h>

class TDEAboutData;
class TDEInstance;

class PMFactory : public KParts::Factory
{
   TQ_OBJECT
  
public:
   PMFactory( );
   virtual ~PMFactory( );

   virtual KParts::Part* createPartObject( TQWidget* parentWidget,
                                     const char* widgetName,
                                     TQObject* parent, const char* name,
                                     const char* classname,
                                     const TQStringList& args );
   
   static TDEInstance* instance( );
   static const TDEAboutData* aboutData( );
private:
   static TDEInstance* s_instance;
   static TDEAboutData* s_aboutData;
};

#endif
