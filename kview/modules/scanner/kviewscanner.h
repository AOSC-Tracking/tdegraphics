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

#ifndef __kviewscanner_h
#define __kviewscanner_h

#include <kparts/plugin.h>
#include <klibloader.h>

class KURL;
class KScanDialog;
namespace KImageViewer { class Viewer; }

class KViewScanner : public KParts::Plugin
{
	Q_OBJECT
  TQ_OBJECT
public:
	KViewScanner( TQObject* tqparent, const char* name, const TQStringList & );
	virtual ~KViewScanner();

//public slots:

private slots:
	void slotScan();
	void slotImgScanned( const TQImage & );

private:
	KScanDialog * m_pScandlg;
	KImageViewer::Viewer * m_pViewer;
};

// vim:sw=4:ts=4:cindent
#endif
