/* poppler-document.cpp: qt interface to poppler
 * Copyright (C) 2005, Net Integration Technologies, Inc.
 * Copyright (C) 2005-2009, Albert Astals Cid <aacid@kde.org>
 * Copyright (C) 2006, Stefan Kebekus <stefan.kebekus@math.uni-koeln.de>
 * Copyright (C) 2006, Wilfried Huss <Wilfried.Huss@gmx.at>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <config.h>
#include <poppler-qt.h>
#include <tqfile.h>
#include <GlobalParams.h>
#include <Outline.h>
#include <PDFDoc.h>
#include <PSOutputDev.h>
#include <Catalog.h>
#include <ErrorCodes.h>
#include <SplashOutputDev.h>
#include <splash/SplashBitmap.h>
#include <DateInfo.h>
#include "poppler-private.h"

#if (POPPLER_VERSION_C < 71000)
#undef bool
#endif

namespace Poppler {

Document *Document::load(const TQString &filePath)
{
  if (!globalParams) {
    globalParams =
#if (POPPLER_VERSION_C >= 83000)
      std::make_unique<GlobalParams>();
#else
      new GlobalParams();
#endif
  }

# if (POPPLER_VERSION_C >= 22003000)
  DocumentData *doc = new DocumentData(std::make_unique<GooString>(TQFile::encodeName(filePath)), {});
# else
  DocumentData *doc = new DocumentData(new GooString(TQFile::encodeName(filePath)), NULL);
# endif
  Document *pdoc;
  if (doc->doc.isOk() || doc->doc.getErrorCode() == errEncrypted) {
    pdoc = new Document(doc);
    if (doc->doc.getErrorCode() == errEncrypted)
      pdoc->data->locked = true;
    else
      pdoc->data->locked = false;
    pdoc->data->m_fontInfoScanner = new FontInfoScanner(&(doc->doc));
    return pdoc;
  }
  else
    return NULL;
}

Document::Document(DocumentData *dataA)
{
  data = dataA;
}

Document::~Document()
{
  delete data;
}

bool Document::isLocked() const
{
  return data->locked;
}

bool Document::unlock(const TQCString &password)
{
  if (data->locked) {
    /* racier then it needs to be */
#   if (POPPLER_VERSION_C >= 22003000)
    DocumentData *doc2 = new DocumentData(std::make_unique<GooString>(data->doc.getFileName()),
                                          GooString(password.data()));
#   else
    GooString *filename = new GooString(data->doc.getFileName());
    GooString *pwd = new GooString(password.data());
    DocumentData *doc2 = new DocumentData(filename, pwd);
    delete pwd;
#   endif
    if (!doc2->doc.isOk()) {
      delete doc2;
    } else {
      delete data;
      data = doc2;
      data->locked = false;
      data->m_fontInfoScanner = new FontInfoScanner(&(data->doc));
    }
  }
  return data->locked;
}

Document::PageMode Document::getPageMode(void) const
{
  switch (data->doc.getCatalog()->getPageMode()) {
    case Catalog::pageModeNone:
      return UseNone;
    case Catalog::pageModeOutlines:
      return UseOutlines;
    case Catalog::pageModeThumbs:
      return UseThumbs;
    case Catalog::pageModeFullScreen:
      return FullScreen;
    case Catalog::pageModeOC:
      return UseOC;
    default:
      return UseNone;
  }
}

int Document::getNumPages() const
{
  return data->doc.getNumPages();
}

TQValueList<FontInfo> Document::fonts() const
{
  TQValueList<FontInfo> ourList;
  scanForFonts(getNumPages(), &ourList);
  return ourList;
}

