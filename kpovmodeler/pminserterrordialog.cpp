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


#include "pminserterrordialog.h"
#include <tdelocale.h>
#include <tqlabel.h>
#include <tqlistbox.h>
#include <tqvbox.h>

PMInsertErrorDialog::PMInsertErrorDialog(
   int numObj, int numErrors,
   const TQStringList& details,
   TQWidget* parent /*= 0*/, const char* name /*= 0*/ )
      : KDialogBase( parent, name, true, i18n( "Insert Errors" ),
                     Help | Ok | User1, Ok, false, i18n( "Details" ) )
{
   TQVBox* page = makeVBoxMainWidget( );
   new TQLabel( i18n( "%1 of %2 objects couldn't be inserted." )
               .arg( numErrors ).arg( numObj ), page );
   
   m_pDetailsLabel = new TQLabel( i18n( "Objects not inserted:" ), page );
   m_pDetailsLabel->hide( );
   
   m_pDetails = new TQListBox( page );
   m_pDetails->insertStringList( details, 0 );
   m_pDetails->setMinimumHeight( 150 );
   m_pDetails->hide( );

}

void PMInsertErrorDialog::slotUser1( )
{
   m_pDetailsLabel->show( );
   m_pDetails->show( );
}
#include "pminserterrordialog.moc"
