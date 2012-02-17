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

#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <tqdict.h>
#include <tqmap.h>
#include <tqstrlist.h>
#include <tqstringlist.h>
#include <tqtimer.h>

#include <kdedmodule.h>
#include <kprocess.h>

namespace KMrml
{
    class DaemonData
    {
    public:
        DaemonData( const TQString& key, const TQString& cmd,
                    uint time, int numRestarts )
            : daemonKey( key ),
              commandline( cmd ),
              timeout( time ),
              apps( true ), // deep copies
              restartOnFailure( numRestarts ),
              process( 0L ),
              timer( 0L )
        {
        }
        ~DaemonData()
        {
            delete process;
            delete timer;
        }
        TQString daemonKey;
        TQString commandline;
        uint timeout;
        TQStrList apps;
        int restartOnFailure;
        KProcess *process;
        TQTimer *timer;
    };

    class Watcher : public KDEDModule
    {
        Q_OBJECT
//        
        K_DCOP

    public:
        Watcher( const TQCString& name = "daemonwatcher" );
        ~Watcher();

    k_dcop:
        virtual bool requireDaemon( const TQCString& clientAppId,
                                    const TQString& daemonKey,
                                    const TQString& commandline,
                                    uint timeout = 60 /* seconds */,
                                    int numRestarts = 5 );
        virtual void unrequireDaemon( const TQCString& clientAppId,
                                      const TQString& daemonKey );
        virtual TQStringList runningDaemons() const;

    k_dcop_signals:
        void daemonExited(const TQString& daemonKey, pid_t pid, int exitStatus);
        void daemonDied( const TQString& daemonKey, pid_t pid );

    protected:
        bool startDaemon( DaemonData *daemon );

    protected slots:
        virtual void slotTimeout();

    private:
        void unrequireDaemon( DaemonData *daemon, const TQCString& clientAppId);
        DaemonData *findDaemonFromProcess( KProcess *proc );
        DaemonData *findDaemonFromTimer( TQTimer *timer );

        void emitExited( DaemonData *daemon );
        void emitFailure( DaemonData *daemon );

    private slots:
        void slotProcExited( KProcess *proc );
        void slotAppUnregistered( const TQCString& appId );

        TQDict<DaemonData> m_daemons;
    };

}

#endif // LAUNCHER_H
