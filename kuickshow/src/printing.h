/* This file is part of the KDE project
   Copyright (C) 2002 Carsten Pfeiffer <pfeiffer@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation, version 2.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef PRINTING_H
#define PRINTING_H

#include <tqfontmetrics.h>
#include <tqstring.h>

#include <tdeprint/kprintdialogpage.h>

class TQCheckBox;
class TQRadioButton;
class KComboBox;
class KPrinter;
class KIntNumInput;

class ImageWindow;


class Printing
{
public:
    static bool printImage( ImageWindow& imageWin, TQWidget *parent = 0L);
    static bool printImageWithTQt( const TQString& filename, KPrinter& printer,
                                  const TQString& originalFileName );

private:
    static void addConfigPages();
    static TQString minimizeString( TQString text, const TQFontMetrics& metrics,
                                   int maxWidth );

};

class KuickPrintDialogPage : public KPrintDialogPage
{
    Q_OBJECT
  

public:
    KuickPrintDialogPage( TQWidget *parent = 0L, const char *name = 0 );
    ~KuickPrintDialogPage();

    virtual void getOptions(TQMap<TQString,TQString>& opts, bool incldef = false);
    virtual void setOptions(const TQMap<TQString,TQString>& opts);

private slots:
    void toggleScaling( bool enable );

private:
    // return values in pixels!
    int scaleWidth() const;
    int scaleHeight() const;

    void setScaleWidth( int pixels );
    void setScaleHeight( int pixels );

    int fromUnitToPixels( float val ) const;
    float pixelsToUnit( int pixels ) const;

    TQCheckBox *m_shrinkToFit;
    TQRadioButton *m_scale;
    KIntNumInput *m_width;
    KIntNumInput *m_height;
    KComboBox *m_units;
    TQCheckBox *m_addFileName;
    TQCheckBox *m_blackwhite;

};

#endif // PRINTING_H
