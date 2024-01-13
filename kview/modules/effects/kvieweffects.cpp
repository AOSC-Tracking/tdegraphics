/* This file is in the public domain */

// $Id$

#include "kvieweffects.h"

#include <tqobjectlist.h>

#include <tdeaction.h>
#include <tdelocale.h>
#include <kgenericfactory.h>
#include <kdebug.h>
#include <kimageviewer/viewer.h>
#include <kimageviewer/canvas.h>
#include <kdialogbase.h>
#include <knuminput.h>
#include <kiconeffect.h>
#include <tqvbox.h>
#include <kcolorbutton.h>
#include <kimageeffect.h>
#include <tqlabel.h>
#include <assert.h>

typedef KGenericFactory<KViewEffects> KViewEffectsFactory;
K_EXPORT_COMPONENT_FACTORY( kview_effectsplugin, KViewEffectsFactory( "kvieweffectsplugin" ) )

KViewEffects::KViewEffects( TQObject* parent, const char* name, const TQStringList & )
	: Plugin( parent, name )
	, m_gamma( 0.5 ), m_lastgamma( -1.0 )
	, m_opacity( 50 ), m_lastopacity( -1 )
	, m_intensity( 50 ), m_lastintensity( -1 )
	, m_color( white )
	, m_image( 0 )
{
	TQObjectList * viewerList = parent->queryList( 0, "KImageViewer Part", false, false );
	m_pViewer = static_cast<KImageViewer::Viewer *>( viewerList->getFirst() );
	delete viewerList;
	if( m_pViewer )
	{
		TDEAction * gammaaction = new TDEAction( i18n( "&Gamma Correction..." ), 0, 0,
				this, TQ_SLOT( gamma() ),
				actionCollection(), "plugin_effects_gamma" );
		TDEAction * blendaction = new TDEAction( i18n( "&Blend Color..." ), 0, 0,
				this, TQ_SLOT( blend() ),
				actionCollection(), "plugin_effects_blend" );
		TDEAction * intensityaction = new TDEAction( i18n( "Change &Intensity (Brightness)..." ), 0, 0,
				this, TQ_SLOT( intensity() ),
				actionCollection(), "plugin_effects_intensity" );
		gammaaction->setEnabled( m_pViewer->canvas()->image() != 0 );
		blendaction->setEnabled( m_pViewer->canvas()->image() != 0 );
		intensityaction->setEnabled( m_pViewer->canvas()->image() != 0 );
		connect( m_pViewer->widget(), TQ_SIGNAL( hasImage( bool ) ), gammaaction, TQ_SLOT( setEnabled( bool ) ) );
		connect( m_pViewer->widget(), TQ_SIGNAL( hasImage( bool ) ), blendaction, TQ_SLOT( setEnabled( bool ) ) );
		connect( m_pViewer->widget(), TQ_SIGNAL( hasImage( bool ) ), intensityaction, TQ_SLOT( setEnabled( bool ) ) );
	}
	else
		kdWarning( 4630 ) << "no KImageViewer interface found - the effects plugin won't work" << endl;
}

KViewEffects::~KViewEffects()
{
	// no need to delete m_image here since it will always be NULL at this
	// point.
	assert( m_image == 0 );
}

void KViewEffects::intensity()
{
	KDialogBase dlg( m_pViewer->widget(), "Intensity Dialog", true /*modal*/, i18n( "Change Intensity" ), KDialogBase::Ok | KDialogBase::Try | KDialogBase::Cancel );
	connect( &dlg, TQ_SIGNAL( tryClicked() ), this, TQ_SLOT( applyIntensity() ) );

	TQVBox * vbox = new TQVBox( &dlg );
	vbox->setSpacing( KDialog::spacingHint() );
	dlg.setMainWidget( vbox );
	KIntNumInput * percent = new KIntNumInput( vbox, "Intensity Input" );
	percent->setRange( 0, 100, 1, true );
	percent->setValue( m_intensity );
	percent->setLabel( i18n( "&Intensity:" ) );
	percent->setSuffix( TQString::fromAscii( "%" ) );
	connect( percent, TQ_SIGNAL( valueChanged( int ) ), this, TQ_SLOT( setIntensity( int ) ) );

	int result = dlg.exec();
	if( result == TQDialog::Accepted )
	{
		applyIntensity();
		m_pViewer->setModified( true );
	}
	else
		if( m_image )
			m_pViewer->canvas()->setImage( *m_image );
	m_lastintensity = -1;
	delete m_image;
	m_image = 0;
}

void KViewEffects::setIntensity( int intensity )
{
	m_intensity = intensity;
}

