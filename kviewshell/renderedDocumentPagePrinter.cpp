
/* This file is part of the KDE project
   Copyright (C) 2005 Stefan Kebekus <kebekus@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include <config.h>

#include <kdebug.h>
#include <tqguardedptr.h>
#include <tqpaintdevicemetrics.h>
#include <kprinter.h>

#include "renderedDocumentPagePrinter.h"

RenderedDocumentPagePrinter::RenderedDocumentPagePrinter(KPrinter *kp)
{
  printer = kp;
  printerPainter = 0;

  if (printer == 0) {
    kdError(1223) << "RenderedDocumentPagePrinter::RenderedDocumentPagePixmap() called with printer == 0" << endl;
    return;
  }

  printerPainter = new TQPainter(printer);
}


TQSize RenderedDocumentPagePrinter::size()
{
  if (printer == 0) {
    kdError(1223) << "RenderedDocumentPagePrinter::size() called with printer == 0" << endl;
    return TQSize(0,0);
  }
  
  TQPaintDeviceMetrics pdm( printer );
  return TQSize(pdm.width(), pdm.height());
}
