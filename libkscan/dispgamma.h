/* This file is part of the KDE Project
   Copyright (C) 2000 Klaas Freitag <freitag@suse.de>  

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

#ifndef DISPGAMMA_H
#define DISPGAMMA_H

#include <tqwidget.h>
#include <tqsizepolicy.h>
#include <tqsize.h>
#include <tqmemarray.h>

extern "C"{
#include <sane/sane.h>
#include <sane/saneopts.h>
}
/**
  *@author Klaas Freitag
  */

class DispGamma : public TQWidget  {
    Q_OBJECT
  TQ_OBJECT
public: 
    DispGamma( TQWidget *tqparent );
    ~DispGamma();

    TQSize tqsizeHint( void );
    TQSizePolicy sizePolicy( void );

    void setValueRef( TQMemArray<SANE_Word> *newVals )
    {
        vals = newVals;
    }
protected:
    void paintEvent (TQPaintEvent *ev );
    void resizeEvent( TQResizeEvent* );

private:

    TQMemArray<SANE_Word> *vals;
    int margin;

   class DispGammaPrivate;
   DispGammaPrivate *d;
};

#endif
