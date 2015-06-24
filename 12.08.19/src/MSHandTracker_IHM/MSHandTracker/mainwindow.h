#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include "imagedisplay.h"
#include "indicator.h"
#include "devicesearcher.h"


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
    Ui::MainWindow *ui;
    ImageDisplay *mImageDisplay;
    Indicator *mSensorConnected;
    Indicator *mTrackingEnabled;
    DeviceSearcher *mDeviceSearcher;
    QThread *mThread;

public slots:
    void searchForDevice();

signals:
    //void needADevice();
    //void trackingStatusChange();
};

#endif // MAINWINDOW_H
