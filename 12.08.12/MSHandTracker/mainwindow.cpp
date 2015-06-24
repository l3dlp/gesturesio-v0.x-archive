#include <mainwindow.h>
#include <ui_mainwindow.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QVBoxLayout *vLayout = new QVBoxLayout();
    QHBoxLayout *hLayout = new QHBoxLayout();

    mImageDisplay = new ImageDisplay(NULL, "C:/Users/Mirou/Desktop/ms.jpg");
    mSensorConnected = new Indicator(NULL, "PrimeSense device connected");
    mTrackingEnabled = new Indicator(NULL, "Hand tracker enabled");



    mImageDisplay->adjustSize();

    QWidget *buttonWidget = new QWidget();
    hLayout->addWidget(mSensorConnected);
    hLayout->addWidget(mTrackingEnabled);
    buttonWidget->setLayout(hLayout);

    vLayout->addWidget(mImageDisplay);
    vLayout->addWidget(buttonWidget);


    this->centralWidget()->setLayout(vLayout);
    this->centralWidget()->setStyleSheet("QWidget { background-color : white}");

    this->resize(mImageDisplay->width(), mImageDisplay->height() + mTrackingEnabled->minimumHeight());
    this->setMaximumSize(this->size());


    mDeviceSearcher = NULL;
    mThread = NULL;

    searchForDevice();


}

MainWindow::~MainWindow()
{
    delete mImageDisplay;
    delete mSensorConnected;
    delete mTrackingEnabled;
    delete mDeviceSearcher;
    delete ui;
}

void MainWindow::searchForDevice()
{
    if(mThread)
    {
        delete mThread;
    }

    mThread = new QThread();

    if(mDeviceSearcher)
    {
        delete mDeviceSearcher;
    }

#ifdef _WINDOWS_
    Sleep(UPDATE_PERIOD_MS);
#else
    usleep(UPDATE_PERIOD_MS * 1000);
#endif

    mDeviceSearcher = new DeviceSearcher();

    connect(mDeviceSearcher, SIGNAL(noDevice()), mSensorConnected, SLOT(changeStatus()));
    connect(mDeviceSearcher, SIGNAL(noDevice()), this, SLOT(searchForDevice()));
    connect(mDeviceSearcher, SIGNAL(deviceFound()), mSensorConnected, SLOT(changeStatus()));


    mDeviceSearcher->moveToThread(mThread);

    connect(mThread, SIGNAL(started()), mDeviceSearcher, SLOT(initiateSearch()));
    mThread->start();
}
