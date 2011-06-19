/***************************************************************************
 *   Copyright (C) 2005 by Wilfried Huss                                   *
 *   Wilfried.Huss@gmx.at                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
 ***************************************************************************/

#include <config.h>

#include <tqlabel.h>
#include <tqpushbutton.h>
#include <tqcheckbox.h>
#include <tqlayout.h>
#include <tqtooltip.h>
#include <tqwhatsthis.h>

#include <kdebug.h>
#include <kiconloader.h>
#include <kglobalsettings.h>
#include <klocale.h>
#include <klineedit.h>


#include "searchWidget.h"

SearchWidget::SearchWidget(TQWidget* tqparent, const char* name, WFlags fl)
  : TQWidget(tqparent, name, fl)
{
  setName("SearchWidget");

  tqsetSizePolicy(TQSizePolicy::MinimumExpanding, TQSizePolicy::Fixed);

  tqlayout = new TQHBoxLayout(this, 4, 6, "SearchWidgetLayout");

  stopButton = new TQPushButton(this, "stopButton");
  stopButton->setPixmap(KGlobal::iconLoader()->loadIcon("stop", KIcon::Small, KIcon::SizeSmall));
  tqlayout->addWidget(stopButton);

  searchLabel = new TQLabel(this, "searchLabel");
  searchLabel->setText(i18n("Search:"));
  tqlayout->addWidget(searchLabel);

  searchText = new KLineEdit(this, "searchText");
  tqlayout->addWidget(searchText);

  searchLabel->setBuddy(searchText);

  findPrevButton = new TQPushButton(this, "findPrevButton");
  findPrevButton->setPixmap(KGlobal::iconLoader()->loadIcon("back", KIcon::NoGroup, KIcon::SizeSmall));
  TQToolTip::add(findPrevButton, i18n("Find previous"));
  tqlayout->addWidget(findPrevButton);

  findNextButton = new TQPushButton(this, "findNextButton");
  findNextButton->setPixmap(KGlobal::iconLoader()->loadIcon("forward", KIcon::NoGroup, KIcon::SizeSmall));
  TQToolTip::add(findNextButton, i18n("Find next"));
  tqlayout->addWidget(findNextButton);

  caseSensitiveCheckBox = new TQCheckBox(this, "caseSensitiveCheckBox");
  caseSensitiveCheckBox->setText(i18n("Case sensitive"));
  tqlayout->addWidget(caseSensitiveCheckBox);

  connect(stopButton, TQT_SIGNAL(clicked()), this, TQT_SIGNAL(stopSearch()));

  connect(findNextButton, TQT_SIGNAL(clicked()), this, TQT_SIGNAL(findNextText()));
  connect(findPrevButton, TQT_SIGNAL(clicked()), this, TQT_SIGNAL(findPrevText()));

  connect(searchText, TQT_SIGNAL(textChanged(const TQString&)), this, TQT_SLOT(textChanged()));

  textChanged();
}

SearchWidget::~SearchWidget()
{
}

TQString SearchWidget::getText() const
{
  return searchText->text();
}

bool SearchWidget::caseSensitive() const
{
  return caseSensitiveCheckBox->isChecked();
}

void SearchWidget::textChanged()
{
  bool empty = searchText->text().isEmpty();

  findNextButton->setDisabled(empty);
  findPrevButton->setDisabled(empty);
  emit searchEnabled(!empty);
}

void SearchWidget::show()
{
  searchText->setEnabled(true);
  searchText->selectAll();
  TQWidget::show();
  emit searchEnabled(!searchText->text().isEmpty());
}

void SearchWidget::hide()
{
  searchText->setEnabled(false);
  TQWidget::hide();
}

void SearchWidget::setFocus()
{
  searchText->setFocus();
}

void SearchWidget::keyPressEvent(TQKeyEvent* e)
{
  if (e->key() == TQt::Key_Escape)
    emit stopSearch();

  if (e->key() == TQt::Key_Return || e->key() == TQt::Key_Enter)
  {
    if (e->state() == TQt::ShiftButton)
      emit findPrevText();
    else
      emit findNextText();
  }
}

#include "searchWidget.moc"
