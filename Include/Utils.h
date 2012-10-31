#include <fstream>
#include "vjason/json.h"

using namespace std;
class Logger
{
public:
		~Logger();
		void Log(string logLine, bool printConsole = false);
		static Logger* GetInstance();

private:
	Logger(char* fileName);
	ofstream _stream;
	static Logger* _instance;
};


// We are using vjason, a destructive parser, so the source buffer will be modified.
class JasonParsor
{
public:
	JasonParsor();
	~JasonParsor();
	bool Parse(char* source); 
	void Print(json_value *value, int ident = 0);
};