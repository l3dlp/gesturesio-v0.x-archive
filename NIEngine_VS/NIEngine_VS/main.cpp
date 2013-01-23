#include <iostream>
#include "NIEngine.h"
#include "Viewer.h"

int main(int argc, char** argv)
{
	NIEngine::GetInstance()->SetProfile(SKEL_PROFILE_HANDS_AND_HEAD);
	NIEngine::GetInstance()->Init();
	NIEngine::GetInstance()->Start();

	Viewer viewer;
	viewer.Init(argc, argv);
	viewer.Run();

	NIEngine::GetInstance()->Terminate(); 

	return 0;
}