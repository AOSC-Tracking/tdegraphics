/*
    tdeiconedit - a small graphics drawing program for creating KDE icons
    Copyright (C) 1998  Thomas Tanghus (tanghus@kde.org)

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/  

#ifndef __TDEICONEDIT_H__
#define __TDEICONEDIT_H__

#include <tqwidget.h>
#include <tqpixmap.h>
#include <tqptrlist.h>

#include <tdemainwindow.h>
#include <kiconloader.h>
#include <tdemenubar.h>
#include <kstatusbar.h>
#include <tdetoolbar.h>
#include <tdeaccel.h>
#include <kurl.h>
#include <tdeaction.h>

#include "knew.h"
#include "tdeicon.h"
#include "tdeiconconfig.h"
#include "tdeicongrid.h"
#include "kresize.h"
#include "properties.h"

class TDEIconEdit;
class KCommandHistory;
typedef TQPtrList<TDEIconEdit> WindowList;

class TQWhatsThis;
class TQToolButton;
class TQLabel;
class PaletteToolBar;

/**
* TDEIconEdit
* @short TDEIconEdit
* @author Thomas Tanghus <tanghus@kde.org>
* @version 0.4
*/
class TDEIconEdit : public TDEMainWindow
{
    Q_OBJECT
  
public:
  TDEIconEdit( KURL url = KURL(), const char *name = "tdeiconedit");
  TDEIconEdit( const TQImage image, const char *name = "tdeiconedit");
  ~TDEIconEdit();

  virtual TQSize sizeHint() const;
  static WindowList windowList;

signals:
  void newname(const TQString &);

public slots:
  virtual void saveProperties(TDEConfig*);
  virtual void readProperties(TDEConfig*);
  void updateProperties();

protected slots:
  void slotNewWin(const TQString & url = 0);
  void slotNew();
  void slotOpen();
  void slotClose();
  void slotSave();
  void slotSaveAs();
  void slotPrint();
  void slotZoomIn();
  void slotZoomOut();
  void slotZoom1();
  void slotZoom2();
  void slotZoom5();
  void slotZoom10();
  void slotCopy();
  void slotCut();
  void slotPaste();
  void slotClear();
  void slotSaved();
  void slotSelectAll();
  void slotOpenRecent(const KURL&);
  void slotToolPointer();
  void slotToolFreehand();
  void slotToolRectangle();
  void slotToolFilledRectangle();
  void slotToolCircle();
  void slotToolFilledCircle();
  void slotToolEllipse();
  void slotToolFilledEllipse();
  void slotToolSpray();
  void slotToolFlood();
  void slotToolLine();
  void slotToolEraser();
  void slotToolSelectRect();
  void slotToolSelectCircle();
  void slotConfigureSettings();
  void slotConfigureKeys();
  void slotShowGrid();
  void slotUpdateZoom( int );
  void slotUpdateStatusColors(uint);
  void slotUpdateStatusColors(uint, uint*);
  void slotUpdateStatusPos(int, int);
  void slotUpdateStatusSize(int, int);
  void slotUpdateStatusMessage(const TQString &);
  void slotUpdateStatusName(const TQString &);
  void slotUpdateStatusModified(bool);
  void slotUpdateStatusScaling(int);
  void slotUpdatePaste(bool);
  void slotUpdateCopy(bool);
  void slotOpenBlank(const TQSize);
  void addRecent(const TQString &);

  virtual void dragEnterEvent(TQDragEnterEvent* event);
  virtual void dropEvent(TQDropEvent *e);

protected:
  void init();
  void setupActions();
  bool setupStatusBar();
  void writeConfig();
  void updateAccels();

  virtual bool queryClose();
  virtual TQWidget *createContainer( TQWidget*, int, const TQDomElement&, int& );

  KCommandHistory* history;
  PaletteToolBar *m_paletteToolBar;
  KStatusBar *statusbar;
  TDEIconEditGrid *grid;
  KGridView *gridview;
  TDEIconEditIcon *icon;
  TQImage img;
  TQString m_name;

  TDEAction *m_actCopy, *m_actPaste, *m_actCut, *m_actPasteNew;
  TDEAction *m_actZoomIn, *m_actZoomOut;
  TDERecentFilesAction *m_actRecent;
};

#endif //__TDEICONEDIT_H__
