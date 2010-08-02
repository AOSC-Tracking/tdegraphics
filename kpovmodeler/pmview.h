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


#ifndef PMVIEW_H
#define PMVIEW_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif 

// include files for Qt
#include <tqwidget.h>

class PMTreeView;
class PMDialogView;
class PMPart;
class TQSplitter;
class KConfig;

/**
 * The PMView class provides the view widget for the PMPart document class.
 */
class PMView : public QWidget
{
   Q_OBJECT
public:
   /**
    * Constructor for the main view
    */
   PMView( PMPart* part, TQWidget* parent = 0, const char* name = 0 );
   /**
    * Destructor for the main view
    */
   ~PMView( );

   /**
    * returns a pointer to the part
    */
   PMPart* part( ) const { return m_pPart; }

   /**
    * contains the implementation for printing functionality TODO*/
   void print( TQPrinter* pPrinter );
   /**
    * saves settings
    */
   void saveConfig( KConfig* cfg );
   /**
    * loads settings
    */
   void restoreConfig( KConfig* cfg );
private:
   PMTreeView* m_pTreeView;
   PMPart* m_pPart;
   PMDialogView* m_pDialogView;
   TQSplitter* m_pMainSplitter;
   TQSplitter* m_pTreeEditSplitter;
};

#endif
