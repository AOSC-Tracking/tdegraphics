// -*- C++ -*-
//
// ghostscript_interface
//
// Part of KDVI - A framework for multipage text/gfx viewers
//
// (C) 2004 Stefan Kebekus
// Distributed under the GPL

#ifndef _PSGS_H_
#define _PSGS_H_

#include <tqcolor.h>
#include <tqobject.h>
#include <tqstring.h>
#include <tqintdict.h>

class PageNumber;
class TQPainter;


class pageInfo
{
public:
  pageInfo(const TQString& _PostScriptString);
  ~pageInfo();

  TQColor    background;
  TQColor    permanentBackground;
  TQString   *PostScriptString;
};


class ghostscript_interface  : public TQObject 
{
 Q_OBJECT
  TQ_OBJECT

public:
  ghostscript_interface();
  ~ghostscript_interface();

  void clear();

  // sets the PostScript which is used on a certain page
  void setPostScript(const PageNumber& page, const TQString& PostScript);

  // sets path from additional postscript files may be read
  void setIncludePath(const TQString &_includePath);

  // Sets the background color for a certain page. If permanent is false then the original 
  // background color can be restored by calling restoreBackground(page).
  // The Option permanent = false is used when we want to display a different paper 
  // color as the one specified in the dvi file.
  void setBackgroundColor(const PageNumber& page, const TQColor& background_color, bool permanent = true);

  // Restore the background to the color which was specified by the last call to setBackgroundColor()
  // With option permanent = true.
  void restoreBackgroundColor(const PageNumber& page);

  // Draws the graphics of the page into the painter, if possible. If
  // the page does not contain any graphics, nothing happens
  void     graphics(const PageNumber& page, double dpi, long magnification, TQPainter* paint);

  // Returns the background color for a certain page. If no color was
  // set, TQt::white is returned.
  TQColor   getBackgroundColor(const PageNumber& page) const;

  TQString  *PostScriptHeaderString;

  /** This method tries to find the PostScript file 'filename' in the
   DVI file's directory (if the base-URL indicates that the DVI file
   is local), and, if that fails, uses kpsewhich to find the file. If
   the file is found, the full path (including file name) is
   returned. Otherwise, the method returns the first argument. TODO:
   use the DVI file's baseURL, once this is implemented.
  */
  static  TQString locateEPSfile(const TQString &filename, const KURL &base);

private:
  void                  gs_generate_graphics_file(const PageNumber& page, const TQString& filename, long magnification);
  TQIntDict<pageInfo>    pageList;

  double                resolution;   // in dots per inch
  int                   pixel_page_w; // in pixels
  int                   pixel_page_h; // in pixels

  TQString               includePath;

  // Output device that ghostscript is supposed tp use. Default is
  // "png256". If that does not work, gs_generate_graphics_file will
  // automatically try other known device drivers. If no known output
  // device can be found, something is badly wrong. In that case,
  // "gsDevice" is set to an empty string, and
  // gs_generate_graphics_file will return immediately.
  TQValueListIterator<TQString> gsDevice;

  // A list of known devices, set by the constructor. This includes
  // "png256", "pnm". If a device is found to not work, its name is
  // removed from the list, and another device name is tried.
  TQStringList           knownDevices;

signals:
  /** Passed through to the top-level kpart. */
  void setStatusBarText( const TQString& );
};

#endif
