#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "initworker.h"

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
    QThread* pInitThread;
    InitWorker* pInitWorker;

private slots:
    void startNIServer();
    void stopNIServer();
    void licenseExpired();
    void LicenseChecked(QString);
    void initFinished();
    void initFailed(QString);
};

#endif // MAINWINDOW_H
