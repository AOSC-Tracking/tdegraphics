/* This file is in the public domain */

// $Id$

#include "kviewtemplate.h"

#include <tqobjectlist.h>

#include <tdeaction.h>
/*#include <tdelocale.h>*/
#include <kgenericfactory.h>
#include <kdebug.h>
#include <kimageviewer/viewer.h>

typedef KGenericFactory<KViewTemplate> KViewTemplateFactory;
K_EXPORT_COMPONENT_FACTORY( kview_templateplugin, KViewTemplateFactory( "kviewtemplateplugin" ) )

KViewTemplate::KViewTemplate( TQObject* parent, const char* name, const TQStringList & )
	: Plugin( parent, name )
{
	TQObjectList * viewerList = parent->queryList( 0, "KImageViewer Part", false, false );
	m_pViewer = static_cast<KImageViewer::Viewer *>( viewerList->getFirst() );
	delete viewerList;
	if( m_pViewer )
	{
		(void) new TDEAction( /*i18n(*/ "&Do Something" /*)*/, 0, 0,
							this, TQT_SLOT( yourSlot() ),
							actionCollection(), "plugin_template" );
	}
	else
		kdWarning( 4630 ) << "no KImageViewer interface found - the template plugin won't work" << endl;
}

KViewTemplate::~KViewTemplate()
{
}

void KViewTemplate::yourSlot()
{
}

// vim:sw=4:ts=4:cindent
#include "kviewtemplate.moc"
