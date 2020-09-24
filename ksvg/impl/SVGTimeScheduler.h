/*
    Copyright (C) 2003 KSVG Team
    This file is part of the KDE project

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef SVGTimeScheduler_H
#define SVGTimeScheduler_H

#include <tqtimer.h>
#include <tqdatetime.h>
#include <tqobject.h>
#include <tqvaluelist.h>

#include "SVGElementImpl.h"
#include "SVGAnimationElementImpl.h"

namespace KSVG
{

typedef TQValueList<SVGElementImpl *> SVGNotifyList;
class SVGTimer
{
public:
	SVGTimer(TQObject *scheduler, unsigned int ms, bool singleShot);
	~SVGTimer();

	bool operator==(const TQTimer *timer);
	const TQTimer *qtimer() const;

	void start(TQObject *receiver, const char *member);
	void stop();

	bool isActive() const;

	unsigned int ms() const;
	bool singleShot() const;

	void notifyAll();
	void addNotify(SVGElementImpl *element);
	void removeNotify(SVGElementImpl *element);

private:
	unsigned int m_ms;
	bool m_invoked, m_singleShot;

	TQTimer *m_timer;
	SVGNotifyList m_notifyList;
};

typedef TQValueList<SVGTimer *> SVGTimerList;
class SVGDocumentImpl;
class SVGTimeScheduler : public TQObject
{
Q_OBJECT
  
public:
	SVGTimeScheduler(SVGDocumentImpl *doc);
	~SVGTimeScheduler();

	// Adds singleShot Timers
	void addTimer(SVGElementImpl *element, unsigned int ms);

	// (Dis-)Connects to interval timer with ms = 'staticTimerInterval'
	void connectIntervalTimer(SVGElementImpl *element);
	void disconnectIntervalTimer(SVGElementImpl *element);

	void startAnimations();
	void toggleAnimations();
	bool animationsPaused() const;

	// time elapsed in seconds after creation of this object
	float elapsed() const;

	static const unsigned int staticTimerInterval;

private slots:
	void slotTimerNotify();

private:
	SVGDocumentImpl *m_doc;
	SVGTimerList m_timerList;
	SVGTimer *m_intervalTimer;
	TQTime m_creationTime;
};

}

#endif
