#include "NIEngine.h"
#include <math.h>
#include "TinyThread/tinythread.h"

using namespace tthread;

#define OPENNI_CONFIG_PATH "./Data/openni.xml"  // cp-concern
#define MAX_NUM_USERS 15

#define GESTURE_DIST_THRSH	100    // The threshold of distance between gesture detected hand position and given hand position

NIEngine* NIEngine::_instance = NULL;

#define CHECK_RC(retVal, what)					    \
	if (retVal != XN_STATUS_OK)				    \
{								    \
	printf("%s failed: %s\n", what, xnGetStatusString(retVal));    \
	return retVal;						    \
}

void XN_CALLBACK_TYPE Gesture_Recognized(xn::GestureGenerator& generator,const XnChar* strGesture, 
										const XnPoint3D* pIDPosition,const XnPoint3D* pEndPosition, void* pCookie)
{
	NIEngine::GetInstance()->GestureRecognized(generator,strGesture,pIDPosition,pEndPosition,pCookie);
}

void XN_CALLBACK_TYPE Gesture_Progress(xn::GestureGenerator& generator,const XnChar* strGesture, 
									   const XnPoint3D* pPosition, XnFloat fProgress, void* pCookie)
{
	
}

void XN_CALLBACK_TYPE User_NewUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie)
{
	NIEngine::GetInstance()->NewUser(generator,nId,pCookie);
}

void XN_CALLBACK_TYPE User_LostUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie)
{
	NIEngine::GetInstance()->LostUser(generator,nId,pCookie);
}

void XN_CALLBACK_TYPE UserCalibration_CalibrationComplete(xn::SkeletonCapability& capability, XnUserID nId, XnCalibrationStatus eStatus, void* pCookie)
{
	NIEngine::GetInstance()->CalibrationCompleted(capability,nId,eStatus,pCookie);
}

NIEngine::NIEngine()
{
	_running = FALSE;
	memset(&_leftHand,0,sizeof(_leftHand));
	memset(&_rightHand,0,sizeof(_rightHand));
	memset(&_leftHandPosProjective,0,sizeof(_leftHandPosProjective));
	memset(&_rightHandPosProjective,0,sizeof(_rightHandPosProjective));
	memset(&_headPosProjective,0,sizeof(_headPosProjective));
}

NIEngine* NIEngine::GetInstance()
{
	if (_instance == NULL)
	{
		_instance = new NIEngine();
	}
	return _instance;
}

XnBool NIEngine::IsRunning()
{
	return _running;
}

XnBool NIEngine::Start()
{
	XnStatus retVal = XN_STATUS_OK;
	xn::EnumerationErrors errors;

	// cp-concern: TRUE
	if (FileExists(OPENNI_CONFIG_PATH) != TRUE)
	{
		printf("Could not find OpenNI configuration file from %s\n",OPENNI_CONFIG_PATH);
		return false;
	}

	printf("Starting NIEngine...\n");

	retVal = _niContext.InitFromXmlFile(OPENNI_CONFIG_PATH,_niScriptNode,&errors);
	if (retVal == XN_STATUS_NO_NODE_PRESENT)
	{
		XnChar strError[1024];
		errors.ToString(strError, 1024);
		printf("%s\n", strError);
		return (retVal);
	}
	else if (retVal != XN_STATUS_OK)
	{
		printf("Open failed: %s\n", xnGetStatusString(retVal));
		return (retVal);
	}

	retVal = _niContext.FindExistingNode(XN_NODE_TYPE_DEPTH,_depthGenerator);
	CHECK_RC(retVal,"No depth generator");

	retVal = _niContext.FindExistingNode(XN_NODE_TYPE_USER,_userGenerator);
	CHECK_RC(retVal,"No user generator");

	retVal = _niContext.FindExistingNode(XN_NODE_TYPE_GESTURE,_gestureGenerator);
	CHECK_RC(retVal,"No gesture generator");

	if (_userGenerator.IsCapabilitySupported(XN_CAPABILITY_SKELETON) != TRUE)
	{
		printf("Supplied user generator doesn't support skeleton\n");
		return FALSE;
	}

	XnCallbackHandle hGesture, hUserCallbacks, hCalibrationStart, hCalibrationComplete, hPoseDetected;

	retVal = _gestureGenerator.RegisterGestureCallbacks(Gesture_Recognized,Gesture_Progress,NULL,hGesture);
	CHECK_RC(retVal,"Register to gesture callbacks");

	retVal = _userGenerator.RegisterUserCallbacks(User_NewUser, User_LostUser, NULL, hUserCallbacks);
	CHECK_RC(retVal, "Register to user callbacks");

	retVal = _userGenerator.GetSkeletonCap().RegisterToCalibrationComplete(UserCalibration_CalibrationComplete, NULL, hCalibrationComplete);
	CHECK_RC(retVal, "Register to calibration complete");

	_userGenerator.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_UPPER);
	retVal = _niContext.StartGeneratingAll();
	CHECK_RC(retVal,"StartGenerating");

	_gestureGenerator.AddGesture("Click",NULL);
	_gestureGenerator.AddGesture("Wave",NULL);

	THREADSTRUCT* param = new THREADSTRUCT;
	param->_this = this;
	thread niThread(StartThread,param);
	//niThread.join();
	niThread.detach();

	printf("NIEngine Started.\n");
	return true;
}

float NIEngine::PointDist(const XnPoint3D* p1, const XnPoint3D* p2)
{
	return sqrt(pow(p1->X - p2->X,2) + pow(p1->Y - p2->Y,2) + pow(p1->Z - p2->Z,2));
}

