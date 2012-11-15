#include <QtGui/QApplication>
#include "mainwindow.h"
#include "NITcpServer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //NITcpServer g_tcpServer;
    MainWindow w;
    w.show();
    
    return a.exec();
}
