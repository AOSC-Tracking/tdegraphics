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

class KIconListBox;
class KIconTemplateContainer;
class TQButtonGroup;
class TQRadioButton;
class TQGroupBox;

struct KIconTemplate
{
  TQString path;
  TQString title;
};

class KIconListBoxItem : public TQListBoxItem    
{
public:
  KIconListBoxItem( KIconTemplate ); 


  const TQString path() { return icontemplate.path; }
  KIconTemplate& iconTemplate() { return icontemplate; }
  void reloadIcon();

protected:
  virtual void paint( TQPainter * );      
  virtual int height( const TQListBox * ) const;      
  virtual int width( const TQListBox * ) const;      
private:
  TQPixmap pm;
  KIconTemplate icontemplate;
};

class KIconListBox : public TQListBox    
{
  Q_OBJECT
  
public:
  KIconListBox( TQWidget *parent ) : TQListBox(parent) {} ;
  const TQString path(int idx) { return ((KIconListBoxItem*)item(idx))->path(); }
  KIconTemplate& iconTemplate(int idx) { return ((KIconListBoxItem*)item(idx))->iconTemplate(); }

};

class KIconTemplateContainer : public TQValueList<KIconTemplate>
{
public:
   static KIconTemplateContainer* self()
   {
      if (!instance)
         instance = new KIconTemplateContainer;
      return instance;
   }

   void save();

private:
   static KIconTemplateContainer* instance;

  const KIconTemplateContainer operator = (const KIconTemplateContainer&);
  KIconTemplateContainer(const KIconTemplateContainer&);

  KIconTemplateContainer();  
  ~KIconTemplateContainer();
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
  KIconListBox *templates;
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
