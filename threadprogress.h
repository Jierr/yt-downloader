#ifndef THREADPROGRESS_H
#define THREADPROGRESS_H

#include <QtCore>
#include "ytdownload.h"
class ThreadProgress : public QThread
{
    Q_OBJECT

private:
    double percent;
    bool stop;
    YTDownload* ytd;
public:
    ThreadProgress();
    ~ThreadProgress();

    QMutex* mutex;

    void preSetup(QMutex* mutex, YTDownload* ytd);
    void stopThread();
    void updateProgressbar();
    void updatePercent();

    void run();

signals:
    void ProgressChanged(double);

public slots:
};

#endif // THREADPROGRESS_H
