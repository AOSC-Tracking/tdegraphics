/*
Copyright (c) 2004 Charles Samuels <charles@kde.org>

  This file is hereby licensed under the GNU General Public License version
  2 or later at your option.

  This file is licensed under the TQt Public License version 1 with the
  condition that the licensed will be governed under the Laws of California
  (USA) instead of Norway.  Disputes will be settled in Santa Clara county
  courts.

  This file is licensed under the following additional license.  Be aware
  that it is identical to the BSD license, except for the added clause 3:

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
 3. By integrating this software into any other software codebase, you waive
    all rights to any patents associated with the stated codebase.

 THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef PHOTOBOOK_H
#define PHOTOBOOK_H

#include <tqsplitter.h>
#include <kfileiconview.h>
#include <kdirlister.h>

#include <kparts/part.h>
#include <kparts/browserextension.h>
#include <kparts/genericfactory.h>

class Previews;
class PhotoBookPart;

class PhotoBook : public TQSplitter
{
Q_OBJECT
TQ_OBJECT
	Previews *mList;
	KAction *mFit;
	KParts::ReadOnlyPart *mViewer;

public:
	PhotoBook(TQWidget *parent, PhotoBookPart *part, const char *name=0);
	void openURL(const KURL &url);
	void updateButton(bool, bool );

public slots:
	void next();
	void previous();
	
signals:
	void emitUpdateButton( bool, bool );
};

class Previews : public KFileIconView
{
  Q_OBJECT
  TQ_OBJECT
	
	KDirLister mDirLister;
	PhotoBook *mPhotoBook;
	TQStringList mMimeTypes;

public:
	Previews(PhotoBook *parent, const TQStringList &mimetypes);
	~Previews();
	void openURL(const KURL &url);

	PhotoBook *bv() { return mPhotoBook; }
	void updateButton( );
    
public slots:
	void go(const KFileItem*);

signals:
	void open(const KURL & ki);

private slots:
	void insertNewFiles(const KFileItemList &);
	void removeItem(KFileItem *item);
	void open(const KFileItem*);
	void doneListing();
	void goToFirst();
	void slotRefreshItems( const KFileItemList& items );
	void slotClearView();
};


class PhotoBookPart : public KParts::ReadOnlyPart
{
Q_OBJECT
  TQ_OBJECT
	PhotoBook *bv;
	KAction *m_pPreviousAction;
	KAction *m_pNextAction;

public:
	PhotoBookPart(
			TQWidget *parentWidget, const char *widgetName,
			TQObject *parent, const char *name=0,
			const TQStringList& args = TQStringList()
		);
	~PhotoBookPart();

	virtual bool openURL(const KURL &url);
	static KAboutData* createAboutData();
	virtual bool openFile();

public slots:
	void slotUpdateButton( bool nextExit, bool previousExit);
};

typedef KParts::GenericFactory<PhotoBookPart> PhotoBookFactory;

class PhotoBookBrowserExtension : public KParts::BrowserExtension
{
Q_OBJECT
  TQ_OBJECT
public:
	PhotoBookBrowserExtension(PhotoBookPart *p);
};


#endif

