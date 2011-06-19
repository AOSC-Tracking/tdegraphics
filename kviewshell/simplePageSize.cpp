// SimplePageSize.cpp
//
// Part of KVIEWSHELL - A framework for multipage text/gfx viewers
//
// (C) 2002-2005 Stefan Kebekus
// Distributed under the GPL

// Add header files alphabetically

#include <config.h>

#include <kdebug.h>
#include <tqpaintdevice.h>

#include "simplePageSize.h"


double SimplePageSize::zoomForHeight(TQ_UINT32 height) const
{
  if (!isValid()) {
    kdError(1223) << "SimplePageSize::zoomForHeight() called when paper height was invalid" << endl;
    return 0.1;
  }
  return (double)(height)/(TQPaintDevice::x11AppDpiY()*(pageHeight.getLength_in_inch()));
}


double SimplePageSize::zoomForWidth(TQ_UINT32 width) const
{
  if (!isValid()) {
    kdError(1223) << "SimplePageSize::zoomForWidth() called when paper width was invalid" << endl;
    return 0.1;
  }
  return (double)(width)/(TQPaintDevice::x11AppDpiX()*(pageWidth.getLength_in_inch()));
}


double SimplePageSize::zoomToFitInto(const SimplePageSize &target) const
{
  if (!isValid() || isSmall() || !target.isValid()) {
    kdWarning(1223) << "SimplePageSize::zoomToFitInto(...) with unsuitable source of target" << endl;
    return 1.0;
  }
  
  double z1 = target.width() / pageWidth;
  double z2 = target.height() / pageHeight;

  return TQMIN(z1,z2);
}
