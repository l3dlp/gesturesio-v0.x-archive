#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include "NIEngine.h"
#include "SocketServer.h"
#include "Utils.h"

using namespace std;

bool CheckLicense()
{
	printf("validating your license...\n");
	string keyword;
	ifstream licenseFile;
	licenseFile.open("license.txt");
	if (licenseFile.is_open())
	{
		getline(licenseFile,keyword);
		licenseFile.close();
	}
	printf("keyword:%s\n", keyword.c_str());

	// call URL
	string url = "http://api.activedooh.com/v1/" + keyword + "/status.txt";
	printf(url.c_str());

	// TODO: find the easiest way to read data from internet(url).

	return true;
}

int main(int argc, char** argv)
{
	Logger logger("log.txt");

	logger.Log("NIServer Running...");
	//printf("NIServer Running...\n");
	//CheckLicense();
	
	NIEngine::GetInstance()->Start();
	
	SocketServer server;
	server.Launch();

	logger.Log("Server launched...");

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