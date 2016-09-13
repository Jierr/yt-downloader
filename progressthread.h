#ifndef PROGRESSTHREAD_H
#define PROGRESSTHREAD_H

#include <QtCore>
#include "mainwindow.h"
#include "ytdownload.h"

enum {RUN_DO, RUN_TERMINATE};
enum {PROGRESS_UPDATE, PROGRESS_HALT};
enum {DOWNLOAD_DO, DOWNLOAD_ABORT, DOWNLOAD_DONE, DOWNLOAD_PREPARE, DOWNLOAD_PREPARED, DOWNLOAD_INIT};
class ProgressThread: public QThread
{
private:
    double percent;
    MainWindow* mw;
    YTDownload* ytd;
    QMutex* mprogress;
    QMutex* mdownload;
    QMutex* mmanage;
    int state_download;
    int state_progress;
    int state_run;
    int event;


public:
    ProgressThread();
    ProgressThread(MainWindow* mw);
    ~ProgressThread();

    void setMainWindow(MainWindow* mw);
    void setProgressMutex(QMutex* mutex);
    void setManageMutex(QMutex* mutex);
    void setYTD(YTDownload* ytd);
    void updatePercent();
    void updateProgressbar();

    void start_prepare(string url);
    void start_download();
    void stop_download();
    void start_progress();
    void stop_progress();
    void do_terminate();
    void do_idle();


    void run();
};

#endif // PROGRESSTHREAD_H
