/****************************************************************************
** $Id$
**
** ImlibWidget: maps an Xlib window with Imlib's contents on a TQWidget
**
** Created : 98
**
** Copyright (C) 1998-2001 by Carsten Pfeiffer.  All rights reserved.
**
****************************************************************************/

#ifndef IMLIBWIDGET_H
#define IMLIBWIDGET_H

#include <tqvariant.h>

#include <tqcursor.h>
#include <tqevent.h>
#include <tqptrlist.h>
#include <tqtimer.h>
#include <tqwidget.h>

#include <kurl.h>

// #include those AFTER TQt-includes!
#include <Imlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
// #include <X11/extensions/shape.h>

#include "imdata.h"
#include "kuickdata.h"

class KuickFile;
class KuickImage;

class ImageCache : public TQObject
{
  TQ_OBJECT
  

public:
  ImageCache( ImlibData *id, int maxImages=1 );
  ~ImageCache();

  void 			setMaxImages( int maxImages );
  int 			maxImages() 		const { return myMaxImages; }

  KuickImage *		getKuimage( KuickFile * file, ImlibColorModifier  );

private:
  ImlibImage *		loadImageWithTQt( const TQString& filename ) const;

  int 			myMaxImages;
  TQValueList<KuickFile*>fileList;
  TQPtrList<KuickImage>	kuickList;
  //  TQPtrList<ImlibImage>	imList;
  ImlibData * 		myId;
  int 			idleCount;

private slots:
  void 			slotBusy();
  void 			slotIdle();

signals:
  void			sigBusy();
  void 			sigIdle();

};


// ------------------------------------------

class TQColor;

class ImlibWidget : public TQWidget
{
  TQ_OBJECT
  

public:

  ImlibWidget( ImData *_idata=0, TQWidget *parent=0, const char *name=0 );
  ImlibWidget( ImData *_idata, ImlibData *id, TQWidget *parent=0,
	       const char *name=0 );
  virtual ~ImlibWidget();

  KURL          url()                   const;
  KuickFile *   currentFile()           const;
  bool		loadImage( KuickFile * file );
  bool		loadImage( const KURL& url );
  bool 		cacheImage(const KURL& url );
  void   	zoomImage( float );
  void 		setBrightness( int );
  void 		setContrast( int );
  void 		setGamma( int );
  void 		setRotation( Rotation );
  void 		setFlipMode( int mode );

  int 		brightness() 		 const; // ### no impl!
  int 		contrast()		 const; // ### no impl!
  int 		gamma() 		 const; // ### no impl!
  Rotation 	rotation() 		 const;
  FlipMode	flipMode() 		 const;

  int 		imageWidth() 		 const;
  int 		imageHeight() 		 const;

  void 		setAutoRender( bool enable )   { isAutoRendering = enable;}
  bool 		isAutoRenderEnabled() 	const  { return isAutoRendering;  }
  void 		setMaxImageCache( int );
  int 		maxImageCache() 	const  { return myMaxImageCache;  }
  const TQColor& backgroundColor() 	const;
  void 		setBackgroundColor( const TQColor& );

  /**
   * @return true if auto-rotation is not possible, e.g. because no metadata
   * about orientation is available
   */
  virtual bool  autoRotate( KuickImage *kuim );

  ImlibData*	getImlibData() const 	       { return id; 		  }

  virtual void  reparent( TQWidget* parent, WFlags f, const TQPoint& p, bool showIt = false );

public slots:
  void	        rotate90();
  void	        rotate270();
  void	        rotate180();
  void 		flipHoriz();
  void 		flipVert();
  void 		showImageOriginalSize();
  inline void 	updateImage() 		{ updateWidget( true ); }


protected:
  KuickImage *	loadImageInternal( KuickFile * file );
  void 		showImage();
  void          setImageModifier();
  void 		rotate( int );
  void 		updateWidget( bool geometryUpdate=true );
  virtual void 	updateGeometry( int width, int height );
  virtual void  loaded( KuickImage * );
  virtual bool  canZoomTo( int newWidth, int newHeight );
  virtual void  rotated( KuickImage *kuim, int rotation );

  void 		closeEvent( TQCloseEvent * );

  inline void	autoUpdate( bool geometryUpdate=false ) {
    if ( isAutoRendering )
      updateWidget( geometryUpdate );
  }

  bool		stillResizing, deleteImData, deleteImlibData;
  bool          imlibModifierChanged;

  KuickImage 	*m_kuim;
  ImageCache 	*imageCache;
  ImlibData     *id;
  ImData    	*idata;
  Window        win;
  ImlibColorModifier mod;

  KuickFile *m_kuickFile;
  TQCursor m_oldCursor;

  static const int ImlibOffset;


private:
  void 		init();
  bool 		isAutoRendering;
  int 		myMaxImageCache;
  TQColor 	myBackgroundColor;


protected slots:
  bool 		cacheImage( KuickFile *file );
  virtual void 	setBusyCursor();
  virtual void	restoreCursor();


signals:
  void 		sigImageError( const KuickFile * file, const TQString& );

};


#endif
