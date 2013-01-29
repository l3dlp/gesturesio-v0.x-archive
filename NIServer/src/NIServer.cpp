#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
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
	Logger::GetInstance()->Log("keyword: " + keyword);
	return keyword;
}

NIServer::license_State NIServer::ValidateLicense(string keyword)
{
    NIServer::license_State licenseStat = LICENSE_UNKNOWN;

	Logger::GetInstance()->Log("validating your license...");

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

    return licenseStat;
}

NIServer::license_State NIServer::CheckLicense()
{
    Logger::GetInstance()->Log("NIServer Running...");

    NIServer::license_State liceneStat = LICENSE_UNKNOWN;

    string keyword = ReadLicense("license.txt");

    logIn = "https://api.activedooh.com/v1/" + keyword + "/log/in.xml";
    logOut = "https://api.activedooh.com/v1/" + keyword + "/log/out.xml";
    clientIn = "https://api.activedooh.com/v1" + keyword + "/client/in.xml";
    clientOut = "https://api.activedooh.com/v1" + keyword + "/client/out.xml";

    if (keyword.empty())
    {
        Logger::GetInstance()->Log("Failed to read keyword...");
    }
    else
    {
        liceneStat = ValidateLicense(keyword);
        if (liceneStat == LICENSE_UNKNOWN)
        {
            Logger::GetInstance()->Log("License validation failed..");
        }
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
    if(isTcpServing == false)
    {
        isTcpServing = true;

        pTcpServer = new NITcpServer();
        pTcpServer->SetClientLog(clientIn,clientOut);
        pTcpServer->Start(PORT);
    }
    return true;
}

bool NIServer::StopTcpService()
{
    if(isTcpServing == true)
    {
        isTcpServing = false;

        pTcpServer->Stop();
        delete pTcpServer;
        pTcpServer = NULL;
    }
    return true;
}

bool NIServer::StartNIService()
{
    bool res = true;

    if(isNIServing == false)
    {
        NIEngine::GetInstance()->SetProfile(SKEL_PROFILE_HANDS_AND_HEAD);
        res = NIEngine::GetInstance()->Init();
        if(res)
        {
            NIEngine::GetInstance()->Start();
            isNIServing = true;
        }
    }

    if(res)
    {
        HttpRequest(logIn.c_str());  // Consumes lots of time.
    }

    return res;
}

void NIServer::StopNIService()
{
    if(isNIServing == true)
    {
        NIEngine::GetInstance()->Terminate();
        isNIServing = false;
    }
    HttpRequest(logOut.c_str()); // Consumes lots of time.
    return;
}
