// KPrintDialogPage_PageOptions.h
//
// Part of KVIEWSHELL - A framework for multipage text/gfx viewers
//
// (C) 2005 Stefan Kebekus
// Distributed under the GPL

// Add header files alphabetically

#ifndef KPRINTDIALOGPAGE_DJVUPAGEOPTIONS_H
#define KPRINTDIALOGPAGE_DJVUPAGEOPTIONS_H


#include <tdeprint/kprintdialogpage.h>


class TQVBoxLayout;
class TQCheckBox;


// This is a fairly standard KPrintDialogPage that allows the user to
// chose page size & placement options: shrink oversized pages, and
// expand small pages

class KPrintDialogPage_DJVUPageOptions : public KPrintDialogPage
{
 public:
  KPrintDialogPage_DJVUPageOptions( TQWidget *parent = 0, const char *name = 0 );
  
  void getOptions( TQMap<TQString,TQString>& opts, bool incldef = false );
  void setOptions( const TQMap<TQString,TQString>& opts );
  bool isValid( TQString& msg );

  TQCheckBox* checkBox_rotate;
  TQCheckBox* checkBox_fitpage;

 private:
  TQVBoxLayout* kprintDialogPage_pageoptions_baseLayout;
};


#endif // KPRINTDIALOGPAGE_PAGEOPTIONS_H
