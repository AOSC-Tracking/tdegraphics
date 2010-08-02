/* This file is part of the KDE project
 * Copyright (C) 2002 Shane Wright <me@shanewright.co.uk>
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

#include <config.h>
#include "kfile_ico.h"

#include <kprocess.h>
#include <klocale.h>
#include <kgenericfactory.h>
#include <kstringvalidator.h>
#include <kdebug.h>

#include <tqdict.h>
#include <tqvalidator.h>
#include <tqcstring.h>
#include <tqfile.h>
#include <tqdatetime.h>

#if !defined(__osf__)
#include <inttypes.h>
#else
typedef unsigned long uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
#endif

typedef KGenericFactory<KIcoPlugin> IcoFactory;

K_EXPORT_COMPONENT_FACTORY(kfile_ico, IcoFactory( "kfile_ico" ))

KIcoPlugin::KIcoPlugin(TQObject *parent, const char *name,
                       const TQStringList &args)

    : KFilePlugin(parent, name, args)
{
    KFileMimeTypeInfo* info = addMimeTypeInfo( "image/x-ico" );

    KFileMimeTypeInfo::GroupInfo* group = 0L;

    group = addGroupInfo(info, "Technical", i18n("Technical Details"));

    KFileMimeTypeInfo::ItemInfo* item;

    item = addItemInfo(group, "Number", i18n("Number of Icons"), TQVariant::Int);

    item = addItemInfo(group, "Dimensions", i18n("Dimensions"), TQVariant::Size);
    item = addItemInfo(group, "Colors", i18n("Colors"), TQVariant::Int);

    item = addItemInfo(group, "DimensionsM", i18n("Dimensions (1st icon)"), TQVariant::Size);
    item = addItemInfo(group, "ColorsM", i18n("Colors (1st icon)"), TQVariant::Int);
}


bool KIcoPlugin::readInfo( KFileMetaInfo& info, uint what)
{


    TQFile file(info.path());

    if (!file.open(IO_ReadOnly))
    {
        kdDebug(7034) << "Couldn't open " << TQFile::encodeName(info.path()) << endl;
        return false;
    }

    TQDataStream dstream(&file);

    // ICO files are little-endian
    dstream.setByteOrder(TQDataStream::LittleEndian);


    // read the beginning of the file and make sure it looks ok
    uint16_t ico_reserved;
    uint16_t ico_type;
    uint16_t ico_count;

    dstream >> ico_reserved;
    dstream >> ico_type;
    dstream >> ico_count;

    if ((ico_reserved != 0) || (ico_type != 1) || (ico_count < 1))
        return false;


    // now loop through each of the icon entries
    uint8_t icoe_width;
    uint8_t icoe_height;
    uint8_t icoe_colorcount;
    uint8_t icoe_reserved;
    uint16_t icoe_planes;
    uint16_t icoe_bitcount;
    uint32_t icoe_bytesinres;
    uint32_t icoe_imageoffset;

    // read the data on the 1st icon
    dstream >> icoe_width;
    dstream >> icoe_height;
    dstream >> icoe_colorcount;
    dstream >> icoe_reserved;
    dstream >> icoe_planes;
    dstream >> icoe_bitcount;
    dstream >> icoe_bytesinres;
    dstream >> icoe_imageoffset;


    // output the useful bits
    KFileMetaInfoGroup group = appendGroup(info, "Technical");
    appendItem(group, "Number", ico_count);

    if (ico_count == 1) {
        appendItem(group, "Dimensions", TQSize(icoe_width, icoe_height));

        if (icoe_colorcount > 0)
            appendItem(group, "Colors", icoe_colorcount);
        else if (icoe_bitcount > 0)
            appendItem(group, "Colors", 2 ^ icoe_bitcount);

    } else {

        appendItem(group, "DimensionsM", TQSize(icoe_width, icoe_height));

        if (icoe_colorcount > 0)
            appendItem(group, "ColorsM", icoe_colorcount);
        else if (icoe_bitcount > 0)
            appendItem(group, "ColorsM", 2 ^ icoe_bitcount);

    }

    return true;
}

#include "kfile_ico.moc"
