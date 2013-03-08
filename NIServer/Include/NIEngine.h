/*****************************************************************************
*                                                                            *
*  NIServer                                                                  *
*  Copyright (C) 2013 MediaStanza Ltd.                                       *
*                                                                            *
*  This file is part of NIServer                                             *
*                                                                            *
*****************************************************************************/

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
			filteredPoint.x = (float)filterX(point.x,timeStamp);
			filteredPoint.y = (float)filterY(point.y,timeStamp);
			filteredPoint.z = (float)filterZ(point.z,timeStamp);
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


//! A singleton class wrapping OpenNI and NiTE's capability.
/*! It encapsulates all processes into a main procedure, which
 * can be run in single thread.
 * The main procedure can parse different commands and switch
 * to different states accordingly.
 */
class NIEngine
{
public:

     ///  Engine's running state. 
	enum State
	{
		Off,       //!< Engine is not running yet(main thread loop not entered).
		Idle,      //!< Ready to process commands.
		Streaming, //!< The engine is streaming data.
		Reading,   //!< The engine is reading data.
		Err,       //!< Some error happens
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
	void (*pStartedHandler)(bool);
	void (*pStoppedHandler)();
	void (*pEndedHandler)();

public:
    static NIEngine* GetInstance(); //!< Get single instance.
    ~NIEngine();
		
	void Start(void (*pHandler)(bool));    //!< Start the engine,which will start to produce required data.
	void Stop(void (*pHandler)());         //!< Stop the engine by stopping producing data.
	void End(void (*pHandler)());          //!< Quit the engine's loop and release all resources.
    void StartReading();                   //!< Start to read produced data.
    void StopReading();                    //!< Stop reading produced data.
	State GetState();                      //!< Read engine's current running state.

	/// Run the thread to handle incoming missions.
	/// @note It's blocking, will wait until the thread ends.
	void RunThread();                      

    void SetProfile(NISkelProfile profile);    //!< Set skeleton profile, to decide which joints to be tracked.
    nite::Point3f GetLeftHandPos();            //!< Get left hand's real world position
    nite::Point3f GetRightHandPos();           //!< Get right hand's real world position.
    nite::Point3f GetLeftHandPosProjective();  //!< Get left hand's projective position.
    nite::Point3f GetRightHandPosProjective(); //!< Get right hand's projective position.
    nite::Point3f GetHeadPosProjective();      //!< Get head's projective position.
    nite::Point3f GetHeadPos();                //!< Get head's real world position.
    std::string GetGesture();                  //!< Get tracked gestures.
	static void MainProc(void* arg);           //!< Main procedure for dedicated thread.

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
