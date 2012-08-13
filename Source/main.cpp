#include <stdio.h>
#include "NIEngine.h"
#include "SocketServer.h"

int main(int argc, char** argv)
{
	printf("NIServer Running...\n");

	NIEngine::GetInstance()->Start();
	
	SocketServer server;
	server.Launch();

	printf("Input command:\n t - terminate server\n x - exit\n");
	bool shouldExit = FALSE;
	while(shouldExit == FALSE)
	{
		int ch = getchar();
		switch (ch)
		{
		case 'x':
			server.Terminate();
			NIEngine::GetInstance()->Stop();
			shouldExit = TRUE;
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