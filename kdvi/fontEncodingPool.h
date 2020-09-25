// fontEncodingPool.h
//
// Part of KDVI - A DVI previewer for the KDE desktop environemt 
//
// (C) 2003 Stefan Kebekus
// Distributed under the GPL

#ifndef _FONTENCODINGPOOL_H
#define _FONTENCODINGPOOL_H

#include "fontEncoding.h"

#include <tqdict.h>

class TQString;


class fontEncodingPool {
 public:
  fontEncodingPool();

  fontEncoding *findByName(const TQString &name);

 private:
  TQDict<fontEncoding> dictionary;
};

#endif
