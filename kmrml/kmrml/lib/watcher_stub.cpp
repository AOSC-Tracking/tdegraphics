//
// Generated in ../server/ via dcopidl -- needs to be in the lib tho.
// Regenerate when necessary by uncommenting the watcher.stub in
// ../server/Makefile.am
//

#include "watcher_stub.h"
#include <dcopclient.h>

#include <kdatastream.h>

namespace KMrml {

Watcher_stub::Watcher_stub( const TQCString& app, const TQCString& obj )
  : DCOPStub( app, obj )
{
}

Watcher_stub::Watcher_stub( DCOPClient* client, const TQCString& app, const TQCString& obj )
  : DCOPStub( client, app, obj )
{
}

bool Watcher_stub::requireDaemon( const TQCString& arg0, const TQString& arg1, const TQString& arg2, uint arg3, int arg4 )
{
    bool result;
    if ( !dcopClient()  ) {
	setStatus( CallFailed );
	return false;
    }
    TQByteArray data, replyData;
    TQCString replyType;
    TQDataStream arg( data, IO_WriteOnly );
    arg << arg0;
    arg << arg1;
    arg << arg2;
    arg << arg3;
    arg << arg4;
    if ( dcopClient()->call( app(), obj(), "requireDaemon(TQCString,TQString,TQString,uint,int)", data, replyType, replyData ) ) {
	if ( replyType == "bool" ) {
	    TQDataStream _reply_stream( replyData, IO_ReadOnly );
	    _reply_stream >> result;
	    setStatus( CallSucceeded );
	} else {
	    callFailed();
	}
    } else { 
	callFailed();
    }
    return result;
}

void Watcher_stub::unrequireDaemon( const TQCString& arg0, const TQString& arg1 )
{
    if ( !dcopClient()  ) {
	setStatus( CallFailed );
	return;
    }
    TQByteArray data, replyData;
    TQCString replyType;
    TQDataStream arg( data, IO_WriteOnly );
    arg << arg0;
    arg << arg1;
    if ( dcopClient()->call( app(), obj(), "unrequireDaemon(TQCString,TQString)", data, replyType, replyData ) ) {
	setStatus( CallSucceeded );
    } else { 
	callFailed();
    }
}

TQStringList Watcher_stub::runningDaemons()
{
    TQStringList result;
    if ( !dcopClient()  ) {
	setStatus( CallFailed );
	return result;
    }
    TQByteArray data, replyData;
    TQCString replyType;
    if ( dcopClient()->call( app(), obj(), "runningDaemons()", data, replyType, replyData ) ) {
	if ( replyType == "TQStringList" ) {
	    TQDataStream _reply_stream( replyData, IO_ReadOnly );
	    _reply_stream >> result;
	    setStatus( CallSucceeded );
	} else {
	    callFailed();
	}
    } else { 
	callFailed();
    }
    return result;
}

} // namespace

