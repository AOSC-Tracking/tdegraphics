/***************************************************************************
                          klineal.h  -  description
                             -------------------
    begin                : Fri Oct 13 2000
    copyright            : (C) 2000 by Till Krech
    email                : till@snafu.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KLINEAL_H
#define KLINEAL_H

#include <kapplication.h>
#include <kpopupmenu.h>
#include <kmainwindow.h>

#include <kcolordialog.h>
#include <kfontdialog.h>

#include <tqlabel.h>
#include <tqpainter.h>
#include <tqwidget.h>
#include <tqcursor.h>

class KLineal : public KMainWindow {
  Q_OBJECT
  
public:
  enum { North=0, West=1, South=2, East=3 };
  /** constructor */
  KLineal(TQWidget*parent=0,const char* name=0);
  /** destructor */
  ~KLineal();
  void move(int x, int y);
  void move(const TQPoint &p);
  TQPoint pos();
  int x();
  int y();

protected:
  void keyPressEvent(TQKeyEvent *e);
  void mousePressEvent(TQMouseEvent *e);
  void mouseReleaseEvent(TQMouseEvent *e);
  void mouseMoveEvent(TQMouseEvent *e);
  void paintEvent(TQPaintEvent *e);
  void enterEvent(TQEvent *e);
  void leaveEvent(TQEvent *e);
  void setupBackground();

  // session management
  virtual void saveProperties( TDEConfig *config );
  virtual void readProperties( TDEConfig *config );

private:
  void drawScale(TQPainter &painter);
  void reLength(int percentOfScreen);
  void setupCursor();
  bool mDragging;
  TQPoint mLastClickPos;
  TQPoint mDragOffset;
  TQLabel *mLabel;
  TQLabel *mColorLabel;
  TQFrame *mColorRect;
  int mOrientation;
  int mLongEdgeLen;
  int mShortEdgeLen;
  KPopupMenu *mMenu;
  KPopupMenu *mLenMenu;
  TQColor mColor;
  TQColor mStoredColor;
  TQCursor mCurrentCursor;
  TQCursor mNorthCursor;
  TQCursor mEastCursor;
  TQCursor mWestCursor;
  TQCursor mSouthCursor;
  TQCursor mDragCursor;
  KColorDialog mColorSelector;
  TQFont mScaleFont;
  bool _clicked;

public slots:
  void setOrientation(int);
  void setNorth();
  void setEast();
  void setSouth();
  void setWest();
  void turnLeft();
  void turnRight();
  void showMenu();
  void hideLabel();
  void showLabel();
  void adjustLabel();
  void setShortLength();
  void setMediumLength();
  void setTallLength();
  void setFullLength();
  void setColor();
  void setFont(TQFont &);
  void setColor(const TQColor &color);
  void choseColor();
  void choseFont();
  void restoreColor();
  void saveSettings();
};
#endif
