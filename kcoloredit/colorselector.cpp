/***************************************************************************
                          colorselector.cpp  -  description
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

#include <math.h>

#include <tqlayout.h>
/* #include <tqtabwidget.h> */
#include <tqradiobutton.h>
#include <tqlabel.h>
#include <tqslider.h>
#include <kseparator.h>
#include <klocale.h>

#include "main.h"
#include "imageselection.h"
#include "texteditselection.h"
#include "colorselector.h"

ColorSelector::ColorSelector(TQWidget *tqparent, const char *name ) : TQWidget(tqparent, name) {
	fComponentsMode = false;
	TQVBoxLayout* topLayout = new TQVBoxLayout(this, 4);
	/*
	TQTabWidget* pages = new TQTabWidget(this);
	 */
	gradientSelection = new GradientSelection(this);
	connect(gradientSelection, TQT_SIGNAL( valueChanged(Color*) ),
		TQT_SLOT( slotGradientSelectionChangeColor(Color*) ));
	connect(gradientSelection, TQT_SIGNAL( synchronizeColor() ),
		TQT_SLOT( slotGradientSelectionSynchronizeColor() ));
	connect(this, TQT_SIGNAL( valueChanged(Color*) ), gradientSelection, TQT_SLOT( slotSetValue(Color*) ));
	/*
	pages->addTab(gradientSelection, i18n( "Gradient" ));
	ImageSelection* imageSelection = new ImageSelection(this);
	connect(imageSelection, TQT_SIGNAL( valueChanged(Color*) ), TQT_SLOT( slotSetColor(Color*) ));
	connect(this, TQT_SIGNAL( valueChanged(Color*) ), imageSelection, TQT_SLOT( slotSetValue(Color*) ));
	pages->addTab(imageSelection, i18n( "Image" ));
	topLayout->addWidget(pages, 10);
	 */
	topLayout->addWidget(gradientSelection, 10);
	KSeparator* hLine = new KSeparator(KSeparator::HLine, this);
	topLayout->addWidget(hLine);
	TQHBoxLayout* tqlayout = new TQHBoxLayout();
	TextEditSelection* textEditSelection = new TextEditSelection(this);
	connect(textEditSelection, TQT_SIGNAL( valueChanged(Color*) ), TQT_SLOT( slotSetColor(Color*) ));
	connect(this, TQT_SIGNAL( valueChanged(Color*) ), textEditSelection, TQT_SLOT( slotSetValue(Color*) ));
	TQVBoxLayout* colorChangeLayout = new TQVBoxLayout();
	colorChangeLayout->setMargin(2);
	TQRadioButton* replaceButton = new TQRadioButton(i18n( "Replace" ), this);
	connect(replaceButton, TQT_SIGNAL( clicked() ), TQT_SLOT( slotColorReplace() ));
	replaceButton->setChecked(true);
	colorChangeButtons.insert(replaceButton);
	colorChangeLayout->addWidget(replaceButton);
	TQRadioButton* changeButton = new TQRadioButton(i18n( "Change" ) + ":", this);
	connect(changeButton, TQT_SIGNAL( clicked() ), TQT_SLOT( slotColorChange() ));
	colorChangeButtons.insert(changeButton);
	colorChangeLayout->addWidget(changeButton);
	colorChangeValue = 0;
	colorChangeSliderWidget = new TQWidget(this);
	TQVBoxLayout* colorChangeSliderLayout = new TQVBoxLayout(colorChangeSliderWidget, 1);
	colorChangeSliderLayout->setMargin(0);
	TQSlider* colorChangeSlider = new TQSlider(0, MAX_COLOR_CHANGE_VALUE,
		MAX_COLOR_CHANGE_VALUE/4, colorChangeValue, Qt::Horizontal, colorChangeSliderWidget);
	colorChangeSlider->setTickInterval(colorChangeSlider->pageStep());
	colorChangeSlider->setTickmarks(TQSlider::Above);
	connect(colorChangeSlider, TQT_SIGNAL( valueChanged(int) ), TQT_SLOT( slotColorChangeValueChanged(int) ));
	colorChangeSliderLayout->addWidget(colorChangeSlider);
	TQHBoxLayout* colorChangeSliderLabelsLayout = new TQHBoxLayout(0);
	TQLabel* subtractLabel = new TQLabel(i18n( "0" ), colorChangeSliderWidget);
	colorChangeSliderLabelsLayout->addWidget(subtractLabel);
	colorChangeSliderLabelsLayout->addStretch(10);
	TQLabel* addLabel = new TQLabel(" " + i18n( "Replace" ), colorChangeSliderWidget);
	colorChangeSliderLabelsLayout->addWidget(addLabel);
	colorChangeSliderLayout->addLayout(colorChangeSliderLabelsLayout);
	colorChangeLayout->addStretch(10);
	colorChangeLayout->addWidget(colorChangeSliderWidget);
	colorChangeLayout->addStretch(10);
	tqlayout->addLayout(colorChangeLayout, 10);
	m_color.setComponents(RGB_MAX_COMPONENT_VALUE, RGB_MAX_COMPONENT_VALUE, RGB_MAX_COMPONENT_VALUE);
	slotColorReplace();
	KSeparator* vLine = new KSeparator(KSeparator::VLine, this);
	tqlayout->addWidget(vLine);
	tqlayout->addWidget(textEditSelection, 1);
	colorPatch = new KColorPatch(this);
	connect(colorPatch, TQT_SIGNAL( colorChanged(const TQColor&) ), TQT_SLOT( slotSetColor(const TQColor&) ));
	colorPatch->setMinimumSize(80, 64);
	tqlayout->addWidget(colorPatch, 10);
	topLayout->addLayout(tqlayout);
}
ColorSelector::~ColorSelector() {
}

