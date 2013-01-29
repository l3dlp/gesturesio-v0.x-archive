#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "NIServer.h"
#include <QtGui>

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

    connect(pInitWorker,SIGNAL(initFinished()),this,SLOT(initFinished()));
    connect(pInitWorker,SIGNAL(licenseChecked(QString)),this,SLOT(licenseChecked(QString)));
    connect(pInitWorker,SIGNAL(engineFinished()),this,SLOT(engineLaunched()));
    connect(pInitWorker,SIGNAL(error(QString)),this,SLOT(initFailed(QString)));

    connect(pInitThread,SIGNAL(started()),pInitWorker,SLOT(process()));
    connect(pInitWorker,SIGNAL(ended()),this,SLOT(serverEnded()));
    connect(pInitWorker,SIGNAL(ended()),pInitThread,SLOT(quit()));
}

MainWindow::~MainWindow()
{
    delete pTimer;
    delete pUi;
}

void MainWindow::initFinished()
{
    pUi->statusLabel->setText("NIEngine launched.Starting Tcp service..");

    NIServer::StartTcpService();
    pUi->statusLabel->setText("Tcp service started. Now it's ready to go!");
}

void MainWindow::licenseChecked(QString stat)
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

void MainWindow::engineLaunched()
{
    pUi->statusLabel->setText("Engine launched.");
}

void MainWindow::serverEnded()
{
    pUi->statusLabel->setText("NIService ended. Now start to end Tcp service..");
    NIServer::StopTcpService();
    pUi->statusLabel->setText("NIServer completely ended.");
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
    pInitWorker->Init();
}

void MainWindow::stopNIServer()
{
    pUi->statusLabel->setText("Stopping server...");
    pInitWorker->End();
}

void MainWindow::licenseExpired()
{
    pTimer->stop();
    pInitWorker->End();
    pUi->statusLabel->setText("Time's out, license expired! NIServer stopped.");
}
