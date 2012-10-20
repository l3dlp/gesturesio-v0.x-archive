#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include "NIEngine.h"
#include "SocketServer.h"
#include "FUBI/Fubi.h"
#include "FUBI/FubiUtils.h"
#include "TinyThread/tinythread.h"

using namespace std;
using namespace tthread;

#define FUBI_CONFIG_PATH "./Data/SamplesConfig.xml"  // cp-concern

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

bool shouldStop = FALSE;

void NIWorkerProc(void*)
{
	Fubi::init(FUBI_CONFIG_PATH);

	while (shouldStop == FALSE)
	{
		Fubi::update();
	}

	Fubi::release();
}

int main(int argc, char** argv)
{
	printf("NIServer Running...\n");
	//CheckLicense();

	thread niThread(NIWorkerProc,NULL);
	//niThread.join();
	niThread.detach();

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
			shouldStop = TRUE;
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