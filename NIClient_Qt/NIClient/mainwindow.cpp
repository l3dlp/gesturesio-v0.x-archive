#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	tcpSocket = new QTcpSocket(this);
	isConnected = false;

    ui->setupUi(this);

	connect(ui->connectBtn, SIGNAL(clicked()), this, SLOT(onConnect()));
	connect(ui->readBtn, SIGNAL(clicked()), this, SLOT(onRead()));
	connect(ui->disconnectBtn, SIGNAL(clicked()), this, SLOT(onDisconnect()));

	connect(tcpSocket, SIGNAL(connected()), this, SLOT(onConnected()));
	connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
	connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readData()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onConnect()
{
	tcpSocket->abort();
	tcpSocket->connectToHost("localhost", 6789);
}

void MainWindow::onRead()
{
	if (isConnected)
	{
		ui->dataLabel->setText("");
		tcpSocket->write((const char*)"getCoordsX",10);
	}
}

void MainWindow::onDisconnect()
{
    tcpSocket->abort();
}

void MainWindow::onConnected()
{
	isConnected = true;
	ui->statusLabel->setText("Server connected.");
}

void MainWindow::onDisconnected()
{
	isConnected = false;
	ui->statusLabel->setText("Server disconnected.");
}

void MainWindow::readData()
{
	char buffer[1024] = {0};

	tcpSocket->read(buffer,tcpSocket->bytesAvailable());
	QString str(buffer);
	ui->dataLabel->setText(str);
}

