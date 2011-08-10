/* This file is part of the KDE project
   Copyright (C) 2002 Carsten Pfeiffer <pfeiffer@kde.org>

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

#include "algorithmdialog.h"
#include "algorithmcombo.h"
#include "collectioncombo.h"

#include <tqhbox.h>
#include <tqlabel.h>
#include <tqlayout.h>
#include <tqscrollview.h>
#include <tqvbox.h>
#include <tqvgroupbox.h>

#include <klocale.h>

using namespace KMrml;

class ScrollView : public TQScrollView
{
public:
    ScrollView(TQWidget* parent = 0, const char* name = 0)
        : TQScrollView(parent, name)
    {
        setFrameStyle(TQFrame::NoFrame);
        m_frame = new TQFrame(viewport(), "ScrollView::m_frame");
        m_frame->setFrameStyle(TQFrame::NoFrame);
        addChild(m_frame, 0, 0);
    };

    TQFrame* frame() {return m_frame;};

protected:
    virtual void viewportResizeEvent(TQResizeEvent* ev)
    {
      TQScrollView::viewportResizeEvent(ev);
      m_frame->resize( kMax(m_frame->tqsizeHint().width(), ev->size().width()),
                       kMax(m_frame->tqsizeHint().height(), ev->size().height()));
    };

private:
      TQFrame* m_frame;
};

AlgorithmDialog::AlgorithmDialog( const AlgorithmList& algorithms,
                                  const CollectionList& collections,
                                  const Collection& currentColl,
                                  TQWidget *parent, const char *name )
    : KDialogBase( parent, name, false, i18n("Configure Query Algorithms"),
                   Ok | Cancel, Ok, false ),
      m_allAlgorithms( algorithms ),
      m_collections( collections )
{
    TQWidget *box = makeMainWidget();

    TQVBoxLayout *mainLayout = new TQVBoxLayout( box, 0, KDialog::spacingHint(),
                                                "mainLayout");

    TQHBoxLayout *collectionLayout = new TQHBoxLayout( 0L, 0, 0, "coll tqlayout");
    collectionLayout->addWidget( new TQLabel( i18n("Collection: "), box ));

    m_collectionCombo = new CollectionCombo( box, "collection combo" );
    m_collectionCombo->setCollections( &m_collections );
    collectionLayout->addWidget( m_collectionCombo );

    mainLayout->addLayout( collectionLayout );
    mainLayout->addSpacing( 14 );

    TQHBox *algoHLayout = new TQHBox( box );
    (void) new TQLabel( i18n("Algorithm: "), algoHLayout);
    m_algoCombo = new AlgorithmCombo( algoHLayout, "algo combo" );

    TQVGroupBox *groupBox = new TQVGroupBox( box, "groupBox" );
    mainLayout->addWidget( groupBox );
    algoHLayout->raise();

    ScrollView *scrollView = new ScrollView( groupBox, "scroll view" );
    m_view = scrollView->frame();
    TQVBoxLayout *viewLayout = new TQVBoxLayout( scrollView );
    viewLayout->setSpacing( KDialog::spacingHint() );
    
    
    collectionChanged( currentColl );
    
    connect( m_algoCombo, TQT_SIGNAL( selected( const Algorithm& ) ),
             TQT_SLOT( initGUI( const Algorithm& ) ));
    connect( m_collectionCombo, TQT_SIGNAL( selected( const Collection& ) ),
             TQT_SLOT( collectionChanged( const Collection& ) ));

    algoHLayout->adjustSize();
    mainLayout->activate();
    algoHLayout->move( groupBox->x() + 10, groupBox->y() - 12 );
    
    box->setMinimumWidth( algoHLayout->tqsizeHint().width() + 
                          4 * KDialog::spacingHint() );
}

AlgorithmDialog::~AlgorithmDialog()
{
}

void AlgorithmDialog::collectionChanged( const Collection& coll )
{
    m_algosForCollection = m_allAlgorithms.algorithmsForCollection( coll );
    m_algoCombo->setAlgorithms( &m_algosForCollection );
    
    initGUI( m_algoCombo->current() );
}

void AlgorithmDialog::initGUI( const Algorithm& algo )
{
    m_algo = algo;
    
    
}

#include "algorithmdialog.moc"
