#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "NIServer.h"
#include <QtGui>
#include "Utils.h"

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

	// Instead of sub-classing the QThread, we create a worker class, which
	// has a process to handle all the stuff, as QThread is actually just a wrapper.
    pServerThread = new QThread;
    pServerWorker = new ServerWorker();
    pServerWorker->moveToThread(pServerThread);

	connect(pServerWorker,SIGNAL(error(QString)),this,SLOT(serverWorkerError(QString)));

    connect(pServerWorker,SIGNAL(initFinished()),this,SLOT(initFinished()));
    connect(pServerWorker,SIGNAL(licenseChecked(QString)),this,SLOT(licenseChecked(QString)));
    connect(pServerWorker,SIGNAL(engineFinished()),this,SLOT(engineLaunched()));

    connect(pServerThread,SIGNAL(started()),pServerWorker,SLOT(process()));

    connect(pServerWorker,SIGNAL(ended()),this,SLOT(serverEnded()));
    connect(pServerWorker,SIGNAL(ended()),pServerThread,SLOT(quit()));
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

	// Yes, for now just to compare the state string.
    if(stat == "TimeLimited license")
    {
        limitedTime = NIServer::GetLimitedTime();
        pTimer->start(limitedTime*60*100);

		Logger::GetInstance()->Log("Timer started for limited license");
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

void MainWindow::serverWorkerError(QString str)
{
    pUi->statusLabel->setText(str);
    pTimer->stop();
}

void MainWindow::startNIServer()
{
    pUi->statusLabel->setText("Checking License...");
    pServerThread->start();
    pServerWorker->Init();
}

void MainWindow::stopNIServer()
{
    pUi->statusLabel->setText("Stopping server...");
    pServerWorker->End();
}

void MainWindow::licenseExpired()
{
    pTimer->stop();
    pServerWorker->End();
    pUi->statusLabel->setText("Time's out, license expired! NIServer stopped.");
}
