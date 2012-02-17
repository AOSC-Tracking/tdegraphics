/* This file is part of the KDE Project
   Copyright (C) 1999 Klaas Freitag <freitag@suse.de>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef __IMG_CANVAS_H__
#define __IMG_CANVAS_H__

#include <tqwidget.h>
#include <tqrect.h>
#include <stdlib.h>
#include <tqsize.h>
#include <tqwmatrix.h>
#include <tqscrollview.h>
#include <tqstrlist.h>

#ifdef USE_KPIXMAPIO
#include <kpixmapio.h>
#endif

class TQPopupMenu;
class TQPixmap;
class TQImage;
class TQPainter;

enum preview_state {
	MOVE_NONE,
	MOVE_TOP_LEFT,
	MOVE_TOP_RIGHT,
	MOVE_BOTTOM_LEFT,
	MOVE_BOTTOM_RIGHT,
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_TOP,
	MOVE_BOTTOM,
	MOVE_WHOLE
};

enum cursor_type {
	CROSS,
	VSIZE,
	HSIZE,
	BDIAG,
	FDIAG,
	ALL,
	HREN
};

const int MIN_AREA_WIDTH = 3;
const int MIN_AREA_HEIGHT = 3;
const int delta = 3;
#ifdef __PREVIEW_CPP__
int max_dpi = 600;
#else
extern int max_dpi;
#endif



class ImageCanvas: public TQScrollView
{
    Q_OBJECT
  
    Q_ENUMS( PopupIDs )
    TQ_PROPERTY( int brightness READ getBrightness WRITE setBrightness )
    TQ_PROPERTY( int contrast READ getContrast WRITE setContrast )
    TQ_PROPERTY( int gamma READ getGamma WRITE setGamma )
    TQ_PROPERTY( int scale_factor READ getScaleFactor WRITE setScaleFactor )

public:
    ImageCanvas( TQWidget *parent = 0,
                 const TQImage *start_image = 0,
                 const char *name = 0);
    ~ImageCanvas( );

    int getBrightness() const;
    int getContrast() const;
    int getGamma() const;

    int getScaleFactor() const;

    const TQImage *rootImage();

    bool hasImage( void ) 	{ return acquired; }
    TQPopupMenu* contextMenu() { return m_contextMenu; }
    TQRect sel( void );

    enum ScaleKinds { UNSPEC, DYNAMIC, FIT_ORIG, FIT_WIDTH, FIT_HEIGHT, ZOOM };

    enum PopupIDs { ID_POP_ZOOM, ID_POP_CLOSE, ID_FIT_WIDTH,
                    ID_FIT_HEIGHT, ID_ORIG_SIZE };

    bool selectedImage( TQImage* );

    ScaleKinds scaleKind();
    const TQString scaleKindString();

    ScaleKinds defaultScaleKind();

    const TQString imageInfoString( int w=0, int h=0, int d=0 );

public slots:
    void setBrightness(int);
    void setContrast(int );
    void setGamma(int );

    void toggleAspect( int aspect_in_mind )
        {
            maintain_aspect = aspect_in_mind;
            repaint();
        }
    virtual TQSize sizeHint() const;
    void newImage( TQImage* );
    void newImageHoldZoom( TQImage* );
    void deleteView( TQImage *);
    void newRectSlot();
    void newRectSlot( TQRect newSel );
    void noRectSlot( void );
    void setScaleFactor( int i );
    void handle_popup(int item );
    void enableContextMenu( bool wantContextMenu );
    void setKeepZoom( bool k );
    void setScaleKind( ScaleKinds k );
    void setDefaultScaleKind( ScaleKinds k );

    /**
     * Highlight a rectangular area on the current image using the given brush
     * and pen.
     * The function returns a id that needs to be given to the remove method.
     */
    int highlight( const TQRect&, const TQPen&, const TQBrush&, bool ensureVis=false );

    /**
     * reverts the highlighted region back to normal view.
     */
    void removeHighlight( int idx = -1 );

    /**
     * permit to do changes to the image that are saved back to the file
     */
    void setReadOnly( bool );

    bool readOnly();
	
signals:
    void noRect( void );
    void newRect( void );
    void newRect( TQRect );
    void scalingRequested();
    void closingRequested();
    void scalingChanged( const TQString& );
    /**
     * signal emitted if the permission of the currently displayed image changed,
     * ie. if it goes from writeable to readable.
     * @param shows if the image is now read only (true) or not.
     */
    void imageReadOnly( bool isRO );
    
protected:
    void drawContents( TQPainter * p, int clipx, int clipy, int clipw, int cliph );

    void timerEvent(TQTimerEvent *);
    void viewportMousePressEvent(TQMouseEvent *);
    void viewportMouseReleaseEvent(TQMouseEvent *);
    void viewportMouseMoveEvent(TQMouseEvent *);

    void resizeEvent( TQResizeEvent * event );

private:
    TQStrList      urls;

    int           scale_factor;
    const TQImage        *image;
    int           brightness, contrast, gamma;


#ifdef USE_KPIXMAPIO
    KPixmapIO	 pixIO;
#endif

    TQWMatrix	 scale_matrix;
    TQWMatrix	 inv_scale_matrix;
    TQPixmap       *pmScaled;
    float	 used_yscaler;
    float	 used_xscaler;
    TQPopupMenu    *m_contextMenu;
    bool		 maintain_aspect;

    int	         timer_id;
    TQRect 	 *selected;
    preview_state moving;
    int 		 cr1,cr2;
    int 		 lx,ly;
    bool 	 acquired;

    /* private functions for the running ant */
    void drawHAreaBorder(TQPainter &p,int x1,int x2,int y,int r = FALSE);
    void drawVAreaBorder(TQPainter &p,int x,int y1,int y2,int r = FALSE);
    void drawAreaBorder(TQPainter *p,int r = FALSE);
    void update_scaled_pixmap( void );

    preview_state classifyPoint(int x,int y);

    class ImageCanvasPrivate;
    ImageCanvasPrivate *d;
};

#endif
