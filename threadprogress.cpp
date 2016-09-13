#include "threadprogress.h"

ThreadProgress::ThreadProgress()
{
    mutex = 0;
    ytd = 0;
    stop = false;
}

ThreadProgress::~ThreadProgress()
{
    mutex = 0;
    ytd = 0;
}

void ThreadProgress::preSetup(QMutex* mutex, YTDownload* ytd)
{
    this->mutex = mutex;
    this->ytd = ytd;
}

void ThreadProgress::stopThread()
{
    mutex->lock();
    stop = true;
    mutex->unlock();
}

void ThreadProgress::updatePercent()
{
    mutex->lock();
    percent = ytd->getProgress().pc;
    mutex->unlock();
}

void ThreadProgress::updateProgressbar()
{
    mutex->lock();
    emit ProgressChanged(percent);
    mutex->unlock();
}

void ThreadProgress::run()
{
    bool cont = true;
    while (cont)
    {
        mutex->lock();
        if (stop) cont = false;
        mutex->unlock();
        if(cont)
        {
            updatePercent();
            updateProgressbar();
            msleep(100);
        }
    }
}

