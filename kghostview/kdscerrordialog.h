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

#ifndef KDSCERRORDIALOG_H
#define KDSCERRORDIALOG_H

#include <kdialog.h>

#include "dscparse_adapter.h"

class TQLabel;
class TQPushButton;
class TQTextEdit;

class KDSCErrorThreshold : public KDSCErrorHandler
{
public:
    KDSCErrorThreshold( int threshold, KDSCErrorHandler* );
    
    Response error( const KDSCError& );

private:
    int _threshold;
    KDSCErrorHandler* _errorHandler;
};

class KDSCErrorDialog : public KDialog, public KDSCErrorHandler
{
    Q_OBJECT
  TQ_OBJECT
	
public:
    KDSCErrorDialog( TQWidget* tqparent = 0 );
    
    Response error( const KDSCError& );

protected:
    TQString description( KDSCError::Type ) const;
    
protected slots:
    void slotOk();
    void slotCancel();
    void slotIgnoreAll();
    
private:
    TQLabel*    _lineNumberLabel;
    TQTextEdit* _lineLabel;
    TQLabel*    _descriptionLabel;
    
    TQPushButton* _okButton;
    TQPushButton* _cancelButton;
    TQPushButton* _ignoreAllButton;

    Response _response;
};

#endif


// vim:sw=4:sts=4:ts=8:noet
