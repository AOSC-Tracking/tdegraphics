// -*- C++ -*-
#ifndef kmultipageINTERFACE_H
#define kmultipageINTERFACE_H
 
#include <dcopobject.h>
 
class kmultipageInterface : virtual public DCOPObject
{
  K_DCOP
       
  k_dcop:
   virtual ASYNC   jumpToReference(const TQString& reference) = 0;
   virtual TQString name_of_current_file() = 0;
   virtual bool    is_file_loaded(const TQString& filename) = 0;
};
 
#endif

