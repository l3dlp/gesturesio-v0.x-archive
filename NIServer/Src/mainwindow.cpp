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

	connect(pServerThread,SIGNAL(started()),pServerWorker,SLOT(process()));

    connect(pServerWorker,SIGNAL(licenseChecked(QString)),this,SLOT(workerLicenseChecked(QString)));
	connect(pServerWorker,SIGNAL(threadRunning()),this,SLOT(workerThreadRunning()));
}

MainWindow::~MainWindow()
{
    delete pTimer;
    delete pUi;
}

void MainWindow::workerLicenseChecked(QString stat)
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

void MainWindow::workerThreadRunning()
{
	pUi->statusLabel->setText("Starting Tcp service...");
	NIServer::StartTcpService(); 
	pUi->statusLabel->setText("Tcp service started. Starting NI service...");
	NIServer::StartNIService();
	pUi->statusLabel->setText("NI service running. Now it's ready to serve the client.");
}

void MainWindow::startNIServer()
{
    pUi->statusLabel->setText("Checking License...");
    pServerThread->start();
}

void MainWindow::stopNIServer()
{
	pUi->statusLabel->setText("Stopping TCP service...");
	NIServer::StopTcpService();
    pUi->statusLabel->setText("TCP service ended.Stopping NI service...");
    NIServer::StopNIService();
	pUi->statusLabel->setText("NI Server completely ended.");
}

void MainWindow::licenseExpired()
{
    pTimer->stop();
    stopNIServer();
    pUi->statusLabel->setText("Time's out, license expired! NIServer stopped.");
}
