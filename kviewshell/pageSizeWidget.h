// -*- C++ -*-
// pageSizeWidget.h
//
// Part of KVIEWSHELL - A framework for multipage text/gfx viewers
//
// (C) 2002 Stefan Kebekus
// Distributed under the GPL

// Add header files alphabetically

#ifndef PAGESIZEWIDGET_H
#define PAGESIZEWIDGET_H

#include "pageSize.h"
#include "pageSizeWidget_base.h"

class TQDoubleValidator;

class pageSizeWidget : public pageSizeWidget_base
{
  Q_OBJECT

public:
  pageSizeWidget( TQWidget* parent = 0, const char* name = 0, WFlags fl = 0 );

  // Sets the page size. If the dialog is already shown, updates all
  // the GUI. Accepts the same strings as input as the setPageSize()
  // method of the pageSize() class.
  void setPageSize(const TQString&);

  void setOrientation(int ori);

  const pageSize &pageSizeData() const {return chosenSize;}

protected slots:
  void paperSize(int);
  void fillTextFields();

  // Dummy function, for convenience. Ignores the argument and calls
  // the fillTextFields() slot.
  void unitsChanged(int);
  void orientationChanged(int = 0);
  void input(const TQString &);

private:
  pageSize           chosenSize;

  TQDoubleValidator  *widthValidator;
  TQDoubleValidator  *heightValidator;
};

#endif // PAGESIZEWIDGET_H
