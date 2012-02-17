/***************************************************************************
                kookaprint.h  -  Printing from the gallery
                             -------------------
    begin                : Tue May 13 2003
    copyright            : (C) 1999 by Klaas Freitag
    email                : freitag@suse.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *  This file may be distributed and/or modified under the terms of the    *
 *  GNU General Public License version 2 as published by the Free Software *
 *  Foundation and appearing in the file COPYING included in the           *
 *  packaging of this file.                                                *
 *
 *  As a special exception, permission is given to link this program       *
 *  with any version of the KADMOS ocr/icr engine of reRecognition GmbH,   *
 *  Kreuzlingen and distribute the resulting executable without            *
 *  including the source code for KADMOS in the source distribution.       *
 *
 *  As a special exception, permission is given to link this program       *
 *  with any edition of TQt, and distribute the resulting executable,       *
 *  without including the source code for TQt in the source distribution.   *
 *                                                                         *
 ***************************************************************************/

#ifndef __KOOKA_PRINT_H__
#define __KOOKA_PRINT_H__

#include <tqobject.h>
#include <tqmap.h>
#include <tqstring.h>
#include <kprinter.h>
#include <tdeprint/kprintdialogpage.h>

class KookaImage;
class KPrinter;
class TQPainter;
class KLineEdit;


class ImageSettings : public KPrintDialogPage
{
public:
    void setOptions( const TQMap<TQString, TQString>& opts );
    void getOptions( TQMap<TQString, TQString>& opts, bool include_def = false );
    bool isValid( TQString& msg );

private:
    KLineEdit *m_width, *m_height;

};


class KookaPrint:public TQObject
{
    Q_OBJECT
  
public:
    KookaPrint(KPrinter*);

    /**
     * The top left edge of the required print position
     */
    virtual TQPoint printPosTopLeft(const TQSize&) const;
    virtual TQPoint printPosTopRight(const TQSize&) const;
    virtual TQPoint printPosBottomLeft(const TQSize&) const;
    virtual TQPoint printPosBottomRight(const TQSize&) const;

    virtual int extraMarginPix() const;

    /**
     * The maximum pixel size of the image (or imagepart) on
     * the current page
     */
    virtual TQSize maxPageSize( int extraShrinkPercent = 0 ) const;

public slots:

    bool printImage( KookaImage* );
    void printFittingToPage(KookaImage *img);
protected:
    typedef enum { SW, NW, NO, SO } MarkerDirection;

    virtual void drawMarkerAroundPoint( const TQPoint& );
    virtual void drawCutSign( const TQPoint&, int, MarkerDirection );
    virtual void drawCornerMarker( const TQSize&, int, int, int, int );

private:

    KPrinter 	*m_printer;
    TQPainter    *m_painter;
    int          m_extraMarginPercent;
};

#endif
