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
#include <tqlayout.h>

PMUnknownView::PMUnknownView( const TQString& viewType,
                              TQWidget* tqparent, const char* name )
      : PMViewBase( tqparent, name )
{
   TQHBoxLayout* hl = new TQHBoxLayout( this );
   TQLabel* l;
   l = new TQLabel( i18n( "Unknown view type \"%1\"" ).tqarg( viewType ), this );
   l->tqsetAlignment( TQt::AlignCenter );
   hl->addWidget( l );
   m_viewType = viewType;
}

TQString PMUnknownView::description( ) const
{
   return i18n( "Unknown" );
}
