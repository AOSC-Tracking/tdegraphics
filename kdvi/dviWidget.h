// -*- C++ -*-
//
// Class: DVIWidget
//
// Widget for displaying TeX DVI files.
// Part of KDVI- A previewer for TeX DVI files.
//
// (C) 2004 Wilfried Huss. Distributed under the GPL.

#ifndef _DVIWIDGET_H_
#define _DVIWIDGET_H_

#include "documentWidget.h"

class PageView;
class DocumentPageCache;
class TQPaintEvent;
class TQMouseEvent;
class TQWidget;
class textSelection;


class DVIWidget : public DocumentWidget
{
  Q_OBJECT
  TQ_OBJECT

public: 
  DVIWidget(TQWidget* tqparent, PageView* sv, DocumentPageCache* cache, const char* name);

signals:
  void SRCLink(const TQString&, TQMouseEvent* e, DocumentWidget*);

private:
  virtual void mousePressEvent(TQMouseEvent* e);
  virtual void mouseMoveEvent(TQMouseEvent* e);
};

#endif
