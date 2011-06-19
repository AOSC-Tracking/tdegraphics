/* This file is part of the KDE project
   Copyright (C) 2001 Carsten Pfeiffer <pfeiffer@kde.org>

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

#ifndef LOADER_H
#define LOADER_H

#include <tqbuffer.h>
#include <tqcstring.h>
#include <tqmap.h>
#include <tqobject.h>

#include <kio/job.h>
#include <kurl.h>

class Download
{
public:
    ~Download() {
        if ( m_buffer.isOpen() )
            m_buffer.close();
    }
    TQBuffer m_buffer;
    // add context of MrmlPart for progress?
};


class Loader : public TQObject
{
    friend class gcc_sucks;
    Q_OBJECT
  TQ_OBJECT

public:
    static Loader *self();
    ~Loader();

    void requestDownload( const KURL& url );

    void removeDownload( const KURL& url );

signals:
    void finished( const KURL& url, const TQByteArray& );

private slots:
    void slotData( KIO::Job *, const TQByteArray& );
    void slotResult( KIO::Job * );

private:
    Loader();

    TQMap<KIO::TransferJob*,Download*> m_downloads;
    typedef TQMapIterator<KIO::TransferJob*,Download*> DownloadIterator;

    static Loader *s_self;

};

#endif // LOADER_H
