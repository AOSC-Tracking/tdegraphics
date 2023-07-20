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

#ifndef PREVIEWER_H
#define PREVIEWER_H

#include <tqwidget.h>
#include <tqimage.h>
#include <tqrect.h>
#include <tqbuttongroup.h>
#include <tqpoint.h>

#include <kruler.h>
#include <tqmemarray.h>

/**
  *@author Klaas Freitag
  */
class ImageCanvas;
class TQCheckBox;
class TQSlider;
class KScanDevice;
class TQComboBox;
class TQRadioButton;
class TQHBoxLayout;

class Previewer : public TQWidget
{
    TQ_OBJECT
  
public:
    Previewer(TQWidget *parent=0, const char *name=0);
    ~Previewer();

    ImageCanvas *getImageCanvas( void ){ return( img_canvas ); }

    /**
     * Static function that returns the image gallery base dir.
     */
    static TQString galleryRoot();
    bool setPreviewImage( const TQImage &image );
    void findSelection();

public slots:
    void newImage( TQImage* );
    void slFormatChange( int id );
    void slOrientChange(int);
    void slSetDisplayUnit( KRuler::MetricStyle unit );
    void setScanSize( int w, int h, KRuler::MetricStyle unit );
    void slCustomChange( void );
    void slNewDimen(TQRect r);
    void slNewScanResolutions( int, int );
    void recalcFileSize( void );
    void slSetAutoSelThresh(int);
    void slSetAutoSelDustsize(int);
    void slSetScannerBgIsWhite(bool b);
    void slConnectScanner( KScanDevice *scan );
protected slots:
    void slScaleToWidth();
    void slScaleToHeight();
    void slAutoSelToggled(bool);
    void slScanBackgroundChanged(int);

signals:
    void newRect( TQRect );
    void noRect( void );
    void setScanWidth(const TQString&);
    void setScanHeight(const TQString&);
    void setSelectionSize( long );

private:
    void checkForScannerBg();

    TQPoint calcPercent( int, int );

    TQHBoxLayout *layout;
    ImageCanvas *img_canvas;
    TQComboBox   *pre_format_combo;
    TQMemArray<TQCString> format_ids;
    TQButtonGroup * bgroup;
    TQRadioButton * rb1;
    TQRadioButton * rb2;
    TQImage       m_previewImage;

    bool imagePiece( TQMemArray<long> src,
                     int& start,
                     int& end );

    int landscape_id, portrait_id;

    double overallWidth, overallHeight;
    KRuler::MetricStyle sizeUnit;
    KRuler::MetricStyle displayUnit;
    bool isCustom;

    int  scanResX, scanResY;
    int  pix_per_byte;
    double selectionWidthMm;
    double selectionHeightMm;

    class PreviewerPrivate;
    PreviewerPrivate *d;
};

#endif
