/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2004 by Luis Carvalho
    email                : lpassos@oninetspeed.pt
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/

#ifndef PMDRAGWIDGET_H
#define PMDRAGWIDGET_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tqwidget.h>
#include <tqstring.h>

/**
 * This class implements a widget that exposes a signal for drag
 * and drop.
 *
 * It will emit startDrag when it's time to start a drag.
 */
class PMDragWidget : public TQWidget
{
   Q_OBJECT
  
public:
   PMDragWidget( TQWidget* parent = 0, const char* name = 0, WFlags f = 0 );
   
   virtual void startDrag( );

protected:
   void mousePressEvent( TQMouseEvent* );
   void mouseReleaseEvent( TQMouseEvent* );
   void mouseMoveEvent( TQMouseEvent* );

private:
   bool dragging;
};

#endif