void ColorSelector::slotSetColor(Color color) {
	m_color = color;
	colorPatch->setColor(TQColor( m_color.component(Color::RED_INDEX),
		m_color.component(Color::GREEN_INDEX),
		m_color.component(Color::BLUE_INDEX) ));
	fComponentsMode = false;
	emit valueChanged(&m_color);
}

void ColorSelector::slotSetColor(Color* color) {
	slotSetColor(*color);
}

void ColorSelector::slotSetColor(const TQColor& color) {
	m_color.setComponent(Color::RED_INDEX, color.red());
	m_color.setComponent(Color::GREEN_INDEX, color.green());
	m_color.setComponent(Color::BLUE_INDEX, color.blue());
	fComponentsMode = false;
	emit valueChanged(&m_color);
}

void ColorSelector::scaledComponent(double* const component, const double componentDiff) {
	double scaledComponentDiff = componentDiff*
		pow(colorChangeValue*1.0/MAX_COLOR_CHANGE_VALUE, 2.0);
	*component += scaledComponentDiff;
	if(*component > RGB_MAX_COMPONENT_VALUE)
		*component = RGB_MAX_COMPONENT_VALUE;
	else if(*component < 0)
		*component = 0;
}

void ColorSelector::slotGradientSelectionChangeColor(Color* gradientSelectionColor) {
	switch(colorChangeMode) {
		case MODE_REPLACE:
			slotSetColor(gradientSelectionColor);
			break;
			
		case MODE_CHANGE:
			gradientSelection->slotIgnoreSetValue(true);
			double rComponent;
			double gComponent;
			double bComponent;
			if(fComponentsMode) {
				rComponent = fRComponent;
				gComponent = fGComponent;
				bComponent = fBComponent;
			} else {
				rComponent = m_color.component(Color::RED_INDEX);
				gComponent = m_color.component(Color::GREEN_INDEX);
				bComponent = m_color.component(Color::BLUE_INDEX);
			}
			double rDiff = gradientSelectionColor->component(Color::RED_INDEX) - rComponent;
			double gDiff = gradientSelectionColor->component(Color::GREEN_INDEX) - gComponent;
			double bDiff = gradientSelectionColor->component(Color::BLUE_INDEX) - bComponent;
			/*
		 	printf("base color = (%i %i %i) gradient color = (%i %i %i)\n",
		 		rComponent, gComponent, bComponent,
		 		gradientSelectionColor->getComponent(Color::RED_INDEX),
		 		gradientSelectionColor->getComponent(Color::GREEN_INDEX),
		 		gradientSelectionColor->getComponent(Color::BLUE_INDEX));
		 	 */
			scaledComponent(&rComponent, rDiff);
			scaledComponent(&gComponent, gDiff);
			scaledComponent(&bComponent, bDiff);
			Color newColor((int)( rComponent + 0.5 ),
				(int)( gComponent + 0.5 ), (int)( bComponent + 0.5 ), "");
			slotSetColor(newColor);
			fRComponent = rComponent;
			fGComponent = gComponent;
			fBComponent = bComponent;
			fComponentsMode = true;
			/*
		 	printf("output color = (%i %i %i)\n",
		 		rComponent, gComponent, bComponent);
		 	 */
			gradientSelection->slotIgnoreSetValue(false);
			break;
	}
}

void ColorSelector::slotGradientSelectionSynchronizeColor() {
	/** Notify the gradient selector to update its color */
	emit valueChanged(&m_color);
}

const Color& ColorSelector::color() {
	return m_color;
}

void ColorSelector::slotColorReplace() {
	colorChangeMode = MODE_REPLACE;
	slotGradientSelectionSynchronizeColor();
	gradientSelection->enableSynchronizeColorButton(false);
	colorChangeSliderWidget->setEnabled(false);
}

void ColorSelector::slotColorChange() {
	colorChangeMode = MODE_CHANGE;
	gradientSelection->enableSynchronizeColorButton(true);
	colorChangeSliderWidget->setEnabled(true);
}

void ColorSelector::slotColorChangeValueChanged(int value) {
	colorChangeValue = value;
}

#include "colorselector.moc"
