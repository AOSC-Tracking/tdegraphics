/***************************************************************************
 *   Copyright (C) 2004 by Enrico Ros <eros.kde@email.it>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <tdelocale.h>
#include <tqcheckbox.h>

// single config pages
#include "dlggeneral.h"
#include "dlgperformance.h"
#include "dlgaccessibility.h"
#include "dlgpresentation.h"

// reimplementing this
#include "preferencesdialog.h"

PreferencesDialog::PreferencesDialog( TQWidget * parent, TDEConfigSkeleton * skeleton )
  : TDEConfigDialog( parent, "preferences", skeleton ),
    m_shellMode(false)
{
  m_general = new DlgGeneral(nullptr);
  m_performance = new DlgPerformance(nullptr);
  m_accessibility = new DlgAccessibility(nullptr);
  m_presentation = new DlgPresentation(nullptr);

  addPage(m_general, i18n("General"), "kpdf", i18n("General Options"));
  addPage(m_accessibility, i18n("Accessibility"), "access", i18n("Reading Aids"));
  addPage(m_performance, i18n("Performance"), "launch", i18n("Performance Tuning"));
  addPage(m_presentation, i18n("Presentation"), "application-x-kpresenter",
          i18n("Options for Presentation Mode"));

  setShellMode(m_shellMode);
}

void PreferencesDialog::setShellMode(bool on)
{
  m_shellMode = on;
  m_general->kcfg_TabsHoverCloseButton->setHidden(!m_shellMode);
  m_general->kcfg_OpenInExistingKPDF->setHidden(!m_shellMode);
}
