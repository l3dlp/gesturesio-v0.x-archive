#include "clsockets/PassiveSocket.h"

class SocketServer
{
private:
	CPassiveSocket _socket;
	CActiveSocket* _pClient;
	bool _shouldStop;

public:
	SocketServer();
	~SocketServer();

	void Launch();
	void Terminate();

private:
	static void StartThread(void* arg);
	void ProcessRequest();
};

typedef struct  
{
	SocketServer* _this;

}SOCKETTHREADSTRUCT;