
/*
   Copyright (c) 2003,2004,2005 Clarence Dang <dang@kde.org>
   All rights reserved.
   
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   
   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
   
   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
   IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
   IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
   NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
   THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#ifndef __kptoolairspray_h__
#define __kptoolairspray_h__

#include <kpcommandhistory.h>
#include <kpcolor.h>
#include <kptool.h>

class TQPixmap;
class TQPoint;
class TQRect;
class TQString;
class TQTimer;

class kpMainWindow;
class kpToolAirSprayCommand;
class kpToolWidgetSpraycanSize;
class kpViewManager;

class kpToolAirSpray : public kpTool
{
Q_OBJECT
  

public:
    kpToolAirSpray (kpMainWindow *);
    virtual ~kpToolAirSpray ();

private:
    TQString haventBegunDrawUserMessage () const;

public:
    virtual void begin ();
    virtual void end ();

private slots:
    void slotSpraycanSizeChanged (int size);
    
public:
    virtual void beginDraw ();
    virtual void draw (const TQPoint &thisPoint, const TQPoint &, const TQRect &);
    virtual void cancelShape ();
    virtual void releasedAllButtons ();
    virtual void endDraw (const TQPoint &, const TQRect &);

public slots:
    void actuallyDraw ();

private:
    kpToolWidgetSpraycanSize *m_toolWidgetSpraycanSize;
    kpToolAirSprayCommand *m_currentCommand;
    TQTimer *m_timer;
    int m_size;
};

class kpToolAirSprayCommand : public kpCommand
{
public:
    kpToolAirSprayCommand (const kpColor &color, int size,
                           kpMainWindow *mainWindow);
    virtual ~kpToolAirSprayCommand ();

    virtual TQString name () const;

    virtual int size () const;
    
    virtual void execute ();
    virtual void unexecute ();

    // interface for TDEToolAirSpray
    void addPoints (const TQPointArray &points);
    void finalize ();
    void cancel ();

private:
    kpColor m_color;
    int m_size;

    TQPixmap *m_newPixmapPtr;
    TQPixmap m_oldPixmap;
    TQRect m_boundingRect;
};

#endif  // __kptoolairspray_h__
