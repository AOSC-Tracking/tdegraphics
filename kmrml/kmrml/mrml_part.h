/* This file is part of the KDE project
   Copyright (C) 2001,2002 Carsten Pfeiffer <pfeiffer@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation, version 2.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef MRMLPART_H
#define MRMLPART_H

#include <tqcstring.h>
#include <tqstringlist.h>

#include <kurl.h>
#include <kparts/factory.h>
#include <kparts/part.h>

#include <kmrml_config.h>

#include "mrml_elements.h"

class TQCheckBox;
class TQHGroupBox;
class TQPushButton;

class KAboutData;
class KComboBox;
class KIntNumInput;

namespace KIO {
    class FileCopyJob;
    class TransferJob;
}

namespace KMrml
{

class AlgorithmDialog;
class Browser;
class CollectionCombo;
class MrmlView;

class MrmlPart : public KParts::ReadOnlyPart
{
    Q_OBJECT
  TQ_OBJECT

public:
    enum tqStatus { NeedCollection, CanSearch, InProgress };

    MrmlPart( TQWidget *parentWidget, const char *widgetName,
              TQObject *tqparent, const char *name, const TQStringList& args );
    ~MrmlPart();

    TQString sessionId() const { return m_sessionId; }
    TQString transactionId() const { return TQString(); } // ###

    void saveState( TQDataStream& stream );
    void restoreState( TQDataStream& stream );

    static KAboutData *createAboutData();

public slots:
    virtual bool openURL( const KURL& );
    virtual bool closeURL();
	
    void slotActivated( const KURL& url, ButtonState );

protected:
    virtual bool openFile();
    Algorithm firstAlgorithmForCollection( const Collection& coll ) const;
    Collection currentCollection() const;

signals:
    /**
     * allow plugins to extend the query
     */
    void aboutToStartQuery( TQDomDocument& );

private slots:
    void slotStartClicked();
    void slotSetStatusBar( const TQString& );
    void slotSetStatusBar( const KURL& url ) { slotSetStatusBar( url.prettyURL() ); }
    void slotHostComboActivated( const TQString& );

    void slotResult( KIO::Job * );
    void slotData( KIO::Job *, const TQByteArray& );

    void slotDownloadResult( KIO::Job * );

    void slotConfigureAlgorithm();
    void slotApplyAlgoConfig();
    void slotAlgoConfigFinished();

private:
    void createQuery( const KURL::List * relevantItems = 0L );
    void initCollections( const TQDomElement& );
    void initAlgorithms( const TQDomElement& );
    void performQuery( TQDomDocument& doc );
    void parseMrml( TQDomDocument& doc );
    void parseQueryResult( TQDomElement& );
    void enableExtensionActions( const KURL& url, bool enable );
    KIO::TransferJob * transferJob( const KURL& url );

    void initHostCombo();
    void enableServerDependentWidgets( bool enable );

    void settqStatus( tqStatus status );

    void contactServer( const KURL& url );
    void downloadReferenceFiles( const KURL::List& downloadList );

    KIO::TransferJob *m_job;
    MrmlView *m_view;
    Config m_config;
    KIntNumInput * m_resultSizeInput;
    CollectionCombo * m_collectionCombo;
    TQPushButton *m_algoButton;
    TQHGroupBox *m_panel;
    TQPushButton *m_startButton;
    TQCheckBox *m_random;
    Browser *m_browser;
    AlgorithmDialog *m_algoConfig;
    KComboBox *m_hostCombo;

    TQPtrList<KIO::FileCopyJob> m_downloadJobs;
    TQStringList m_tempFiles;

    TQString m_sessionId;
    KURL::List m_queryList; // a list of valid LOCAL (!) urls to query for

    CollectionList m_collections;
    AlgorithmList m_algorithms;

    tqStatus m_status;
    static uint s_sessionId;

};

class PartFactory : public KParts::Factory
{
    Q_OBJECT
  TQ_OBJECT

public:
    PartFactory();
    ~PartFactory();

    static KInstance * instance();

protected:
    virtual KParts::Part * createPartObject( TQWidget *parentWidget = 0,
                                        const char *widgetName = 0,
                                        TQObject *tqparent = 0,
                                        const char *name = 0,
                                        const char *classname = "KParts::Part",
                                        const TQStringList& args = TQStringList() );

private:
    static KInstance * s_instance;

};

}

#endif // MRMLPART_H