void KViewEffects::applyIntensity()
{
	kdDebug( 4630 ) << k_funcinfo << endl;
	if( m_intensity == m_lastintensity )
		return; // nothing to do

	TQImage * work = workImage();
	if( work )
	{
		KImageEffect::intensity( *work, m_intensity * 0.01 );
		m_pViewer->canvas()->setImage( *work );
		delete work;
		m_lastintensity = m_intensity;
	}
}

void KViewEffects::blend()
{
	KDialogBase dlg( m_pViewer->widget(), "Blend Color Dialog", true /*modal*/, i18n( "Blend Color" ), KDialogBase::Ok | KDialogBase::Try | KDialogBase::Cancel );
	connect( &dlg, TQ_SIGNAL( tryClicked() ), this, TQ_SLOT( applyBlend() ) );

	TQVBox * vbox = new TQVBox( &dlg );
	vbox->setSpacing( KDialog::spacingHint() );
	dlg.setMainWidget( vbox );
	KIntNumInput * opacity = new KIntNumInput( vbox, "Opacity Input" );
	opacity->setRange( 0, 100, 1, true );
	opacity->setValue( m_opacity );
	opacity->setLabel( i18n( "O&pacity:" ) );
	opacity->setSuffix( TQString::fromAscii( "%" ) );
	connect( opacity, TQ_SIGNAL( valueChanged( int ) ), this, TQ_SLOT( setOpacity( int ) ) );
	TQLabel * label = new TQLabel( i18n( "Blend c&olor:" ), vbox );
	KColorButton * color = new KColorButton( m_color, vbox, "Color Input Button" );
	label->setBuddy( color );
	connect( color, TQ_SIGNAL( changed( const TQColor & ) ), this, TQ_SLOT( setColor( const TQColor & ) ) );

	int result = dlg.exec();
	if( result == TQDialog::Accepted )
	{
		applyBlend();
		m_pViewer->setModified( true );
	}
	else
		if( m_image )
			m_pViewer->canvas()->setImage( *m_image );
	m_lastopacity = -1;
	delete m_image;
	m_image = 0;
}

void KViewEffects::setOpacity( int opacity )
{
	m_opacity = opacity;
}

void KViewEffects::setColor( const TQColor & color )
{
	m_color = color;
}

void KViewEffects::applyBlend()
{
	if( m_opacity == m_lastopacity )
		return; // nothing to do

	TQImage * work = workImage();
	if( work )
	{
		KImageEffect::blend( m_color, *work, m_opacity * 0.01 );
		m_pViewer->canvas()->setImage( *work );
		delete work;
		m_lastopacity = m_opacity;
	}
}

void KViewEffects::gamma()
{
	KDialogBase dlg( m_pViewer->widget(), "Gamma Correction Dialog", true /*modal*/, i18n( "Gamma Correction" ), KDialogBase::Ok | KDialogBase::Try | KDialogBase::Cancel );
	connect( &dlg, TQ_SIGNAL( tryClicked() ), this, TQ_SLOT( applyGammaCorrection() ) );

	// create dialog
	KDoubleNumInput * gammavalue = new KDoubleNumInput( 0.0, 1.0, 0.5, 0.01, 4, &dlg, "Gamma value input" );
	gammavalue->setRange( 0.0, 1.0, 0.01, true );
	connect( gammavalue, TQ_SIGNAL( valueChanged( double ) ), this, TQ_SLOT( setGammaValue( double ) ) );
	gammavalue->setLabel( i18n( "Gamma value:" ) );
	dlg.setMainWidget( gammavalue );

	int result = dlg.exec();
	if( result == TQDialog::Accepted )
	{
		// apply gamma correction
		applyGammaCorrection();
		m_pViewer->setModified( true );
	}
	else
	{
		if( m_image )
			m_pViewer->canvas()->setImage( *m_image );
	}
	m_lastgamma = -1;
	delete m_image;
	m_image = 0;
}

void KViewEffects::setGammaValue( double gamma )
{
	m_gamma = gamma;
	kdDebug( 4630 ) << "m_gamma = " << m_gamma << endl;
	// TODO: show effect on the fly if requested
}

void KViewEffects::applyGammaCorrection()
{
	if( m_gamma == m_lastgamma )
		return; // nothing to do

	TQImage * corrected = workImage();
	if( corrected )
	{
		TDEIconEffect::toGamma( *corrected, m_gamma );
		m_pViewer->canvas()->setImage( *corrected );
		delete corrected;
		m_lastgamma = m_gamma;
	}
}

inline TQImage * KViewEffects::workImage()
{
	if( ! m_image )
	{
		const TQImage * canvasimage = m_pViewer->canvas()->image();
		if( canvasimage )
			m_image = new TQImage( *canvasimage );
	}
	if( m_image )
	{
		TQImage * changed = new TQImage( *m_image );
		changed->detach();
		return changed;
	}
	return 0;
}
#include "kvieweffects.moc"
