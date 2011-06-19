/***************************************************************************
                          loadpalettedlg.cpp  -  description
                             -------------------
    begin                : Sat Jul 8 2000
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

#include <string.h>

#include <tqlayout.h>
#include <tqlabel.h>
#include <tqstring.h>
#include <tqstringlist.h>
#include <tqpushbutton.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <kpalette.h>
#include <kfiledialog.h>

#include "main.h"
#include "palette.h"
#include "loadpalettedlg.h"

LoadPaletteDlg::LoadPaletteDlg(TQWidget *tqparent, const char *name) 
        : KDialogBase(tqparent, name, true, i18n( "Load Palette" ), 
          Ok|Cancel, Ok, true) {
	fileName = "";
    TQWidget *mainWidget = new TQWidget( this );
    setMainWidget( mainWidget );
	TQVBoxLayout* topLayout = new TQVBoxLayout(mainWidget, 0, spacingHint());
	TQLabel* label = new TQLabel(i18n( "Select a palette:" ), mainWidget);
	topLayout->addWidget(label);
	paletteBox = new TQComboBox(false, mainWidget);
	browseFileNameInserted = false;
	TQStringList palettesList = Palette::kdePalettes();
	for(TQStringList::Iterator palette = palettesList.begin();
		palette != palettesList.end(); ++palette) {
		bool prepend = (*palette).tqcontains( "colors/Custom_Colors" );
		TQString fileName = locate("config", (*palette));
		if(prepend) {
			palettesFileNames.prepend(fileName);
			setFileName(&fileName);
		}	else {
			palettesFileNames.append(fileName);
			if(palette == palettesList.begin())
				setFileName(&fileName);
		}
		TQString paletteName = (*palette).mid(palettesDir.length() + 1);
		if(paletteName == "Custom_Colors")
			paletteName = i18n("Custom Colors");
		else if(paletteName == "Recent_Colors")
			paletteName = i18n("Recent Colors");
		if(prepend)
			paletteBox->insertItem(paletteName, 0);
		else
			paletteBox->insertItem(paletteName);
	}
	connect(paletteBox, TQT_SIGNAL( activated(int) ), TQT_SLOT( setFileName(int) ));
	topLayout->addWidget(paletteBox);
	TQHBoxLayout* browseLayout = new TQHBoxLayout( mainWidget );
	TQPushButton* browseButton = new TQPushButton(i18n( "Browse..." ), 
        mainWidget);
	connect(browseButton, TQT_SIGNAL( clicked() ), TQT_SLOT( browseFileNames() ));
	browseLayout->addWidget(browseButton);
	browseLayout->addStretch(10);
	topLayout->addLayout(browseLayout);
	topLayout->addStretch(10);
	resize(300, 155);
}
LoadPaletteDlg::~LoadPaletteDlg() {
}
	
void LoadPaletteDlg::setFileName(TQString* fileName) {
	this->fileName = *fileName;
}

void LoadPaletteDlg::setFileName(int index) {
	setFileName(&palettesFileNames[index]);
}

void LoadPaletteDlg::browseFileNames() {
	TQString fileToOpen = KFileDialog::getOpenFileName(lastOpenPaletteFileDir,
		i18n("*|All Files"), this, i18n("Open File"));
	if(!fileToOpen.isEmpty()) {
		fileName = fileToOpen;
		if(browseFileNameInserted) {
			paletteBox->removeItem(0);
			palettesFileNames.remove(palettesFileNames.begin());
		}
		paletteBox->insertItem(fileName, 0);
		paletteBox->setCurrentItem(0);
		palettesFileNames.prepend(fileName);
		browseFileNameInserted = true;
		lastOpenPaletteFileDir = fileName;
	}
}

TQString LoadPaletteDlg::getFileName() {
	return fileName;
}
#include "loadpalettedlg.moc"
