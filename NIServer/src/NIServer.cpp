#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <stack>
#include <time.h>
#include "NIEngine.h"
#include "Utils.h"
#include "NIServer.h"

string NIServer::keyword = "";
string NIServer::logIn = "";
string NIServer::logOut = "";
string NIServer::clientIn = "";
string NIServer::clientOut = "";

int NIServer::limitedTime = 0;
bool NIServer::isNIServing = false;
bool NIServer::isTcpServing = false;
NITcpServer* NIServer::pTcpServer = NULL;

string NIServer::ReadLicense(char* fileName)
{
	string keyword = "";
    ifstream licenseFile;

    licenseFile.open(fileName);
	if (licenseFile.is_open())
	{
		getline(licenseFile,keyword);
		licenseFile.close();
	}	
	Logger::GetInstance()->Log("Read license keyword: " + keyword);
	return keyword;
}

NIServer::license_State NIServer::ValidateLicense(string keyword)
{
    NIServer::license_State licenseStat = LICENSE_UNKNOWN;

	Logger::GetInstance()->Log("Validating license: " + keyword);

	//clock_t tBegin = clock();

	// call URL
	string url = "https://api.activedooh.com/v1/" + keyword + "/status.xml";
	string response = HttpRequest(url.c_str());

	// parse xml
    vector<XmlElement> elements;
    bool res = ParseXml(response,elements);

    if(res && elements.empty() == false)
    {
        vector<XmlElement>::iterator it;
        for (it = elements.begin(); it != elements.end(); it++)
        {
            if (it->name == "status")
            {
                licenseStat = (license_State)atoi(it->data.c_str());
            }
            if (it->name == "time_limit" )
            {
                limitedTime = atoi(it->data.c_str());
            }
        }
    }

	//clock_t tEnd = clock();
	//double time = (double(tEnd - tBegin)/CLOCKS_PER_SEC);

	//char buff[50];
	//sprintf(buff,"license validated in %f seconds.", time);
	//string timereport = buff;
	//Logger::GetInstance()->Log(timereport);

    return licenseStat;
}

NIServer::license_State NIServer::CheckLicense()
{
    Logger::GetInstance()->Log("Start to check license..");

    NIServer::license_State liceneStat = LICENSE_UNKNOWN;

    string keyword = ReadLicense("license.txt");

    logIn = "https://api.activedooh.com/v1/" + keyword + "/log/in.xml";
    logOut = "https://api.activedooh.com/v1/" + keyword + "/log/out.xml";
    clientIn = "https://api.activedooh.com/v1" + keyword + "/client/in.xml";
    clientOut = "https://api.activedooh.com/v1" + keyword + "/client/out.xml";

    if (!keyword.empty())
    {
        liceneStat = ValidateLicense(keyword);
    }

    return liceneStat;
}

int NIServer::GetLimitedTime()
{
    return limitedTime;
}

// NOTE: For some reason, tcp socket can't receive data from client when this
// function is called in QThread. So for now, just call it from main thread,
// actually tcp socket doesn't cost too much bandwidth so it fine to do it in main thread.
bool NIServer::StartTcpService()
{
	Logger::GetInstance()->Log("Starting Tcp service");

    if(isTcpServing == false)
    {
		isTcpServing = true;

		try
		{
			pTcpServer = new NITcpServer();
			pTcpServer->SetClientLog(clientIn,clientOut);
			pTcpServer->Start(PORT);
		}
		catch(...)
		{
			Logger::GetInstance()->Log("***Exception*** happens on starting Tcp server.");
			return false;
		}
    }

	int port = PORT;
	std::ostringstream s;
	s << port;
	Logger::GetInstance()->Log("Tcp service started, listening on port: " + s.str());

    return true;
}

bool NIServer::StopTcpService()
{
	Logger::GetInstance()->Log("Ending Tcp service..");

    if(isTcpServing == true)
    {
        isTcpServing = false;

        pTcpServer->Stop();
        delete pTcpServer;
        pTcpServer = NULL;
    }

	Logger::GetInstance()->Log("Tcp service ended");

    return true;
}

void NIServer::NIServiceStarted(bool success)
{
	if (success)
	{
		NIEngine::GetInstance()->StartReading();
		isNIServing = true;
		HttpRequest(logIn.c_str());
	}
}

void NIServer::StartNIService()
{
    if(isNIServing == false)
    {
        NIEngine::GetInstance()->SetProfile(SKEL_PROFILE_HANDS_AND_HEAD);
        NIEngine::GetInstance()->Start(NIServiceStarted);
    }
}

void NIServer::NIServiceStopped()
{
	isNIServing = false;
}

void NIServer::StopNIService()
{
	if (isNIServing == true)
	{
		NIEngine::GetInstance()->Stop(NIServiceStopped);
	}
}

void NIServer::NIServiceEnded()
{
	isNIServing = false;
	HttpRequest(logOut.c_str());
}

void NIServer::EndNIService()
{
	NIEngine::GetInstance()->End(NIServiceEnded);
}
