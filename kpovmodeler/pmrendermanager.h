//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2001 by Andreas Zehender
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

#ifndef PMRENDERMANAGER_H
#define PMRENDERMANAGER_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "pmcontrolpoint.h"
#include "pmmatrix.h"
#include "pmobject.h"
#include "pmviewstructure.h"

#include <tqobject.h>
#include <tqptrlist.h>
#include <tqcolor.h>
#include <tqptrstack.h>
#include <tqvaluestack.h>
#include <kstaticdeleter.h>
#include <GL/gl.h>

class PMGLView;
class PMCamera;
class PMPoint;
class KConfig;
class TQString;

/**
 * Used internally by PMRenderManager.
 *
 * This class stores informations for one render task.
 */
class PMRenderTask
{
public:
   PMRenderTask( PMGLView* view, PMObject* active, PMObject* top,
                 PMControlPointList* controlPoints, double aspectRatio,
                 int visibilityLevel )
   {
      m_pView = view;
      m_pActiveObject = active;
      m_pTopLevelObject = top;
      m_pControlPoints = controlPoints;
      m_aspectRatio = aspectRatio;
      m_visibilityLevel = visibilityLevel;
   }
   
   ~PMRenderTask( ) { };
   
   PMGLView* view( ) const { return m_pView; }
   PMObject* activeObject( ) const { return m_pActiveObject; }
   PMObject* topLevelObject( ) const { return m_pTopLevelObject; }
   PMControlPointList* controlPoints( ) const { return m_pControlPoints; }
   double aspectRatio( ) const { return m_aspectRatio; }
   int visibilityLevel( ) const { return m_visibilityLevel; }

   void setView( PMGLView* view ) { m_pView = view; }
   void setActiveObject( PMObject* obj ) { m_pActiveObject = obj; }
   void setTopLevelObject( PMObject* obj ) { m_pTopLevelObject = obj; }
   void setControlPoints( PMControlPointList* list ) { m_pControlPoints = list; }
   void setAspectRatio( double ar ) { m_aspectRatio = ar; }
   void setVisibilityLevel( int l ) { m_visibilityLevel = l; }
   
private:
   PMGLView* m_pView;
   PMObject* m_pActiveObject;
   PMObject* m_pTopLevelObject;
   PMControlPointList* m_pControlPoints;
   double m_aspectRatio;
   int m_visibilityLevel;
};

typedef TQPtrList<PMRenderTask> PMRenderTaskList;
typedef TQPtrListIterator<PMRenderTask> PMRenderTaskListIterator;

/**
 * Class that controls the background rendering
 */
class PMRenderManager : public QObject
{
   Q_OBJECT
public:
   /**
    * Returns a pointer to the render manager
    */
   static PMRenderManager* theManager( );
   /**
    * destructor
    */
   ~PMRenderManager( );
   /**
    * Adds the @ref PMGLView to the list of views that have to be rendered.
    *
    * @param view The view
    * @param active The active object
    * @param top The top level object (normally the scene)
    * @param controlPoints A pointer to the list of control points for the
    *        active object
    * @param graphicalChange If true the view will be rendered with higher
    *        priority
    */
   void addView( PMGLView* view, PMObject* active, PMObject* top,
                 PMControlPointList* controlPoints, double aspectRatio,
                 int visibilityLevel, bool graphicalChange );
   /**
    * Removes the view from the list of views that have to be rendered
    */
   void removeView( PMGLView* view );

   /**
    * Call this method if a PMGLView was created
    */
   void viewCreated( ) { m_nViews++; }
   /**
    * Call this method if a PMGLView was deleted
    */
   void viewDeleted( ) { m_nViews--; }

   /**
    * Returns true if the render manager holds a task for the view
    */
   bool containsTask( PMGLView* view ) const;

   /**
    * Returns the color for the control points
    */
   TQColor controlPointColor( int i ) const;
   /**
    * Sets the control point color
    */
   void setControlPointColor( int i, const TQColor& c );
   /**
    * Returns the color for graphical objects
    */
   TQColor graphicalObjectColor( int i ) const;
   /**
    * Sets the graphical object color
    */
   void setGraphicalObjectColor( int i, const TQColor& c );
   /**
    * Returns the color for the coordinate axes
    */
   TQColor axesColor( int i ) const;
   /**
    * Sets the axes color
    */
   void setAxesColor( int i, const TQColor& c );
   /**
    * Returns the background color
    */
   TQColor backgroundColor( ) const { return m_backgroundColor; }
   /**
    * Sets the background color
    */
   void setBackgroundColor( const TQColor& c ) { m_backgroundColor = c; }
   /**
    * Returns the field of view color.
    */
   TQColor fieldOfViewColor( ) const { return m_fieldOfViewColor; }
   /**
    * Sets the field of view color
    */
   void setFieldOfViewColor( const TQColor& c ) { m_fieldOfViewColor = c; }
   
   /**
    * Sets the grid color
    */
   void setGridColor( const TQColor& c ) { m_gridColor = c; }
   /**
    * Returns the grid color
    */
   TQColor gridColor( ) { return m_gridColor; }
   /**
    * Sets the grid distance
    */
   void setGridDistance( int d );
   /**
    * Returns the grid distance
    */
   int gridDistance( ) { return m_gridDistance; }

   /**
    * Returns true if the camera views with complex projections
    * are rendered with high detail
    */
   bool highDetailCameraViews( ) const { return m_highDetailCameraView; }
   /**
    * Sets the highDetailCameraView flag
    */
   void setHighDetailCameraViews( bool yes ) { m_highDetailCameraView = yes; }
   
