#include "NIEngine.h"
#include "tinythread.h"

NIEngine* NIEngine::_instance = 0;

NIEngine::NIEngine()
{

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

void NIEngine::ProcessData()
{
	nite::UserTrackerFrameRef userTrackerFrame;
	nite::HandTrackerFrameRef handFrame;
	_isAlive = true;
	_shouldRun = true;

	printf("NIEngine running...\n");

	while (_isAlive)
	{
		if (_shouldRun == false)
		{
			continue;
		}

		nite::Status niteRc = _pUserTracker->readFrame(&userTrackerFrame);

		if (niteRc != nite::STATUS_OK)
		{
			printf("Read next user tracker frame failed.\n");
			continue;
		}

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

		// Read hand gesture
		niteRc = _pHandTracker->readFrame(&handFrame);
		if (niteRc != nite::STATUS_OK)
		{
			printf("Failed to read next hand frame.\n");
			continue;
		}

		const nite::Array<nite::GestureData>& gestures = handFrame.getGestures();
		for (int i = 0; i < gestures.getSize(); ++i)
		{
			if (gestures[i].isComplete())
			{
				nite::HandId handID;
				_pHandTracker->startHandTracking(gestures[i].getCurrentPosition(),&handID);
			}
		}

		const nite::Array<nite::HandData>& hands = handFrame.getHands();
		for (int i = 0; i < hands.getSize(); ++i)
		{
			const nite::HandData& hand = hands[i];
			if (hand.isTracking())
			{
				if (hand.isNew())
				{
					printf("New hand %d found.\n",hand.getId());
				}
				//printf("Hand Position: %5.2f,%5.2f,%5.2f\n",hand.getPosition().x,hand.getPosition().y,hand.getPosition().z);
			}
			else
			{
				printf("Hand %d lost\n",hand.getId());
			}
		}

	}
}

void NIEngine::ReadSkeleton(const nite::UserData* pUser)
{
	float coordinates[4] = {0};

	const nite::SkeletonJoint& leftHand = pUser->getSkeleton().getJoint(nite::JOINT_LEFT_HAND);
	const nite::SkeletonJoint& rightHand = pUser->getSkeleton().getJoint(nite::JOINT_RIGHT_HAND);

	// Convert to projective coordination
	//_pUserTracker->convertJointCoordinatesToDepth(leftHand.getPosition().x,leftHand.getPosition().y,leftHand.getPosition().z,
	//											&coordinates[0],&coordinates[1]);

	// NOTE: use 0.5 as I'm not sure now if NiTE2 has new definition/improvement on the confidence.
	if (leftHand.getPositionConfidence() >= 0.5f)
	{
		//printf("L(%5.2f,%5.2f,%5.2f)\n",leftHand.getPosition().x,leftHand.getPosition().y,leftHand.getPosition().z);
	}
	if (rightHand.getPositionConfidence() >= 0.5f)
	{
		//printf("R(%5.2f,%5.2f,%5.2f)\n",rightHand.getPosition().x,rightHand.getPosition().y,rightHand.getPosition().z);
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

	//_pHandTracker->setSmoothingFactor(0.1);

	// Create a dedicated thread to handle the data
	THREADSTRUCT* param = new THREADSTRUCT;
	param->_this = this;
	// TinyThread++, a portable thread implementation is used.
	// More info here: http://tinythreadpp.bitsnbites.eu/
	tthread::thread niThread(StartThread,param);
	//niThread.join();
	niThread.detach();

	printf("NIEngine initialized successfully.\n");
	return true;
}

void NIEngine::Terminate()
{
	_isAlive = false; // Wait until the thread ends?

	//delete _pHandTracker;
	//delete _pUserTracker;
	//nite::NiTE::shutdown();
	//openni::OpenNI::shutdown();
	printf("NIEngine terminated.\n");
}

void NIEngine::Start()
{
	_shouldRun = true;
	printf("Start reading.\n");
}

void NIEngine::Stop()
{
	_shouldRun = false;
	printf("Stop reading.\n");
}