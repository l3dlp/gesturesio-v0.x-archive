#include <fstream>

using namespace std;
class Logger
{
public:
		Logger(char* fileName);
		~Logger();
		void Log(char* logLine);

private:
	ofstream m_stream;
};


// We are using vjason, a destructive parser, so the source buffer will be modified.
class JasonParsor
{
public:
	JasonParsor();
	~JasonParsor();
	bool Parse(char* source); 
};