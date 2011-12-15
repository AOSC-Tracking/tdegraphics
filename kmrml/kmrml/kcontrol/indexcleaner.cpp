#include <kdebug.h>
#include <kprocess.h>

#include <kmrml_config.h>
#include "indexcleaner.h"

#include <tdeversion.h>
#if TDE_VERSION < 306
  #define TQUOTE( x ) x
#else
  #define TQUOTE( x ) KProcess::quote( x )
#endif

using namespace KMrmlConfig;

IndexCleaner::IndexCleaner( const TQStringList& dirs,
                            const KMrml::Config *config,
                            TQObject *parent, const char *name )
    : TQObject( parent, name ),
      m_dirs( dirs ),
      m_config( config ),
      m_process( 0L )
{
    m_stepSize = 100 / dirs.count();
}

IndexCleaner::~IndexCleaner()
{
    if ( m_process )
    {
        m_process->kill();
        delete m_process;
        m_process = 0L;
    }
}

void IndexCleaner::start()
{
    startNext();
}

void IndexCleaner::slotExited( KProcess *proc )
{
    emit advance( m_stepSize );

    if ( !proc->normalExit() )
        kdWarning() << "Error removing old indexed directory" << endl;

    m_process = 0L;

    startNext();
}

void IndexCleaner::startNext()
{
    if ( m_dirs.isEmpty() )
    {
        emit advance( 100 );
        emit finished();
        return;
    }

#if TDE_VERSION < 306
    m_process = new KShellProcess();
#else
    m_process = new KProcess();
    m_process->setUseShell( true );
#endif
    connect( m_process, TQT_SIGNAL( processExited( KProcess * )),
             TQT_SLOT( slotExited( KProcess * ) ));

    TQString cmd = m_config->removeCollectionCommandLine();

    TQString dir = m_dirs.first();
    m_dirs.pop_front();

    int index = cmd.find( "%d" );
    if ( index != -1 )
        cmd.replace( index, 2, TQUOTE( dir ) );
    else // no %d? What else can we do?
        cmd.append( TQString::fromLatin1(" ") + TQUOTE( dir ) );

    *m_process << cmd;

    if ( !m_process->start() )
    {
        kdWarning() << "Error starting: " << cmd << endl;

        delete m_process;
        m_process = 0L;

        startNext();
    }
}

#include "indexcleaner.moc"
