
#include "NITcpServer.h"
#include <iostream>
#include "Utils.h"

using namespace std;

NITcpServer::NITcpServer(QObject *parent): QObject(parent)
{
    connect(&server,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
}

void NITcpServer::Start(int port)
{
    server.listen(QHostAddress::Any, port);
}

void NITcpServer::Stop()
{
   server.close();

   for (int i = 0; i < clientList.length(); i++)
   {
	   delete clientList[i];
   }
   clientList.clear();
}

NITcpServer::~NITcpServer()
{
}

void NITcpServer::acceptConnection()
{
    QTcpSocket* client = server.nextPendingConnection();
	if (client != NULL)
	{
		NITcpSocket* socket = new NITcpSocket(client);
		connect(socket, SIGNAL(removeMe(NITcpSocket* )), this, SLOT(clientDisconnected(NITcpSocket*)));
		clientList.push_back(socket);
	}

    // Report to webserver that a new client connected.
    // Currently comment out as it takes too much time.
    //HttpRequest(clientIn.c_str());
    //Logger::GetInstance()->Log(clientIn);
}

void NITcpServer::clientDisconnected(NITcpSocket* socket)
{
	clientList.removeOne(socket);
}

void NITcpServer::SetClientLog(string in, string out)
{
    clientIn = in;
    clientOut = out;
}
