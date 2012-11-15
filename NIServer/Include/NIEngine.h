#include <XnCppWrapper.h>
#include <list>
#include "OneEuroFilter.h"

typedef struct 
{
	bool isRightHand;
	std::string name;
	float timeStamp;
}GESTURERECORD;

//// 3D point filter
// Using one euro filter, a simple speed-based Low-pass filter.
// More info here: http://www.lifl.fr/~casiez/1euro/
struct Point3DFilter
{
	Point3DFilter(double _freq, double _mincutoff, double _beta, double _dcutoff): isEnabled(true)
	{
		filterX.setFreq(_freq);
		filterX.setMincutoff(_mincutoff);
		filterX.setBeta(_beta);
		filterX.setDcutoff(_dcutoff);

		filterY.setFreq(_freq);
		filterY.setMincutoff(_mincutoff);
		filterY.setBeta(_beta);
		filterY.setDcutoff(_dcutoff);

		filterZ.setFreq(_freq);
		filterZ.setMincutoff(_mincutoff);
		filterZ.setBeta(_beta);
		filterZ.setDcutoff(_dcutoff);
	}

private:
	one_euro_filter<> filterX;
	one_euro_filter<> filterY;
	one_euro_filter<> filterZ;
	bool isEnabled;

public:
	XnPoint3D filter(XnPoint3D point, double timeStamp)
	{
		XnPoint3D filteredPoint;
		
		if (isEnabled == true)
		{
			filteredPoint.X = filterX(point.X,timeStamp);
			filteredPoint.Y = filterY(point.Y,timeStamp);
			filteredPoint.Z = filterZ(point.Z,timeStamp);
		}
		else
		{
			filteredPoint = point;
			printf("filter disabled! \n");
		}

		return filteredPoint;
	}

	void enable(bool enabled)
	{
		isEnabled = enabled;
	}
};

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
