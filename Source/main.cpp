#include "NIEngine.h"
#include "SocketServer.h"

int main(int argc, char** argv)
{
	NIEngine* niEngine = NIEngine::GetInstance();
	//niEngine->Start();
	SocketServer server;
	server.Launch();
	return 0;
}