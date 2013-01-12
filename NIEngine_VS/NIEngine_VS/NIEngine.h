#include <string>
#include "OpenNI.h"
#include "NiTE.h"

typedef enum
{
	SKEL_PROFILE_HANDS,
	SKEL_PROFILE_HANDS_AND_HEAD
}NISkelProfile;

typedef struct GestureInfo 
{
	std::string name;
	uint64_t timeStamp;
};

class NIEngine
{
private:
	const static int MAX_DISTANCE = 9999;
	const static short INVALID_ID = 0;
	const static int NUM_OF_SUPPORTED_JOINT = nite::JOINT_RIGHT_FOOT + 1;// in NiTE2, JOINT_RIGHT_FOOT is the last joint, starting from 0.
	const static int GESTURE_EXPIRED_TIME = 1; // ms
	static NIEngine* _instance;
	openni::Device _device;
	nite::UserTracker* _pUserTracker;
	nite::HandTracker* _pHandTracker;
	bool _isAlive;
	bool _shouldRun;
	int _jointMap[NUM_OF_SUPPORTED_JOINT]; 
	nite::SkeletonJoint _joint[NUM_OF_SUPPORTED_JOINT];
	GestureInfo _gesture;
	uint64_t _latestTs;

public:
	static NIEngine* GetInstance();
	~NIEngine();
	bool Init();
	void Terminate();
	void Start();
	void Stop();
	void SetProfile(NISkelProfile profile);
	nite::Point3f GetLeftHandPos();
	nite::Point3f GetRightHandPos();
	nite::Point3f GetLeftHandPosProjective();
	nite::Point3f GetRightHandPosProjective();
	nite::Point3f GetHeadPosProjective();
	nite::Point3f GetHeadPos();
	std::string GetGesture(bool leftHand);
    nite::Point3f WorldToProjective(const nite::Point3f& position);

private:
	NIEngine();
	static void StartThread(void* arg);
	void ProcessData();
	nite::UserId SelectActiveUser(const nite::Array<nite::UserData>& users);
	void ManageTracker(const nite::Array<nite::UserData>& users, nite::UserId activeID);
	void ReadSkeleton(const nite::UserData* pUser);
	void ReadGestureByID(const nite::Array<nite::GestureData>& gestures, nite::UserId activeID);
	nite::UserId FindGestureOwner(const nite::Point3f& handPoint,int xDist, int yDist); // xDist/yDist - represents distance from current hand point's x/y
	std::string GetNameFromGestureType(nite::GestureType type);
};

typedef struct  
{
	NIEngine* _this;

}THREADSTRUCT;