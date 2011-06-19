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

#ifndef MRML_ELEMENTS_H
#define MRML_ELEMENTS_H

#include <tqdom.h>
#include <tqmap.h>
#include <tqstringlist.h>
#include <tqvaluelist.h>

#include "mrml_shared.h"
#include "propertysheet.h"

#include <assert.h>

namespace KMrml
{
    class PropertySheet;

    class QueryParadigm
    {
    public:
        QueryParadigm() {}
        QueryParadigm( const TQDomElement& elem );
        bool matches( const QueryParadigm& other ) const;
        TQString type() const { return m_type; }

//         bool operator== ( const QueryParadigm& p1, const QueryParadigm& p2 )

    private:
        TQString m_type;
        TQMap<TQString,TQString> m_attributes;

        static bool equalMaps( const TQMap<TQString,TQString>,
                               const TQMap<TQString,TQString> );
    };

    class QueryParadigmList : protected TQValueList<QueryParadigm>
    {
    public:
        typedef TQValueListIterator<QueryParadigm> Iterator;
        typedef TQValueListConstIterator<QueryParadigm> ConstIterator;

        void initFromDOM( const TQDomElement& elem );
        bool matches( const QueryParadigmList& other ) const;
    };

    class MrmlElement
    {
    public:
        MrmlElement() {}
        MrmlElement( const TQDomElement& elem );
        virtual ~MrmlElement() {}

        TQString id() const      { return m_id;   }
        TQString name() const    { return m_name; }
        TQString attribute( const TQString& name ) const { return m_attributes[ name ]; }
        QueryParadigmList paradigms() const { return m_paradigms; }


        TQMapConstIterator<TQString,TQString> attributeIterator() const {
            return m_attributes.begin();
        }
        TQMapConstIterator<TQString,TQString> end() const { return m_attributes.end(); }

        bool isValid() const { return !m_name.isNull() && !m_id.isNull(); }

    protected:
        TQString m_id;
        TQString m_name;
        QueryParadigmList m_paradigms;
        TQMap<TQString,TQString> m_attributes;

        void setOtherAttributes( TQDomElement& elem ) const;
    };

    class Algorithm : public MrmlElement
    {
    public:
        Algorithm() { m_collectionId = "adefault"; }
        Algorithm( const TQDomElement& elem );
        TQString type() const { return m_type; }

        TQString collectionId() const
        {
            return m_collectionId;
        }
        void setCollectionId( const TQString& id )
        {
            m_collectionId = id;
        }

        TQDomElement toElement( TQDomElement& tqparent ) const;
        const PropertySheet& propertySheet() const;

        static Algorithm defaultAlgorithm();

    private:
        TQString m_type;
        PropertySheet m_propertySheet;

        TQString m_collectionId;
    };

    class Collection : public MrmlElement
    {
    public:
        Collection() {}
        Collection( const TQDomElement& elem );
    };

    template <class t> class MrmlElementList : public TQValueList<t>
    {
    public:
        typedef TQValueListIterator<t> Iterator;
        typedef TQValueListConstIterator<t> ConstIterator;

        /**
         * Creates an invalid element.
         */
        MrmlElementList( const TQString& tagName ) :
            TQValueList<t>(),
            m_tagName( tagName ) {}
        MrmlElementList( const TQDomElement& elem, const TQString& tagName ) :
            TQValueList<t>(),
            m_tagName( tagName )
        {
            initFromDOM( elem );
        }
        virtual ~MrmlElementList() {};

        void initFromDOM( const TQDomElement& elem )
        {
            assert( !m_tagName.isEmpty() );

            TQValueList<t>::clear();

            TQDomNodeList list = elem.elementsByTagName( m_tagName );
            for ( uint i = 0; i < list.length(); i++ )
            {
                TQDomElement elem = list.item( i ).toElement();
                t item( elem );
                if ( item.isValid() )
                    append( item );
            }
        }

        t findByName( const TQString& name ) const
        {
            TQValueListConstIterator<t> it = TQValueList<t>::begin();
            for ( ; it != TQValueList<t>::end(); ++it )
            {
                if ( (*it).name() == name )
                    return *it;
            }

            return t();
        }

        t findById( const TQString& id ) const
        {
            TQValueListConstIterator<t> it = TQValueList<t>::begin();
            for ( ; it != TQValueList<t>::end(); ++it )
            {
                if ( (*it).id() == id )
                    return *it;
            }

            return MrmlElement();
        }

        TQStringList itemNames() const {
            TQStringList list;
            TQValueListConstIterator<t> it = TQValueList<t>::begin();
            for ( ; it != TQValueList<t>::end(); ++it )
                list.append( (*it).name() );

            return list;
        }

        void setItemName( const TQString& tagName ) { m_tagName = tagName; }
        TQString tagName() const { return m_tagName; }

    private:
        TQString m_tagName;
        MrmlElementList();
    };

    class AlgorithmList : public MrmlElementList<Algorithm>
    {
    public:
        AlgorithmList() :
            MrmlElementList<Algorithm>( MrmlShared::algorithm() )
        {}

        AlgorithmList algorithmsForCollection( const Collection& coll ) const;
    };

    class CollectionList : public MrmlElementList<Collection>
    {
    public:
        CollectionList() :
            MrmlElementList<Collection>( MrmlShared::collection() )
        {}
    };


    TQValueList<TQDomElement> directChildElements( const TQDomElement& tqparent,
                                                 const TQString& tagName);
    TQDomElement firstChildElement( const TQDomElement& tqparent,
                                   const TQString& tagName );


    TQDataStream& operator<<( TQDataStream& stream, const QueryParadigm& );
    TQDataStream& operator>>( TQDataStream& stream, QueryParadigm& );

    TQDataStream& operator<<( TQDataStream& stream, const QueryParadigmList& );
    TQDataStream& operator>>( TQDataStream& stream, QueryParadigmList& );

    TQDataStream& operator<<( TQDataStream& stream, const MrmlElement& );
    TQDataStream& operator>>( TQDataStream& stream, MrmlElement& );

    TQDataStream& operator<<( TQDataStream& stream, const Algorithm& );
    TQDataStream& operator>>( TQDataStream& stream, Algorithm& );

    TQDataStream& operator<<( TQDataStream& stream, const Collection& );
    TQDataStream& operator>>( TQDataStream& stream, Collection& );

    template <class t> TQDataStream& operator<<( TQDataStream&,
                                                const MrmlElementList<t>& );
    template <class t> TQDataStream& operator>>( TQDataStream&,
                                                MrmlElementList<t>& );

    TQDataStream& operator<<( TQDataStream&, const AlgorithmList& );
    TQDataStream& operator>>( TQDataStream&, AlgorithmList& );

}

#endif // MRML_ELEMENTS_H
