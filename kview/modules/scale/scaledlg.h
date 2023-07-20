/*  This file is part of the KDE project
    Copyright (C) 2002 Matthias Kretz <kretz@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

*/

// $Id$

#ifndef __scaledlg_h_
#define __scaledlg_h_

#include <tqobject.h>

class TQLabel;
class KFloatSpinBox;
class KComboBox;
class TQCheckBox;
class TQVBox;
class TQSize;

class ScaleDlg : public TQObject
{
	TQ_OBJECT
  
	public:
		ScaleDlg( const TQSize & originalsize, TQVBox * parent, const char * name = 0 );
		~ScaleDlg();

	private slots:
		void slotNewWidth( float );
		void slotNewHeight( float );
		void slotNewWidth2( float );
		void slotNewHeight2( float );
		void slotResolutionX( float );
		void slotResolutionY( float );
		void slotChangeNewSizeUnit( int );
		void slotChangeNewSizeUnit2( int );
		void slotChangeResolutionUnit( int );

	private:
		TQSize m_origsize;
		int m_newsizeunit;
		int m_newsizeunit2;
		int m_resolutionunit;

		float m_newwidth, m_newheight; // in Pixel
		float m_resx, m_resy; // in dpi

		TQLabel * m_pOldWidth;
		TQLabel * m_pOldHeight;
		KFloatSpinBox * m_pNewWidth;
		KFloatSpinBox * m_pNewHeight;
		KComboBox * m_pNewSizeUnit;
		KFloatSpinBox * m_pRatioX;
		KFloatSpinBox * m_pRatioY;
		TQCheckBox * m_pLinkRatio;

		KFloatSpinBox * m_pNewWidth2;
		KFloatSpinBox * m_pNewHeight2;
		KComboBox * m_pNewSizeUnit2;
		KFloatSpinBox * m_pResolutionX;
		KFloatSpinBox * m_pResolutionY;
		TQCheckBox * m_pLinkResolution;
		KComboBox * m_pResolutionUnit;
};

#endif
