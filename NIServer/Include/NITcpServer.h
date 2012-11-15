#ifndef NITCPSERVER_H
#define NITCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include "CmdParser.h"

class NITcpServer: public QObject
{
    Q_OBJECT

public:
    NITcpServer(QObject* parent = 0);
    ~NITcpServer();

    void Start(int port);
    void Stop();
    void SetClientLog(std::string, std::string);

private slots:
    void acceptConnection();
    void startRead();
    void startWrite(QString);
    void clientDisconnected();

signals:
    void queryAvaiable(QString cmd);

private:
    QTcpServer server;
    QTcpSocket* client;
    CmdParser parser;
    std::string clientIn;
    std::string clientOut;

    static const int BUFFER_SIZE = 1024;
};



#endif // NITCPSERVER_H
