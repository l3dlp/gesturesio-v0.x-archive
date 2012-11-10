#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->connectBtn,SIGNAL(clicked()),this,SLOT(connectServer()));
    connect(ui->disconnectBtn,SIGNAL(clicked()),this,SLOT(disconnectServer()));
    connect(ui->querybtn,SIGNAL(clicked()),this,SLOT(query()));
    connect(ui->cmdText,SIGNAL(textChanged()),this,SLOT(sendCommand()));
    connect(&client, SIGNAL(dataAvailable(QString)), this, SLOT(updateData(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectServer()
{
    client.connectServer("127.0.0.1",5678);
}

void MainWindow::disconnectServer()
{
    client.disconnectServer();
}

void MainWindow::query()
{
    QString str = "getCoordsT";
    client.transfer(str,true);
}

void MainWindow::sendCommand()
{
    client.transfer(ui->cmdText->toPlainText(),false);
}

void MainWindow::updateData(QString data)
{
    ui->serverEchoText->setText(data);
}
