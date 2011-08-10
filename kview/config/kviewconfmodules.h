/*  This file is part of the KDE project
    Copyright (C) 2002-2003 Matthias Kretz <kretz@kde.org>

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

#ifndef KVIEWCONFMODULES_H
#define KVIEWCONFMODULES_H

#include <kcmodule.h>

class TQVButtonGroup;

class KViewGeneralConfig : public KCModule
{
	Q_OBJECT
  TQ_OBJECT
	public:
		KViewGeneralConfig( TQWidget * parent, const char * name = 0, const TQStringList & args = TQStringList() );
		~KViewGeneralConfig();

		virtual void load();
		virtual void save();
		virtual void defaults();

	private slots:
		void resizeChanged( int );

	private:
		TQVButtonGroup * m_pResizeGroup;
		int m_savedPosition;
};

// vim:sw=4:ts=4

#endif // KVIEWCONFMODULES_H
