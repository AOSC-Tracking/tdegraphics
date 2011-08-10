/**
 * Copyright (C) 1997-2002 the KGhostView authors. See file AUTHORS.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef MARKLIST_H
#define MARKLIST_H

#include <tqcheckbox.h>
#include <tqtable.h>

class KGVMiniWidget;

class MarkListItem : public TQWidget
{
    Q_OBJECT
  TQ_OBJECT
public:
    MarkListItem( TQWidget *parent, const TQString &text, const TQString &tip, const TQColor &color, KGVMiniWidget*, int );

    bool isChecked() const;

public slots:
    void toggle();
    void setChecked( bool checked );
    void setPixmap( TQPixmap thumbnail );

    void setSelected( bool selected );

private:
    void resizeEvent( TQResizeEvent * );
    void paintEvent( TQPaintEvent* );
private:
    TQWidget *_thumbnailW;
    TQCheckBox *_checkBox;
    TQColor _backgroundColor;
    KGVMiniWidget* _miniWidget;
    const int _pageNum;
    bool _requested;
};

class MarkList: public TQTable
{
    Q_OBJECT
  TQ_OBJECT

public:
    MarkList( TQWidget* parent = 0, const char* name = 0, KGVMiniWidget* = 0 );

    TQValueList<int> markList() const;
    void insertItem( const TQString& text, int index = -1,
                     const TQString& tip = TQString() );

public slots:
    void select( int index );
    void markCurrent();
    void markAll();
    void markEven();
    void markOdd();
    void toggleMarks();
    void removeMarks();
    void clear();

protected:
    virtual void viewportResizeEvent ( TQResizeEvent * );

signals:
    void selected( int );

private:
    int _selected;
    KGVMiniWidget* _miniWidget;
};

#endif

// vim:sw=4:sts=4:ts=8:noet
