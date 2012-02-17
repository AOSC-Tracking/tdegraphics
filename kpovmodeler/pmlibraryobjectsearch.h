//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Luis Carvalho
    email                : lpassos@mail.telepac.pt
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/

#ifndef PMLIBRARYOBJECTSEARCH_H
#define PMLIBRARYOBJECTSEARCH_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tqwidget.h>

class TQLineEdit;
class TQListBox;
class TQPushButton;
class PMLibraryEntryPreview;
class KListView;

/**
 * Search widget for Library Objects.
 * It also provides drag. If the user doesn't have a clear idea
 * where the objects he wants are, this is the dialog to use.
 */
class PMLibraryObjectSearch: public TQWidget
{
   Q_OBJECT
  
public:
   PMLibraryObjectSearch( TQWidget *parent );

private slots:
   void slotSearchButtonPressed( );

private:
   TQLineEdit*   m_pSearch;
   TQPushButton* m_pSearchButton;
   KListView* m_pFileList;
   PMLibraryEntryPreview* m_pPreview;
};

#endif
