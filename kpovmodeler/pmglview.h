//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2003 by Andreas Zehender
    email                : zehender@kde.org
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#ifndef PMGLVIEW_H
#define PMGLVIEW_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tqwidget.h>
#include <tqpoint.h>
#include <tqdatetime.h>
#include <tqtimer.h>
#include <tqpixmap.h>

#include "pmviewbase.h"
#include "pmviewfactory.h"
#include "pmcontrolpoint.h"
#include "pmvector.h"
#include "pmmatrix.h"
#include "pmobjectaction.h"

const int controlPointSize = 7;

class PMObject;
class PMPart;
class PMCamera;
class KConfig;
class TQComboBox;

/**
 * Widget for rendering the scene with OpenGL
 */
class PMGLView : public PMViewBase
{
   Q_OBJECT
  TQ_OBJECT
public:
   /**
    * Type of the view
    */
   enum PMViewType { PMViewPosX, PMViewNegX, PMViewPosY, PMViewNegY,
                     PMViewPosZ, PMViewNegZ, PMViewCamera };
   /**
    * Constructor
    */
   PMGLView( PMPart* part, PMViewType t,
             TQWidget* tqparent = 0, const char* name = 0, WFlags f = 0 );
   /**
    * Destructor
    */
   ~PMGLView( );

   /** */
   virtual TQString viewType( ) const { return TQString( "glview" ); }
   /** */
   virtual TQString description( ) const;
   /** */
   virtual void restoreViewConfig( PMViewOptions* );
   /** */
   virtual void saveViewConfig( PMViewOptions* ) const;

   /**
    * Enables/disables translating the view with the mouse
    */
   void enableTranslateMode( bool yes = true );
   /**
    * Enables/disables scaling the view with the mouse
    */
   void enableScaleMode( bool yes = true );

   /**
    * Returns true if the opengl projection is up to date
    */
   bool isProjectionUpToDate( ) const { return m_projectionUpToDate; }
   /**
    * Sets the projection up to date flag
    */
   void setProjectionUpToDate( bool yes ) { m_projectionUpToDate = yes; }

   /**
    * Sets the scale
    */
   void setScale( double scale );
   /**
    * Returns the scale
    */
   double scale( ) const { return m_dScale; }
   /**
    * Sets the views translation in x direction
    */
   void setTranslationX( double d );
   /**
    * Returns the views translation in x direction
    */
   double translationX( ) const { return m_dTransX; }
   /**
    * Sets the views translation in y direction
    */
   void setTranslationY( double d );
   /**
    * Returns the views translation in y direction
    */
   double translationY( ) const { return m_dTransY; }

   /**
    * Returns the 2D control points position in the view
    */
   const TQPtrList<PMVector>& controlPointsPosition( ) const
   {
      return m_controlPointsPosition;
   }
   /**
    * Returns the last right mouse click position
    */
   PMVector contextClickPosition( ) const { return m_contextClickPosition; }

   /**
    * Returns the view type
    */
   PMViewType type( ) const { return m_type; }
   /**
    * Sets the view type
    */
   void setType( PMViewType t );
   /**
    * Sets the camera
    */
   void setCamera( PMCamera* c );
   /**
    * Returns the camera
    */
   PMCamera* camera( ) const { return m_pCamera; }

   /**
    * Saves the configuration
    */
   static void saveConfig( KConfig* cfg );
   /**
    * Restores the configuration
    */
   static void restoreConfig( KConfig* cfg );

   /**
    * Returns true if the glx stuff was initialized successfully
    */
   bool isValid( ) const;
   /**
    * Sets this view as the current rendering view
    */
   void makeCurrent( );
   /**
    * Swaps the opengl buffers
    */
   void swapBuffers( );

   /**
    * Returns the view type as string
    */
   static TQString viewTypeAsString( PMViewType t );
   /**
    * Sets the direct rendering flag
    */
   static void enableDirectRendering( bool yes ) { s_bDirect = yes; }
   /**
    * Returns the direct rendering flag
    */
   static bool isDirectRenderingEnabled( ) { return s_bDirect; }
public slots:
   /**
    * Sets the view normal vector to the positive x-axes
    */
   void slotSetTypePosX( ) { setType( PMViewPosX ); }
   /**
    * Sets the view normal vector to the negative x-axes
    */
   void slotSetTypeNegX( ) { setType( PMViewNegX ); }
   /**
    * Sets the view normal vector to the positive y-axes
    */
   void slotSetTypePosY( ) { setType( PMViewPosY ); }
   /**
    * Sets the view normal vector to the negative y-axes
    */
   void slotSetTypeNegY( ) { setType( PMViewNegY ); }
   /**
    * Sets the view normal vector to the positive z-axes
    */
   void slotSetTypePosZ( ) { setType( PMViewPosZ ); }
   /**
    * Sets the view normal vector to the negative z-axes
    */
   void slotSetTypeNegZ( ) { setType( PMViewNegZ ); }

