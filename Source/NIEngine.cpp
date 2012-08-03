#include "NIEngine.h"
#include "TinyThread/tinythread.h"

using namespace tthread;

#define OPENNI_CONFIG_PATH "../../../Data/openni.xml"  // cp-concern
#define MAX_NUM_USERS 15

NIEngine* NIEngine::_instance = NULL;

#define CHECK_RC(retVal, what)					    \
	if (retVal != XN_STATUS_OK)				    \
{								    \
	printf("%s failed: %s\n", what, xnGetStatusString(retVal));    \
	return retVal;						    \
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

}

NIEngine* NIEngine::GetInstance()
{
	if (_instance == NULL)
	{
		_instance = new NIEngine();
	}
	return _instance;
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


	if (_userGenerator.IsCapabilitySupported(XN_CAPABILITY_SKELETON) != TRUE)
	{
		printf("Supplied user generator doesn't support skeleton\n");
		return FALSE;
	}

	XnCallbackHandle hUserCallbacks, hCalibrationStart, hCalibrationComplete, hPoseDetected;
	retVal = _userGenerator.RegisterUserCallbacks(User_NewUser, User_LostUser, NULL, hUserCallbacks);
	CHECK_RC(retVal, "Register to user callbacks");

	retVal = _userGenerator.GetSkeletonCap().RegisterToCalibrationComplete(UserCalibration_CalibrationComplete, NULL, hCalibrationComplete);
	CHECK_RC(retVal, "Register to calibration complete");

	_userGenerator.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_UPPER);
	retVal = _niContext.StartGeneratingAll();
	CHECK_RC(retVal,"StartGenerating");

	THREADSTRUCT* param = new THREADSTRUCT;
	param->_this = this;
	thread niThread(StartThread,param);
	//niThread.join();
	niThread.detach();

	return true;
}

void NIEngine::NewUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie)
{
	_userGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
}

void NIEngine::LostUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie)
{

}

void NIEngine::CalibrationCompleted(xn::SkeletonCapability& capability, XnUserID nId, XnCalibrationStatus eStatus, void* pCookie)
{
	if (eStatus == XN_CALIBRATION_STATUS_OK)
	{
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
		XnSkeletonJointTransformation leftHand;
		XnSkeletonJointTransformation rightHand;

		for(XnUInt16 i = 0; i < numOfUsers; i++)
		{
			if(_userGenerator.GetSkeletonCap().IsTracking(users[i]) == FALSE)
				continue;

			_userGenerator.GetSkeletonCap().GetSkeletonJoint(users[i],XN_SKEL_LEFT_HAND,leftHand);
			_userGenerator.GetSkeletonCap().GetSkeletonJoint(users[i],XN_SKEL_RIGHT_HAND,rightHand);
			printf("user %d: left hand at (%6.2f,%6.2f,%6.2f); rigt hand at (%6.2f,%6.2f,%6.2f)\n",users[i],
				leftHand.position.position.X,
				leftHand.position.position.Y,
				leftHand.position.position.Z,
				rightHand.position.position.X,
				rightHand.position.position.Y,
				rightHand.position.position.Z);
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
	_shouldStop = TRUE;
	while(_running == TRUE){} // Wait until the thread ends.

	_niContext.StopGeneratingAll();

	_niScriptNode.Release();
	_depthGenerator.Release();
	_userGenerator.Release();
	_niContext.Release();

	return true;
}

// Helper functions
XnBool NIEngine::FileExists(const char *fn)
{
	XnBool exists;
	xnOSDoesFileExist(fn, &exists);
	return exists;
}