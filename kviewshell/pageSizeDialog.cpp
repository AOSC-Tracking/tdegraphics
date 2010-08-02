// pageSizeDialog.cpp
//
// Part of KVIEWSHELL - A framework for multipage text/gfx viewers
//
// (C) 2002-2003 Stefan Kebekus
// Distributed under the GPL

// Add header files alphabetically

#include <config.h>

#include <kapplication.h>
#include <kcombobox.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <tqlabel.h>
#include <tqlayout.h>
#include <tqlineedit.h>
#include <tqvalidator.h>
#include <tqhbox.h>



#include "pageSizeDialog.h"
#include "pageSizeWidget.h"


pageSizeDialog::pageSizeDialog( TQWidget *parent, pageSize *userPrefdPageSize, const char *name, bool modal)
  :KDialogBase( parent, name, modal, i18n("Page Size"), Ok|Apply|Cancel, Ok, 
		true )
{
  userPreferredPageSize = userPrefdPageSize;
  pageSizeW = new pageSizeWidget(this, "PageSizeWidget");
  pageSizeW->setSizePolicy( TQSizePolicy( (TQSizePolicy::SizeType)3, (TQSizePolicy::SizeType)3, 0, 0, 
					 pageSizeW->sizePolicy().hasHeightForWidth() ) );
  setMainWidget(pageSizeW);
}


void pageSizeDialog::slotOk()
{
  if (userPreferredPageSize != 0)
    *userPreferredPageSize = pageSizeW->pageSizeData();
  accept();
}


void pageSizeDialog::slotApply()
{
  if (userPreferredPageSize != 0)
    *userPreferredPageSize = pageSizeW->pageSizeData();
}


void pageSizeDialog::setPageSize(const TQString& name)
{
  if (pageSizeW == 0)
    return;
  pageSizeW->setPageSize(name);
}


#include "pageSizeDialog.moc"
