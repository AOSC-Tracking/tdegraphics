/*
    KDE Draw - a small graphics drawing program for the KDE
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

#include <tqlayout.h>
#include <tqpainter.h>
#include <tqbuttongroup.h>
#include <tqradiobutton.h>
#include <tqpushbutton.h>

#include <kconfig.h>
#include <klocale.h>
#include <kglobal.h>
#include <kiconloader.h>
#include <kdebug.h>
#include <kapplication.h>

#include "knew.h"

KIconTemplateContainer* KIconTemplateContainer::instance = 0;

void createStandardTemplates(KIconTemplateContainer *list)
{
    KIconLoader *kil = KGlobal::iconLoader();

    KIconTemplate it;
    it.path = kil->iconPath("standard", KIcon::User);
    it.title = i18n("Standard File");
    list->append(it);
    
    it.path = kil->iconPath("source", KIcon::User);
    it.title = i18n("Source File");
    list->append(it);

    it.path = kil->iconPath("compressed", KIcon::User);
    it.title = i18n("Compressed File");
    list->append(it);

    it.path = kil->iconPath("folder", KIcon::User);
    it.title = i18n("Standard Folder");
    list->append(it);

    it.path = kil->iconPath("package", KIcon::User);
    it.title = i18n("Standard Package");
    list->append(it);

    it.path = kil->iconPath("mini-folder", KIcon::User);
    it.title = i18n("Mini Folder");
    list->append(it);

    it.path = kil->iconPath("mini-package", KIcon::User);
    it.title = i18n("Mini Package");
    list->append(it);
}

void KIconTemplateContainer::save()
{
  KConfig *k = kapp->config();
  k->setGroup("Templates");
  
  TQStringList names;
  for (TQValueListIterator<KIconTemplate> iter = begin(); iter != end(); iter++)
  {
      names.append((*iter).title);
  }
  
  k->writeEntry("Names",  names);
  
  for(unsigned int i = 0; i < names.count(); i++)
  {    
    k->writePathEntry(names[i], (*this)[i].path);
  }
}

KIconTemplateContainer::KIconTemplateContainer() : TQValueList<KIconTemplate>()
{  
  TQStrList names;
  KConfig *k = kapp->config();
  k->setGroup("Templates");
  k->readListEntry("Names", names);
  for(int i = 0; i < (int)names.count(); i++)
  {
    KIconTemplate it;
    it.path = k->readPathEntry(names.at(i));
    it.title = names.at(i);
    //kdDebug(4640) << "Template: " << names.at(i) << "\n" << path.data() << endl;
    append(it);
  }

  if(count() == 0)
  {
    createStandardTemplates(this);
  }
}

KIconTemplateContainer::~KIconTemplateContainer()
{
  clear();
}

KIconListBoxItem::KIconListBoxItem( KIconTemplate t )
   : TQListBoxItem(), icontemplate(t)
{
  //kdDebug(4640) << "KIconListBoxItem - " << t->path.data() << ", " << t->title.data() << endl;
 
  pm.load(t.path);
  setText( t.title );
}

void KIconListBoxItem::reloadIcon()
{
  pm.load(icontemplate.path);
  setText( icontemplate.title );
}
 


void KIconListBoxItem::paint( TQPainter *p )
{
  p->drawPixmap( 3, 0, pm );
  TQFontMetrics fm = p->fontMetrics();
  int yPos;                       // vertical text position
  if ( pm.height() < fm.height() )
    yPos = fm.ascent() + fm.leading()/2;
  else
    yPos = pm.height()/2 - fm.height()/2 + fm.ascent();
  p->drawText( pm.width() + 5, yPos, text() );
}

int KIconListBoxItem::height(const TQListBox *lb ) const
{
  return QMAX( pm.height(), lb->fontMetrics().lineSpacing() + 1 );
}
    
int KIconListBoxItem::width(const TQListBox *lb ) const
{
  return pm.width() + lb->fontMetrics().width( text() ) + 6;
}

NewSelect::NewSelect(TQWidget *parent) : TQWidget( parent )
{
  wiz = (KWizard*) parent;
  grp = new TQButtonGroup( this );
  connect( grp, TQT_SIGNAL( clicked( int ) ), TQT_SLOT( buttonClicked( int ) ) );
  grp->setExclusive( true );
  
  TQVBoxLayout* ml = new TQVBoxLayout( this );
  ml->addWidget( grp, 1 );
  //ml->addWidget(grp, 10, AlignLeft);
  TQVBoxLayout* l = new TQVBoxLayout( grp, 10 );
  
  rbscratch = new TQRadioButton( i18n( "Create from scratch" ), grp );
  l->addWidget( rbscratch, 1 );
  //l->addWidget(rbscratch, 5, AlignLeft);
  
  rbtempl = new TQRadioButton( i18n( "Create from template" ), grp );
  l->addWidget( rbtempl, 1 );
  //l->addWidget(rbtempl, 5, AlignLeft);
  
  //grp->setMinimumSize(grp->childrenRect().size());
  
  grp->setButton( 0 );
}

NewSelect::~NewSelect()
{
}

void NewSelect::buttonClicked(int id)
{
  //kdDebug(4640) << "Button: " << id << endl;

  emit iconopenstyle(id);
}

NewFromTemplate::NewFromTemplate( TQWidget* parent )
  : TQWidget( parent )
{
  wiz = (KWizard*) parent;
  
  TQVBoxLayout* ml = new TQVBoxLayout(this);
  
  grp = new TQGroupBox( i18n( "Templates" ), this );
  ml->addWidget( grp, 1 );
  //ml->addWidget(grp, 10, AlignLeft);
  
  TQHBoxLayout* l = new TQHBoxLayout( grp, 15 );
  
  templates = new KIconListBox( grp );
  connect( templates, TQT_SIGNAL( highlighted( int ) ), TQT_SLOT( checkSelection( int ) ) );
  l->addWidget( templates );
    
  for( int i = 0; i < (int) KIconTemplateContainer::self()->count(); i++ )
    templates->insertItem( new KIconListBoxItem( *KIconTemplateContainer::self()->at( i ) ) );
}

NewFromTemplate::~NewFromTemplate()
{
}

void NewFromTemplate::checkSelection( int )
{
  //kdDebug(4640) << "checkSelection(int) " << templates->currentItem()  << endl;
  if( templates->currentItem() != -1 )
    wiz->finishButton()->setEnabled( true );
  else
    wiz->finishButton()->setEnabled( false );
}

KNewIcon::KNewIcon( TQWidget* parent )
  : KWizard( parent, 0, true )
{
  //kdDebug(4640) << "KNewIcon" << endl;
  setCaption( i18n( "Create New Icon" ) );
  resize( 400, 250 );
  
  openstyle = 0;

  finishButton()->setEnabled( true );
  cancelButton()->setEnabled( true );
  nextButton()->setEnabled( false );

  select = new NewSelect( this );
  connect( select, TQT_SIGNAL( iconopenstyle( int ) ), TQT_SLOT( iconOpenStyle( int ) ) );

  scratch = new KResizeWidget( this, 0, TQSize( 32, 32 ) );
  // this doesn't accept default valid size, besides spin buttons won't allow 
  // an invalid size to be set by the user - forces user to change valid default 
  // size to create the new icon object -
  connect( scratch, TQT_SIGNAL( validSize( bool ) ), TQT_SLOT( checkPage( bool ) ) );
  connect(this, TQT_SIGNAL(selected(const TQString &)), this, TQT_SLOT(checkPage(const TQString &)));
  templ = new NewFromTemplate(this);
  templ->hide();

  addPage(select, i18n("Select Icon Type"));
  addPage(scratch, i18n("Create From Scratch"));
  act = scratch;
  //addPage(templ, i18n("Create From Template"));
}

KNewIcon::~KNewIcon()
{
  delete select;
  delete scratch;
  delete templ;
}

void KNewIcon::okClicked()
{
  if(openstyle == Blank)
    emit newicon(scratch->getSize());
  else
    emit newicon(templ->path());
  hide();
  setResult(1);
  accept();
}

void KNewIcon::cancelClicked()
{
  hide();
  setResult(0);
  reject();
}

void KNewIcon::iconOpenStyle(int style)
{
  openstyle = style;

  if( act )
    removePage( act );

  if(openstyle == Blank)
  {
    act = scratch;
    setNextEnabled( act, true );
    addPage( scratch, i18n( "Create From Scratch" ) );
  }
  else if( openstyle == Template )
  {
    act = templ;
    setNextEnabled( act, true );
    addPage( templ, i18n( "Create From Template" ) );
  }
}

void KNewIcon::checkPage( bool b)
{
    //kdDebug(4640) << "checkPage(int) " << openstyle << " " << p << endl;
    if(openstyle == Blank)
        finishButton()->setEnabled(true);
    else if( !b )
        finishButton()->setEnabled(false);
    else
        templ->checkSelection(0);
}

void KNewIcon::checkPage(const TQString &)
{
  if(currentPage() == select || openstyle == Blank)
    finishButton()->setEnabled(true);
  else
    finishButton()->setEnabled(false);
}
#include "knew.moc"





