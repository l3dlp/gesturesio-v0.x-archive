#pragma once

#include "NIEngine.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace ManagedNI
{
	public ref class ManagedNIEngine
	{
	public:
		event EventHandler^ Started;
		event EventHandler^ Stopped;
		event EventHandler^ Ended;

		ManagedNIEngine();

		~ManagedNIEngine();  // Dispose()
		!ManagedNIEngine();  // Finalize()

		void Init(); // Blocking
		void End();
		void Start();
		void Stop();

	private:
		typedef void(_cdecl* StartedCallback)(bool success);
		typedef void(_cdecl* StoppedCallback)();
		typedef void(_cdecl* EndedCallback)();

		delegate void EventDelegate();

		GCHandle startedGCHandle;
		GCHandle stoppedGCHandle;
		GCHandle endedGCHandle;

		EventDelegate^ startedDelegate;
		EventDelegate^ stoppedDelegate;
		EventDelegate^ endedDelegate;

		bool initialized;
		bool running;

		void OnStarted();
		void OnStopped();
		void OnEnded();
	};
}