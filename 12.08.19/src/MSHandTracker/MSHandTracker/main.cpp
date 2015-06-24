#include <XnOpenNI.h>		// OpenNI header
#include <XnCppWrapper.h>	// OpenNI c++ Wrapper
#include "macros.h"			// Some macros for display, OpenNI error check ...
#include <XnVNite.h>		// NITE headers
#include <opencv.hpp>		// OpenCV Headers
#include <pthread.h>		// pthread Headers



bool gSessionStarted = false;
bool gRun;
XnPoint3D gPos;
XnPoint3D gPosIntermediate;
XnPoint3D gPos2D;
xn::DepthGenerator mDepthGenerator;
xn::HandsGenerator mHandGenerator;
xn::GestureGenerator mGestureGenerator;
XnCallbackHandle hGestureIntermediateStageCompleted, hGestureProgress, hGestureReadyForNextIntermediateStage, hHandCallBack;
XnStatus mOpenNIChecker = XN_STATUS_OK;

#ifndef min
#define min(a,b) (a<b)?a:b
#endif

void XN_CALLBACK_TYPE Recognized (xn::GestureGenerator &generator, const XnChar *strGesture, const XnPoint3D *pIDPosition, const XnPoint3D *pEndPosition, void *pCookie)
{
	if(!gSessionStarted)
	{
		//printf("Gesture recognized : %s\n", strGesture);
		gSessionStarted = true;
		mOpenNIChecker = mHandGenerator.StartTracking(*pEndPosition);
		OpenNIErrorCheck(mOpenNIChecker,"StartTracking");
	}
}

void XN_CALLBACK_TYPE Progress (xn::GestureGenerator &generator, const XnChar *strGesture, const XnPoint3D *pPosition, XnFloat fProgress, void *pCookie)
{
	gPosIntermediate = *pPosition;
	mDepthGenerator.ConvertRealWorldToProjective(1, &gPosIntermediate, &gPos2D);
}




void XN_CALLBACK_TYPE HandCreation (xn::HandsGenerator &generator, XnUserID user, const XnPoint3D *pPosition, XnFloat fTime, void *pCookie)
{
	gPos = *pPosition;
	mDepthGenerator.ConvertRealWorldToProjective(1, &gPos, &gPos2D);
}

void XN_CALLBACK_TYPE HandDestruction (xn::HandsGenerator &generator, XnUserID user, XnFloat fTime, void *pCookie)
{
	gSessionStarted = false;
}

void XN_CALLBACK_TYPE HandUpdating (xn::HandsGenerator &generator, XnUserID user, const XnPoint3D *pPosition, XnFloat fTime, void *pCookie)
{
	//printf("Updating hand !!\n");
	gPos = *pPosition;
	mDepthGenerator.ConvertRealWorldToProjective(1, &gPos, &gPos2D);

	// ******************************************************************
	// *	Envoyer pPos.X, pPos.Y et pPos.X pour les coordonnées 3D	*
	// *	Envoyer pPos2D.X et pPos2D.Y pour les coordonnées images	*
	// ******************************************************************
}



IplImage *image;


void* displayDebug(void* data)
{
	cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE); 
	cvMoveWindow("mainWin", 320, 240);
	xn::DepthMetaData dmd;
	mDepthGenerator.GetMetaData(dmd);
	int sleep_period_ms = 30;
	if(dmd.FPS() > 0)
	{
		int sleep_period_ms = (int)(1000.f / (float)(dmd.FPS()));
	}

	while (gRun)
	{
		cvShowImage("mainWin", image);
		cvWaitKey(sleep_period_ms);
	}


	return NULL;
}


