/****************************************************************************
** $Id$
**
** Copyright (C) 2002 Carsten Pfeiffer <pfeiffer@kde.org>
**
****************************************************************************/

#ifndef INDEXCLEANER_H
#define INDEXCLEANER_H

#include <tqobject.h>
#include <tqstringlist.h>

class KProcess;

namespace KMrml
{
    class Config;
}

namespace KMrmlConfig
{
    class IndexCleaner : public TQObject
    {
        Q_OBJECT
  

    public:
        IndexCleaner( const TQStringList& dirs, const KMrml::Config *config,
                      TQObject *parent = 0, const char *name = 0 );
        ~IndexCleaner();

        void start();

    signals:
        void advance( int value );
        void finished();

    private slots:
        void slotExited( KProcess * );

    private:
        int m_stepSize;
        void startNext();

        TQStringList m_dirs;
        const KMrml::Config *m_config;
        KProcess *m_process;
    };

}


#endif // INDEXCLEANER_H
