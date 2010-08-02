//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2002 by Andreas Zehender
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


#ifndef PMACTIONS_H
#define PMACTIONS_H

#include <tqguardedptr.h>
#include <kaction.h>

class TQComboBox;
class TQSpinBox;
class TQLabel;
class TQToolButton;

/**
 * Combobox action for the toolbar.
 *
 * Copied from konq_actions.h, author: Simon Hausmann <hausmann@kde.org>
 */
class PMComboAction : public KAction
{
   Q_OBJECT
public:
   PMComboAction( const TQString& text, int accel, const TQObject* receiver, const char* member, TQObject* parent, const char* name );
   ~PMComboAction( );
   
   virtual int plug( TQWidget* w, int index = -1 );
   
   virtual void unplug( TQWidget* w );
   
   TQGuardedPtr<TQComboBox> combo( ) { return m_combo; }

   void setMaximumWidth( int w ) { m_maxWidth = w; }
   void setMinimumWidth( int w ) { m_minWidth = w; }
   
signals:
   void plugged( );
   
private:
   TQGuardedPtr<TQComboBox> m_combo;
   const TQObject* m_receiver;
   const char* m_member;
   int m_minWidth, m_maxWidth;
};

/**
 * Label action for the toolbar.
 *
 * Copied from konq_actions.h, author: Simon Hausmann <hausmann@kde.org>
 */
class PMLabelAction : public KAction
{
   Q_OBJECT
public:
   PMLabelAction( const TQString &text, TQObject *parent = 0, const char *name = 0 );
   
   virtual int plug( TQWidget *widget, int index = -1 );
   virtual void unplug( TQWidget *widget );
   TQToolButton* button( ) { return m_button; }
   
private:
   TQToolButton* m_button;
};

/**
 * Spinbox action for the toolbar.
 */
class PMSpinBoxAction : public KAction
{
   Q_OBJECT
public:
   PMSpinBoxAction( const TQString& text, int accel, const TQObject* receiver, const char* member, TQObject* parent, const char* name );
   ~PMSpinBoxAction( );
   
   virtual int plug( TQWidget* w, int index = -1 );
   virtual void unplug( TQWidget* w );
   
   TQGuardedPtr<TQSpinBox> spinBox( ) { return m_spinBox; }

signals:
   void plugged( );
   
private:
   TQGuardedPtr<TQSpinBox> m_spinBox;
   const TQObject* m_receiver;
   const char* m_member;
};


#endif
