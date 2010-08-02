/* This file is part of the KDE Project
   Copyright (C)2001 Nikolas Zimmermann <wildfox@kde.org>
                     Klaas Freitag <freitag@suse.de>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef SCAN_H
#define SCAN_H

#include <tqimage.h>
#include <kscan.h>

class ScanParams;
class KScanDevice;
class Previewer;
class TQSplitter;

class ScanDialog : public KScanDialog
{
   Q_OBJECT

public:
   ScanDialog( TQWidget *parent=0, const char *name=0, bool modal=false );
   ~ScanDialog();

   virtual bool setup();

private:
   void createOptionsTab( void );

protected slots:
   void slotFinalImage( TQImage *, ImgScanInfo * );
   void slotNewPreview( TQImage * );
   void slotScanStart( );
   void slotScanFinished( KScanStat status );
   void slotAcquireStart();

private slots:
   void slotAskOnStartToggle(bool state);
   void slotNetworkToggle( bool state);


   void slotClose();
private:
   ScanParams   *m_scanParams;
   KScanDevice  *m_device;
   Previewer    *m_previewer;
   TQImage       m_previewImage;
   bool         good_scan_connect;
   TQCheckBox    *cb_askOnStart;
   TQCheckBox    *cb_network;
   TQSplitter    *splitter;
   class ScanDialogPrivate;
   ScanDialogPrivate *d;
};

class ScanDialogFactory : public KScanDialogFactory
{
public:
   ScanDialogFactory( TQObject *parent=0, const char *name=0 );

protected:
   virtual KScanDialog * createDialog( TQWidget *parent=0, const char *name=0,
				       bool modal=false );


};

#endif // SCAN_H
