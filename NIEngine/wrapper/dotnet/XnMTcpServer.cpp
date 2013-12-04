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
		inService = false;

		IPAddress^ localAddr = IPAddress::Parse( "127.0.0.1" );
		IPEndPoint^ ipEndpoint = gcnew IPEndPoint(localAddr, PORT);

		listener = gcnew Socket(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp);
		listener->Bind(ipEndpoint);
	}

	TcpServer::!TcpServer()
	{
		
	}

	void TcpServer::Start()
	{
		if (inService == false)
		{
			inService = true;

			listener->Listen(1);

			ThreadStart^ threadDelegate = gcnew ThreadStart(this, &TcpServer::Service);
			serviceThread = gcnew Thread(threadDelegate);
			serviceThread->Start();
		}
	}

	void TcpServer::Stop()
	{
		if (inService)
		{
			inService = false;

			try
			{
				listener->Shutdown(SocketShutdown::Both);
				listener->Close();
			}
			catch(...)
			{

			}
			
			if (serviceThread)
			{
				// How to stop accept and terminate the thread?
				//serviceThread->Join();
				//serviceThread = nullptr;
			}
		}
	}

	void TcpServer::Service()
	{
		while(true)
		{
			Socket^ server = listener->Accept();
			try
			{
				NetworkStream^ ns = gcnew NetworkStream(server);
				StreamReader^ sr = gcnew StreamReader(ns);
				StreamWriter^ sw = gcnew StreamWriter(ns);
				sw->AutoFlush = true;
				String^ msg = sr->ReadLine();

				Console::Write(msg);
				Console::Write("\n");
				String^ res = MsgReceived(msg); // Process the message
				Console::Write(res);
				Console::Write("\n");

				sw->WriteLine(msg); // echo

				sr->Close();
				sw->Close();
				ns->Close();
			}
			catch(...)
			{
				Console::WriteLine("Server reading exception: the client might be closed forcefully");
			}
			finally
			{
				server->Shutdown(SocketShutdown::Both);
				server->Close();
			}

		}
	}
}