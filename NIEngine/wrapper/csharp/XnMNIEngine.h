#pragma once

#include "NIEngine.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace ManagedNI
{
	public ref class ManagedNIEngine
	{
	public:
		typedef void(_cdecl* StartedCallback)(bool success);
		delegate void EventDelegate();

		ManagedNIEngine();

		~ManagedNIEngine();  // Dispose()
		!ManagedNIEngine();  // Finalize()

		void Start();
		void Stop();
		void End();


	private:
		GCHandle startedGCHandle;
		EventDelegate^ startedCallback;
		
		void OnStarted();
	};
}