int main(int argc, char* argv[])
{
	xn::Context mContext;
	mOpenNIChecker = mContext.Init();
	OpenNIErrorCheck(mOpenNIChecker,"Context initialisation");

	mOpenNIChecker = mDepthGenerator.Create(mContext);
	OpenNIErrorCheck(mOpenNIChecker,"depth generator creation");
	if(!mDepthGenerator.GetHandle())
	{
		printf("Maybe you forgot to plugg your PrimeSense device ??\n");
		return -1;
	}

	
	mOpenNIChecker = mHandGenerator.Create(mContext);
	OpenNIErrorCheck(mOpenNIChecker,"hand generator creation");
	
	mOpenNIChecker = mGestureGenerator.Create(mContext);
	OpenNIErrorCheck(mOpenNIChecker, "Gesture generator creation");

	mOpenNIChecker = mGestureGenerator.AddGesture("Click", NULL);//&box);
	OpenNIErrorCheck(mOpenNIChecker, "Add Click gesture");

	mOpenNIChecker = mGestureGenerator.AddGesture("Wave", NULL);//&box);
	OpenNIErrorCheck(mOpenNIChecker, "Add Wave gesture");


	XnCallbackHandle hGestureCallbackHandle, hHandCallbackHandle;


	mOpenNIChecker = mContext.StartGeneratingAll();
	OpenNIErrorCheck(mOpenNIChecker, "Start generating all");

	gRun = true;
	int count = 0;
	xn::DepthMetaData dmd;
	mDepthGenerator.GetMetaData(dmd);
	const XnDepthPixel *depthMap;

	mContext.SetGlobalMirror(true);

	image = NULL;
	bool registred = true;

	mOpenNIChecker = mGestureGenerator.RegisterGestureCallbacks(Recognized, Progress, NULL, hGestureCallbackHandle);
	OpenNIErrorCheck(mOpenNIChecker, "register to gesture callbacks recognized and progress");

	mOpenNIChecker = mHandGenerator.RegisterHandCallbacks(HandCreation, HandUpdating, HandDestruction, NULL, hHandCallbackHandle);
	OpenNIErrorCheck(mOpenNIChecker, "register to hand callbacks create, destroy and update");

	// Set smoothing level if first argument was set
	float smoothing = 0.1f;
	if(argc > 1)
	{
		smoothing = atof(argv[1]);
	}

	mHandGenerator.SetSmoothing(smoothing);

	mContext.WaitOneUpdateAll(mDepthGenerator);

	pthread_t thread;
	depthMap = mDepthGenerator.GetDepthMap();
	if(depthMap)
	{
		if(!image)
		{
			image = cvCreateImage(cvSize(dmd.FullXRes(), dmd.FullYRes()), IPL_DEPTH_8U, 1);
			cvZero(image);			
			pthread_create(&thread, NULL, displayDebug, NULL);
		}
		cvZero(image);

		depthMap = mDepthGenerator.GetDepthMap();
		for(unsigned int i = 0 ; i < dmd.FullYRes() ; i++)
		{
			for(unsigned int j = 0 ; j < dmd.FullXRes() ; j++)
			{
				image->imageData[i * dmd.FullXRes() +j] = min(depthMap[i * dmd.FullXRes() +j]/10, 255);
			}
		}
	}
	else
	{
		MS_DEBUG_MSG("ERROR : No Depth map! Abort !!");
		return -1;
	}


	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0);

	while((mOpenNIChecker = mContext.WaitOneUpdateAll(mDepthGenerator)) == XN_STATUS_OK && gRun)
	{
		
		depthMap = mDepthGenerator.GetDepthMap();
		if(depthMap)
		{
			cvZero(image);
			for(unsigned int i = 0 ; i < dmd.FullYRes() ; i++)
			{
				for(unsigned int j = 0 ; j < dmd.FullXRes() ; j++)
				{
					
					image->imageData[i * dmd.FullXRes() +j] = min(depthMap[i * dmd.FullXRes() +j]/10, 255);
				}
			}
		}

		//mSessionManager->Update(&mContext);
		if(gSessionStarted)
		{			
			//printf("IN SESSION - %g %g %g\n", gPos.X, gPos.Y, gPos.Z);

			CvPoint seed = cvPoint((int)gPos2D.X, (int)gPos2D.Y);
			cvCircle(image, seed, image->height/4, cvScalar(255.));
		}
		else
		{
			cvPutText(image, "No tracking", cvPoint(image->width/2, image->height/2), &font, cvScalar(255.));
			mOpenNIChecker = mHandGenerator.StopTrackingAll();
			OpenNIErrorCheck(mOpenNIChecker, "stop tracking all");
		}
		count++;
	}

	OpenNIErrorCheck(mOpenNIChecker, "Waiting depth and updating all");
	
	mDepthGenerator.Release();
	mHandGenerator.Release();

	mContext.Release();

	return 0;
}