#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include "NIEngine.h"
#include "Utils.h"
#include "expat/expat.h"
#include "NIServer.h"

using namespace std;

struct XmlElement
{
	string name;
	string data;
};

string g_keyword;
string g_logIn;
string g_logOut;
string g_clientIn;
string g_clientOut;

int g_limitedTime = 0;
bool g_isRunning = false;

int Eventcnt = 0;

stack<XmlElement> elemStack;
vector<XmlElement> elemVector;

void default_hndl(void *data, const char *s, int len) 
{
	fwrite(s, len, sizeof(char), stdout);
}

void printcurrent(XML_Parser p)
{
	XML_SetDefaultHandler(p, default_hndl);
	XML_DefaultCurrent(p);
	XML_SetDefaultHandler(p, (XML_DefaultHandler) 0);
}

void start_hndl(void *data, const char *el, const char **attr)
{
	printf("\n%4d: Start tag %s - ", Eventcnt++, el);
	printcurrent((XML_Parser) data);

	// push current element
	XmlElement tmp;
	tmp.name = string(el);
	elemStack.push(tmp);
}

void end_hndl(void *data, const char *el) 
{
	printf("\n%4d: End tag %s -\n", Eventcnt++, el);
	 
	elemStack.pop();
}

void char_hndl(void *data, const char *txt, int txtlen) 
{
	printf("\n%4d: Text - ", Eventcnt++);
	fwrite(txt, txtlen, sizeof(char), stdout);
	
	// save current element
	XmlElement tmp = elemStack.top();
	tmp.data = string(txt,txtlen);
	elemVector.push_back(tmp);
}

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
	XML_Parser p = XML_ParserCreate(NULL);
	if (p)
	{
		XML_UseParserAsHandlerArg(p);
		XML_SetElementHandler(p, start_hndl, end_hndl);
		XML_SetCharacterDataHandler(p, char_hndl);

		elemVector.clear();
		bool parseRes = XML_Parse(p, response.c_str(), response.size(), true);
		if (parseRes)
		{
			// parse successfully, now we need to check the value
			if (elemVector.empty() == false)
			{
				vector<XmlElement>::iterator it;
				for (it = elemVector.begin(); it != elemVector.end(); it++)
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

void StartNIServer()
{
    if(g_isRunning == false)
    {
        HttpRequest(g_logIn.c_str());

        NIEngine::GetInstance()->Start();

        g_isRunning = true;
    }

    return;
}

void StopNIServer()
{
    if(g_isRunning == true)
    {
        NIEngine::GetInstance()->Stop();
        HttpRequest(g_logOut.c_str());

        g_isRunning = false;
    }
    return;
}
