/*
    $Id$

    Requires the TQt widget libraries, available at no cost at
    http://www.troll.no

    Copyright (C) 1996 Bernd Johannes Wuebben
                       wuebben@math.cornell.edu

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.



*/

#include <stdio.h>

#include <tqlayout.h>
#include <klocale.h>
#include <knuminput.h>
#include <tqdir.h>
#include <tqprinter.h>
#include <tqframe.h>
#include <tqbuttongroup.h>
#include <tqcheckbox.h>
#include <tqcombobox.h>
#include <tqgroupbox.h>
#include <tqlabel.h>
#include <tqlineedit.h>
#include <tqlistbox.h>
#include <tqpushbutton.h>
#include <tqradiobutton.h>
#include <tqscrollbar.h>
#include <tqtooltip.h>

#include "kfax.h"
#include "options.h"

OptionsDialog::OptionsDialog( TQWidget *parent, const char *name)
    : KDialogBase( parent, name, true, i18n("Configure"), Ok|Cancel)
{
  TQWidget *mainWidget = new TQWidget(this);
  setMainWidget(mainWidget);

  TQVBoxLayout *mainLayout = new TQVBoxLayout(mainWidget, 0, spacingHint());

  bg = new TQGroupBox(mainWidget,"bg");
  bg->setColumnLayout( 0, Qt::Horizontal );
  mainLayout->addWidget( bg );

  TQVBoxLayout *vbl = new TQVBoxLayout(bg->tqlayout());

  TQHBoxLayout *hbl1 = new TQHBoxLayout();

  vbl->addLayout( hbl1 );

  displaylabel = new TQLabel(i18n("Display options:"), bg,"displaylabel");
  displaylabel->setFixedSize( displaylabel->tqsizeHint() );
  hbl1->addWidget( displaylabel );

  landscape = new TQCheckBox(i18n("Landscape"), bg,"Landscape");
  landscape->setFixedSize( landscape->tqsizeHint() );
  hbl1->addSpacing( 10 );
  hbl1->addWidget( landscape );

  flip = new TQCheckBox(i18n("Upside down"), bg,"upsidedown");
  flip->setFixedSize( flip->tqsizeHint() );
  hbl1->addSpacing( 10 );
  hbl1->addWidget( flip );

  invert = new TQCheckBox(i18n("Invert"), bg,"invert");
  invert->setFixedSize( invert->tqsizeHint() );
  hbl1->addSpacing( 10 );
  hbl1->addWidget( invert );

  vbl->addSpacing( 20 );

  TQHBoxLayout *hbl8 = new TQHBoxLayout();
  vbl->addLayout( hbl8 );
  hbl8->addSpacing( 10 );

  resgroup = new TQButtonGroup(bg,"resgroup");
  resgroup->setFrameStyle(TQFrame::NoFrame);
  hbl8->addWidget( resgroup );

  TQHBoxLayout *hbl2 = new TQHBoxLayout(resgroup);

  reslabel = new TQLabel(i18n("Raw fax resolution:"),resgroup,"relabel");
  hbl2->addWidget( reslabel );

  resauto = new TQRadioButton(i18n("Auto"),resgroup,"resauto");
  hbl2->addSpacing( 20 );
  hbl2->addWidget( resauto );

  fine = new TQRadioButton(i18n("Fine"),resgroup,"fine");
  hbl2->addSpacing( 30 );
  hbl2->addWidget( fine );

  normal = new TQRadioButton(i18n("Normal"),resgroup,"normal");
  hbl2->addSpacing( 30 );
  hbl2->addWidget( normal );

  vbl->addSpacing( 20 );

  TQHBoxLayout *hbl3 = new TQHBoxLayout();
  vbl->addLayout( hbl3 );

  lsblabel = new TQLabel(i18n("Raw fax data are:"), bg,"lsblabel");
  hbl3->addSpacing( 10 );
  hbl3->addWidget( lsblabel );

  lsb = new TQCheckBox(i18n("LS-Bit first"), bg,"lsbitfirst");
  hbl3->addSpacing( 10 );
  hbl3->addWidget( lsb );

  vbl->addSpacing( 15 );

  TQHBoxLayout *hbl9 = new TQHBoxLayout();
  vbl->addLayout( hbl9 );
  hbl9->addSpacing( 10 );

  rawgroup = new TQButtonGroup(bg,"rawgroup");
  hbl9->addWidget( rawgroup );

  TQHBoxLayout *hbl4 = new TQHBoxLayout( rawgroup );

  rawgroup->setFrameStyle(TQFrame::NoFrame);

  rawlabel = new TQLabel(i18n("Raw fax format:"),rawgroup,"rawlabel");
  rawlabel->setFixedSize( rawlabel->tqsizeHint() );
  hbl4->addWidget( rawlabel );

  g3 = new TQRadioButton("G3",rawgroup,"g3");
  connect(g3,TQT_SIGNAL(clicked()), this, TQT_SLOT(g3toggled()));
  hbl4->addSpacing( 20 );
  hbl4->addWidget( g3 );

  g32d = new TQRadioButton("G32d",rawgroup,"g32d");
  connect(g32d,TQT_SIGNAL(clicked()), this,TQT_SLOT(g32toggled()));
  hbl4->addSpacing( 30 );
  hbl4->addWidget( g32d );

  g4 = new TQRadioButton("G4",rawgroup,"g4");
  connect(g4,TQT_SIGNAL(clicked()), this, TQT_SLOT(g4toggled()));
  hbl4->addSpacing( 30 );
  hbl4->addWidget( g4 );

  vbl->addSpacing( 20 );

  TQHBoxLayout *hbl5 = new TQHBoxLayout();
  vbl->addLayout( hbl5 );

  widthlabel = new TQLabel(i18n("Raw fax width:"),bg,"widthlabel");
  hbl5->addSpacing( 10 );
  hbl5->addWidget( widthlabel );

  widthedit = new KIntNumInput(1, bg);
  widthedit->setRange(1, 10000, 1, false);
  hbl5->addWidget( widthedit );

  heightlabel = new TQLabel(i18n("Height:"),bg,"heightlabel");
  hbl5->addSpacing( 10 );
  hbl5->addWidget( heightlabel );

  heightedit = new KIntNumInput(1, bg);
  heightedit->setRange(0, 100000, 1, false);
  hbl5->addWidget( heightedit );

  geomauto = new TQCheckBox(i18n("Auto"),bg,"geomauto");
  connect(geomauto,TQT_SIGNAL(clicked()),this,TQT_SLOT(geomtoggled()));
  hbl5->addSpacing( 10 );
  hbl5->addWidget( geomauto );
}


