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

#include "pmunknownview.h"
#include <klocale.h>
#include <layout.h>

PMUnknownView::PMUnknownView( const TQString& viewType,
                              TQWidget* parent, const char* name )
      : PMViewBase( parent, name )
{
   TQHBoxLayout* hl = new TQHBoxLayout( this );
   TQLabel* l;
   l = new TQLabel( i18n( "Unknown view type \"%1\"" ).arg( viewType ), this );
   l->setAlignment( TQt::AlignCenter );
   hl->addWidget( l );
   m_viewType = viewType;
}

TQString PMUnknownView::description( ) const
{
   return i18n( "Unknown" );
}
