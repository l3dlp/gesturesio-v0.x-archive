#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include "NIEngine.h"
#include "SocketServer.h"
#include "Utils.h"
#include "curl/curl.h"
#include "expat/expat.h"

using namespace std;

size_t write_to_string(void *ptr, size_t size, size_t nmemb, std::string& stream)
{
	size_t realsize = size * nmemb;
	std::string temp(static_cast<const char*>(ptr), realsize);
	stream = temp;
	return realsize;
}

string HttpRequest(const char* url)
{
	CURL *curl;
	CURLcode res;
	string response;
	
	curl_global_init(CURL_GLOBAL_DEFAULT);
	
	curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl,CURLOPT_URL,url);
		curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,0L); // skip peer verification
		curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,write_to_string);
		curl_easy_setopt(curl,CURLOPT_WRITEDATA,&response);

		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
		{
			fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
		}

		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();

	return response;
}

struct XmlElement
{
	string name;
	string data;
};

enum LICENSE_STAT
{
	INVALID = 0,
	VALID,
	TIMELIMITED,
	UNKNOWN
};

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

string logIn;
string logOut;

bool CheckLicense()
{
	LICENSE_STAT licenseStat = UNKNOWN;
	int limitTime = 0;

	Logger::GetInstance()->Log("validating your license...");
	string keyword = "";
	ifstream licenseFile;
	char buffer[100];
	bool isValid = false;

	licenseFile.open("license.txt");
	if (licenseFile.is_open())
	{
		getline(licenseFile,keyword);
		licenseFile.close();
	}

	Logger::GetInstance()->Log("keyword: " + keyword);

	if (keyword.empty() == false)
	{
		// call URL
		string url = "https://api.activedooh.com/v1/" + keyword + "/status.xml";
		logIn = "https://api.activedooh.com/v1/" + keyword + "/log/in.xml";
		logOut = "https://api.activedooh.com/v1/" + keyword + "/log/out.xml";

		string response = HttpRequest(url.c_str());

		// parse the jason response
		//int size = response.size();
		//std::vector<char> buffer(size + 1);
		//memcpy(&buffer[0],response.c_str(),size);
		//JasonParsor parsor;
		//isValid = parsor.Parse(&buffer[0]);

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
				isValid = true;  // turn it to false later when read the real value

				// parse successfully, now we need to check the value
				if (elemVector.empty() == false)
				{
					vector<XmlElement>::iterator it;
					for (it = elemVector.begin(); it != elemVector.end(); it++)
					{
						if (it->name == "status")
						{
							licenseStat = (LICENSE_STAT)atoi(it->data.c_str());
						}
						if (it->name == "time_limit" )
						{
							limitTime = atoi(it->data.c_str());
						}
					}
				}
			}
		}
	}

	// now we read the value and take some actions
	// licenseState
	// limiTime

	return isValid;
}

int main(int argc, char** argv)
{
	Logger::GetInstance()->Log("NIServer Running...");

	bool valid = CheckLicense();
	if (valid == false)
	{
		return -1;
	}
	
	HttpRequest(logIn.c_str());

	NIEngine::GetInstance()->Start();
	
	SocketServer server;
	server.Launch();

	Logger::GetInstance()->Log("Server launched...");

	Logger::GetInstance()->Log("Input command:\n t - terminate server\n x - exit\n",true);
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

	HttpRequest(logOut.c_str());
	return 1;
}