   /**
    * Called when an object is changed.
    * @see PMPart::objectChanged( )
    */
   void slotObjectChanged( PMObject* obj, const int mode, TQObject* sender );
   /**
    * Restarts rendering
    */
   void slotRefresh( );
   /**
    * Clears all data
    */
   void slotClear( );
   /**
    * Stops rendering
    */
   void slotStopRendering( );
   /**
    * Repaints the view if it is a camera view
    */
   void slotActiveRenderModeChanged( );

   /**
    * Connected to the render manager
    */
   void slotRenderingStarted( PMGLView* view );
   /**
    * Connected to the render manager
    */
   void slotAboutToUpdate( PMGLView* view );
   /**
    * Connected to the render manager
    */
   void slotRenderingFinished( PMGLView* view );

protected slots:
   /**
    * Sets the view type to camera view
    */
   void slotCameraView( int id );
   /**
    * Called when an object action was selected in the context menu
    */
   void slotObjectAction( int id );
   /**
    * Called when a control point was selected in the context menu
    */
   void slotControlPoint( int id );
   /**
    * Aligns the selected control points to the grid
    */
   void slotSnapToGrid( );

   void slotMouseChangeTimer( );
   void slotAutoScroll( );

signals:
   /**
    * Emitted when rendering has to be restarted
    */
   void refresh( PMGLView* w );
   /**
    * Emitted when an object is changed
    */
   void objectChanged( PMObject* obj, const int mode, TQObject* sender );
   /**
    * Emitted when the mouse is over a control point
    */
   void controlPointMessage( const TQString& msg );
   /**
    * Emitted in the destructor
    */
   void destroyed( PMGLView* v );
   /**
    * Emitted when the view type changes
    */
   void viewTypeChanged( const TQString& str );

protected:
   /**
    * Initializes the glx stuff
    */
   virtual void initializeGL( );
   /** */
   virtual void resizeEvent( TQResizeEvent* e );
   /** */
   virtual void paintEvent( TQPaintEvent* e );
   /** */
   virtual void mousePressEvent( TQMouseEvent* e );
   /** */
   virtual void mouseReleaseEvent( TQMouseEvent* e );
   /** */
   virtual void mouseMoveEvent( TQMouseEvent* e );
   /** */
   virtual void keyPressEvent( TQKeyEvent* e );
   /**
    * Event to zoom in / zoom out the viewport by mouse wheel
    */
   virtual void wheelEvent( TQWheelEvent* e );

private:
   /**
    * Updates the control points
    */
   void updateControlPoints( );
   /**
    * Recalculates the position of the control points on the screen
    */
   void recalculateControlPointPosition( );
   /**
    * Recalculates m_controlPointsTransformation and
    * m_inversePointsTransformation
    */
   void recalculateTransformations( );
   /**
    * Returns the mouse 3D position, when the control point cp is selected
    *
    * x and y are the screen coordinates of the mouse.
    */
   PMVector mousePosition( PMControlPoint* cp, int x, int y );
   /**
    * Checks if a control point is under the mouse
    */
   void checkUnderMouse( int x, int y );
   /**
    * Repaints the view
    */
   void tqrepaint( bool graphicalChange = false );
   /**
    * Starts a graphical change
    */
   void startChange( const TQPoint& mousePos );
   /**
    * Graphical Change
    */
   void graphicalChange( const TQPoint& mousePos );
   /**
    * Selects/deselecs the control point. If cp is 0, all control points are
    * selected/deselected.
    */
   void selectControlPoint( PMControlPoint* cp, bool select, bool deselectOthers = true );
   /**
    * Invalidates the projection and repaints the view
    */
   void invalidateProjection( bool graphicaChange = false );

   /**
    * Starts multiple selection mode
    */
   void startSelection( );
   /**
    * Restores the widget under the selection rect
    */
   void restoreSelectionBox( );
   /**
    * Saves the widget under the selection rect
    */
   void saveSelectionBox( );
   /**
    * Paints the selection box
    */
   void paintSelectionBox( );
   /**
    * Calculates the selection box
    */
   void calculateSelectionBox( int& sx, int& sy, int& ex, int& ey, int& w, int& h );

   double screenToInternalX( int x ) const;
   double screenToInternalY( int y ) const;

   /**
    * Returns the top level object for rendering (a declaration or the scene)
    */
   PMObject* topLevelRenderingObject( PMObject* obj ) const;
   /**
    * Displays the context menu
    */
   void contextMenu( );

