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
