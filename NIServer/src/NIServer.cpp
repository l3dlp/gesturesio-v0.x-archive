#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include "NIEngine.h"
#include "Utils.h"
#include "NIServer.h"
#include "NITcpServer.h"

using namespace std;

#define SERVER_PORT 6789

string g_keyword;
string g_logIn;
string g_logOut;
string g_clientIn;
string g_clientOut;

int g_limitedTime = 0;
bool g_isRunning = false;
NITcpServer* g_tcpServer = NULL;

string ReadLicense(char* fileName)
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

license_State ValidateLicense(string keyword)
{
    license_State licenseStat = LICENSE_UNKNOWN;

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
                g_limitedTime = atoi(it->data.c_str());
            }
        }
    }

    return licenseStat;
}

license_State CheckLicense()
{
    Logger::GetInstance()->Log("NIServer Running...");

    license_State liceneStat = LICENSE_UNKNOWN;

    string keyword = ReadLicense("license.txt");

    g_logIn = "https://api.activedooh.com/v1/" + keyword + "/log/in.xml";
    g_logOut = "https://api.activedooh.com/v1/" + keyword + "/log/out.xml";
    g_clientIn = "https://api.activedooh.com/v1" + keyword + "/client/in.xml";
    g_clientOut = "https://api.activedooh.com/v1" + keyword + "/client/out.xml";

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

int GetLimitedTime()
{
    return g_limitedTime;
}

void StartNIService()
{
    if(g_isRunning == false)
    {
        HttpRequest(g_logIn.c_str());

        g_tcpServer = new NITcpServer();
        g_tcpServer->SetClientLog(g_clientIn,g_clientOut);
        g_tcpServer->Start(SERVER_PORT);

        NIEngine::GetInstance()->SetProfile(SKEL_PROFILE_HANDS_AND_HEAD);
        NIEngine::GetInstance()->Start();

        g_isRunning = true;
    }

    return;
}

void StopNIService()
{
    if(g_isRunning == true)
    {
        g_tcpServer->Stop();
        delete g_tcpServer;
        g_tcpServer = NULL;

        NIEngine::GetInstance()->Stop();
        HttpRequest(g_logOut.c_str());

        g_isRunning = false;
    }
    return;
}
