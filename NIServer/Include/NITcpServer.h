#ifndef NITCPSERVER_H
#define NITCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>


class NITcpServer: public QObject
{
    Q_OBJECT

public:
    NITcpServer(QObject* parent = 0);
    ~NITcpServer();

public slots:
    void acceptConnection();
    void startRead();

private:
    QTcpServer server;
    QTcpSocket* client;
    static const int BUFFER_SIZE = 1024;
};



#endif // NITCPSERVER_H
