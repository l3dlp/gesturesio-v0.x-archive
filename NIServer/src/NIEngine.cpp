#include <algorithm>
#include "NIEngine.h"
#include "tinyThread/tinythread.h"

NIEngine* NIEngine::_instance = 0;

NIEngine::NIEngine()
{
    _pUserTracker = NULL;
    _pHandTracker = NULL;
}

NIEngine::~NIEngine()
{

}

NIEngine* NIEngine::GetInstance()
{
    if (_instance == 0)
    {
        _instance = new NIEngine();
    }
    return _instance;
}

void NIEngine::StartThread(void* arg)
{
    THREADSTRUCT* ts = (THREADSTRUCT*)arg;
    ts->_this->ProcessData();
}

void NIEngine::ConstructFilters()
{
	double frequency = 30;   // 30FPS by default
	double mincutoff = 1;    // min cutoff frequency
	double beta = 0.05;      // cutoff slope
	double dcutoff = 1;      // cutoff frequency for derivate

	// Construct filters
	for(int i = 0; i < NUM_OF_SUPPORTED_JOINT; i++)
	{
		_filters[i] = new Point3DFilter(frequency,mincutoff,beta,dcutoff);
	}
}

void NIEngine::DestructFilters()
{
	for(int i = 0; i < NUM_OF_SUPPORTED_JOINT; i++)
	{
		if(_filters[i] != NULL)
		{
			delete _filters[i];
			_filters[i] = NULL;
		}
	}
}

void NIEngine::ProcessData()
{
    nite::UserTrackerFrameRef userTrackerFrame;
    nite::HandTrackerFrameRef handFrame;
	_isAlive = true;
	_shouldRun = true;
	_shouldRead = false;
	ConstructFilters();
    printf("NIEngine running...\n");

    while (_shouldRun)
    {
        if (_shouldRead == false)
        {
            continue;
        }

        if(_pUserTracker == NULL)
            continue;

        nite::Status niteRc = _pUserTracker->readFrame(&userTrackerFrame);

        if (niteRc != nite::STATUS_OK)
        {
            printf("Read next user tracker frame failed.\n");
            continue;
        }

        _latestTs = userTrackerFrame.getTimestamp();
        const nite::Array<nite::UserData>& users = userTrackerFrame.getUsers();

        // NOTE: we can still read valid ID even when the user left, because NiTE will keep it
        // for a certain time to allow user come back before really drop the ID.
        // So keep it in mind in case we need to handle it - stop reading skeleton data immediately once user left.
        nite::UserId activeID = SelectActiveUser(users);

        if (activeID != INVALID_ID)
        {
            // The tracker manager will make sure we track one user only, to save bandwidth.
            ManageTracker(users, activeID);

            // Read data from active user
            const nite::UserData* pUser = userTrackerFrame.getUserById(activeID);
            if (pUser->getSkeleton().getState() == nite::SKELETON_TRACKED)
            {
                ReadSkeleton(pUser);
            }
        }

        // Read gesture
        if(_pHandTracker == NULL)
            continue;

        niteRc = _pHandTracker->readFrame(&handFrame);
        if (niteRc != nite::STATUS_OK)
        {
            printf("Failed to read next hand frame.\n");
            continue;
        }

        if (activeID != INVALID_ID)
        {
            ReadGestureByID(handFrame.getGestures(),activeID);
        }
    }
	DestructFilters();
	_isAlive = false;
}

std::string NIEngine::GetNameFromGestureType(nite::GestureType type)
{
    std::string name = "";

    switch(type)
    {
    case nite::GESTURE_CLICK:
        name = "click";
        break;
    case nite::GESTURE_WAVE:
        name = "wave";
        break;
    default:
        break;
    }
    return name;
}

