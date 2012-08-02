#include "NIEngine.h"

int main(int argc, char** argv)
{
	NIEngine* niEngine = NIEngine::GetInstance();
	niEngine->Start();

	return 0;
}