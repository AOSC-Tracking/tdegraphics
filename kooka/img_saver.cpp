/***************************************************************************
                          img_saver.cpp  -  description
                             -------------------
    begin                : Mon Dec 27 1999
    copyright            : (C) 1999 by Klaas Freitag
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

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>


#include <tdeglobal.h>
#include <tdeconfig.h>
#include <kdialog.h>
#include <kimageio.h>
#include <kseparator.h>
#include <tdelocale.h>
#include <tdemessagebox.h>
#include <kdebug.h>
#include <tdeio/jobclasses.h>
#include <tdeio/file.h>
#include <tdeio/job.h>
#include <tdeio/netaccess.h>
#include <tdetempfile.h>
#include <kinputdialog.h>

#include <tqdir.h>
#include <tqlayout.h>
#include <tqfileinfo.h>
#include <tqimage.h>
#include <tqmessagebox.h>
#include <tqvbox.h>
#include <tqbuttongroup.h>

#include "resource.h"
#include "img_saver.h"
#include "previewer.h"
#include "kookaimage.h"

FormatDialog::FormatDialog( TQWidget *parent, const TQString&, const char *name )
   :KDialogBase( parent, name, true,
                 /* Tabbed,*/ i18n( "Kooka Save Assistant" ),
		 Ok|Cancel, Ok )

{
   buildHelp();
   // readConfig();
   // TQFrame *page = addPage( TQString( "Save the image") );
   TQFrame *page = new TQFrame( this );
   page->setFrameStyle( TQFrame::Box | TQFrame::Sunken );
   TQ_CHECK_PTR( page );
   setMainWidget( page );

   TQVBoxLayout *bigdad = new TQVBoxLayout( page, marginHint(), spacingHint());
   TQ_CHECK_PTR(bigdad);

   // some nice words
   TQLabel *l0 = new TQLabel( page );
   TQ_CHECK_PTR(l0);
   l0->setText( i18n( "<B>Save Assistant</B><P>Select an image format to save the scanned image." ));
   bigdad->addWidget( l0 );

   KSeparator* sep = new KSeparator( KSeparator::HLine, page);
   bigdad->addWidget( sep );

   // Layout-Boxes
   // TQHBoxLayout *hl1= new TQHBoxLayout( );  // Caption
   TQHBoxLayout *lhBigMiddle = new TQHBoxLayout( spacingHint() );  // Big middle
   TQ_CHECK_PTR(lhBigMiddle);
   bigdad->addLayout( lhBigMiddle );
   TQVBoxLayout *lvFormatSel = new TQVBoxLayout( spacingHint() );  // Selection List
   TQ_CHECK_PTR(lvFormatSel);
   lhBigMiddle->addLayout( lvFormatSel );

   // Insert Scrolled List for formats
   TQLabel *l1 = new TQLabel( page );
   TQ_CHECK_PTR(l1);
   l1->setText( i18n( "Available image formats:" ));

   lb_format = new TQListBox( page, "ListBoxFormats" );
   TQ_CHECK_PTR(lb_format);

#ifdef USE_KIMAGEIO
   TQStringList fo = KImageIO::types();
#else
   TQStringList fo = TQImage::outputFormatList();
#endif
   kdDebug(28000) << "#### have " << fo.count() << " image types" << endl;
   lb_format->insertStringList( fo );
   connect( lb_format, TQ_SIGNAL( highlighted(const TQString&)),
	    TQ_SLOT( showHelp(const TQString&)));

   // Insert label for helptext
   l_help = new TQLabel( page );
   TQ_CHECK_PTR(l_help);
   l_help->setFrameStyle( TQFrame::Panel|TQFrame::Sunken );
   l_help->setText( i18n("-No format selected-" ));
   l_help->setAlignment( AlignVCenter | AlignHCenter );
   l_help->setMinimumWidth(230);

   // Insert Selbox for subformat
   l2 = new TQLabel( page );
   TQ_CHECK_PTR(l2);
   l2->setText( i18n( "Select the image sub-format" ));
   cb_subf = new TQComboBox( page, "ComboSubFormat" );
   TQ_CHECK_PTR( cb_subf );

   // Checkbox to store setting
   cbDontAsk  = new TQCheckBox(i18n("Don't ask again for the save format if it is defined."),
			      page );
   TQ_CHECK_PTR( cbDontAsk );

   TQFrame *hl = new TQFrame(page);
   TQ_CHECK_PTR( hl );
   hl->setFrameStyle( TQFrame::HLine|TQFrame::Sunken );

   // bigdad->addWidget( l_caption, 1 );
   lvFormatSel->addWidget( l1, 1 );
   lvFormatSel->addWidget( lb_format, 6 );
   lvFormatSel->addWidget( l2, 1 );
   lvFormatSel->addWidget( cb_subf, 1 );

   lhBigMiddle->addWidget( l_help, 2 );
   //bigdad->addStretch(1);
   bigdad->addWidget( hl, 1 );
   bigdad->addWidget( cbDontAsk , 2 );

   bigdad->activate();

}

