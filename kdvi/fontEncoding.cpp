// fontEncoding.cpp
//
// Part of KDVI - A DVI previewer for the KDE desktop environemt 
//
// (C) 2003 Stefan Kebekus
// Distributed under the GPL

#include <config.h>
#ifdef HAVE_FREETYPE

#include <kdebug.h>
#include <kprocio.h>
#include <tqfile.h>
#include <tqstringlist.h>

#include "fontEncoding.h"

//#define DEBUG_FONTENC

fontEncoding::fontEncoding(const TQString &encName)
{
#ifdef DEBUG_FONTENC
  kdDebug(4300) << "fontEncoding( " << encName << " )" << endl;
#endif

  _isValid = false;
  // Use kpsewhich to find the encoding file.
  KProcIO proc;
  TQString encFileName;
  proc << "kpsewhich" << encName;
  if (proc.start(KProcess::Block) == false) {
    kdError(4300) << "fontEncoding::fontEncoding(...): kpsewhich could not be started." << endl;
    return;
  }
  proc.readln(encFileName);
  encFileName = encFileName.stripWhiteSpace();

  if (encFileName.isEmpty()) {
    kdError(4300) << TQString("fontEncoding::fontEncoding(...): The file '%1' could not be found by kpsewhich.").arg(encName) << endl;
    return;
  }

#ifdef DEBUG_FONTENC
  kdDebug(4300) << "FileName of the encoding: " << encFileName << endl;
#endif  

  TQFile file( encFileName );
  if ( file.open( IO_ReadOnly ) ) {
    // Read the file (excluding comments) into the TQString variable
    // 'fileContent'
    TQTextStream stream( &file );
    TQString fileContent;
    while ( !stream.atEnd() ) 
      fileContent += stream.readLine().section('%', 0, 0); // line of text excluding '\n' until first '%'-sign
    file.close();
    
    fileContent = fileContent.stripWhiteSpace();
    
    // Find the name of the encoding
    encodingFullName = fileContent.section('[', 0, 0).simplifyWhiteSpace().mid(1);
#ifdef DEBUG_FONTENC
    kdDebug(4300) << "encodingFullName: " << encodingFullName << endl;
#endif
    
    fileContent = fileContent.section('[', 1, 1).section(']',0,0).simplifyWhiteSpace();
    TQStringList glyphNameList = TQStringList::split( '/', fileContent );
    
    int i = 0;
    for ( TQStringList::Iterator it = glyphNameList.begin(); (it != glyphNameList.end())&&(i<256); ++it ) {
      glyphNameVector[i] = (*it).simplifyWhiteSpace();
#ifdef DEBUG_FONTENC
      kdDebug(4300) << i << ": " << glyphNameVector[i] << endl;
#endif
      i++;
    }
    for(; i<256; i++)
      glyphNameVector[i] = ".notdef";
  } else {
    kdError(4300) << TQString("fontEncoding::fontEncoding(...): The file '%1' could not be opened.").arg(encFileName) << endl;
    return;
  }

  _isValid = true;
}


#endif // HAVE_FREETYPE
