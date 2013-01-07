#include <iostream>
#include "NIEngine.h"

int main(int argc, char** argv)
{
	NIEngine::GetInstance()->Init();
	NIEngine::GetInstance()->Start();

	std::cin.get();

	NIEngine::GetInstance()->Terminate(); 

	std::cin.get();

	return 0;
}