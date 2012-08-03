#include "SocketServer.h"

#define MAX_PACKET 4096

SocketServer::SocketServer()
{
	_pClient = NULL;
}

SocketServer::~SocketServer()
{

}

void SocketServer::Launch()
{
	_socket.Initialize();
	_socket.Listen((const uint8*)"localhost",6789);  // cp-concern: uint8
	_shouldStop = FALSE;

	// TODO: offload to dedicated thread
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

void SocketServer::Terminate()
{
	_shouldStop = TRUE;
}