/***************************************************************************
                          kookapref.h  - Preferences
                             -------------------
    begin                : Sun Jan 16 2000
    copyright            : (C) 2000 by Klaas Freitag
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
#ifndef KOOKAPREF_H
#define KOOKAPREF_H

#include <kdialogbase.h>
#include <tqframe.h>

class TDEConfig;
class TQLabel;
class KIntNumInput;
class KColorButton;
class ImageSelectLine;
class KScanEntry;
class TQRadioButton;
class KURLRequester;
class TQCheckBox;

#define STARTUP_READ_IMAGE "ReadImageOnStart"
#define CFG_GROUP_OCR_DIA  "ocrDialog"
#define CFG_OCRAD_BINARY   "ocradBinary"
#define CFG_GOCR_BINARY    "gocrBinary"

class KookaPreferences : public KDialogBase
{
    TQ_OBJECT
  
public:
    KookaPreferences();
    static TQString tryFindGocr( void );
    static TQString tryFindBinary( const TQString&, const TQString& );

public slots:
    void slotOk( void );
    void slotApply( void );
    void slotDefault( void );

private slots:
    bool checkOCRBinIntern( const TQString&, const TQString&, bool );

    void slCheckOnGOCR( const TQString& );
    void slCheckOnOCRAD( const TQString& );

signals:
    void dataSaved();

private:
    void setupStartupPage();
    void setupSaveFormatPage();
    void setupThumbnailPage();
    void setupOCRPage();
    KURLRequester* binaryCheckBox( TQWidget *, const TQString& );

    TQCheckBox *cbNetQuery;
    TQCheckBox *cbSkipFormatAsk;
    TQCheckBox *cbFilenameAsk;
    TQCheckBox *cbShowScannerSelection;
    TDEConfig   *konf;
    TQCheckBox *cbReadStartupImage;

    KIntNumInput *m_thumbWidth;
    KIntNumInput *m_thumbHeight;
    KIntNumInput *m_frameWidth;
    ImageSelectLine *m_tileSelector;
    KColorButton *m_colButt1;
    KColorButton *m_colButt2;

    KURLRequester *m_urlReqGocr;
    KURLRequester *m_urlReqOcrad;

    TQRadioButton *m_gocrBut;
    TQRadioButton *m_kadmosBut;
    TQRadioButton *m_ocradBut;
    TQString       m_prevOCREngine;
};


#endif // KOOKAPREF_H
