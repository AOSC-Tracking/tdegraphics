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

#ifndef SHARED_H
#define SHARED_H

// maybe use mrml_const.h from libMRML, unfortunately not installed
// by gift 0.1.6pre2

#include <tqshared.h>
#include <tqstring.h>

class MrmlShared
{
public:
// attribute/element names for mrml
    static void ref();
    static bool deref();

    static const TQString& sessionId()           { return *m_sessionId; }
    static const TQString& transactionId()       { return *m_transactionId; }
    static const TQString& algorithm()           { return *m_algorithm; }
    static const TQString& algorithmId()         { return *m_algorithmId; }
    static const TQString& algorithmName()       { return *m_algorithmName; }
    static const TQString& algorithmList()       { return *m_algorithmList; }
    static const TQString& algorithmType()       { return *m_algorithmType; }
    static const TQString& collectionId()        { return *m_collectionId; }
    static const TQString& collectionList()      { return *m_collectionList; }
    static const TQString& collection()          { return *m_collection; }
    static const TQString& collectionName()      { return *m_collectionName; }
    static const TQString& queryParadigm()       { return *m_queryParadigm; }
    static const TQString& queryParadigmList()   { return *m_queryParadigmList; }
    static const TQString& configureSession()    { return *m_configureSession; }

    // property sheet stuff
    static const TQString& propertySheet()       { return *m_propertySheet; }
    static const TQString& propertySheetId()     { return *m_propertySheetId; }
    static const TQString& propertySheetType()   { return *m_propertySheetType; }
    static const TQString& sendName()            { return *m_sendName; }
    static const TQString& sendType()            { return *m_sendType; }
    static const TQString& sendValue()           { return *m_sendValue; }
    static const TQString& maxSubsetSize()       { return *m_maxSubsetSize; }
    static const TQString& minSubsetSize()       { return *m_minSubsetSize; }
    static const TQString& caption()             { return *m_caption; }
    static const TQString& from()                { return *m_from; }
    static const TQString& to()                  { return *m_to; }
    static const TQString& step()                { return *m_step; }
    static const TQString& sendBooleanInverted() { return *m_sendBooleanInverted; }

    static const TQString& multiSet()            { return *m_multiSet; }
    static const TQString& subset()              { return *m_subset; }
    static const TQString& setElement()          { return *m_setElement; }
    static const TQString& boolean()             { return *m_boolean; }
    static const TQString& numeric()             { return *m_numeric; }
    static const TQString& textual()             { return *m_textual; }
    static const TQString& panel()               { return *m_panel; }
    static const TQString& clone()               { return *m_clone; }
    static const TQString& reference()           { return *m_reference; }

    static const TQString& element()             { return *m_element; }
    static const TQString& attribute()           { return *m_attribute; }
    static const TQString& attributeName()       { return *m_attributeName; }
    static const TQString& attributeValue()      { return *m_attributeValue; }
    static const TQString& children()            { return *m_children; }
    static const TQString& none()                { return *m_none; }

    static const TQString& visibility()          { return *m_visibility; }
    static const TQString& visible()             { return *m_visible; }
    static const TQString& invisible()           { return *m_invisible; }
    static const TQString& popup()               { return *m_popup; }
//     static const TQString& ()                 { return *m_; }

    // meta-data
    static const TQString& mrml_data()           { return *m_mrml_data; }

    // tdeio_mrml tasks
    static const TQString& tdeio_task()            { return *m_tdeio_task; }
    static const TQString& tdeio_initialize()      { return *m_tdeio_initialize; }
    static const TQString& tdeio_startQuery()      { return *m_tdeio_startQuery; }


private:
    static const TQString * m_sessionId;
    static const TQString * m_transactionId;
    static const TQString * m_algorithm;
    static const TQString * m_algorithmId;
    static const TQString * m_algorithmName;
    static const TQString * m_algorithmList;
    static const TQString * m_algorithmType;
    static const TQString * m_collectionId;
    static const TQString * m_collectionList;
    static const TQString * m_collection;
    static const TQString * m_collectionName;
    static const TQString * m_queryParadigm;
    static const TQString * m_queryParadigmList;
    static const TQString * m_configureSession;

    // property sheet stuff
    static const TQString * m_propertySheet;
    static const TQString * m_propertySheetId;
    static const TQString * m_propertySheetType;
    static const TQString * m_sendName;
    static const TQString * m_sendType;
    static const TQString * m_sendValue;
    static const TQString * m_maxSubsetSize;
    static const TQString * m_minSubsetSize;
    static const TQString * m_caption;
    static const TQString * m_from;
    static const TQString * m_to;
    static const TQString * m_step;
    static const TQString * m_sendBooleanInverted;

    static const TQString * m_multiSet;
    static const TQString * m_subset;
    static const TQString * m_setElement;
    static const TQString * m_boolean;
    static const TQString * m_numeric;
    static const TQString * m_textual;
    static const TQString * m_panel;
    static const TQString * m_clone;
    static const TQString * m_reference;

    static const TQString * m_element;
    static const TQString * m_attribute;
    static const TQString * m_attributeName;
    static const TQString * m_attributeValue;
    static const TQString * m_children;
    static const TQString * m_none;

    static const TQString * m_visibility;
    static const TQString * m_visible;
    static const TQString * m_invisible;
    static const TQString * m_popup;
//     static const TQString * m_;

    // meta-data
    static const TQString * m_mrml_data;

    // tdeio_mrml tasks
    static const TQString * m_tdeio_task;
    static const TQString * m_tdeio_initialize;
    static const TQString * m_tdeio_startQuery;

private:
    static void cleanup();
    static void init();

    static int s_references;
};

#endif // SHARED_H
