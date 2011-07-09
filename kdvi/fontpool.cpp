//
// fontpool.cpp
//
// (C) 2001-2004 Stefan Kebekus
// Distributed under the GPL

#include <config.h>

#include <kdebug.h>
#include <kinstance.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kprocess.h>
#include <kprocio.h>
#include <math.h>
#include <tqapplication.h>
#include <tqfile.h>
#include <tqimage.h>
#include <tqpainter.h>
#include <stdlib.h>

#include "fontpool.h"
#include "performanceMeasurement.h"
#include "prefs.h"
#include "TeXFont.h"

//#define DEBUG_FONTPOOL



// List of permissible MetaFontModes which are supported by kdvi.

//const char *MFModes[]       = { "cx", "ljfour", "lexmarks" };
//const char *MFModenames[]   = { "Canon CX", "LaserJet 4", "Lexmark S" };
//const int   MFResolutions[] = { 300, 600, 1200 };

#ifdef PERFORMANCE_MEASUREMENT
TQTime fontPoolTimer;
bool fontPoolTimerFlag;
#endif

//#define DEBUG_FONTPOOL

fontPool::fontPool()
  :  progress( "fontgen",  // Chapter in the documentation for help.
	       i18n( "KDVI is currently generating bitmap fonts..." ),
	       i18n( "Aborts the font generation. Don't do this." ),
	       i18n( "KDVI is currently generating bitmap fonts which are needed to display your document. "
		     "For this, KDVI uses a number of external programs, such as MetaFont. You can tqfind "
		     "the output of these programs later in the document info dialog." ),
	       i18n( "KDVI is generating fonts. Please wait." ),
	       0 )
{
#ifdef DEBUG_FONTPOOL
  kdDebug(4300) << "fontPool::fontPool() called" << endl;
#endif

  setName("Font Pool");

  displayResolution_in_dpi = 100.0; // A not-too-bad-default
  useFontHints             = true;
  CMperDVIunit             = 0;
  extraSearchPath          = TQString();
  fontList.setAutoDelete(true);


#ifdef HAVE_FREETYPE
  // Initialize the Freetype Library
  if ( FT_Init_FreeType( &FreeType_library ) != 0 ) {
    kdError(4300) << "Cannot load the FreeType library. KDVI proceeds without FreeType support." << endl;
    FreeType_could_be_loaded = false;
  } else
    FreeType_could_be_loaded = true;
#endif

  // Check if the QT library supports the alpha channel of
  // pixmaps. Experiments show that --depending of the configuration
  // of QT at compile and runtime or the availability of the XFt
  // extension, alpha channels are either supported, or silently
  // converted to 1-bit masks.
  TQImage start(1, 1, 32); // Generate a 1x1 image, black with alpha=0x10
  start.setAlphaBuffer(true);
  TQ_UINT32 *destScanLine = (TQ_UINT32 *)start.scanLine(0);
  *destScanLine = 0x80000000;
  TQPixmap intermediate(start);
  TQPixmap dest(1,1);
  dest.fill(TQt::white);
  TQPainter paint( &dest );
  paint.drawPixmap(0, 0, intermediate);
  paint.end();
  start = dest.convertToImage().convertDepth(32);
  TQ_UINT8 result = *(start.scanLine(0)) & 0xff;

  if ((result == 0xff) || (result == 0x00)) {
#ifdef DEBUG_FONTPOOL
    kdDebug(4300) << "fontPool::fontPool(): TQPixmap does not support the alpha channel" << endl;
#endif
    TQPixmapSupportsAlpha = false;
  } else {
#ifdef DEBUG_FONTPOOL
    kdDebug(4300) << "fontPool::fontPool(): TQPixmap supports the alpha channel" << endl;
#endif
    TQPixmapSupportsAlpha = true;
  }
}


fontPool::~fontPool()
{
#ifdef DEBUG_FONTPOOL
  kdDebug(4300) << "fontPool::~fontPool() called" << endl;
#endif

  // need to manually clear the fonts _before_ freetype gets unloaded
  fontList.clear();

#ifdef HAVE_FREETYPE
  if (FreeType_could_be_loaded == true)
    FT_Done_FreeType( FreeType_library );
#endif
}


void fontPool::setParameters( bool _useFontHints )
{
  // Check if glyphs need to be cleared
  if (_useFontHints != useFontHints) {
    double displayResolution = displayResolution_in_dpi;
    TeXFontDefinition *fontp = fontList.first();
    while(fontp != 0 ) {
      fontp->setDisplayResolution(displayResolution * fontp->enlargement);
      fontp=fontList.next();
    }
  }

  useFontHints = _useFontHints;
}


