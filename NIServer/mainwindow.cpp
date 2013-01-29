#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "NIServer.h"
#include <QtGui>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(licenseExpired()));
    connect(ui->startBtn,SIGNAL(clicked()),this,SLOT(startNIServer()));
    connect(ui->stopBtn,SIGNAL(clicked()),this,SLOT(stopNIServer()));
}

MainWindow::~MainWindow()
{
    delete timer;
    delete ui;
}

void MainWindow::startNIServer()
{
    bool res;
    int limitedTime;
    QString str;
    ui->statusLabel->setText("Checking License...");
    NIServer::license_State stat = NIServer::CheckLicense();

    switch(stat)
    {
    case NIServer::LICENSE_VALID:
        res = NIServer::StartNIService();
        if(res)
        {
           ui->statusLabel->setText("License is valid, NIServer started.");
        }
        else
        {
           ui->statusLabel->setText("License is valid, but failed to start NIEngine.");
        }
        break;
    case NIServer::LICENSE_INVALID:
        ui->statusLabel->setText("License is invalid. NIServer is unable to start.");
        break;
    case NIServer::LICENSE_TIMELIMITED:
        limitedTime = NIServer::GetLimitedTime();
        timer->start(limitedTime*60*100);
        str.sprintf("License will expire in %d minutes. NIServer started.",limitedTime);
        ui->statusLabel->setText(str);
        break;
    case NIServer::LICENSE_UNKNOWN:
    default:
        ui->statusLabel->setText("License validation failed. NIServer will not start.");
        break;
    }
}

void MainWindow::stopNIServer()
{
    NIServer::StopNIService();
}

void MainWindow::licenseExpired()
{
    timer->stop();
    //NIServer::StopNIService();
    //QMessageBox::information(this,"info","Times out, license expired! NIServer stopped.");
    ui->statusLabel->setText("Times out, license expired! NIServer stopped.");
}