void FormatDialog::showHelp( const TQString& item )
{
   TQString helptxt = format_help[ item ];

   if( !helptxt.isEmpty() ) {
      // Set the hint
      l_help->setText( helptxt );

      // and check subformats
      check_subformat( helptxt );
   } else {
      l_help->setText( i18n("-no hint available-" ));
   }
}

void FormatDialog::check_subformat( const TQString & format )
{
   // not yet implemented
   kdDebug(28000) << "This is format in check_subformat: " << format << endl;
   cb_subf->setEnabled( false );
   // l2 = Label "select subformat" ->bad name :-|
   l2->setEnabled( false );
}

void FormatDialog::setSelectedFormat( TQString fo )
{
   TQListBoxItem *item = lb_format->findItem( fo );

   if( item )
   {
      // Select it.
      lb_format->setSelected( lb_format->index(item), true );
   }
}


TQString FormatDialog::getFormat( ) const
{
   int item = lb_format->currentItem();

   if( item > -1 )
   {
      const TQString f = lb_format->text( item );
      return( f );
   }
   return( "BMP" );
}


TQCString FormatDialog::getSubFormat( ) const
{
   // Not yet...
   return( "" );
}

#include "formathelp.h"
void FormatDialog::buildHelp( void )
{
   format_help.insert( TQString::fromLatin1("BMP"), HELP_BMP );
   format_help.insert( TQString::fromLatin1("PNM"), HELP_PNM );
   format_help.insert( TQString::fromLatin1("JPEG"), HELP_JPG );
   format_help.insert( TQString::fromLatin1("JPG"), HELP_JPG );
   format_help.insert( TQString::fromLatin1("EPS"), HELP_EPS );
}


/* ********************************************************************** */

ImgSaver::ImgSaver(  TQWidget *parent, const KURL dir_name )
   : TQObject( parent )
{

   if( dir_name.isEmpty() || dir_name.protocol() != "file" )
   {
      kdDebug(28000) << "ImageServer initialised with wrong dir " << dir_name.url() << endl;
      directory = Previewer::galleryRoot();
   }
   else
   {
      /* A path was given */
      if( dir_name.protocol() != "file"  )
      {
	 kdDebug(28000) << "ImgSaver: Can only save local image, sorry !" << endl;
      }
      else
      {
	 directory = dir_name.directory(true, false);
      }
   }

   kdDebug(28000) << "ImageSaver uses dir <" << directory << endl;
   createDir( directory );
   readConfig();

   last_file = "";
   last_format ="";

}


ImgSaver::ImgSaver( TQWidget *parent )
   :TQObject( parent )
{
   directory = Previewer::galleryRoot();
   createDir( directory );

   readConfig();

   last_file = "";
   last_format ="";

}


/* Needs a full qualified directory name */
void ImgSaver::createDir( const TQString& dir )
{
   KURL url( dir );

   if( ! TDEIO::NetAccess::exists(url, false, 0) )
   {
      kdDebug(28000) << "Wrn: Directory <" << dir << "> does not exist -> try to create  !" << endl;
      // if( mkdir( TQFile::encodeName( dir ), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH ) != 0 )
      if( TDEIO::mkdir( KURL(dir)))
      {
        KMessageBox::sorry(0, i18n("The folder\n%1\n does not exist and could not be created;\n"
                        "please check the permissions.").arg(dir));
      }
   }
#if 0
   if( ! fi.isWritable() )
   {
        KMessageBox::sorry(0, i18n("The directory\n%1\n is not writeable;\nplease check the permissions.")
                .arg(dir));
   }
#endif
}

/**
 *   This function asks the user for a filename or creates
 *   one by itself, depending on the settings
 **/
