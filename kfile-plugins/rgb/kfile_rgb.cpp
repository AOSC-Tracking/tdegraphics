/* This file is part of the KDE project
 * Copyright (C) 2004 Melchior FRANZ <mfranz@kde.org>
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
#include "kfile_rgb.h"

#include <tqfile.h>
#include <tqvalidator.h>

#include <kdebug.h>
#include <kgenericfactory.h>


typedef KGenericFactory<KRgbPlugin> RgbFactory;

K_EXPORT_COMPONENT_FACTORY(kfile_rgb, RgbFactory("kfile_rgb"))


KRgbPlugin::KRgbPlugin(TQObject *tqparent, const char *name, const TQStringList &args) :
	KFilePlugin(tqparent, name, args)
{
	KFileMimeTypeInfo* info = addMimeTypeInfo("image/x-rgb");

	KFileMimeTypeInfo::GroupInfo* group = 0;
	KFileMimeTypeInfo::ItemInfo* item;


	group = addGroupInfo(info, "Comment", i18n("Comment"));

	item = addItemInfo(group, "ImageName", i18n("Name"), TQVariant::String);
	setAttributes(item, KFileMimeTypeInfo::Modifiable);
	setHint(item, KFileMimeTypeInfo::Description);


	group = addGroupInfo(info, "Technical", i18n("Technical Details"));

	item = addItemInfo(group, "Dimensions", i18n("Dimensions"), TQVariant::Size);
	setHint(item, KFileMimeTypeInfo::Size);
	setUnit(item, KFileMimeTypeInfo::Pixels);

	item = addItemInfo(group, "BitDepth", i18n("Bit Depth"), TQVariant::Int);
	setUnit(item, KFileMimeTypeInfo::BitsPerPixel);

	item = addItemInfo(group, "ColorMode", i18n("Color Mode"), TQVariant::String);
	item = addItemInfo(group, "Compression", i18n("Compression"), TQVariant::String);
	item = addItemInfo(group, "SharedRows",
			i18n("percentage of avoided vertical redundancy (the higher the better)",
			"Shared Rows"), TQVariant::String);

}


bool KRgbPlugin::readInfo(KFileMetaInfo& info, uint /*what*/)
{
	TQFile file(info.path());

	if (!file.open(IO_ReadOnly)) {
		kdDebug(7034) << "Couldn't open " << TQFile::encodeName(info.path()).data() << endl;
		return false;
	}

	TQDataStream dstream(&file);

	TQ_UINT16 magic;
	TQ_UINT8  storage;
	TQ_UINT8  bpc;
	TQ_UINT16 dimension;
	TQ_UINT16 xsize;
	TQ_UINT16 ysize;
	TQ_UINT16 zsize;
	TQ_UINT32 pixmin;
	TQ_UINT32 pixmax;
	TQ_UINT32 dummy;
	char     imagename[80];
	TQ_UINT32 colormap;

	dstream >> magic;
	dstream >> storage;
	dstream >> bpc;
	dstream >> dimension;
	dstream >> xsize;
	dstream >> ysize;
	dstream >> zsize;
	dstream >> pixmin;
	dstream >> pixmax;
	dstream >> dummy;
	dstream.readRawBytes(imagename, 80);
	imagename[79] = '\0';
	dstream >> colormap;
	TQ_UINT8 u8;
	for (uint i = 0; i < 404; i++)
		dstream >> u8;

	if (magic != 474)
		return false;

	KFileMetaInfoGroup group;

	group = appendGroup(info, "Technical");

	if (dimension == 1)
		ysize = 1;
	appendItem(group, "Dimensions", TQSize(xsize, ysize));
	appendItem(group, "BitDepth", zsize * 8 * bpc);

	if (zsize == 1)
		appendItem(group, "ColorMode", i18n("Grayscale"));
	else if (zsize == 2)
		appendItem(group, "ColorMode", i18n("Grayscale/Alpha"));
	else if (zsize == 3)
		appendItem(group, "ColorMode", i18n("RGB"));
	else if (zsize == 4)
		appendItem(group, "ColorMode", i18n("RGB/Alpha"));

	if (!storage)
		appendItem(group, "Compression", i18n("Uncompressed"));
	else if (storage == 1) {
		long compressed = file.size() - 512;
		long verbatim = xsize * ysize * zsize;
		appendItem(group, "Compression", i18n("Runlength Encoded")
				+ TQString(", %1%").tqarg(compressed * 100.0 / verbatim, 0, 'f', 1));

		long k;
		TQ_UINT32 offs;
		TQMap<TQ_UINT32, uint> map;
		TQMap<TQ_UINT32, uint>::Iterator it;
		TQMap<TQ_UINT32, uint>::Iterator end = map.end();
		for (k = 0; k < (ysize * zsize); k++) {
			dstream >> offs;
			if ((it = map.tqfind(offs)) != end)
				map.tqreplace(offs, it.data() + 1);
			else
				map[offs] = 0;
		}
		for (k = 0, it = map.begin(); it != end; ++it)
			k += it.data();

		if (k)
			appendItem(group, "SharedRows", TQString("%1%").tqarg(k * 100.0
					/ (ysize * zsize), 0, 'f', 1));
		else
			appendItem(group, "SharedRows", i18n("None"));
	} else
		appendItem(group, "Compression", i18n("Unknown"));


	group = appendGroup(info, "Comment");
	appendItem(group, "ImageName", imagename);

	file.close();
	return true;
}


bool KRgbPlugin::writeInfo(const KFileMetaInfo& info) const
{
	TQFile file(info.path());

	if (!file.open(IO_WriteOnly|IO_Raw)) {
		kdDebug(7034) << "couldn't open " << TQFile::encodeName(info.path()).data() << endl;
		return false;
	}

	if (!file.at(24)) {
		kdDebug(7034) << "couldn't set offset" << endl;
		return false;
	}

	TQDataStream dstream(&file);
	TQString s = info["Comment"]["ImageName"].value().toString();
	s.truncate(79);

	unsigned i;
	for (i = 0; i < s.length(); i++)
		dstream << TQ_UINT8(s.latin1()[i]);
	for (; i < 80; i++)
		dstream << TQ_UINT8(0);

	file.close();
	return true;
}


// restrict to 79 ASCII characters
TQValidator* KRgbPlugin::createValidator(const TQString&, const TQString &,
		const TQString &, TQObject* tqparent, const char* name) const
{
	return new TQRegExpValidator(TQRegExp("[\x0020-\x007E]{79}"), tqparent, name);
}


#include "kfile_rgb.moc"
