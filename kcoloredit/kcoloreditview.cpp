/***************************************************************************
                          kcoloreditview.cpp  -  description
                             -------------------
    begin                : Sat Jul  8 09:57:28 CEST 2000
    copyright            : (C) 2000 by Artur Rataj
    email                : art@zeus.polsl.gliwice.pl
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// include files for TQt
#include <tqpainter.h>
#include <tqlayout.h>
#include <tqpushbutton.h>
#include <tqvgroupbox.h>
#include <tqlineedit.h>
#include <tqcolor.h>
#include <tqprinter.h>

// include files for KDE
#include <kseparator.h>
#include <klocale.h>

// application specific includes
#include "main.h"
#include "kcoloreditview.h"
#include "kcoloreditdoc.h"
#include "kcoloredit.h"
#include "palette.h"

KColorEditView::KColorEditView(TQWidget *tqparent, const char *name) : TQSplitter(tqparent, name) {
	colorSelector = new ColorSelector(this);
	colorSelector->slotSetColor(
		Color( RGB_MAX_COMPONENT_VALUE, RGB_MAX_COMPONENT_VALUE, RGB_MAX_COMPONENT_VALUE, "" ));
	TQWidget* paletteViewArea = new TQWidget(this);
	TQVBoxLayout* paletteLayout = new TQVBoxLayout(paletteViewArea);
	paletteView = new PaletteView(16, 16, 2, this, paletteViewArea);
	paletteLayout->addWidget(paletteView, 10);
	TQHBoxLayout* tqlayout = new TQHBoxLayout();
	TQVBoxLayout* addColorLayout = new TQVBoxLayout(4);
	addColorLayout->setMargin(8);
	TQHBoxLayout* buttonsLayout = new TQHBoxLayout(4);
	TQPushButton* addColorButton = new TQPushButton(i18n( "Add Color" ), paletteViewArea);
	connect(addColorButton, TQT_SIGNAL( clicked() ), TQT_SLOT( slotAddColor() ));
	buttonsLayout->addWidget(addColorButton);
	buttonsLayout->addStretch(10);
	addColorLayout->addLayout(buttonsLayout);
	TQCheckBox* atCursorCheckBox = new TQCheckBox(i18n( "At cursor" ), paletteViewArea);
	connect(atCursorCheckBox, TQT_SIGNAL( toggled(bool) ), TQT_SLOT( slotAddColorAtCursor(bool) ));
	addColorLayout->addWidget(atCursorCheckBox);
	overwriteCheckBox = new TQCheckBox(i18n( "Overwrite" ), paletteViewArea);
	connect(overwriteCheckBox, TQT_SIGNAL( toggled(bool) ), TQT_SLOT( slotAddColorOverwrite(bool) ));
	slotAddColorAtCursor(false);
	slotAddColorOverwrite(false);
	addColorLayout->addWidget(overwriteCheckBox);
	addColorLayout->addStretch(10);
	//KSeparator* hLine = new KSeparator(KSeparator::HLine, paletteViewArea);
	//addColorLayout->addWidget(hLine);
	//addColorLayout->addStretch(10);
	//TQCheckBox* cursorFollowsChosenColor = new TQCheckBox(i18n( "Cursor follows" ), paletteViewArea);
	//addColorLayout->addWidget(cursorFollowsChosenColor);
	//connect(cursorFollowsChosenColor, TQT_SIGNAL( toggled(bool) ),
	//	paletteView, TQT_SLOT( slotCursorFollowsChosenColor(bool) ));
	//cursorFollowsChosenColor->toggle();
	paletteView->slotCursorFollowsChosenColor(true);
	tqlayout->addLayout(addColorLayout, 0);
	TQVGroupBox* colorAtCursorFrame = new TQVGroupBox(i18n("Color at Cursor"), paletteViewArea);
	TQWidget* colorAtCursorFrameArea = new TQWidget(colorAtCursorFrame);
	TQVBoxLayout* colorAtCursorLayout = new TQVBoxLayout(colorAtCursorFrameArea, 4);
	TQHBoxLayout* colorNameLayout = new TQHBoxLayout(0);
	TQLabel* nameLabel = new TQLabel(i18n( "Name" ) + ": ", colorAtCursorFrameArea);
	colorNameLayout->addWidget(nameLabel, 0);
	colorName = new TQLineEdit(colorAtCursorFrameArea);
	connect(colorName, TQT_SIGNAL( textChanged(const TQString&) ), TQT_SLOT( slotSetColorName(const TQString&) ));
	colorNameLayout->addWidget(colorName, 10);
	colorAtCursorLayout->addLayout(colorNameLayout);
	TQGridLayout* colorAtCursorComponentsLayout = new TQGridLayout(3, 6, 4);
	colorAtCursorLayout->addLayout(colorAtCursorComponentsLayout);
	colorAtCursorComponentsLayout->setColStretch(1, 10);
	colorAtCursorComponentsLayout->addColSpacing(2, 8);
	colorAtCursorComponentsLayout->setColStretch(4, 10);
	colorAtCursorComponentsLayout->setColStretch(5, 10);
	TQLabel* hLabel = new TQLabel("H: ", colorAtCursorFrameArea);
	colorAtCursorComponentsLayout->addWidget(hLabel, 0, 0);
	colorAtCursorHValueLabel = new TQLabel("", colorAtCursorFrameArea);
	setColorAtCursorComponentValueLabelSizes(colorAtCursorHValueLabel);
	colorAtCursorComponentsLayout->addWidget(colorAtCursorHValueLabel, 0, 1);
	TQLabel* sLabel = new TQLabel("S: ", colorAtCursorFrameArea);
	colorAtCursorComponentsLayout->addWidget(sLabel, 1, 0);
	colorAtCursorSValueLabel = new TQLabel("", colorAtCursorFrameArea);
	setColorAtCursorComponentValueLabelSizes(colorAtCursorSValueLabel);
	colorAtCursorComponentsLayout->addWidget(colorAtCursorSValueLabel, 1, 1);
	TQLabel* vLabel = new TQLabel("V: ", colorAtCursorFrameArea);
	colorAtCursorComponentsLayout->addWidget(vLabel, 2, 0);
	colorAtCursorVValueLabel = new TQLabel("", colorAtCursorFrameArea);
	setColorAtCursorComponentValueLabelSizes(colorAtCursorVValueLabel);
	colorAtCursorComponentsLayout->addWidget(colorAtCursorVValueLabel, 2, 1);
	TQLabel* rLabel = new TQLabel("R: ", colorAtCursorFrameArea);
	colorAtCursorComponentsLayout->addWidget(rLabel, 0, 3);
	colorAtCursorRValueLabel = new TQLabel("", colorAtCursorFrameArea);
	setColorAtCursorComponentValueLabelSizes(colorAtCursorRValueLabel);
	colorAtCursorComponentsLayout->addWidget(colorAtCursorRValueLabel, 0, 4);
	TQLabel* gLabel = new TQLabel("G: ", colorAtCursorFrameArea);
	colorAtCursorComponentsLayout->addWidget(gLabel, 1, 3);
	colorAtCursorGValueLabel = new TQLabel("", colorAtCursorFrameArea);
	setColorAtCursorComponentValueLabelSizes(colorAtCursorGValueLabel);
	colorAtCursorComponentsLayout->addWidget(colorAtCursorGValueLabel, 1, 4);
	TQLabel* bLabel = new TQLabel("B: ", colorAtCursorFrameArea);
	colorAtCursorComponentsLayout->addWidget(bLabel, 2, 3);
	colorAtCursorBValueLabel = new TQLabel("", colorAtCursorFrameArea);
	setColorAtCursorComponentValueLabelSizes(colorAtCursorBValueLabel);
	colorAtCursorComponentsLayout->addWidget(colorAtCursorBValueLabel, 2, 4);
	TQHBoxLayout* colorAtCursorRgbStringLayout = new TQHBoxLayout();
	TQLabel* colorAtCursorRgbStringLabel =
		new TQLabel("RGB " + i18n( "hex." ) + ": ", colorAtCursorFrameArea);
	colorAtCursorRgbStringLayout->addWidget(colorAtCursorRgbStringLabel);
	colorAtCursorRgbStringValueLabel = new TQLabel("", colorAtCursorFrameArea);
	colorAtCursorRgbStringValueLabel->setFixedWidth(
		colorAtCursorRgbStringValueLabel->fontMetrics().width( TQString("8888888") ));
	colorAtCursorRgbStringLayout->addWidget(colorAtCursorRgbStringValueLabel);
	colorAtCursorRgbStringLayout->addStretch();
	colorAtCursorLayout->addLayout(colorAtCursorRgbStringLayout);
	tqlayout->addWidget(colorAtCursorFrame, 10);
	tqlayout->addSpacing(8);
	paletteLayout->addSpacing(4);
	paletteLayout->addLayout(tqlayout);
	paletteLayout->addSpacing(4);
	inColorNameChanging = false;
	doNotUpdateColorLabels = false;
}

KColorEditView::~KColorEditView() {
}

void KColorEditView::setColorAtCursorComponentValueLabelSizes(TQLabel* const label) {
	label->setMinimumWidth(label->fontMetrics().width( TQString("888") ));
	label->setMaximumWidth(label->fontMetrics().width( TQString("88888") ));
}

KColorEditDoc *KColorEditView::document() const {
  KColorEditApp *theApp=(KColorEditApp *) parentWidget();

  return theApp->document();
}

void KColorEditView::print(TQPrinter *pPrinter) {
  TQPainter printpainter;
  printpainter.begin(pPrinter);

  // TODO: add your printing code here

  printpainter.end();
}

void KColorEditView::chooseColor(Color* const color) {
	colorSelector->slotSetColor(color);
}

void KColorEditView::slotCursorPosChanged(int position) {
	Palette* palette = document()->paletteHistory()->editableStream();
	if(position < palette->length()) {
		Color* color = palette->color(position);
		TQString string;
		inColorNameChanging = true;
		colorName->setText(color->name());
		colorName->setEnabled(true);
		inColorNameChanging = false;
		int rComponent = color->component(Color::RED_INDEX);
		int gComponent = color->component(Color::GREEN_INDEX);
		int bComponent = color->component(Color::BLUE_INDEX);
		colorAtCursorRValueLabel->setText(string.setNum( rComponent ));
		colorAtCursorGValueLabel->setText(string.setNum( gComponent ));
		colorAtCursorBValueLabel->setText(string.setNum( bComponent ));
		TQColor hsvColor(rComponent, gComponent, bComponent);
		int hComponent;
		int sComponent;
		int vComponent;
		hsvColor.hsv(&hComponent, &sComponent, &vComponent);
		colorAtCursorHValueLabel->setText(string.setNum( hComponent ));
		colorAtCursorSValueLabel->setText(string.setNum( sComponent ));
		colorAtCursorVValueLabel->setText(string.setNum( vComponent ));
		colorAtCursorRgbStringValueLabel->setText(string.sprintf( "%02x%02x%02x",
			rComponent, gComponent, bComponent ));
	} else {
		colorName->setText("");
		colorName->setEnabled(false);
		colorAtCursorHValueLabel->setText("");
		colorAtCursorSValueLabel->setText("");
		colorAtCursorVValueLabel->setText("");
		colorAtCursorRValueLabel->setText("");
		colorAtCursorGValueLabel->setText("");
		colorAtCursorBValueLabel->setText("");
		colorAtCursorRgbStringValueLabel->setText("");
	}
}

void KColorEditView::slotViewColorNames(bool viewColorNames) {
	paletteView->slotViewColorNames(viewColorNames);
}

void KColorEditView::updateColorValueLabels() {
	if(!doNotUpdateColorLabels)
		slotCursorPosChanged(document()->paletteCursorPos());
}

void KColorEditView::redraw(bool newDocument) {
	if(newDocument)
		paletteView->setScrollBarValue(0);
	paletteView->redraw();
	updateColorValueLabels();
}

void KColorEditView::slotAddColor() {
    Color color = colorSelector->color();
    Palette* palette = document()->paletteHistory()->editableStream();
    color.setName("");
    int index;
    if(addColorAtCursor)
        index = document()->paletteCursorPos();
    else
        index = palette->length();
    switch(addColorMode)
    {
        case INSERT_COLOR:
            document()->insert(index, color);
            break;

        case REPLACE_COLOR:
            if(index < palette->length())
                document()->replace(index, color);
            else
                document()->insert(index, color);
            break;

    }
}

void KColorEditView::slotAddColorAtCursor(bool atCursor) {
	addColorAtCursor = atCursor;
	overwriteCheckBox->setEnabled(addColorAtCursor);
}

void KColorEditView::slotAddColorOverwrite(bool overwrite) {
	if(overwrite)
		addColorMode = REPLACE_COLOR;
	else
		addColorMode = INSERT_COLOR;
}

void KColorEditView::slotSetColorName(const TQString& name) {
	if(!inColorNameChanging) {
		/*
		Palette* palette = getDocument()->getPaletteHistory()->getEditableStream();
		int cursorPos = getDocument()->getPaletteCursorPos();
		if(cursorPos < palette->length()) {
			palette->getColor(cursorPos)->setName(name);
			getDocument()->setModified(true);
			getDocument()->slotRedrawAllViews(this);
		}
		 */
		Palette* palette = document()->paletteHistory()->editableStream();
		int cursorPos = document()->paletteCursorPos();
		if(cursorPos < palette->length()) {
			Color newColor(
				palette->color(cursorPos)->component(Color::RED_INDEX),
				palette->color(cursorPos)->component(Color::GREEN_INDEX),
				palette->color(cursorPos)->component(Color::BLUE_INDEX),
				name);
			doNotUpdateColorLabels = true;
			document()->replace(cursorPos, newColor);
			doNotUpdateColorLabels = false;
		}
	}
}
#include "kcoloreditview.moc"