ImgSaveStat ImgSaver::saveImage( TQImage *image )
{
   ImgSaveStat stat;
   picType imgType;

   if( !image ) return( ISS_ERR_PARAM );

   /* Find out what kind of image it is  */
   if( image->depth() > 8 )
   {
      imgType = PT_HICOLOR_IMAGE;
   }
   else
   {
      if( image->depth() == 1 || image->numColors() == 2 )
      {
	 kdDebug(28000) << "This is black And White!" << endl;
	 imgType = PT_BW_IMAGE;
      }
      else
      {
	 imgType  = PT_COLOR_IMAGE;
	 if( image->allGray() )
	 {
	    imgType = PT_GRAY_IMAGE;
	 }
      }
   }


   TQString format = findFormat( imgType );
   TQString subformat = findSubFormat( format );
   // Call save-Function with this params

   if( format.isEmpty() )
   {
    	kdDebug(28000) << "Save canceled by user -> no save !" << endl;
    	return( ISS_SAVE_CANCELED );
   }

   kdDebug(28000) << "saveImage: Directory is " << directory << endl;
   TQString filename = createFilename( format );

   TDEConfig *konf = TDEGlobal::config ();
   konf->setGroup( OP_FILE_GROUP );

   if( konf->readBoolEntry( OP_ASK_FILENAME, false ) )
   {
       bool ok;
       TQString text = KInputDialog::getText( i18n( "Filename" ), i18n("Enter filename:"),
					     filename, &ok );

       if(ok)
       {
	   filename = text;
       }
   }
       
   TQString fi = directory + "/" + filename;

   if( extension(fi).isEmpty() )
   {
       if( ! fi.endsWith( "." )  )
       {
	   fi+= ".";
       }
       fi+=format.lower();
   }


   kdDebug(28000) << "saveImage: saving file <" << fi << ">" << endl;
   stat = save( image, fi, format, subformat );

   return( stat );

}

/**
 *  This member creates a filename for the image to save.
 *  This is done by numbering all existing files and adding
 *  one
 **/
TQString ImgSaver::createFilename( TQString format )
{
    if( format.isNull() || format.isEmpty() ) return( 0 );

    TQString s = "kscan_*." + format.lower();
    TQDir files( directory, s );
    long c = 1;

    TQString num;
    num.setNum(c);
    TQString fname = "kscan_" + num.rightJustify(4, '0') + "." + format.lower();

    while( files.exists( fname ) ) {
        num.setNum(++c);
        fname = "kscan_" + num.rightJustify(4, '0') + "." + format.lower();
    }

    return( fname );
}

/**
 *   This function gets a filename from the parent. The filename must not be relative.
 **/
ImgSaveStat ImgSaver::saveImage( TQImage *image, const KURL& filename, const TQString& imgFormat )
{
    TQString format = imgFormat;

    /* Check if the filename is local */
    if( !filename.isLocalFile())
    {
	kdDebug(29000) << "ImgSaver: Can only save local image, sorry !" << endl;
	return( ISS_ERR_PROTOCOL );
    }

    TQString localFilename;
    localFilename = filename.directory( false, true) + filename.fileName();

    kdDebug(28000) << "saveImage: Saving "<< localFilename << " in format " << format << endl;
    if( format.isEmpty() )
	format = "BMP";

    return( save( image, localFilename, format, "" ) );
}


/*
 * findFormat does all the stuff with the dialog.
 */
TQString ImgSaver::findFormat( picType type )
{
   TQString format;
   TDEConfig *konf = TDEGlobal::config ();
   konf->setGroup( OP_FILE_GROUP );

   if( type == PT_THUMBNAIL )
   {
      return( "BMP" );
   }

   // real images
   switch( type )
   {
      case PT_THUMBNAIL:
	 format = konf->readEntry( OP_FORMAT_THUMBNAIL, "BMP" );
	 kdDebug( 28000) << "Format for Thumbnails: " << format << endl;
	 break;
      case PT_PREVIEW:
	 format = konf->readEntry( OP_PREVIEW_FORMAT, "BMP" );
	 kdDebug( 28000) << "Format for Preview: " << format << endl;
	 break;
      case PT_COLOR_IMAGE:
	 format = konf->readEntry( OP_FORMAT_COLOR, "nothing" );
	 kdDebug( 28000 ) <<  "Format for Color: " << format << endl;
	 break;
      case PT_GRAY_IMAGE:
	 format = konf->readEntry( OP_FORMAT_GRAY, "nothing" );
	 kdDebug( 28000 ) <<  "Format for Gray: " << format << endl;
	 break;
      case PT_BW_IMAGE:
	 format = konf->readEntry( OP_FORMAT_BW, "nothing" );
	 kdDebug( 28000 ) <<  "Format for BlackAndWhite: " << format << endl;
	 break;
      case PT_HICOLOR_IMAGE:
	 format = konf->readEntry( OP_FORMAT_HICOLOR, "nothing" );
	 kdDebug( 28000 ) <<  "Format for HiColorImage: " << format << endl;
	 break;
      default:
	 format = "nothing";
	 kdDebug( 28000 ) <<  "ERR: Could not find image type !" << endl;

	 break;
   }

   if( type != PT_PREVIEW ) /* Use always bmp-Default for preview scans */
   {
      if( format == "nothing" || ask_for_format )
      {
	 format = startFormatDialog( type );
      }
   }
   return( format );

}

