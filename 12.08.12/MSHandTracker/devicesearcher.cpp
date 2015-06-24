#include "devicesearcher.h"
#include <QThread>



DeviceSearcher::DeviceSearcher(QObject *parent) :
    QObject(parent)
{
    mSearching = true;
    mRun = false;
    mContext.Release();
}

bool DeviceSearcher::CheckOpenNIStatus()
{
    return (mOpenNIChecker == XN_STATUS_OK)?true:false;
}

void DeviceSearcher::searchDevice()
{
    while (mSearching)
    {
        mContext.Init();
        if(mDepthGenerator.Create(mContext) == XN_STATUS_OK)
        {
            mSearching = false;
            mRun = true;

            emit deviceFound();
        }
        else
        {
            mContext.Release();
#ifdef _WINDOWS_
            Sleep(UPDATE_PERIOD_MS);
#else
            usleep(UPDATE_PERIOD_MS * 1000);
#endif
        }
    }

    mDepthGenerator.StartGenerating();


    while(mRun)
    {
        mOpenNIChecker = mContext.WaitOneUpdateAll(mDepthGenerator);
        if(!CheckOpenNIStatus())
        {
            mRun = false;
            mSearching = true;
            mContext.Release();
            emit noDevice();
        }
    }

}

void DeviceSearcher::initiateSearch()
{
    searchDevice();

}
