/** 
 * Copyright (C) 2001-2002 the KGhostView authors. See file AUTHORS.
 * 	
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "kgvmainwidget.h"
#include <kurl.h>
#include <kurldrag.h>

KGVMainWidget::KGVMainWidget( TQWidget* parent, const char* name )
  : TQWidget( parent, name ) {}

void KGVMainWidget::keyPressEvent( TQKeyEvent* event )
{
    if( event->key() == Key_Space && event->state() != ShiftButton ) {
	event->accept();
	emit spacePressed();
    }
}

void KGVMainWidget::dropEvent( TQDropEvent* ev )
{
    KURL::List lst;
    if (  KURLDrag::decode(  ev, lst ) ) {
	emit urlDropped( lst.first() );
    }
}


void KGVMainWidget::dragEnterEvent( TQDragEnterEvent * ev )
{
    ev->accept();
}

#include "kgvmainwidget.moc"
