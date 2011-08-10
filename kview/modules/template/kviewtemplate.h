/* This file is in the public domain */

// $Id$

#ifndef __kviewtemplate_h
#define __kviewtemplate_h

#include <kparts/plugin.h>

namespace KImageViewer { class Viewer; }

class KViewTemplate : public KParts::Plugin
{
	Q_OBJECT
  TQ_OBJECT
public:
	KViewTemplate( TQObject* parent, const char* name, const TQStringList & );
	virtual ~KViewTemplate();

private slots:
	void yourSlot();

private:
	KImageViewer::Viewer * m_pViewer;
};

// vim:sw=4:ts=4:cindent
#endif
