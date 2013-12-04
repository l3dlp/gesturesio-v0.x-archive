#pragma once

using namespace System;
using namespace System::Net::Sockets;
using namespace System::Threading;

namespace ManagedNI
{
	public ref class TcpServer
	{
	public:
		static const int PORT = 5678;
		static const int LIMIT = 3;  // Numbers of concurrent clients

		delegate String^ MsgEventHandler(String^ message);
		event MsgEventHandler^ MsgReceived;

		TcpServer();
		!TcpServer();

		void Start();
		void Stop();

	private:
		Socket^ listener;
		Thread^ serviceThread;
		bool inService;

		void Service();
	};
}