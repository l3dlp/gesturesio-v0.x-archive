#include <XnCppWrapper.h>

class NIEngine
{
private:
	static NIEngine* _instance;
	xn::Context _niContext;
	xn::ScriptNode _niScriptNode;
	xn::DepthGenerator _depthGenerator;
	xn::UserGenerator _userGenerator;
	XnBool _shouldStop;
	XnBool _running;
	XnSkeletonJointPosition _leftHand;
	XnSkeletonJointPosition _rightHand;
	XnSkeletonJointPosition _headPos;
	XnSkeletonJointOrientation _headOrient;
	XnPoint3D _leftHandPosProjective;
	XnPoint3D _rightHandPosProjective;
	XnPoint3D _headPosProjective;

public:
	static NIEngine* GetInstance();
	XnBool Start();
	XnBool Stop();
	void NewUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie);
	void LostUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie);
	void CalibrationCompleted(xn::SkeletonCapability& capability, XnUserID nId, XnCalibrationStatus eStatus, void* pCookie);
	~NIEngine();
	XnBool IsRunning();
	XnSkeletonJointPosition GetLeftHandPos();
	XnSkeletonJointPosition GetRightHandPos();
	XnPoint3D GetLeftHandPosProjective();
	XnPoint3D GetRightHandPosProjective();
	XnPoint3D GetHeadPosProjective();

private:
	NIEngine();
	XnBool FileExists(const char *fn);
	static void StartThread(void* arg);
	void ProcessData();
};

typedef struct  
{
	NIEngine* _this;

}THREADSTRUCT;