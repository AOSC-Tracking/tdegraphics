/***************************************************************************
                          imgnamecombo.cpp - combobox for image names 
                             -------------------                                         
    begin                : Tue Nov 13 2001
    copyright            : (C) 2001 by Klaas Freitag                         
    email                : freitag@suse.de
 ***************************************************************************/


/***************************************************************************
 *                                                                         *
 *  This file may be distributed and/or modified under the terms of the    *
 *  GNU General Public License version 2 as published by the Free Software *
 *  Foundation and appearing in the file COPYING included in the           *
 *  packaging of this file.                                                *
 *
 *  As a special exception, permission is given to link this program       *
 *  with any version of the KADMOS ocr/icr engine of reRecognition GmbH,   *
 *  Kreuzlingen and distribute the resulting executable without            *
 *  including the source code for KADMOS in the source distribution.       *
 *
 *  As a special exception, permission is given to link this program       *
 *  with any edition of TQt, and distribute the resulting executable,       *
 *  without including the source code for TQt in the source distribution.   *
 *                                                                         *
 ***************************************************************************/

#include <tqlayout.h>
#include <tqlabel.h>
#include <tqlistview.h>

#include <kcombobox.h>

#include <kdebug.h>
#include <klocale.h>
#include <kfiletreebranch.h>

#include "imgnamecombo.h"
#include "img_saver.h"

ImageNameCombo::ImageNameCombo( TQWidget *parent )
   : KComboBox( parent )
{
   setInsertionPolicy( TQComboBox::AtTop );
}

ImageNameCombo::~ImageNameCombo()
{
   
}

void ImageNameCombo::slotPathRemove( KFileTreeBranch *branch, const TQString& relPath )
{
   TQString path = branch->name() + TQString::tqfromLatin1(" - ") + relPath;

   kdDebug(28000) << "ImageNameCombo: Removing " << path << endl;
   TQString select = currentText();
   
   if( items.contains( path ))
   {
      kdDebug(28000) << "ImageNameCombo: Item exists-> deleting" << endl;
      items.remove( path );
   }

   /* */
   rewriteList( branch, select );
}

void ImageNameCombo::rewriteList( KFileTreeBranch *branch, const TQString& selText )
{
   clear();
   for ( TQStringList::Iterator it = items.begin(); it != items.end(); ++it )
   {
      insertItem( branch->pixmap(), *it );
   }

   int index = items.findIndex( selText );
   setCurrentItem( index );
}

void ImageNameCombo::slotGalleryPathChanged( KFileTreeBranch* branch, const TQString& relativPath )
{
   TQString newPath;

   newPath = branch->name() + TQString::tqfromLatin1(" - ") + relativPath;

   kdDebug( 28000) << "Inserting " << newPath << " to combobox" << endl;

   setCurrentItem( newPath, true /* insert if missing */ );
}

/* The End */
#include "imgnamecombo.moc"
