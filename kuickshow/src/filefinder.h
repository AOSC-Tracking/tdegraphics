/* This file is part of the KDE project
   Copyright (C) 1998-2003 Carsten Pfeiffer <pfeiffer@kde.org>

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

#ifndef FILEFINDER_H
#define FILEFINDER_H

#include <tqevent.h>

#include <klineedit.h>

class KURLCompletion;

class FileFinder : public KLineEdit
{
    Q_OBJECT

public:
    FileFinder( TQWidget *parent=0, const char *name=0 );
    ~FileFinder();

    KURLCompletion *completion() {
        return static_cast<KURLCompletion*>( completionObject() );
    }

    virtual void hide();

signals:
    void enterDir( const TQString& );

protected:
    virtual void focusOutEvent( TQFocusEvent * );
    virtual void keyPressEvent( TQKeyEvent * );

private slots:
    void slotAccept( const TQString& );

};

#endif // FILEFINDER_H