   /**
    * Sets the gl drawing color
    */
   static void setGLColor( const TQColor& c );

   /**
    * Saves the configuration
    */
   void saveConfig( KConfig* cfg );
   /**
    * Restores the configuration
    */
   void restoreConfig( KConfig* cfg );

   /**
    * Returns true if the glx extension is available
    */
   static bool hasOpenGL( );
   /**
    * Disables OpenGL rendering
    */
   static void disableOpenGL( );
   
public slots:
   /**
    * Stops rendering
    */
   void slotStopRendering( );
   /**
    * Call this when rendering settings have been changed
    */
   void slotRenderingSettingsChanged( );
   
signals:
   /**
    * Emitted when rendering starts for the view v
    */
   void renderingStarted( PMGLView* v );
   /**
    * Emitted just before the view is updated
    */
   void aboutToUpdate( PMGLView* v );
   /**
    * Emitted when rendering has been finished for the view v
    */
   void renderingFinished( PMGLView* v );
   /**
    * Emitted when rendering settings (colors ...) have been changed
    */
   void renderingSettingsChanged( );
   /**
    * Emitted when rendering has started
    */
   void renderingStarted( );
   /**
    * Emitted when rendering has finished
    */
   void renderingFinished( );
   
protected:
   virtual void timerEvent( TQTimerEvent* );
   
private:
   /**
    * constructor
    */
   PMRenderManager( );
   
   /**
    * Restarts rendering
    */
   void restartRendering( );
   
   /**
    * The background task for rendering
    */
   void renderTask( );
   /**
    * Renders one object
    */
   void renderObject( PMObject* obj );
   /**
    * Renders the view structure, subdivides the lines in high
    detail camera views */
   void renderViewStructure( PMViewStructure& vs );
   /**
    * Renders the view structure without subdivisions
    */
   void renderViewStructureSimple( PMPointArray& points, PMLineArray& lines,
                                   int numberOfLines = -1 );
   /**
    * Renders the control points
    */
   void renderControlPoints( );
   /**
    * Draws the grid
    */
   void renderGrid( );
   /**
    * Draws the coordinate axis
    */
   void renderAxes( );
   /**
    * Draws the field of view for camera views
    */
   void renderFieldOfView( );
   /**
    * Draws the view descriptions
    */
   void renderDescription( );
   /**
    * Renders the string
    */
   void renderString( const TQString& str, double x, double y );
   
   /**
    * Transforms and renders the view structure for special
    * camera projection types.
    */
   void transformProjection( PMPoint* points, int size, PMCamera* camera );
   /**
    * Sets the projection for the current view
    */
   void setProjection( );
   /**
    * Sets the projection for a camera view
    */
   void setCameraProjection( );

   /**
    * Calculates the view transformation for the camera c
    */
   PMMatrix viewTransformation( PMCamera* c ) const;

   /**
    * List of render tasks. The first has the highest priority
    */
   TQPtrList<PMRenderTask> m_renderTasks;
   /**
    * Flag for background rendering
    */
   bool m_bStopTask, m_bStartTask, m_bTaskIsRunning;
   /**
    * The color for view structures of graphical objects.
    *
    * index 0: normal color, 1: selected
    */
   TQColor m_graphicalObjectColor[2];
   /**
    * The color for view structures of textures
    */
   TQColor m_textureColor[2];
   /**
    * The color for the coordinate axes
    */
   TQColor m_axesColor[3];
   /**
    * The background color
    */
   TQColor m_backgroundColor;
   /**
    * color for control points
    *
    * index 0: normal color, 1: selected
    */
   TQColor m_controlPointColor[2];
   /**
    * Color for the field of view box
    */
   TQColor m_fieldOfViewColor;
   
   /**
    * Grid distance and color
    */
   int m_gridDistance;
   TQColor m_gridColor;
   /**
    * If true, lines are subdivided in camera views with complex
    * projections
    */
   bool m_highDetailCameraView;
   
   /**
    * Number of rendered lines between calls of processEvents( )
    */
   unsigned int m_nMaxRenderedLines;

   /**
    * Number of gl views
    */
   unsigned int m_nViews;
   /**
    * The render manager (singleton pattern)
    */
   static PMRenderManager* s_pManager;
   static KStaticDeleter<PMRenderManager> s_staticDeleter;

   // Member variables to save stack space during rendering
   PMRenderTask* m_pCurrentTask;
   PMGLView* m_pCurrentGlView;
   TQPtrStack<PMMatrix> m_matrixStack; // I don't know if the build in gl matrix stack is deep enough
   bool m_selected;
   PMObject* m_pDeselectObject;
   PMObjectList m_objectToRenderStack;
   TQPtrStack<PMObject> m_quickColorObjects;
   TQPtrStack<TQColor> m_quickColors;
   TQColor m_currentColor;
   TQValueStack<int> m_visibilityStack;
   int m_currentVisibility;
   unsigned int m_renderedLines;
   PMMatrix m_controlPointTransformation;

   // for transformProjection
   bool m_specialCameraMode;
   PMMatrix m_viewTransformation;
   double m_upLength, m_rightLength, m_directionLength;
   double m_anglex, m_angley;
   PMViewStructure m_subdivisionViewStructure;

   PMViewStructure m_axesViewStructure[3];
   bool m_axesViewStructureCreated;

   static bool s_hasOpenGL;
   static bool s_hasOpenGLChecked;
};

#endif