TeXFontDefinition* fontPool::appendx(const TQString& fontname, TQ_UINT32 checksum, TQ_UINT32 scale, double enlargement)
{
  // Reuse font if possible: check if a font with that name and
  // natural resolution is already in the fontpool, and use that, if
  // possible.
  TeXFontDefinition *fontp = fontList.first();
  while( fontp != 0 ) {
    if ((fontname == fontp->fontname) && ( (int)(enlargement*1000.0+0.5)) == (int)(fontp->enlargement*1000.0+0.5)) {
      // if font is already in the list
      fontp->mark_as_used();
      return fontp;
    }
    fontp=fontList.next();
  }

  // If font doesn't exist yet, we have to generate a new font.

  double displayResolution = displayResolution_in_dpi;

  fontp = new TeXFontDefinition(fontname, displayResolution*enlargement, checksum, scale, this, enlargement);
  if (fontp == 0) {
    kdError(4300) << i18n("Could not allocate memory for a font structure!") << endl;
    exit(0);
  }
  fontList.append(fontp);

#ifdef PERFORMANCE_MEASUREMENT
  fontPoolTimer.start();
  fontPoolTimerFlag = false;
#endif

  // Now start kpsewhich/MetaFont, etc. if necessary
  return fontp;
}


TQString fontPool::status()
{
#ifdef DEBUG_FONTPOOL
  kdDebug(4300) << "fontPool::status() called" << endl;
#endif

  TQString       text;
  TQStringList   tmp;

  if (fontList.isEmpty())
    return i18n("The fontlist is currently empty.");

  text.append("<table WIDTH=\"100%\" NOSAVE >");
  text.append( TQString("<tr><td><b>%1</b></td> <td><b>%2</b></td> <td><b>%3</b></td> <td><b>%4</b> <td><b>%5</b></td> <td><b>%6</b></td></tr>")
	       .tqarg(i18n("TeX Name"))
	       .tqarg(i18n("Family"))
	       .tqarg(i18n("Zoom"))
	       .tqarg(i18n("Type"))
	       .tqarg(i18n("Encoding"))
	       .tqarg(i18n("Comment")) );

 TeXFontDefinition *fontp = fontList.first();
  while ( fontp != 0 ) {
    TQString errMsg, encoding;

    if (!(fontp->flags & TeXFontDefinition::FONT_VIRTUAL)) {
#ifdef HAVE_FREETYPE
      encoding = fontp->getFullEncodingName();
#endif
      if (fontp->font != 0)
	errMsg = fontp->font->errorMessage;
      else
	errMsg = i18n("Font file not found");
    }

#ifdef HAVE_FREETYPE
    tmp << TQString ("<tr><td>%1</td> <td>%2</td> <td>%3%</td> <td>%4</td> <td>%5</td> <td>%6</td></tr>")
      .tqarg(fontp->fontname)
      .tqarg(fontp->getFullFontName())
      .tqarg((int)(fontp->enlargement*100 + 0.5))
      .tqarg(fontp->getFontTypeName())
      .tqarg(encoding)
      .tqarg(errMsg);
#endif

    fontp=fontList.next();
  }

  tmp.sort();
  text.append(tmp.join("\n"));
  text.append("</table>");

  return text;
}


bool fontPool::areFontsLocated()
{
#ifdef DEBUG_FONTPOOL
  kdDebug(4300) << "fontPool::areFontsLocated() called" << endl;
#endif

  // Is there a font whose name we did not try to find out yet?
  TeXFontDefinition *fontp = fontList.first();
  while( fontp != 0 ) {
    if ( !fontp->isLocated() )
      return false;
    fontp=fontList.next();
  }

#ifdef DEBUG_FONTPOOL
  kdDebug(4300) << "... yes, all fonts are located (but not necessarily loaded)." << endl;
#endif
  return true; // That says that all fonts are located.
}


