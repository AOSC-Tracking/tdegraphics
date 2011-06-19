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

#ifndef PMUNKNOWNVIEW_H
#define PMUNKNOWNVIEW_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tqlabel.h>
#include "pmviewbase.h"

/**
 * Helper view for unknown view types
 */
class PMUnknownView : public PMViewBase
{
public:
   /**
    * Default constructor
    */
   PMUnknownView( const TQString& viewType, TQWidget* tqparent,
                  const char* name = 0 );
   /** */
   virtual TQString viewType( ) const { return m_viewType; }
   /** */
   virtual TQString description( ) const;
private:
   TQString m_viewType;
};

#endif