nite::UserId NIEngine::FindGestureOwner(const nite::Point3f& handPoint, int xDist, int yDist)
{
    nite::UserId foundID = INVALID_ID;
    float handX;
    float handY;

    _pUserTracker->convertJointCoordinatesToDepth(handPoint.x,handPoint.y,handPoint.z,&handX,&handY);

    nite::UserTrackerFrameRef userTrackerFrame;
    nite::Status rc = _pUserTracker->readFrame(&userTrackerFrame);
    if (rc == nite::STATUS_OK)
    {
        const nite::UserMap& userLabels = userTrackerFrame.getUserMap();
        const nite::UserId* pLabels = userLabels.getPixels();
        int rowSize = userLabels.getStride() / sizeof(nite::UserId);
        // Define current hand point's surrounding window to look for user ID.
        int startX = (std::max)((int)handX - xDist,0);
        int endX = (std::min)((int)handX + xDist,userLabels.getWidth());
        int startY = (std::max)((int)handY - yDist,0);
        int endY = (std::min)((int)handY + yDist,userLabels.getHeight());

        bool hasMultiUsers = false;
        nite::UserId prevID = INVALID_ID;

        for (int y = startY; y <= endY; ++y)
        {
            for (int x = startX; x <= endX; ++x)
            {
                nite::UserId curID = pLabels[y * rowSize + x];
                if ( curID > INVALID_ID )
                {
                    foundID = curID;

                    if (curID != prevID && prevID > INVALID_ID)
                    {
                        hasMultiUsers = true;
                    }
                    prevID = curID;
                }
            }
            pLabels += rowSize;
        }

        if (hasMultiUsers)
        {
            foundID = INVALID_ID; // Invalidate it if there are more than one users inside the window.
        }
        //printf("w %d h %d x %d y %d r %d\n",userLabels.getWidth(),userLabels.getHeight(),(int)handX,(int)handY,rowSize);
    }

    return foundID;
}

void NIEngine::ReadSkeleton(const nite::UserData* pUser)
{
	nite::Point3f tmpPoint;
	nite::Point3f filteredPoint;

    for (int i = 0; i < NUM_OF_SUPPORTED_JOINT; ++i)
    {
        if (_jointMap[i] == 1)
        {
            _joint[i] = pUser->getSkeleton().getJoint((nite::JointType)i);
            tmpPoint = _joint[i].getPosition();
			filteredPoint = _filters[i]->filter(tmpPoint,_latestTs);
			_projJoint[i] = WorldToProjective(filteredPoint);
        }
    }
}

void NIEngine::ReadGestureByID(const nite::Array<nite::GestureData>& gestures, nite::UserId activeID)
{
    for (int i = 0; i < gestures.getSize(); ++i)
    {
        if (gestures[i].isComplete())
        {
            const nite::Point3f handPos = gestures[i].getCurrentPosition();
            nite::UserId ownerID = FindGestureOwner(handPos,0,0); // Current test shows that we don't need the window so far.
            //printf("Gesture detected. ID=%d %d\n",ownerID,activeID);
            if (ownerID == activeID)
            {
                GestureInfo tmpGesture;

                nite::GestureType gType = gestures[i].getType();
                tmpGesture.name = GetNameFromGestureType(gType);
                tmpGesture.timeStamp = _latestTs;

                _gestures.push_back(tmpGesture);
                printf("Gesture detected ID %d type %d\n",ownerID,gType);
            }
        }
    }
}

void NIEngine::ManageTracker(const nite::Array<nite::UserData>& users, nite::UserId activeID)
{
    for (int i = 0; i < users.getSize(); ++i)
    {
        const nite::UserData& user = users[i];
        nite::UserId id = user.getId();
        nite::SkeletonState stat = user.getSkeleton().getState();

        if (id == activeID)
        {
            if(stat != nite::SKELETON_TRACKED)
            {
                _pUserTracker->startSkeletonTracking(id);
            }
        }
        else
        {
            if (stat == nite::SKELETON_TRACKED)
            {
                _pUserTracker->stopSkeletonTracking(id);
            }
        }
    }
}

// Select active user from users in FOV
// Current algorithm is to pick up the closest user.
nite::UserId NIEngine::SelectActiveUser(const nite::Array<nite::UserData>& users)
{
    float closestDist = MAX_DISTANCE;
    nite::UserId activeID = INVALID_ID;

    for (int i = 0; i < users.getSize(); ++i)
    {
        const nite::UserData& user = users[i];
        nite::UserId id = user.getId();

        const nite::Point3f& com = user.getCenterOfMass(); // Calculate distance from COM.
        float dist = sqrtf(pow(com.x,2)+pow(com.y,2)+pow(com.z,2));

        if (dist < closestDist)
        {
            closestDist = dist;
            activeID = id;
        }
    }

    return activeID;
}

