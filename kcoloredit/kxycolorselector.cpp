/***************************************************************************
                          kxycolorselector.cpp  -  description
                             -------------------
    begin                : Fri Jul 7 2000
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

#include <tqpainter.h>
#include <tqpixmap.h>
#include <tqimage.h>
#include <kimageeffect.h>
#include <kpalette.h>

#include "kxycolorselector.h"

KXYColorSelector::KXYColorSelector(TQWidget *tqparent, const char *name) : KXYSelector(tqparent,name) {
	setBackgroundMode(NoBackground);
	setRange(0, 0, COMPONENT_SELECTION_RESOLUTION - 1, COMPONENT_SELECTION_RESOLUTION - 1);
	setType(TYPE_NONE);
	setGlobalComponent(0);
	setMinimumSize(RGB_MAX_COMPONENT_VALUE/2 + 4, RGB_MAX_COMPONENT_VALUE/2 + 4);
	setMaximumSize(512, 512);
	updateContents();
}
KXYColorSelector::~KXYColorSelector()	{

}

void KXYColorSelector::setType(const int type) {
	this->type = type;
}

void KXYColorSelector::updateContents() {
	drawPalette(&pixmap);
	tqrepaint();
}

void KXYColorSelector::resizeEvent(TQResizeEvent*) {
	setValues(xValue(), yValue());
	updateContents();
}

void KXYColorSelector::drawContents(TQPainter* painter) {
	painter->drawPixmap(contentsRect().x(), contentsRect().y(), pixmap);
}

void KXYColorSelector::drawCursor(TQPainter* painter, int x, int y) {
	TQColor color;
	int colorX = x - contentsRect().x();
	int colorY = y - contentsRect().y();
	if(colorX < 0)
		colorX = 0;
	if(colorY < 0)
		colorY = 0;
	if(colorX > contentsRect().width() - 1)
		colorX = contentsRect().width() - 1;
	if(colorY > contentsRect().height() - 1)
		colorY = contentsRect().height() - 1;
	setColor(&color, colorX, colorY);
	TQColor cursorColor;
	if(( 2*color.red() + 4*color.green() + 1*color.blue() )*1.0/
		( 2*255 + 4*255 + 1*255 ) > 0.65)
		cursorColor = TQt::black;
	else
		cursorColor = TQt::white;
	painter->setPen(TQPen( cursorColor ));
	const int lineBegin = 2;
	const int lineEnd = 6;
	painter->drawLine(x + lineBegin, y - lineBegin, x + lineEnd, y - lineEnd);
	painter->drawLine(x + lineBegin, y + lineBegin, x + lineEnd, y + lineEnd);
	painter->drawLine(x - lineBegin, y + lineBegin, x - lineEnd, y + lineEnd);
	painter->drawLine(x - lineBegin, y - lineBegin, x - lineEnd, y - lineEnd);
}

void KXYColorSelector::setGlobalComponent(const int component) {
	m_globalComponent = component;
}

int KXYColorSelector::globalComponent() const{
	return m_globalComponent;
}

void KXYColorSelector::setColor(TQColor* const color, const int x, const int y) {
	int xSize = contentsRect().width();
	int ySize = contentsRect().height();
	switch(type) {
		case TYPE_HS:
			color->setHsv(360*x/xSize, 256*( ySize - 1 - y )/ySize,
                    globalComponent());
			break;

		case TYPE_VS:
			color->setHsv(globalComponent(), 256*( ySize - 1 - y )/ySize,
                    256*x/xSize);
			break;

		case TYPE_HV:
			color->setHsv(360*x/xSize, globalComponent(),
                    256*( ySize - 1 - y )/ySize);
			break;

		case TYPE_RG:
			color->setRgb(x/xSize, 256*( ySize - 1 - y )/ySize,
                    globalComponent());
			break;

		case TYPE_GB:
			color->setRgb(globalComponent(), 256*x/xSize,
                    256*( ySize - 1 - y )/ySize);
			break;

		case TYPE_BR:
			color->setRgb(256*( ySize - 1 - y )/ySize, globalComponent(),
                    256*x/xSize);
			break;

		case TYPE_NONE:
			color->setRgb(192, 192, 192);
			break;

	}
}

TQColor* KXYColorSelector::standardColorsPalette() {
	TQColor* palette = new TQColor[STANDARD_PALETTE_SIZE];
	int i = 0;
	palette[i++] = TQt::red;
	palette[i++] = TQt::green;
	palette[i++] = TQt::blue;
	palette[i++] = TQt::cyan;
	palette[i++] = TQt::magenta;
	palette[i++] = TQt::yellow;
	palette[i++] = TQt::darkRed;
	palette[i++] = TQt::darkGreen;
	palette[i++] = TQt::darkBlue;
	palette[i++] = TQt::darkCyan;
	palette[i++] = TQt::darkMagenta;
	palette[i++] = TQt::darkYellow;
	palette[i++] = TQt::white;
	palette[i++] = TQt::lightGray;
	palette[i++] = TQt::gray;
	palette[i++] = TQt::darkGray;
	palette[i++] = TQt::black;
	return palette;
}

void KXYColorSelector::drawPalette(TQPixmap* pixmap) {
	int xSize = contentsRect().width();
	int ySize = contentsRect().height();
	TQImage image(xSize, ySize, 32);
	TQColor color;
	int x;
	int y;

	if(type != TYPE_NONE) {
		for (y = 0; y < ySize; ++y)
		{
			unsigned int* p = (unsigned int*)image.scanLine(y);
			for(x = 0; x < xSize; ++x)
			{
				setColor(&color, x, y);
				*p = color.rgb();
				++p;
			}
		}
		if (TQColor::numBitPlanes() <= 8)
		{
	    TQColor* standardPalette = standardColorsPalette();
			KImageEffect::dither(image, standardPalette, STANDARD_PALETTE_SIZE);
			delete[] standardPalette;
		}
	}
	pixmap->convertFromImage(image);
}

#include "kxycolorselector.moc"
