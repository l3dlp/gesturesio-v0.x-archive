#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include "NIEngine.h"
#include "SocketServer.h"
#include "Utils.h"
#include "curl/curl.h"

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


bool CheckLicense()
{
	printf("validating your license...\n");
	string keyword;
	ifstream licenseFile;
	char buffer[100];

	licenseFile.open("license.txt");
	if (licenseFile.is_open())
	{
		getline(licenseFile,keyword);
		licenseFile.close();
	}
	printf("keyword:%s\n", keyword.c_str());

	if (keyword.empty() == false)
	{
		// call URL
		string url = "https://api.activedooh.com/v1/" + keyword + "/status.json";
		string response = HttpRequest(url.c_str());

		// parse the jason response
		memcpy(&buffer,response.c_str(),response.length());
		JasonParsor parsor;
		parsor.Parse(buffer);

		return true;
	}

	return false;
}

int main(int argc, char** argv)
{
	Logger logger("log.txt");

	logger.Log("NIServer Running...");
	//printf("NIServer Running...\n");
	bool valid = CheckLicense();
	if (valid == false)
	{
		return -1;
	}

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