#include "XnMNIEngine.h"

//using namespace System::Runtime::InteropServices;

namespace ManagedNI
{
	ManagedNIEngine::ManagedNIEngine()
	{
		initialized = false;
		running = false;

		startedDelegate = gcnew EventDelegate(this, &ManagedNIEngine::OnStarted);
		startedGCHandle = GCHandle::Alloc(startedDelegate);

		stoppedDelegate = gcnew EventDelegate(this, &ManagedNIEngine::OnStopped);
		stoppedGCHandle = GCHandle::Alloc(stoppedDelegate);

		endedDelegate = gcnew EventDelegate(this, &ManagedNIEngine::OnEnded);
		endedGCHandle = GCHandle::Alloc(endedDelegate);
	}

	ManagedNIEngine::~ManagedNIEngine()
	{

	}

	ManagedNIEngine::!ManagedNIEngine()
	{
		if (startedGCHandle.IsAllocated)
		{
			startedGCHandle.Free();
		}

		if (stoppedGCHandle.IsAllocated)
		{
			stoppedGCHandle.Free();
		}

		if (endedGCHandle.IsAllocated)
		{
			endedGCHandle.Free();
		}
	}
	
	void ManagedNIEngine::OnStarted()
	{
		running = true;

		NIEngine::GetInstance()->StartReading();
		Started(this,gcnew EventArgs());
	}

	void ManagedNIEngine::OnStopped()
	{
		running = false;

		NIEngine::GetInstance()->StopReading();
		Stopped(this,gcnew EventArgs());
	}

	void ManagedNIEngine::OnEnded()
	{
		initialized = false;
		Ended(this,gcnew EventArgs());
	}

	void ManagedNIEngine::Start()
	{
		if (initialized == true && running == false)
		{
			IntPtr ptr = Marshal::GetFunctionPointerForDelegate(startedDelegate);

			NIEngine::GetInstance()->SetProfile(SKEL_PROFILE_HANDS_AND_HEAD);
			NIEngine::GetInstance()->Start((StartedCallback)(void*)ptr);
		}
	}

	void ManagedNIEngine::Stop()
	{
		if (running)
		{
			IntPtr ptr = Marshal::GetFunctionPointerForDelegate(stoppedDelegate);

			NIEngine::GetInstance()->Stop((StoppedCallback)(void*)ptr);
		}
	}

	void ManagedNIEngine::Init()
	{
		if (initialized == false)
		{
			initialized = true;
			NIEngine::GetInstance()->RunThread();
		}
	}

	void ManagedNIEngine::End()
	{
		if (initialized)
		{
			IntPtr ptr = Marshal::GetFunctionPointerForDelegate(endedDelegate);
			NIEngine::GetInstance()->End((EndedCallback)(void*)ptr);
		}
	}
}