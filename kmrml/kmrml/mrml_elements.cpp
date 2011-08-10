/* This file is part of the KDE project
   Copyright (C) 2002 Carsten Pfeiffer <pfeiffer@kde.org>

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

#include "mrml_elements.h"
#include "mrml_shared.h"

#include <kdatastream.h>

#include <tqdom.h>

using namespace KMrml;

//
// MrmlElement is currently the baseclass for Algorithm and Collection. Both
// may have a single child-element <query-paradigm-list>, with a number of
// <query-paradigm> elements as tqchildren.
//

MrmlElement::MrmlElement( const TQDomElement& elem )
{
    TQValueList<TQDomElement> list =
        KMrml::directChildElements( elem, MrmlShared::queryParadigmList() );

    Q_ASSERT( list.count() < 2 ); // There can be only one.

    if ( list.count() )
        m_paradigms.initFromDOM( list.first() );
}


void MrmlElement::setOtherAttributes( TQDomElement& elem ) const
{
    TQMapConstIterator<TQString,TQString> it = m_attributes.begin();
    for ( ; it != m_attributes.end(); ++it )
    {
        elem.setAttribute( it.key(), it.data() );
    }
}


///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////


AlgorithmList AlgorithmList::algorithmsForCollection( const Collection& coll ) const
{
    AlgorithmList list;

    AlgorithmList::ConstIterator it = begin();
    for ( ; it != end(); ++it )
    {
        Algorithm algo = *it;
        if ( algo.paradigms().matches( coll.paradigms() ) )
        {
            algo.setCollectionId( coll.id() );
            list.append( algo );
        }
    }

    return list;
}

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////


Collection::Collection( const TQDomElement& elem )
    : MrmlElement( elem )
{
    TQDomNamedNodeMap attrs = elem.attributes();
    for ( uint i = 0; i < attrs.length(); i++ )
    {
        TQDomAttr attribute = attrs.item( i ).toAttr();
        TQString name = attribute.name();

        if ( name == MrmlShared::collectionName() )
            m_name = attribute.value();
        else if ( name == MrmlShared::collectionId() )
            m_id = attribute.value();

        else // custom attributes
            m_attributes.insert( name, attribute.value() );
    }
}


///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////


Algorithm::Algorithm( const TQDomElement& elem )
    : MrmlElement( elem )
{
    TQDomNamedNodeMap attrs = elem.attributes();

    for ( uint i = 0; i < attrs.length(); i++ )
    {
        TQDomAttr attribute = attrs.item( i ).toAttr();
        TQString name = attribute.name();

        if ( name == MrmlShared::algorithmName() )
            m_name = attribute.value();
        else if ( name == MrmlShared::algorithmId() )
            m_id = attribute.value();
        else if ( name == MrmlShared::algorithmType() )
            m_type = attribute.value();

        // not really necessary
        else if ( name == MrmlShared::collectionId() )
            m_collectionId = attribute.value();

        else // custom attributes
            m_attributes.insert( name, attribute.value() );
    }

    TQDomElement propsElem = firstChildElement(elem, MrmlShared::propertySheet());
    m_propertySheet.initFromDOM( propsElem );

    qDebug("############# new algorithm: name: %s, id: %s, type: %s", m_name.latin1(), m_id.latin1(), m_type.latin1());
}

Algorithm Algorithm::defaultAlgorithm()
{
    Algorithm algo;
    algo.m_id = "adefault";
    algo.m_type = "adefault"; // ### not in the DTD
    algo.m_name = "dummy";

    return algo;
}

TQDomElement Algorithm::toElement( TQDomElement& tqparent ) const
{
    TQDomDocument doc = tqparent.ownerDocument();
    TQDomElement algorithm = doc.createElement( MrmlShared::algorithm() );
    tqparent.appendChild( algorithm );
    setOtherAttributes( algorithm );

    if ( !m_name.isEmpty() )
        algorithm.setAttribute( MrmlShared::algorithmName(), m_name );
    if ( !m_id.isEmpty() )
        algorithm.setAttribute( MrmlShared::algorithmId(), m_id );
    if ( !m_type.isEmpty() )
        algorithm.setAttribute( MrmlShared::algorithmType(), m_type );

    if ( !m_collectionId.isEmpty() )
        algorithm.setAttribute( MrmlShared::collectionId(), m_collectionId );
    return algorithm;
}

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////


QueryParadigm::QueryParadigm( const TQDomElement& elem )
{
    TQDomNamedNodeMap attrs = elem.attributes();
    for ( uint i = 0; i < attrs.count(); i++ )
    {
        TQDomAttr attr = attrs.item( i ).toAttr();
        m_attributes.insert( attr.name(), attr.value() );
        if ( attr.name() == "type" )
            m_type = attr.value();
    }
}

bool QueryParadigm::matches( const QueryParadigm& other ) const
{
    return m_attributes.isEmpty() || other.m_attributes.isEmpty() ||
        equalMaps( m_attributes, other.m_attributes );
}

bool QueryParadigm::equalMaps( const TQMap<TQString,TQString> m1,
                               const TQMap<TQString,TQString> m2 )
{
    if ( m1.count() != m2.count() )
        return false;

    TQMapConstIterator<TQString,TQString> it = m1.begin();
    for ( ; it != m1.end(); ++it )
    {
        TQMapConstIterator<TQString,TQString> it2 = m2.find( it.key() );
        if ( it2 == m2.end() || it.data() != it2.data() )
            return false;
    }

    return true;
}

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////


void QueryParadigmList::initFromDOM( const TQDomElement& elem )
{
    clear();

    TQValueList<TQDomElement> list =
        KMrml::directChildElements( elem, MrmlShared::queryParadigm() );

    TQValueListConstIterator<TQDomElement> it = list.begin();
    for ( ; it != list.end(); ++it )
    {
        append( QueryParadigm( *it ));
    }
}

// two QueryParadigmLists match, when there is at least one pair of
// QueryParadigms that match (all attribute-value pairs are equal, or there
// are no attributes at all).
bool QueryParadigmList::matches( const QueryParadigmList& other ) const
{
    ConstIterator it = begin();

    for ( ; it != end(); ++it )
    {
        ConstIterator oit = other.begin();
        for ( ; oit != other.end(); ++oit )
            if ( (*it).matches( *oit ) )
                return true;
    }

    return false;
}


///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

TQValueList<TQDomElement> KMrml::directChildElements( const TQDomElement& tqparent,
                                                    const TQString& tagName )
{
    TQValueList<TQDomElement> list;

    TQDomNode node = tqparent.firstChild();
    while ( !node.isNull() )
    {
        if ( node.isElement() && node.nodeName() == tagName )
            list.append( node.toElement() );

        node = node.nextSibling();
    }

    return list;
}

TQDomElement KMrml::firstChildElement( const TQDomElement& tqparent,
                                      const TQString& tagName )
{
    TQDomNode node = tqparent.firstChild();
    while ( !node.isNull() )
    {
        if ( node.isElement() && node.nodeName() == tagName )
            return node.toElement();

        node = node.nextSibling();
    }

    return TQDomElement();
}

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////


TQDataStream& KMrml::operator<<( TQDataStream& stream, const QueryParadigm& )
{

    return stream;
}
TQDataStream& KMrml::operator>>( TQDataStream& stream, QueryParadigm& )
{

    return stream;
}

TQDataStream& KMrml::operator<<( TQDataStream& stream, const QueryParadigmList& )
{

    return stream;
}
TQDataStream& KMrml::operator>>( TQDataStream& stream, QueryParadigmList& )
{

    return stream;
}

TQDataStream& KMrml::operator<<( TQDataStream& stream, const MrmlElement& )
{

    return stream;
}
TQDataStream& KMrml::operator>>( TQDataStream& stream, MrmlElement& )
{

    return stream;
}

TQDataStream& KMrml::operator<<( TQDataStream& stream, const Algorithm& )
{

    return stream;
}
TQDataStream& KMrml::operator>>( TQDataStream& stream, Algorithm& )
{

    return stream;
}

TQDataStream& KMrml::operator<<( TQDataStream& stream, const Collection& )
{

    return stream;
}
TQDataStream& KMrml::operator>>( TQDataStream& stream, Collection& )
{
    return stream;
}

template <class t> TQDataStream& KMrml::operator<<( TQDataStream& stream,
                                                   const MrmlElementList<t>& )
{

    return stream;
}
template <class t> TQDataStream& KMrml::operator>>( TQDataStream& stream,
                                                   MrmlElementList<t>& )
{

    return stream;
}

TQDataStream& KMrml::operator<<( TQDataStream& stream, const AlgorithmList& )
{

    return stream;
}
TQDataStream& KMrml::operator>>( TQDataStream& stream, AlgorithmList& )
{

    return stream;
}

