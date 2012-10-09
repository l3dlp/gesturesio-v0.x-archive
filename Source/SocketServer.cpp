#include "SocketServer.h"
#include "TinyThread/tinythread.h"
#include "NIEngine.h"

// Simple protocol:
// server: localhost
// port: 6789
// command: GetHands
// returned string format: 
// "lx ly lz rx ry rz"  // 6 values representing lefthand x/y/z, righthand x/y/z, delimited by space. And the data type is float.


using namespace tthread;

#define MAX_PACKET  1024
#define BUFFER_SIZE ((MAX_PACKET)*4)  // sizeof(int)=4

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
	char data[BUFFER_SIZE];

	while (_shouldStop == FALSE)
	{
		memset(&data,0,BUFFER_SIZE);
		if((_pClient = _socket.Accept()) != NULL)
		{
			// Receive request from the client.
			if (_pClient->Receive(MAX_PACKET))
			{
				if (_pClient->GetBytesReceived() <= BUFFER_SIZE)
				{
					memcpy(&data,_pClient->GetData(),_pClient->GetBytesReceived());

					printf("Received request \"%s\". Sending response.\n",data);

					if (strcmp(data,"GetHands") == 0)
					{
						XnPoint3D leftHandPos = NIEngine::GetInstance()->GetLeftHandPosProjective();
						XnPoint3D rightHandPos = NIEngine::GetInstance()->GetRightHandPosProjective();
						//printf("%f %f %f %f %f %f\n",leftHandPos.X,leftHandPos.Y,leftHandPos.Z,rightHandPos.X,rightHandPos.Y,rightHandPos.Z);
						sprintf_s(data,"%f %f %f %f %f %f",leftHandPos.X,leftHandPos.Y,leftHandPos.Z,
							rightHandPos.X,rightHandPos.Y,rightHandPos.Z);
						_pClient->Send((const uint8*)&data,sizeof(data));
					}

					if (strcmp(data,"GetHead") == 0)
					{
						XnPoint3D headPos = NIEngine::GetInstance()->GetHeadPosProjective();
						sprintf_s(data,"%f %f %f",headPos.X,headPos.Y,headPos.Z);
						_pClient->Send((const uint8*)&data,sizeof(data));
					}
				}
			}
			_pClient->Close();
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