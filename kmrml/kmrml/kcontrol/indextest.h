/****************************************************************************
** $Id$
**
** Copyright (C) 2002 Carsten Pfeiffer <pfeiffer@kde.org> 
**
****************************************************************************/

#ifndef INDEXTEST_H
#define INDEXTEST_H

class IndexTest : public TQObject
{
    Q_OBJECT
  
    
public:
    IndexTest();
    ~IndexTest();
    
private slots:
    void slotFinished( bool success );
    void slotProgress( int percent, const TQString& message );
    
};


#endif // INDEXTEST_H
