#include <fstream>
#include <vector>
#include "vjason/json.h"

using namespace std;

//// Xml Parser //////
struct XmlElement
{
    string name;
    string data;
};

// Using expat,an XML parser library written in C.
// It is a stream-oriented parser in which an application registers handlers
// for things the parser might find in the XML document (like start tags).
// more info here: http://expat.sourceforge.net/
bool ParseXml(string str, vector<XmlElement>& elements);


//// Http Request /////
// Using libcurl,an easy-to-use client-side URL transfer library
// more info here: http://curl.haxx.se/libcurl/
string HttpRequest(const char* url);


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


//// Jason Parser ////
// NOTE: We are using vjason, a destructive parser, so the source buffer will be modified.
// more info here: http://code.google.com/p/vjson/
class JasonParsor
{
public:
	JasonParsor();
	~JasonParsor();
	bool Parse(char* source); 
	void Print(json_value *value, int ident = 0);
};
