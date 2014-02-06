/*
    KDE Draw - a small graphics drawing program for the KDE.
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

#ifndef __KNEWICON_H__
#define __KNEWICON_H__

#include <tqwidget.h>
#include <tqpixmap.h>
#include <tqimage.h>
#include <tqlistbox.h>

#include <kwizard.h>

#include "kresize.h"

class TDEIconListBox;
class TDEIconTemplateContainer;
class TQButtonGroup;
class TQRadioButton;
class TQGroupBox;

struct TDEIconTemplate
{
  TQString path;
  TQString title;
};

class TDEIconListBoxItem : public TQListBoxItem    
{
public:
  TDEIconListBoxItem( TDEIconTemplate ); 


  const TQString path() { return icontemplate.path; }
  TDEIconTemplate& iconTemplate() { return icontemplate; }
  void reloadIcon();

protected:
  virtual void paint( TQPainter * );      
  virtual int height( const TQListBox * ) const;      
  virtual int width( const TQListBox * ) const;      
private:
  TQPixmap pm;
  TDEIconTemplate icontemplate;
};

class TDEIconListBox : public TQListBox    
{
  Q_OBJECT
  
public:
  TDEIconListBox( TQWidget *parent ) : TQListBox(parent) {} ;
  const TQString path(int idx) { return ((TDEIconListBoxItem*)item(idx))->path(); }
  TDEIconTemplate& iconTemplate(int idx) { return ((TDEIconListBoxItem*)item(idx))->iconTemplate(); }

};

class TDEIconTemplateContainer : public TQValueList<TDEIconTemplate>
{
public:
   static TDEIconTemplateContainer* self()
   {
      if (!instance)
         instance = new TDEIconTemplateContainer;
      return instance;
   }

   void save();

private:
   static TDEIconTemplateContainer* instance;

  const TDEIconTemplateContainer operator = (const TDEIconTemplateContainer&);
  TDEIconTemplateContainer(const TDEIconTemplateContainer&);

  TDEIconTemplateContainer();  
  ~TDEIconTemplateContainer();
};

class NewSelect : public TQWidget
{
  Q_OBJECT
  
public:
  NewSelect(TQWidget *parent);
  ~NewSelect();

signals:
  void iconopenstyle(int);

protected slots:
  void buttonClicked(int);

protected:
  KWizard *wiz;
  TQButtonGroup *grp;
  TQRadioButton *rbscratch, *rbtempl;
};

class NewFromTemplate : public TQWidget
{
  Q_OBJECT
  
public:
  NewFromTemplate(TQWidget *parent);
  ~NewFromTemplate();

  const TQString path() { return TQString(templates->path(templates->currentItem())); }

public slots:
  void checkSelection(int);
 
protected:
  TDEIconListBox *templates;
  KWizard *wiz;
  TQGroupBox *grp;
};

class KNewIcon : public KWizard
{
  Q_OBJECT
  
public:
  KNewIcon(TQWidget *parent);
  ~KNewIcon();

  enum { Blank = 0, Template = 1};
  int openStyle() { return openstyle; }
  const TQString templatePath() { return TQString(templ->path()); }
  const TQSize templateSize() { return scratch->getSize(); }

protected slots:
  void okClicked();
  void cancelClicked();
  void iconOpenStyle(int);
  void checkPage(bool);
  void checkPage(const TQString &);

signals:
  void newicon(const TQSize);
  void newicon(const TQString);

protected:
  NewSelect       *select;
  KResizeWidget   *scratch;
  NewFromTemplate *templ;
  TQWidget         *act;
  int              openstyle;
};

#endif // __KNEWICON_H__
