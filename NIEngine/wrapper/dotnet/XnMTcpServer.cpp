#include "XnMTcpServer.h"

using namespace System;
using namespace System::IO;
using namespace System::Net;
using namespace System::Net::Sockets;
using namespace System::Threading;

namespace ManagedNI
{
	TcpServer::TcpServer()
	{
		IPAddress^ ipAddress = Dns::GetHostEntry("127.0.0.1")->AddressList[0];
		IPEndPoint^ ipEndpoint = gcnew IPEndPoint(ipAddress, PORT);
		listener = gcnew Socket(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp);
		listener->Bind(ipEndpoint);
	}

	TcpServer::!TcpServer()
	{

	}

	void TcpServer::Start()
	{
		listener->Listen(1);

		ThreadStart^ threadDelegate = gcnew ThreadStart(this, &TcpServer::Service);
		Thread^ newThread = gcnew Thread(threadDelegate);
		newThread->Start();
	}

	void TcpServer::Stop()
	{
		listener->Close();

	}

	void TcpServer::Service()
	{
		//while(true)
		{
			Socket^ serverSocket = listener->Accept();
			listener->Close();

			try
			{
				NetworkStream^ ns = gcnew NetworkStream(serverSocket);
				StreamReader^ sr = gcnew StreamReader(ns);
				StreamWriter^ sw = gcnew StreamWriter(ns);
				sw->AutoFlush = true;
				String^ cmd = sr->ReadLine();
				Console::Write(cmd);
				sw->WriteLine(cmd); // echo

				sr->Close();
				sw->Close();
				ns->Close();
			}
			finally
			{
				serverSocket->Shutdown(SocketShutdown::Both);
				serverSocket->Close();
			}
		}
	}
}