bool Document::scanForFonts( int numPages, TQValueList<FontInfo> *fontList ) const
{
  FONTS_LIST_TYPE items = data->m_fontInfoScanner->scan( numPages );

#if (POPPLER_VERSION_C < 82000)
  if ( NULL == items )
    return false;
#endif
#if (POPPLER_VERSION_C < 76000)
  if ( FONTS_LIST_IS_EMPTY(items) ) {
    delete items;
    return false;
  }
#endif

  for ( int i = 0; i < FONTS_LIST_LENGTH(items); ++i ) {
    TQString fontName;
    ::FontInfo *fontInfo = FONTS_LIST_GET(items, i);
    if (fontInfo->getName())
      fontName = fontInfo->getName()->GOO_GET_CSTR();

    FontInfo font(fontName,
                  fontInfo->getEmbedded(),
                  fontInfo->getSubset(),
                  (Poppler::FontInfo::Type)(fontInfo->getType()));
    fontList->append(font);
  }
# if (POPPLER_VERSION_C >= 82000)
  for (auto entry : items) {
    delete entry;
  }
# elif (POPPLER_VERSION_C >= 76000)
  for (auto entry : *items) {
    delete entry;
  }
  delete items;
# elif (POPPLER_VERSION_C >= 70000)
  deleteGooList<::FontInfo>(items);
# else
  deleteGooList(items, ::FontInfo);
# endif
  return true;
}

/* borrowed from kpdf */
TQString Document::getInfo( const TQString & type ) const
{
  // [Albert] Code adapted from pdfinfo.cpp on xpdf
  Object info;
  if ( data->locked )
    return NULL;

# if (POPPLER_VERSION_C >= 58000)
  info = data->doc.getDocInfo();
# else
  data->doc.getDocInfo( &info );
# endif
  if ( !info.isDict() )
    return NULL;

  TQString result;
  Object obj;
  CONST_064 GooString *s1;
  GBool isUnicode;
  Unicode u;
  int i;
  Dict *infoDict = info.getDict();

#if (POPPLER_VERSION_C >= 58000)
  obj = infoDict->lookup( (char*)type.latin1() );
#else
  infoDict->lookup( (char*)type.latin1(), &obj );
#endif
  if (!obj.isNull() && obj.isString())
  {
    s1 = obj.getString();
    if ( ( s1->getChar(0) & 0xff ) == 0xfe && ( s1->getChar(1) & 0xff ) == 0xff )
    {
      isUnicode = gTrue;
      i = 2;
    }
    else
    {
      isUnicode = gFalse;
      i = 0;
    }
    while ( i < obj.getString()->getLength() )
    {
      if ( isUnicode )
      {
	u = ( ( s1->getChar(i) & 0xff ) << 8 ) | ( s1->getChar(i+1) & 0xff );
	i += 2;
      }
      else
      {
	u = s1->getChar(i) & 0xff;
	++i;
      }
      result += unicodeToTQString( &u, 1 );
    }
#   if (POPPLER_VERSION_C < 58000)
    obj.free();
    info.free();
#   endif
    return result;
  }
# if (POPPLER_VERSION_C < 58000)
  obj.free();
  info.free();
# endif
  return NULL;
}

/* borrowed from kpdf */
TQDateTime Document::getDate( const TQString & type ) const
{
  // [Albert] Code adapted from pdfinfo.cpp on xpdf
  if ( data->locked )
    return TQDateTime();

  Object info;
# if (POPPLER_VERSION_C >= 58000)
  info = data->doc.getDocInfo();
# else
  data->doc.getDocInfo( &info );
# endif
  if ( !info.isDict() ) {
#   if (POPPLER_VERSION_C < 58000)
    info.free();
#   endif
    return TQDateTime();
  }

  Object obj;
  int year, mon, day, hour, min, sec, tz_hour, tz_minute;
  char tz;
  Dict *infoDict = info.getDict();
  TQString result;

#if (POPPLER_VERSION_C >= 58000)
  obj = infoDict->lookup( (char*)type.latin1() );
#else
  infoDict->lookup( (char*)type.latin1(), &obj );
#endif
  if (!obj.isNull() && obj.isString())
  {
#   if (POPPLER_VERSION_C >= 21008000)
    const GooString *s = obj.getString();
#   else
    TQString tqs = UnicodeParsedString(obj.getString());
    const char *s = tqs.latin1();
#   endif
    // TODO do something with the timezone information
    if (parseDateString(s, &year, &mon, &day, &hour, &min, &sec, &tz, &tz_hour, &tz_minute))
    {
      TQDate d( year, mon, day );  //CHECK: it was mon-1, Jan->0 (??)
      TQTime t( hour, min, sec );
      if ( d.isValid() && t.isValid() ) {
#	if (POPPLER_VERSION_C < 58000)
	obj.free();
	info.free();
#	endif
	return TQDateTime( d, t );
      }
    }
  }
# if (POPPLER_VERSION_C < 58000)
  obj.free();
  info.free();
# endif
  return TQDateTime();
}

