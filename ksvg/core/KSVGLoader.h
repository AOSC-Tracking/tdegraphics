/*
    Copyright (C) 2001-2003 KSVG Team
    This file is part of the KDE project

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef KSVGLoader_H
#define KSVGLoader_H

#include <tqobject.h>

class KURL;

namespace KIO
{
	class Job;
	class TransferJob;
}

namespace KJS
{
	class Object;
	class ExecState;
}

namespace KSVG
{

struct ImageStreamMap;

typedef struct
{
	KIO::Job *job;
	KJS::ExecState *exec;
	KJS::Object *callBackFunction, *status;
} PostUrlData;
	
class SVGImageElementImpl;
class SVGElementImpl;
class SVGDocumentImpl;
class KSVGLoader : public QObject
{
Q_OBJECT
public:
	KSVGLoader();
	~KSVGLoader();

	void getSVGContent(::KURL url);
	void newImageJob(SVGImageElementImpl *impl, ::KURL url);

	static TQString getUrl(::KURL url, bool local = false);
	void postUrl(::KURL url, const TQByteArray &data, const TQString &mimeType, KJS::ExecState *exec, KJS::Object &callBackFunction, KJS::Object &status);
	static TQString getCharacterData(::KURL url, const TQString &id);
	static SVGElementImpl *getSVGFragment(::KURL, SVGDocumentImpl *doc, const TQString &id);

signals:
	void gotResult(TQIODevice *);
	void imageReady(SVGImageElementImpl *);

private slots:
	void slotData(KIO::Job *, const TQByteArray &);
	void slotResult(KIO::Job *);

private:
	static TQString loadXML(::KURL);

	PostUrlData m_postUrlData;

	TQByteArray m_data;
	KIO::TransferJob *m_job;
	TQMap<KIO::TransferJob *, ImageStreamMap *> m_imageJobs;
};

}

#endif

// vim:ts=4:noet
