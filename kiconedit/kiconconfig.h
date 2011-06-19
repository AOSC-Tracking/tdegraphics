/*
    KDE Icon Editor - a small icon drawing program for the KDE.
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

#ifndef __KICONCONFIG_H__
#define __KICONCONFIG_H__

#include <tqpixmap.h>
#include <kdialogbase.h>

#include "knew.h"
#include "utils.h"
#include "properties.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

class KAccel;
class KKeyChooser;
class KColorButton;
class KURLRequester;
class TQComboBox;

class KTemplateEditDlg : public KDialogBase
{
  Q_OBJECT
  TQ_OBJECT
public:
  KTemplateEditDlg(TQWidget *);
  ~KTemplateEditDlg() {};

  TQString name();
  TQString path();
  void setName(const TQString &);
  void setPath(const TQString &);
  
public slots:
  void slotTextChanged();

protected:
  TQLineEdit *ln_name;
  KURLRequester *ln_path;
};

class KTemplateConfig : public TQWidget
{
  Q_OBJECT
  TQ_OBJECT
public:
  KTemplateConfig(TQWidget*);
  ~KTemplateConfig();

  void saveSettings();

protected slots:
  void add();
  void edit();
  void remove();
  void checkSelection(int);

protected:
  KIconListBox *templates;
  TQPushButton *btadd, *btedit, *btremove;
};

class KBackgroundConfig : public TQWidget
{
  Q_OBJECT
  TQ_OBJECT
public:
  KBackgroundConfig(TQWidget *tqparent);
  ~KBackgroundConfig();

public slots:
  void saveSettings();
  void selectColor(const TQColor & newColor);
  void selectPixmap();

signals:

protected slots:
  void slotBackgroundMode(int);

protected:
  KColorButton *btcolor;
  TQPushButton *btpix;
  TQPixmap pix;
  TQString pixpath;
  TQWidget::BackgroundMode bgmode;
  TQLabel *lb_ex;
};

class KMiscConfig : public TQWidget
{
  Q_OBJECT
  TQ_OBJECT
public:
  KMiscConfig(TQWidget *tqparent);
  ~KMiscConfig();

public slots:
  void saveSettings();
  void pasteMode(bool);
  void showRulers(bool);

protected slots:
  void slotTransparencyDisplayType(int);

signals:

protected:
  bool pastemode, showrulers;
  TQRadioButton *rbp;
  KColorButton *m_solidColorButton;
  KColorButton *m_checkerboardColor1Button;
  KColorButton *m_checkerboardColor2Button;
  TQComboBox *m_checkerboardSizeCombo;
};

class KIconConfig : public KDialogBase
{
  Q_OBJECT
  TQ_OBJECT

public:
  
  KIconConfig(TQWidget *tqparent);
  ~KIconConfig();

protected slots:
  void slotApply();
  void slotOk();
  void finis();

protected:
  KTemplateConfig *temps;
  KBackgroundConfig *backs;
  KMiscConfig *misc;
};

#endif //__KICONCONFIG_H__
