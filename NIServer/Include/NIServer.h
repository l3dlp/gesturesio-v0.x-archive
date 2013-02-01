#ifndef NISERVER_H
#define NISERVER_H
#include <string>
#include "NITcpServer.h"

using namespace std;

class NIServer
{
public:
    enum license_State
    {
        LICENSE_INVALID = 0,    // Invalid license
        LICENSE_VALID,          // Valid license
        LICENSE_TIMELIMITED,    // Valid license but with time limitation
        LICENSE_UNKNOWN         // Some errors might occur during the license validation
    };

public:
    static bool StartNIService();

	// We can't stop NIService immediately as data reading is running in different thread, 
	// The correct order: 
	// 1. SignalToStopNIService()    // It signals the reading thread to end.
	// 2. CanStopNIService()         // To see if reading thread is really ended.
	// 3. StopNIService()            // Reading thread ends, now we really stop NIService(release all resources).
	static void SignalToStopNIService(); 
	static bool CanStopNIService();
    static void StopNIService();

    static bool StartTcpService();
    static bool StopTcpService();
    static int GetLimitedTime();
    static license_State CheckLicense();

private:
    NIServer();
    ~NIServer();

    static string ReadLicense(char* fileName);
    static license_State ValidateLicense(string keyword);

private:
    const static int PORT = 6789;

    static int limitedTime;
    static bool isNIServing;
    static bool isTcpServing;
    static NITcpServer* pTcpServer;

    static string keyword;
    static string logIn;
    static string logOut;
    static string clientIn;
    static string clientOut;
};

#endif // NISERVER_H
