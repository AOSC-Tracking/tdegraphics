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


#ifndef PMERRORDIALOG_H
#define PMERRORDIALOG_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif 

#include <kdialogbase.h>
#include <tqvaluelist.h>
#include <tqptrdict.h>
#include <tqptrlist.h>

class KConfig;
class TQTextEdit;
class PMObject;

#include "pmmessage.h"

/**
 * Dialog that is shown if some errors or warnings occurred during
 * parsing or execution of commands.
 */
class PMErrorDialog : public KDialogBase
{
   Q_OBJECT
  
public:
   /**
    * Creates a modal PMErrorDialog with parent and name.
    *
    * messages is the message list. If the list contains a message of type
    * FatalError, the 'Proceed" button will not be shown.
    *
    * PMErrorDialog::exec( ) returns TQDialog::Accepted if the command
    * should be continued.
    */
   PMErrorDialog( const PMMessageList& messages, int errorFlags,
                  TQWidget* parent = 0, const char* name = 0 );

   static void saveConfig( KConfig* cfg );
   static void restoreConfig( KConfig* cfg );

protected:
   virtual void resizeEvent( TQResizeEvent* ev );
   
private:
   void displayMessages( );
   
   static TQSize s_size;
   TQTextEdit* m_pTextView;
   TQPtrDict< TQPtrList<PMMessage> > m_messageDict;
   TQPtrList<PMMessage> m_messages;
};

#endif
