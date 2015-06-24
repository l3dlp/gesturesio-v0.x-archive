#ifndef DEVICESEARCHER_H
#define DEVICESEARCHER_H

#include <QObject>
#include <XnCppWrapper.h>
#include <XnVNite.h>		// NITE headers

#define UPDATE_PERIOD_MS 1000

class DeviceSearcher : public QObject
{
    Q_OBJECT
public:
    explicit DeviceSearcher(QObject *parent = 0);
    virtual ~DeviceSearcher();
    

private:
    bool mSearching;
    bool mRun;
    bool mTracking;
    xn::Context mContext;
    xn::GestureGenerator mGestureGenerator;

    bool CheckOpenNIStatus();
    void searchDevice();

    void purge();

signals:
    void noDevice();
    void deviceFound();
    void trackingStarted();
    void trackingStopped();
    
public slots:
    void initiateSearch();

    
};

#endif // DEVICESEARCHER_H
