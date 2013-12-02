#include "Utils.h"
#include <ctime>
#include <stack>
#include <algorithm>

using namespace std;

//////Logger//////////////////////////////////
const string  LOG_FILE_NAME = "log.txt";
Logger* Logger::_instance = NULL;

Logger::Logger(const char* fileName)
{
	_stream.open(fileName,ios::app);
}

Logger* Logger::GetInstance()
{
	if (_instance == NULL)
	{
        _instance = new Logger(LOG_FILE_NAME.c_str());
	}
	return _instance;
}

void Logger::Log(string logLine, bool printConsole)
{
	time_t now = time(0);
	tm* gmtm = gmtime(&now);
	if (gmtm != NULL)
	{
        string str(asctime(gmtm));
        std::replace(str.begin(),str.end(),'\n','\t'); // replace newline with tab
        _stream << str.c_str() << logLine.c_str() << endl;
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