void fontPool::locateFonts()
{
  kpsewhichOutput = TQString();

  // First, we try and find those fonts which exist on disk
  // already. If virtual fonts are found, they will add new fonts to
  // the list of fonts whose font files need to be located, so that we
  // repeat the lookup.
  bool vffound;
  do {
    vffound = false;
    locateFonts(false, false, &vffound);
  } while(vffound);
  
  // If still not all fonts are found, look again, this time with
  // on-demand generation of PK fonts enabled.
  if (!areFontsLocated())
    locateFonts(true, false);
  
  // If still not all fonts are found, we look for TFM files as a last
  // resort, so that we can at least draw filled rectangles for
  // characters.
  if (!areFontsLocated())
    locateFonts(false, true);
  
  // If still not all fonts are found, we give up. We mark all fonts
  // as 'located', so that wee won't look for them any more, and
  // present an error message to the user.
  if (!areFontsLocated()) {
    markFontsAsLocated();
    TQString details = TQString("<qt><p><b>PATH:</b> %1</p>%2</qt>").tqarg(getenv("PATH")).tqarg(kpsewhichOutput);
    KMessageBox::detailedError( 0, i18n("<qt><p>KDVI was not able to locate all the font files "
					"which are necessary to display the current DVI file. "
					"Your document might be unreadable.</p></qt>"),
				details,
				i18n("Not All Font Files Found") );
  }
}


