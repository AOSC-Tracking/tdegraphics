//
// Generated in ../server/ via dcopidl -- needs to be in the lib tho.
// Regenerate when necessary by uncommenting the watcher.stub in
// ../server/Makefile.am
//

#ifndef __WATCHER_STUB__
#define __WATCHER_STUB__

#include <dcopstub.h>
#include <tqdict.h>
#include <tqptrlist.h>
#include <tqmap.h>
#include <tqstrlist.h>
#include <tqstringlist.h>
#include <tqtimer.h>
#include <kdedmodule.h>
#include <kprocess.h>

namespace KMrml {

class Watcher_stub : public DCOPStub
{
public:
    Watcher_stub( const TQCString& app, const TQCString& id );
    Watcher_stub( DCOPClient* client, const TQCString& app, const TQCString& id );
    virtual bool requireDaemon( const TQCString& clientAppId, const TQString& daemonKey, const TQString& commandline, uint timeout, int numRestarts );
    virtual void unrequireDaemon( const TQCString& clientAppId, const TQString& daemonKey );
    virtual TQStringList runningDaemons();
protected:
    Watcher_stub() : DCOPStub( never_use ) {};
};

} // namespace

#endif
