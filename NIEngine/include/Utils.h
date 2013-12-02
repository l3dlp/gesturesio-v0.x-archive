#include <fstream>
#include <vector>

using namespace std;

//// Logger ////
// Saving info to log file.
class Logger
{
public:
		~Logger();
        void Log(string logLine, bool printConsole = false);
		static Logger* GetInstance();

private:
    Logger(const char* fileName);
	ofstream _stream;
	static Logger* _instance;
};
