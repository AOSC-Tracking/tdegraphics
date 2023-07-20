/***************************************************************************
               thumbview.h  - Class to display thumbnailed images
                             -------------------
    begin                : Tue Apr 18 2002
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

#ifndef __THUMBVIEW_H__
#define __THUMBVIEW_H__

#include <tqwidget.h>
#include <tqimage.h>
#include <tqpixmap.h>
#include <tqcolor.h>
#include <tqvbox.h>

#include <kiconview.h>
#include <kurl.h>
#include <tdeio/previewjob.h>
#include <tdefileitem.h>
#include <tdefileiconview.h>

/* TDEConfig group definitions */
#define MARGIN_COLOR1 "MarginColor1"
#define MARGIN_COLOR2 "MarginColor2"
#define PIXMAP_WIDTH  "pixmapWidth"
#define PIXMAP_HEIGHT "pixmapHeight"
#define THUMB_MARGIN  "thumbnailMargin"
#define THUMB_GROUP   "thumbnailView"
#define BG_WALLPAPER  "BackGroundTile"
#define STD_TILE_IMG  "kooka/pics/thumbviewtile.png"

class TQPixmap;
class TQListViewItem;
class KProgress;
namespace TDEIO {
    class PreviewJob;
}

class ThumbView: public TQVBox /* TDEIconView */
{
   TQ_OBJECT
  

public:

   ThumbView( TQWidget *parent, const char *name=0 );
   ~ThumbView();

   void setCurrentDir( const KURL& s)
      { m_currentDir = s; }
   KURL currentDir( ) const
      { return m_currentDir; }

   TQSize tumbSize( ) const
      {
	 return( TQSize( m_pixWidth, m_pixHeight ));
      }

   int thumbMargin() const
      {
	 return m_thumbMargin;
      }
public slots:
   void slSetThumbSize( int w, int h )
      {
	 m_pixWidth  = w;
	 m_pixHeight = h;
      }
   void slSetThumbSize( const TQSize& s )
      {
	 m_pixWidth  = s.width();
	 m_pixHeight = s.height();
      }

   void slSetThumbMargin( int m )
      {
	 m_thumbMargin = m;
      }

   void slNewFileItems( const KFileItemList& );
   void slGotPreview( const KFileItem*, const TQPixmap& );
   void slPreviewResult( TDEIO::Job* );

   /**
    *  This connects to the IconView's executed signal and tells the packager
    *  to select the image
    */
   void slDoubleClicked( TQIconViewItem* );

   /**
    *  indication that a image changed, needs to be reloaded.
    */
   void slImageChanged( KFileItem * );
   void slImageDeleted( KFileItem * );
   void slSetBackGround( );
   void slCheckForUpdate( KFileItem* );
   bool readSettings();
   void clear();

    void slImageRenamed( KFileItem*, const KURL& );

protected:

   void saveConfig();

signals:
   /**
    * selects a TQListViewItem from the thumbnail. This signal only makes
    * sense if connected to a ScanPackager.
    */
   void selectFromThumbnail( const KURL& );

private:
   TQPixmap createPixmap( const TQPixmap& ) const;

   bool    deleteImage( KFileItem* );
   TDEIconView *m_iconView;
   KProgress *m_progress;

   KURL    m_currentDir;
   TQPixmap m_basePix;
   int     m_pixWidth;
   int     m_pixHeight;
   int     m_thumbMargin;
   TQColor  m_marginColor1;
   TQColor  m_marginColor2;
   TQString m_bgImg;
   int     m_cntJobsStarted;
   TDEIO::PreviewJob *m_job;

    KFileItemList m_pendingJobs;
};

#endif
