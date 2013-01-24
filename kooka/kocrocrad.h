/***************************************************************************
                     kocrocrad.h - ocr dialog for ocrad
                             -------------------
    begin                : Tue Jul 15 2003
    copyright            : (C) 2003 by Klaas Freitag
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


#ifndef KOCROCRAD_H
#define KOCROCRAD_H

#include <kdialogbase.h>
#include <tqimage.h>
#include <tqstring.h>

#include <kscanslider.h>
#include <kanimwidget.h>

#include "kocrbase.h"

#define CFG_GROUP_OCRAD "ocrad"
#define CFG_OCRAD_LAYOUT_DETECTION "layoutDetection"
#define CFG_OCRAD_EXTRA_ARGUMENTS  "extraArguments"
#define CFG_OCRAD_FORMAT "format"
#define CFG_OCRAD_CHARSET "charset"
/**
  *@author Klaas Freitag
  */

class KSpellConfig;
class KURLRequester;
class TDEProcess;
class TQLabel;
class TQComboBox;

class ocradDialog: public KOCRBase
{
    Q_OBJECT
  
public:
    ocradDialog( TQWidget*, KSpellConfig* );
    ~ocradDialog();

    TQString getOCRCmd( void ) const
        { return m_ocrCmd;}

    EngineError setupGui();

    TQString ocrEngineName() const;
    TQString ocrEngineDesc() const;
    TQString ocrEngineLogo() const;

    TQString orfUrl() const;

    int layoutDetectionMode() const;

    /**
     * returns the numeric version of the ocrad program.
     *
     * Attention: This method returns 10 for ocrad v. 0.10 and 8 for ocrad-0.8
     */
    int getNumVersion();

public slots:
    void enableFields(bool);
    void introduceImage( KookaImage* );

protected:
    void writeConfig();


private:
    void version( const TQString& exe );

private slots:
    void slReceiveStdIn( TDEProcess *proc, char *buffer, int buflen);

private:

    TQString      m_ocrCmd;
    KURLRequester *m_orfUrlRequester;
    TQComboBox      *m_layoutMode;
    TQLabel         *m_binaryLabel;
    TDEProcess       *m_proc;
    int             m_version;
};

#endif
