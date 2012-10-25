#include "Utils.h"
#include <ctime>

using namespace std;

Logger::Logger(char* fileName)
{
	m_stream.open(fileName,ios::app);
}

void Logger::Log(char* logLine)
{
	time_t now = time(0);
	tm* gmtm = gmtime(&now);
	if (gmtm != NULL)
	{
		m_stream << asctime(gmtm) << logLine << endl;
	}
}

Logger::~Logger()
{
	m_stream.close();
}