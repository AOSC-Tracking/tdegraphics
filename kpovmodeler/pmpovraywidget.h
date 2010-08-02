/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001 by Andreas Zehender
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

#ifndef PMPOVRAYWIDGET_H
#define PMPOVRAYWIDGET_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <kdialog.h>
#include <tqcstring.h>
#include <tqdatetime.h>

class PMPovrayRenderWidget;
class PMPovrayOutputWidget;
class PMRenderMode;

class TQProgressBar;
class TQPushButton;
class TQLabel;
class TQScrollView;
class KURL;

/**
 * Widget with toolbar, statusbar and a @ref PMPovrayRenderWidget
 */
class PMPovrayWidget : public KDialog
{
   Q_OBJECT
public:
   /**
    * Standard constructor
    */
   PMPovrayWidget( TQWidget* parent = 0, const char* name = 0 );
   /**
    * Destructor
    */
   virtual ~PMPovrayWidget( );

   /**
    * Starts rendering for the povray code in the byte array with
    * render mode m.
    * @see PMRenderMode
    */
   bool render( const TQByteArray& scene, const PMRenderMode& m,
                const KURL& documentURL );

public slots:
   void slotStop( );
   void slotSuspend( );
   void slotResume( );
   void slotClose( );
   void slotSave( );
   void slotPovrayOutput( );

protected slots:
   void slotRenderingFinished( int returnStatus );
   void slotProgress( int i );
   void slotLineFinished( int line );
   void slotUpdateSpeed( );
   
protected:
   void showSpeed( double pps );
   
private:
   PMPovrayRenderWidget* m_pRenderWidget;
   PMPovrayOutputWidget* m_pPovrayOutputWidget;
   TQPushButton* m_pStopButton;
   TQPushButton* m_pSuspendButton;
   TQPushButton* m_pResumeButton;
   TQPushButton* m_pSaveButton;
   TQPushButton* m_pPovrayOutputButton;
   TQProgressBar* m_pProgressBar;
   TQLabel* m_pProgressLabel;
   TQScrollView* m_pScrollView;
   int m_height, m_width;
   bool m_bRunning;
   TQTime m_lastSpeedTime;
   TQTimer* m_pProgressTimer;
   bool m_speedInfo;
   bool m_immediateUpdate;
   float m_speed;
   int m_line;
   bool m_stopped;
   static bool s_imageFormatsRegistered;
};

#endif
