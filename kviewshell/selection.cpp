//
// selection.cpp
//
// Part of KDVI - A previewer for TeX DVI files.
//
// (C) 2001--2004 Stefan Kebekus
// Distributed under the GPL

#include <config.h>

#include <tqapplication.h>
#include <tqclipboard.h>

#include "selection.h"

TextSelection::TextSelection()
  : page(PageNumber::invalidPage),
    selectedText(TQString())
{}


void TextSelection::set(const PageNumber& pageNr, TQ_INT32 start, TQ_INT32 end, const TQString& text)
{
  page              = pageNr;
  selectedTextStart = start;
  selectedTextEnd   = end;
  if (page != 0)
    selectedText = text;
  else
    selectedText = TQString();

  if (page != 0) {
    TQApplication::clipboard()->setSelectionMode(true);
    TQApplication::clipboard()->setText(selectedText);
  }
}


bool TextSelection::operator== (const TextSelection& s) const
{
  // We don't check if the strings are equal because for corretly constructed selection objects this is always
  // the case if the following condition holds.
  return (page == s.page && selectedTextStart == s.selectedTextStart && selectedTextEnd == s.selectedTextEnd);
}


bool TextSelection::operator!= (const TextSelection& s) const
{
  return !(*this == s);
}


void TextSelection::copyText() const
{
  if (!isEmpty()) {
    TQApplication::clipboard()->setSelectionMode(false);
    TQApplication::clipboard()->setText(selectedText);
  }
}


void TextSelection::clear()
{
  set(0, -1, -1, TQString());
}

