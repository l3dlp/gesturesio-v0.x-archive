#ifndef NISERVER_H
#define NISERVER_H
#include <string>
#include "NITcpServer.h"

using namespace std;

//! A singleton class that provides services including NI and Tcp.
class NIServer
{
public:
	//! License state
    enum license_State
    {
        LICENSE_INVALID = 0,    //!< Invalid license
        LICENSE_VALID,          //!< Valid license
        LICENSE_TIMELIMITED,    //!< Valid license but with time limitation
        LICENSE_UNKNOWN         //!< Some errors might occur during the license validation
    };

public:
    static bool StartNIService();         //!< Start Nature Interaction service.
    static bool StopNIService();          //!< Stop Nature Interaction service.
    static bool StartTcpService();        //!< Start Tcp service.
    static bool StopTcpService();         //!< Stop Tcp service.
	static bool Exit();                   //!< Quit server.
	static bool IsNIRunning();            //!< To see if server is running.
    static int GetLimitedTime();          //!< Get limited license's valid time.
    static license_State CheckLicense();  //!< Check if license is valid.

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
