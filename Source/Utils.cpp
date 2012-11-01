#include "Utils.h"
#include <ctime>
#include "curl/curl.h"

using namespace std;

#define  LOG_FILE_NAME "log.txt"


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


Logger* Logger::_instance = NULL;

Logger::Logger(char* fileName)
{
	_stream.open(fileName,ios::app);
}

Logger* Logger::GetInstance()
{
	if (_instance == NULL)
	{
		_instance = new Logger(LOG_FILE_NAME);
	}
	return _instance;
}

void Logger::Log(string logLine, bool printConsole)
{
	time_t now = time(0);
	tm* gmtm = gmtime(&now);
	if (gmtm != NULL)
	{
		_stream << asctime(gmtm) << logLine.c_str() << endl;
	}
	if (printConsole)
	{
		printf(logLine.c_str());
		printf("\n");
	}
}

Logger::~Logger()
{
	_stream.close();
}

JasonParsor::JasonParsor()
{

}

JasonParsor::~JasonParsor()
{

}

#define IDENT(n) for (int i = 0; i < n; ++i) printf("    ")

void JasonParsor::Print(json_value *value, int ident)
{
	IDENT(ident);
	if (value->name) printf("\"%s\" = ", value->name);
	switch(value->type)
	{
	case JSON_NULL:
		printf("null\n");
		break;
	case JSON_OBJECT:
	case JSON_ARRAY:
		printf(value->type == JSON_OBJECT ? "{\n" : "[\n");
		for (json_value *it = value->first_child; it; it = it->next_sibling)
		{
			Print(it, ident + 1);
		}
		IDENT(ident);
		printf(value->type == JSON_OBJECT ? "}\n" : "]\n");
		break;
	case JSON_STRING:
		printf("\"%s\"\n", value->string_value);
		break;
	case JSON_INT:
		printf("%d\n", value->int_value);
		break;
	case JSON_FLOAT:
		printf("%f\n", value->float_value);
		break;
	case JSON_BOOL:
		printf(value->int_value ? "true\n" : "false\n");
		break;
	}
}


bool JasonParsor::Parse(char* source)
{
	char *errorPos = 0;
	char *errorDesc = 0;
	int errorLine = 0;
	block_allocator allocator(1 << 10); // 1 KB per block

	json_value *root = json_parse(source, &errorPos, &errorDesc, &errorLine, &allocator);
	if (root)
	{
		Print(root);
		return true;
	}

	//printf("Error at line %d: %s\n%s\n\n", errorLine, errorDesc, errorPos);
	return false;
}
