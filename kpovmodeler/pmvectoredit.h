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


#ifndef PMVECTOREDIT_H
#define PMVECTOREDIT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tqwidget.h>
#include <tqptrvector.h>
#include <tqlineedit.h>
#include "pmvector.h"

/**
 * Edit widget for @ref PMVector
 */
class PMVectorEdit : public QWidget
{
   Q_OBJECT
public:
   /**
    * Creates an edit widget for 2D vectors.
    *
    * The labels shown are descriptionX and descriptionY
    */
   PMVectorEdit( const TQString& descriptionX,
                 const TQString& descriptionY,
                 TQWidget* parent, const char* name = 0 );
   /**
    * Creates an edit widget for 3D vectors.
    *
    * The labels shown are descriptionX, descriptionY and descriptionZ
    */
   PMVectorEdit( const TQString& descriptionX,
                 const TQString& descriptionY, const TQString& descriptionZ,
                 TQWidget* parent, const char* name = 0 );
   /**
    * Creates an edit widget for 4D vectors.
    *
    * The labels shown are descriptionA, descriptionB, descriptionC
    * and description D
    */
   PMVectorEdit( const TQString& descriptionA, const TQString& descriptionB,
                 const TQString& descriptionC, const TQString& descriptionD,
                 TQWidget* parent, const char* name = 0 );

   /**
    * Sets the displayed vector
    */
   void setVector( const PMVector& v, int precision = 5 );
   /**
    * Returns the displayed vector
    */
   PMVector vector( ) const;

   /**
    * Returns true if the text for each coordinate is a valid
    * float value. Otherwise an error message is shown.
    */
   bool isDataValid( );

   /**
    * Enables or disables read only mode
    */
   void setReadOnly( bool yes = true );
   /**
    * Reimplemented from QWidget
    */
   virtual void setEnabled( bool yes );
signals:
   /**
    * Emitted when one of the coordinates is changed
    */
   void dataChanged( );

protected slots:
   void slotTextChanged( const TQString& );
private:
   TQPtrVector<TQLineEdit> m_edits;
};

#endif
