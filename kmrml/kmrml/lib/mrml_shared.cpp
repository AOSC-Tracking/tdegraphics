/* This file is part of the KDE project
   Copyright (C) 2002 Carsten Pfeiffer <pfeiffer@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "mrml_shared.h"

// mrml stuff
const TQString * MrmlShared::m_sessionId = 0L;
const TQString * MrmlShared::m_transactionId = 0L;
const TQString * MrmlShared::m_algorithm = 0L;
const TQString * MrmlShared::m_algorithmId = 0L;
const TQString * MrmlShared::m_algorithmName = 0L;
const TQString * MrmlShared::m_algorithmList = 0L;
const TQString * MrmlShared::m_algorithmType = 0L;
const TQString * MrmlShared::m_collectionId = 0L;
const TQString * MrmlShared::m_collectionList = 0L;
const TQString * MrmlShared::m_collection = 0L;
const TQString * MrmlShared::m_collectionName = 0L;
const TQString * MrmlShared::m_queryParadigm = 0L;
const TQString * MrmlShared::m_queryParadigmList = 0L;
const TQString * MrmlShared::m_configureSession = 0L;

const TQString * MrmlShared::m_propertySheet = 0L;
const TQString * MrmlShared::m_propertySheetId = 0L;
const TQString * MrmlShared::m_propertySheetType = 0L;
const TQString * MrmlShared::m_sendName = 0L;
const TQString * MrmlShared::m_sendType = 0L;
const TQString * MrmlShared::m_sendValue = 0L;
const TQString * MrmlShared::m_maxSubsetSize = 0L;
const TQString * MrmlShared::m_minSubsetSize = 0L;
const TQString * MrmlShared::m_caption = 0L;
const TQString * MrmlShared::m_from = 0L;
const TQString * MrmlShared::m_to = 0L;
const TQString * MrmlShared::m_step = 0L;
const TQString * MrmlShared::m_sendBooleanInverted = 0L;

const TQString * MrmlShared::m_element = 0L;
const TQString * MrmlShared::m_attribute = 0L;
const TQString * MrmlShared::m_attributeName = 0L;
const TQString * MrmlShared::m_attributeValue = 0L;
const TQString * MrmlShared::m_children = 0L;
const TQString * MrmlShared::m_none = 0L;

const TQString * MrmlShared::m_multiSet = 0L;
const TQString * MrmlShared::m_subset = 0L;
const TQString * MrmlShared::m_setElement = 0L;
const TQString * MrmlShared::m_boolean = 0L;
const TQString * MrmlShared::m_numeric = 0L;
const TQString * MrmlShared::m_textual = 0L;
const TQString * MrmlShared::m_panel = 0L;
const TQString * MrmlShared::m_clone = 0L;
const TQString * MrmlShared::m_reference = 0L;

const TQString * MrmlShared::m_visibility = 0L;
const TQString * MrmlShared::m_visible = 0L;
const TQString * MrmlShared::m_invisible = 0L;
const TQString * MrmlShared::m_popup = 0L;
// const TQString * MrmlShared::m_ = 0L;

// meta-data
const TQString * MrmlShared::m_mrml_data = 0L;

// kio_mrml tasks
const TQString * MrmlShared::m_kio_task = 0L;
const TQString * MrmlShared::m_kio_initialize = 0L;
const TQString * MrmlShared::m_kio_startQuery = 0L;


int MrmlShared::s_references = 0;

void MrmlShared::ref()
{
    if ( s_references == 0 )
        init();

    s_references++;
}

bool MrmlShared::deref()
{
    if ( s_references > 0 )
        s_references--;

    if ( s_references == 0 )
    {
        // ### delete all strings here...

        return true;
    }

    return false;
}

void MrmlShared::init()
{
    m_sessionId = new TQString ( "session-id" ) ;
    m_transactionId = new TQString ( "transaction-id" ) ;
    m_algorithm = new TQString ( "algorithm" ) ;
    m_algorithmId = new TQString ( "algorithm-id" ) ;
    m_algorithmName = new TQString ( "algorithm-name" ) ;
    m_algorithmList = new TQString ( "algorithm-list" ) ;
    m_algorithmType = new TQString ( "algorithm-type" ) ;
    m_collectionId = new TQString ( "collection-id" ) ;
    m_collectionList = new TQString ( "collection-list" ) ;
    m_collection = new TQString ( "collection" ) ;
    m_collectionName = new TQString ( "collection-name" ) ;
    m_queryParadigm = new TQString ( "query-paradigm" ) ;
    m_queryParadigmList = new TQString ( "query-paradigm-list" ) ;
    m_configureSession = new TQString ( "configure-session" ) ;

    m_propertySheet = new TQString ( "property-sheet" ) ;
    m_propertySheetId = new TQString ( "property-sheet-id" ) ;
    m_propertySheetType = new TQString ( "property-sheet-type" ) ;
    m_sendName = new TQString ( "send-name" ) ;
    m_sendType = new TQString ( "send-type" ) ;
    m_sendValue = new TQString ( "send-value" ) ;
    m_maxSubsetSize = new TQString ( "maxsubsetsize" ) ;
    m_minSubsetSize = new TQString ( "minsubsetsize" ) ;
    m_caption = new TQString ( "caption" ) ;
    m_from = new TQString ( "from" ) ;
    m_to = new TQString ( "to" ) ;
    m_step = new TQString ( "step" ) ;
    m_sendBooleanInverted = new TQString ( "send-boolean-inverted" ) ;

    m_element = new TQString ( "element" ) ;
    m_attribute = new TQString ( "attribute" ) ;
    m_attributeName = new TQString ( "attribute-name" ) ;
    m_attributeValue = new TQString ( "attribute-value" ) ;
    m_children = new TQString ( "children" ) ;
    m_none = new TQString ( "none" ) ;

    m_multiSet = new TQString ( "multi-set" ) ;
    m_subset = new TQString ( "subset" ) ;
    m_setElement = new TQString ( "set-element" ) ;
    m_boolean = new TQString ( "boolean" ) ;
    m_numeric = new TQString ( "numeric" ) ;
    m_textual = new TQString ( "textual" ) ;
    m_panel = new TQString ( "panel" ) ;
    m_clone = new TQString ( "clone" ) ;
    m_reference = new TQString ( "reference" ) ;

    m_visibility = new TQString ( "visibility" ) ;
    m_visible = new TQString ( "visible" ) ;
    m_invisible = new TQString ( "invisible" ) ;
    m_popup = new TQString ( "popup" ) ;
// m_ = new TQString ( "" ) ;

// meta-data
    m_mrml_data = new TQString ( "mrml_data" ) ;

// kio_mrml tasks
    m_kio_task = new TQString ( "kio_task" ) ;
    m_kio_initialize = new TQString ( "kio_initialize" ) ;
    m_kio_startQuery = new TQString ( "kio_startQuery" ) ;
}

void MrmlShared::cleanup()
{
    delete m_sessionId;
    delete m_transactionId;
    delete m_algorithm;
    delete m_algorithmId;
    delete m_algorithmName;
    delete m_algorithmList;
    delete m_algorithmType;
    delete m_collectionId;
    delete m_collectionList;
    delete m_collection;
    delete m_collectionName;
    delete m_queryParadigm;
    delete m_queryParadigmList;
    delete m_configureSession;

    // property sheet stuff
    delete m_propertySheet;
    delete m_propertySheetId;
    delete m_propertySheetType;
    delete m_sendName;
    delete m_sendType;
    delete m_sendValue;
    delete m_maxSubsetSize;
    delete m_minSubsetSize;
    delete m_caption;
    delete m_from;
    delete m_to;
    delete m_step;
    delete m_sendBooleanInverted;

    delete m_multiSet;
    delete m_subset;
    delete m_setElement;
    delete m_boolean;
    delete m_numeric;
    delete m_textual;
    delete m_panel;
    delete m_clone;
    delete m_reference;

    delete m_element;
    delete m_attribute;
    delete m_attributeName;
    delete m_attributeValue;
    delete m_children;
    delete m_none;

    delete m_visibility;
    delete m_visible;
    delete m_invisible;
    delete m_popup;
//     delete m_;

    // meta-data
    delete m_mrml_data;

    // kio_mrml tasks
    delete m_kio_task;
    delete m_kio_initialize;
    delete m_kio_startQuery;

}
