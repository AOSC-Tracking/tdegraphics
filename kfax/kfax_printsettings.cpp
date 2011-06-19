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

#include "kfax_printsettings.h"

#include <klocale.h>
#include <tqcheckbox.h>
#include <tqlayout.h>
#include <tqwhatsthis.h>

KFAXPrintSettings::KFAXPrintSettings(TQWidget *tqparent, const char *name)
: KPrintDialogPage(tqparent, name)
{
	TQString whatsThisScaleFullPage = i18n( "<qt>"
		"<p><strong>'Ignore Paper Margins'</strong></p>"
		"<p>"
		"If this checkbox is enabled, the paper margins will be ignored "
		"and the fax will be printed on the full paper size."
		"</p>"
		"<p>"
		"If this checkbox is disabled, KFax will respect the standard paper "
		"margins and print the fax inside this printable area."
		"</p>"
						" </qt>" );
	TQString whatsThisCenterHorz = i18n( "<qt>"
		"<p><strong>'Horizontal centered'</strong></p>"
		"<p>"
		"If this checkbox is enabled, the fax will be centered horizontally "
		"on the page."
		"</p>"
		"<p>"
		"If this checkbox is disabled, the fax will be printed at the left "
		"side of the page."
		"</p>"
						" </qt>" );
	TQString whatsThisCenterVert = i18n( "<qt>"
		"<p><strong>'Vertical centered'</strong></p>"
		"<p>"
		"If this checkbox is enabled, the fax will be centered vertically "
		"on the page."
		"</p>"
		"<p>"
		"If this checkbox is disabled, the fax will be printed at the top "
		"of the page."
		"</p>"
						" </qt>" );

	setTitle(i18n("&Layout"));

	m_scaleFullPage = new TQCheckBox(i18n("Ignore paper margins"), this);
	TQWhatsThis::add(m_scaleFullPage, whatsThisScaleFullPage);
	m_center_horz = new TQCheckBox(i18n("Horizontal centered"), this);
	TQWhatsThis::add(m_center_horz, whatsThisCenterHorz);
	m_center_vert = new TQCheckBox(i18n("Vertical centered"), this);
	TQWhatsThis::add(m_center_vert, whatsThisCenterVert);

	TQVBoxLayout *l0 = new TQVBoxLayout(this, 0, 10);
	l0->addWidget(m_scaleFullPage);
	l0->addWidget(m_center_horz);
	l0->addWidget(m_center_vert);
	l0->addStretch(1);
}

KFAXPrintSettings::~KFAXPrintSettings()
{
}

void KFAXPrintSettings::getOptions(TQMap<TQString,TQString>& opts, bool /*incldef*/)
{
	opts[APP_KFAX_SCALE_FULLPAGE] = (m_scaleFullPage->isChecked() ? "true" : "false");
	opts[APP_KFAX_CENTER_HORZ]    = (m_center_horz->isChecked() ? "true" : "false");
	opts[APP_KFAX_CENTER_VERT]    = (m_center_vert->isChecked() ? "true" : "false");
}

void KFAXPrintSettings::setOptions(const TQMap<TQString,TQString>& opts)
{
	m_scaleFullPage->setChecked(opts[APP_KFAX_SCALE_FULLPAGE] == "true");
	m_center_horz->setChecked(opts[APP_KFAX_CENTER_HORZ] != "false");
	m_center_vert->setChecked(opts[APP_KFAX_CENTER_VERT] == "true");
}

#include "kfax_printsettings.moc"
