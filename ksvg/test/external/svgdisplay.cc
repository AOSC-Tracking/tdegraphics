#include <kapplication.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#include <kaboutdata.h>
#include "SVGTestWidget.h"

static KCmdLineOptions options[] =
{
	{ "+URL",   I18N_NOOP("SVG file to open"), 0 },
	KCmdLineLastOption
};

int main(int argc, char **argv)
{

	TDEAboutData *about = new TDEAboutData( "KSVG test app", I18N_NOOP("KSVG test"), "0.1", I18N_NOOP("KSVG standalone test app") );
	TDECmdLineArgs::init(argc, argv, about);
	TDECmdLineArgs::addCmdLineOptions( options );
	TDECmdLineArgs *args = TDECmdLineArgs::parsedArgs();
	if(args->count() == 0)
	{
		args->usage();
		return 0;
	}
	KURL url = args->url(0);
	TDEApplication a( argc, argv );
	SVGTestWidget *w = new SVGTestWidget(url);
	a.setMainWidget(w);
	w->show();

	return a.exec();
}
