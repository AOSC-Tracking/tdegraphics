/* This file is part of the KDE project
 * Copyright (C) 2003 <bradh@frogmouth.net>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *  $Id$
 */

#ifndef __TDEFILE_EXR_H__
#define __TDEFILE_EXR_H__

#include <tdefilemetainfo.h>
#include <kurl.h>

class TQStringList;

class KExrPlugin: public KFilePlugin
{
    TQ_OBJECT
  
    
public:
    KExrPlugin( TQObject *parent, const char *name, const TQStringList& preferredItems );

    virtual bool readInfo( KFileMetaInfo& info, uint );
};

#endif
