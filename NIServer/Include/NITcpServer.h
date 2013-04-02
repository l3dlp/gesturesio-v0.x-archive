#ifndef NITCPSERVER_H
#define NITCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include "NITcpSocket.h"

//! Qt based class for Tcp service
/*! It uses Qt's QTcpServer and QTcpSocket to:
 * - listen to dedicated port.
 * - handle incoming connection.
 * - parse client's command.
 * - send result back to client.
 */
class NITcpServer: public QObject
{
    Q_OBJECT

public:
    NITcpServer(QObject* parent = 0);
    ~NITcpServer();

    void Start(int port);  //!< Start Tcp server.
    void Stop();           //!< Stop Tcp server.
    void SetClientLog(std::string, std::string);  //!< Pass client's log information which is used to report to web server.

private slots:
    void acceptConnection();
    void clientDisconnected(NITcpSocket* socket);

private:
    QTcpServer server;
	QList<NITcpSocket*> clientList;

    std::string clientIn;
    std::string clientOut;
};



#endif // NITCPSERVER_H
