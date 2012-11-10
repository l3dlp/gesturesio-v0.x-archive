
#include "NITcpServer.h"
#include <iostream>
#include "Utils.h"

using namespace std;

NITcpServer::NITcpServer(QObject *parent): QObject(parent)
{
    connect(&server,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
    connect(this,SIGNAL(queryAvaiable(QString)),&parser,SLOT(parse(QString)));
    connect(&parser,SIGNAL(parsed(QString)),this,SLOT(startWrite(QString)));

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
    connect(client,SIGNAL(disconnected()),this,SLOT(clientDisconnected()));

    // Report to webserver that a new client connected.
    HttpRequest(clientIn.c_str()); // send message to web server
}

void NITcpServer::startRead()
{
    char buffer[BUFFER_SIZE] = {0};
    client->read(buffer,client->bytesAvailable());
    QString str(buffer);

    emit queryAvaiable(str);
}

void NITcpServer::startWrite(QString echo)
{
    if(client)
        client->write(echo.toAscii(),echo.length());
}

void NITcpServer::clientDisconnected()
{
    // Report to webserver that the client is disconnected.
    HttpRequest(clientOut.c_str()); // send message to web server
}

void NITcpServer::SetClientLog(string in, string out)
{
    clientIn = in;
    clientOut = out;
}