void fontPool::locateFonts(bool makePK, bool locateTFMonly, bool *virtualFontsFound)
{
  // Set up the kpsewhich process. If pass == 0, look for vf-fonts and
  // disable automatic font generation as vf-fonts can't be
  // generated. If pass == 0, ennable font generation, if it was
  // enabled globally.
  emit setStatusBarText(i18n("Locating fonts..."));

  TQStringList shellProcessCmdLine;

  KProcIO kpsewhichIO;
  // If PK fonts are generated, the kpsewhich command will re-route
  // the output of MetaFont into its stderr. Here we make sure this
  // output is intercepted and parsed.
  tqApp->connect(&kpsewhichIO, TQT_SIGNAL(receivedStderr(KProcess *, char *, int)),
		this, TQT_SLOT(mf_output_receiver(KProcess *, char *, int)));
  
  
  kpsewhichIO.setUseShell(true);
  
  // Now generate the command line for the kpsewhich
  // program. Unfortunately, this can be rather long and involved...
  shellProcessCmdLine += "kpsewhich";
  shellProcessCmdLine += TQString("--dpi 1200");
  shellProcessCmdLine += TQString("--mode lexmarks");
  
  // Disable automatic pk-font generation.
  if (makePK == true)
    shellProcessCmdLine += "--mktex pk";
  else
    shellProcessCmdLine += "--no-mktex pk";
  
  // Names of fonts that shall be located
  TQ_UINT16 numFontsInJob = 0;
  TeXFontDefinition *fontp = fontList.first();
  while ( fontp != 0 ) {
    if (!fontp->isLocated()) {
      numFontsInJob++;
      
      if (locateTFMonly == true)
	shellProcessCmdLine += KShellProcess::quote(TQString("%1.tfm").tqarg(fontp->fontname));
      else {
#ifdef HAVE_FREETYPE
	if (FreeType_could_be_loaded == true) {
	  const TQString &filename = fontsByTeXName.findFileName(fontp->fontname);
	  if (!filename.isEmpty())
	    shellProcessCmdLine += KShellProcess::quote(TQString("%1").tqarg(filename));
	}
#endif
	shellProcessCmdLine += KShellProcess::quote(TQString("%1.vf").tqarg(fontp->fontname));
	shellProcessCmdLine += KShellProcess::quote(TQString("%1.1200pk").tqarg(fontp->fontname));
      }
    }
    fontp=fontList.next();
  }

  if (numFontsInJob == 0)
    return;

  progress.setTotalSteps(numFontsInJob, &kpsewhichIO);

  // Now run... kpsewhich. In case of error, kick up a fuss.
  MetafontOutput = TQString();
  kpsewhichOutput += "<p><b>"+shellProcessCmdLine.join(" ")+"</b></p>";
  kpsewhichIO << shellProcessCmdLine;
  TQString importanceOfKPSEWHICH = i18n("<p>KDVI relies on the <b>kpsewhich</b> program to locate font files "
				       "on your hard disc and to generate PK fonts, if necessary.</p>");
  if (kpsewhichIO.start(KProcess::NotifyOnExit, false) == false) {
    TQString msg = i18n(	"<p>The shell process for the kpsewhich program could not "
			"be started. Consequently, some font files could not be found, "
			"and your document might by unreadable. If this error is reproducable "
			"please report the issue to the KDVI developers using the 'Help' menu.<p>" );
    TQApplication::restoreOverrideCursor();
    KMessageBox::error( 0, TQString("<qt>%1%2</qt>").tqarg(importanceOfKPSEWHICH).tqarg(msg),
			i18n("Problem locating fonts - KDVI") );
    markFontsAsLocated();
    return;
  }
  
  // We wait here while the kpsewhich program is concurrently
  // running. Every second we call processEvents() to keep the GUI
  // updated. This is important, e.g. for the progress dialog that is
  // shown when PK fonts are generated by MetaFont.
  while(kpsewhichIO.wait(1) == false)
    tqApp->processEvents();
  progress.hide();
  
  // Handle fatal errors.
  if (!kpsewhichIO.normalExit()) {
    KMessageBox::sorry( 0, "<qt><p>The font generation was aborted. As a result, "
			"some font files could not be located, and your document might be unreadable.</p></qt>",
			i18n("Font generation aborted - KDVI") );
    
    // This makes sure the we don't try to run kpsewhich again
    if (makePK == false)
      markFontsAsLocated();
  } else
    if (kpsewhichIO.exitStatus() == 127) {
      // An exit status of 127 means that the kpsewhich executable
      // could not be found. We give extra explanation then.
      TQApplication::restoreOverrideCursor();
      TQString msg = i18n( "<p>There were problems running kpsewhich. As a result, "
			  "some font files could not be located, and your document might be unreadable.</p>"
			  "<p><b>Possible reason:</b> The kpsewhich program is perhaps not installed on your system, or it "
			  "cannot be found in the current search path.</p>"
			  "<p><b>What you can do:</b> The kpsewhich program is normally contained in distributions of the TeX "
			  "typesetting system. If TeX is not installed on your system, you could install the TeTeX distribution (www.tetex.org). "
			  "If you are sure that TeX is installed, please try to use the kpsewhich program from the command line to check if it "
			  "really works.</p>");
      TQString details = TQString("<qt><p><b>PATH:</b> %1</p>%2</qt>").tqarg(getenv("PATH")).tqarg(kpsewhichOutput);
      
      KMessageBox::detailedError( 0, TQString("<qt>%1%2</qt>").tqarg(importanceOfKPSEWHICH).tqarg(msg), details,
				  i18n("Problem locating fonts - KDVI") );
      // This makes sure the we don't try to run kpsewhich again
      markFontsAsLocated();
      return;
    }
  
  // Create a list with all filenames found by the kpsewhich program.
  TQStringList fileNameList;
  TQString line;
  while(kpsewhichIO.readln(line) >= 0)
    fileNameList += line;
  
  // Now associate the file names found with the fonts
  fontp=fontList.first();
  while ( fontp != 0 ) {
    if (fontp->filename.isEmpty() == true) {
      TQStringList matchingFiles;
#ifdef HAVE_FREETYPE
      const TQString &fn = fontsByTeXName.findFileName(fontp->fontname);
      if (!fn.isEmpty())
	matchingFiles = fileNameList.grep(fn);
#endif
      if (matchingFiles.isEmpty() == true)
	matchingFiles += fileNameList.grep(fontp->fontname+".");
      
      if (matchingFiles.isEmpty() != true) {
#ifdef DEBUG_FONTPOOL
	kdDebug(4300) << "Associated " << fontp->fontname << " to " << matchingFiles.first() << endl;
#endif
	TQString fname = matchingFiles.first();
	fontp->fontNameReceiver(fname);
	fontp->flags |= TeXFontDefinition::FONT_KPSE_NAME;
	if (fname.endsWith(".vf")) {
	  if (virtualFontsFound != 0)
	    *virtualFontsFound = true;
	  // Constructing a virtual font will most likely insert other
	  // fonts into the fontList. After that, fontList.next() will
	  // no longer work. It is therefore safer to start over.
	  fontp=fontList.first();
	  continue;
	}
      }
    } // of if (fontp->filename.isEmpty() == true)
    fontp = fontList.next();
  }
}


void fontPool::setCMperDVIunit( double _CMperDVI )
{
#ifdef DEBUG_FONTPOOL
  kdDebug(4300) << "fontPool::setCMperDVIunit( " << _CMperDVI << " )" << endl;
#endif

  if (CMperDVIunit == _CMperDVI)
    return;

  CMperDVIunit = _CMperDVI;

  TeXFontDefinition *fontp = fontList.first();
  while(fontp != 0 ) {
    fontp->setDisplayResolution(displayResolution_in_dpi * fontp->enlargement);
    fontp=fontList.next();
  }
}


