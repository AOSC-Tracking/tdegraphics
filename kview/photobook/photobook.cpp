/*
Copyright (c) 2004,2005 Charles Samuels <charles@kde.org>

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


#include "photobook.h"

#include <tdeparts/componentfactory.h>
#include <tdeaction.h>
#include <tdestdaccel.h>
#include <kstandarddirs.h>

Previews::Previews(PhotoBook *parent, const TQStringList &mimetypes)
	: KFileIconView(parent, 0)
{
	mPhotoBook = parent;
	mMimeTypes = mimetypes;

	connect(&mDirLister, TQT_SIGNAL(clear()), TQT_SLOT(slotClearView()));
	connect(&mDirLister, TQT_SIGNAL(completed()), TQT_SLOT(doneListing()));
	connect(
			&mDirLister, TQT_SIGNAL(newItems(const KFileItemList &)),
			TQT_SLOT(insertNewFiles(const KFileItemList &))
		);
	connect(
			signaler(), TQT_SIGNAL(fileSelected(const KFileItem*)),
			TQT_SLOT(open(const KFileItem*))
		);
	connect(
			&mDirLister, TQT_SIGNAL(deleteItem(KFileItem *)),
			TQT_SLOT(removeItem(KFileItem *))
		);
	
	connect(
			&mDirLister, TQT_SIGNAL( refreshItems( const KFileItemList& ) ),
			TQT_SLOT( slotRefreshItems( const KFileItemList& ) )
		);
	
	setFixedWidth(128);
}

Previews::~Previews()
{

}


void Previews::slotRefreshItems( const KFileItemList& items )
{
	KFileItemListIterator it( items );
	for ( ; it.current(); ++it )
		updateView( it.current() );
}

void Previews::slotClearView()
{
	clearView();
}


void Previews::insertNewFiles(const KFileItemList &items)
{
	addItemList(items);
}

void Previews::removeItem(KFileItem *item)
{
	KFileIconView::removeItem(item);
}


void Previews::go(const KFileItem *k)
{
	setSelected(k, true);
	setCurrentItem(k);
	ensureItemVisible(k);
	open(k);
	updateButton();
}

void Previews::updateButton()
{
	mPhotoBook->updateButton( ( nextItem(currentFileItem())!=0L ), ( prevItem(currentFileItem())!=0L ));
}

void Previews::openURL(const KURL &url)
{
	mDirLister.setMimeFilter(mMimeTypes);
	mDirLister.openURL(url);
}

void Previews::open(const KFileItem *item)
{
	if (!item->isDir())
	{
		const KURL &u = item->url();
		emit open(u);
		updateButton();
	}
}

void Previews::doneListing()
{
	// eep! -- this should not be necessary
	TQTimer::singleShot(0, this, TQT_SLOT(goToFirst()));
}

void Previews::goToFirst()
{
	if (firstFileItem())
		go(firstFileItem());
	else
		updateButton();
}



PhotoBook::PhotoBook(TQWidget *parent, PhotoBookPart *part, const char *name)
	: TQSplitter(parent, name)
{
	
	TQStringList mimetypes;
	
	TDETrader::OfferList offers = TDETrader::self()->query(
			"KImageViewer/Viewer", "KParts/ReadOnlyPart",
			"DesktopEntryName == 'kviewviewer'", TQString()
		);

	for (
			TDETrader::OfferList::Iterator i(offers.begin());
			i != offers.end(); ++i
		)
	{
		KService::Ptr service = *i;
		mViewer = KParts::ComponentFactory::
			createPartInstanceFromService<KParts::ReadOnlyPart>( service, this, 0, this );
		
		// is this the correct way to get the supported mimetypes?
		if (mViewer)
		{
			KSimpleConfig e(locate("services", service->desktopEntryPath()));
			e.setGroup("Desktop Entry");
			mimetypes = TQStringList::split(';', e.readEntry("MimeType"));
			break;
		}
	}
		
	
	mList = new Previews(this, mimetypes);

	mFit = mViewer->action("fittowin");

	connect(mList, TQT_SIGNAL(open(const KURL&)), mViewer, TQT_SLOT(openURL(const KURL&)));
	connect(mViewer, TQT_SIGNAL(completed()), mFit, TQT_SLOT(activate()));

	part->insertChildClient(mViewer);
}

void PhotoBook::openURL(const KURL &url)
{
	mList->openURL(url);
	mList->setIgnoreMaximumSize();
	mList->showPreviews();
}

void PhotoBook::next()
{
	KFileItem *k = mList->nextItem(mList->currentFileItem());
	if (k)
		mList->go(k);
}

void PhotoBook::previous()
{
	KFileItem *k = mList->prevItem(mList->currentFileItem());
	if (k)
		mList->go(k);
}


void PhotoBook::updateButton( bool nextExist, bool previousExit)
{
	emit emitUpdateButton( nextExist, previousExit );
}

K_EXPORT_COMPONENT_FACTORY(libphotobook, PhotoBookFactory )

PhotoBookPart::PhotoBookPart(
		TQWidget *parentWidget, const char *widgetName,
		TQObject *parent, const char *name,
		const TQStringList&
	)
	: KParts::ReadOnlyPart(parent, name)
{
	setInstance(PhotoBookFactory::instance());

	new PhotoBookBrowserExtension(this);
	bv = new PhotoBook(parentWidget, this, widgetName);
	setWidget(bv);

	connect(
			bv, TQT_SIGNAL(emitUpdateButton(bool, bool)),
			this, TQT_SLOT(slotUpdateButton(bool, bool))
		);
	m_pNextAction = KStdAction::next(bv, TQT_SLOT(next()), actionCollection(), "next");
	m_pPreviousAction = KStdAction::prior(bv, TQT_SLOT(previous()), actionCollection(), "previous");

	setXMLFile( "photobookui.rc" );

}

PhotoBookPart::~PhotoBookPart()
{

}


void PhotoBookPart::slotUpdateButton( bool nextExit, bool previousExit)
{
	m_pPreviousAction->setEnabled( previousExit );
	m_pNextAction->setEnabled( nextExit );
}

bool PhotoBookPart::openURL(const KURL &url)
{
	bv->openURL(url);
	return true;
}


TDEAboutData* PhotoBookPart::createAboutData()
{
	return new TDEAboutData(
			"photobook", I18N_NOOP("Photo Book"),
			"1.0", I18N_NOOP("Easily browse a folder of photographs"),
			TDEAboutData::License_GPL,
			I18N_NOOP("Copyright (c) 2004,2005 Charles Samuels <charles@kde.org>")
		);
}

bool PhotoBookPart::openFile()
{
	return true;
}



PhotoBookBrowserExtension::PhotoBookBrowserExtension(PhotoBookPart *p)
	: KParts::BrowserExtension(p, "PhotoBookBrowserExtension")
{

}




#include "photobook.moc"

