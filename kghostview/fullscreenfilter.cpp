/**
 * Copyright (C) 2003, Lu�s Pedro Coelho
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
#include "fullscreenfilter.h"

#include "kgvshell.h"
#include "kgv_view.h"
#include "kgv_miniwidget.h"
#include "kgvpageview.h"

FullScreenFilter::FullScreenFilter( KGVShell& tqparent )
	:TQObject( &tqparent, "full-screen-filter" ),
	 tqparent( tqparent )
{
}

bool FullScreenFilter::eventFilter( TQObject* /*object*/, TQEvent* ev) {
	if ( TQKeyEvent* keyevent = dynamic_cast<TQKeyEvent*>( ev ) ) {
		if ( keyevent->key() == Key_Escape ) {
			tqparent.setFullScreen( false );
			keyevent->accept();
			return true;
		}
	}
	if ( TQMouseEvent* mouseevent = dynamic_cast<TQMouseEvent*>( ev ) ) {
		if ( mouseevent->stateAfter() & mouseevent->button() & Qt::LeftButton ) {
			// if ( The whole image is visible at once )
			if ( tqparent.m_gvpart->pageView()->contentsHeight() <= tqparent.m_gvpart->widget()->height() &&
			     tqparent.m_gvpart->pageView()->contentsWidth() <= tqparent.m_gvpart->widget()->width() ) {
				tqparent.m_gvpart->miniWidget()->nextPage();
				mouseevent->accept();
				return true;
			}
		}
	}
	return false;
}

#include "fullscreenfilter.moc"

