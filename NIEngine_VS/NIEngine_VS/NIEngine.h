#include "OpenNI.h"
#include "NiTE.h"

class NIEngine
{
private:
	const static int MAX_DISTANCE = 9999;
	const static short INVALID_ID = 0;
	static NIEngine* _instance;
	openni::Device _device;
	nite::UserTracker* _pUserTracker;
	nite::HandTracker* _pHandTracker;
	bool _isAlive;
	bool _shouldRun;

public:
	static NIEngine* GetInstance();
	~NIEngine();
	bool Init();
	void Terminate();
	void Start();
	void Stop();

private:
	NIEngine();
	static void StartThread(void* arg);
	void ProcessData();
	nite::UserId SelectActiveUser(const nite::Array<nite::UserData>& users);
	void ManageTracker(const nite::Array<nite::UserData>& users, nite::UserId activeID);
	void ReadSkeleton(const nite::UserData* pUser);
	nite::UserId FindGestureOwner(const nite::Point3f& handPoint,int xDist, int yDist); // xDist/yDist - represents distance from current hand point's x/y
};

typedef struct  
{
	NIEngine* _this;

}THREADSTRUCT;