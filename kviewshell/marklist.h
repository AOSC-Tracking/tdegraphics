// -*- C++ -*-
/* This file is part of the KDE project
   Copyright (C) 2004 Wilfried Huss <Wilfried.Huss@gmx.at>

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

#ifndef MARKLIST_H
#define MARKLIST_H

#include "pageNumber.h"

#include <tqpixmap.h>
#include <tqptrvector.h>
#include <tqscrollview.h>

class TQCheckBox;
class TQLabel;
class KPopupMenu;

class DocumentPageCache;

class MarkList;
class MarkListWidget;


/****** ThumbnailWidget ******/


class ThumbnailWidget : public TQWidget
{
  Q_OBJECT
  TQ_OBJECT

public:
  ThumbnailWidget(MarkListWidget* tqparent_, const PageNumber& _pageNumber, DocumentPageCache*);

private:
  virtual void paintEvent(TQPaintEvent*);
  virtual void resizeEvent(TQResizeEvent*);

private slots:
  void setThumbnail();

private:
  PageNumber pageNumber;

  bool needsUpdating;

  DocumentPageCache* pageCache;

  MarkListWidget* tqparent;

  TQPixmap thumbnail;
};


/****** MarkListWidget ******/


class MarkListWidget : public TQWidget
{
  Q_OBJECT
  TQ_OBJECT

public:
  MarkListWidget(TQWidget* _parent, MarkList*, const PageNumber& _pageNumber, DocumentPageCache*, bool _showThumbnail = true);

  bool isChecked() const;

  bool isVisible();

public slots:
  void toggle();
  void setChecked( bool checked );

  void setSelected( bool selected );

  int setNewWidth(int width);

signals:
  /** Emitted when the Page is selected in the ThumbnailView. */
  void selected(const PageNumber&);

  /** Emitted on right click. */
  void showPopupMenu(const PageNumber& pageNumber, const TQPoint& position);

protected:
  virtual void mousePressEvent(TQMouseEvent*);

private:

  bool showThumbnail;

  ThumbnailWidget* thumbnailWidget;
  TQCheckBox* checkBox;
  TQLabel* pageLabel;
  TQColor _backgroundColor;

  const PageNumber pageNumber;

  DocumentPageCache* pageCache;

  static const int margin = 5;

  MarkList* markList;
};


/****** MarkList ******/


class MarkList: public TQScrollView
{
    Q_OBJECT
  TQ_OBJECT

public:
  MarkList(TQWidget* tqparent = 0, const char* name = 0);
  virtual ~MarkList();

  void setPageCache(DocumentPageCache*);

  TQValueList<int> selectedPages() const;

  PageNumber currentPageNumber() { return currentPage; }

  PageNumber numberOfPages() { return widgetList.count(); }

  virtual TQSize tqsizeHint() const { return TQSize(); }

public slots:
  void setNumberOfPages(int numberOfPages, bool showThumbnails = true);

  void thumbnailSelected(const PageNumber& pageNumber);
  void setCurrentPageNumber(const PageNumber& pageNumber);

  void clear();

  void slotShowThumbnails(bool);
  void tqrepaintThumbnails();

  void updateWidgetSize(const PageNumber&);

protected:
  virtual void viewportResizeEvent(TQResizeEvent*);

  virtual void mousePressEvent(TQMouseEvent*);

signals:
  void selected(const PageNumber&);

private slots:
  void showPopupMenu(const PageNumber& pageNumber, const TQPoint& position);

  void selectAll();
  void selectEven();
  void selectOdd();
  void toggleSelection();
  void removeSelection();

private:
  TQPtrVector<MarkListWidget> widgetList;

  PageNumber currentPage;

  PageNumber clickedThumbnail;

  DocumentPageCache* pageCache;

  bool showThumbnails;

  KPopupMenu* contextMenu;
};

#endif
