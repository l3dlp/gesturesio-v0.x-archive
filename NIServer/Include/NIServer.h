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
	/// Create thread to handle Nature Interaction service.
	/// @note It's blocking, will end until the thread ends.
	/// @note Should be called before starting NI service.
	static void RunNIServiceThread();

	/// Start Nature Interaction service.
	/// @note It's non-blocking.
    static void StartNIService();       
	
	/// Stop Nature Interaction service.
	/// @note It's non-blocking.
    static void StopNIService();   

	/// End Nature Interaction service.
	/// @note It's non-blocking. The thread process will exit.
	static void EndNIService();   

    static bool StartTcpService();        //!< Start Tcp service.
    static bool StopTcpService();         //!< Stop Tcp service.  

    static int GetLimitedTime();          //!< Get limited license's valid time.
    static license_State CheckLicense();  //!< Check if license is valid.

	static bool IsNIServicing();         //!< Check if NI service is still running.

private:
    NIServer();
    ~NIServer();

    static string ReadLicense(char* fileName);
    static license_State ValidateLicense(string keyword);

	static void NIServiceStarted(bool);  // NI service started callback handler.
	static void NIServiceStopped();      // NI service stopped callback handler.
	static void NIServiceEnded();        // NI service ended callback handler.

private:
    const static int PORT = 6789;

    static int limitedTime;
	static bool isNIServiceThreadRunning;
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