void NIEngine::GestureRecognized(xn::GestureGenerator& generator,const XnChar* strGesture, 
								 const XnPoint3D* pIDPosition,const XnPoint3D* pEndPosition, void* pCookie)
{
	// Identify which hand holds this gesture
	float leftDist = PointDist(&_leftHand.position,pEndPosition);
	float rightDist = PointDist(&_rightHand.position,pEndPosition);

	GESTURERECORD gestureRec;
	gestureRec.name = strGesture;

	gestureRec.timeStamp = 0; //TODO: add current timestamp

	if (rightDist < GESTURE_DIST_THRSH)
	{
		gestureRec.isRightHand = true;
	}
	else if (leftDist < GESTURE_DIST_THRSH)
	{
		gestureRec.isRightHand = false;
	}
	else
	{
		gestureRec.timeStamp = -1.0f; //invalid
	}
	printf("%d hand detected.%f %f \n",gestureRec.isRightHand,leftDist,rightDist);

	_gestures.push_back(gestureRec);

	//printf("idp %f %f %f\n",pIDPosition->X,pIDPosition->Y,pIDPosition->Z);
	//printf("enp %f %f %f\n",pEndPosition->X,pEndPosition->Y,pEndPosition->Z);
	//printf("rh %f %f %f\n",_rightHand.position.X,_rightHand.position.Y,_rightHand.position.Z);
	//printf("pos dis %f\n",PointDist(pIDPosition,pEndPosition));
	//printf("Gesture %s detected ld %f rd %f\n",strGesture,leftDist,rightDist);
	
}

void NIEngine::NewUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie)
{
	printf("User %d found \n",nId);
	_userGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
}

void NIEngine::LostUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie)
{
	printf("User %d lost \n",nId);
}

void NIEngine::CalibrationCompleted(xn::SkeletonCapability& capability, XnUserID nId, XnCalibrationStatus eStatus, void* pCookie)
{
	if (eStatus == XN_CALIBRATION_STATUS_OK)
	{
		printf("User %d calibrated.\n",nId);
		// Calibration succeeded
		_userGenerator.GetSkeletonCap().StartTracking(nId);
	}
}

void NIEngine::ProcessData()
{
	_shouldStop = FALSE;
	_running = TRUE;
	XnUInt16 numOfUsers;
	XnUserID users[MAX_NUM_USERS];
	
	while (_shouldStop == FALSE)
	{
		_niContext.WaitOneUpdateAll(_userGenerator);
		numOfUsers = MAX_NUM_USERS;

		_userGenerator.GetUsers(users, numOfUsers);
		int numTracked=0;
		int userToPrint=-1;

		for(XnUInt16 i = 0; i < numOfUsers; i++)
		{
			if(_userGenerator.GetSkeletonCap().IsTracking(users[i]) == FALSE)
				continue;

			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(users[i],XN_SKEL_LEFT_HAND,_leftHand);
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(users[i],XN_SKEL_RIGHT_HAND,_rightHand);
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(users[i],XN_SKEL_HEAD,_headPos);
			_userGenerator.GetSkeletonCap().GetSkeletonJointOrientation(users[i],XN_SKEL_HEAD,_headOrient);
			_depthGenerator.ConvertRealWorldToProjective(1,&_leftHand.position,&_leftHandPosProjective);
			_depthGenerator.ConvertRealWorldToProjective(1,&_rightHand.position,&_rightHandPosProjective);
			_depthGenerator.ConvertRealWorldToProjective(1,&_headPos.position,&_headPosProjective);

			//printf("user %d: left hand at (%6.2f,%6.2f,%6.2f); rigt hand at (%6.2f,%6.2f,%6.2f)\n",users[i],
			//	_leftHand.position.X,
			//	_leftHand.position.Y,
			//	_leftHand.position.Z,
			//	_rightHand.position.X,
			//	_rightHand.position.Y,
			//	_rightHand.position.Z);
		}
	}
	_running = FALSE;

}

void NIEngine::StartThread(void* arg)
{
	THREADSTRUCT* ts = (THREADSTRUCT*)arg;
	ts->_this->ProcessData();
}

XnBool NIEngine::Stop()
{
	if (_running == FALSE)
	{
		return TRUE;
	}
	
	_shouldStop = TRUE;
	printf("Stopping NIEngine...\n");

	// TODO: find proper way to release the resource and destruct the instance.

	//_niContext.StopGeneratingAll();

	//_niScriptNode.Release();
	//_depthGenerator.Release();
	//_userGenerator.Release();
	//_niContext.Release();

	return true;
}

// Helper functions
XnBool NIEngine::FileExists(const char *fn)
{
	XnBool exists;
	xnOSDoesFileExist(fn, &exists);
	return exists;
}

XnSkeletonJointPosition NIEngine::GetLeftHandPos()
{
	return _leftHand;
}

XnSkeletonJointPosition NIEngine::GetRightHandPos()
{
	return _rightHand;
}

XnPoint3D NIEngine::GetLeftHandPosProjective()
{
	return _leftHandPosProjective;
}

XnPoint3D NIEngine::GetRightHandPosProjective()
{
	return _rightHandPosProjective;
}

XnPoint3D NIEngine::GetHeadPosProjective()
{
	return _headPosProjective;
}

GESTURERECORD NIEngine::GetGesture()
{
	GESTURERECORD gestureRec;
	
	if (_gestures.empty() == false)
	{
		gestureRec = _gestures.back();
		_gestures.pop_back();
	}
	else
	{
		gestureRec.timeStamp = -1.0f;
	}
	return gestureRec;
}