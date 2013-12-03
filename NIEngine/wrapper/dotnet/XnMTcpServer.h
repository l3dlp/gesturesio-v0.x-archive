#pragma once

using namespace System::Net::Sockets;

namespace ManagedNI
{
	public ref class TcpServer
	{
	public:
		static const int PORT = 5678;
		static const int LIMIT = 3;  // Numbers of concurrent clients

		TcpServer();
		!TcpServer();

		void Start();
		void Stop();

	private:
		Socket^ listener;

		void Service();
	};
}