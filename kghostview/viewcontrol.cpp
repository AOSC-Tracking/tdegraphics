/****************************************************************************
**
** A dialog for the selection of the view of a document.
**
** Copyright (C) 1997 by Mark Donohoe.
** Based on original work by Tim Theisen.
**
** This code is freely distributable under the GNU Public License.
**
*****************************************************************************/

#include <tqcombobox.h>
#include <tqframe.h>
#include <tqgroupbox.h>
#include <tqlabel.h>
#include <tqpushbutton.h>
#include <tqlayout.h>


#include "viewcontrol.h"
#include "viewcontrol.moc"

#include <tdelocale.h>
#include <tdeapplication.h>
#include <kbuttonbox.h>
#include <kseparator.h>

ViewControl::ViewControl( TQWidget *parent, const char *name )
	: TQDialog( parent, name )
{
	setFocusPolicy(TQWidget::StrongFocus);

	TQBoxLayout *topLayout = new TQVBoxLayout( this, 10 );

	TQGroupBox* vcGroupBox;
	vcGroupBox = new TQGroupBox(  this );
	vcGroupBox->setFrameStyle( TQFrame::NoFrame );
	//vcGroupBox->setTitle( i18n("Force Changes To") );
	//vcGroupBox->setAlignment( 1 );

	topLayout->addWidget( vcGroupBox, 10 );

	TQGridLayout *grid = new TQGridLayout( vcGroupBox, 3, 2, 10 );

	grid->setRowStretch(0,0);
	grid->setRowStretch(1,10);


	grid->setColStretch(0,0);
	grid->setColStretch(1,10);


	magComboBox = new TQComboBox( FALSE, vcGroupBox );
	magComboBox->setFixedHeight( magComboBox->sizeHint().height() );


	//magComboBox->hide();

	connect ( magComboBox, TQT_SIGNAL (activated (int)),
		  this, TQT_SLOT (slotMagSelection (int)) );
	grid->addWidget( magComboBox, 0, 1 );



	mediaComboBox = new TQComboBox( FALSE, vcGroupBox );
	mediaComboBox->setFixedHeight( magComboBox->sizeHint().height() );

	connect ( mediaComboBox, TQT_SIGNAL (activated (int)),
		  this, TQT_SLOT (slotMediaSelection (int)) );

	grid->addWidget( mediaComboBox, 1, 1 );

	orientComboBox = new TQComboBox( FALSE, vcGroupBox );
	orientComboBox->insertItem(i18n("Portrait"));
	orientComboBox->insertItem(i18n("Landscape"));
	orientComboBox->insertItem(i18n("Seascape"));
	orientComboBox->insertItem(i18n("Upside Down"));
	orientComboBox->setFixedHeight( magComboBox->sizeHint().height() );

	connect ( orientComboBox, TQT_SIGNAL (activated (int)),
		  this, TQT_SLOT (slotOrientSelection (int)) );
	grid->addWidget( orientComboBox, 2, 1 );

	int labelWidth = 0;

	TQLabel* vcLabel;
	vcLabel = new TQLabel( magComboBox, i18n("&Magnification"), vcGroupBox );
	vcLabel->setAlignment( AlignRight | AlignVCenter | ShowPrefix );
	if ( vcLabel->sizeHint().width() > labelWidth )
		labelWidth = vcLabel->sizeHint().width();
	vcLabel->setMinimumWidth( labelWidth );

	vcLabel->hide();

	grid->addWidget( vcLabel, 0, 0 );


	vcLabel = new TQLabel( mediaComboBox, i18n("M&edia"), vcGroupBox );
	vcLabel->setAlignment( AlignRight | AlignVCenter | ShowPrefix );
	if ( vcLabel->sizeHint().width() > labelWidth )
		labelWidth = vcLabel->sizeHint().width();
	vcLabel->setMinimumWidth( labelWidth );

	grid->addWidget( vcLabel, 1, 0 );

	vcLabel = new TQLabel( orientComboBox, i18n("&Orientation"), vcGroupBox );
	vcLabel->setAlignment( AlignRight | AlignVCenter | ShowPrefix );
	if ( vcLabel->sizeHint().width() > labelWidth )
		labelWidth = vcLabel->sizeHint().width();
	vcLabel->setMinimumWidth( labelWidth );

	grid->addWidget( vcLabel, 2, 0 );

	vcGroupBox->setMinimumHeight( 2*orientComboBox->sizeHint().height()+20 );
	vcGroupBox->setMinimumWidth(
		40 + labelWidth + orientComboBox->sizeHint().width() );

        KSeparator* sep = new KSeparator( KSeparator::HLine, this);
	topLayout->addWidget( sep );

	// CREATE BUTTONS

	KButtonBox *bbox = new KButtonBox( this );
	bbox->addStretch( 10 );

	apply = bbox->addButton( KStdGuiItem::apply() );
	connect( apply, TQT_SIGNAL(clicked()), TQT_SLOT(slotApplyClicked()) );

	TQPushButton *closebtn = bbox->addButton( KStdGuiItem::close() );
	connect( closebtn, TQT_SIGNAL(clicked()), TQT_SLOT(reject()) );


	bbox->layout();
	topLayout->addWidget( bbox );

	topLayout->activate();

	prevmag = prevmedia = prevorient = 0;
	applyEnable (false);
}

void
ViewControl::updateMag (int mag)
{
  magComboBox->setCurrentItem (mag);
  prevmag = mag;
}


void
ViewControl::applyEnable (bool enable)
{
  apply->setEnabled (enable);
}

void
ViewControl::slotApplyClicked()
{
  emit applyChanges();
  applyEnable (false);
}

void
ViewControl::slotMagSelection (int i)
{
  if (i != prevmag)
    {
      applyEnable (true);
      prevmag = i;
    }
}

void
ViewControl::slotMediaSelection (int i)
{
  if (i != prevmedia)
    {
      applyEnable (true);
      prevmedia = i;
    }
}

void
ViewControl::slotOrientSelection (int i)
{
  if (i != prevorient)
    {
      applyEnable (true);
      prevorient = i;
    }
}
