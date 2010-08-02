/* This file is part of the KDE project
 * Copyright (C) 2005 Steffen Hansen <hansen@kde.org>
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
 */

#ifndef KCAMERARAWPLUGIN_H
#define KCAMERARAWPLUGIN_H

#include <kfilemetainfo.h>

class TQImage;

class KCameraRawPlugin: public KFilePlugin {
    Q_OBJECT

public:
    KCameraRawPlugin(TQObject *parent, const char *name, const TQStringList& args);
    virtual bool readInfo(KFileMetaInfo& info, uint what);

private:
    bool createPreview(const TQString &path, TQImage &img);
};

#endif /* KCAMERARAWPLUGIN_H */
