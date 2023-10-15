/* This file is part of the KDE project
   Copyright (C) 2002 Carsten Pfeiffer <pfeiffer@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation, version 2.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include <dcopclient.h>

#include <tdeapplication.h>
#include <kdebug.h>
#include <tdeversion.h>
#include <tdelocale.h>
#include <tdemessagebox.h>

#include "watcher.h"

using namespace KMrml;

Watcher::Watcher( const TQCString& name )
    : KDEDModule( name )
{
    m_daemons.setAutoDelete( true );

    // safety, for clients that die without unregistering
    TDEApplication::dcopClient()->setNotifications( true );
    connect( TDEApplication::dcopClient(),
             TQT_SIGNAL( applicationRemoved( const TQCString& )),
             TQT_SLOT( slotAppUnregistered( const TQCString& )));
}

Watcher::~Watcher()
{
    TDEApplication::dcopClient()->setNotifications( false );
}

bool Watcher::requireDaemon( const TQCString& clientAppId,
                             const TQString& daemonKey,
                             const TQString& commandline,
                             uint timeout /* seconds */,
                             int restartOnFailure )
{
    if ( !TDEApplication::dcopClient()->isApplicationRegistered( clientAppId ) )
        kdWarning() << "Watcher::requireDaemon: " << daemonKey
                    << ": Client AppID is not registered with DCOP: "
                    << clientAppId << endl;

    DaemonData *daemon = m_daemons.find( daemonKey );

    if ( daemon )
    {
        if ( !daemon->apps.find( clientAppId ) )
            daemon->apps.append( clientAppId );

        // timeout, commandline and restart values are: first come, first serve
        return true; // process already running, all fine
    }

    else // start daemon
    {
        daemon = new DaemonData( daemonKey, commandline,
                                 timeout, restartOnFailure );
        m_daemons.insert( daemonKey, daemon );
        daemon->apps.append( clientAppId );

#if TDE_VERSION >= 306
        daemon->process = new TDEProcess();
        daemon->process->setUseShell( true );
#else
        daemon->process = new KShellProcess();
#endif
        daemon->process->setEnvironment( "LC_ALL", "C" );
        daemon->process->setEnvironment( "LANG", "C" );
        daemon->process->setEnvironment( "LANGUAGE", "C" );
        *daemon->process << commandline;
        connect( daemon->process, TQT_SIGNAL( processExited( TDEProcess * ) ),
                 TQT_SLOT( slotProcExited( TDEProcess * )));
        return startDaemon( daemon );
    }
}

void Watcher::unrequireDaemon( const TQCString& clientAppId,
                               const TQString& daemonKey )
{
    unrequireDaemon( m_daemons.find( daemonKey ), clientAppId );
}

void Watcher::unrequireDaemon( DaemonData *daemon,
                               const TQCString& clientAppId )
{
    if ( daemon )
    {
        daemon->apps.remove( clientAppId.data() );
        if ( daemon->apps.isEmpty() )
        {
            if ( !daemon->timer )
            {
                daemon->timer = new TQTimer();
                connect( daemon->timer, TQT_SIGNAL( timeout() ),
                         TQT_SLOT( slotTimeout() ));
            }
            daemon->timer->start( daemon->timeout * 1000, true );
        }
    }
    else
        kdWarning() << "Watcher::unrequireDaemon: daemon unknown. client: "
                    << clientAppId << endl;
}

TQStringList Watcher::runningDaemons() const
{
    TQStringList result;
    TQDictIterator<DaemonData> it( m_daemons );
    for ( ; it.current(); ++it )
        result.append( it.current()->commandline );

    return result;
}

void Watcher::slotProcExited( TDEProcess *proc )
{
    DaemonData *daemon = findDaemonFromProcess( proc );

    if ( proc->normalExit() )
    {
        emitExited( daemon );
        return;
    }

    if ( daemon )
    {
        if ( --daemon->restartOnFailure <= 0 )
        {
            if ( KMessageBox::questionYesNo( 0L,
                             i18n("<qt>The server with the command line"
                                  "<br>%1<br>"
                                  "is not available anymore. Do you want to "
                                  "restart it?" ).arg( daemon->commandline ),
                                            i18n("Service Failure"), i18n("Restart Server"), i18n("Do Not Restart") )
                 == KMessageBox::Yes )
            {
                daemon->restartOnFailure = 1;
            }
        }

        if ( daemon->restartOnFailure > 0 )
        {
            startDaemon( daemon );
            return;
        }
    }

    emitFailure( daemon );
}

bool Watcher::startDaemon( DaemonData *daemon )
{
    if ( daemon->process->start( TDEProcess::NotifyOnExit ) )
        return true;

    else
    {
        if ( KMessageBox::questionYesNo( 0L,
                      i18n("Unable to start the server with the "
                           "command line"
                           "<br>%1<br>"
                          "Try again?").arg( daemon->commandline ),
                                         i18n("Service Failure"), i18n("Try Again"), i18n("Do Not Try") )
             == KMessageBox::Yes )
        {
            return startDaemon( daemon );
        }
    }

    return false;
}

void Watcher::slotTimeout()
{
    TQTimer *timer = const_cast<TQTimer *>(static_cast<const TQTimer *>(sender()));
    DaemonData *daemon = findDaemonFromTimer( timer );
    if ( daemon )
    {
        if ( daemon->apps.isEmpty() )
        {
            // the daemon and TDEProcess might get deleted by killing the
            // TDEProcess (through slotProcExited()), so don't dereference
            // daemon after proc->kill()
            TQString key = daemon->daemonKey;

            // noone registered during the timeout, so kill the daemon
            if ( !daemon->process->kill() )
                daemon->process->kill( SIGKILL );

            m_daemons.remove( key );
        }
    }
}

DaemonData * Watcher::findDaemonFromProcess( TDEProcess *proc )
{
    DaemonData *daemon;
    TQDictIterator<DaemonData> it( m_daemons );
    for ( ; (daemon = it.current()); ++it )
    {
        if ( daemon->process == proc )
            return daemon;
    }

    return 0L;
}

DaemonData * Watcher::findDaemonFromTimer( TQTimer *timer )
{
    DaemonData *daemon;
    TQDictIterator<DaemonData> it( m_daemons );
    for ( ; (daemon = it.current()); ++it )
    {
        if ( daemon->timer == timer )
            return daemon;
    }

    return 0L;
}

void Watcher::slotAppUnregistered( const TQCString& appId )
{
    if ( m_daemons.isEmpty() )
        return;

    DaemonData *daemon;
    TQDictIterator<DaemonData> it( m_daemons );
    for ( ; (daemon = it.current()); ++it )
    {
        if ( daemon->apps.find( appId ) != -1 )
            unrequireDaemon( daemon, appId );
    }
}

void Watcher::emitExited( DaemonData *daemon )
{
    if ( daemon )
    {
        daemonExited( daemon->daemonKey,
                      daemon->process->pid(),
                      daemon->process->exitStatus() );

        m_daemons.remove( daemon->daemonKey );
    }
}

void Watcher::emitFailure( DaemonData *daemon )
{
    if ( daemon )
    {
        daemonDied( daemon->daemonKey, daemon->process->pid() );
        m_daemons.remove( daemon->daemonKey ); // deletes daemon + TDEProcess
    }
}

extern "C" {
    KDE_EXPORT KDEDModule *create_daemonwatcher(const TQCString & obj )
    {
        return new Watcher( obj );
    }
}


#include "watcher.moc"
