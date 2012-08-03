#include <stdio.h>
#include "NIEngine.h"
#include "SocketServer.h"

int main(int argc, char** argv)
{
	printf("NIServer\n");
	NIEngine* niEngine = NIEngine::GetInstance();
	niEngine->Start();
	printf("NIEngine Started.\n");
	SocketServer server;
	server.Launch();

	printf("Press key:\n");
	// Wait for command
	while(1)
	{
		int ch = getchar();
		switch (ch)
		{
		case 'x':
			niEngine->Stop();
			break;
		case 't':
			server.Terminate();
			break;
		default:
			break;
		}
	}

	return 1;
}