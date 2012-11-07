#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "NITcpClient.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
public slots:
    void connectServer();
    void disconnectServer();
    void sendCommand();

private:
    Ui::MainWindow *ui;
    NITcpClient client;
};

#endif // MAINWINDOW_H
