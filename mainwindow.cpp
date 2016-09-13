#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "progressthread.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));


    ytd.setProgressCallback(progress_callback);
    ytd.setProgressMutex(&mprogress);

    worker = new ProgressThread(this);
    worker->setProgressMutex(&mprogress);
    worker->setYTD(&ytd);
}

MainWindow::~MainWindow()
{

    delete ui;
}

void MainWindow::showVideoResult()
{
    bool cont = false;
    string url_decoded = "";
    stringstream assemble;
    int v = 1;

    cont = pbs.first();
    while(cont)
    {
        assemble.clear();
        assemble.str("");
        url_decoded = "";
        uri_decode(url_decoded, pbs.get_url());
        assemble<< endl << "=============<[Resultat   " << v << "]>=============";
        ui->statusTextEdit->append(assemble.str().c_str());
        assemble.str(""); assemble << "Qualität:       " << pbs.get_quality();
        ui->statusTextEdit->append(assemble.str().c_str());
        assemble.str("");; assemble << "Playback:      " << endl << url_decoded;
        ui->statusTextEdit->append(assemble.str().c_str());
        ui->comboBox->addItem(QString::number(v) + " " + QString(pbs.get_quality().c_str()));

        ++v;
        cont = pbs.next();
    }
}

QMutex& MainWindow::getProgressMutex()
{
    return mprogress;
}

void MainWindow::on_actionExit_triggered()
{
   // close();
}

void MainWindow::on_prepareButton_clicked()
{
    url = ui->urlLineEdit->text();
    ui->comboBox->clear();
    ui->statusTextEdit->clear();
    ui->statusTextEdit->append(url);
    ui->progressBar->setValue(0);

    worker->start_prepare(url.toStdString());


    if(ytd.init(url.toStdString()) == 0)
    {
        ytd.setState(STATE_INIT);
        if(!ytd.parseVideoHtml())
        {
            showVideoResult();
            ytd.setState(STATE_READY);
        }
        else
        {
            ytd.setState(STATE_FAIL);
            ui->statusTextEdit->append(">>Fehler beim parsen der Website!");
        }
    }
    else
    {
        ytd.setState(STATE_FAIL);
        ui->statusTextEdit->append(">>Fehler beim Initialisieren von CURL!");
        ui->statusTextEdit->append((string(">>Message") + ytd.getCurlError()).c_str());
    }
}

void MainWindow::on_downloadButton_clicked()
{
    if(ytd.getState() == STATE_READY)
    {
        if(ui->comboBox->count() == 0)
        {
            ytd.setState(STATE_FAIL);
            ui->statusTextEdit->append(">>Fehler: Kein Playback");
            ytd.setQualityIndex(-1);
        }
        else
        {
            ytd.setQualityIndex(ui->comboBox->currentIndex());
            ui->statusTextEdit->append(QString(">>Starte Download Index: " +
                                       QString::number(ytd.getQualityIndex())));

            ui->statusTextEdit->append(QString(">>Download finished with status ") +
                                       QString::number(ytd.downloadVideo(0)));

        }
    }
}



