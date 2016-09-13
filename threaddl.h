#ifndef THREADDL_H
#define THREADDL_H

#include <QtCore>
#include "ytdownload.h"

class ThreadDL : public QThread
{

private:
    YTDownload* ytd;
    QMutex* mdownload;
public:
    ThreadDL();
    ~ThreadDL();
    void downloadPrepare(YTDownload* ytd);
};

#endif // THREADDL_H
