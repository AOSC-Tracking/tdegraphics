/*
 * This file is part of the KDE project, added by Bryce Nesbitt
 *
 * This is a plugin for Konqeror/KFile which processes 'extra' information
 * contained in a .gif image file (In particular the comment, and resolution).
 *
 **************************************
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

#include <stdlib.h>
#include "tdefile_gif.h"

#include <kurl.h>
#include <kprocess.h>
#include <tdelocale.h>
#include <kgenericfactory.h>
#include <kdebug.h>

#include <tqcstring.h>
#include <tqfile.h>
#include <tqdatetime.h>
#include <tqdict.h>
#include <tqvalidator.h>
#include <tqimage.h>

typedef KGenericFactory<KGifPlugin> GifFactory;

K_EXPORT_COMPONENT_FACTORY(tdefile_gif, GifFactory("tdefile_gif"))

KGifPlugin::KGifPlugin(TQObject *parent, const char *name,
                       const TQStringList &args)
    : KFilePlugin(parent, name, args)
{
    kdDebug(7034) << "gif KFileMetaInfo plugin\n";

    KFileMimeTypeInfo* info = addMimeTypeInfo( "image/gif" );

    KFileMimeTypeInfo::GroupInfo* group = 0L;

    group = addGroupInfo(info, "General", i18n("General"));

    KFileMimeTypeInfo::ItemInfo* item;

    item = addItemInfo(group, "Version", i18n("Version"), TQVariant::String);

    item = addItemInfo( group, "Dimensions", i18n("Dimensions"), TQVariant::Size );
    setHint( item, KFileMimeTypeInfo::Size );
    setUnit( item, KFileMimeTypeInfo::Pixels );

    item = addItemInfo(group, "BitDepth", i18n("Bit Depth"), TQVariant::Int);
    setUnit(item, KFileMimeTypeInfo::BitsPerPixel);

}

bool KGifPlugin::readInfo( KFileMetaInfo& info, uint what )
{
    Q_UNUSED( what );

    kdDebug(7034) << "gif KFileMetaInfo readInfo\n";

    TQFile file(info.path());

    if (!file.open(IO_ReadOnly)) {
	kdDebug(7034) << "Couldn't open " << TQFile::encodeName(info.path()).data() << endl;
	return false;
    }

    TQDataStream fstream(&file);

    bool isGIF87a = false;
    char magic[7];
    TQ_UINT16 width = 0;
    TQ_UINT16 height = 0;
    TQ_UINT8 miscInfo = 0;

    fstream.readRawBytes( magic, 6 ); 
    magic[6] = 0x00; // null terminate

    // I have special requirements...
    fstream.setByteOrder( TQDataStream::LittleEndian );
    fstream >> width;
    fstream >> height;
    fstream >> miscInfo;

    KFileMetaInfoGroup group = appendGroup(info, "General");

    if ( 0 == strncmp( magic, "GIF89a", 6 ) ) {
	appendItem( group, "Version", i18n("GIF Version 89a") );
    } else if ( 0 == strncmp( magic, "GIF87a", 6 ) ) {
	appendItem( group, "Version", i18n("GIF Version 87a") );
	isGIF87a = true;
    } else {
	appendItem( group, "Version", i18n("Unknown") );
    }

    appendItem( group, "Dimensions", TQSize(width, height) );

    if ( isGIF87a ) {
	appendItem( group, "BitDepth", ( (miscInfo & 0x07) + 1) );
    }

    file.close();

    return true;
}

#include "tdefile_gif.moc"
