/* This file is part of the KDE project
   Copyright (C) 2001,2002 Carsten Pfeiffer <pfeiffer@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation, version 2.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef MRML_CREATOR_H
#define MRML_CREATOR_H

#include <tqdom.h>

#include <kurl.h>

#include "mrml_elements.h"
#include "mrml_shared.h"

namespace MrmlCreator
{
    enum Relevance { Relevant = 1, Irrelevant = -1 };

    TQDomElement createMrml( TQDomDocument& doc,
                            const TQString& sessionId,
                            const TQString& transactionId = TQString::null );
    TQDomElement configureSession( TQDomElement& mrml,
                                  const KMrml::Algorithm& algo,
                                  const TQString& sessionId );
    TQDomElement addQuery( TQDomElement& mrml, int resultSize );
    TQDomElement addRelevanceList( TQDomElement& query );
    /**
     * Creates a <user-relevance-element> with the given attributes set.
     */
    void createRelevanceElement( TQDomDocument& doc, TQDomElement& parent,
                                 const TQString& url, Relevance relevance );

}

#endif // MRML_CREATOR_H
