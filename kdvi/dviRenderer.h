// -*- C++ -*-
//
// Class: dviRenderer
//
// Class for rendering TeX DVI files.
// Part of KDVI- A previewer for TeX DVI files.
//
// (C) 2001-2005 Stefan Kebekus. Distributed under the GPL.

#ifndef _dvirenderer_h_
#define _dvirenderer_h_

#include "bigEndianByteReader.h"
#include "documentRenderer.h"
#include "fontpool.h"

#include <kurl.h>
#include <tqintdict.h>
#include <tqpointarray.h>
#include <tqtimer.h>
#include <tqvaluestack.h>
#include <tqvaluevector.h>

class Anchor;
class DocumentWidget;
class dviRenderer;
class fontProgressDialog;
class ghostscript_interface;
class infoDialog;
class KAction;
class KDVIMultiPage;
class KPrinter;
class KProcess;
class KProgressDialog;
class KShellProcess;
class PreBookmark;
class TeXFontDefinition;

extern const int MFResolutions[];

class DVI_SourceFileAnchor {
 public:
  DVI_SourceFileAnchor() {}
  DVI_SourceFileAnchor(const TQString& name, TQ_UINT32 ln, TQ_UINT32 pg, const Length& _distance_from_top)
    : fileName(name), line(ln), page(pg),
      distance_from_top(_distance_from_top) {}

  TQString    fileName;
  TQ_UINT32   line;
  TQ_UINT32   page;
  Length     distance_from_top;
};

/** Compound of registers, as defined in section 2.6.2 of the DVI
    driver standard, Level 0, published by the TUG DVI driver
    standards committee. */

struct framedata {
  long dvi_h;
  long dvi_v;
  long w;
  long x;
  long y;
  long z;
  int pxl_v;
};


/* this information is saved when using virtual fonts */

typedef	void	(dviRenderer::*set_char_proc)(unsigned int, unsigned int);
typedef void    (dviRenderer::*parseSpecials)(char *, TQ_UINT8 *);

struct drawinf {
  struct framedata            data;
  TeXFontDefinition          *fontp;
  set_char_proc	              set_char_p;

  TQIntDict<TeXFontDefinition> *fonttable;
  TeXFontDefinition	      *_virtual;
};



class dviRenderer : public DocumentRenderer, bigEndianByteReader
{
  Q_OBJECT
  TQ_OBJECT

public:
  dviRenderer(TQWidget *tqparent);
  ~dviRenderer();

  virtual bool	setFile(const TQString &fname, const KURL &base);

  class dvifile *dviFile;

  void          setPrefs(bool flag_showPS, const TQString &editorCommand, bool useFontHints );

  virtual bool  supportsTextSearch() const {return true;}

  bool		showPS() { return _postscript; }
  int		curr_page() { return current_page+1; }
  virtual bool  isValidFile(const TQString& fileName) const;


  /** This method will try to parse the reference part of the DVI
      file's URL, (either a number, which is supposed to be a page
      number, or src:<line><filename>) and see if a corresponding
      section of the DVI file can be found. If so, it returns an
      anchor to that section. If not, it returns an invalid anchor. */
  virtual Anchor        parseReference(const TQString &reference);
  
  // These should not be public... only for the moment
  void          read_postamble();
  void          draw_part(double current_dimconv, bool is_vfmacro);
  void          set_vf_char(unsigned int cmd, unsigned int ch);
  void          set_char(unsigned int cmd, unsigned int ch);
  void          set_empty_char(unsigned int cmd, unsigned int ch);
  void          set_no_char(unsigned int cmd, unsigned int ch);
  void          applicationDoSpecial(char * cp);

  void          special(long nbytes);
  void          printErrorMsgForSpecials(const TQString& msg);
  void          color_special(const TQString& cp);
  void          html_href_special(const TQString& cp);
  void          html_anchor_end();
  void          draw_page();

public slots:
  void          exportPS(const TQString& fname = TQString(), const TQString& options = TQString(), KPrinter* printer = 0);
  void          exportPDF();

  void          showInfo();
  void          handleSRCLink(const TQString &linkText, TQMouseEvent *e, DocumentWidget *widget);

  void          embedPostScript();
  void          abortExternalProgramm();

  /** simply emits "setStatusBarText( TQString() )". This is used
      in dviRenderer::mouseMoveEvent(), see the explanation there. */
  void          clearStatusBar();

  virtual void  drawPage(double res, RenderedDocumentPage *page);
  virtual void  getText(RenderedDocumentPage* page);

  /** Slots used in conjunction with external programs */
  void          dvips_output_receiver(KProcess *, char *buffer, int buflen);
  void          dvips_terminated(KProcess *);
  void          editorCommand_terminated(KProcess *);

signals:
  /** Passed through to the top-level kpart. */
  //  void setStatusBarText( const TQString& );

private slots:
  /** This method shows a dialog that tells the user that source
      information is present, and gives the opportunity to open the
      manual and learn more about forward and inverse search */
  void          showThatSourceInformationIsPresent();

private:
  /** URL to the DVI file

  This field is initialized by the setFile() method. See the
  explanation there.  */
  KURL baseURL;


