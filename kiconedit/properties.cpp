/*
    TDE Icon Editor - a small graphics drawing program for the TDE
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

#include <tdeconfig.h>
#include <kdebug.h>
#include <kapplication.h>

#include "properties.h"

TDEIconEditProperties* TDEIconEditProperties::m_self = 0;

TDEIconEditProperties* TDEIconEditProperties::self()
{
    if (!m_self)
        m_self = new TDEIconEditProperties();
    return m_self;
}

TDEIconEditProperties::TDEIconEditProperties() : TQObject()
{
    TDEConfig *config = kapp->config();

    config->setGroup( "Appearance" );

    m_bgMode = (TQWidget::BackgroundMode)config->readNumEntry( "BackgroundMode", TQWidget::FixedPixmap);
    m_bgColor = config->readColorEntry( "BackgroundColor", &gray);
    m_bgPixmap = config->readPathEntry("BackgroundPixmap");

    config->setGroup( "Grid" );
    m_pasteTransparent = config->readBoolEntry( "PasteTransparent", false );
    m_showGrid = config->readBoolEntry( "ShowGrid", true );
    m_gridScale = config->readNumEntry( "GridScaling", 10 );
    m_showRulers = config->readBoolEntry( "ShowRulers", true );

    if(config->readEntry( "TransparencyDisplayType", "Checkerboard" ) == "Checkerboard")
    {
      m_transparencyDisplayType = TDEIconEditGrid::TRD_CHECKERBOARD;
    }
    else
    {
      m_transparencyDisplayType = TDEIconEditGrid::TRD_SOLIDCOLOR;
    }

    TQColor checkColor1(255, 255, 255);
    TQColor checkColor2(127, 127, 127);

    m_checkerboardColor1 = config->readColorEntry( "CheckerboardColor1", &checkColor1);
    m_checkerboardColor2 = config->readColorEntry( "CheckerboardColor2", &checkColor2);

    TQString checkerboardSize = config->readEntry( "CheckerboardSize", "Medium" );

    if(checkerboardSize == "Small")
    {
      m_checkerboardSize = TDEIconEditGrid::CHK_SMALL;
    }
    else
    if(checkerboardSize == "Medium")
    {
      m_checkerboardSize = TDEIconEditGrid::CHK_MEDIUM;
    }
    else
    {
      m_checkerboardSize = TDEIconEditGrid::CHK_LARGE;
    }

    TQColor solidColor(255, 255, 255);
    m_transparencySolidColor = config->readColorEntry( "TransparencySolidColor", &solidColor);
}

TDEIconEditProperties::~TDEIconEditProperties()
{
  kdDebug(4640) << "TDEIconEditProperties: Deleting properties" << endl;
  m_self = 0;
}

void TDEIconEditProperties::save()
{
    TDEConfig *config = kapp->config();

    config->setGroup( "Appearance" );

    config->writeEntry("BackgroundMode", m_bgMode );
    config->writeEntry("BackgroundColor", m_bgColor );
    config->writePathEntry("BackgroundPixmap", m_bgPixmap );

    config->setGroup( "Grid" );
    config->writeEntry("PasteTransparent", m_pasteTransparent );
    config->writeEntry("ShowGrid", m_showGrid );
    config->writeEntry("GridScaling", m_gridScale );
    config->writeEntry("ShowRulers", m_showRulers );

    TQString transparencyDisplayType;

    switch(m_transparencyDisplayType)
    {
      case TDEIconEditGrid::TRD_SOLIDCOLOR:
        transparencyDisplayType = "SolidColor";
        break;
      case TDEIconEditGrid::TRD_CHECKERBOARD:
      default:
        transparencyDisplayType = "Checkerboard";
        break;
    }

    config->writeEntry( "TransparencyDisplayType", transparencyDisplayType );
    config->writeEntry( "CheckerboardColor1", m_checkerboardColor1 );
    config->writeEntry( "CheckerboardColor2", m_checkerboardColor2 );

    TQString checkerboardSize;

    switch(m_checkerboardSize)
    {
      case TDEIconEditGrid::CHK_SMALL:
        checkerboardSize = "Small";
        break;
      case TDEIconEditGrid::CHK_MEDIUM:
        checkerboardSize = "Medium";
        break;
      case TDEIconEditGrid::CHK_LARGE:
      default:
        checkerboardSize = "Large";
        break;
    }

    config->writeEntry( "CheckerboardSize", checkerboardSize );
    config->writeEntry( "TransparencySolidColor", m_transparencySolidColor );

    config->sync();
}

