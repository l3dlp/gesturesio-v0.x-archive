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