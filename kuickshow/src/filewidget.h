/* This file is part of the KDE project
   Copyright (C) 1998-2003 Carsten Pfeiffer <pfeiffer@kde.org>

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

#ifndef FILEWIDGET_H
#define FILEWIDGET_H

#include <tqevent.h>

#include <kdiroperator.h>

class FileFinder;
class KFileItem;

class FileWidget : public KDirOperator
{
    Q_OBJECT
  TQ_OBJECT

public:
    enum WhichItem { Previous, Next, Current };

    FileWidget( const KURL& url, TQWidget *parent = 0L, const char *name = 0L );
    ~FileWidget();

    bool hasFiles() const;
    void reloadConfiguration();

    void setInitialItem( const TQString& filename );

    KFileItem *getCurrentItem( bool onlyImage ) const {
	return getItem( Current, onlyImage );
    }

    void setCurrentItem( const KFileItem * );
    void setCurrentItem( const TQString& filename ) {
	KDirOperator::setCurrentItem( filename );
    }

    KFileItem * gotoFirstImage();
    KFileItem * gotoLastImage();
    KFileItem * getNext( bool go=true );
    KFileItem * getPrevious( bool go=true );


    KFileItem *getItem( WhichItem which, bool onlyImage ) const;

    static bool isImage( const KFileItem * );
    static void setImage( KFileItem& item, bool enable );

    void initActions();

signals:
    void finished();

protected:
    virtual bool eventFilter( TQObject *o, TQEvent * );
    virtual void resizeEvent( TQResizeEvent * );
    virtual void activatedMenu( const KFileItem *, const TQPoint& );
    virtual TQSize sizeHint() const;

private slots:
    void slotReturnPressed( const TQString& text );
    void findCompletion( const TQString& );
    void slotViewChanged();

    void slotItemsCleared();
    void slotItemDeleted( KFileItem * );
    void slotHighlighted( const KFileItem * );

    void slotURLEntered( const KURL& url );
    void slotFinishedLoading();

private:
    KFileView * fileView() const { return (KFileView*) view(); }

    bool m_validCompletion;
    FileFinder *m_fileFinder;
    TQString m_currentURL;
    TQString m_initialName;

};


#endif // FILEWIDGET_H