struct optionsinfo *  OptionsDialog::getInfo(){

  if(resauto->isChecked())
    oi.resauto = 1;

  if(fine->isChecked())
    oi.fine = 1;
  else
    oi.fine = 0;

  if(landscape->isChecked())
    oi.landscape = 1;
  else
    oi.landscape = 0;

  if(flip->isChecked())
    oi.flip = 1;
  else
    oi.flip = 0;

  if(invert->isChecked())
    oi.invert = 1;
  else
    oi.invert = 0;

  if(lsb->isChecked())
    oi.lsbfirst = 1;
  else
    oi.lsbfirst = 0;

  if(geomauto->isChecked())
    oi.geomauto = 1;


  if(g3->isChecked()){
    oi.raw = 3;
  }

  if(g32d->isChecked()){
    oi.raw = 2;
    oi.geomauto = 0;

  }

  if(g4->isChecked()){
    oi.raw = 4;
    oi.geomauto = 0;
  }

  oi.height = heightedit->value();
  oi.width = widthedit->value();



  return &oi;

}

void OptionsDialog::setWidgets(struct optionsinfo* newoi ){

  if(!newoi)
    return;

  if(newoi->resauto == 1){
    resauto->setChecked(newoi->resauto);
    fine->setChecked(!newoi->resauto);
    normal->setChecked(!newoi->resauto);
  }
  else{
    if(newoi->fine == 1){
      resauto->setChecked(FALSE);
      fine->setChecked(TRUE);
      normal->setChecked(FALSE);
    }
    else{
      resauto->setChecked(FALSE);
      fine->setChecked(FALSE);
      normal->setChecked(TRUE);
    }
  }

  if(newoi->landscape == 1)
    landscape->setChecked(TRUE);
  else
    landscape->setChecked(FALSE);

  if(newoi->flip == 1)
    flip->setChecked(TRUE);
  else
    flip->setChecked(FALSE);

  if(newoi->invert == 1)
    invert->setChecked(TRUE);
  else
    invert->setChecked(FALSE);

  if(newoi->lsbfirst == 1)
    lsb->setChecked(TRUE);
  else
    lsb->setChecked(FALSE);

  if(newoi->raw == 3){
    geomauto->setEnabled(TRUE);
    g3->setChecked(TRUE);
  }

  if(newoi->raw == 2){
    geomauto->setEnabled(FALSE);
    g32d->setChecked(TRUE);
  }

  if(newoi->raw == 4){
    geomauto->setEnabled(FALSE);
    g4->setChecked(TRUE);
  }
  widthedit->setValue(newoi->width);
  heightedit->setValue(newoi->height);

  // auto height and width can only work with g3 faxes
  if(newoi->geomauto == 1 && newoi->raw != 4 && newoi->raw != 2){
    geomauto->setChecked(TRUE);
    widthedit->setEnabled(FALSE);
    heightedit->setEnabled(FALSE);
  }
  else{
    geomauto->setChecked(FALSE);
    widthedit->setEnabled(TRUE);
    heightedit->setEnabled(TRUE);

  }

}


void OptionsDialog::g32toggled(){

  geomauto->setChecked(FALSE);
  geomauto->setEnabled(FALSE);
  widthedit->setEnabled(TRUE);
  heightedit->setEnabled(TRUE);

}

void OptionsDialog::g4toggled(){

  geomauto->setChecked(FALSE);
  geomauto->setEnabled(FALSE);
  widthedit->setEnabled(TRUE);
  heightedit->setEnabled(TRUE);


}


void OptionsDialog::g3toggled(){

  geomauto->setEnabled(TRUE);
  geomauto->setChecked(TRUE);
  widthedit->setEnabled(FALSE);
  heightedit->setEnabled(FALSE);


}

void OptionsDialog::geomtoggled(){

  if(geomauto->isChecked()){

    widthedit->setEnabled(FALSE);
    heightedit->setEnabled(FALSE);

  }
  else{

    widthedit->setEnabled(TRUE);
    heightedit->setEnabled(TRUE);

  }

}

void OptionsDialog::slotHelp(){
  kapp->invokeHelp();
}


#include "options.moc"
