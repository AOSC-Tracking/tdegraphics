/****************************************************************************
** $Id: .emacs,v 1.3 2002/02/20 15:06:53 gis Exp $
**
** Created : 2002
**
** Copyright (C) 2002 Carsten Pfeiffer <pfeiffer@kde.org> 
**
****************************************************************************/

#ifndef KUICKIMAGE_H
#define KUICKIMAGE_H

#include <tqimage.h>
#include <tqobject.h>

#include <kurl.h>

#include "kuickdata.h"
#include "kuickfile.h"

// #include those AFTER TQt-includes!
#include <Imlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
// #include <X11/extensions/shape.h>


class KuickImage : public TQObject
{
  Q_OBJECT
  

public:
  enum ResizeMode { FAST, SMOOTH };

  KuickImage( const KuickFile * file, ImlibImage *im, ImlibData *id );
  ~KuickImage();

  int 		width() 	const { return myWidth;   }
  int 		height()	const { return myHeight;  }
  int 		originalWidth() const { return myOrigWidth; }
  int 		originalHeight() const { return myOrigHeight; }

  void 		resize( int width, int height, KuickImage::ResizeMode mode );
  void 		restoreOriginalSize();
  void		rotate( Rotation rot );
  bool		rotateAbs( Rotation rot );
  void 		flip( FlipMode flipMode );
  bool 		flipAbs( int mode );
  ImlibImage *	imlibImage()	const { return myIm;      }
  Pixmap& 	pixmap();
  void 		renderPixmap();
//  const TQString& filename() 	const { return myFilename;}
  const KuickFile& file()       const { return *myFile; }
  const KURL& url()             const { return myFile->url(); }

  void 		setDirty( bool d )    { myIsDirty = d;    }
  bool 		isDirty() 	const { return myIsDirty; }
  Rotation      absRotation()   const { return myRotation; }
  FlipMode      flipMode()      const { return myFlipMode; }

  static ImlibImage * toImage( ImlibData *id, TQImage& image );

private:
  void      fastResize( int newWidth, int newHeight );
  bool 		smoothResize( int width, int height );
  /**
   * Note: caller must delete it!
   */
  TQImage * 	newTQImage() const;

  const KuickFile * myFile;

  int 		myWidth;
  int 		myHeight;
  ImlibImage * 	myOrigIm;
  ImlibImage * 	myIm;
  ImlibData  * 	myId;
  Pixmap 	myPixmap;
  bool 		myIsDirty;

  int 		myOrigWidth;
  int 		myOrigHeight;
  Rotation 	myRotation;
  FlipMode 	myFlipMode;

signals:
  void 		startRendering();
  void 		stoppedRendering();
};


#endif // KUICKIMAGE_H
