/*  This file is part of the KDE project
    Copyright (C) 2002 Matthias Kretz <kretz@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

*/

// $Id$

#ifndef IMAGELISTITEM_H
#define IMAGELISTITEM_H

#include <tdelistview.h>
#include <kurl.h>
#include <tqstring.h>

class TQImage;

class ImageListItem : public TDEListViewItem
{
	public:
		ImageListItem( TDEListView * parent, const KURL & url );
		~ImageListItem();

		const TQImage * image() const;
		const TQString & file() const;
		const KURL & url() const;

		virtual int rtti() const { return 48294; }

	private:
		TQImage * m_pImage;
		TQString m_filename;
		KURL m_url;
};

// vim:sw=4:ts=4
#endif // IMAGELISTITEM_H