void fontPool::setDisplayResolution( double _displayResolution_in_dpi )
{
#ifdef DEBUG_FONTPOOL
  kdDebug(4300) << "fontPool::setDisplayResolution( displayResolution_in_dpi=" << _displayResolution_in_dpi << " ) called" << endl;
#endif
  
  // Ignore minute changes by less than 2 DPI. The difference would
  // hardly be visible anyway. That saves a lot of re-painting,
  // e.g. when the user resizes the window, and a flickery mouse
  // changes the window size by 1 pixel all the time.
  if ( fabs(displayResolution_in_dpi - _displayResolution_in_dpi) <= 2.0 ) {
#ifdef DEBUG_FONTPOOL
    kdDebug(4300) << "fontPool::setDisplayResolution(...): resolution wasn't changed. Aborting." << endl;
#endif
    return;
  }
  
  displayResolution_in_dpi = _displayResolution_in_dpi;
  double displayResolution = displayResolution_in_dpi;
  
  TeXFontDefinition *fontp = fontList.first();
  while(fontp != 0 ) {
    fontp->setDisplayResolution(displayResolution * fontp->enlargement);
    fontp=fontList.next();
  }
  
  // Do something that causes re-rendering of the dvi-window
  /*@@@@
  emit fonts_have_been_loaded(this);
  */
}


void fontPool::markFontsAsLocated()
{
  TeXFontDefinition *fontp=fontList.first();
  while ( fontp != 0 ) {
    fontp->markAsLocated();
    fontp = fontList.next();
  }
}



void fontPool::mark_fonts_as_unused()
{
#ifdef DEBUG_FONTPOOL
  kdDebug(4300) << "fontPool::mark_fonts_as_unused() called" << endl;
#endif
  
  TeXFontDefinition  *fontp = fontList.first();
  while ( fontp != 0 ) {
    fontp->flags &= ~TeXFontDefinition::FONT_IN_USE;
    fontp=fontList.next();
  }
}


void fontPool::release_fonts()
{
#ifdef DEBUG_FONTPOOL
  kdDebug(4300) << "Release_fonts" << endl;
#endif

  TeXFontDefinition  *fontp = fontList.first();
  while(fontp != 0) {
    if ((fontp->flags & TeXFontDefinition::FONT_IN_USE) != TeXFontDefinition::FONT_IN_USE) {
      fontList.removeRef(fontp);
      fontp = fontList.first();
    } else
      fontp = fontList.next();
  }
}


void fontPool::mf_output_receiver(KProcess *, char *buffer, int buflen)
{
  // Paranoia.
  if (buflen < 0)
    return;
  
  TQString op = TQString::fromLocal8Bit(buffer, buflen);

  kpsewhichOutput.append(op);
  MetafontOutput.append(op);
  
  // We'd like to print only full lines of text.
  int numleft;
  bool show_prog = false;
  while( (numleft = MetafontOutput.tqfind('\n')) != -1) {
    TQString line = MetafontOutput.left(numleft+1);
#ifdef DEBUG_FONTPOOL
    kdDebug(4300) << "MF OUTPUT RECEIVED: " << line;
#endif
    // Search for a line which marks the beginning of a MetaFont run
    // and show the progress dialog at the end of this method.
    if (line.tqfind("kpathsea:") == 0)
      show_prog = true;

    // If the Output of the kpsewhich program contains a line starting
    // with "kpathsea:", this means that a new MetaFont-run has been
    // started. We filter these lines out and update the display
    // accordingly.
    int startlineindex = line.tqfind("kpathsea:");
    if (startlineindex != -1) {
      int endstartline  = line.tqfind("\n",startlineindex);
      TQString startLine = line.mid(startlineindex,endstartline-startlineindex);

      // The last word in the startline is the name of the font which we
      // are generating. The second-to-last word is the resolution in
      // dots per inch. Display this info in the text label below the
      // progress bar.
      int lastblank     = startLine.tqfindRev(' ');
      TQString fontName  = startLine.mid(lastblank+1);
      int secondblank   = startLine.tqfindRev(' ',lastblank-1);
      TQString dpi       = startLine.mid(secondblank+1,lastblank-secondblank-1);

      progress.show();
      progress.increaseNumSteps( i18n("Currently generating %1 at %2 dpi").tqarg(fontName).tqarg(dpi) );
    }
    MetafontOutput = MetafontOutput.remove(0,numleft+1);
  }
}


#include "fontpool.moc"