   /**
    * Type of the view (camera, xy, ... )
    */
   PMViewType m_type;
   /**
    * Pointer to the part
    */
   PMPart* m_pPart;
   /**
    * True if "scale view" is active
    */
   bool m_bScaleMode;
   double m_scaleIntX, m_scaleIntY;
   /**
    * true if "translate view" is active
    */
   bool m_bTranslateMode;
   /**
     * True if "scale view" or "translate view" is active and the left
     * mouse button is pressed.
     */
   bool m_bMousePressed;
   /**
    * MidButton pressed
    */
   bool m_bMidMousePressed;
   /**
    * True if a graphical change is active
    */
   bool m_bGraphicalChangeMode;
   bool m_bMementoCreated;
   /**
    * The old mouse position
    */
   TQPoint m_mousePos;
   TQPoint m_changeStartPos;
   TQPoint m_currentMousePos;
   TQTimer m_startTimer;
   TQTime m_changeStartTime;
   bool m_bDeselectUnderMouse;
   bool m_bSelectUnderMouse;

   /**
    * Member variables for multiple selection mode
    */
   TQPixmap m_selectionPixmap[4];
   TQPoint m_selectionStart, m_selectionEnd;
   bool m_bMultipleSelectionMode;
   bool m_bSelectionStarted;

   /**
    * Member variables for autoscroll
    */
   bool m_bAutoScroll;
   double m_autoScrollSpeed;
   TQTimer m_autoScrollTimer;
   TQTime m_lastAutoScrollUpdate;
   int m_autoScrollDirectionX, m_autoScrollDirectionY;
   /**
    * Rendering
    */
   bool m_bAboutToUpdate;

   /**
    * Scale of the view
    */
   double m_dScale;
   /**
    * X-translation of the view
    */
   double m_dTransX;
   /**
    * Y-translation of the view
    */
   double m_dTransY;

   /**
    * Control points of the active object
    */
   PMControlPointList m_controlPoints;
   /**
    * Control point under the mouse
    */
   PMControlPoint* m_pUnderMouse;
   /**
    * Position of the control points on the screen
    */
   TQPtrList<PMVector> m_controlPointsPosition;
   /**
    * Position of the last right mouse click
    */
   PMVector m_contextClickPosition;
   /**
    * Transformation of the control points
    *
    * Always m_viewTransformation * m_objectsTransformation.
    */
   PMMatrix m_controlPointsTransformation;
   /**
    * Inverse of m_controlPointsTransformation
    */
   PMMatrix m_inversePointsTransformation;
   /**
    * True if m_inversePointsTransformation is valid
    */
   bool m_bInverseValid;
   /**
    * Normal vector of the view
    */
   PMVector m_normal;
   /**
    * Transformation of the view (scale and translation)
    */
   PMMatrix m_viewTransformation;
   /**
    * Transformation of the active object
    */
   PMMatrix m_objectsTransformation;
   /**
    * The camera
    */
   PMCamera* m_pCamera;
   PMObject* m_pActiveObject;
   /**
    * true if the opengl projection is up to date
    */
   bool m_projectionUpToDate;
   int m_visibilityLevel;
   /**
    * The current object actions
    */
   TQPtrList<PMObjectAction> m_objectActions;
   static bool s_bDirect;
};

/**
 * Options class for the opengl view
 */
class PMGLViewOptions : public PMViewOptions
{
public:
   PMGLViewOptions( )
   {
      m_glViewType = PMGLView::PMViewPosX;
   }
   PMGLViewOptions( PMGLView::PMViewType t )
   {
      m_glViewType = t;
   }
   virtual PMViewOptions* copy( ) const { return new PMGLViewOptions( *this ); }
   virtual TQString viewType( ) const { return TQString( "glview" ); }
   PMGLView::PMViewType glViewType( ) const { return m_glViewType; }
   void setGLViewType( PMGLView::PMViewType t ) { m_glViewType = t; }
   virtual void loadData( TQDomElement& e );
   virtual void saveData( TQDomElement& e );

private:
   PMGLView::PMViewType m_glViewType;
};

/**
 * Factory class for 3d views
 */
class PMGLViewFactory : public PMViewTypeFactory
{
public:
   PMGLViewFactory( ) { }
   virtual TQString viewType( ) const { return TQString( "glview" ); }
   virtual TQString description( ) const;
   virtual TQString description( PMViewOptions* ) const;
   virtual TQString iconName( ) const { return TQString( "pmglview" ); }
   virtual PMViewBase* newInstance( TQWidget* tqparent, PMPart* part ) const
   {
      return new PMGLView( part, PMGLView::PMViewPosX, tqparent );
   }
   virtual PMViewOptions* newOptionsInstance( ) const;
   virtual PMViewOptionsWidget* newOptionsWidget( TQWidget*, PMViewOptions* );
};

/**
 * Configuration widget for the view tqlayout settings dialog
 */
class PMGLViewOptionsWidget : public PMViewOptionsWidget
{
   Q_OBJECT
  TQ_OBJECT
public:
   /**
    * Default constructor
    */
   PMGLViewOptionsWidget( TQWidget* tqparent, PMViewOptions* o );

protected slots:
   void slotGLViewTypeChanged( int );

private:
   PMGLViewOptions* m_pOptions;
   TQComboBox* m_pGLViewType;
};

#endif
