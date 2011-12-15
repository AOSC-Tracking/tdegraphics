/**
 * Copyright (C) 2001 the KGhostView authors. See file AUTHORS.
 * 	
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <tqlabel.h>
#include <layout.h>
#include <textedit.h>

#include <kdebug.h>
#include <klocale.h>
#include <kseparator.h>
#include <kpushbutton.h>
#include <kstdguiitem.h>

#include "kdscerrordialog.h"
#include "kdscerrordialog.moc"

KDSCErrorThreshold::KDSCErrorThreshold( int threshold, 
                                        KDSCErrorHandler* errorHandler ) :
    _threshold( threshold ),
    _errorHandler( errorHandler )
{}

KDSCErrorHandler::Response KDSCErrorThreshold::error( const KDSCError& err )
{
    if( _errorHandler != 0 && err.severity() >= _threshold )
	return _errorHandler->error( err );
    else
	// Cancel is the default handling strategy for dsc_error_fn, so 
	// we keep it. This cancels error handling and *not* document 
	// parsing! 
	return Cancel;
}
    
KDSCErrorDialog::KDSCErrorDialog( TQWidget* parent ) :
    KDialog( parent, "dscerrordialog", true ),
    _response( Ok )
{
    TQVBoxLayout* vbox = new TQVBoxLayout( this, marginHint(), spacingHint() );
    
    _lineNumberLabel = new TQLabel( this );
    vbox->addWidget( _lineNumberLabel );
    
    _lineLabel = new TQTextEdit( this );
    _lineLabel->setReadOnly( true );
    vbox->addWidget( _lineLabel );
    
    _descriptionLabel = new TQLabel( this );
    vbox->addWidget( _descriptionLabel );

    KSeparator* sep = new KSeparator( KSeparator::HLine, this );
    vbox->addWidget( sep );

    TQHBoxLayout* hbox = new TQHBoxLayout( vbox );

    hbox->addStretch();
    
    _okButton = new KPushButton( KStdGuiItem::ok(), this );
    hbox->addWidget( _okButton );
    _cancelButton = new KPushButton( KStdGuiItem::cancel(), this );
    hbox->addWidget( _cancelButton );
    _ignoreAllButton = new TQPushButton( i18n("Ignore All"), this );
    hbox->addWidget( _ignoreAllButton );

    connect( _okButton,     TQT_SIGNAL( clicked() ), this, TQT_SLOT( slotOk()     ) );
    connect( _cancelButton, TQT_SIGNAL( clicked() ), this, TQT_SLOT( slotCancel() ) );
    connect( _ignoreAllButton, TQT_SIGNAL( clicked() ), 
             this, TQT_SLOT( slotIgnoreAll() ) );
}

KDSCErrorHandler::Response KDSCErrorDialog::error( const KDSCError& err )
{
    switch( err.severity() )
    {
    case KDSCError::Information:
	setCaption( i18n( "DSC Information" ) ); 
	break;
    case KDSCError::Warning:
	setCaption( i18n( "DSC Warning" ) ); 
	break;
    case KDSCError::Error:
	setCaption( i18n( "DSC Error" ) ); 
	break;
    }

    _lineNumberLabel->setText( i18n( "On line %1:" ).arg( err.lineNumber() ) );
    _lineLabel->setText( err.line() );
    _descriptionLabel->setText( description( err.type() ) );
    
    exec();

    kdDebug(4500) << "KDSCErrorDialog: returning " << _response << endl;
	
    return _response;
}

TQString KDSCErrorDialog::description( KDSCError::Type type ) const
{
    switch( type )
    {
    case KDSCError::BBox: 
	return "TODO"; 
    case KDSCError::EarlyTrailer: 
	return "TODO"; 
    case KDSCError::EarlyEOF: 
	return "TODO"; 
    case KDSCError::PageInTrailer: 
	return "TODO"; 
    case KDSCError::PageOrdinal: 
	return "TODO"; 
    case KDSCError::PagesWrong: 
	return "TODO"; 
    case KDSCError::EPSNoBBox: 
	return "TODO"; 
    case KDSCError::EPSPages: 
	return "TODO"; 
    case KDSCError::NoMedia: 
	return "TODO"; 
    case KDSCError::AtEnd: 
	return "TODO"; 
    case KDSCError::DuplicateComment: 
	return "TODO"; 
    case KDSCError::DuplicateTrailer: 
	return "TODO"; 
    case KDSCError::BeginEnd: 
	return "TODO"; 
    case KDSCError::BadSection: 
	return "TODO"; 
    case KDSCError::LongLine: 
	return i18n( "Lines in DSC documents must be shorter than 255 "
	             "characters." ); 
    case KDSCError::IncorrectUsage: 
	return "TODO"; 
    default: return "TODO"; 
    }
}

void KDSCErrorDialog::slotOk()
{
    _response = Ok;
    accept();
}

void KDSCErrorDialog::slotCancel()
{
    _response = Cancel;   
    accept();
}

void KDSCErrorDialog::slotIgnoreAll()
{
    _response = IgnoreAll;   
    accept();
}

// vim:sw=4:sts=4:ts=8:noet