// Run from logic thread.
bool NIEngine::Init()
{
    openni::Status niRc;
    nite::Status niteRc;

    niRc = openni::OpenNI::initialize();
    if (niRc != openni::STATUS_OK)
    {
        printf("Failed to initialize OpenNI.\n %s\n",openni::OpenNI::getExtendedError());
        return false;
    }

    const char* deviceUri = openni::ANY_DEVICE;
    niRc = _device.open(deviceUri);
    if (niRc != openni::STATUS_OK)
    {
        printf("Failed to open device.\n%s\n", openni::OpenNI::getExtendedError());
        return false;
    }

    nite::NiTE::initialize();
    _pUserTracker = new nite::UserTracker();
    _pHandTracker = new nite::HandTracker();

    niteRc = _pUserTracker->create(&_device);
    if (niteRc != nite::STATUS_OK)
    {
        printf("Failed to create user tracker.\n");
        return false;
    }

    niteRc = _pHandTracker->create(&_device);
    if (niteRc != nite::STATUS_OK)
    {
        printf("Failed to create hand tracker.\n");
        return false;
    }

    _pHandTracker->startGestureDetection(nite::GESTURE_CLICK);
    _pHandTracker->startGestureDetection(nite::GESTURE_WAVE);
    //_pHandTracker->setSmoothingFactor(0.1);

    printf("NIEngine initialized successfully.\n");
    return true;
}

// Run from logic thread.
void NIEngine::Finalize()
{
	delete _pHandTracker;
	_pHandTracker = NULL;
	delete _pUserTracker;
	_pUserTracker = NULL;
	nite::NiTE::shutdown();
	openni::OpenNI::shutdown();
}

// Run from logic thread.
void NIEngine::SignalToEnd()
{
    _shouldRun = false;
}

bool NIEngine::IsAlive()
{
	return _isAlive;
}

void NIEngine::StartReading()
{
    _shouldRead = true;
}

void NIEngine::StopReading()
{
    _shouldRead = false;
}

void NIEngine::SetProfile(NISkelProfile profile)
{
    // Reset mask
    std::memset(_jointMap,0,sizeof(_jointMap));

    switch(profile)
    {
    case SKEL_PROFILE_HANDS:
        _jointMap[nite::JOINT_LEFT_HAND] = 1;
        _jointMap[nite::JOINT_RIGHT_HAND] = 1;
        break;
    case SKEL_PROFILE_HANDS_AND_HEAD:
        _jointMap[nite::JOINT_LEFT_HAND] = 1;
        _jointMap[nite::JOINT_RIGHT_HAND] = 1;
        _jointMap[nite::JOINT_HEAD] = 1;
        break;
    default:
        break;
    }
}

nite::Point3f NIEngine::GetLeftHandPos()
{
    nite::Point3f pos;
    if (_jointMap[nite::JOINT_LEFT_HAND] == 1)
    {
        pos = _joint[nite::JOINT_LEFT_HAND].getPosition();
    }
    return pos;
}

nite::Point3f NIEngine::GetLeftHandPosProjective()
{
    nite::Point3f pos;
    if (_jointMap[nite::JOINT_LEFT_HAND] == 1)
    {
        pos = _projJoint[nite::JOINT_LEFT_HAND];
    }
    return pos;
}

nite::Point3f NIEngine::GetRightHandPos()
{
    nite::Point3f pos;
    if (_jointMap[nite::JOINT_RIGHT_HAND] == 1)
    {
        pos = _joint[nite::JOINT_RIGHT_HAND].getPosition();
    }
    return pos;
}

nite::Point3f NIEngine::GetRightHandPosProjective()
{
    nite::Point3f pos;
    if (_jointMap[nite::JOINT_RIGHT_HAND] == 1)
    {
        pos = _projJoint[nite::JOINT_RIGHT_HAND];
    }
    return pos;
}

nite::Point3f NIEngine::GetHeadPos()
{
    nite::Point3f pos;
    if (_jointMap[nite::JOINT_HEAD] == 1)
    {
        pos = _joint[nite::JOINT_HEAD].getPosition();
    }
    return pos;
}

nite::Point3f NIEngine::GetHeadPosProjective()
{
    nite::Point3f pos;
    if (_jointMap[nite::JOINT_HEAD] == 1)
    {
        pos = _projJoint[nite::JOINT_HEAD];
    }
    return pos;
}

std::string NIEngine::GetGesture()
{
    std::string gesture = "NONE";

    if(_gestures.empty() == false)
    {
        GestureInfo tmpGesture = _gestures.back();
        _gestures.pop_back();
        double timeDiff = (double)(_latestTs - tmpGesture.timeStamp)/1000;

        if ( timeDiff <= GESTURE_EXPIRED_TIME)
        {
            gesture = tmpGesture.name;
        }
        printf("td %f gesture %s\n",timeDiff,gesture.c_str());
    }

    return gesture;
}

nite::Point3f NIEngine::WorldToProjective(const nite::Point3f& orig)
{
    nite::Point3f pos;

    if (_pUserTracker != NULL)
    {
        _pUserTracker->convertJointCoordinatesToDepth(orig.x,orig.y,orig.z,&pos.x,&pos.y);
    }
    return pos;
}