TQString ImgSaver::picTypeAsString( picType type ) const
{
   TQString res;

   switch( type )
   {
      case PT_COLOR_IMAGE:
	 res = i18n( "palleted color image (16 or 24 bit depth)" );
	 break;
      case PT_GRAY_IMAGE:
	 res = i18n( "palleted gray scale image (16 bit depth)" );
	 break;
      case PT_BW_IMAGE:
	 res = i18n( "lineart image (black and white, 1 bit depth)" );
	 break;
      case PT_HICOLOR_IMAGE:
	 res = i18n( "high (or true-) color image, not palleted" );
	 break;
      default:
	 res = i18n( "Unknown image type" );
	 break;
   }
   return( res );
}


TQString ImgSaver::startFormatDialog( picType type)
{

   FormatDialog fd( 0, picTypeAsString( type ), "FormatDialog" );

   // set default values
   if( type != PT_PREVIEW )
   {
      TQString defFormat = getFormatForType( type );
      fd.setSelectedFormat( defFormat );
   }

   TQString format;
   if( fd.exec() )
   {
      format = fd.getFormat();
      kdDebug(28000) << "Storing to format <" << format << ">" << endl;
      bool ask = fd.askForFormat();
      kdDebug(28000)<< "Store askFor is " << ask << endl;
      storeFormatForType( type, format, ask );
      subformat = fd.getSubFormat();
   }
   return( format );
}


/*
 *  This method returns true if the image format given in format is remembered
 *  for that image type.
 */
bool ImgSaver::isRememberedFormat( picType type, TQString format ) const
{
   if( getFormatForType( type ) == format )
   {
      return( true );
   }
   else
   {
      return( false );
   }

}




TQString ImgSaver::getFormatForType( picType type ) const
{
   TDEConfig *konf = TDEGlobal::config ();
   TQ_CHECK_PTR( konf );
   konf->setGroup( OP_FILE_GROUP );

   TQString f;

   switch( type )
   {
      case PT_COLOR_IMAGE:
	 f = konf->readEntry( OP_FORMAT_COLOR, "BMP" );
	 break;
      case PT_GRAY_IMAGE:
	 f = konf->readEntry( OP_FORMAT_GRAY, "BMP" );
	 break;
      case PT_BW_IMAGE:
	 f = konf->readEntry( OP_FORMAT_BW, "BMP" );
	 break;
      case PT_HICOLOR_IMAGE:
	 f = konf->readEntry( OP_FORMAT_HICOLOR, "BMP" );
	 break;
      default:
	 f = "BMP";
	 break;
   }
   return( f );
}


void ImgSaver::storeFormatForType( picType type, TQString format, bool ask )
{
   TDEConfig *konf = TDEGlobal::config ();
   TQ_CHECK_PTR( konf );
   konf->setGroup( OP_FILE_GROUP );

   konf->writeEntry( OP_FILE_ASK_FORMAT, ask );
   ask_for_format = ask;

   switch( type )
   {
      case PT_COLOR_IMAGE:
	 konf->writeEntry( OP_FORMAT_COLOR, format );
	 break;
      case PT_GRAY_IMAGE:
	 konf->writeEntry( OP_FORMAT_GRAY, format  );
	 break;
      case PT_BW_IMAGE:
	 konf->writeEntry( OP_FORMAT_BW, format );
	 break;
      case PT_HICOLOR_IMAGE:
	 konf->writeEntry( OP_FORMAT_HICOLOR, format );
	 break;
      default:
	 kdDebug(28000) << "Wrong Type  - cant store format setting" << endl;
	 break;
   }
   konf->sync();
}


