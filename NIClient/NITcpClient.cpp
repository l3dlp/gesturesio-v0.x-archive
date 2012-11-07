
#include "NITcpClient.h"
#include <QHostAddress>


NITcpClient::NITcpClient(QObject *parent): QObject(parent)
{
    connect(&client,SIGNAL(connected()),this,SLOT(serverConnected()));
    isConnected = false;
}

NITcpClient::~NITcpClient()
{
    client.close();
}

void NITcpClient::connectServer(QString address, quint16 port)
{
    QHostAddress addr(address);
    client.connectToHost(addr,port);
}

void NITcpClient::disconnectServer()
{
    client.disconnectFromHost();
    isConnected = false;
}

void NITcpClient::serverConnected()
{
    connect(&client,SIGNAL(readyRead()),this,SLOT(startRead()));
    isConnected = true;

}

void NITcpClient::transfer(QString msg)
{
    if(isConnected)
    {
        client.write(msg.toAscii(),msg.length());
    }
}

void NITcpClient::startRead()
{
    char buffer[BUFFER_SIZE] = {0};
    client.read(buffer,client.bytesAvailable());
    qDebug(buffer);
}
