#include "Utils.h"
#include <ctime>
#include "vjason/json.h"

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

JasonParsor::JasonParsor()
{

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
		//printf(root);
		return true;
	}

	//printf("Error at line %d: %s\n%s\n\n", errorLine, errorDesc, errorPos);
	return false;
}