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
    

private:
    bool mSearching;
    bool mRun;
    xn::Context mContext;
    xn::DepthGenerator mDepthGenerator;
    xn::HandsGenerator mHandGenerator;
    xn::GestureGenerator mGestureGenerator;
    XnStatus mOpenNIChecker;

    bool CheckOpenNIStatus();
    void searchDevice();


signals:
    void noDevice();
    void deviceFound();
    
public slots:
    void initiateSearch();

    
};

#endif // DEVICESEARCHER_H
