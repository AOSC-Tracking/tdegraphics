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

#ifndef PROPERTYSHEET_H
#define PROPERTYSHEET_H

#include <tqdom.h>
#include <tqstring.h>
#include <tqptrlist.h>

class TQWidget;

namespace KMrml
{
    class PropertySheet
    {
    public:
        enum Type
        {
            MultiSet = 1,       // ??
            Subset,             // radio-button/combobox or listbox
            SetElement,         // CheckBox -> disables/enables children?
            Boolean,            // CheckBox
            Numeric,            // Slider/Spinbox
            Textual,            // lineedit
            Panel,              // groupbox?
            Clone,
            Reference
        };
        enum Visibility
        {
            Visible,
            Invisible,
            Popup
        };
        enum SendType
        {
            Element = 1,
            Attribute,
            AttributeName,
            AttributeValue,
            Children,
            None
        };

        PropertySheet();
        PropertySheet( const TQDomElement& elem );
        PropertySheet( const PropertySheet& ps );
        ~PropertySheet() {};

        PropertySheet& operator=( const PropertySheet& ps );

        bool isValid() const {
            // required mrml attributes
            return !m_id.isNull() && m_type != 0 && m_sendType != 0;
        }
        void initFromDOM( const TQDomElement& elem );

        void toElement( TQDomElement& parent );

        TQWidget * createWidget( TQWidget *parent, const char *name = 0 );

    private:
        static Visibility getVisibility( const TQString& value );
        static Type getType( const TQString& value );
        static SendType getSendType( const TQString& value );
        static int toInt( const TQString& value, int defaultValue = 0 );

        void init();


        // update operator=() when adding data members!

        TQPtrList<PropertySheet> m_subSheets;
        Visibility m_visibility;
        Type m_type;
        TQString m_caption;
        TQString m_id;

        SendType m_sendType;
        TQString m_sendName;
        TQString m_sendValue;

        int m_minRange;
        int m_maxRange;
        int m_stepSize;

        // Type = Subset && m_minSubsetSize == m_maxSubsetSize == 1 -> Combobox
        // or radio buttons.
        // > max > 1  -> Listbox with multiselection
        int m_minSubsetSize;
        int m_maxSubsetSize;

    };

}

#endif // PROPERTYSHEET_H
