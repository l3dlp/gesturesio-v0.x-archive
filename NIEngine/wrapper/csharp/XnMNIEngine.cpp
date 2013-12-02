#include "XnMNIEngine.h"

//using namespace System::Runtime::InteropServices;

namespace ManagedNI
{
	ManagedNIEngine::ManagedNIEngine()
	{
		NIEngine::GetInstance()->RunThread();
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
	}
	
	void ManagedNIEngine::OnStarted()
	{

	}

	void ManagedNIEngine::Start()
	{
		startedCallback = gcnew EventDelegate(this, &ManagedNIEngine::OnStarted);
		startedGCHandle = GCHandle::Alloc(startedCallback);
		IntPtr ptr = Marshal::GetFunctionPointerForDelegate(startedCallback);
		NIEngine::GetInstance()->Start((StartedCallback)(void*)ptr);
	}

	void ManagedNIEngine::Stop()
	{

	}

	void ManagedNIEngine::End()
	{

	}
}