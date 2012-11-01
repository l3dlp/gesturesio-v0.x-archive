#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
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

int Eventcnt = 0;

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
}

void end_hndl(void *data, const char *el) 
{
	printf("\n%4d: End tag %s -\n", Eventcnt++, el);
}

void char_hndl(void *data, const char *txt, int txtlen) 
{
	printf("\n%4d: Text - ", Eventcnt++);
	fwrite(txt, txtlen, sizeof(char), stdout);
}

void proc_hndl(void *data, const char *target, const char *pidata) 
{
	printf("\n%4d: Processing Instruction - ", Eventcnt++);
	printcurrent((XML_Parser) data);
}

bool CheckLicense()
{
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
			XML_SetProcessingInstructionHandler(p, proc_hndl);
			bool parseRes = XML_Parse(p, response.c_str(), response.size(), true);
			if (!parseRes)
			{
				printf("xml parsing error\n");
			}
		}
	}

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

	return 1;
}