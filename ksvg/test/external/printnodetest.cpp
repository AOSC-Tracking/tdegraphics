#include <DocumentFactory.h>
#include <SVGSVGElement.h>
#include <SVGDocument.h>
#include <SVGWindow.h>

#include <tdeapplication.h>
#include <tdecmdlineargs.h>
#include <tdelocale.h>
#include <tdeaboutdata.h>
#include <kdebug.h>

#include "printnodetest.moc"

using namespace KSVG;

static TDECmdLineOptions options[] =
{
	{ "+URL",   I18N_NOOP("SVG file to open"), 0 },
	TDECmdLineLastOption
};

Worker::Worker(const KURL &url)
{
	m_doc = DocumentFactory::self()->requestDocument(this, TQT_SLOT(slotParsingFinished(bool, const TQString &)));

	if(!m_doc)
		return;
	
	if(!DocumentFactory::self()->startParsing(m_doc, url))
		return;
}

Worker::~Worker()
{
	delete m_doc;
}

void Worker::slotParsingFinished(bool, const TQString &)
{
	SVGWindow win = m_doc->window();	
	SVGSVGElement root = m_doc->rootElement();
	
	kdDebug() << win.printNode(root).string() << endl;
	exit(0);
}

int main(int argc, char **argv)
{
	TDEAboutData *about = new TDEAboutData("KSVG printNode test app", I18N_NOOP("KSVG test"), "0.1", I18N_NOOP("KSVG printNode test app"));
	TDECmdLineArgs::init(argc, argv, about);
	TDECmdLineArgs::addCmdLineOptions(options);
	TDECmdLineArgs *args = TDECmdLineArgs::parsedArgs();
	if(args->count() == 0)
	{
		args->usage();
		return -1;
	}
	
	TDEApplication a(argc, argv);
	(void) new Worker(args->url(0));
	return a.exec();
}

