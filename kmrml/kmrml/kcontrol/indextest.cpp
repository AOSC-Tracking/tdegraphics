#include "indexer.h"
#include <kmrml_config.h>
#include "indextest.moc"

#include <tdeapplication.h>
#include <tdeconfig.h>
#include <tdeglobal.h>

using namespace KMrmlConfig;

IndexTest::IndexTest()
{
    KMrml::Config *config = new KMrml::Config( TDEGlobal::config() );
    Indexer *indexer = new Indexer( *config, this );
    connect( indexer, TQT_SIGNAL( finished( bool )), TQT_SLOT( slotFinished( bool )));
    connect( indexer, TQT_SIGNAL( progress( int, const TQString& )),
             TQT_SLOT( slotProgress( int, const TQString& )));

    indexer->startIndexing( "/home/gis/testcoll" );
}

IndexTest::~IndexTest()
{

}

void IndexTest::slotFinished( bool success )
{
    tqDebug("##### FINISHED: %i", success );
}

void IndexTest::slotProgress( int percent, const TQString& message )
{
    tqDebug("--- progress: %i: %s", percent, message.latin1());
}

int main( int argc, char **argv )
{
    TDEApplication app( argc, argv, "indextest" );
    IndexTest *test = new IndexTest();

    return app.exec();
}
