#include <string>
#include <list>
#include <queue>
#include "OpenNI.h"
#include "NiTE.h"
#include "OneEuroFilter.h"

typedef enum
{
    SKEL_PROFILE_HANDS,
    SKEL_PROFILE_HANDS_AND_HEAD
}NISkelProfile;

typedef struct
{
    std::string name;
    uint64_t timeStamp;
}GestureInfo;

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
	nite::Point3f filter(const nite::Point3f point, double timeStamp)
	{
		nite::Point3f filteredPoint;

		if (isEnabled == true)
		{
			filteredPoint.x = filterX(point.x,timeStamp);
			filteredPoint.y = filterY(point.y,timeStamp);
			filteredPoint.z = filterZ(point.z,timeStamp);
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
public:
	enum State
	{
		Off,       // Engine is not running yet.
		Idle,      // Ready to process commands.
		Streaming, // The device starts to stream data
		Reading,   // The engine starts to read stream
		Err,       // Some error happens
	};

private:
	enum Mission
	{
		ToStartSteaming,
		ToStopStreaming,
		ToStartReading,
		ToStopReading,
		ToEnd,
		DoNothing
	};

    const static int MAX_DISTANCE = 9999;
    const static short INVALID_ID = 0;
    const static int NUM_OF_SUPPORTED_JOINT = nite::JOINT_RIGHT_FOOT + 1;// in NiTE2, JOINT_RIGHT_FOOT is the last joint, starting from 0.
    const static int GESTURE_EXPIRED_TIME = 1; // ms
    static NIEngine* _instance;
    openni::Device _device;
    nite::UserTracker* _pUserTracker;
    nite::HandTracker* _pHandTracker;
    int _jointMap[NUM_OF_SUPPORTED_JOINT];
    nite::SkeletonJoint _joint[NUM_OF_SUPPORTED_JOINT];
    nite::Point3f _projJoint[NUM_OF_SUPPORTED_JOINT]; // Projective joints
	Point3DFilter* _filters[NUM_OF_SUPPORTED_JOINT];
    std::list<GestureInfo> _gestures;
    uint64_t _latestTs;
	std::queue<Mission> _missions;
	State _curState;

public:
    static NIEngine* GetInstance();
    ~NIEngine();
		
	void Start();
	void Stop();
	void Quit();
    void StartReading();
    void StopReading(); 
	State GetState();

    void SetProfile(NISkelProfile profile);
    nite::Point3f GetLeftHandPos();
    nite::Point3f GetRightHandPos();
    nite::Point3f GetLeftHandPosProjective();
    nite::Point3f GetRightHandPosProjective();
    nite::Point3f GetHeadPosProjective();
    nite::Point3f GetHeadPos();
    std::string GetGesture();
	static void StartThread(void* arg);

private:
    NIEngine();
	bool Init();
	void Finalize();
	void Read();
    void ProcessData();
    nite::UserId SelectActiveUser(const nite::Array<nite::UserData>& users);
    void ManageTracker(const nite::Array<nite::UserData>& users, nite::UserId activeID);
    void ReadSkeleton(const nite::UserData* pUser);
    void ReadGestureByID(const nite::Array<nite::GestureData>& gestures, nite::UserId activeID);
    nite::UserId FindGestureOwner(const nite::Point3f& handPoint,int xDist, int yDist); // xDist/yDist - represents distance from current hand point's x/y
    std::string GetNameFromGestureType(nite::GestureType type);
    nite::Point3f WorldToProjective(const nite::Point3f& orig);
	void ConstructFilters();
	void DestructFilters();
};

typedef struct
{
    NIEngine* _this;

}THREADSTRUCT;
