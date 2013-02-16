
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <tdecmdlineargs.h>
#include <tdeaboutdata.h>
#include <tdelocale.h>

#include "klineal.h"

#define RESTORE_RULERS(type) {				\
	int n = 1;					\
	while (TDEMainWindow::canBeRestored(n)) {		\
		type* ruler = new type;			\
		ruler->restore(n);			\
		a.setMainWidget(ruler);			\
		ruler->show();				\
		n++;					\
	}						\
}

static const char homePageURL[] =
	"http://www.snafu.de/~till/";
static const char freeFormText[] =
	"\"May the source be with you.\"";



static TDECmdLineOptions options[] =
{
  TDECmdLineLastOption
  // INSERT YOUR COMMANDLINE OPTIONS HERE
};

int main(int argc, char *argv[])
{


  TDEAboutData aboutData( "kruler", I18N_NOOP("TDE Screen Ruler"),
    VERSION,
		I18N_NOOP("A screen ruler for the Trinity Desktop Environment"),
		TDEAboutData::License_GPL,
    "(c) 2000, Till Krech",
		freeFormText,
		homePageURL);
  aboutData.addAuthor("Till Krech",I18N_NOOP("Programming"), "till@snafu.de");
  aboutData.addCredit("Gunnstein Lye",I18N_NOOP("Initial port to KDE 2"), "gl@ez.no");
  TDECmdLineArgs::init( argc, argv, &aboutData );
  TDECmdLineArgs::addCmdLineOptions( options ); // Add our own options.

	TDEApplication a;

  if (a.isRestored()) {
    RESTORE_RULERS(KLineal)
  }
  else {
    KLineal *ruler = new KLineal();
    a.setMainWidget(ruler);
    ruler->show();
  }

  return a.exec();
}
