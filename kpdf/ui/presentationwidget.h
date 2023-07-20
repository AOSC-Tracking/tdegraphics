/***************************************************************************
 *   Copyright (C) 2004 by Enrico Ros <eros.kde@email.it>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef _KPDF_PRESENTATIONWIDGET_H_
#define _KPDF_PRESENTATIONWIDGET_H_

#include <tqdialog.h>
#include <tqpixmap.h>
#include <tqstringlist.h>
#include <tqvaluevector.h>
#include "core/observer.h"
#include "core/pagetransition.h"

class TDEAccel;
class TDEActionCollection;
class TDEToolBar;
class TQTimer;

class KPDFDocument;
class KPDFPage;
class KPDFLink;
class PresentationFrame;

/**
 * @short A widget that shows pages as fullscreen slides (with transitions fx).
 *
 * This is a fullscreen widget that displays 
 */
class PresentationWidget : public TQDialog, public DocumentObserver
{
    TQ_OBJECT
  
    public:
        PresentationWidget( TQWidget * parent, KPDFDocument * doc );
        ~PresentationWidget();

        void setupActions( TDEActionCollection * ac );

        // inherited from DocumentObserver
        uint observerId() const { return PRESENTATION_ID; }
        void notifySetup( const TQValueVector< KPDFPage * > & pages, bool documentChanged );
        void notifyViewportChanged( bool smoothMove );
        void notifyPageChanged( int pageNumber, int changedFlags );
        bool canUnloadPixmap( int pageNumber );

    protected:
        // widget events
//        bool event( TQEvent * e );
        void keyPressEvent( TQKeyEvent * e );
        void wheelEvent( TQWheelEvent * e );
        void mousePressEvent( TQMouseEvent * e );
        void mouseReleaseEvent( TQMouseEvent * e );
        void mouseMoveEvent( TQMouseEvent * e );
        void paintEvent( TQPaintEvent * e );

    private:
        const KPDFLink * getLink( int x, int y, TQRect * geometry = 0 ) const;
        void testCursorOnLink( int x, int y );
        void overlayClick( const TQPoint & position );
        void changePage( int newPage );
        void generatePage();
        void generateIntroPage( TQPainter & p );
        void generateContentsPage( int page, TQPainter & p );
        void generateOverlay();
        void initTransition( const KPDFPageTransition *transition );
        const KPDFPageTransition defaultTransition() const;
        const KPDFPageTransition defaultTransition( int ) const;

        // cache stuff
        int m_width;
        int m_height;
        TQPixmap m_lastRenderedPixmap;
        TQPixmap m_lastRenderedOverlay;
        TQRect m_overlayGeometry;
        const KPDFLink * m_pressedLink;
        bool m_handCursor;

        // transition related
        TQTimer * m_transitionTimer;
        TQTimer * m_overlayHideTimer;
        TQTimer * m_nextPageTimer;
        int m_transitionDelay;
        int m_transitionMul;
        TQValueList< TQRect > m_transitionRects;

        // misc stuff
        KPDFDocument * m_document;
        TQValueVector< PresentationFrame * > m_frames;
        int m_frameIndex;
        TQStringList m_metaStrings;
        TDEToolBar * m_topBar;
        TDEAccel * m_accel;

    private slots:
        void slotNextPage();
        void slotPrevPage();
        void slotFirstPage();
        void slotLastPage();
        void slotHideOverlay();
        void slotTransitionStep();
};

#endif
