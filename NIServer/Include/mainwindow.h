#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ServerWorker.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow* pUi;
    QTimer* pTimer;
    QThread* pServerThread;
    ServerWorker* pServerWorker;

private slots:
    void startNIServer();
    void stopNIServer();

    void workerLicenseChecked(QString);
	void workerThreadRunning();

    void licenseExpired();
};

#endif // MAINWINDOW_H
