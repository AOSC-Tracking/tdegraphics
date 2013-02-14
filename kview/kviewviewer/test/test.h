#ifndef KIMAGEVIEWERTEST_H
#define KIMAGEVIEWERTEST_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif 

#include <tdeapplication.h>
#include <tdeparts/mainwindow.h>

namespace KParts { class ReadWritePart; }
 
class Test : public KParts::MainWindow
{
    Q_OBJECT
  
public:
    Test();
    virtual ~Test();
    void load(const KURL& url);

private:
	KParts::ReadWritePart * m_part;
};

#endif // KIMAGEVIEWERTEST_H
