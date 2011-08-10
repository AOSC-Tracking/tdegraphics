/*
 *  This file is part of the KDE libraries
 *  Copyright (c) 2005 Helge Deller <deller@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License version 2 as published by the Free Software Foundation.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 **/

#ifndef KFAX_PRINTSETTINGS_H
#define KFAX_PRINTSETTINGS_H

#include <kdeprint/kprintdialogpage.h>

class TQCheckBox;

#define APP_KFAX_SCALE_FULLPAGE "app-kfax-scalefullpage"
#define APP_KFAX_CENTER_HORZ    "app-kfax-center-horz"
#define APP_KFAX_CENTER_VERT    "app-kfax-center-vert"

class KFAXPrintSettings : public KPrintDialogPage
{
	Q_OBJECT
  TQ_OBJECT
public:
	KFAXPrintSettings(TQWidget *parent = 0, const char *name = 0);
	~KFAXPrintSettings();

	void getOptions(TQMap<TQString,TQString>& opts, bool incldef = false);
	void setOptions(const TQMap<TQString,TQString>& opts);

private:
	TQCheckBox	*m_scaleFullPage;
	TQCheckBox	*m_center_horz;
	TQCheckBox	*m_center_vert;
};

#endif
