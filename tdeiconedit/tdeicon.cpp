/*
    TDE Icon Editor - a small graphics drawing program for the TDE
    Copyright (C) 1998  Thomas Tanghus (tanghus@kde.org)

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/  

#include <stdlib.h>

#include <tqimage.h>

#include <tdemessagebox.h>
#include <tdefiledialog.h>
#include <tdeio/netaccess.h>
#include <kimageio.h>
#include <kdebug.h>
#include <tdelocale.h>

#include "tdeicon.h"
#include "utils.h"

TDEIconEditIcon::TDEIconEditIcon(TQObject *parent, const TQImage *img, KURL url) 
   : TQObject(parent)
{
    f = 0;
    _lastdir = "/";
    //checkUnNamedBackup(img);

    if(!url.isEmpty())
    {
        open(img, url);
    }
}


TDEIconEditIcon::~TDEIconEditIcon()
{
}


bool TDEIconEditIcon::open(const TQImage *image, KURL url)
{
    TQImage *img = (TQImage*)image;

    if(url.isEmpty())
        return false;

    kdDebug(4640) << "TDEIconEditIcon::open " << url.prettyURL() << endl;
    
    if(!url.isValid()) // try to see if it is a relative filename
    {
        kdDebug(4640) << "TDEIconEditIcon::open (malformed) " << url.prettyURL() << endl;

        TQFileInfo fi(url.url());
        if(fi.isRelative())
            url = "file:" + fi.absFilePath();

        if(!url.isValid()) // Giving up
        {
            TQString msg = i18n("The URL: %1 \nseems to be malformed.\n").arg(url.url());
            KMessageBox::sorry((TQWidget*)parent(), msg);
            return false;
        }
    }

    TQString filename;

    if(url.isLocalFile())
    {
        filename = url.path();
    }
    else
    {
        if(!TDEIO::NetAccess::download( url, filename, (TQWidget*)parent() ))
        {
            TQString msg = i18n("There was an error loading:\n%1\n").arg(url.prettyURL());
            KMessageBox::error((TQWidget*)parent(), msg);
            return false;
        }
    }

    bool loadedOk = img->load(filename);

    if(!url.isLocalFile())
    {
        TDEIO::NetAccess::removeTempFile( filename );
    }

    if(!loadedOk) 
    {
          TQString msg = i18n("There was an error loading:\n%1\n").arg(url.prettyURL());
          KMessageBox::error((TQWidget*)parent(), msg);
    }
    else
    {
        kdDebug(4640) << "TDEIconEditIcon::open - Image loaded" << endl;
        
        // _url is saved off for use in saving the image to the same 
        // file later - should include full path
        if(url.isLocalFile())
        {
            _url = url.path();
        }
        else
        {
            _url = "";
        }
        
        kdDebug(4640) << "TDEIcon: _url: " << _url << endl;
        
        // this causes updates of preview, color palettes, etc.    
        emit loaded(img);
        kdDebug(4640) << "loaded(img)" <<  endl;    
        
        // this is the name that shows up in status bar - 
        // should be filename with path
        emit newname(url.prettyURL()); 
        kdDebug(4640) << "newname(_url) : " << url.prettyURL() << endl;    
        
        emit addrecent(url.prettyURL());
        
        kdDebug(4640) << "TDEIconEditIcon::open - done" << endl;
    }

    return loadedOk;
}



bool TDEIconEditIcon::promptForFile(const TQImage *img)
{
    kdDebug(4640) << "TDEIconEditIcon::promptForFile(const TQImage *img)" << endl;
    /*
    TQString filter = i18n("*|All Files (*)\n"
                        "*.xpm|XPM (*.xpm)\n"
                        "*.png|PNG (*.png)\n"
                        "*.gif|GIF files (*.gif)\n"
                        "*.jpg|JPEG files (*.jpg)\n"
                        "*.ico|Icon files (*.ico)\n");

  
    KURL url = KFileDialog::getOpenURL( TQString(), filter );
    */
    bool loaded = false;
    KURL url = KFileDialog::getImageOpenURL( TQString(), static_cast<TQWidget*>(parent()) );

    if( !url.isEmpty() )
    {
        loaded = open( img, url );
    }

    return loaded;
}



bool TDEIconEditIcon::saveAs(const TQImage *image)
{
    kdDebug(4640) << "TDEIconEditIcon::saveAs" << endl;

    TQString file;

    //Get list of file types..
    KFileDialog *dialog=new KFileDialog(TQString(), TQString(), static_cast<TQWidget*>(parent()), "file dialog", true);
    dialog->setCaption( i18n("Save Icon As") );
    dialog->setKeepLocation( true );
    dialog->setMimeFilter( KImageIO::mimeTypes(KImageIO::Writing), "image/png" );
    dialog->setOperationMode( KFileDialog::Saving );

    if(dialog->exec()==TQDialog::Accepted)
    {
        file = dialog->selectedFile();
        if( file.isNull() )
        {
            delete dialog;
            return false;
        }
        if ( !KImageIO::canWrite(KImageIO::type(file)) )
        {
            if ( KImageIO::canWrite(KImageIO::typeForMime(dialog->currentFilter())) )
                file += "."+KImageIO::suffix(KImageIO::typeForMime(dialog->currentFilter()));
            else
                file += ".png";
        }
    }
    else
    {
        delete dialog;
        return false;
    }
        
    delete dialog;

    if(TQFile::exists(file))
    {
        int r=KMessageBox::warningContinueCancel(static_cast<TQWidget*>(parent()),
            i18n( "A file named \"%1\" already exists. "
                  "Overwrite it?" ).arg(file),
            i18n( "Overwrite File?" ),
            i18n( "&Overwrite" ) );
  
        if(r==KMessageBox::Cancel)
        {
            return false;
        }
    }

    return save( image, file );
}



bool TDEIconEditIcon::save(const TQImage *image, const TQString &_filename)
{
    kdDebug(4640) << "TDEIconEditIcon::save" << endl;
    TQString filename = _filename;

    if(filename.isEmpty())
    {
        if(_url.isEmpty())
        {
            return saveAs(image);
        }
        else
        {
            KURL turl(_url);
            filename = turl.path();
        }    
    }

    TQImage *img = (TQImage*)image;
    img->setAlphaBuffer(true);
    
    KURL turl(filename);
    TQString str = turl.path();
    bool savedOk = false;

    /* base image type on file extension - let kimageio
    take care of this determination */
    
    if(img->save(str, KImageIO::type(str).ascii()))    
    {
        kdDebug(4640) << "img->save()) successful" << endl;
        emit saved();    
        _url = filename;
        
        // emit signal to change title bar to reflect new name
        emit newname(filename); 
        kdDebug(4640) << "newname(filenamme) : " << _url << endl;    
        savedOk = true;
        emit addrecent(filename);
    }
    else 
    {
        TQString msg = i18n("There was an error saving:\n%1\n").arg(str);
        KMessageBox::error((TQWidget*)parent(), msg);
        kdDebug(4640) << "TDEIconEditIcon::save - " << msg << endl;
    }

    kdDebug(4640) << "TDEIconEditIcon::save - done" << endl;    

    return savedOk;
}


#include "tdeicon.moc"