 /** This method parses a color specification of type "gray 0.5", "rgb
     0.5 0.7 1.0", "hsb ...", "cmyk .." or "PineGreen". See the source
     code for details. */
 TQColor parseColorSpecification(const TQString& colorSpec);
 
 /** This map contains the colors which are known by name. This field
     is initialized in the method parseColorSpecification() as soon as
     it is needed. */
 TQMap<TQString, TQColor> namedColors;

  /* This method locates special PDF characters in a string and
     replaces them by UTF8. See Section 3.2.3 of the PDF reference
     guide for information */
  TQString PDFencodingToTQString(const TQString& pdfstring);

  void  setResolution(double resolution_in_DPI);

  fontPool      font_pool;
  infoDialog    *info;

  double        resolutionInDPI;

  // @@@ explanation
  void          prescan(parseSpecials specialParser);
  void          prescan_embedPS(char *cp, TQ_UINT8 *);
  void          prescan_removePageSizeInfo(char *cp, TQ_UINT8 *);
  void          prescan_parseSpecials(char *cp, TQ_UINT8 *);
  void          prescan_ParsePapersizeSpecial(const TQString& cp);
  void          prescan_ParseBackgroundSpecial(const TQString& cp);
  void          prescan_ParseHTMLAnchorSpecial(const TQString& cp);
  void          prescan_ParsePSHeaderSpecial(const TQString& cp);
  void          prescan_ParsePSBangSpecial(const TQString& cp);
  void          prescan_ParsePSQuoteSpecial(const TQString& cp);
  void          prescan_ParsePSSpecial(const TQString& cp);
  void          prescan_ParsePSFileSpecial(const TQString& cp);
  void          prescan_ParseSourceSpecial(const TQString& cp);
  void          prescan_setChar(unsigned int ch);

  /* */
  TQValueVector<PreBookmark> prebookmarks;



  /** Utility fields used by the embedPostScript method*/
  KProgressDialog *embedPS_progress;
  TQ_UINT16         embedPS_numOfProgressedFiles;

  /** Shrink factor. Units are not quite clear */
  double	shrinkfactor;
  
  TQString       errorMsg;
  
  /** Methods which handle certain special commands. */
  void epsf_special(const TQString& cp);
  void source_special(const TQString& cp);
  
  /** TPIC specials */
  void TPIC_setPen_special(const TQString& cp);
  void TPIC_addPath_special(const TQString& cp);
  void TPIC_flushPath_special();
  
  /** This timer is used to delay clearing of the statusbar. Clearing
      the statusbar is delayed to avoid awful flickering when the
      mouse moves over a block of text that contains source
      hyperlinks. The signal timeout() is connected to the method
      clearStatusBar() of *this. */
  TQTimer        clearStatusBarTimer;
  
  // List of source-hyperlinks on all pages. This vector is generated
  // when the DVI-file is first loaded, i.e. when draw_part is called
  // with PostScriptOutPutString != NULL
  TQValueVector<DVI_SourceFileAnchor>  sourceHyperLinkAnchors;
  
  // If not NULL, the text currently drawn represents a source
  // hyperlink to the (relative) URL given in the string;
  TQString          *source_href;
  
  // If not NULL, the text currently drawn represents a hyperlink to
  // the (relative) URL given in the string;
  TQString          *HTML_href;
  
  TQString           editorCommand;
  
  /** Stack for register compounds, used for the DVI-commands PUSH/POP
      as explained in section 2.5 and 2.6.2 of the DVI driver standard,
      Level 0, published by the TUG DVI driver standards committee. */
  TQValueStack<struct framedata> stack;
  
  /** A stack where color are stored, according to the documentation of
      DVIPS */
  TQValueStack<TQColor> colorStack;
  
  /** The global color is to be used when the color stack is empty */
  TQColor              globalColor;
    
  /** If PostScriptOutPutFile is non-zero, then no rendering takes
      place. Instead, the PostScript code which is generated by the
      \special-commands is written to the PostScriptString */
  TQString          *PostScriptOutPutString;
  
  ghostscript_interface *PS_interface;
  
  /** true, if gs should be used, otherwise, only bounding boxes are
      drawn. */
  bool    	   _postscript;
  
  /** This flag is used when rendering a dvi-page. It is set to "true"
      when any dvi-command other than "set" or "put" series of commands
      is encountered. This is considered to mark the end of a word. */
  bool              line_boundary_encountered;
  bool              word_boundary_encountered;
  
  unsigned int	   current_page;
  
  /** Used to run and to show the progress of dvips and friends. */
  fontProgressDialog *progress;
  KShellProcess      *proc;
  KPrinter           *export_printer;
  TQString             export_fileName;
  TQString             export_tmpFileName;
  TQString             export_errorString;
  
  /** Data required for handling TPIC specials */ 
  float       penWidth_in_mInch;
  TQPointArray TPIC_path;
  TQ_UINT16    number_of_elements_in_path;
  
  struct drawinf	currinf;
  RenderedDocumentPage* currentlyDrawnPage;
};

#endif
