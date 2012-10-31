#include "NIEngine.h"
#include <math.h>
#include "TinyThread/tinythread.h"
#include "Utils.h"

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

	Logger::GetInstance()->Log("Starting NIEngine...");

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
	_userGenerator.GetSkeletonCap().SetSmoothing(0.5); // Set global smoothing factor. This will affect all joints. 0.5 is the default value.
	retVal = _niContext.StartGeneratingAll();
	CHECK_RC(retVal,"StartGenerating");

	_gestureGenerator.AddGesture("Click",NULL);
	//_gestureGenerator.AddGesture("Wave",NULL);

	THREADSTRUCT* param = new THREADSTRUCT;
	param->_this = this;
	thread niThread(StartThread,param);
	//niThread.join();
	niThread.detach();

	Logger::GetInstance()->Log("NIEngine Started.");
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
	if (gestureRec.timeStamp >= 0)
	{
		printf("%s hand gesture detected.%f %f \n",gestureRec.isRightHand ? "right":"left",leftDist,rightDist);
		_gestures.push_back(gestureRec);
	}

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
	
	double frequency = 30; // 30FPS by default
	double mincutoff = 1;  // min cutoff frequency
	double beta = 0.05;     // cutoff slope
	double dcutoff = 1;    // cutoff frequency for derivate

	Point3DFilter headFilter(frequency,mincutoff,beta,dcutoff);
	Point3DFilter leftHandFilter(frequency,mincutoff,beta,dcutoff);
	Point3DFilter rightHandFilter(frequency,mincutoff,beta,dcutoff);

	double lastTs = 0;  // last time stamp

	while (_shouldStop == FALSE)
	{
		_niContext.WaitOneUpdateAll(_userGenerator);
		numOfUsers = MAX_NUM_USERS;

		_userGenerator.GetUsers(users, numOfUsers);
		int numTracked=0;
		int userToPrint=-1;

		// simple user selection - choose the closest one
		XnUserID closestUserID = 0;  
		float smallestDist = 99999; // Math::Maxfloat?
		for(XnUInt16 i = 0; i < numOfUsers; i++)
		{
			if(_userGenerator.GetSkeletonCap().IsTracking(users[i]) == FALSE)
				continue;

			XnPoint3D com;
			_userGenerator.GetCoM(users[i],com);
			float dist = sqrtf(com.Z * com.Z + com.X * com.X); // calc distance in x-z plane
			if (dist < smallestDist)
			{
				smallestDist = dist;
				closestUserID = users[i];
			}
		}

		// Valid user ID starts from 1
		if(closestUserID > 0)
		{
			XnSkeletonJointPosition tmpPos;
			XnPoint3D filteredTmpPos;
			XnPoint3D tmpPosProjective;
			XnSkeletonJointOrientation tmpOrient;

			XnUserID curUserID = closestUserID;

			double currentTs = (double)_userGenerator.GetTimestamp() / 1000000;

			// Monitor frame dropping
			if (currentTs - lastTs > 0.034)
			{
				printf("%d frames dropped!\n",(int)abs(currentTs-lastTs / 0.033));
			}

			// Filter left hand
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(curUserID,XN_SKEL_LEFT_HAND,tmpPos);
			if (tmpPos.fConfidence > 0.5)
			{
				filteredTmpPos = leftHandFilter.filter(tmpPos.position,currentTs);
				//printf("time-diff %f pos-diff %f \n",currentTs - lastTs, PointDist(&tmpPos.position,&filteredTmpPos));
				_depthGenerator.ConvertRealWorldToProjective(1,&filteredTmpPos,&tmpPosProjective);
				_leftHand = tmpPos;
				_leftHand.position = filteredTmpPos;
				_leftHandPosProjective = tmpPosProjective;
			}
							
			// Filter right hand
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(curUserID,XN_SKEL_RIGHT_HAND,tmpPos);
			if (tmpPos.fConfidence > 0.5)
			{
				filteredTmpPos = rightHandFilter.filter(tmpPos.position,currentTs);
				//printf("time-diff %f pos-diff %f \n",currentTs - lastTs, PointDist(&tmpPos.position,&filteredTmpPos));
				_depthGenerator.ConvertRealWorldToProjective(1,&filteredTmpPos,&tmpPosProjective);
				_rightHand = tmpPos;
				_rightHand.position = filteredTmpPos;
				_rightHandPosProjective = tmpPosProjective;
			}

			// Filter head
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(curUserID,XN_SKEL_HEAD,tmpPos);
			_userGenerator.GetSkeletonCap().GetSkeletonJointOrientation(curUserID,XN_SKEL_HEAD,tmpOrient);
			if (tmpPos.fConfidence > 0.5)
			{
				filteredTmpPos = headFilter.filter(tmpPos.position,currentTs);
				_depthGenerator.ConvertRealWorldToProjective(1,&filteredTmpPos,&tmpPosProjective);
				_headPos = tmpPos;
				_headPos.position = filteredTmpPos;
				_headPosProjective = tmpPosProjective;

				_headOrient = tmpOrient; // not filtered
			}

			lastTs = currentTs;
			
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

	Logger::GetInstance()->Log("Stopping NIEngine...");
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