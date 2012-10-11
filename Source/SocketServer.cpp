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

					//printf("Received request \"%s\". Sending response.\n",data);

					XnPoint3D leftHandPos = NIEngine::GetInstance()->GetLeftHandPosProjective();
					XnPoint3D rightHandPos = NIEngine::GetInstance()->GetRightHandPosProjective();
					XnPoint3D headPos = NIEngine::GetInstance()->GetHeadPosProjective();

					// retrieve gesture

					std::string leftGesture = "NONE";
					std::string rightGesture = "NONE";

					GESTURERECORD gestureRec = NIEngine::GetInstance()->GetGesture();
					if (gestureRec.timeStamp >= 0) 		// TODO: check timestamp and give up outdate gestures
					{
						printf("valid gesture %s %d\n",gestureRec.name.c_str(),gestureRec.isRightHand);

						if (gestureRec.isRightHand == true)
						{
							rightGesture = gestureRec.name;
						}
						else
						{
							leftGesture = gestureRec.name;
						}
					}

					if (strcmp(data,"getCoordsT") == 0)
					{
						//printf("%f %f %f %f %f %f\n",leftHandPos.X,leftHandPos.Y,leftHandPos.Z,rightHandPos.X,rightHandPos.Y,rightHandPos.Z);
						sprintf_s(data,"%f %f %f %f %f %f %f %f %f %s %s",leftHandPos.X,leftHandPos.Y,leftHandPos.Z,
							rightHandPos.X,rightHandPos.Y,rightHandPos.Z,headPos.X,headPos.Y,headPos.Z,leftGesture.c_str(),rightGesture.c_str());

					}
					else if (strcmp(data,"getCoordsX") == 0)
					{
						int length = 0;
						length += sprintf_s(data,"<?xml version=\"1.0\" encoding=\"utf-8\">");
						length += sprintf(data + length,"<coords>");
						length += sprintf(data + length,"<leftHand a=\"%s\" x=\"%f\" y=\"%f\" z=\"%f\"></leftHand>",leftGesture.c_str(),leftHandPos.X,leftHandPos.Y,leftHandPos.Z);
						length += sprintf(data + length,"<rightHand a=\"%s\" x=\"%f\" y=\"%f\" z=\"%f\"></rightHand>",rightGesture.c_str(),rightHandPos.X,rightHandPos.Y,rightHandPos.Z);
						length += sprintf(data + length,"<headPos x=\"%f\" y=\"%f\" z=\"%f\"></headPos>",headPos.X,headPos.Y,headPos.Z);
						sprintf(data + length,"</coords>");
					}
					else if (strcmp(data,"getCoordsJ") == 0)
					{
						int length = 0;
						length += sprintf_s(data,"[");
						length += sprintf(data + length,"\"coords\",{");
						length += sprintf(data + length,"\"leftHandX\":\"%f\",\"leftHandY\":\"%f\",\"leftHandZ\":\"%f\",",leftHandPos.X,leftHandPos.Y,leftHandPos.Z);
						length += sprintf(data + length,"\"rightHandX\":\"%f\",\"rightHandY\":\"%f\",\"rightHandZ\":\"%f\",",rightHandPos.X,rightHandPos.Y,rightHandPos.Z);
						length += sprintf(data + length,"\"headPosX\":\"%f\",\"headPosY\":\"%f\",\"headPosZ\":\"%f\",",headPos.X,headPos.Y,headPos.Z);
						sprintf(data + length,"}]");
					}

					_pClient->Send((const uint8*)&data,sizeof(data));
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