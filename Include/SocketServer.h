#include <string>
#include "clsockets/PassiveSocket.h"

using namespace std;

class SocketServer
{
private:
	CPassiveSocket _socket;
	CActiveSocket* _pClient;
	bool _shouldStop;
	string _clientIn;
	string _clientOut;

public:
	SocketServer();
	~SocketServer();

	void Launch();
	void Terminate();
	void SetClientLog(string clientIn, string clientOut);

private:
	static void StartThread(void* arg);
	void ProcessRequest();
};

typedef struct  
{
	SocketServer* _this;

}SOCKETTHREADSTRUCT;