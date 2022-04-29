/* poppler-private.h: qt interface to poppler
 * Copyright (C) 2005, Net Integration Technologies, Inc.
 * Copyright (C) 2005-2008, Albert Astals Cid <aacid@kde.org>
 * Copyright (C) 2006, Kristian Høgsberg <krh@bitplanet.net>
 * Copyright (C) 2006, Wilfried Huss <Wilfried.Huss@gmx.at>
 * Copyright (C) 2007, Pino Toscano <pino@kde.org>
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

#ifndef POPPLER_PRIVATE_H
#define POPPLER_PRIVATE_H

#include <tqdom.h>

#include <config.h>
#include <Object.h>
#include <PDFDoc.h>
#include <FontInfo.h>

#if defined(HAVE_SPLASH)
# undef HAVE_SPLASH
#endif

#if defined(HAVE_SPLASH)
#include <SplashOutputDev.h>
#else
class SplashOutputDev;
#endif
#if defined(HAVE_POPPLER_064)
#define CONST_064 const
#else
#define CONST_064
#endif
#if defined(HAVE_POPPLER_071)
#define GBool  bool
#define gTrue  true
#define gFalse false
#endif
#if defined(HAVE_POPPLER_072)
#define GOO_GET_CSTR c_str
#else
#define GOO_GET_CSTR getCString
#endif
#if defined(HAVE_POPPLER_076)
#include <vector>
class OutlineItem;
#define OUTLINE_ITEMS_TYPE const std::vector<OutlineItem*>
#define OUTLINE_ITEMS_LENGTH(goo) goo->size()
#  if defined(HAVE_POPPLER_082)
#define FONTS_LIST_TYPE std::vector<::FontInfo*>
#define FONTS_LIST_LENGTH(goo) goo.size()
#define FONTS_LIST_IS_EMPTY(goo) goo.empty()
#define FONTS_LIST_GET(goo, i) goo[i]
#  else
#define FONTS_LIST_TYPE std::vector<::FontInfo*>*
#define FONTS_LIST_LENGTH(goo) goo->size()
#define FONTS_LIST_IS_EMPTY(goo) goo->empty()
#define FONTS_LIST_GET(goo, i) (*goo)[i]
#  endif
#define FIND_PAGE_ARGS(ref)	ref
#else
#define OUTLINE_ITEMS_TYPE CONST_064 GooList
#define OUTLINE_ITEMS_LENGTH(goo) goo->getLength()
#define FONTS_LIST_TYPE GooList*
#define FONTS_LIST_LENGTH(goo) goo->getLength()
#define FONTS_LIST_IS_EMPTY(goo) (goo == NULL || goo->getLength() == 0)
#define FONTS_LIST_GET(goo, i) (::FontInfo*)goo->get(i)
#define FIND_PAGE_ARGS(ref)	ref.num, ref.gen
#endif

namespace Poppler {
    
class DocumentData;

TQString unicodeToTQString(CONST_064 Unicode* u, int len);

TQString UnicodeParsedString(CONST_064 GooString *s1);

GooString *TQStringToGooString(const TQString &s);

class LinkDestinationData {
  public:
     LinkDestinationData( CONST_064 LinkDest *l, GooString *nd, Poppler::DocumentData *pdfdoc ) : ld(l), namedDest(nd), doc(pdfdoc)
     {
     }

     CONST_064 LinkDest *ld;
     GooString *namedDest;
     Poppler::DocumentData *doc;
};

class DocumentData {
  public:
#   if defined(HAVE_POPPLER_2203)
    DocumentData(std::unique_ptr<GooString> &&filePath, const std::optional<GooString> &password) : doc(std::move(filePath), password), m_fontInfoScanner(0), m_outputDev(0) {}
#   else
    DocumentData(GooString *filePath, GooString *password) : doc(filePath,password), m_fontInfoScanner(0), m_outputDev(0) {}
#   endif

    ~DocumentData()
    {
#if defined(HAVE_SPLASH)
        delete m_outputDev;
#endif
        delete m_fontInfoScanner;
    }

    SplashOutputDev *getOutputDev()
    {
#if defined(HAVE_SPLASH)
        if (!m_outputDev)
        {
            SplashColor white;
            white[0] = 255;
            white[1] = 255;
            white[2] = 255;
            m_outputDev = new SplashOutputDev(splashModeXBGR8, 4, gFalse, white);
            m_outputDev->startDoc(doc.getXRef());
        }
#endif
        return m_outputDev;
    }

    void addTocChildren( TQDomDocument * docSyn, TQDomNode * parent, OUTLINE_ITEMS_TYPE * items );

  class PDFDoc doc;
  bool locked;
  FontInfoScanner *m_fontInfoScanner;
  SplashOutputDev *m_outputDev;
};

}

#endif // POPPLER_PRIVATE_H
