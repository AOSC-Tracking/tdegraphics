// fontMap.cpp
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

#include "fontMap.h"

//#define DEBUG_FONTMAP

fontMap::fontMap()
{
  // Read the map file of ps2pk which will provide us with a
  // dictionary "TeX Font names" <-> "Name of font files, Font Names
  // and Encodings" (example: the font "Times-Roman" is called
  // "ptmr8y" in the DVI file, but the Type1 font file name is
  // "utmr8a.pfb". We use the map file of "ps2pk" because that progam
  // has, like kdvi (and unlike dvips), no built-in fonts.
  
  // Finding ps2pk.map is not easy. In teTeX < 3.0, the kpsewhich
  // program REQUIRES the option "--format=dvips config". In teTeX =
  // 3.0, the option "--format=map" MUST be used. Since there is no
  // way to give both options at the same time, there is seemingly no
  // other way than to try both options one after another. We use the
  // teTeX 3.0 format first.
  KProcIO proc;
  proc << "kpsewhich" << "--format=map" << "ps2pk.map";
  if (proc.start(KProcess::Block) == false) {
    kdError(4700) << "fontMap::fontMap(): kpsewhich could not be started." << endl;
    return;
  }

  TQString map_fileName;
  proc.readln(map_fileName);
  map_fileName = map_fileName.stripWhiteSpace();
  if (map_fileName.isEmpty()) {
    // Map file not found? Then we try the teTeX < 3.0 way of finding
    // the file.
    proc << "kpsewhich" << "--format=dvips config" << "ps2pk.map";
    if (proc.start(KProcess::Block) == false) {
      kdError(4700) << "fontMap::fontMap(): kpsewhich could not be started." << endl;
      return;
    }
    proc.readln(map_fileName);
    map_fileName = map_fileName.stripWhiteSpace();
    
    // If both versions fail, then there is nothing left to do.
    if (map_fileName.isEmpty()) {
      kdError(4700) << "fontMap::fontMap(): The file 'ps2pk.map' could not be found by kpsewhich." << endl;
      return;
    }
  }
  
  TQFile file( map_fileName );
  if ( file.open( IO_ReadOnly ) ) {
    TQTextStream stream( &file );
    TQString line;
    while ( !stream.atEnd() ) {
      line = stream.readLine().simplifyWhiteSpace();
      if (line.at(0) == '%')
	continue;
      
      TQString TeXName  = line.section(' ', 0, 0);
      TQString FullName = line.section(' ', 1, 1);
      TQString fontFileName = line.section('<', -1).stripWhiteSpace().section(' ', 0, 0);
      TQString encodingName = line.section('<', -2, -2).stripWhiteSpace().section(' ', 0, 0);
      // It seems that sometimes the encoding is prepended by the
      // letter '[', which we ignore
      if ((!encodingName.isEmpty()) && (encodingName[0] == '['))
        encodingName = encodingName.mid(1);

      double slant = 0.0;
      int i = line.find("SlantFont");
      if (i >= 0) {
	bool ok;
	slant = line.left(i).section(' ', -1, -1 ,TQString::SectionSkipEmpty).toDouble(&ok);
	if (ok == false)
	  slant = 0.0;
      }
      
      fontMapEntry &entry = fontMapEntries[TeXName];
      
      entry.slant        = slant;
      entry.fontFileName = fontFileName;
      entry.fullFontName = FullName;
      if (encodingName.endsWith(".enc"))
	entry.fontEncoding = encodingName;
      else
	entry.fontEncoding = TQString();
    }
    file.close();
  } else
    kdError(4300) << TQString("fontMap::fontMap(): The file '%1' could not be opened.").arg(map_fileName) << endl;
  
#ifdef DEBUG_FONTMAP
  kdDebug(4300) << "FontMap file parsed. Results:" << endl;
  TQMap<TQString, fontMapEntry>::Iterator it;
  for ( it = fontMapEntries.begin(); it != fontMapEntries.end(); ++it ) 
    kdDebug(4300) << "TeXName: " << it.key() 
		  << ", FontFileName=" << it.data().fontFileName 
		  << ", FullName=" << it.data().fullFontName 
		  << ", Encoding=" << it.data().fontEncoding
		  << "." << endl;;
#endif
}


const TQString &fontMap::findFileName(const TQString &TeXName)
{
  TQMap<TQString, fontMapEntry>::Iterator it = fontMapEntries.find(TeXName);
  
  if (it != fontMapEntries.end())
    return it.data().fontFileName;
  else
    return TQString();
}


const TQString &fontMap::findFontName(const TQString &TeXName)
{
  TQMap<TQString, fontMapEntry>::Iterator it = fontMapEntries.find(TeXName);
  
  if (it != fontMapEntries.end())
    return it.data().fullFontName;
  else
    return TQString();
}


const TQString &fontMap::findEncoding(const TQString &TeXName)
{
  TQMap<TQString, fontMapEntry>::Iterator it = fontMapEntries.find(TeXName);
  
  if (it != fontMapEntries.end())
    return it.data().fontEncoding;
  else
    return TQString();
}


double fontMap::findSlant(const TQString &TeXName)
{
  TQMap<TQString, fontMapEntry>::Iterator it = fontMapEntries.find(TeXName);
  
  if (it != fontMapEntries.end())
    return it.data().slant;
  else
    return 0.0;
}

#endif // HAVE_FREETYPE
