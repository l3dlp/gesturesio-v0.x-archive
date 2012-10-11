#include <XnCppWrapper.h>
#include <list>


typedef struct 
{
	bool isRightHand;
	std::string name;
	float timeStamp;
}GESTURERECORD;

class NIEngine
{
private:
	static NIEngine* _instance;
	xn::Context _niContext;
	xn::ScriptNode _niScriptNode;
	xn::DepthGenerator _depthGenerator;
	xn::UserGenerator _userGenerator;
	xn::GestureGenerator _gestureGenerator;
	XnBool _shouldStop;
	XnBool _running;
	XnSkeletonJointPosition _leftHand;
	XnSkeletonJointPosition _rightHand;
	XnSkeletonJointPosition _headPos;
	XnSkeletonJointOrientation _headOrient;
	XnPoint3D _leftHandPosProjective;
	XnPoint3D _rightHandPosProjective;
	XnPoint3D _headPosProjective;
	std::list<GESTURERECORD> _gestures;

public:
	static NIEngine* GetInstance();
	XnBool Start();
	XnBool Stop();
	void NewUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie);
	void LostUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie);
	void CalibrationCompleted(xn::SkeletonCapability& capability, XnUserID nId, XnCalibrationStatus eStatus, void* pCookie);
	void GestureRecognized(xn::GestureGenerator& generator,
							const XnChar* strGesture, 
							const XnPoint3D* pIDPosition,
							const XnPoint3D* pEndPosition, void* pCookie);

	float PointDist(const XnPoint3D* p1, const XnPoint3D* p2);

	~NIEngine();
	XnBool IsRunning();
	XnSkeletonJointPosition GetLeftHandPos();
	XnSkeletonJointPosition GetRightHandPos();
	XnPoint3D GetLeftHandPosProjective();
	XnPoint3D GetRightHandPosProjective();
	XnPoint3D GetHeadPosProjective();
	GESTURERECORD GetGesture();

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