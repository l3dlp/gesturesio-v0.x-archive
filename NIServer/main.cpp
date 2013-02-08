#include <QtGui/QApplication>
#include "mainwindow.h"
#include "Utils.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
	Logger::GetInstance()->Log("");
	Logger::GetInstance()->Log("");
	Logger::GetInstance()->Log("NI Server launched");

    MainWindow w;
    w.show();

    return a.exec();
}