TQString ImgSaver::findSubFormat( TQString format )
{
   kdDebug(28000) << "Searching Subformat for " << format << endl;
   return( subformat );

}

/**
   private save() does the work to save the image.
   the filename must be complete and local.
**/
ImgSaveStat ImgSaver::save( TQImage *image, const TQString &filename,
			    const TQString &format,
			    const TQString &subformat )
{

   bool result = false;
   kdDebug(28000) << "in ImgSaver::save: saving " << filename << endl;
   if( ! format || !image )
   {
      kdDebug(28000) << "ImgSaver ERROR: Wrong parameter Format <" << format << "> or image" << endl;
      return( ISS_ERR_PARAM );
   }

   if( image )
   {
      // remember the last processed file - only the filename - no path
      TQFileInfo fi( filename );
      TQString dirPath = fi.dirPath();
      TQDir dir = TQDir( dirPath );

      if( ! dir.exists() )
      {
	 /* The dir to save in always should exist, except in the first preview save */
	 kdDebug(28000) << "Creating dir " << dirPath << endl;
	 if( !dir.mkdir( dirPath ) )
	 {
	    kdDebug(28000) << "ERR: Could not create directory" << endl;
	 }
      }

      if( fi.exists() && !fi.isWritable() )
      {
	 kdDebug(28000) << "Cant write to file <" << filename << ">, cant save !" << endl;
	 result = false;
	 return( ISS_ERR_PERM );
      }

      /* Check the format, is it writable ? */
#ifdef USE_KIMAGEIO
      if( ! KImageIO::canWrite( format ) )
      {
	 kdDebug(28000) << "Cant write format <" << format << ">" << endl;
	 result = false;
	 return( ISS_ERR_FORMAT_NO_WRITE );
      }
#endif
      kdDebug(28000) << "ImgSaver: saving image to <" << filename << "> as <" << format << "/" << subformat <<">" << endl;

      result = image->save( filename, format.latin1() );


      last_file = fi.absFilePath();
      last_format = format.latin1();
   }

   if( result )
      return( ISS_OK );
   else {
      last_file = "";
      last_format = "";
      return( ISS_ERR_UNKNOWN );
   }

}


void ImgSaver::readConfig( void )
{

   TDEConfig *konf = TDEGlobal::config ();
   TQ_CHECK_PTR( konf );
   konf->setGroup( OP_FILE_GROUP );
   ask_for_format = konf->readBoolEntry( OP_FILE_ASK_FORMAT, true );

   TQDir home = TQDir::home();
}





TQString ImgSaver::errorString( ImgSaveStat stat )
{
   TQString re;

   switch( stat ) {
      case ISS_OK:           re = i18n( " image save OK      " ); break;
      case ISS_ERR_PERM:     re = i18n( " permission error   " ); break;
      case ISS_ERR_FILENAME: re = i18n( " bad filename       " ); break;
      case ISS_ERR_NO_SPACE: re = i18n( " no space on device " ); break;
      case ISS_ERR_FORMAT_NO_WRITE: re = i18n( " could not write image format " ); break;
      case ISS_ERR_PROTOCOL: re = i18n( " can not write file using that protocol "); break;
      case ISS_SAVE_CANCELED: re = i18n( " user canceled saving " ); break;
      case ISS_ERR_UNKNOWN:  re = i18n( " unknown error      " ); break;
      case ISS_ERR_PARAM:    re = i18n( " parameter wrong    " ); break;

      default: re = "";
   }
   return( re );

}

TQString ImgSaver::extension( const KURL& url )
{
   TQString extension = url.fileName();

   int dotPos = extension.findRev( '.' );
   if( dotPos > 0 )
   {
      int len = extension.length();
      extension = extension.right( len - dotPos -1 );
   }
   else
   {
      /* No extension was supplied */
      extension = TQString();
   }
   return extension;
}


