#include "progressthread.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

ProgressThread::ProgressThread()
{
    this->mw = 0;
    this->mprogress = 0;
    this->state_run = RUN_DO;
    this->state_progress = PROGRESS_HALT;
    this->state_download = DOWNLOAD_INIT;
}

ProgressThread::ProgressThread(MainWindow* mw)
{
    this->mw = mw;
    this->mprogress = &(mw->mprogress);
    this->state_run = RUN_DO;
    this->state_progress = PROGRESS_HALT;
    this->state_download = DOWNLOAD_INIT;
}

ProgressThread::~ProgressThread()
{
    this->mw = 0;
    this->mprogress = 0;
}


void ProgressThread::setMainWindow(MainWindow* mw)
{
    this->mw = mw;
}

void ProgressThread::setProgressMutex(QMutex* mutex)
{
    this->mprogress = mutex;
}


void ProgressThread::setYTD(YTDownload* ytd)
{
    this->ytd = ytd;
}


void ProgressThread::updatePercent()
{
    mprogress->lock();
    percent = ytd->getProgress().pc;
    mprogress->unlock();
}

void ProgressThread::updateProgressbar()
{
    mprogress->lock();
    mw->ui->progressBar->setValue(percent);
    mprogress->unlock();
}

/*
enum {RUN_DO, RUN_TERMINATE};
enum {PROGRESS_UPDATE, PROGRESS_HALT};
enum {DOWNLOAD_DO, DOWNLOAD_ABORT, DOWNLOAD_DONE, DOWNLOAD_PREPARE, DOWNLOAD_PREPARED, DOWNLOAD_PREPARE_ABORT, DOWNLOAD_INIT};
*/
void ProgressThread::start_prepare(string url)
{
    mmanage->lock();
    if((state_download == DOWNLOAD_DONE) ||
       (state_download == DOWNLOAD_INIT))
    {
        ytd->setUrl(url);
        state_download = DOWNLOAD_PREPARE;
    }
    mmanage->unlock();
}

void ProgressThread::start_download()
{
    mmanage->lock();
    if(state_download == DOWNLOAD_PREPARED)
        state_download = DOWNLOAD_DO;
    mmanage->unlock();

}

void ProgressThread::stop_download()
{
    mmanage->lock();
    if(state_download == DOWNLOAD_DO)
        state_download = DOWNLOAD_ABORT;
    mmanage->unlock();
}

void ProgressThread::start_progress()
{
    mmanage->lock();
    state_download = PROGRESS_UPDATE;
    mmanage->unlock();

}

void ProgressThread::stop_progress()
{
    mmanage->lock();
    state_download = PROGRESS_HALT;
    mmanage->unlock();
}

void ProgressThread::do_terminate()
{

}

void ProgressThread::do_idle()
{

}

/*
enum {RUN_DO, RUN_TERMINATE};
enum {PROGRESS_UPDATE, PROGRESS_HALT};
enum {DOWNLOAD_DO, DOWNLOAD_ABORT, DOWNLOAD_DONE, DOWNLOAD_PREPARE, DOWNLOAD_PREPARED, DOWNLOAD_INIT};
*/
void ProgressThread::run()
{
    bool cont = true;
    if(!mw || !mprogress)
        return;

    while(cont)
    {
        mprogress->lock();
        if (state_run == RUN_TERMINATE) cont = false;
        mprogress->unlock();
        if(cont)
        {
            switch (state_download)
            {
                DOWNLOAD_PREPARE:
                    break;
            }

            switch (state_progress)
            {
                default:
                    break;
            }

            updatePercent();
            updateProgressbar();
            msleep(100);
        }
    }
}


