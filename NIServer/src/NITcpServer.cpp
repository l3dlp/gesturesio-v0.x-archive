
#include "NITcpServer.h"
#include <iostream>
#include "Utils.h"

using namespace std;

NITcpServer::NITcpServer(QObject *parent): QObject(parent)
{
	client = NULL;

    connect(&server,SIGNAL(newConnection()),this,SLOT(acceptConnection()));

    // Once command from client is received, forward it to the command parser.
    connect(this,SIGNAL(queryAvaiable(QString)),&parser,SLOT(parse(QString)));

    // Once command from client is parsed, send back the data client asks for.
    connect(&parser,SIGNAL(parsed(QString)),this,SLOT(startWrite(QString)));
}

void NITcpServer::Start(int port)
{
    server.listen(QHostAddress::Any, port);
}

void NITcpServer::Stop()
{
   server.close();
}

NITcpServer::~NITcpServer()
{
}

void NITcpServer::acceptConnection()
{
    client = server.nextPendingConnection();
	if (client != NULL)
	{
		connect(client,SIGNAL(readyRead()),this,SLOT(startRead()));
		connect(client,SIGNAL(disconnected()),this,SLOT(clientDisconnected()));
	}

    // Report to webserver that a new client connected.
    // Currently comment out as it takes too much time.
    //HttpRequest(clientIn.c_str());
    //Logger::GetInstance()->Log(clientIn);
}

void NITcpServer::startRead()
{
    char buffer[BUFFER_SIZE] = {0};
	if (client != NULL)
	{
		client->read(buffer,client->bytesAvailable());
		QString str(buffer);
		std::string stdstr(buffer);
		Logger::GetInstance()->Log("Cmd from client: " + stdstr);
		emit queryAvaiable(str);
	}
}

void NITcpServer::startWrite(QString echo)
{
	Logger::GetInstance()->Log("feedback to client:\n" + echo.toStdString());

    if(client != NULL)
	{
        client->write(echo.toAscii(),echo.length()+1); // +1 to include a null character
	}
    //qDebug("%d %s",echo.length(),echo.toAscii());
}

void NITcpServer::clientDisconnected()
{
	if (client != NULL)
	{
		// Don't use "delete client;" here, as it'll crash, let QTcpSocket to handle it.
		client->deleteLater(); 
	}
	
    // Report to webserver that the client is disconnected.
    //HttpRequest(clientOut.c_str());
}

void NITcpServer::SetClientLog(string in, string out)
{
    clientIn = in;
    clientOut = out;
}
