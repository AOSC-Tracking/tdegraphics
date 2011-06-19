// -*- C++ -*-
// KPrintDialogPage_PageOptions.h
//
// Part of KVIEWSHELL - A framework for multipage text/gfx viewers
//
// (C) 2005 Stefan Kebekus
// Distributed under the GPL

// Add header files alphabetically

#ifndef KPRINTDIALOGPAGE_PAGEOPTIONS_H
#define KPRINTDIALOGPAGE_PAGEOPTIONS_H

#include <kdeprint/kprintdialogpage.h>

class TQCheckBox;
class TQVBoxLayout;


// This is a fairly standard KPrintDialogPage that allows the user to
// chose page size & placement options: center page on paper, shrink
// oversized pages, and expand small pages

class KPrintDialogPage_PageOptions : public KPrintDialogPage
{
 public:
  KPrintDialogPage_PageOptions( TQWidget *tqparent = 0, const char *name = 0 );
  
  void getOptions( TQMap<TQString,TQString>& opts, bool incldef = false );
  void setOptions( const TQMap<TQString,TQString>& opts );
  bool isValid( TQString& msg );

  TQCheckBox* checkBox_center;
  TQCheckBox* checkBox_rotate;
  TQCheckBox* checkBox_shrink;
  TQCheckBox* checkBox_expand;

 private:
  TQVBoxLayout* kprintDialogPage_pageoptions_baseLayout;
};


#endif // KPRINTDIALOGPAGE_PAGEOPTIONS_H
