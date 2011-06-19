/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Andreas Zehender
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


#include "pmfiledialog.h"
#include "pmpart.h"

#include <kfilefiltercombo.h>


PMFileDialog::PMFileDialog( const TQString& startDir, const TQString& filter, TQWidget* tqparent, const char* name, bool modal )
      : KFileDialog( startDir, filter, tqparent, name, modal )
{

}

TQString PMFileDialog::getImportFileName( TQWidget* tqparent, PMPart* part,
                                         PMIOFormat*& format )
{
   PMIOManager* manager = part->ioManager( );
   TQString filter;
   TQPtrListIterator<PMIOFormat> it( manager->formats( ) );
   TQPtrList<PMIOFormat> formats;

   for( ; it.current( ); ++it )
   {
      if( it.current( )->services( ) & PMIOFormat::Import )
      {
         TQStringList patterns = it.current( )->importPatterns( );
         TQStringList::Iterator pit;
         for( pit = patterns.begin( ); pit != patterns.end( ); ++pit )
         {
            if( !filter.isEmpty( ) )
               filter += "\n";
            filter += *pit;
            formats.append( it.current( ) );
         }
      }
   }

   PMFileDialog dlg( TQString(), filter, tqparent, "import file dialog", true );
   dlg.setOperationMode( Opening );
   dlg.setMode( KFile::File | KFile::LocalOnly );
   dlg.setCaption( i18n( "Import" ) );
   dlg.filterWidget->setEditable( false );
   dlg.exec( );

   format = formats.at( dlg.filterWidget->currentItem( ) );

   return dlg.selectedFile( );
}

TQString PMFileDialog::getExportFileName( TQWidget* tqparent, PMPart* part,
                                         PMIOFormat*& format, TQString& selectedFilter )
{
   PMIOManager* manager = part->ioManager( );
   TQString filter;
   TQPtrListIterator<PMIOFormat> it( manager->formats( ) );
   TQPtrList<PMIOFormat> formats;

   for( ; it.current( ); ++it )
   {
      if( it.current( )->services( ) & PMIOFormat::Export )
      {
         TQStringList patterns = it.current( )->exportPatterns( );
         TQStringList::Iterator pit;
         for( pit = patterns.begin( ); pit != patterns.end( ); ++pit )
         {
            if( !filter.isEmpty( ) )
               filter += "\n";
            filter += *pit;
            formats.append( it.current( ) );
         }
      }
   }

   PMFileDialog dlg( TQString(), filter, tqparent, "export file dialog", true );
   dlg.setOperationMode( Saving );
   dlg.setMode( KFile::File | KFile::LocalOnly );
   dlg.setCaption( i18n( "Export" ) );
   dlg.filterWidget->setEditable( false );
   dlg.exec( );

   format = formats.at( dlg.filterWidget->currentItem( ) );
   selectedFilter = dlg.currentFilter( );

   return dlg.selectedFile( );
}

#include "pmfiledialog.moc"
