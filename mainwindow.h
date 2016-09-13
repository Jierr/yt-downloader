#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QtCore>
#include <sstream>
#include <math.h>
#include "ytdownload.h"
#include "url.hpp"


namespace Ui {
class MainWindow;
}
class ProgressThread;
class MainWindow : public QMainWindow
{
    Q_OBJECT

    friend class ProgressThread;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void showVideoResult();
    QMutex& getProgressMutex();

private slots:
    void on_actionExit_triggered();

    void on_prepareButton_clicked();

    void on_downloadButton_clicked();


private:
    Ui::MainWindow *ui;
    QString url;
    YTDownload ytd;
    ProgressThread* worker;
    QMutex mprogress;


};

#endif // MAINWINDOW_H
