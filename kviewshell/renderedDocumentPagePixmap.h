//
// Class: RenderedDocumentPagePixmap
//
// Widget for displaying TeX DVI files.
// Part of KDVI- A previewer for TeX DVI files.
//
// (C) 2004-2005 Stefan Kebekus. Distributed under the GPL.


#ifndef _rendereddocumentpagepixmap_h_
#define _rendereddocumentpagepixmap_h_

#include "renderedDocumentPage.h"

#include <tqpainter.h>
#include <tqpixmap.h>


// This class contains everything documentRenderer needs to know 
// about a certain page.
class RenderedDocumentPagePixmap: public RenderedDocumentPage, public TQPixmap
{
  TQ_OBJECT
  

public:
  RenderedDocumentPagePixmap();

  virtual ~RenderedDocumentPagePixmap();

  /** Returns a pointer to the paintDevice (in most implementation
      either a TQPixmap, or a TQPrinter). The pointer returned is valid
      as long as *this RenderedDocumentPage exists. This method is
      used by the renderer to draw on the page. */
  virtual TQPainter *getPainter();

  /** This implementation delete the TQPainter *pt (and thereby initiates the drawing) */
  virtual void returnPainter(TQPainter *pt) {delete pt;}

  TQPixmap accessiblePixmap();

  void resize(const TQSize& size);
  void resize(int width, int height);

  virtual TQSize size() { return TQPixmap::size(); }
  virtual int width() { return TQPixmap::width(); }
  virtual int height() { return TQPixmap::height(); }

  /** Returns the amount of memory used by this pixmap */
  unsigned int memory();

private:
  TQPixmap* _accessiblePixmap;
  /** true if _accessiblePixmap needs updateing */
  bool dirty;
};


#endif
