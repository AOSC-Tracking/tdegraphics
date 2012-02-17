/* This file is part of the KDE project
   Copyright (C) 2001 Carsten Pfeiffer <pfeiffer@kde.org>

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

#ifndef MRML_VIEW_H
#define MRML_VIEW_H

#include <tqevent.h>
#include <tqframe.h>
#include <tqptrlist.h>
#include <tqpixmap.h>
#include <tqpixmapcache.h>
#include <tqscrollview.h>

class TQDomDocument;
class TQDomElement;
class TQTimer;

class KComboBox;

namespace KMrml
{

class MrmlViewItem;


class MrmlViewItemList : public TQPtrList<MrmlViewItem>
{
protected:
    // sort by similarity
    virtual int compareItems( TQPtrCollection::Item, TQPtrCollection::Item );

};


class MrmlView : public TQScrollView
{
    friend class MrmlViewItem;

    Q_OBJECT
  

public:
    MrmlView( TQWidget *parent = 0L, const char *name = 0L );
    ~MrmlView();

    MrmlViewItem * addItem( const KURL& url, const KURL& thumbURL,
                            const TQString& similarity );
    MrmlViewItem * addItem( const KURL& url, const KURL& thumbURL,
                            double similarity );


    void addRelevanceToQuery( TQDomDocument&, TQDomElement& parent );

    void clear();

    bool isEmpty() const { return m_items.isEmpty(); }

    void stopDownloads();

    void saveState( TQDataStream& stream );
    void restoreState( TQDataStream& stream );

signals:
    void activated( const KURL& url, ButtonState button );
    void onItem( const KURL& url );

protected:
    virtual void resizeEvent( TQResizeEvent * );

private slots:
    void slotLayout();
    void slotDownloadFinished( const KURL&, const TQByteArray& );

private:
    /**
     * @returns a _temporary_ pointer to a pixmap. Copy it!
     */
    TQPixmap * getPixmap( const KURL& url );

    MrmlViewItemList m_items;
    TQTimer *m_timer;
    TQPixmapCache m_pixmapCache;
    TQPixmap m_unavailablePixmap;


};


class MrmlViewItem : public TQFrame
{
    Q_OBJECT
  

public:
    enum Relevance
    {
        Relevant   = 0,
        Neutral    = 1,
        Irrelevant = 2
    };

    MrmlViewItem( const KURL& url, const KURL& thumbURL, double similarity,
                  MrmlView *view, const char *name=0L );
    virtual ~MrmlViewItem();

    void setPixmap( const TQPixmap& pixmap );

    void createRelevanceElement( TQDomDocument& document, TQDomElement& parent );

    double similarity() const { return m_similarity; }

    void setSimilarity( double value );

    virtual TQSize sizeHint() const;

    const KURL& url() const { return m_url; }
    const KURL& thumbURL() const { return m_thumbURL; }

    bool hasRemotePixmap() const { return !m_thumbURL.isLocalFile() && m_hasRemotePixmap; }

    Relevance relevance() const;
    void setRelevance( Relevance relevance );

protected:
    virtual void paintEvent( TQPaintEvent * );
    virtual void resizeEvent( TQResizeEvent * );

    virtual void mousePressEvent( TQMouseEvent * );
    virtual void mouseMoveEvent( TQMouseEvent * );
    virtual void mouseReleaseEvent( TQMouseEvent * );

private:
    bool hitsPixmap( const TQPoint& ) const;
    MrmlView * view() const { return m_view; }

    inline int pixmapX() const {
        return TQMAX( margin, (width() - m_pixmap.width()) / 2);
    }
    inline int pixmapY() const {
        return m_combo->y() - similarityHeight - m_pixmap.height() - margin;
    }

    KComboBox *m_combo; // for relevance
    MrmlView *m_view;

    KURL m_url;
    KURL m_thumbURL;

    TQPixmap m_pixmap;

    double m_similarity;
    const int similarityFullWidth;
    bool m_hasRemotePixmap;

    TQPoint pressedPos;

    static const int spacing = 3;
    static const int margin = 5;
    static const int similarityHeight = 4;

};

TQDataStream& operator <<( TQDataStream& stream, const KMrml::MrmlViewItem& );

}

#endif // MRML_VIEW_H
