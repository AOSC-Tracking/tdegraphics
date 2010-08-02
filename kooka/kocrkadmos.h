/***************************************************************************
                kocrkadmos.h - ocr dialog for KADMOS ocr engine
                             -------------------
    begin                : Sun Jun 11 2000
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
 *  with any edition of Qt, and distribute the resulting executable,       *
 *  without including the source code for Qt in the source distribution.   *
 *                                                                         *
 ***************************************************************************/


#ifndef KOCRKADMOS_H
#define KOCRKADMOS_H

#include <kdialogbase.h>
#include <tqmap.h>

#include "kocrbase.h"
/**
  *@author Klaas Freitag
  */



class KScanCombo;
class TQWidget;
class TQButtonGroup;
class KConfig;
class TQCheckBox;
class KSpellConfig;
class TQRadioButton;

class KadmosClassifier   /* Not yet used FIXME */
{
public:
   KadmosClassifier( TQString lang, TQString filename );
   TQString getCmplFilename() const { return path+filename; }
   TQString getFilename()     const { return filename; }
   TQString language()        const { return languagesName; }

   void setPath( const TQString& p ) { path=p; }
private:

   TQString filename;
   TQString path;
   TQString languagesName;
};


class KadmosDialog: public KOCRBase
{
    Q_OBJECT
public:
    KadmosDialog( TQWidget *, KSpellConfig *spellConfig );
    ~KadmosDialog();

    typedef TQMap<TQString, TQString> StrMap;

    EngineError setupGui();
    bool getAutoScale();
    bool getNoiseReduction();
    bool getSelClassifier(TQString&) const;
    TQString getSelClassifierName() const;

    TQString ocrEngineName() const;
    TQString ocrEngineDesc() const;
    TQString ocrEngineLogo() const;

public slots:
    void enableFields(bool);

protected:
    void writeConfig();

    void setupPreprocessing( TQVBox *box );
    void setupSegmentation(  TQVBox *box );
    void setupClassification( TQVBox *box );

    EngineError findClassifiers();
    EngineError findClassifierPath();
private slots:

    void slFontChanged( int id );

private:
    StrMap                m_classifierTranslate;

    TQCheckBox             *m_cbNoise;
    TQCheckBox             *m_cbAutoscale;
    TQString                m_customClassifierPath;

    QButtonGroup	  *m_bbFont;

    TQRadioButton          *m_rbMachine;
    TQRadioButton          *m_rbHand;
    TQRadioButton          *m_rbNorm;

    TQGroupBox      	  *m_gbLang;

    TQComboBox             *m_cbLang;

    TQStringList            m_ttfClassifier;
    TQStringList            m_handClassifier;
    TQStringList            m_classifierPath;

    bool                   m_haveNorm;

    typedef TQMap<TQString, TQString> StringMap;
    StringMap m_longCountry2short;
};

#endif
