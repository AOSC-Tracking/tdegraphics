/* This file is in the public domain */

// $Id$

#ifndef KVIEWEFFECTS_H
#define KVIEWEFFECTS_H

#include <kparts/plugin.h>
#include <tqcolor.h>

namespace KImageViewer { class Viewer; }

class KViewEffects : public KParts::Plugin
{
	Q_OBJECT
  
public:
	KViewEffects( TQObject* parent, const char* name, const TQStringList & );
	virtual ~KViewEffects();

private slots:
	void intensity();
	void setIntensity( int );
	void applyIntensity();

	void blend();
	void setOpacity( int );
	void setColor( const TQColor & );
	void applyBlend();

	void gamma();
	void setGammaValue( double );
	void applyGammaCorrection();

private:
	TQImage * workImage();

	KImageViewer::Viewer * m_pViewer;
	double m_gamma, m_lastgamma;
	int m_opacity, m_lastopacity;
	int m_intensity, m_lastintensity;
	TQColor m_color;
	TQImage * m_image;
};

// vim:sw=4:ts=4:cindent
#endif // KVIEWEFFECTS_H
