/***************************************************************************
                          paletteview.cpp  -  description
                             -------------------
    begin                : Sun Jul 9 2000
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

#include <tqlayout.h>

#include "kcoloreditview.h"
#include "paletteview.h"

PaletteView::PaletteView(const int defaultCellWidth, const int defaultCellHeight, const int cellSpacing,
	KColorEditView* view, TQWidget *parent, const char *name) :
	TQFrame(parent, name, TQWidget::WResizeNoErase*0) {
	setFrameStyle(StyledPanel|Sunken);
	setLineWidth(2);
	TQGridLayout* topLayout = new TQGridLayout(this, 2, 2);
	topLayout->setMargin(2);
	topLayout->setRowStretch(0, 10);
	topLayout->setRowStretch(1, 0);
	topLayout->setColStretch(0, 10);
	topLayout->setColStretch(1, 0);
	scrollBar = new TQScrollBar(this);
	hScrollBar = new TQScrollBar(0, 1, 1, 1, 0, TQt::Horizontal, this);
	scrolledArea = new PaletteViewScrolledArea(defaultCellWidth,
		defaultCellHeight, cellSpacing, scrollBar, hScrollBar, view, this);
	connect(scrollBar, TQ_SIGNAL( valueChanged(int) ),
		TQ_SLOT( slotRepaintScrolledArea() ));
	topLayout->addWidget(scrolledArea, 0, 0);
	connect(hScrollBar, TQ_SIGNAL( valueChanged(int) ),
		TQ_SLOT( slotRepaintScrolledArea() ));
	TQHBoxLayout* hScrollBarLayout = new TQHBoxLayout();
	hScrollBarLayout->addWidget(hScrollBar, 10);
	hScrollBarLayout->addWidget(new TQWidget(this), 0);
	topLayout->addLayout(hScrollBarLayout, 1, 0);
	topLayout->addWidget(scrollBar, 0, 1);
}

PaletteView::~PaletteView() {
}

void PaletteView::redraw() {
	slotRepaintScrolledArea();
}

void PaletteView::setScrollBarValue(const int value) {
	scrollBar->setValue(value);
	hScrollBar->setValue(0);
}

void PaletteView::slotViewColorNames(bool viewColorNames) {
	scrolledArea->slotViewColorNames(viewColorNames);
	setScrollBarValue(0);
	scrolledArea->redraw();
}

void PaletteView::slotCursorFollowsChosenColor(bool follows) {
	scrolledArea->slotCursorFollowsChosenColor(follows);
}

void PaletteView::slotRepaintScrolledArea() {
	scrolledArea->redraw();
}
#include "paletteview.moc"
