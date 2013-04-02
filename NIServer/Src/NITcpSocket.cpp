
#include "NITcpSocket.h"
#include "Utils.h"

using namespace std;

NITcpSocket::NITcpSocket(QTcpSocket* socket): Socket(socket)
{	
	connect(Socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
	connect(Socket,SIGNAL(disconnected()),this,SLOT(disconnected()));

	// Once command from client is received, forward it to the command parser.
	connect(this,SIGNAL(parseCommand(QString)),&parser,SLOT(parse(QString)));
	// Once command from client is parsed, send back the data client asks for.
	connect(&parser,SIGNAL(parsed(QString)),this,SLOT(readyWrite(QString)));
}

NITcpSocket::~NITcpSocket()
{
}

void NITcpSocket::readyRead()
{
	char buffer[BUFFER_SIZE] = {0};
	Socket->read(buffer,Socket->bytesAvailable());

	QString str(buffer);
	std::string stdstr(buffer);

	emit parseCommand(str);
	
	Logger::GetInstance()->Log("Command from " + Socket->peerName().toStdString() + ": " + stdstr);
}

void NITcpSocket::readyWrite(QString data)
{
	Socket->write(data.toAscii(),data.length()+1); // +1 to include a null character

	Logger::GetInstance()->Log("feedback to " + Socket->peerName().toStdString() + ":\n" + data.toStdString());
}

void NITcpSocket::disconnected()
{
	// Don't use "delete ;" here, as it'll crash, let QTcpSocket to handle it.
	Socket->deleteLater(); 
	
	emit removeMe(this);

	// Report to webserver that the client is disconnected.
	//HttpRequest(clientOut.c_str());
}
