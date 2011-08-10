/***************************************************************************
                          scanpackager.h  -  description
                             -------------------
    begin                : Fri Dec 17 1999
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


#ifndef SCANPACKAGER_H
#define SCANPACKAGER_H

#include <tqlistview.h>
#include <tqimage.h>
#include <tqpixmap.h>
#include <tqdragobject.h>
#include <tqmap.h>
#include <klistview.h>
#include <kio/job.h>
#include <kio/global.h>
#include <kio/file.h>
#include <kfiletreeview.h>


/**
  *@author Klaas Freitag
  */

class KURL;
class TQPopupMenu;
class KFileTreeViewItem;
class KookaImage;
class KookaImageMeta;
class KFileTreeBranch;


typedef enum{ Dummy, NameSearch, UrlSearch } SearchType;

class JobDescription
{
public:
   enum JobType { NoJob, ImportJob, RenameJob, ExportJob };
   JobDescription():jobType( NoJob ), kioJob(0L), pitem(0L) {}
   JobDescription( KIO::Job* kiojob, KFileTreeViewItem *new_item, JobType type ) :
      jobType(type), kioJob(kiojob), pitem(new_item) {}

   JobType type( void ) { return( jobType ); }
   KFileTreeViewItem *item( void ) { return( pitem ); }
   KIO::Job* job( void ){ return( kioJob ); }
private:
   JobType       jobType;
   KIO::Job*     kioJob;
   KFileTreeViewItem* pitem;
};

class ScanPackager : public KFileTreeView
{
    Q_OBJECT
  TQ_OBJECT
public:
    ScanPackager( TQWidget *parent);
    ~ScanPackager();
    virtual TQString getImgName( TQString name_on_disk );

    TQString 	getCurrImageFileName( bool ) const;
    KookaImage* getCurrImage() const;

    KFileTreeBranch* openRoot( const KURL&, bool open=false );

   TQPopupMenu *contextMenu() const { return m_contextMenu; }
   void         openRoots();

public slots:
   void         slSelectImage( const KURL& );
   void 	slAddImage( TQImage *img, KookaImageMeta* meta = 0 );
   void         slShowContextMenue(TQListViewItem *, const TQPoint &, int );

   void         slotExportFile( );
    void        slotImportFile();
   void         slotCanceled(KIO::Job*);
   void         slotCurrentImageChanged( TQImage* );

   void         slotDecorate( KFileTreeViewItem* );
   void         slotDecorate( KFileTreeBranch*, const KFileTreeViewItemList& );

   void         slotSelectDirectory( const TQString& );

protected:
   virtual void contentsDragMoveEvent( TQDragMoveEvent *e );

protected slots:
   void         slClicked( TQListViewItem * );
   void         slFileRename( TQListViewItem*, const TQString&, int );
   // void         slFilenameChanged( KFileTreeViewItem*, const KURL & );
   void         slImageArrived( KFileTreeViewItem *item, KookaImage* image );
   void         slotCreateFolder( );
   void         slotDeleteItems( );
   void         slotUnloadItems( );
   void         slotUnloadItem( KFileTreeViewItem *curr );
   void         slotDirCount( KFileTreeViewItem *item, int cnt );
   void         slotUrlsDropped( TQWidget*, TQDropEvent*, KURL::List& urls, KURL& copyTo );
   void         slotDeleteFromBranch( KFileItem* );
   void         slotStartupFinished( KFileTreeViewItem * );
signals:
   void         showImage  ( KookaImage* );
   void         deleteImage( KookaImage* );
   void         unloadImage( KookaImage* );
   void         galleryPathSelected( KFileTreeBranch* branch, const TQString& relativPath );
   void         directoryToRemove( KFileTreeBranch *branch, const TQString& relativPath );
   void         showThumbnails( KFileTreeViewItem* );

   void         aboutToShowImage( const KURL& ); /* starting to load image */
   void         imageChanged( KFileItem* );     /* the image has changed  */

    void         fileDeleted( KFileItem* );
    void         fileChanged( KFileItem* );
    void         fileRenamed( KFileItem*, const KURL& );

private:
   TQString     localFileName( KFileTreeViewItem* it ) const;
   void 	loadImageForItem( KFileTreeViewItem* item );
   TQCString     getImgFormat( KFileTreeViewItem* item ) const;

    TQString 	 buildNewFilename( TQString cmplFilename, TQString currFormat ) const;
   KFileTreeViewItem *spFindItem( SearchType type, const TQString name, const KFileTreeBranch* branch = 0 );
   TQString       itemDirectory( const KFileTreeViewItem*, bool relativ = false ) const;

   // int 	        readDir( TQListViewItem *parent, TQString dir_to_read );
    void         showContextMenu( TQPoint p, bool show_folder = true );

    TQString      m_currImportDir;
    TQString      m_currCopyDir;
    TQString      currSelectedDir;
    KIO::Job     *copyjob;
    int          img_counter;
    TQPopupMenu    *m_contextMenu;

    // like m_nextUrlToSelect in KFileTreeView but for our own purposes (showing the image)
    KURL         m_nextUrlToShow;

   TQPixmap       m_floppyPixmap;
   TQPixmap       m_grayPixmap;
   TQPixmap       m_bwPixmap;
   TQPixmap       m_colorPixmap;

   KFileTreeBranch *m_defaultBranch;
   bool          m_startup;
};

#endif
