
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


#ifndef __kp_tool_widget_base_h__
#define __kp_tool_widget_base_h__

#include <tqframe.h>
#include <tqpair.h>
#include <tqpixmap.h>
#include <tqrect.h>
#include <tqvaluevector.h>
#include <tqwidget.h>


class TQPainter;


// TODO: frame becomes a combobox when its tqparent kpToolToolBar becomes too small
class kpToolWidgetBase : public TQFrame
{
Q_OBJECT
  TQ_OBJECT

public:
    kpToolWidgetBase (TQWidget *tqparent, const char *name);  // must provide a name for config to work
    virtual ~kpToolWidgetBase ();

public:
    void addOption (const TQPixmap &pixmap, const TQString &toolTip = TQString());
    void startNewOptionRow ();

    // Call this at the end of your constructor.
    // If the default row & col could not be read from the config,
    // <fallBackRow> & <fallBackCol> are passed to setSelected().
    void finishConstruction (int fallBackRow, int fallBackCol);

private:
    TQValueVector <int> spreadOutElements (const TQValueVector <int> &sizes, int maxSize);

public:  // (only have to use these if you don't use finishConstruction())
    // (rereads from config file)
    TQPair <int, int> defaultSelectedRowAndCol () const;
    int defaultSelectedRow () const;
    int defaultSelectedCol () const;

    void saveSelectedAsDefault () const;

    void relayoutOptions ();

public:
    int selectedRow () const;
    int selectedCol () const;

    int selected () const;

    bool hasPreviousOption (int *row = 0, int *col = 0) const;
    bool hasNextOption (int *row = 0, int *col = 0) const;

public slots:
    // (returns whether <row> and <col> were in range)
    virtual bool setSelected (int row, int col, bool saveAsDefault);
    bool setSelected (int row, int col);

    bool selectPreviousOption ();
    bool selectNextOption ();

signals:
    void optionSelected (int row, int col);

protected:
    virtual void mousePressEvent (TQMouseEvent *e);
    virtual void drawContents (TQPainter *painter);

    void setInvertSelectedPixmap (bool yes = true) { m_invertSelectedPixmap = yes; }
    bool m_invertSelectedPixmap;

    // coulbe be a TQFrame or a ComboBox
    TQWidget *m_baseWidget;

    TQValueVector < TQValueVector <TQPixmap> > m_pixmaps;
    TQValueVector < TQValueVector <TQString> > m_toolTips;

    TQValueVector < TQValueVector <TQRect> > m_pixmapRects;

    int m_selectedRow, m_selectedCol;
};

#endif  // __kp_tool_widget_base_h__
