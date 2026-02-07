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

#include "poppler-private.h"
#include "poppler-link-qt3.h"

#include <tqstring.h>

#include <goo/gmem.h>

#include <Outline.h>
#include <Link.h>

namespace Poppler {

/* borrowed from kpdf */
TQString unicodeToTQString(CONST_064 Unicode* u, int len)
{
    TQString ret;
    ret.setLength(len);
    TQChar* qch = (TQChar*) ret.unicode();
    for (;len;--len)
      *qch++ = (TQChar) *u++;
    return ret;
}

TQString UnicodeParsedString(CONST_064 GooString *s1)
{
    GBool isUnicode;
    int i;
    Unicode u;
    TQString result;
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
    while
    (
#       if (POPPLER_VERSION_C >= 25010000)
        i < s1->size()
#       else
        i < s1->getLength()
#       endif
    )
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
    return result;
}

GooString *TQStringToGooString(const TQString &s)
{
    int len = s.length();
    char *cstring = (char *)gmallocn(s.length(), sizeof(char));
    for (int i = 0; i < len; ++i)
      cstring[i] = s.at(i).unicode();
    GooString *ret = new GooString(cstring, len);
    gfree(cstring);
    return ret;
}


void DocumentData::addTocChildren( TQDomDocument * docSyn, TQDomNode * parent, OUTLINE_ITEMS_TYPE * items )
{
    int numItems = OUTLINE_ITEMS_LENGTH(items);
    for ( int i = 0; i < numItems; ++i )
    {
        // iterate over every object in 'items'
        OutlineItem * outlineItem =
#if (POPPLER_VERSION_C >= 76000)
		(*items)[i];
#else
		(OutlineItem *)items->get( i );
#endif

        // 1. create element using outlineItem's title as tagName
        TQString name;
#if (POPPLER_VERSION_C >= 24002000)
        const std::vector<Unicode> &uVec = outlineItem->getTitle();
        name = unicodeToTQString( uVec.data(), uVec.size() );
#else
        CONST_064 Unicode * uniChar = outlineItem->getTitle();
        int titleLength = outlineItem->getTitleLength();
        name = unicodeToTQString(uniChar, titleLength);
#endif

        if ( name.isEmpty() )
            continue;

        TQDomElement item = docSyn->createElement( name );
        parent->appendChild( item );

        // 2. find the page the link refers to
        CONST_064 ::LinkAction * a = outlineItem->getAction();
        if ( a && ( a->getKind() == actionGoTo || a->getKind() == actionGoToR ) )
        {
            // page number is contained/referenced in a LinkGoTo
            CONST_064 LinkGoTo * g = static_cast< CONST_064 LinkGoTo * >( a );
            CONST_064 LinkDest * destination = g->getDest();
            if ( !destination && g->getNamedDest() )
            {
                // no 'destination' but an internal 'named reference'. we could
                // get the destination for the page now, but it's VERY time consuming,
                // so better storing the reference and provide the viewport on demand
                CONST_064 GooString *s = g->getNamedDest();
#               if (POPPLER_VERSION_C >= 25010000)
                int sLen = s->size();
#               else
                int sLen = s->getLength();
#               endif
                TQChar *charArray = new TQChar[sLen];
                for (int i = 0; i < sLen; ++i)
                {
                    charArray[i] = TQChar(s->GOO_GET_CSTR()[i]);
                }
                TQString aux(charArray, sLen);
                item.setAttribute( "DestinationName", aux );
                delete[] charArray;
            }
            else if ( destination && destination->isOk() )
            {
                LinkDestinationData ldd(destination, NULL, this);
                item.setAttribute( "Destination", LinkDestination(ldd).toString() );
            }
            if ( a->getKind() == actionGoToR )
            {
                CONST_064 LinkGoToR * g2 = static_cast< CONST_064 LinkGoToR * >( a );
                item.setAttribute( "ExternalFileName", g2->getFileName()->GOO_GET_CSTR() );
            }
        }

        // 3. recursively descend over children
        outlineItem->open();
        OUTLINE_ITEMS_TYPE * children = outlineItem->getKids();
        if ( children )
            addTocChildren( docSyn, &item, children );
    }
}

}