bool ImgSaver::renameImage( const KURL& fromUrl, KURL& toUrl, bool askExt,  TQWidget *overWidget )
{
   /* Check if the provided filename has a extension */
   TQString extTo = extension( toUrl );
   TQString extFrom = extension( fromUrl );
   KURL targetUrl( toUrl );

   if( extTo.isEmpty() && !extFrom.isEmpty() )
   {
      /* Ask if the extension should be added */
      int result = KMessageBox::Yes;
      TQString fName = toUrl.fileName();
      if( ! fName.endsWith( "." )  )
      {
	 fName += ".";
      }
      fName += extFrom;

      if( askExt )
      {

	 TQString s;
	 s = i18n("The filename you supplied has no file extension.\nShould the correct one be added automatically? ");
	 s += i18n( "That would result in the new filename: %1" ).arg( fName);

	 result = KMessageBox::questionYesNo(overWidget, s, i18n( "Extension Missing"),
					     i18n("Add Extension"), i18n("Do Not Add"),
					     "AutoAddExtensions" );
      }

      if( result == KMessageBox::Yes )
      {
	 targetUrl.setFileName( fName );
	 kdDebug(28000) << "Rename file to " << targetUrl.prettyURL() << endl;
      }
   }
   else if( !extFrom.isEmpty() && extFrom != extTo )
   {
       if( ! ((extFrom.lower() == "jpeg" && extTo.lower() == "jpg") ||
	      (extFrom.lower() == "jpg"  && extTo.lower() == "jpeg" )))
       {
	   /* extensions differ -> TODO */
	   KMessageBox::error( overWidget,
			       i18n("Format changes of images are currently not supported."),
			       i18n("Wrong Extension Found" ));
	   return(false);
       }
   }

   bool success = false;

   if( TDEIO::NetAccess::exists( targetUrl, false,0 ) )
   {
      kdDebug(28000)<< "Target already exists - can not copy" << endl;
   }
   else
   {
      if( TDEIO::file_move(fromUrl, targetUrl) )
      {
	 success = true;
      }
   }
   return( success );
}


TQString ImgSaver::tempSaveImage( KookaImage *img, const TQString& format, int colors )
{

    KTempFile *tmpFile = new KTempFile( TQString(), "."+format.lower());
    tmpFile->setAutoDelete( false );
    tmpFile->close();

    KookaImage tmpImg;

    if( colors != -1 && img->numColors() != colors )
    {
	// Need to convert image
	if( colors == 1 || colors == 8 || colors == 24 || colors == 32 )
	{
	    tmpImg = img->convertDepth( colors );
	    img = &tmpImg;
	}
	else
	{
	    kdDebug(29000) << "ERROR: Wrong color depth requested: " << colors << endl;
	    img = 0;
	}
    }

    TQString name;
    if( img )
    {
	name = tmpFile->name();

	if( ! img->save( name, format.latin1() ) ) name = TQString();
    }
    delete tmpFile;
    return name;
}

bool ImgSaver::copyImage( const KURL& fromUrl, const KURL& toUrl, TQWidget *overWidget )
{

   /* Check if the provided filename has a extension */
   TQString extTo = extension( toUrl );
   TQString extFrom = extension( fromUrl );
   KURL targetUrl( toUrl );

   if( extTo.isEmpty() && !extFrom.isEmpty())
   {
      /* Ask if the extension should be added */
      int result = KMessageBox::Yes;
      TQString fName = toUrl.fileName();
      if( ! fName.endsWith( "." ))
	 fName += ".";
      fName += extFrom;

      TQString s;
      s = i18n("The filename you supplied has no file extension.\nShould the correct one be added automatically? ");
      s += i18n( "That would result in the new filename: %1" ).arg( fName);

      result = KMessageBox::questionYesNo(overWidget, s, i18n( "Extension Missing"),
					  i18n("Add Extension"), i18n("Do Not Add"),
					  "AutoAddExtensions" );

      if( result == KMessageBox::Yes )
      {
	 targetUrl.setFileName( fName );
      }
   }
   else if( !extFrom.isEmpty() && extFrom != extTo )
   {
      /* extensions differ -> TODO */
       if( ! ((extFrom.lower() == "jpeg" && extTo.lower() == "jpg") ||
	      (extFrom.lower() == "jpg"  && extTo.lower() == "jpeg" )))
       {
	   KMessageBox::error( overWidget, i18n("Format changes of images are currently not supported."),
			       i18n("Wrong Extension Found" ));
	   return(false);
       }
   }

   TDEIO::Job *copyjob = TDEIO::copy( fromUrl, targetUrl, false );

   return( copyjob ? true : false );
}


/* extension needs to be added */

#include "img_saver.moc"
