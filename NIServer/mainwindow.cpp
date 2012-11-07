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
    int limitedTime;
    QString str;
    ui->statusLabel->setText("Checking License...");
    license_State stat = CheckLicense();

    switch(stat)
    {
    case LICENSE_VALID:
        //StartNIServer();
        ui->statusLabel->setText("License is valid, NIServer started.");
        break;
    case LICENSE_INVALID:
        ui->statusLabel->setText("License is invalid. NIServer is unable to start.");
        break;
    case LICENSE_TIMELIMITED:
        limitedTime = GetLimitedTime();
        timer->start(limitedTime*60*100);
        str.sprintf("License will expire in %d minutes. NIServer started.",limitedTime);
        ui->statusLabel->setText(str);
        break;
    case LICENSE_UNKNOWN:
    default:
        ui->statusLabel->setText("License validation failed. NIServer will not start.");
        break;
    }
}

void MainWindow::stopNIServer()
{
    StopNIServer();
}

void MainWindow::licenseExpired()
{
    timer->stop();
    //StopNIServer();
    //QMessageBox::information(this,"info","Times out, license expired! NIServer stopped.");
    ui->statusLabel->setText("Times out, license expired! NIServer stopped.");
}
