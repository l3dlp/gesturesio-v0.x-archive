#include <XnCppWrapper.h>


class NIEngine
{
private:
	static NIEngine* _instance;
	xn::Context _niContext;
	xn::ScriptNode _niScriptNode;
	xn::DepthGenerator _depthGenerator;
	xn::UserGenerator _userGenerator;
	XnBool _shouldStop;

public:
	static NIEngine* GetInstance();
	XnBool Start();
	XnBool Stop();

	~NIEngine();

private:
	NIEngine();
	XnBool FileExists(const char *fn);

};