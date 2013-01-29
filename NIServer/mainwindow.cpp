#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "NIServer.h"
#include <QtGui>
#include <QMetaType>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    pUi(new Ui::MainWindow)
{
    qRegisterMetaType<NIServer::license_State>("NILicenseStat");

    pUi->setupUi(this);
    pTimer = new QTimer();

    connect(pTimer,SIGNAL(timeout()),this,SLOT(licenseExpired()));
    connect(pUi->startBtn,SIGNAL(clicked()),this,SLOT(startNIServer()));
    connect(pUi->stopBtn,SIGNAL(clicked()),this,SLOT(stopNIServer()));

    pInitThread = new QThread;
    pInitWorker = new InitWorker();
    pInitWorker->moveToThread(pInitThread);

    connect(pInitWorker,SIGNAL(licenseFinished(QString)),this,SLOT(LicenseChecked(QString)));
    connect(pInitWorker,SIGNAL(finished()),this,SLOT(initFinished()));
    connect(pInitWorker,SIGNAL(error(QString)),this,SLOT(initFailed(QString)));

    connect(pInitThread,SIGNAL(started()),pInitWorker,SLOT(process()));
    connect(pInitWorker,SIGNAL(finished()),pInitThread,SLOT(quit()));
}

MainWindow::~MainWindow()
{
    delete pTimer;
    delete pUi;
}

void MainWindow::initFinished()
{
    pUi->statusLabel->setText("NIServer initialize successfully.");
}

void MainWindow::LicenseChecked(QString stat)
{
    int limitedTime;
    QString str;

    if(stat == "TimeLimitedLicense")
    {
        limitedTime = NIServer::GetLimitedTime();
        pTimer->start(limitedTime*60*100);
        str.sprintf("License will expire in %d minutes.Starting NIEngine...",limitedTime);
        pUi->statusLabel->setText(str);
    }
    else
    {
        pUi->statusLabel->setText(stat);
    }
}

void MainWindow::initFailed(QString str)
{
    pUi->statusLabel->setText(str);
    pTimer->stop();
}

void MainWindow::startNIServer()
{
    pUi->statusLabel->setText("Checking License...");
    pInitThread->start();
}

void MainWindow::stopNIServer()
{
    NIServer::StopNIService();
}

void MainWindow::licenseExpired()
{
    pTimer->stop();
    //NIServer::StopNIService();
    //QMessageBox::information(this,"info","Times out, license expired! NIServer stopped.");
    pUi->statusLabel->setText("Times out, license expired! NIServer stopped.");
}
