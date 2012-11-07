
#include "NITcpServer.h"
#include <iostream>

using namespace std;

NITcpServer::NITcpServer(QObject *parent): QObject(parent)
{
    connect(&server,SIGNAL(newConnection()),this,SLOT(acceptConnection()));

    server.listen(QHostAddress::Any,5678);
}

NITcpServer::~NITcpServer()
{
    server.close();
}

void NITcpServer::acceptConnection()
{
    client = server.nextPendingConnection();
    connect(client,SIGNAL(readyRead()),this,SLOT(startRead()));
}

void NITcpServer::startRead()
{
    char buffer[BUFFER_SIZE] = {0};
    client->read(buffer,client->bytesAvailable());
    client->write(buffer,sizeof(buffer)); // echo
    qDebug(buffer);
}
