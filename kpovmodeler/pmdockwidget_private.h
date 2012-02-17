/* This file is part of the KDE libraries
   Copyright (C) 2000 Max Judin <novaprint@mtu-net.ru>
   Modified 2002 Andreas Zehender <zehender@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

/*
   IMPORTANT Note: This file compiles also in TQt-only mode by using the NO_KDE2 precompiler definition
*/

#ifndef KDOCKWIDGET_PRIVATE_H
#define KDOCKWIDGET_PRIVATE_H

#include <tqwidget.h>
#include <tqpushbutton.h>

#ifndef NO_KDE2
#include <netwm_def.h>
#endif

class TQFrame;

/**
 * Like TQSplitter but specially designed for dockwidgets stuff.
 * @internal
 *
 * @author Max Judin.
*/
class PMDockSplitter : public TQWidget
{
  Q_OBJECT
  
public:
  PMDockSplitter(TQWidget *parent= 0, const char *name= 0, Qt::Orientation orient=Qt::Vertical, int pos= 50, bool highResolution=false);  
  virtual ~PMDockSplitter(){};

  void activate(TQWidget *c0, TQWidget *c1 = 0L);
  void deactivate();

  int separatorPos() const;
  void setSeparatorPos(int pos, bool do_resize = true);

  virtual bool eventFilter(TQObject *, TQEvent *);
  virtual bool event( TQEvent * );

  TQWidget* getFirst() const { return child0; }
  TQWidget* getLast() const { return child1; }
  TQWidget* getAnother( TQWidget* ) const;
  void updateName();

  void setOpaqueResize(bool b=true);
  bool opaqueResize() const;

  void setKeepSize(bool b=true);
  bool keepSize() const;

  void setHighResolution(bool b=true);
  bool highResolution() const;

   // MODIFICATION (Zehender)
   Qt::Orientation splitterOrientation( ) const { return orientation; }

protected:
  int checkValue( int ) const;
  virtual void resizeEvent(TQResizeEvent *);

private:
  void setupMinMaxSize();

  TQWidget *child0, *child1;
  Qt::Orientation orientation;
  bool initialised;
  TQFrame* divider;
  int xpos;
  bool mOpaqueResize, mKeepSize, mHighResolution;
};

/**
 * A mini-button usually placed in the dockpanel.
 * @internal
 *
 * @author Max Judin.
*/
class PMDockButton_Private : public TQPushButton
{
  Q_OBJECT
  
public:
  PMDockButton_Private( TQWidget *parent=0, const char *name=0 );
  ~PMDockButton_Private();

protected:
  virtual void drawButton( TQPainter * );
  virtual void enterEvent( TQEvent * );
  virtual void leaveEvent( TQEvent * );

private:
  bool moveMouse;
};

/**
 * additional PMDockWidget stuff (private)
*/
class PMDockWidgetPrivate : public TQObject
{
  Q_OBJECT
  
public:
  PMDockWidgetPrivate();
  ~PMDockWidgetPrivate();

public slots:
  /**
   * Especially used for Tab page docking. Switching the pages requires additional setFocus() for the embedded widget.
   */
  void slotFocusEmbeddedWidget(TQWidget* w = 0L);

public:
  int index;
  int splitPosInPercent;
  bool pendingFocusInEvent;
  bool blockHasUndockedSignal;

#ifndef NO_KDE2
  NET::WindowType windowType;
#endif

  TQWidget *_parent;
  bool transient;
};

#endif
