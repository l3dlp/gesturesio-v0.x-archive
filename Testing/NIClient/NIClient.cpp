#include "clsockets/ActiveSocket.h"

int main(int argc, char** argv)
{
	CActiveSocket socket;
	char data[50];
	memset(&data,0,50);

	socket.Initialize();
	if (socket.Open((const UINT8*)"localhost",6789))
	{
		printf("Connected to the server.\n");
		if (socket.Send((const uint8*)"t",1))
		{
			socket.Receive(1);
			memcpy(&data,socket.GetData(),1);
			printf("%s\n",data);
			
			socket.Close();
		}
	}

	return 1;
}