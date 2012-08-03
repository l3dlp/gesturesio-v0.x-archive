#include "SocketServer.h"
#include "TinyThread/tinythread.h"

using namespace tthread;

#define MAX_PACKET 4096

SocketServer::SocketServer()
{
	_pClient = NULL;
}

SocketServer::~SocketServer()
{

}

void SocketServer::StartThread(void* arg)
{
	SOCKETTHREADSTRUCT* ts = (SOCKETTHREADSTRUCT*)arg;
	ts->_this->ProcessRequest();
}

void SocketServer::ProcessRequest()
{
	_shouldStop = FALSE;

	while (_shouldStop == FALSE)
	{
		if((_pClient = _socket.Accept()) != NULL)
		{
			printf("Received request.\n");

			// Receive request from the client.
			if (_pClient->Receive(MAX_PACKET))
			{
				printf("Sending response.\n");

				// Send response to client and close connection to the client
				_pClient->Send((const uint8*)_pClient->GetData(),_pClient->GetBytesReceived());
				_pClient->Close();
			}
			delete _pClient;
		}
	}
}

void SocketServer::Launch()
{
	_socket.Initialize();
	_socket.Listen((const uint8*)"localhost",6789);  // cp-concern: uint8
	_shouldStop = FALSE;

	SOCKETTHREADSTRUCT* param = new SOCKETTHREADSTRUCT;
	param->_this = this;
	thread niThread(StartThread,param);
	//niThread.join();
	niThread.detach();
}

void SocketServer::Terminate()
{
	_shouldStop = TRUE;
}