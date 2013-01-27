/***************************************************************************
               thumbviewitem.h  - Thumbnailview items
                             -------------------
    begin                : Tue Apr 24 2002
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

#ifndef __THUMBVIEWITEM_H__
#define __THUMBVIEWITEM_H__

#include <kiconview.h>
#include <kurl.h>
#include <tdeio/previewjob.h>
#include <tdefileitem.h>
#include <tdefileiconview.h>

class KFileTreeViewItem;


class ThumbViewItem: public KFileIconViewItem
{
public:
   ThumbViewItem( TQIconView *parent,
		  const TQString &text,
		  const TQPixmap &pixmap,
		  KFileItem *fi );

    void setItemUrl( const KURL& u );

    KURL itemUrl() const
      { return m_url; }

private:
   KURL m_url;


};

#endif
