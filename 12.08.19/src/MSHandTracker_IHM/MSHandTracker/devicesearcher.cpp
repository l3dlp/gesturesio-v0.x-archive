#include "devicesearcher.h"
#include <QThread>





#define MAX_CHAR_SIZE 1024
#define MS_DEBUG_MSG(args) printf("[DEBUG at %s:%d] : ", __FUNCTION__ ,__LINE__);printf(args);printf("\n");
#define OpenNIErrorCheck(err,what) if(err != XN_STATUS_OK){MS_DEBUG_MSG(what) MS_DEBUG_MSG(	xnGetStatusString(err))}


bool gSessionStarted;
bool gRun;
XnPoint3D gPos;
XnPoint3D gPosIntermediate;
XnPoint3D gPos2D;
XnStatus gOpenNIChecker;
xn::DepthGenerator gDepthGenerator;
xn::HandsGenerator gHandGenerator;

void XN_CALLBACK_TYPE HandCreation (xn::HandsGenerator &generator, XnUserID user, const XnPoint3D *pPosition, XnFloat fTime, void *pCookie)
{
    gPos = *pPosition;
    gDepthGenerator.ConvertRealWorldToProjective(1, &gPos, &gPos2D);
}

void XN_CALLBACK_TYPE HandDestruction (xn::HandsGenerator &generator, XnUserID user, XnFloat fTime, void *pCookie)
{
    gSessionStarted = false;
}

void XN_CALLBACK_TYPE HandUpdating (xn::HandsGenerator &generator, XnUserID user, const XnPoint3D *pPosition, XnFloat fTime, void *pCookie)
{
    //printf("Updating hand !!\n");
    gPos = *pPosition;
    gDepthGenerator.ConvertRealWorldToProjective(1, &gPos, &gPos2D);

    // ******************************************************************
    // *	Envoyer pPos.X, pPos.Y et pPos.X pour les coordonnées 3D	*
    // *	Envoyer pPos2D.X et pPos2D.Y pour les coordonnées images	*
    // ******************************************************************
}


void XN_CALLBACK_TYPE Recognized (xn::GestureGenerator &generator, const XnChar *strGesture, const XnPoint3D *pIDPosition, const XnPoint3D *pEndPosition, void *pCookie)
{
    if(!gSessionStarted)
    {
        //printf("Gesture recognized : %s\n", strGesture);
        gSessionStarted = true;
        gOpenNIChecker = gHandGenerator.StartTracking(*pEndPosition);
        OpenNIErrorCheck(gOpenNIChecker,"StartTracking");
    }
}

void XN_CALLBACK_TYPE Progress (xn::GestureGenerator &generator, const XnChar *strGesture, const XnPoint3D *pPosition, XnFloat fProgress, void *pCookie)
{
    gPosIntermediate = *pPosition;
    gDepthGenerator.ConvertRealWorldToProjective(1, &gPosIntermediate, &gPos2D);
}



DeviceSearcher::DeviceSearcher(QObject *parent) :
    QObject(parent)
{
    purge();
}

DeviceSearcher::~DeviceSearcher()
{
    purge();
}

void DeviceSearcher::purge()
{
    mSearching = true;
    mRun = false;
    mContext.Release();
    gSessionStarted = false;
    gOpenNIChecker = XN_STATUS_OK;
    gPos.X = -1.f;
    gPos.Y = -1.f;
    gPos.Z = -1.f;
    gPos2D.X = -1.f;
    gPos2D.Y = -1.f;
    gPos2D.Z = 0.f;
    gPosIntermediate.X = -1.f;
    gPosIntermediate.Y = -1.f;
    gPosIntermediate.Z = -1.f;
}

bool DeviceSearcher::CheckOpenNIStatus()
{
    return (gOpenNIChecker == XN_STATUS_OK)?true:false;
}

void DeviceSearcher::searchDevice()
{

    while (mSearching)
    {
        #ifdef _WINDOWS_
                    Sleep(UPDATE_PERIOD_MS);
        #else
                    usleep(UPDATE_PERIOD_MS * 1000);
        #endif
        mContext.Init();
        if(gDepthGenerator.Create(mContext) == XN_STATUS_OK)
        {
            mSearching = false;
            mRun = true;

            emit deviceFound();
        }
        else
        {
            mContext.Release();
        }
    }

    gOpenNIChecker = gHandGenerator.Create(mContext);
    OpenNIErrorCheck(gOpenNIChecker,"hand generator creation");

    gOpenNIChecker = mGestureGenerator.Create(mContext);
    OpenNIErrorCheck(gOpenNIChecker, "Gesture generator creation");

    gOpenNIChecker = mGestureGenerator.AddGesture("Click", NULL);//&box);
    OpenNIErrorCheck(gOpenNIChecker, "Add Click gesture");

    gOpenNIChecker = mGestureGenerator.AddGesture("Wave", NULL);//&box);
    OpenNIErrorCheck(gOpenNIChecker, "Add Wave gesture");


    XnCallbackHandle hGestureCallbackHandle, hHandCallbackHandle;


    gOpenNIChecker = mContext.StartGeneratingAll();
    OpenNIErrorCheck(gOpenNIChecker, "Start generating all");

    gOpenNIChecker = mGestureGenerator.RegisterGestureCallbacks(Recognized, Progress, NULL, hGestureCallbackHandle);
    OpenNIErrorCheck(gOpenNIChecker, "register to gesture callbacks recognized and progress");

    gOpenNIChecker = gHandGenerator.RegisterHandCallbacks(HandCreation, HandUpdating, HandDestruction, NULL, hHandCallbackHandle);
    OpenNIErrorCheck(gOpenNIChecker, "register to hand callbacks create, destroy and update");

    gOpenNIChecker = gHandGenerator.SetSmoothing(0.1);
    OpenNIErrorCheck(gOpenNIChecker, "set hand tracker smoothing");

    mTracking = false;
    while(mRun)
    {
        gOpenNIChecker = mContext.WaitOneUpdateAll(gDepthGenerator);
        if(!CheckOpenNIStatus())
        {
            emit noDevice();
            purge();
        }
        else if(!mTracking && gSessionStarted)
        {
            mTracking = true;
            emit trackingStarted();
        }
        else if(mTracking && !gSessionStarted)
        {
            mTracking = false;
            emit trackingStopped();
        }
    }

    if(mTracking)
    {
        mTracking = false;
        emit trackingStopped();
    }

}

void DeviceSearcher::initiateSearch()
{
    purge();
    searchDevice();
    purge();

}