bool Document::isEncrypted() const
{
  return data->doc.isEncrypted();
}

bool Document::isLinearized() const
{
  return data->doc.isLinearized();
}

bool Document::okToPrint() const
{
  return data->doc.okToPrint();
}

bool Document::okToChange() const
{
  return data->doc.okToChange();
}

bool Document::okToCopy() const
{
  return data->doc.okToCopy();
}

bool Document::okToAddNotes() const
{
  return data->doc.okToAddNotes();
}

double Document::getPDFVersion() const
{
  return data->doc.getPDFMajorVersion () + data->doc.getPDFMinorVersion() / 10.0;
}

void Document::getPdfVersion(int *major, int *minor) const
{
  if (major)
    *major = data->doc.getPDFMajorVersion();
  if (minor)
    *minor = data->doc.getPDFMinorVersion();
}

TQDomDocument *Document::toc() const
{
  Outline * outline = data->doc.getOutline();
  if ( !outline )
    return NULL;

  OUTLINE_ITEMS_TYPE * items = outline->getItems();
  if ( !items || OUTLINE_ITEMS_LENGTH(items) < 1 )
    return NULL;

  TQDomDocument *toc = new TQDomDocument();
  if ( OUTLINE_ITEMS_LENGTH(items) > 0 )
    data->addTocChildren( toc, toc, items );

  return toc;
}

LinkDestination *Document::linkDestination( const TQString &name )
{
  GooString * namedDest = TQStringToGooString( name );
  LinkDestinationData ldd(NULL, namedDest, data);
  LinkDestination *ld = new LinkDestination(ldd);
  delete namedDest;
  return ld;
}

bool Document::print(const TQString &fileName, TQValueList<int> pageList, double hDPI, double vDPI, int rotate)
{
  return print(fileName, pageList, hDPI, vDPI, rotate, -1, -1);
}

bool Document::print(const TQString &file, TQValueList<int> pageList, double hDPI, double vDPI, int rotate, int paperWidth, int paperHeight)
{
#if (POPPLER_VERSION_C >= 30000)
  std::vector<int> pages;
  TQValueList<int>::iterator it;
  for (it = pageList.begin();  it != pageList.end();  ++it ) {
	pages.push_back(*it);
  }
  PSOutputDev *psOut = new PSOutputDev(file.latin1(), &(data->doc), NULL, pages, psModePS, paperWidth, paperHeight);
#elif (POPPLER_VERSION_C >= 20000)
  PSOutputDev *psOut = new PSOutputDev(file.latin1(), &(data->doc), NULL, 1, data->doc.getNumPages(), psModePS, paperWidth, paperHeight);
#elif (POPPLER_VERSION_C >= 16000)
  PSOutputDev *psOut = new PSOutputDev(file.latin1(), &(data->doc), data->doc.getXRef(), data->doc.getCatalog(), NULL, 1, data->doc.getNumPages(), psModePS, paperWidth, paperHeight);
#else
  PSOutputDev *psOut = new PSOutputDev(file.latin1(), data->doc.getXRef(), data->doc.getCatalog(), NULL, 1, data->doc.getNumPages(), psModePS, paperWidth, paperHeight);
#endif

  if (psOut->isOk()) {
    TQValueList<int>::iterator it;
    for (it = pageList.begin(); it != pageList.end(); ++it )
      data->doc.displayPage(psOut, *it, hDPI, vDPI, rotate, gFalse, gTrue, gTrue);

    delete psOut;
    return true;
  } else {
    delete psOut;
    return false;
  }
}

}
