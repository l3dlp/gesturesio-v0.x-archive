#ifndef NITCPSOCKET_H
#define NITCPSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include "CmdParser.h"

class NITcpSocket: public QObject
{
    Q_OBJECT

	QTcpSocket* Socket;

public:
    NITcpSocket(QTcpSocket* socket);
    virtual ~NITcpSocket();

private slots:
	void readyRead();
	void readyWrite(QString data);
	void disconnected();

signals:
	void parseCommand(QString cmd);
	void removeMe(NITcpSocket* socket);

private:
	CmdParser parser;
	static const int BUFFER_SIZE = 1024;
};

#endif // NITCPSOCKET_H
