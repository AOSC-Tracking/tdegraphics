/***************************************************************************
                imageselectline.h - select a background image.
                             -------------------
    begin                : ?
    copyright            : (C) 2002 by Klaas Freitag
    email                : freitag@suse.de

    $Id$
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

#ifndef __IMGSELECTLINE_H__
#define __IMGSELECTLINE_H__

#include <tqhbox.h>

/**
 *
 */

class KURL;
class KURLComboBox;
class TQPushButton;
class TQStringList;

class ImageSelectLine:public TQHBox
{
   Q_OBJECT
  
public:
   ImageSelectLine( TQWidget *parent, const TQString& text );

   KURL selectedURL() const;
   void setURL( const KURL& );
   void setURLs( const TQStringList& );

protected slots:
   void slSelectFile();
   void slUrlActivated( const KURL& );

private:

   KURL m_currUrl;
   KURLComboBox *m_urlCombo;
   TQPushButton  *m_buttFileSelect;

};


#endif
